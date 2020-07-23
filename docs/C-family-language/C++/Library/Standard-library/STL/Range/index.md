# Range

## Range and iterator

在维基百科[Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library)中提及：

> The [concept](https://en.wikipedia.org/wiki/Concept_(generic_programming)) of iterators as implemented by STL can be difficult to understand at first: for example, if a value pointed to by the iterator is deleted, the iterator itself is then no longer valid. This is a common source of errors. Most implementations of the STL provide a debug mode that is slower, but can locate such errors if used. A similar problem exists in other languages, for example [Java](https://en.wikipedia.org/wiki/Java_(programming_language)). [Ranges](https://en.wikipedia.org/wiki/Range_(computer_science)#Range_as_an_alternative_to_iterator) have been proposed as a safer, more flexible alternative to iterators.

显然，基于range的generic programming有着一些优势，本章对range的一些内容进行梳理。

### wikipedia [Range as an alternative to iterator](https://en.wikipedia.org/wiki/Range_(computer_programming)#Range_as_an_alternative_to_iterator)



### Range is an abstraction as iterator is

和iterator类似，range也是一种抽象，它是**behavior-defined**（参见`Theory\Programming-paradigm\Generic-programming\Templates-and-Duck-Typing\Templates-and-Duck-Typing.md`），只要支持`begin`和`end`、iteration操作，那么就可以作为range；

### Range is an enhancement to iterator

range是对iterator的增强，两者都旨在generic programming。

## cppreference [Ranges library (C++20)](https://en.cppreference.com/w/cpp/ranges)



## boost [Boost range](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/index.html)





## Andrei Alexandrescu [Iterators Must Go](https://accu.org/content/conf2009/AndreiAlexandrescu_iterators-must-go.pdf)



## 总结

除了本章，在以下章节中，也描述了range相关内容:

- `C++\Library\Standard-library\STL\Idiom\Range-based-idiom`
- `C++\Language-reference\Statements\Iteration-statements\For\For.md`

