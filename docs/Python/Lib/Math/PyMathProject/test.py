#!/usr/bin/env python3
"""
单链表文法解析器（递归下降）

文法（右递归，LL(1)）:
    list  → value '->' list
          | 'nil'
    value → NUMBER | IDENT | STRING

用法:
    python linked_list_parser.py "1 -> 2 -> 3 -> nil"
    python linked_list_parser.py --dot "a -> b -> nil" > tree.dot
    python linked_list_parser.py            # 进入交互模式
"""

import argparse
import ast
import re
import sys
from dataclasses import dataclass, field
from typing import List, Optional


# ============================================================
# 1. 词法分析
# ============================================================

@dataclass
class Token:
    kind: str  # NUMBER / IDENT / STRING / ARROW / NIL / EOF
    text: str
    pos: int  # 在源字符串中的偏移，用于报错


# 顺序很重要：ARROW 必须在 NUMBER 之前，否则 "->" 中的 "-" 可能被误认
TOKEN_SPEC = [
    ("WS", r"\s+"),  # r前缀表示原始字符串，不进行转义
    ("ARROW", r"->"),
    ("NUMBER", r"-?\d+(?:\.\d+)?"),  # ?: 表示: 非捕获分组（non-capturing group）
    # 该regex的结构为: (?:A|B)*，它表示普通字符或转义序列,重复零次或多次
    # [^"\\]: 任意不是双引号、也不是反斜杠的字符(普通字符)
    # \\.	: 一个反斜杠加任意一个字符,即转义序列,如 \"、\n、\\
    ("STRING", r'"(?:[^"\\]|\\.)*"'),
    ("IDENT", r"[A-Za-z_][A-Za-z0-9_]*"),  # identifier
]
"""
MASTER_RE的值为: (?P<WS>\s+)|(?P<ARROW>->)|(?P<NUMBER>-?\d+(?:\.\d+)?)|(?P<STRING>"(?:[^"\\]|\\.)*")|(?P<IDENT>[A-Za-z_][A-Za-z0-9_]*)
- 整个表达式由 5 个命名分组用 |(或)连接
- (?P<名字>模式) 是 Python 的命名捕获组语法,匹配后可通过 m.lastgroup 知道是哪一组匹配成功,即 token 的类型。
- | 从左到右依次尝试,第一个匹配成功的分支胜出,所以顺序很重要。
"""
MASTER_RE = re.compile("|".join(f"(?P<{name}>{pat})" for name, pat in TOKEN_SPEC))

KEYWORDS = {"nil": "NIL"}
VALUE_KINDS = {"NUMBER", "IDENT", "STRING"}  # 链表的节点的值类型
FIXED_KINDS = {"ARROW", "NIL", "EOF"}  # 文本固定的 token，报错时无需重复显示内容

TOKEN_NAMES = {
    "ARROW": "'->'",
    "NIL": "'nil'",
    "NUMBER": "数字",
    "IDENT": "标识符",
    "STRING": "字符串",
    "EOF": "输入结束",
}


class ParseError(Exception):
    def __init__(self, message: str, pos: int, source: str):
        super().__init__(message)
        self.message = message
        self.pos = pos
        self.source = source

    def __str__(self) -> str:
        line_start = self.source.rfind("\n", 0, self.pos) + 1
        line_end = self.source.find("\n", self.pos)
        if line_end == -1:
            line_end = len(self.source)
        line = self.source[line_start:line_end]
        lineno = self.source.count("\n", 0, self.pos) + 1
        col = self.pos - line_start
        return (f"语法错误（第 {lineno} 行，第 {col + 1} 列）：{self.message}\n"
                f"  {line}\n"
                f"  {' ' * col}^")


def tokenize(source: str) -> List[Token]:
    tokens: List[Token] = []
    pos = 0
    while pos < len(source):
        m = MASTER_RE.match(source, pos)
        if not m:
            raise ParseError(f"非法字符 {source[pos]!r}", pos, source)
        kind, text = m.lastgroup, m.group()
        if kind == "IDENT":
            kind = KEYWORDS.get(text, "IDENT")
        if kind != "WS":
            tokens.append(Token(kind, text, pos))
        pos = m.end()
    tokens.append(Token("EOF", "", len(source)))
    return tokens


def describe(tok: Token) -> str:
    name = TOKEN_NAMES.get(tok.kind, tok.kind)
    return name if tok.kind in FIXED_KINDS else f"{name} {tok.text!r}"


# ============================================================
# 2. 解析树（具体语法树）
# ============================================================

@dataclass
class PTNode:
    label: str  # 非终结符名，或终结符类别
    children: List["PTNode"] = field(default_factory=list)
    token: Optional[Token] = None  # 只有终结符（叶子）才有

    @property
    def is_terminal(self) -> bool:
        return self.token is not None


# ============================================================
# 3. 递归下降解析器：每个非终结符对应一个方法
# ============================================================

