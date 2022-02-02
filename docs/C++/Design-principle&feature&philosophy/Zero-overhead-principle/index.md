# Zero-overhead principle

1、C++的design是遵循Zero-overhead principle，作为programmer，在开发的时候，也需要遵循zero overhead principle，即"You don't pay for what you don't use"

2、C++ Zero-overhead principle其实就是optimization principle



## isocpp [Big Picture Issues](https://isocpp.org/wiki/faq/big-picture) # [What is the zero-overhead principle?](https://isocpp.org/wiki/faq/big-picture#zero-overhead-principle)

The zero-overhead principle is a guiding principle for the design of C++. It states that: *What you don’t use, you don’t pay for* (in time or space) and further: *What you do use, you couldn’t hand code any better.*

In other words, no feature should be added to C++ which would make any existing code (not using the new feature) larger or slower, nor should any feature be added for which the compiler would generate code that is not as good as a programmer would create without using the feature.

## stackoverflow [What does 'Zero Cost Abstraction' mean?](https://stackoverflow.com/questions/69178380/what-does-zero-cost-abstraction-mean)

[A](https://stackoverflow.com/a/69178445/10173843)

Zero Cost Abstractions means adding higher-level programming concepts, like generics, collections and so on do not come with a run-time cost, only compiler time cost (the code will be slower to compile). Any operation on zero-cost abstractions is as fast as you would write out matching functionality by hand using lower-level programming concepts (for loops, counters, ifs and so on).

> NOTE: 
>
> 上面这段话是从compile time和run time来进行分析的，显然使用compile time实现的concept是"zero cost abstraction"

## cppreference [Zero-overhead principle](https://en.cppreference.com/w/cpp/language/Zero-overhead_principle)



## 例证

一、C++ data model: object的memory不会占用超过data member，C++没有额外导致它的扩大

二、static polymorphism

## 素材

quora [**What are zero-cost abstractions in programming languages?**](https://www.quora.com/What-are-zero-cost-abstractions-in-programming-languages)