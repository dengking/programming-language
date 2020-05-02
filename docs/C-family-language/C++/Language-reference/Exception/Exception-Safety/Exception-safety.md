# Exception safety



## 维基百科[Exception safety](https://en.wikipedia.org/wiki/Exception_safety)

There are several levels of exception safety (in decreasing order of safety):

- **No-throw guarantee**
- **Strong exception safety**
- **Basic exception safety**
- **No exception safety**



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

