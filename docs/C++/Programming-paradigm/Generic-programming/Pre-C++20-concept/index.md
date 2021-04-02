# 关于本章

本章的标题"Pre C++20 concept"的含义是: 在C++20之前，如何使用类似于C++20 concept的用法来进行generic programming，下面是我目前已知的方式:

| 方式                                   | 章节                                            |
| -------------------------------------- | ----------------------------------------------- |
| The Boost Concept Check Library (BCCL) | `C++\Library\Boost\Boost-Concept-Check-Library` |
| ConceptC++                             | `./ConceptC++`                                  |



1、`Detection`

2、可以使用trait来实现static reflection，用于判断type

## 如何书写valid expression？

1、如果有object，则使用这个object来写valid expression

2、如果没有object，则使用`std::declval`来模拟object，然后写valid expression

这在 `std-declval` 章节中，也进行了总结。