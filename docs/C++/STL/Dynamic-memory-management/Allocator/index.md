# STL allocator



## wanweibaike [STL allocator](https://en.wanweibaike.com/wiki-STL%20allocator)

In [C++](https://en.wanweibaike.com/wiki-C%2B%2B) [computer programming](https://en.wanweibaike.com/wiki-Computer_programming), **allocators** are a component of the [C++ Standard Library](https://en.wanweibaike.com/wiki-C%2B%2B_Standard_Library). The standard [library](https://en.wanweibaike.com/wiki-Software_library) provides several [data structures](https://en.wanweibaike.com/wiki-Data_structures), such as [list](https://en.wanweibaike.com/wiki-List_(computing)) and [set](https://en.wanweibaike.com/wiki-Set_(computer_science)), commonly referred to as [containers](https://en.wanweibaike.com/wiki-Container_(data_structure)). A common trait among these containers is their ability to change size during the [execution](https://en.wanweibaike.com/wiki-Run_time_(program_lifecycle_phase)) of the [program](https://en.wanweibaike.com/wiki-Computer_program). To achieve this, some form of [dynamic memory allocation](https://en.wanweibaike.com/wiki-Dynamic_memory_allocation) is usually required. Allocators handle all the requests for [allocation and deallocation](https://en.wanweibaike.com/wiki-Manual_memory_management) of memory for a given container. The C++ Standard Library provides general-purpose allocators that are used by default, however, custom allocators may also be supplied by the [programmer](https://en.wanweibaike.com/wiki-Programmer).

> NOTE:
>
> 1、STL采用的是policy-based design: 将allocator作为container的template parameter

### Custom allocators

One of the main reasons for writing a custom allocator is performance. Utilizing a specialized custom allocator may substantially improve the performance or memory usage, or both, of the program.[[4\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-effectivestl-4)[[15\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-aue-15) The default allocator uses `operator new` to allocate memory.[[16\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-C++03_20.4.1.1/3-16) This is often implemented as a thin layer around the [C](https://en.wanweibaike.com/wiki-C_(programming_language)) heap [allocation functions](https://en.wanweibaike.com/wiki-Malloc),[[17\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-moderncpp-17) which are usually optimized for infrequent allocation of large memory blocks. This approach may work well with containers that mostly allocate large chunks of memory, like [vector](https://en.wanweibaike.com/wiki-Vector_(C%2B%2B)) and [deque](https://en.wanweibaike.com/wiki-Deque).[[15\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-aue-15) However, for containers that require frequent allocations of small objects, such as [map](https://en.wanweibaike.com/wiki-Map_(C%2B%2B)) and [list](https://en.wanweibaike.com/wiki-List_(computing)), using the default allocator is generally slow.[[4\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-effectivestl-4)[[17\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-moderncpp-17) Other common problems with a [malloc](https://en.wanweibaike.com/wiki-Malloc)-based allocator include poor [locality of reference](https://en.wanweibaike.com/wiki-Locality_of_reference),[[4\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-effectivestl-4) and excessive [memory fragmentation](https://en.wanweibaike.com/wiki-Fragmentation_(computer)).[[4\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-effectivestl-4)[[17\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-moderncpp-17)

### Usage

When instantiating one of the standard containers, the allocator is specified through a [template](https://en.wanweibaike.com/wiki-Template_(programming)) argument, which [defaults](https://en.wanweibaike.com/wiki-Default_argument) to `std::allocator<T>`:[[20\]](https://en.wanweibaike.com/wiki-STL allocator#cite_note-C++03_23.2/1-20)

```C++
namespace std {
  template <class T, class Allocator = allocator<T> > class vector;
// ...
```

> NOTE: 
>
> policy-based design

## cppreference [Dynamic memory management](https://en.cppreference.com/w/cpp/memory) # [Allocators](https://en.cppreference.com/w/cpp/memory#Allocators)

Allocators are class templates encapsulating memory allocation strategy. This allows generic containers to decouple memory management from the data itself.

> NOTE: 
>
> 一、decouple解耦
>
> 二、这些allocator的实现，最终都是依赖于new operator

