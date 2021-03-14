# Named requirements

前面已经对named requirements进行了详细介绍。

在`Theory\Programming-paradigm\Generic-programming\Type-requirement`的《Specification-of-type-requirement》章节中，已经提及了C++的named requirement，按照其中的思想来理解cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)是非常容易的。

需要注意的是，C++ named requirement是generic programming的concept，在C++20中它们都可以使用concept feature进行formalize。

## Named requirement is behavior-based

其实本节标题的"Named requirement is behavior based"是和"Template-is-behavior-based"章节的观点是重复的，我在此处添加的目的仅仅是为了提示。

可以看到cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)中描述的各种requirement，requirement规定了对type、object of the type可以执行哪些operation/behavior(这其实就是behavior-based)，operation/behavior是通过expression来进行形式化地表达/描述的，对于这些operation/behavior，要么在language层提供了uniform syntax来进行描述，要么在standard library提供了uniform interface/API，（基本上）每种operation/behavior都有对应的**magic function**，显然这是符合**uniform function model**的。除此之外，standard library还提供了trait来对其进行static reflection。

总的来说: 它是基于behavior，而非基于type的。

> NOTE: 
>
> 在 `./Magic-function` 中，对C++ magic function进行了总结。
>
> 关于 **uniform function model** ，参见 `C++\Language-reference\Basic-concept`。

### Syntax

在language层提供了uniform syntax来描述对type、object的operation/behavior，典型的例子是Basic named requirement，每种都有其对应的syntax。

### Interface/API

在standard library中提供了uniform interface/API来描述对type、object的operation/behavior，典型的例子是: 

| Named requirement                                            | Interface/API/Magic function                                 | Trait                                                        |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [*Swappable*](https://en.cppreference.com/w/cpp/named_req/Swappable) | [std::swap](https://en.cppreference.com/w/cpp/algorithm/swap) and the user-defined `swap()` | [std::is_swappable](https://en.cppreference.com/w/cpp/types/is_swappable) |



### Supported operations trait

C++ standard library的`Utility library#Language support#Type support#Type traits#Supported operations`提供了trait来对这些named requirement进行校验。

## cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)

The *named requirements* listed on this page are the named requirements used in the normative text of the C++ standard to define the expectations of the **standard library**.

> NOTE: 主要用于standard library。

Some of these requirements are being formalized in C++20 using the [concepts](https://en.cppreference.com/w/cpp/language/constraints) language feature. Until then(直到那时), the burden is on the programmer to ensure that library templates are instantiated with template arguments that satisfy these requirements. Failure to do so may result in very complex compiler diagnostics.

> NOTE: 
>
> "until then"的意思是: "直到那时"；上面这段话的意思是: 在C++20之前，compiler并不会在template instantiation之前交易template argument是否满足named requirement，而是由programmer来保证。因此，这就可能导致: "Failure to do so may result in very complex compiler diagnostics"，这是C++20 concept着力解决的问题之一。

### Basic

| Named requirement                                            | Expression(uniform syntax) | Trait                                                        |
| ------------------------------------------------------------ | -------------------------- | ------------------------------------------------------------ |
| [DefaultConstructible](https://en.cppreference.com/w/cpp/named_req/DefaultConstructible) | `T u`                      | [std::is_default_constructible](https://en.cppreference.com/w/cpp/types/is_default_constructible) |
| [MoveConstructible](https://en.cppreference.com/w/cpp/named_req/MoveConstructible)(C++11) | `T u = rv;`                | [std::is_move_constructible](https://en.cppreference.com/w/cpp/types/is_move_constructible) |
| [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) | `T u = v;`                 | [std::is_copy_constructible](https://en.cppreference.com/w/cpp/types/is_copy_constructible) |
| [MoveAssignable](https://en.cppreference.com/w/cpp/named_req/MoveAssignable)(C++11) | `t = rv`                   | [std::is_move_assignable](https://en.cppreference.com/w/cpp/types/is_move_assignable) |
| [CopyAssignable](https://en.cppreference.com/w/cpp/named_req/CopyAssignable) | `t = v`                    | [std::is_copy_assignable](https://en.cppreference.com/w/cpp/types/is_copy_assignable) |
| [Destructible](https://en.cppreference.com/w/cpp/named_req/Destructible) | `u.~T()`                   | [std::is_destructible](https://en.cppreference.com/w/cpp/types/is_destructible) |

> NOTE: 每一种都有其对应的magic function

### Type properties



### Library-wide



| Named requirement                                            | Interface/API/Magic function                                 | Trait                                                        |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [*Swappable*](https://en.cppreference.com/w/cpp/named_req/Swappable) | [std::swap](https://en.cppreference.com/w/cpp/algorithm/swap) and the user-defined `swap()` | [std::is_swappable](https://en.cppreference.com/w/cpp/types/is_swappable) |
| [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) | [std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke) | [std::is_invocable](https://en.cppreference.com/w/cpp/types/is_invocable) |


