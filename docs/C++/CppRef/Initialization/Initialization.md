# Initialization



## cppreference [Initialization](https://en.cppreference.com/w/cpp/language/initialization)

*Initialization* of a variable provides its initial value at the time of **construction**.

> NOTE: initialization发生在object construction阶段，关于object的lifetime，参见`C++\Language-reference\Basic-concept\Data-model\Object\Object-storage-duration-and-lifetime`。

### Initializer

> NOTE: 
>
> 一、提供了"initializer"其实就是带参的initialization
>
> 二、Terminology: initializer
>
> "initializer"是cppreference中用来表示**初始值**的术语，在下面文章中使用了这个术语: 
>
> - cppreference [new expression](https://en.cppreference.com/w/cpp/language/new)
> - cppreference [Declarators](https://en.cppreference.com/w/cpp/language/declarations#Declarators) 

The initial value may be provided in the **initializer** section of a [declarator](https://en.cppreference.com/w/cpp/language/declarations) or a [new expression](https://en.cppreference.com/w/cpp/language/new).

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

> NOTE: 
>
> 一、原文的这部分内容其实非常好的对C++ initialization进行了分类，它的分类标准是: 是否提供了initializer:
>
> 1、提供了initializer
>
> 需要注意的是，"initializer"和我们平时所说的"参数"并不能画等号，典型的例子就是: `std::string s{};`，它并没有参数，但是它有initializer `{}`。
>
> 2、没有提供initializer
>
> "If no initializer is provided, the rules of [default initialization](https://en.cppreference.com/w/cpp/language/default_initialization) apply."
>
> 

### Non-local variables

> NOTE: 原文的这部分内容放到了 `Non-local-variables-initialization` 章节中

### Static local variables

For initialization of locals (that is, block scope) static and thread-local variables, see [static local variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables).

> NOTE: 
>
> 一、参见: 
>
> `C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Static-storage-duration\Static-local-object`

### Class members

Non-static data members can be initialized with [member initializer list](https://en.cppreference.com/w/cpp/language/initializer_list) or with a [default member initializer](https://en.cppreference.com/w/cpp/language/data_members#Member_initialization).

## 其他initialization

下面是一些其他initialization: 

|                                                              |                                                              |                |
| ------------------------------------------------------------ | ------------------------------------------------------------ | -------------- |
| [Zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization) | Sets the initial value of an object to zero.                 |                |
| [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization) | This is the initialization performed when a variable is constructed with no initializer. | no initializer |
| [Constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization) | Sets the initial values of the [static](https://en.cppreference.com/w/cpp/language/storage_duration) variables to a compile-time constant. |                |


