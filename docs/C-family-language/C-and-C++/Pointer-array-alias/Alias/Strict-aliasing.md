# Strict aliasing

在C++、C中，都描述了strict aliasing内容。

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
>
> 

## Why?

alignment requirement。

compiler optimization。