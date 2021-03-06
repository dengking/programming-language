# Trivial

按照维基百科[Modification to the definition of plain old data](https://en.wikipedia.org/wiki/C++11#Modification_to_the_definition_of_plain_old_data)中的说法，`trivial`是c++11引入的。

“trivial”的含义是“普通的、平凡的”，在C++中，它的含义是：不包含C++中的一些特性（相对而言比较高级的）：

- runtime polymorphism（所以trivial type和polymorphic type是相对的）
- advanced feature：
  - non-static members with default initializers

## What is trivial type？



在阅读cppreference的时候，多次碰到这个词语，有：

- [TrivialType](https://en.cppreference.com/w/cpp/named_req/TrivialType)

- [TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)
- [Trivial default constructor](https://en.cppreference.com/w/cpp/language/default_constructor#Trivial_default_constructor)
- [Trivial copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor#Trivial_copy_constructor)
- [Trivial move constructor](https://en.cppreference.com/w/cpp/language/move_constructor#Trivial_move_constructor)
- [Trivial copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment#Trivial_copy_assignment_operator)
- [Trivial move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment#Trivial_move_assignment_operator)
- [Trivial destructor](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor)



## TO READ

https://stackoverflow.com/questions/3899223/what-is-a-non-trivial-constructor-in-c

https://www.geeksforgeeks.org/trivial-classes-c/

http://olafurw.com/2019-02-14-trivial-code/

https://isocpp.org/blog/2018/03/quick-q-what-is-a-non-trivial-constructor-in-cpp

https://www.geeksforgeeks.org/trivial-classes-c/