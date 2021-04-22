# 关于本章

需要考虑的问题：

- 唯一性
- race condition

## Static-Initialization-Order-Fiasco 和 它的解决方案

在 `Static-Initialization-Order-Fiasco` 中，描述了它；下面对它的解决方案进行汇总: 

### How to prevent

1、construct on first use，参见 `Construct-On-First-Use-Idiom` 章节

2、singleton，参见`Singleton`

3、Nifty counter idiom

## Idioms

| name                                       | 描述章节                                   | 参见                                                         |
| ------------------------------------------ | ------------------------------------------ | ------------------------------------------------------------ |
| Attach-by-Initialization                   | - `C++\Idiom\OOP\Attach-by-Initialization` |                                                              |
| Construct-On-First-Use                     | - `Construct-On-First-Use-idiom`           | - stackoverflow [Construct on first use + forced initialization to solve static initialization order fiasco?](https://stackoverflow.com/questions/25122575/construct-on-first-use-forced-initialization-to-solve-static-initialization-or) <br>- parashift [Construct on first use](http://www.parashift.com/c++-faq/construct-on-first-use-v2.html) |
| Runtime Static Initialization Order Idioms |                                            | - [More C++ Idioms/Runtime Static Initialization Order Idioms](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Runtime_Static_Initialization_Order_Idioms) |
| Nifty counter idiom                        | - `Nifty-Counter-idiom`                    | - [More C++ Idioms/Nifty counter idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Nifty_Counter) |



## Pattern

Singleton: 参见`C++\Pattern\Singleton`



## CppCoreGuidelines [I.22: Avoid complex initialization of global objects](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i22-avoid-complex-initialization-of-global-objects)

## TODO

在下面章节中，提及了static variable的initialization问题:

cppreference [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)