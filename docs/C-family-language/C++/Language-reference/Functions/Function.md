# Function

本文介绍C++中function的概念，以及与此相关的内容。C++是复杂的，这体现在它的function上，我们将看到C++中function的内容是非常繁杂的。

## Classification of function

本节描述C++ function的分类:

### 1) 基于programming paradigm

上面的分类方法是采用的在`C++\Language-reference\Basic-concept\index.md#Extend to OOP`中总结的思路:

> 首先描述Non-OOP，然后描述OOP

| 类别    |                                                              | 章节                                                         |
| ------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| OOP     | [Member functions](https://en.cppreference.com/w/cpp/language/member_functions) | `C++\Language-reference\Classes\Members\Non-static-member\Function-member` |
| Non-OOP | free function                                                |                                                              |



### 2) 是否有名

| 类别             |                                                              | 章节                                                 |
| ---------------- | ------------------------------------------------------------ | ---------------------------------------------------- |
| named function   |                                                              |                                                      |
| unnamed function | [lambda-expressions](https://en.cppreference.com/w/cpp/language/lambda) | `C++\Language-reference\Functions\Lambda-expression` |

### 3) 其他

[Friend](https://en.cppreference.com/w/cpp/language/friend) function

## Callable 

本节标题的callable是指C++中支持function call expression的，它包括: 

| 类别                  | explanation                                                  | 章节                                                         |
| --------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| function lvalue       | function is not object                                       |                                                              |
| reference to function | reference不是object，所以它无法归入function object中         |                                                              |
| function object       | cppreference [FunctionObject](https://en.cppreference.com/w/cpp/named_req/FunctionObject) | [Function objects](https://en.cppreference.com/w/cpp/language/functions#Function_objects) |



## cppreference [Functions](https://en.cppreference.com/w/cpp/language/functions)

Functions are C++ entities that associate a sequence of [statements](https://en.cppreference.com/w/cpp/language/statements) (a *function body*) with a *name* and a list of zero or more *function parameters*.

> NOTE: 有一点需要注意的是，function的value category是rvalue，这在cppreference [Value categories](https://en.cppreference.com/w/cpp/language/value_category)中是由特殊说明的，这是因为function满足: `i`特性，即"has identity"，参见`C++\Language-reference\Expressions\Value-categories`，但是"Functions are not objects"。

### Termination

> NOTE: 函数终止，参见`./Function-termination`章节。

A function can terminate by [returning](https://en.cppreference.com/w/cpp/language/return) or by [throwing](https://en.cppreference.com/w/cpp/language/throw) an [exception](https://en.cppreference.com/w/cpp/language/exceptions).



### [Coroutine](https://en.cppreference.com/w/cpp/language/coroutines) is a kind of function

> NOTE: 符合uniform function model，参见 `C++\Language-reference\Basic-concept\index.md` 
>
> 暂时还没有学习coroutine。



### [Function declaration](https://en.cppreference.com/w/cpp/language/function) and [function definition](https://en.cppreference.com/w/cpp/language/function) 

> NOTE: 参见`C++\Language-reference\Functions\Function-declaration`。
>
> 将对能够用于function declaration的所有specifier进行说明。

### Functions are not objects

> NOTE: 在`C++\Language-reference\Basic-concept\index.md`的"Thoughts"段中也进行了说明。

Functions are not objects: there are no arrays of functions and functions cannot be **passed by value** or returned from other functions. Pointers and references to functions are allowed, and may be used where functions themselves cannot.

> NOTE: 由于function is not object，因此在C++中，**function is not first class citizen**，但是C++中，提供了walkround让可以间接地实现: **function is not first class citizen**，这将在后面进行详细说明。
>
> [Implicit conversions#Function to pointer](https://en.cppreference.com/w/cpp/language/implicit_conversion#Function_to_pointer) 

### [Function overloading](https://en.cppreference.com/w/cpp/language/overload_resolution)

> NOTE: 这是C++的重要特性，参见`C++\Language-reference\Functions\Function-overload`



### Function type

> NOTE: 函数也是有类型的

Each function has a type, which consists of the function's **return type**, the types of all parameters (after array-to-pointer and function-to-pointer transformations, see [parameter list](https://en.cppreference.com/w/cpp/language/function#Parameter_list)) , whether the function is [`noexcept`](https://en.cppreference.com/w/cpp/language/noexcept_spec) or not (since C++17), and, for non-static member functions, cv-qualification and ref-qualification (since C++11). 

> NOTE: 上面这段话的意思是，function type是由如下组成的:
>
> - return type
> - types of all parameters
> - whether the function is [`noexcept`](https://en.cppreference.com/w/cpp/language/noexcept_spec) or not (since C++17)
> - for non-static member functions, cv-qualification and ref-qualification (since C++11) (free function是没有cv-qualification和ref-qualification的)
>
> 需要注意的是: function overload是不考虑return type的，这和function type不同。

Function types also have [language linkage](https://en.cppreference.com/w/cpp/language/language_linkage). 

> NOTE: 关于language linkage，参见`C-and-C++\From-source-code-to-exec\Link\Linkage\Language-linkage`。

There are no cv-qualified function types (not to be confused with the types of [cv-qualified functions](https://en.cppreference.com/w/cpp/language/member_functions) such as `int f() const;` or functions returning [cv-qualified types](https://en.cppreference.com/w/cpp/language/cv), such as [std::string](http://en.cppreference.com/w/cpp/string/basic_string) `const f();`). Any cv-qualifier is ignored if it is added to an alias for a function type.

> NOTE: 对于function而言，它的CV是完全没有语义的，我们需要从"Function and data model"来进行理解，function位于memory中，它们对应的是instruction，显然，修改instruction是没有任何意义的。



### [Function objects](https://en.cppreference.com/w/cpp/language/functions#Function_objects)

> NOTE: 参见`C++\Language-reference\Functions\First-class-function\Function-objects`章节。











