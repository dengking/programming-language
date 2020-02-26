[TOC]



# 需要掌握的理论知识

我认为， 要想理解cpython的`pgen`，你需要掌握一定的compiler的front end的原理，以dragon book来说的话，你至少要掌握了如下章节的内容：

- Chapter 3 Lexical Analysis
  - 3.6 Finite Automata
  - 3.7 From Regular Expressions to Automata
- Chapter 4 Syntax Analysis
  - 4.2 Context-Free Grammars
  - 4.4 Top-Down Parsing



# 综述

Full Grammar specification of python is [here](https://docs.python.org/3/reference/grammar.html)，它被[Guido van Rossum](https://en.wikipedia.org/wiki/Guido_van_Rossum)称为EBNF-like grammar . EBNF-like grammar所扩展的其实就是对regular expression中的一些标记的支持，所以它的语法看起来和正则表达式比较类似，下面会进行说明. Full Grammar specification is a file, which is human-readable, the task of `pgen` is reading the file, parsing it and constructing predictive parsing table. 下面将对整个`pgen`的实现原理进行概括性的描述，然后分章节对具体的实现细节进行描述。

EBNF-like grammar的每条语法是以[production](https://en.wikipedia.org/wiki/Production_(computer_science))的方式来描述的，对于我们而言，首先需要能够理解这些grammar，如下是grammar中中一些特殊符号的含义：

- `[]`表示可选，optional
- `|`表示或，or
- `+`表示大于等于1，more than 1
- `*`表示任意多个，any

在将grammar file读入内存后，`pgen`使用`tokenize`模块的`generate_tokens`来将grammar file中的内容转换为stream of token，所以后续对grammar file的解析就是基于这个stream of token的。正如前面所说的，python的EBNF-like grammar非常类似于regular expression，也就是说我们使用regular expression来描述python的语法，也就是说regular expression是python语法的meta-grammar。为什么要强调呢？因为`pgen`要想实现parsing grammar的任务，就必须知道meta-grammar。现在我们已经知道了meta-grammar是regular expression，所以`pgen`就可以按照regular expression的语法来进行解析。通常，可以根据regular expression构造出对应的DFA，`pgen`中就是采取的这种方式，程序内部使用DFA来表示production的body（regular expression）。根据regular expression来构建DFA，所采用的是如下算法：

| Name                                                         | Function                                                     | Chapter in dragon book                                 |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------ |
| [Thompson's construction algorithm](https://en.wikipedia.org/wiki/Thompson's_construction) | transforming a [regular expression](https://en.wikipedia.org/wiki/Regular_expression) into an equivalent [nondeterministic finite automaton](https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton) (NFA) | 3.7.4 Construction of an NFA from a Regular Expression |
| [subset construction algorithm](https://en.wikipedia.org/wiki/Subset_construction_algorithm) | [converting](https://en.wikipedia.org/wiki/Automata_construction) a [nondeterministic finite automaton](https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton) (NFA) into a [deterministic finite automaton](https://en.wikipedia.org/wiki/Deterministic_finite_automaton) (DFA) which recognizes the same [formal language](https://en.wikipedia.org/wiki/Formal_language) | 3.7.1 Conversion of an NFA to a DFA                    |
| [DFA minimization](https://en.wikipedia.org/wiki/DFA_minimization) | transforming a given [deterministic finite automaton](https://en.wikipedia.org/wiki/Deterministic_finite_automaton) (DFA) into an equivalent DFA that has a minimum number of states. |                                                        |



在解析完grammar后，`pgen`就得到了使用`DFA`表示的语法了（每条语法对应一个`DFA`），接着`pgen`就根据这些`DFA`来构造predictive parsing table，所采用的是Algorithm 4.31 described in chapter 4.4.3 LL(1) Grammars of dragon book ，在此不对此算法进行赘述。



# 实现细节

## `metaparser.py`

实现对grammar的parse。

### `class GrammarParser`

#### `parse`

使用[Thompson's construction algorithm](https://en.wikipedia.org/wiki/Thompson%27s_construction)来将每个产生式（一条grammar）的body转换为一个DFA(在`automata.py`中定义，参见下一节)，成员变量`_current_rule_name`对应的是产生式的头。在grammar文件中定义了多条grammar，所以`parse`方法每次处理一条grammar，并`yield`该grammar对应的DFA对象，也就是说，`parse`方法是一个generator。

`parse`采用[stream](https://en.wikipedia.org/wiki/Stream_(computing))处理的方式：一次处理一个token。

`_parse_rhs`中`rhs`的含义是right hand side的含义，对于一个production而言，它的right hand side就是它的body。

EBNF-like grammar中包含有如下括号：

- `[]`
- `()`

显然，`GrammarParser`的需要处理的一个问题就是括号的匹配，本程序中，实现括号的匹配所使用的是隐式的[calling stack](https://en.wikipedia.org/wiki/Call_stack)，而不是使用一个显式的堆栈。

几个辅助函数如下：

`_gettoken`获取下一个token信息，并将它保存在成员变量`self.type, self.value, self.begin, self.end, self.line`中

`_expect`比较当前成员变量与入参值，然后调用`_gettoken`



##### 描述python grammar的grammar: meta-grammar

正如在[综述](#综述)章节中所描述的，python grammar的meta-grammar是regular expression，所以对grammar的解析是按照解析regular expression的方式来实现的，具体来说，就是使用的用[Thompson's construction algorithm](https://en.wikipedia.org/wiki/Thompson%27s_construction)算法。

`class GrammarParser`对[Thompson's construction algorithm](https://en.wikipedia.org/wiki/Thompson%27s_construction)算法的使用是非常清晰的，它给出了语法的通用格式如下：

`rule->rhs->items->item->atom` 

下面使用类似于正则表达式的方式对上述结构中的每一层进行解释：

- `rule: NAME ':' rhs NEWLINE` 

- `rhs: items ('|' items)*`

- `items: item+`

- `item: '[' rhs ']' | atom ['+' | '*']`

- `atom: '(' rhs ')' | NAME | STRING`

可以看出，上诉语法描述出了python grammar的结构，`class GrammarParser`就是沿着上诉hierarchy进行逐层解析的，下面的表格是根据上述hierarchy而画出的，每一列分别对应一层，在第三行添加了解析该层的对应的函数。

|      | rule                         | `rhs`                     | items          | item                                   | atom                |
| ---- | ---------------------------- | ------------------------- | -------------- | -------------------------------------- | ------------------- |
| 语法 | `rule: NAME ':' rhs NEWLINE` | `rhs: items ('|' items)*` | `items: item+` | `item: '[' rhs ']' | atom ['+' | '*']` | `atom: '(' rhs ')'` |
| 函数 | `parse`                      | `_parse_rhs`              | `_parse_items` | `_parse_item`                          | `_parse_atom`       |

从上述表格可以看出，`class GrammarParser`的代码结构是非常清晰的，它的实现也是简洁易懂的。



## `automata.py`

The implementation of converting the body of the production to a DFA is in `automata.py`

automata也是图，如何来表示node（state）和edge（transition），下面是automata和graph之间的对应关系

| graph | automata   |
| ----- | ---------- |
| node  | state      |
| edge  | transition |

在`automata.py`中定义了两种automata：

- NFA
- DFA

### NFA

`class NFAArc`

表示图的边

`class NFAState`

表示图的节点

`class NFA`

一个NFA其实是一张有向图，每个NFA有一个起始状态和一个终止状态，分别对应了成员变量`start`和`end`。

### DFA

`class DFA`

类方法`from_nfa`是一个工厂方法，它使用[subset construction algorithm](https://en.wikipedia.org/wiki/Subset_construction_algorithm)，根据一个NFA构造一个DFA。

`class DFAState`



