# ixodoi.expertscolumn [C++ Programming: Void* Vs. Templates](http://ixodoi.expertscolumn.com/article/c-programming-void-vs-templates)

> NOTE: 
>
> 1、`void*`就是一种典型的type erasure
>
> 2、不同programming language实现generic programming: 
>
> `void*`是c的generic programming。
>
> duck type是python的generic programming。
>
> template是`c++`的generic programming。



**Everyone that works with Template knows how powerful there are. However, many times when I encounter a C programmer that just starting to learn / work with C++ I ran into the same question: Why is Template better than Void\*?**

> NOTE: 这也是我的疑问

I believe the answer is quite simple and I will try to explain it in this post, but first one must understand the fundamental differences between the two methods.

## What is `void*`?

`void*` is a mechanism in the C programming language that allows you to write a general code. I.e. you can write general functions (or structures) that receive (contain) pointer to Void and actually send whatever type you want to it. Void pointers are often used to allow functions to operate on data of an unknown type.

## What is Template?

Template is a mechanism in the C++ programming that allows you to create a collection of functions and containers that operate on typed data! I.e. each individual function / container created from the template is working with a very specific type. The generic of the code is rising from the fact that many different typed functions / containers can be created from the same template.

It is important to understand that the `void*` is not a C++ mechanism and the only reason you can use it in C++ is because C++ has a back-support to all C features. However, the fact you can use it does not mean you should! Especially since Template is much better than `void*`.

## The main differences between Template and Void\*

1、Using `void*` we have only one instance of the code. With Templates, on the other hand, we have several versions of the code, since the compiler creates a separate version of the function for each specified type

2、Unlike with Templates, when using `void*`, the compiler cannot distinguish types. Therefore:

\- The compiler can not perform type checking.

\- We can not perform type specific operations such as operators or inner methods.

\- We can not force two general parameters (or more) to be of the same type.

### Template advantages

 1、 Easier to write. You create only one generic version of your function and there is no need for switch of casting (unlike `void*`).

2、Templates provide a straightforward way of abstracting type information.

3、Templates are type-safe. Template is a compiler time mechanism. Therefore, the types are known at compile time and all error are discovered then (unlike with the `void*` mechanism that cause run time errors).

### Template disadvantage:

The fact that Template is a compiler-time mechanism is one of Templates greatest advantage. However it is also the cause to its disadvantage.

In Templates, the moment we create a specific container from the template for type `X` – this container will only hold instances of type `X`. This is unlike with `void*` that allows the user to enter completely different types into the same container.

However, if we want to enter different types with a common general idea into the same container we can always go around it by using polymorphism.