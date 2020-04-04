# Initialization

先看看[learncpp](https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/)的[1.4 — Variable assignment and initialization](https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/)中关于initialization的介绍，因为其中结合了具体的示例，更加容易理解，然后再来看cppreference中对[Initialization](https://en.cppreference.com/w/cpp/language/initialization)的更加标准化的定义。



## learncpp [1.4 — Variable assignment and initialization](https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/)

One downside of assignment is that it requires at least two statements: one to define the variable, and one to assign the value.

These two steps can be combined. When a variable is defined, you can also provide an initial value for the variable at the same time. This is called **initialization**.

C++ supports three basic ways to initialize a variable. First, we can do **copy initialization** by using an equals sign:

```c++
int width = 5; // copy initialization of value 5 into variable width
```

Much like copy assignment, this copies the value on the right-hand side of the equals to the variable being created on the left-hand side.

Second, we can do **direct initialization** by using parenthesis.

```c++
int width( 5 ); // direct initialization of value 5 into variable width
```

For simple data types (like integers), copy and direct initialization are essentially the same. But for some advanced types, direct initialization can perform better than copy initialization. Prior to C++11, direct initialization was recommended over copy initialization in most cases because of the performance boost.



## Initialization VS assignment

两者之间的相同点是都会改变object的值。



## cppreference [Initialization](https://en.cppreference.com/w/cpp/language/initialization)

*Initialization* of a variable provides its initial value at the time of construction.

> NOTE: 



## TO READ

learncpp [1.6 — Uninitialized variables and undefined behavior](https://www.learncpp.com/cpp-tutorial/uninitialized-variables-and-undefined-behavior/)

learncpp [8.5b — Non-static member initialization](https://www.learncpp.com/cpp-programming/8-5b-non-static-member-initialization/)

learncpp [8.5a — Constructor member initializer lists](https://www.learncpp.com/cpp-tutorial/8-5a-constructor-member-initializer-lists/)

learncpp [11.4 — Constructors and initialization of derived classes](https://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/)

learncpp [10.7 — std::initializer_list](https://www.learncpp.com/cpp-tutorial/10-7-stdinitializer_list/)

learncpp [9.12 — Copy initialization](https://www.learncpp.com/cpp-tutorial/9-12-copy-initialization/)