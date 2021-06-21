# [Understanding Ownership](https://doc.rust-lang.org/book/ch04-00-understanding-ownership.html#understanding-ownership)

> NOTE: 
>
> 一、rust在语言层使用了ownership概念，它是rust的核心特性；相比之下，C++的ownership，并没有出现在语言层，相比之下，弱了很多
>
> 

Ownership is Rust’s most unique feature, and it enables Rust to make memory safety guarantees without needing a garbage collector. Therefore, it’s important to understand how ownership works in Rust. In this chapter, we’ll talk about ownership as well as several related features: borrowing, slices, and how Rust lays data out in memory.

## [What Is Ownership?](https://doc.rust-lang.org/book/ch04-01-what-is-ownership.html#what-is-ownership)

Rust’s central feature is *ownership*. Although the feature is straightforward to explain, it has deep implications for the rest of the language.

All programs have to manage the way they use a computer’s memory while running. Some languages have garbage collection that constantly looks for no longer used memory as the program runs; in other languages, the programmer must explicitly allocate and free the memory. Rust uses a third approach: memory is managed through a system of ownership with a set of rules that the compiler checks at compile time. None of the ownership features slow down your program while it’s running.

> NOTE: 
>
> 一、rust的ownership是可以在compile time进行check的，也就是说，是static的

## [References and Borrowing](https://doc.rust-lang.org/book/ch04-02-references-and-borrowing.html#references-and-borrowing)

