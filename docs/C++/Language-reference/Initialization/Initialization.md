# Initialization

## 思路

C++的initialization不仅种类繁多而且规则冗杂，下面是比较好的整理思路:

首先需要区分每种initialization的syntax，因为compiler根据syntax来决定采用何种initialization；

其次是需要搞清楚每种initialization的**effect**，对effect的讨论主要集中在对下面三种type的effect: 

| type           | 注解        |
| -------------- | ----------- |
| built-in type  | int、char等 |
| OOP class type | class       |
| aggregate type | array       |

各种initialization的差异重要体现在它们的**effect**上。

## cppreference [Initialization](https://en.cppreference.com/w/cpp/language/initialization)

*Initialization* of a variable provides its initial value at the time of **construction**.

> NOTE: initialization发生在object construction阶段，关于object的lifetime，参见`C++\Language-reference\Basic-concept\Data-model\Object\Object-storage-duration-and-lifetime`。

### Initializer

The initial value may be provided in the **initializer** section of a [declarator](https://en.cppreference.com/w/cpp/language/declarations) or a [new expression](https://en.cppreference.com/w/cpp/language/new).

> ### Terminology: initializer
>
> "initializer"是cppreference中用来表示**初始值**的术语，在下面文章中使用了这个术语: 
>
> - cppreference [new expression](https://en.cppreference.com/w/cpp/language/new)
> - cppreference [Declarators](https://en.cppreference.com/w/cpp/language/declarations#Declarators) 



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

> NOTE: 参见`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Static-storage-duration\Non-local-object`

### Static local variables

> NOTE: 参见`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Static-storage-duration\Static-local-object`

### Class members

Non-static data members can be initialized with [member initializer list](https://en.cppreference.com/w/cpp/language/initializer_list) or with a [default member initializer](https://en.cppreference.com/w/cpp/language/data_members#Member_initialization).

## 其他initialization

下面是一些其他initialization: 

|                                                              |                                                              |                |
| ------------------------------------------------------------ | ------------------------------------------------------------ | -------------- |
| [Zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization) | Sets the initial value of an object to zero.                 |                |
| [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization) | This is the initialization performed when a variable is constructed with no initializer. | no initializer |
| [Constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization) | Sets the initial values of the [static](https://en.cppreference.com/w/cpp/language/storage_duration) variables to a compile-time constant. |                |



## Draft



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





### TO READ

learncpp [1.6 — Uninitialized variables and undefined behavior](https://www.learncpp.com/cpp-tutorial/uninitialized-variables-and-undefined-behavior/)

learncpp [8.5b — Non-static member initialization](https://www.learncpp.com/cpp-programming/8-5b-non-static-member-initialization/)

learncpp [8.5a — Constructor member initializer lists](https://www.learncpp.com/cpp-tutorial/8-5a-constructor-member-initializer-lists/)

learncpp [11.4 — Constructors and initialization of derived classes](https://www.learncpp.com/cpp-tutorial/114-constructors-and-initialization-of-derived-classes/)

learncpp [10.7 — std::initializer_list](https://www.learncpp.com/cpp-tutorial/10-7-stdinitializer_list/)

learncpp [9.12 — Copy initialization](https://www.learncpp.com/cpp-tutorial/9-12-copy-initialization/)