# Default constructors

## cppreference [Default constructors](https://en.cppreference.com/w/cpp/language/default_constructor)

A default constructor is a [constructor](https://en.cppreference.com/w/cpp/language/constructor) which can be called with no arguments (either defined with an empty parameter list, or with default arguments provided for every parameter). A type with a public default constructor is [*DefaultConstructible*](https://en.cppreference.com/w/cpp/named_req/DefaultConstructible).

> NOTE: 
>
> 一、在wikipedia [C++11#Modification to the definition of plain old data](https://en.wikipedia.org/wiki/C++11#Modification_to_the_definition_of_plain_old_data) 中提及了**statically initialized**，这让我想起了这样的问题：
>
> [*DefaultConstructible*](https://en.cppreference.com/w/cpp/named_req/DefaultConstructible) 能够被  **statically initialized**？

### Syntax



### Implicitly-declared default constructor

> NOTE: 
>
> 一、这一段 和 下面的 "Implicitly-defined default constructor"都是在描述compiler默认生成的default constructor

If no user-declared constructors of any kind are provided for a class type (struct, class, or union), the compiler will always declare a default constructor as an `inline public` member of its class.

#### (since C++11)

If some user-declared constructors are present, the user may still force the automatic generation of a default constructor by the compiler that would be implicitly-declared otherwise with the keyword `default`.

> NOTE: 
>
> 一、典型的例子就是为了使一个类型成为trivial type，为其添加defaulted default constructor，参见 microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019)

### Implicitly-defined default constructor

> NOTE: 
>
> 一、这一段描述的是Implicitly-defined default constructor的implementation细节

If the implicitly-declared default constructor is not defined as deleted, it is defined (that is, a function body is generated and compiled) by the compiler if [odr-used](https://en.cppreference.com/w/cpp/language/definition#ODR-use) or [needed for constant evaluation](https://en.cppreference.com/w/cpp/language/constant_expression#Functions_and_variables_needed_for_constant_evaluation) (since C++11), and it has the same effect as a user-defined constructor with empty body and empty initializer list. That is, it calls the default constructors of the bases and of the non-static members of this class.

> NOTE: 
>
> 上面描述了它的implementation细节

### Deleted implicitly-declared default constructor

> NOTE: 
>
> 一、描述了哪些情况下compiler无法生成default constructor

### Trivial default constructor

> NOTE: 
>
> 这在 `Trivial` 章节中进行了讨论

### Eligible default constructor(since C++20)

> NOTE: 
>
> 暂未学习

