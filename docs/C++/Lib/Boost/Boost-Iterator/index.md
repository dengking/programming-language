# 关于本章

本章描述boost iterator库的设计与实现，boost iterator库和boost的很多其他的library一样，所采用的是[generic programming](https://www.boost.org/community/generic_programming.html)，我觉得理解boost iterator library的设计思想的最佳途径是按照[The Boost.Iterator Library](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/index.html) 中给出的文档的顺序来进行阅读，即首先阅读《[Standard Proposal For New-Style Iterators](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html) ([PDF](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.pdf))》，boost iterator library就是按照其中描述的concept来进行实现的，并且在《[`iterator_facade`](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/iterator_facade.html) ([PDF](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/iterator_facade.pdf))》、《[`iterator_adaptor`](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/iterator_adaptor.html) ([PDF](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/iterator_adaptor.pdf))》中，会引用很多《[Standard Proposal For New-Style Iterators](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html) ([PDF](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.pdf))》中描述的概念，然后依次阅读《[`iterator_facade`](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/iterator_facade.html) ([PDF](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/iterator_facade.pdf))》、《[`iterator_adaptor`](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/iterator_adaptor.html) ([PDF](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/iterator_adaptor.pdf))》。

另外，按照在文章`Theory\Programming-paradigm\Generic-programming\Templates-and-Duck-Typing\Templates-and-Duck-Typing.md`中，总结的思想来理解boost library的设计与实现。