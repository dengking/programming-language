# Non-local object

对于non-local object，cppreference是在[Initialization#Non-local variables](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables)章节进行描述的。

## cppreference [Initialization#Non-local variables](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables)

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

### Static initialization

> NOTE: static initialization的含义是什么？它是发生在compile-time吗？后面有专门的章节对它进行介绍



### Dynamic initialization



### Early dynamic initialization



### Deferred dynamic initialization



### End

在cppreference [Initialization#Notes](https://en.cppreference.com/w/cpp/language/initialization#Notes)章节，有这样的描述: 

> The order of destruction of non-local variables is described in [std::exit](https://en.cppreference.com/w/cpp/utility/program/exit).