# Ranges library (C++20)

首先搞清楚C++20为什么引入range library。

## What's new in C++

在C++20之前，C++中也是由range的概念的: `"begin, end"`，后面为了区分，使用`"begin, end"`来表示；C++20前的algorithm都是基于`"begin, end"`的；

C++20其，C++引入了ranges library，显然它是显式对对range的定义 ，它的引入重要是为了解决iterator的问题。

### wikipedia [Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library)

> The [concept](https://en.wikipedia.org/wiki/Concept_(generic_programming)) of iterators as implemented by STL can be difficult to understand at first: for example, if a value pointed to by the iterator is deleted, the iterator itself is then no longer valid. This is a common source of errors. Most implementations of the STL provide a debug mode that is slower, but can locate such errors if used. A similar problem exists in other languages, for example [Java](https://en.wikipedia.org/wiki/Java_(programming_language)). 
>
> [Ranges](https://en.wikipedia.org/wiki/Range_(computer_science)#Range_as_an_alternative_to_iterator) have been proposed as a safer, more flexible alternative to iterators.

显然，基于range的generic programming有着一些优势，本章对range的一些内容进行梳理。

### wikipedia [Range as an alternative to iterator](https://en.wikipedia.org/wiki/Range_(computer_programming)#Range_as_an_alternative_to_iterator)

Another meaning of *range* in computer science is an alternative to [iterator](https://en.wikipedia.org/wiki/Iterator). When used in this sense, range is defined as "a pair of begin/end iterators packed together".[[1\]](https://en.wikipedia.org/wiki/Range_(computer_programming)#cite_note-itersmustgo-1) It is argued [[1\]](https://en.wikipedia.org/wiki/Range_(computer_programming)#cite_note-itersmustgo-1) that "Ranges are a superior abstraction" (compared to iterators) for several reasons, including better safety.

In particular, such ranges are supported in [Boost C++ Libraries](https://en.wikipedia.org/wiki/Boost_C%2B%2B_Libraries)[[2\]](https://en.wikipedia.org/wiki/Range_(computer_programming)#cite_note-2) and the [D](https://en.wikipedia.org/wiki/D_(programming_language)) standard library.[[3\]](https://en.wikipedia.org/wiki/Range_(computer_programming)#cite_note-3)



### Andrei Alexandrescu [Iterators Must Go](https://accu.org/content/conf2009/AndreiAlexandrescu_iterators-must-go.pdf)





### Range is an abstraction as iterator be

和iterator类似，range也是一种抽象，它是**behavior-defined**（参见`Theory\Programming-paradigm\Generic-programming\Templates-and-Duck-Typing\Templates-and-Duck-Typing.md`），只要支持`begin`和`end`、iteration操作，那么就可以作为range，这就包括了c++的各种容器，正如cppreference [Range-based for loop](https://en.cppreference.com/w/cpp/language/range-for)中所述：

> Used as a more readable equivalent to the traditional [for loop](https://en.cppreference.com/w/cpp/language/for) operating over a range of values, such as all elements in a container.



### Range is an enhancement to iterator

range是对iterator的增强，两者都旨在generic programming。



## cppreference [Ranges library (C++20)](https://en.cppreference.com/w/cpp/ranges)



## boost [Boost range](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/index.html)





## cppreference [Ranges library (C++20)](https://en.cppreference.com/w/cpp/ranges)

The ranges library provides components for dealing with ranges of elements, including a variety of view adapters.


