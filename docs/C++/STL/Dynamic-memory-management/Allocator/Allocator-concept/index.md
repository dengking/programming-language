# Allocator 



## cppreference [C++ named requirements: *Allocator*](https://en.cppreference.com/w/cpp/named_req/Allocator)

Encapsulates strategies for access/addressing, allocation/deallocation and construction/destruction of objects.

> NOTE: 
>
> 一、典型的policy-based design，[`std::allocator`](https://en.cppreference.com/w/cpp/memory/allocator) 是它的default implementation:
>
> > The `std::allocator` class template is the default [*Allocator*](https://en.cppreference.com/w/cpp/named_req/Allocator) used by all standard library containers if no user-specified allocator is provided.
>
> [std::vector](https://en.cppreference.com/w/cpp/container/vector)
>
> ```C++
> template<
>     class T,
>     class Allocator = std::allocator<T>
> > class vector;
> ```
>
> 

### Fancy pointers

> NOTE: 
>
> 1、下面关于fancy pointer的介绍值得借鉴
>
> a、raw pointer用于access virtual address space
>
> b、fancy pointer用于access" objects allocated in address spaces that differ from the homogeneous(同质的) virtual address space "
>
> 2、关于"memory mapped files"，参见:
>
> a、wikipedia [Memory-mapped file](https://en.wikipedia.org/wiki/Memory-mapped_file)
>
> b、工程Linux-OS，其中对它进行了介绍
>
> 

When the member type `pointer` is not a **raw pointer type**, it is commonly referred to as a ["fancy pointer"](http://wg21.link/p0773r0). Such pointers were introduced to support segmented memory architectures and are used today to access objects allocated in address spaces that differ from the homogeneous(同质的) virtual address space that is accessed by raw pointers. An example of a fancy pointer is the mapping address-independent pointer [`boost::interprocess::offset_ptr`](http://www.boost.org/doc/libs/release/doc/html/interprocess/offset_ptr.html), which makes it possible to allocate node-based data structures such as [std::set](https://en.cppreference.com/w/cpp/container/set) in shared memory and memory mapped files mapped in different addresses in every process.

### Examples

> NOTE:
>
> 1、这个例子演示了如下custom allocator

