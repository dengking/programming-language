# boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html)

> NOTE: 
>
> 1、这篇文章非常值得一读，原文它总结了boost library的实现中所使用到的generic programming technique，关于C++ generic programming，参见`C++\Language-reference\Template\Programming-paradigm\Generic-programming`章节。
>
> 2、我就是通过这篇文章，开始了generic programming的学习的
>
> 3、本文着重介绍了generic programming的一些technique，其中，最最让我印象深刻的是对"concept"的介绍，非常好，通过"Anatomy of a Concept"章节的描述可以看出: boost 是遵循经典的generic programming的思想的。
>
> concept是GP的重要组成部分，C++20添加了concept特性以支持generic programming concept，参见`C++\Language-reference\Template\Programming-paradigm\Generic-programming\Concepts`章节；boost很早就采用了concept technique，它的The Boost Concept Check Library (BCCL) 支持concept，参见`C++\Library\Boost\Boost-Concept-Check-Library`章节。

## Introduction

Generic programming is about generalizing software components so that they can be easily reused in a wide variety of situations. In C++, class and function templates are particularly effective mechanisms for generic programming because they make the generalization possible without sacrificing efficiency.

> NOTE: 
>
> generic programming能够保证
>
> 1、"reused": code reuse
>
> 2、"effective "、"sacrificing efficiency": performance、static polymorphism

> NOTE: 原文的这一节主要讲述generic programming

### C generic programming

As a simple example of generic programming, we will look at how one might generalize the `memcpy()` function of the C standard library. An implementation of `memcpy()` might look like the following:



```c++
void* memcpy(void* region1, const void* region2, size_t n)
{
  const char* first = (const char*)region2;
  const char* last = ((const char*)region2) + n;
  char* result = (char*)region1;
  while (first != last)
    *result++ = *first++;
  return result;
}
```

### C++ generic programming

> NOTE:
>
> 1、这一段关于C++ 和 C generic  programming的比较是非常好的，通过这一段可知: 相比于C void pointer，C++ concept是更加强大的、抽象的，因此能够实现更大的power

The `memcpy()` function is already generalized to some extent by the use of `void*` so that the function can be used to copy **arrays** of different kinds of data. But what if the data we would like to copy is not in an **array**? Perhaps it is in a linked list. Can we generalize the notion of copy to any sequence of elements? Looking at the body of `memcpy()`, the function's **minimal requirements** are that it needs to *traverse* through the sequence using some sort of pointer, *access* elements pointed to, *write* the elements to the destination, and *compare* pointers to know when to stop. The C++ standard library groups requirements such as these into **concepts**, in this case the [Input Iterator](http://en.cppreference.com/w/cpp/concept/InputIterator) concept (for `region2`) and the [Output Iterator](http://en.cppreference.com/w/cpp/concept/OutputIterator) concept (for `region1`).



If we rewrite the `memcpy()` as a function template, and use the [Input Iterator](http://en.cppreference.com/w/cpp/concept/InputIterator) and [Output Iterator](http://en.cppreference.com/w/cpp/concept/OutputIterator) concepts to describe the requirements on the template parameters, we can implement a highly reusable `copy()` function in the following way:

```c++
template <typename InputIterator, typename OutputIterator>
OutputIterator
copy(InputIterator first, InputIterator last, OutputIterator result)
{
  while (first != last)
    *result++ = *first++;
  return result;
}
```

Using the generic `copy()` function, we can now copy elements from any kind of sequence, including a linked list that exports iterators such as `std::list `.

```C++
#include <list>
#include <vector>
#include <iostream>

int main()
{
  const int N = 3;
  std::vector<int> region1(N);
  std::list<int> region2;

  region2.push_back(1);
  region2.push_back(0);
  region2.push_back(3);
  
  std::copy(region2.begin(), region2.end(), region1.begin());

  for (int i = 0; i < N; ++i)
    std::cout << region1[i] << " ";
  std::cout << std::endl;
}
```



## Anatomy of a Concept

> NOTE: "anatomy"的含义是"解剖"。

A **concept** is a set of requirements consisting of valid expressions, associated types, invariants, and complexity guarantees. A type that satisfies the requirements is said to **model** the concept. A concept can extend the requirements of another concept, which is called **refinement**.

> NOTE: 
>
> 1、**refinement**的意思是“细化”，这个词语一定要注意，在boost library的doc中它出现的频率非常高。
>
> 2、类比:
>
> GP concept <--> OOP interface
>
> GP concept model <--> OOP interface implementation
>
> GP concept refinement<--> OOP class inheritance

1) **Valid Expressions** are C++ expressions which must compile successfully for the objects involved in the expression to be considered *models* of the concept.

> NOTE: 
>
> 1、**Valid Expressions**的含义非常重要
>
> 2、**Valid Expressions** 是behavior-based
>
> 3、**Valid Expressions** 是 compile-time、static

