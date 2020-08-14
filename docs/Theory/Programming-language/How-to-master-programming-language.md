# How to master programming language

你是否思考过：how many programming languages are there？Google了一下，第一个回答是[there are about **700 programming languages**](https://careerkarma.com/blog/how-many-coding-languages-are-there/)，如此 之多。维基百科的[List of programming languages](https://en.wikipedia.org/wiki/List_of_programming_languages)中，收录了大部分programming language，从[Timeline of programming languages](https://en.wikipedia.org/wiki/Timeline_of_programming_languages)中，我们大致可以看到 programming language的变迁与发展。面对如此之多、不断发展、层出不穷的programming language，我不禁思考：how to master programming language？本文就对这个问题来进行探讨。



## 从语言的设计者的角度出发来学习一门语言

让我产生这种想法的是在对c和`c++`中都有的[Storage duration和Linkage](https://en.cppreference.com/w/c/language/storage_duration)概念的思考：

在c和`c++`中都有[Storage duration和Linkage](https://en.cppreference.com/w/c/language/storage_duration)的概念，在对这两个概念的理解过程中，我发现这两个概念是和program的编译、运行有着非常密切的关联；后来想想，这其实是必然的，c++和c是[Compiled language](https://en.wikipedia.org/wiki/Compiled_language)，如果我们站在c和c++语言的设计者的角度来思考，他们在设计这两门语言的时候，肯定也需要考虑程序在编译过程、链接过程、运行过程中可能涉及到的一些细节，所以这门语言需要提供各种各样的**specifier**来供programmer对这些过程、性质进行控制（或者说对这些过程进行描述）；

> NOTE: 一个典型的例子是`static`，programmer可以用它来对storage duration、linkage进行描述，关于此，参见cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)。

从根本上来说，我们使用programming language来描述计算（在文章[Language](https://dengking.github.io/Post/Language/Language/)中有对此的分析），越是底层的语言，需要考虑的计算细节越多，所以语言的设计者需要提供specifier来对这些内容进行描述，进行控制。

如何设计一门programming language？或者说：语言的设计者在设计这一门语言的时候，需要考虑到哪些问题？这个问题我无法给出准确的答案，只能够给出基于使用python、c++和c的经验给出一些经验之谈，当我们阅读这些语言的official doc的时候，会发现programming language主要涉及两大块：language reference和standard library，其实，这些就是这门语言的standard，这就是这门语言的的design，下面以此为主要线索来进行详细说明。

对于一门programming language，有时，我们是需要考虑它的implementation的，思考implementation，能够帮助我们理解programming language中的一些概念，更加重要的是，只有这样，我们才能够掌握一门programming language的全貌。

### Design of programming language

将这部分内容放到了单独的一篇：[Design-of-programming-language](./Design-of-programming-language.md)。

### Implementation of programming language

将这部分内容放到了单独的一篇：[Implementation-of-programming-language](./Implementation-of-programming-language.md)。



### What's new in the programming language

> “Living languages must change,
> must adapt,
> must grow.”
> – Edward Finegan

上面这段话是Bjarne Stroustrup的[C++ in 2005](http://www.stroustrup.com/DnE2005.pdf)的开篇词，他的这篇文章描述了c++后续的发展方向。

Programming language是在不断演进的，各种programming language都会有自己的版本计划，在每个版本中，设计者会引入新的特性，这就是本节标题的What's new in the programming language的含义，即在这个版本中，引入了哪些新的特性。

作为开发者，在学习programming language的时候，可以从这些新特性，即从what is new入手来进行学习，首先搞清楚设计者引入这些新特性的意图、这些新特性所能够解决的问题、application，然后再去了解它们的grammar。这种学习方式是更加高效的。

### 首先学习property，然后学习描述这些property的specifier

为了准确地使用programming language提供的specifier，我们需要首先理解这些specifier所描述的property，这就是本机标题的含义。

关于此的一个典型例子就是[Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)，programmer可以用它来对storage duration、linkage进行描述，因此，在此之前，我们需要首先搞清楚storage duration、linkage的概念 。

### Philosophy

programming language的设计者往往是遵循着一定的philosophy来设计这门语言的，作为使用者，了解这门语言的philosophy，也有助于我们对它的掌握。



#### [Philosophy of python](https://www.python.org/dev/peps/pep-0020/)

#### [C++#Philosophy](https://en.wikipedia.org/wiki/C%2B%2B#Philosophy)

在维基百科[C++#Philosophy](https://en.wikipedia.org/wiki/C%2B%2B#Philosophy)中，描述了Philosophy of C++（在https://en.wikipedia.org/wiki/C++中对此有描述）




