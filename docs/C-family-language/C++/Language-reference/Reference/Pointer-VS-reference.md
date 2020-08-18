# Pointer VS reference



## [What are the differences between a pointer variable and a reference variable in C++?](https://stackoverflow.com/questions/57483/what-are-the-differences-between-a-pointer-variable-and-a-reference-variable-in)

> NOTE: 这篇文章的总结是非常棒的，优先阅读[这个回答](https://stackoverflow.com/a/596750)，因为它是概括性的，然后阅读[这个回答](https://stackoverflow.com/a/57492)，因为它枚举了非常多的具体情况。



**Pointer is an object, while reference is not**, 这一点是在阅读cppreference [Pointer declaration#Pointer to function](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_functions)中下面这一段时受启发而想到的:

> Unlike functions or references to functions, pointers to functions are objects and thus can be stored in arrays, copied, assigned, etc.

在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)中，也对此进行了说明:

> The following entities are not objects: value, reference, function