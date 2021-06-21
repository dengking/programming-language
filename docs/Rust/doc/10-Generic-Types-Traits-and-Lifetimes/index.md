# [Generic Types, Traits, and Lifetimes](https://doc.rust-lang.org/book/ch10-00-generics.html#generic-types-traits-and-lifetimes)

Every programming language has tools for effectively handling the duplication of concepts. In Rust, one such tool is *generics*. Generics are abstract stand-ins for concrete types or other properties. When we’re writing code, we can express the behavior of generics or how they relate to other generics without knowing what will be in their place when compiling and running the code.

> NOTE:
>
> 一、rust通过generic来实现generic programming
>
> 二、"stand-in"其实就是placeholder

