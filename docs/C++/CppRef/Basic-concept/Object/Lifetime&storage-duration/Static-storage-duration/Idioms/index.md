# 关于本章

需要考虑的问题：

- 唯一性
- race condition

## Static-Initialization-Order-Fiasco 和 它的解决方案

在 `Static-Initialization-Order-Fiasco` 中，描述了它；下面对它的解决方案进行汇总: 

一、construct on first use，参见:

1、 `Construct-On-First-Use-Idiom` 章节

2、pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/)

二、singleton，参见`Singleton`

三、Nifty counter idiom



## Idioms

| name                                       | 描述章节                                                     |
| ------------------------------------------ | ------------------------------------------------------------ |
| Attach-by-Initialization                   | - `C++\Idiom\OOP\Attach-by-Initialization`                   |
| Construct-On-First-Use                     | - `Construct-On-First-Use-idiom`                             |
| Runtime Static Initialization Order Idioms | [More C++ Idioms/Runtime Static Initialization Order Idioms](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Runtime_Static_Initialization_Order_Idioms) |
| Nifty counter idiom                        | - `Nifty-Counter-idiom`                                      |



## Pattern

Singleton: 参见`C++\Pattern\Singleton`



## CppCoreGuidelines [I.22: Avoid complex initialization of global objects](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i22-avoid-complex-initialization-of-global-objects)