class Parser:
    def __init__(self, source: str):
        self.source = source
        self.tokens = tokenize(source)
        self.i = 0  # 当前token的索引

    # ---------- 辅助 ----------
    def peek(self) -> Token:
        return self.tokens[self.i]

    def error(self, message: str):
        raise ParseError(message, self.peek().pos, self.source)

    def expect(self, kind: str) -> PTNode:
        tok = self.peek()
        if tok.kind != kind:
            self.error(f"期望 {TOKEN_NAMES[kind]}，实际得到 {describe(tok)}")
        self.i += 1
        return PTNode(kind, token=tok)

    # ---------- 入口 ----------
    def parse(self) -> PTNode:
        tree = self.parse_list()
        if self.peek().kind != "EOF":
            self.error(f"'nil' 之后不应再有内容，实际得到 {describe(self.peek())}")
        return tree

    # list → value '->' list | 'nil'
    def parse_list(self) -> PTNode:
        tok = self.peek()
        if tok.kind == "NIL":
            return PTNode("list", [self.expect("NIL")])
        if tok.kind in VALUE_KINDS:
            value = self.parse_value()
            arrow = self.expect("ARROW")
            rest = self.parse_list()  # 右递归
            return PTNode("list", [value, arrow, rest])
        self.error(f"期望一个值或 'nil'，实际得到 {describe(tok)}")

    # value → NUMBER | IDENT | STRING
    def parse_value(self) -> PTNode:
        tok = self.peek()
        if tok.kind in VALUE_KINDS:
            return PTNode("value", [self.expect(tok.kind)])
        self.error(f"期望一个值，实际得到 {describe(tok)}")


# ============================================================
# 4. 解析树的输出
# ============================================================

def node_label(node: PTNode) -> str:
    if node.is_terminal:
        return f"{node.label} {node.token.text!r}"
    return node.label


def format_tree(root: PTNode) -> str:
    """树状图形式"""
    lines = [node_label(root)]

    def walk(node: PTNode, prefix: str):
        for idx, child in enumerate(node.children):
            last = idx == len(node.children) - 1
            lines.append(prefix + ("└── " if last else "├── ") + node_label(child))
            walk(child, prefix + ("    " if last else "│   "))

    walk(root, "")
    return "\n".join(lines)


def format_brackets(node: PTNode) -> str:
    """括号形式，如 [list [value 1] -> [list nil]]"""
    if node.is_terminal:
        return node.token.text
    inner = " ".join(format_brackets(c) for c in node.children)
    return f"[{node.label} {inner}]"


def to_dot(root: PTNode) -> str:
    """Graphviz DOT 格式，可用 `dot -Tpng tree.dot -o tree.png` 渲染"""
    lines = ["digraph ParseTree {",
             '  node [fontname="Helvetica"];']
    counter = 0

    def esc(s: str) -> str:
        return s.replace("\\", "\\\\").replace('"', '\\"')

    def visit(node: PTNode) -> str:
        nonlocal counter
        my_id = f"n{counter}"
        counter += 1
        if node.is_terminal:
            lines.append(f'  {my_id} [label="{esc(node.token.text)}", shape=box, '
                         f'style=filled, fillcolor="#e8f0fe"];')
        else:
            lines.append(f'  {my_id} [label="{node.label}", shape=ellipse];')
        for child in node.children:
            lines.append(f"  {my_id} -> {visit(child)};")
        return my_id

    visit(root)
    lines.append("}")
    return "\n".join(lines)


# ============================================================
# 5. 由解析树构造真实的单链表
# ============================================================

@dataclass
class Node:
    value: object
    next: Optional["Node"] = None


def convert_value(value_node: PTNode) -> object:
    tok = value_node.children[0].token
    if tok.kind == "NUMBER":
        return float(tok.text) if "." in tok.text else int(tok.text)
    if tok.kind == "STRING":
        return ast.literal_eval(tok.text)
    return tok.text


def build_linked_list(tree: PTNode) -> Optional[Node]:
    """沿着右递归的 list 节点向下走，依次接到链表尾部"""
    head: Optional[Node] = None
    tail: Optional[Node] = None
    node = tree
    while len(node.children) == 3:  # list → value '->' list
        value_node, _arrow, node = node.children
        new = Node(convert_value(value_node))
        if tail is None:
            head = new
        else:
            tail.next = new
        tail = new
    return head  # 到达 list → 'nil'


def format_linked_list(head: Optional[Node]) -> str:
    parts = []
    cur = head
    while cur is not None:
        parts.append(repr(cur.value))
        cur = cur.next
    parts.append("None")
    return " -> ".join(parts)


def format_cons(head: Optional[Node]) -> str:
    values = []
    cur = head
    while cur is not None:
        values.append(repr(cur.value))
        cur = cur.next
    return "".join(f"Cons({v}, " for v in values) + "Nil" + ")" * len(values)


# ============================================================
# 6. 命令行入口
# ============================================================

def run(source: str, dot: bool = False) -> int:
    try:
        tree = Parser(source).parse()
    except ParseError as e:
        print(e, file=sys.stderr)
        return 1

    if dot:
        print(to_dot(tree))
        return 0

    head = build_linked_list(tree)
    print("== 解析树 ==")
    print(format_tree(tree))
    print("\n== 括号形式 ==")
    print(format_brackets(tree))
    print("\n== Cons 形式 ==")
    print(format_cons(head))
    print("\n== 构造的单链表 ==")
    print(format_linked_list(head))
    return 0


def repl():
    print("输入单链表，例如 1 -> 2 -> 3 -> nil（空行或 Ctrl-D 退出）")
    while True:
        try:
            line = input("list> ")
        except EOFError:
            print()
            break
        if not line.strip():
            break
        run(line)
        print()


def main():
    ap = argparse.ArgumentParser(description="单链表文法解析器")
    ap.add_argument("source", nargs="?", help="要解析的单链表文本")
    ap.add_argument("--dot", action="store_true", help="以 Graphviz DOT 格式输出解析树")
    args = ap.parse_args()

    if args.source is None:
        repl()
    else:
        sys.exit(run(args.source, dot=args.dot))


if __name__ == "__main__":
    main()
