# 关于本章

下面对本章的idiom进行了总结：

template metaprogramming中核心的idiom：

[SFINAE-trait-enable-if](./SFINAE-trait-enable-if/index.md)

template metaprogramming的设计思想：

[Policy-based-design](./Policy-based-design/Policy-based-design.md)

简化代码的技巧：

| idiom                                                        | example                                                      | summary                                                      |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Object Generator](./Object-Generator/Object-Generator.md)   | [std::make_pair](https://en.cppreference.com/w/cpp/utility/pair/make_pair)、[std::make_shared](https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared) | 充分运用function template的automatically deduce parameter特性，进而实现简化代码的目的 |
| [Return Type Resolver](./Return-Type-Resolver/Return-Type-Resolver.md) | `std::set<int> random_s = getRandomN(10);`                   | 提供**templatized conversion operator function**，充分运用compiler自动地调用**conversion operator function**来讲将**temporary object**转换为目标变量（即等号左侧的变量）从而达到简化代码的目的 |
| [nullptr](./nullptr/nullptr.md)                              |                                                              | 对[Return Type Resolver](./Return-Type-Resolver/Return-Type-Resolver.md)的应用 |



template alias：

| idiom                                                | example                | summary                                                      |
| ---------------------------------------------------- | ---------------------- | ------------------------------------------------------------ |
| [Type Generator](./Type-Generator/Type-Generator.md) | `Directory<int>::type` | 其实就是Templated Typedef，c++11的using，即[Template aliases](https://en.wikipedia.org/wiki/C++11#Template_aliases)能够简化type generator。 |
|                                                      |                        |                                                              |

reflection：

| idiom                                                   | example | summary                                  |
| ------------------------------------------------------- | ------- | ---------------------------------------- |
| [Member Detector](./Member-Detector/Member-Detector.md) |         | 实现Compile-time reflection capabilities |
|                                                         |         |                                          |



OOP：

| idiom                                                        | idiom using it                                               |                  | summary                                                      |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ---------------- | ------------------------------------------------------------ |
| [Curiously Recurring Template Pattern](./Curiously-recurring-template-pattern/Curiously-recurring-template-pattern.md) | [Non-copyable Mixin](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin) <br> | Mixin-from-above | Specialize a base class using the derived class as a template argument，从而实现Static polymorphism |
| [Parameterized Base Class](./Parameterized-Base-Class/Parameterized-Base-Class.md) |                                                              | Mixin-from-below | To abstract out an aspect in a reusable module and combine it in a given type when required. |
| [Barton-Nackman trick](Barton–Nackman-trick/Barton–Nackman-trick.md) |                                                              |                  | 已经过时，现代compiler已经支持了。                           |

