# Named requirements

前面已经对named requirements进行了详细介绍。

## Named requirement is behavior-based

其实本节标题的"Named requirement is behavior based"是和"Template-is-behavior-based"章节的观点是重复的，我在此处添加的目的仅仅是为了提示。

可以看到cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)中描述的各种type requirement的Requirements要求object of the type能够满足各种**Expression**(可以对type、object执行哪些operation)，这其实就是behavior-based。

uniform syntax

各个concept都有对应的syntax

uniform interface

各个concept都有对应的standard library interface

uniform concept

C++提供了concept来对这些named requirement进行校验

## cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)

The *named requirements* listed on this page are the named requirements used in the normative text of the C++ standard to define the expectations of the **standard library**.

> NOTE: 主要用于standard library。

Some of these requirements are being formalized in C++20 using the [concepts](https://en.cppreference.com/w/cpp/language/constraints) language feature. Until then(直到那时), the burden is on the programmer to ensure that library templates are instantiated with template arguments that satisfy these requirements. Failure to do so may result in very complex compiler diagnostics.

> NOTE: 
>
> "until then"的意思是: "直到那时"；上面这段话的意思是: 在C++20之前，compiler并不会在template instantiation之前交易template argument是否满足named requirement，而是由programmer来保证。因此，这就可能导致: "Failure to do so may result in very complex compiler diagnostics"，这是C++20 concept着力解决的问题之一。

### Basic



### Type properties



### Library-wide



| concept                                                      |      | chapter                                                      |
| ------------------------------------------------------------ | ---- | ------------------------------------------------------------ |
| [Swappable](https://en.cppreference.com/w/cpp/named_req/Swappable) (C++11) |      | `C-family-language\C++\Idiom\OOP\Resource-management\Non-throwing-swap` |
|                                                              |      |                                                              |

### Container

#### Container element

### Iterator