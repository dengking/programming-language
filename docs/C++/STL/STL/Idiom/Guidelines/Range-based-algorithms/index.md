# Range-based algorithms

本文标题的含义是"C++ algorithm是基于range的"，它是C++ algorithm的使用模式，本文对此进行总结。

下面是对本节标题的观点的说明。

## Before C++20

Before C++20，algorithm基本上都是如下模式: 

```C++
template <typename IntputIterator, typename OutputIterator>
OutputIterator algorithm(IntputIterator begin, IntputIterator end, OutputIterator out)
{
    
}
```



### 1、fluentcpp [Smart Output Iterators: A Symmetrical Approach to Range Adaptors](https://www.fluentcpp.com/2017/11/28/output-iterator-adaptors-symmetry-range-adaptors/)

Some of the algorithms of the STL have a structure in common: they take one or more ranges in input, do something more or less elaborate with them, and produce an output in a destination range.

For example, `std::copy` merely copies the inputs to the outputs, `std::transform` applies a function onto the inputs and sends the results as outputs, and [`std::set_difference`](https://www.fluentcpp.com/2017/01/09/know-your-algorithms-algos-on-sets/) takes two input ranges and outputs to a destination range the elements that are in the first one but not in the second.

There are several ways to express this kind of input-operation-output structure on ranges in C++. To illustrate them, let’s take the example of `std::transform` since it is such a [central algorithm in the STL](https://www.fluentcpp.com/2017/02/13/transform-central-algorithm/).

To make the code examples lighter, let’s suppose that we have some modified versions of STL algorithms that take an input range instead of two iterators, for instance:



### 2、[C++ algorithms library](https://en.cppreference.com/w/cpp/algorithm/)

C++ algorithms library中的大部分algorithm是典型的“range-based algorithm”，即它们的入参一般都是：

a、`InputIt first`

b、`InputIt last`

## Since C++20

C++20引入了range library，这重构了C++ algorithm，参见 `Ranges-library-C++20` 章节。