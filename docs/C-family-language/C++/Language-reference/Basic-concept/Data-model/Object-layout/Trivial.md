# Trivial

按照维基百科[Modification to the definition of plain old data](https://en.wikipedia.org/wiki/C++11#Modification_to_the_definition_of_plain_old_data)中的说法，`trivial`是c++11引入的。

在阅读cppreference的时候，多次碰到这个词语，有：

- [TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)
- [Trivial copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor#Trivial_copy_constructor)
- [Trivial move constructor](https://en.cppreference.com/w/cpp/language/move_constructor#Trivial_move_constructor)
- [Trivial copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment#Trivial_copy_assignment_operator)
- [Trivial move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment#Trivial_move_assignment_operator)

最最核心的描述是[TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)，trivial不允许任何virtual，那么这就意味中，它[non-polymorphic object](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)。



[TrivialType](https://en.cppreference.com/w/cpp/named_req/TrivialType)

[Trivial default constructor](https://en.cppreference.com/w/cpp/language/default_constructor#Trivial_default_constructor)

[std::is_trivial](https://en.cppreference.com/w/cpp/types/is_trivial)



## Trivial classes

### geeksforgeeks [Trivial classes in C++](https://www.geeksforgeeks.org/trivial-classes-c/)