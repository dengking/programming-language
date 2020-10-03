# Initialization



## cppreference [Initialization](https://en.cppreference.com/w/cpp/language/initialization)



For each declarator, the **initializer** may be one of the following:

| initializer            | explanation                                                  | 注解                                                      |
| ---------------------- | ------------------------------------------------------------ | --------------------------------------------------------- |
| `( expression-list )`  | comma-separated list of arbitrary expressions and **braced-init-lists in parentheses** | 需要注意的是，必须是 **braced-init-lists in parentheses** |
| `= expression`         |                                                              |                                                           |
| `{ initializer-list }` | **braced-init-list**: possibly empty, comma-separated list of expressions and other **braced-init-lists** |                                                           |

Depending on context, the **initializer** may invoke:

| initialization                                               | example                         | 注解 |
| ------------------------------------------------------------ | ------------------------------- | ---- |
| [Value initialization](https://en.cppreference.com/w/cpp/language/value_initialization) | `std::string s{};`              |      |
| [Direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) | `std::string s("hello");`       |      |
| [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization) | `std::string s = "hello";`      |      |
| [List initialization](https://en.cppreference.com/w/cpp/language/list_initialization) | `std::string s{'a', 'b', 'c'};` |      |
| [Aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization) | `char a[3] = {'a', 'b'};`       |      |
| [Reference initialization](https://en.cppreference.com/w/cpp/language/reference_initialization) | `char& c = a[0];`               |      |

If no initializer is provided, the rules of [default initialization](https://en.cppreference.com/w/cpp/language/default_initialization) apply.

### Non-local variables

> NOTE: 首先需要搞清楚non-local variable的概念，参见
>
> - `Theory\Resource-management\Memory-management\Variable`章节
>
> - `C-family-language\C++\Language-reference\Basic-concept\Data-model\Object\Storage.md`章节
>
> 

All non-local variables with static [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) are initialized as part of program startup, before the execution of the [main function](https://en.cppreference.com/w/cpp/language/main_function) begins (unless deferred, see below). 

All non-local variables with thread-local storage duration are initialized as part of thread launch, sequenced-before the execution of the thread function begins. 

For both of these classes of variables, initialization occurs in two distinct stages:

#### Static initialization

> NOTE: static initialization的含义是什么？它是发生在compile-time吗？后面有专门的章节对它进行介绍



#### Dynamic initialization



#### Early dynamic initialization



#### Deferred dynamic initialization



### Static local variables



### Class members





|                                                              |                                                              |      |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ---- |
| [Zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization) | Sets the initial value of an object to zero.                 |      |
| [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization) | This is the initialization performed when a variable is constructed with no initializer. |      |
| [Constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization) | Sets the initial values of the [static](https://en.cppreference.com/w/cpp/language/storage_duration) variables to a compile-time constant. |      |





## TODO: value initialization VS zero-size initializer list

两者的grammar是相同的，compiler如何来进行区分呢？

在akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/) 中对这个问题进行了讨论。









## draft



先看看[learncpp](https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/)的[1.4 — Variable assignment and initialization](https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/)中关于initialization的介绍，因为其中结合了具体的示例，更加容易理解，然后再来看cppreference中对[Initialization](https://en.cppreference.com/w/cpp/language/initialization)的更加标准化的定义。



### learncpp [1.4 — Variable assignment and initialization](https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/)

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



### Initialization VS assignment

两者之间的相同点是都会改变object的值。





### TO READ

learncpp [1.6 — Uninitialized variables and undefined behavior](https://www.learncpp.com/cpp-tutorial/uninitialized-variables-and-undefined-behavior/)

learncpp [8.5b — Non-static member initialization](https://www.learncpp.com/cpp-programming/8-5b-non-static-member-initialization/)

learncpp [8.5a — Constructor member initializer lists](https://www.learncpp.com/cpp-tutorial/8-5a-constructor-member-initializer-lists/)

learncpp [11.4 — Constructors and initialization of derived classes](https://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/)

learncpp [10.7 — std::initializer_list](https://www.learncpp.com/cpp-tutorial/10-7-stdinitializer_list/)

learncpp [9.12 — Copy initialization](https://www.learncpp.com/cpp-tutorial/9-12-copy-initialization/)