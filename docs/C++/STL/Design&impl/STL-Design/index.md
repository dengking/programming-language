# STL design



## Generic programming

[Generic programming](https://en.wikipedia.org/wiki/Generic_programming)是STL的核心思想，SLT的设计中，运用了C++ generic programming的一些idiom:

1、policy-based design



下面是一些运用了generic programming或者对STL进行阐述的文章：

1、[The Boost Graph Library (BGL)](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/)



关于Generic programming参见:

1、`Theory\Programming-paradigm\Generic-programming`

## Design pattern

STL中使用到的一些design pattern。

### stackoverflow [design patterns used in STL(standard template library)](https://stackoverflow.com/questions/2698474/design-patterns-used-in-stlstandard-template-library)

The STL stack is a container adapter. An adapter is a design pattern. The iterator is also a design pattern. The STL function objects are related to the command pattern.

Patterns:

1、Adapter (container adapters)

\- stack

\- queues

\- priority queues

2、Iterator

3、Command + Adapter (function adapters)

4、Iterator + Adapter (iterator adapters)

\- reverse iterators

\- insert iterators

\- stream iterators

5、Template Method (STL algorithms using user-specified functions)

6、Which creational pattern? (Allocators)

The way these patterns are implemented is very different from the way they are implemented in an object oriented design. Josuttis wrote "the STL concept contradicts the original idea of object-oriented programming". This is what is causing the confusion around your question.

### Iterator pattern

[Iterator pattern](https://en.wikipedia.org/wiki/Iterator_pattern)

### Adapter pattern

正如在[design patterns used in STL(standard template library)](https://stackoverflow.com/questions/2698474/design-patterns-used-in-stlstandard-template-library)的[回答](https://stackoverflow.com/a/2698775)中所说：

> The STL stack is a container adapter.



关于adapter pattern，参见

1、wikipedia [Adapter pattern](https://en.wikipedia.org/wiki/Adapter_pattern)



## Policy-based design in STL

1、STL中的policy based design

2、这体现了"C++特性-prefer-Static-over-dynamic-polymorphism"

3、CppCoreGuidelines-Per.11 Move computation from run time to compile time



包括:

1、allocator

2、`char_trait`

