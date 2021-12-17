# [**Building** a **PEG** **Parser**](https://medium.com/@gvanrossum_83706/building-a-peg-parser-d4869b5958fb)

Inspired by only a partial understanding of **PEG** parsing I decided to build one. The result may not be a great general-purpose **PEG** **parser** generator — there are already many of those (e.g. [TatSu](https://pypi.org/project/TatSu/) is written in Python and generates Python code) — but it was a good way to learn about **PEG**, and it furthers my goal of replacing CPython’s **parser** with one built from a **PEG** grammar.

[This is part 2 of my **PEG** series. See the [Series Overview](https://medium.com/@gvanrossum_83706/peg-parsing-series-de5d41b2ed60?sk=0a7ce9003b13aae8126a4a23812eb035) for the rest.]

In this section I lay the groundwork for understanding how the generated **parser** works, by showing a simple hand-written **parser**.

(By the way, as an experiment, I’m not sprinkling links all over my writings. If there’s something you don’t understand, just Google it. :-)

The most common way of **PEG** parsing uses a recursive descent **parser** with unlimited **backtracking**. Take the toy grammar from last week’s article:

```
statement: assignment | expr | if_statement
expr: expr '+' term | expr '-' term | term
term: term '*' atom | term '/' atom | atom
atom: NAME | NUMBER | '(' expr ')'
assignment: target '=' expr
target: NAME
if_statement: 'if' expr ':' statement
```

A super-abstract recursive descent **parser** for this language would define a function for each symbol that tries to call the functions corresponding to the alternatives. For example, for `statement` we’d have this function:

```python
def statement():
    if assignment():
        return True
    if expr():
        return True
    if if_statement():
        return True
    return False
```

Of course this is too simplistic: it leaves out essential details about the **parser**’s input and output.

Let’s start with the input side. Classic parsers use a separate **tokenizer** which breaks the input (a text file or string) into a series of **tokens**, such as keywords, identifiers (names), numbers and operators. **PEG** parsers (like other modern parsers such as ANTLR) often unify tokenizing and parsing, but for my project I chose to keep the separate **tokenizer**.

Tokenizing Python is complicated enough that I don’t want to reimplement it using **PEG**’s formalism. For example, you have to keep track of **indentation** (this requires a stack inside the tokenizer), and the handling of newlines in Python is interesting (they are significant except inside matching brackets). The many types of string quotes also cause some complexity. In short, I have no beef with Python’s existing tokenizer, so I want to keep it. (Aside: CPython has two tokenizers —an internal one used by the **parser**, written in C, and the standard library one, which is a faithful reimplementation in pure Python. This is helpful for my project.)

Classic tokenizers typically have a simple interface whereby you call a function, e.g. `get_token()`, which returns the next token in the input, consuming the input a few characters at a time. The `tokenize` module simplifies this even further: its basic API is a **generator** which yields one token at a time. Each token is a `TypeInfo` object which has several fields, the most important ones of which indicate the *type* of the token (e.g. `NAME`, `NUMBER`, `STRING`), and its *string* value, meaning the string of characters comprising the token (e.g. `abc`, `42`, or `"hello world"`). There are also additional fields that give the **coordinates** of the token in the input file, which is useful for error reporting.

A special token type is`ENDMARKER`, which indicates that the end of the input file has been reached. The generator terminates if you ignore this and try to get the next token.

But I digress. How do we implement **unlimited backtracking**? Backtracking requires you to be able to remember a position in the source code and re-parse from that point. The tokenizer API doesn’t allow us to reset its input pointer, but it’s easy to capture the stream of tokens in an array and replay it from there, so that’s what we do. (You could also do this using `itertools.tee()`, but based on warnings in the docs that’s probably less efficient in our case.)

I suppose you could just first tokenize the entire input into a Python list and then use that as the **parser** input, but that would mean if there’s an invalid token near the end of the file (such as a string with a missing closing quote) and there’s also a syntax error earlier in the file, you would get an error message about the bad token first. I would find that a poor user experience, since the syntax error could actually be the root cause for the bad string. So my design tokenizes on demand, and the list becomes a lazy list.

The basic API is very simple. The `Tokenizer` object encapsulates the array of tokens and the position in that array. It has three basic methods:

- `get_token()` returns the next token, advancing the position in the array (reading another token from the source if we’re at the end of the array);
- `mark()` returns the current **position** in the array;
- `reset(pos)` sets the position in the array (the argument *must* be something you got from `mark()`).

We add one convenience function, `peek_token()` which returns the next token without advancing the position.

Here, then, is the core of the `Tokenizer` class:

```python
class Tokenizer:
    def __init__(self, tokengen):
        """Call with tokenize.generate_tokens(...)."""
        self.tokengen = tokengen
        self.tokens = []
        self.pos = 0
    def mark(self):
        return self.pos
    def reset(self, pos):
        self.pos = pos
    def get_token(self):
        token = self.peek_token()
        self.pos += 1
        return token
    def peek_token(self):
        if self.pos == len(self.tokens):
            self.tokens.append(next(self.tokengen))
        return self.tokens[self.pos]
```

Now, there are various things still missing (and the names of the methods and instance variables should really start with an underscore), but this will do as a sketch of the `Tokenizer` API.

The **parser** also needs to become a class, so that `statement()`, `expr()` and so on can become methods. The tokenizer becomes an instance variable, but we don’t want the parsing methods to call `get_token()` directly — instead, we give the `**Parser**` class an `expect()` method which can succeed or fail just like a parsing method. The argument to `expect()` is the expected token — either a string (like `"+"`) or a token type (like `NAME`). I’ll get to the return type after discussing the **parser**’s output.

In my first sketch of the **parser**, the parsing functions just returned `True` or `False`. That’s fine for theoretical computer science (where the question a **parser** answers is “is this a valid string in the language?”) but not when you’re **building** a **parser** — instead, we want the **parser** to create an AST. So let’s just arrange it so that each parsing method returns a `Node` object on success, or `None` on failure.

The `Node` class can be super simple:

```python
class Node:
    def __init__(self, type, children):
        self.type = type
        self.children = children
```

Here, `type` indicates what kind of **AST node** this is (e.g. an `"add"` node or an `"if"` node), and `children` is a list of nodes and tokens (instances of `TokenInfo`). This is enough for a compiler to generate code or do other analysis such as linting or static type checking, although in the future I’d like to change the way we represent the AST.

To fit into this scheme, the `expect()` method returns a `TokenInfo` object on success, and `None` on failure. To support backtracking, I wrap the tokenizer’s `mark()` and `reset()` methods (no API change here). Here then is the infrastructure for the `**Parser**` class:

```python
class Parser:
    def __init__(self, tokenizer):
        self.tokenizer = tokenizer
    def mark(self):
        return self.tokenizer.mark()
    def reset(self, pos):
        self.tokenizer.reset(pos)
    def expect(self, arg):
        token = self.tokenizer.peek_token()
        if token.type == arg or token.string == arg:
            return self.tokenizer.get_token()
        return None
```

Again, I’ve left out some details, but this works.

At this point I need to introduce an important requirement for parsing methods: a parsing method either returns a `Node`, positioning the tokenizer after the last token of the grammar rule it recognized; or it returns `None`, *and then it leaves the tokenizer position unchanged*. If a parsing method reads several tokens and then decides to fail, it *must* restore the tokenizer’s position. That’s what `mark()` and `reset()` are for. Note that `expect()` also follows this rule.

So here’s a sketch of the actual **parser**. Note that I am using Python 3.8’s walrus operator (`:=`):

```python
class ToyParser(Parser):
    def statement(self):
        if a := self.assignment():
            return a
        if e := self.expr():
            return e
        if i := self.if_statement():
            return i
        return None
    def expr(self):
        if t := self.term():
            pos = self.mark()
            if op := self.expect("+"):
                if e := self.expr():
                    return Node("add", [t, e])
            self.reset(pos)
            if op := self.expect("-"):
                if e := self.expr():
                    return Node("sub", [t, e])
            self.reset(pos)
            return t
        return None
    def term(self):
        # Very similar...
    def atom(self):
        if token := self.expect(NAME):
            return token
        if token := self.expect(NUMBER):
            return token
        pos = self.mark()
        if self.expect("("):
            if e := self.expr():
                if self.expect(")"):
                    return e
        self.reset(pos)
        return None
```

I’ve left some parsing methods as exercises for the reader — this is really more to give a flavor of what such a **parser** looks like, and eventually we’ll generate code like this automatically from the grammar. Constants like `NAME `and `NUMBER` are imported from the `token` module in the standard library. (This ties us further to Python tokenization; there are ways around this that we should explore if we want to make a more general **PEG** **parser** generator.)

Also note that I cheated a bit: `expr` is left-recursive, but I made the **parser** *right-*recursive, because recursive-descent parsers don’t work with left-recursive grammar rules. There’s a fix for this, but it’s still the topic of some academic research and I’d like to present it separately. Just realize that this version doesn’t correspond 100% with the toy grammar.

The key things I want you to get at this point are:

- Grammar rules correspond to **parser** methods, and when a grammar rule references another grammar rule, its parsing method calls the other rule’s parsing method.
- When multiple items make up an alternative, the parsing method calls the corresponding methods one after the other.
- When a grammar rule references a token, its parsing method calls `expect()`.
- When a parsing method successfully recognizes its grammar rule at the given input position, it returns a corresponding AST node; when it fails to recognize its grammar rule, it returns `None`.
- Parsing methods must explicitly reset the tokenizer position when they abandon a parse after having consumed one or more tokens (directly, or indirectly by calling another parsing method that succeeded). This applies when abandoning one alternative to try the next, and also when abandoning the parse altogether.

If all parsing methods abide by these rules, it’s not necessary to use `mark()` and `reset()` around a single parsing method. You can prove this using induction.

As an aside, it’s tempting to try to get rid of the explicit `mark()` and `reset()` calls by using a context manager and a `with` statement, but this doesn’t work: the `reset()` call shouldn’t be called upon success! As a further fix you could try to use exceptions for control flow, so the context manager knows whether to reset the tokenizer (I think TatSu does something like this). For example, you could arrange for this to work:

```
    def statement(self):
        with self.alt():
            return self.assignment()
        with self.alt():
            return self.expr()
        with self.alt():
            return self.if_statement()
        raise ParsingFailure
```

In particular, the little ladder of `if` statements in `atom()` for recognizing a parenthesized expression could become:

```python
        with self.alt():
            self.expect("(")
            e = self.expr()
            self.expect(")")
            return e
```

But I find this too “magical” — when reading such code you must stay aware that each parsing method (and `expect()`) may raise an exception, and that this exception is caught and ignored by the context manager in the `with` statement. That’s pretty unusual, although definitely supported (by returning true from `__exit__`). Also, my ultimate goal is to generate C, not Python, and in C there’s no `with` statement to alter the control flow.

Anyway, here are some topics for future installments:

- generating parsing code from the grammar;
- packrat parsing (memoization);
- EBNF features like `(x | y)`, `[x y ...]`, `x*`, `x+`;
- tracing (for debugging the **parser** or grammar);
- **PEG** features like lookahead and “cut”;
- how to handling left recursive rules;
- generating C code.

License for this article and the code shown: [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)