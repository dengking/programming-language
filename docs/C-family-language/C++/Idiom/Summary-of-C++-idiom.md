# Summary of C++ idiom

## 一些基础的idiom

RAII、swap、temporary

## Idiom about resource

[RAII](./RAII/RAII.md)和[The-rule-of-three-five-zero](./The-rule-of-three-five-zero/The-rule-of-three-five-zero.md)其实都是resource management的一些技巧，它能够保住使用者规避一些错误。

在[What is the copy-and-swap idiom?](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom)中有这样的描写：

> In order to use the copy-and-swap idiom, we need three things: a working copy-constructor, a working destructor (both are the basis of any wrapper, so should be complete anyway), and a `swap` function.

copy-and-swap idiom依赖于：

- copy constructor

[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body)<-[Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)<- [Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap) 





### 对resource的operation

c++ idiom中有多个关于resource的idiom，可以看到这些idiom都充分利用copy、move、swap、destruct。



### Idiom about pointer

[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body)->Smart Pointer 

## Idiom about template

Curiously recurring template pattern

expression template



### Idiom about virtual

Virtual Constructor

Virtual Friend Function

## Idiom concerning temporary

temporary的特性：自动创建、自动销毁。

使用了temporary的idiom、消除temporary的idiom。

### [Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap)

Create-Temporary-and-Swap

这是显然的应用temporary。



### [Clear-and-minimize](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Clear-and-minimize)

This is sometimes called the ***swap with temporary idiom***.

显然它是依赖于：temporary、swap，其实它可以看做是将swap idiom用于container上。





### [Temporary Base Class](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Temporary_Base_Class)



### [Temporary Proxy](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Temporary_Proxy) 



### [Execute-Around Pointer](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Execute-Around_Pointer)

每个aspect的，其实都相当于一个temporary。

> Overloaded -> operator of visualizableVector creates a temporary proxy object and it is
> returned.

### [Computational Constructor](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Computational_Constructor)

消除temporary



