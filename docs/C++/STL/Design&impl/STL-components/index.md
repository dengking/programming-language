# STL components

随着C++的发展，STL中，包含的library越来越多。

## STL六大组件

### csdn [STL标准库六大组件 关系 作用](https://blog.csdn.net/qq_31108501/article/details/55049937)

1、容器（Containers）：各种数据结构，如Vector,List,Deque,Set,Map,用来存放数据，STL容器是一种Class Template,就体积而言，这一部分很像冰山载海面的比率。

2、算法（Algorithms）：各种常用算法如Sort,Search,Copy,Erase,从实现的角度来看，STL算法是一种Function Templates。

3、迭代器（Iterators）：扮演容器与算法之间的胶合剂，是所谓的“泛型指针”，共有五种类型，以及其它衍生变化，从实现的角度来看，迭代器是一种将：Operators*,Operator->,Operator++,Operator--等相关操作予以重载的Class Template。所有STL容器都附带有自己专属的迭代器——是的，只有容器设计者才知道如何遍历自己的元素，原生指针（Native pointer）也是一种迭代器。

4、仿函数（Functors）： 行为类似函数，可作为算法的某种策略（Policy）,从实现的角度来看，仿函数是一种重载了Operator()的Class 或 Class Template。一般函数指针可视为狭义的仿函数。

5、配接器（适配器）（Adapters）：一种用来修饰容器（Containers）或仿函数（Functors）或迭代器（Iterators）接口的东西，例如：STL提供的Queue和Stack，虽然看似容器，其实只能算是一种容器配接器，因为 它们的底部完全借助Deque，所有操作有底层的Deque供应。改变Functor接口者，称为Function Adapter;改变Container接口者，称为Container Adapter;改变Iterator接口者，称为Iterator Adapter。配接器的实现技术很难一言蔽之，必须逐一分析。

6、分配器（Allocators）：负责空间配置与管理，从实现的角度来看，配置器是一个实现了动态空间配置、空间管理、空间释放的Class Template。

——《STL源码剖析》

## STL three three main components

三大重要组件。

### geeksforgeeks [std::istream_iterator and std::ostream_iterator in C++ STL](https://www.geeksforgeeks.org/stdistream_iterator-stdostream_iterator-c-stl/)

The [STL](https://www.geeksforgeeks.org/the-c-standard-template-library-stl/) is a very powerful library in C++. It is strongly built on the principles of template programming.
**The STL library has three main components :**

1、**Containers:** These classes define the data structures which are used to contain the data. The data may be stored in linked lists, or trees or arrays. The containers provided in the STL are vector, dequeue, list, forward list, set, multiset, map and multimap.

2、**Algorithms:** The STL library also provides us functions to process the data stored in containers. These functions are provided in the *algorithm* header file

3、**Iterators:** Iterators are the link between the Containers and the Algorithms. They are the common interface to these classes. An Iterator is an object which can be used to iterator over the elements in a container. Thus iterators are used by Algorithms to modify the containers.

All three components are so designed that they confirm to the principles of data abstraction. Thus any object which holds data and *behaves like a container*, is a container. Similarly, any iterator which sweeps through the elements in a container is an iterator.

> NOTE: 
>
> duck type



