# [C++ Programming: Void* Vs. Templates](http://ixodoi.expertscolumn.com/article/c-programming-void-vs-templates)

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

