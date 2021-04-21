# Non-local object

1、对于non-local object，cppreference是在[Initialization#Non-local variables](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables)章节进行描述的。

2、在 pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/) 中，其实已经涵盖了cppreference [Initialization#Non-local variables](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables) 中的内容

## cppreference [Initialization#Non-local variables](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables)

> NOTE: 首先需要搞清楚non-local variable的概念，参见
>
> - `Theory\Resource-management\Memory-management\Variable`章节
>
> - `C-family-language\C++\Language-reference\Basic-concept\Data-model\Object\Storage.md`章节
>
> 

All **non-local variables** with static [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) are initialized as part of program startup, before the execution of the [main function](https://en.cppreference.com/w/cpp/language/main_function) begins (unless deferred, see below). 

All non-local variables with thread-local storage duration are initialized as part of thread launch, sequenced-before the execution of the thread function begins. 

For both of these classes of variables, initialization occurs in two distinct stages:

### Static initialization

> NOTE: static initialization的含义是什么？它是发生在compile-time吗？是的



### Dynamic initialization

> NOTE: 
>
> 可以看到，dynamic initialization的order是非常复杂的，且存在着unordered，如果program依赖于这个order，则可能导致一些问题。
>
> 关于ordered、unordered，参见`C++\Guide\Order`。

After all static initialization is completed, dynamic initialization of non-local variables occurs in the following situations:

1) *Unordered dynamic initialization*,

2) *Partially-ordered dynamic initialization*, (since C++17)

3) *Ordered dynamic initialization*, which applies to all other non-local variables: 

Within a single translation unit, initialization of these variables is always [sequenced](https://en.cppreference.com/w/cpp/language/eval_order) in exact order their definitions appear in the source code.

> NOTE: ordered

Initialization of static variables in different translation units is indeterminately sequenced. 

> NOTE: unordered

Initialization of thread-local variables in different translation units is unsequenced.

> NOTE: unordered



> NOTE: 
>
> 显然，initialization order是建立在translation unit上的一个规则，参见`C-and-C++\From-source-code-to-exec\Phases-of-translation\Separate-compilation-model`。
>
> 其实上述规则可以简单地总结如下: 
>
> translation unit之内: ordered
>
> translation unit之间: unordered
>
> 上面提及的unordered这是static initialization order fiasco的根源；由于这种unordered是out of control ，因此programmer必须要使用一种可以control的替换方案。
>
> 这是解决这类问题的主要思路。



### Early dynamic initialization



### Deferred dynamic initialization



### End

在cppreference [Initialization#Notes](https://en.cppreference.com/w/cpp/language/initialization#Notes)章节，有这样的描述: 

> The order of destruction of non-local variables is described in [std::exit](https://en.cppreference.com/w/cpp/utility/program/exit).