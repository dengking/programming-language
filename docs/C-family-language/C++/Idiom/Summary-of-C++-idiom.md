# Summary of C++ idiom

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