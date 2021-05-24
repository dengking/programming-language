# 关于本章

本章讨论C++中如何query type。C++中，分为static type和dynamic type，因此，C++对它们分别提供了operator:

|              | operator   | 章节         |
| ------------ | ---------- | ------------ |
| 查询静态类型 | `decltype` | `./decltype` |
| 查询动态类型 | typeid     | `./typeid`   |



## stackoverflow ['typeid' versus 'typeof' in C++](https://stackoverflow.com/questions/1986418/typeid-versus-typeof-in-c)

C++ language has no such thing as `typeof`. You must be looking at some compiler-specific extension. If you are talking about GCC's `typeof`, then a similar feature is present in C++11 through the keyword `decltype`. Again, C++ has no such `typeof` keyword.

`typeid` is a C++ language operator which returns type identification information at run time. It basically returns a `type_info` object, which is equality-comparable with other `type_info` objects.