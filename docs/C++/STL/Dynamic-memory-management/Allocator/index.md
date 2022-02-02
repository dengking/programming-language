# STL allocator



## wikipedia [STL allocator](https://en.wikipedia.org/wiki/Allocator_(C%2B%2B))

In [C++](https://en.wikipedia.org/wiki/C%2B%2B) [computer programming](https://en.wikipedia.org/wiki/Computer_programming), **allocators** are a component of the [C++ Standard Library](https://en.wikipedia.org/wiki/C%2B%2B_Standard_Library). The standard [library](https://en.wikipedia.org/wiki/Software_library) provides several [data structures](https://en.wikipedia.org/wiki/Data_structures), such as [list](https://en.wikipedia.org/wiki/List_(computing)) and [set](https://en.wikipedia.org/wiki/Set_(computer_science)), commonly referred to as [containers](https://en.wikipedia.org/wiki/Container_(data_structure)). A common trait among these containers is their ability to change size during the [execution](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) of the [program](https://en.wikipedia.org/wiki/Computer_program). To achieve this, some form of [dynamic memory allocation](https://en.wikipedia.org/wiki/Dynamic_memory_allocation) is usually required. Allocators handle all the requests for [allocation and deallocation](https://en.wikipedia.org/wiki/Manual_memory_management) of memory for a given container. The C++ Standard Library provides general-purpose allocators that are used by default, however, custom allocators may also be supplied by the [programmer](https://en.wikipedia.org/wiki/Programmer).

> NOTE:
>
> 1、STL采用的是policy-based design: 将allocator作为container的template parameter



### Usage

When instantiating one of the standard containers, the allocator is specified through a [template](https://en.wanweibaike.com/wiki-Template_(programming)) argument, which [defaults](https://en.wanweibaike.com/wiki-Default_argument) to `std::allocator<T>`:[[20\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-C++03_23.2/1-20)

```C++
namespace std {
  template <class T, class Allocator = allocator<T> > class vector;
// ...
```

> NOTE: 
>
> 一、典型的policy-based design

## cppreference [Dynamic memory management](https://en.cppreference.com/w/cpp/memory) # [Allocators](https://en.cppreference.com/w/cpp/memory#Allocators)

Allocators are class templates encapsulating memory allocation strategy. This allows generic containers to decouple memory management from the data itself.

> NOTE: 
>
> 一、decouple解耦、strategy design pattern
>
> 二、这些allocator的实现，最终都是依赖于new operator

