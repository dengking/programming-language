# char_traits

是在阅读[C++ Type Traits | Dr Dobb's](http://www.drdobbs.com/cpp/c-type-traits/184404270)时，发现其中有对`std::char_trait`的描述。

`char_traits`是典型的policy-based design。

## cppreference [std::char_traits](https://en.cppreference.com/w/cpp/string/char_traits)

The `char_traits` class is a traits class template that abstracts basic character and string operations for a given character type.

> NOTE: 在c++ standard library中，基本上，所有涉及basic character and string operation的，都是由[std::char_traits](https://en.cppreference.com/w/cpp/string/char_traits)提供的。

The defined operation set is such that generic algorithms almost always can be implemented in terms of it. It is thus possible to use such algorithms with almost any possible character or string type, just by supplying a customized `char_traits` class.

> NOTE: 这段话的中心思想是：`char_traits`的设计思想是支持generic algorithms



## [What is the point of STL Character Traits?](https://stackoverflow.com/questions/5319770/what-is-the-point-of-stl-character-traits)



## use of `char_traits` in standard library

[std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string)

[Input/output library](Input/output library)