2) **Associated Types** are types that are related to the modeling type in that they participate in one or more of the **valid expressions**. Typically associated types can be accessed either through typedefs nested within a class definition for the modeling type, or they are accessed through a [traits class](https://www.boost.org/community/generic_programming.html#traits).

3) **Invariants** are run-time characteristics of the objects that must always be true, that is, the functions involving the objects must preserve these characteristics. The invariants often take the form of pre-conditions and post-conditions.

> NOTE: 
>
> 1、invariant是dynamic的
>
> 2、上述invariant、pre-condition、post-condition是design-by-contract领域的内容

4) **Complexity Guarantees** are maximum limits on how long the execution of one of the valid expressions will take, or how much of various resources its computation will use.

The concepts used in the C++ Standard Library are documented at the [C++ reference website](http://en.cppreference.com/w/).



## Traits

A traits class provides a way of associating information with a **compile-time entity** (a type, integral constant, or address). 

> NOTE: 此处使用entity的概念来定义trait，更加地准确。

For example, the class template [`std::iterator_traits`](http://en.cppreference.com/w/cpp/iterator/iterator_traits) looks something like this:

```c++
template <class Iterator>
struct iterator_traits {
  typedef ... iterator_category;
  typedef ... value_type;
  typedef ... difference_type;
  typedef ... pointer;
  typedef ... reference;
};
```

The traits' `value_type` gives generic code the type which the iterator is "pointing at", while the `iterator_category` can be used to select more efficient algorithms depending on the iterator's capabilities.

A key feature of traits templates is that they're *non-intrusive*: they allow us to associate information with arbitrary types, including built-in types and types defined in third-party libraries, Normally, traits are specified for a particular type by (partially) specializing the traits template.

> NOTE: 上面这段话中的*non-intrusive*的含义是“非侵入式的”，现在反省一下对trait的使用，往往是单独定义一个class的。

## Tag Dispatching

> NOTE: tag dispatch是一种idiom

The relation between tag dispatching and traits classes is that the property used for dispatching (in this case the `iterator_category`) is often accessed through a traits class. 

```c++
namespace std {
  struct input_iterator_tag { };
  struct bidirectional_iterator_tag { };
  struct random_access_iterator_tag { };

  namespace detail {
    template <class InputIterator, class Distance>
    void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
      while (n--) ++i;
    }

    template <class BidirectionalIterator, class Distance>
    void advance_dispatch(BidirectionalIterator& i, Distance n, 
       bidirectional_iterator_tag) {
      if (n >= 0)
        while (n--) ++i;
      else
        while (n++) --i;
    }

    template <class RandomAccessIterator, class Distance>
    void advance_dispatch(RandomAccessIterator& i, Distance n, 
       random_access_iterator_tag) {
      i += n;
    }
  }

  template <class InputIterator, class Distance>
  void advance(InputIterator& i, Distance n) {
    typename iterator_traits<InputIterator>::iterator_category category;
    detail::advance_dispatch(i, n, category);
  }
}
```

## Adaptors

An *adaptor* is a class template which builds on another type or types to provide a new interface or behavioral variant. Examples of standard adaptors are [std::reverse_iterator](http://en.cppreference.com/w/cpp/iterator/reverse_iterator), which adapts an iterator type by reversing its motion upon increment/decrement, and [std::stack ](http://en.cppreference.com/w/cpp/container/stack), which adapts a container to provide a simple stack interface.

## Type Generators

> NOTE:
>
> 1、C++11已经将它标准化了

## Object Generators



## Policy Classes

A policy class is a template parameter used to transmit(传输) **behavior**. An example from the standard library is `std::allocator`, which supplies（提供） memory management behaviors to standard [containers](http://en.cppreference.com/w/cpp/container).

Policy classes have been explored in detail by [Andrei Alexandrescu](http://www.moderncppdesign.com/) in [this chapter](http://www.informit.com/articles/article.aspx?p=167842) of his book, *Modern C++ Design*. He writes:

> In brief, policy-based class design fosters(促进) assembling a class with complex behavior out of many little classes (called policies), each of which takes care of only one behavioral or structural aspect. As the name suggests, a policy establishes an interface pertaining(关于) to a specific issue. You can implement policies in various ways as long as you respect the **policy interface**.
>
> Because you can mix and match policies, you can achieve a combinatorial set of behaviors by using a small core of elementary components.

Andrei's description of **policy classes** suggests that their power is derived from granularity（粒度） and orthogonality（正交）. **Less-granular policy interfaces** have been shown to work well in practice, though. [This paper](http://svn.boost.org/svn/boost/tags/release/Boost_1_30_2/boost/libs/utility/iterator_adaptors.pdf) describes an old version of [`iterator_adaptor`](https://www.boost.org/doc/libs/release/libs/iterator/doc/iterator_adaptor.html) that used non-orthogonal policies. There is also precedent in the standard library: [`std::char_traits`](http://en.cppreference.com/w/cpp/string/char_traits), despite its name, acts as a policies class that determines the behaviors of [std::basic_string](http://en.cppreference.com/w/cpp/string/basic_string).