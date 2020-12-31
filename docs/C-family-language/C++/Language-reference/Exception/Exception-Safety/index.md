# Exception safety



## wikipedia [Exception safety](https://en.wikipedia.org/wiki/Exception_safety)

There are several levels of exception safety (in decreasing order of safety):

### No-throw guarantee

also known as **failure transparency**: Operations are guaranteed to succeed and satisfy all requirements even in exceptional situations. If an exception occurs, it will be handled internally and not observed by clients.

### Strong exception safety

also known as **commit or rollback semantics**: Operations can fail, but failed operations are guaranteed to have no side effects, leaving the original values intact(原封不动).[[4\]](https://en.wikipedia.org/wiki/Exception_safety#cite_note-4)

### Basic exception safety

also known as a **no-leak guarantee**: Partial execution of failed operations can result in side effects, but all [invariants](https://en.wikipedia.org/wiki/Invariant_(computer_science)) are preserved and there are no [resource leaks](https://en.wikipedia.org/wiki/Resource_leak) (including [memory leaks](https://en.wikipedia.org/wiki/Memory_leak)). Any stored data will contain valid values which may differ from the original values.

### No exception safety

No guarantees are made.



Usually, at least basic exception safety is required to write robust code in such languages. Higher levels of safety can sometimes be difficult to achieve, and might incur an overhead due to extra copying. A key mechanism for exception safety is a `finally` clause, or similar [exception handling syntax](https://en.wikipedia.org/wiki/Exception_handling_syntax), which ensure that certain code is *always* run when a block is exited, including by exceptions. Several languages have constructs that simplify this, notably using the [dispose pattern](https://en.wikipedia.org/wiki/Dispose_pattern), named as `using`, `with`, or `try`-with-resources.

> NOTE: 这其实和resource management密切相关的

## microsoft [How to: Design for exception safety](https://docs.microsoft.com/en-us/cpp/cpp/how-to-design-for-exception-safety?view=vs-2019)



## boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html)

> NOTE: 这篇文章非常好

### Abstract

> NOTE: 这一节描述了本文的abstract，其中提及的“automated testing procedure for verifying exception-safety”是需要了解的。

### 1 What is exception-safety?



More formally, we can describe a component as minimally exception-safe if, when exceptions are thrown from within that component, its invariants are intact. 

> NOTE: 第一次阅读这段话的时候，没有搞清楚它的含义，其实它所说的就是[**Basic exception safety**](https://en.wikipedia.org/wiki/Exception_safety)。

### 2 Myths and Superstitions

Exception-safety seems straightforward so far: it doesn't constitute anything more than we'd expect from code using more traditional error-handling techniques.

> NOTE: 这段话的意思是：异常安全到目前为止似乎很简单:它所构成的内容并不比我们使用更传统的错误处理技术所期望的代码更多。
>
> 关于这段话中的“ traditional error-handling techniques”的含义，参见维基百科[Exception handling](https://en.wikipedia.org/wiki/Exception_handling)。

It's almost as though exceptions are viewed as a *mysterious attack* on otherwise correct code, from which we must protect ourselves. Needless to say, this doesn't lead to a healthy relationship with error handling! During standardization, a democratic process which requires broad support for changes, I encountered many widely-held superstitions. In order to even begin the discussion of exception-safety in generic components, it may be worthwhile confronting a few of them.

> NOTE: 这就好像异常被看作是对正确代码的神秘攻击，我们必须保护自己。
> 不用说，这并不会导致错误处理的健康关系!
> 标准化是一个需要广泛支持变革的民主过程，在标准化过程中，我遇到了许多广为流传的迷信。
> 为了开始讨论通用组件中的异常安全性，有必要面对其中的一些问题。



## How to improve exception safety? 

我们需要考虑的是如何来improve exception safety。

### Preallocate

#### Copy and swap idiom

Copy and swap idiom能够提供strong exception safety:

> Get the new data ready before we replace the old

即在修改正确，先将需要的resource准备好。

参见 `C++\Idiom\OOP\Copy-and-swap` 章节。

#### Placement new





### Stack unwind

参见`C++\Idiom\OOP\Scope-Guard`章节 。