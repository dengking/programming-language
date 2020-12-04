# Interpreter pattern

和它相关的pattern:

1 [Composite pattern](https://en.wikipedia.org/wiki/Composite_pattern) 

wikipedia [Interpreter pattern](https://en.wikipedia.org/wiki/Interpreter_pattern):

> The [syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) of a sentence in the language is an instance of the [composite pattern](https://en.wikipedia.org/wiki/Composite_pattern) and is used to evaluate (interpret) the sentence for a client.[[1\]](https://en.wikipedia.org/wiki/Interpreter_pattern#cite_note-GoF-1):243 See also [Composite pattern](https://en.wikipedia.org/wiki/Composite_pattern).

因为formal language是典型的符合 containing关系的，因此它是可以可以使用tree structure的；

2 [Iterator pattern](https://en.wikipedia.org/wiki/Iterator_pattern)

其实它更多的是和 [composite pattern](https://en.wikipedia.org/wiki/Composite_pattern) 相关，通过[Iterator pattern](https://en.wikipedia.org/wiki/Iterator_pattern)，可以为 tree structure添加多种algorithm/behavior。

3 [Command pattern](https://en.wikipedia.org/wiki/Command_pattern)

三地址码，其实非常类似于一个command；

## wikipedia [Interpreter pattern](https://en.wikipedia.org/wiki/Interpreter_pattern)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), the **interpreter pattern** is a [design pattern](https://en.wikipedia.org/wiki/Design_pattern_(computer_science)) that specifies how to evaluate sentences in a language. The basic idea is to have a [class](https://en.wikipedia.org/wiki/Class_(computer_science)) for each symbol ([terminal](https://en.wikipedia.org/wiki/Terminal_symbol) or [nonterminal](https://en.wikipedia.org/wiki/Nonterminal_symbol)) in a [specialized computer language](https://en.wikipedia.org/wiki/Domain_specific_languages). The [syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) of a sentence in the language is an instance of the [composite pattern](https://en.wikipedia.org/wiki/Composite_pattern) and is used to evaluate (interpret) the sentence for a client.[[1\]](https://en.wikipedia.org/wiki/Interpreter_pattern#cite_note-GoF-1):243 See also [Composite pattern](https://en.wikipedia.org/wiki/Composite_pattern).