# Strict aliasing



## cppreference `Object#Strict aliasing`

Accessing an object using an expression of a type other than the type with which it was created is **undefined behavior** in many cases, see [reinterpret_cast](reinterpret_cast.html#Type_aliasing) for the list of exceptions and examples.



## creference `Objects and alignment#Strict aliasing`

Given an object with *effective type* `T1`, using an lvalue expression (typically, **dereferencing a pointer**) of a different type `T2` is **undefined behavior**

> NOTE: 
>
> 下面是展示“**dereferencing a pointer**”的一个简单的例子，这个例子源自`creference Objects and alignment#Strict aliasing`: 
>
> ```c++
> #include <cstdio>
> 
> int main()
> {
> 	int i = 7;
> 	float* pf = (float*) (&i);
> 	float d = *pf; // UB: float lvalue *pf cannot be used to access int
> }
> 
> ```



## Why?

### Alignment requirement

在`C-family-language\C-and-C++\From-source-code-to-exec\ABI\Alignment`中对alignment进行了深入分析。

如果不遵循alignment，则会导致非常严重的问题，在`C-family-language\C-and-C++\From-source-code-to-exec\ABI\Alignment`给出了example。

### Compiler optimization

对于没有遵循strict aliasing的program，compiler可能会进行错误的optimization，进而导致生成非常诡异的程序，因为它是undefined的。关于此的一个例子是quarkslab [Unaligned accesses in C/C++: what, why and solutions to do it properly](https://blog.quarkslab.com/unaligned-accesses-in-cc-what-why-and-solutions-to-do-it-properly.html) 。

