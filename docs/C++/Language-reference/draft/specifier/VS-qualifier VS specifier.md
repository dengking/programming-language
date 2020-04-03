[TOC]



# qualifier and specifier

specifier的含义是：[计] 说明符；指示语

qualifier的含义是：限定符、限定词、修饰符、修饰词

其实从两者的含义就可以看出它们之间的区别了：specifier用于指定到底是`function`，还是`struct`，而qualifier则是为了在添加一些限制；

如c中的[Declarations](https://en.cppreference.com/w/c/language/declarations)：

`specifiers-and-qualifiers declarators-and-initializers;`







# [Correlation between specifier and qualifier?](https://stackoverflow.com/questions/3610943/correlation-between-specifier-and-qualifier)



## [A](https://stackoverflow.com/a/3611082)

Most of it doesn't make sense.

*Specifier* and *qualifier* are defined in the C++ standard. *Qualifier* is just an integral part of a *specifier*. For example, *type specifier* in a declaration can include *cv-qualifiers*. I don't see the reason to quote everything from the standard on this topic.

Cv-qualifiers are not restricted to lvalues. Rvalues of *class* types can also be cv-qualified. It is possible to cv-qualify an rvalue of non-class type, but it will have no effect and will be ignored.

The use of `const` qualifier that you show in your example with `foo` is just a syntactic form, which actually means that the const-qualifier is applied to the implied `this` parameter of the `foo` method: `const A* this`. I.e. in this case it does indeed qualify an lvalue, but it is `*this`, not `foo`.

The term *qualifier* also appears in the context of *qualified names*. Name like `some_class::some_member` (or `some_namespace::some_name`) are called *qualified names* and the `some_class::` part is a *qualifier*.

The idea that if something is an lvalue then you can modify it is totally incorrect. There are *modifiable*lvalues and *non-modifiable* lvalues. An object declared as `const int i = 5` is an lvalue, yet you can't modify it. Ordinary functions are also lvalues in C++, yet you can't modify a function.