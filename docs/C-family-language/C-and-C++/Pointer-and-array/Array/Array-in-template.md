# Array in template

本文讨论在array在template中的一些列问题。



## Parameter passing

Function template的template parameter的template argument是一个array，在这种情况下就涉及到array的parameter passing问题，在前面的章节已知，array的parameter passing的方式有:

1) by value: array to pointer decay

2) by reference

本文将结合具体的例子来对这两种方式进行分析。

### stackoverflow [why does the array decay to a pointer in a template function](https://stackoverflow.com/questions/7797839/why-does-the-array-decay-to-a-pointer-in-a-template-function)

I don't understand why the array decays to a pointer in a template function.

> NOTE: 这篇文章的原问题所讨论的是"array decay to a pointer"，在 stackoverflow [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c) 中已经对此进行了探讨；我所关注的将array用于function template时，可以使用的两种 parameter passing的写法，以及两种写法的差异；
>
> 我是在阅读 stackoverflow [What is std::decay and when it should be used?](https://stackoverflow.com/questions/25732386/what-is-stddecay-and-when-it-should-be-used) 的 [回答](https://stackoverflow.com/a/25732651) 的**评论**:
>
> > It is **array decay**. For example: `template<typename T> void f(T&);` `f("abc");` `T` is `char(&)[4]`, but `template<typename T> void f(T);` `f("abc");` `T` is `char*`; You can also find an explanation here: [stackoverflow.com/questions/7797839/…](https://stackoverflow.com/questions/7797839/why-does-the-array-decay-to-a-pointer-in-a-template-function) 
>
> 发现的这篇文章。
>
> 显然，当pass-by-reference的时候，是不会发生 **array to pointer decay** 的。上面的 **评论** 已经描述了这种情况了。

```C++
#include <iostream>

template<class T>
void f(T buff)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << "f:buff size:" << sizeof(buff) << std::endl;       //prints 4
}

template<class T>
void f1(T &buff)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << "f:buff size:" << sizeof(buff) << std::endl;       //prints 3
}

int main(int argc, char *argv[])
{
	const char buff[3] = { 0, 0, 0 };
	std::cout << "buff size:" << sizeof(buff) << std::endl;         //prints 3
	f(buff);
	f1(buff);
	return 0;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```C++
> buff size:3
> void f(T) [with T = const char*]
> f:buff size:8
> void f1(T&) [with T = const char [3]]
> f:buff size:3
> ```

[A](https://stackoverflow.com/a/7797872)

Because arrays can not be passed by value as a function parameter.
When you pass them by value they decay into a pointer.

In this function:

```cpp
template <class T>
void f(T buff) {
```

T can not be `char (&buff)[3]` as this is a reference. The compiler would have tried `char (buff)[3]` to pass by value but that is not allowed. So to make it work arrays decay to pointers.

Your second function works because here the array is passed by reference:

```cpp
template <class T>
void f1(T& buff) {

// Here T& => char (&buff)[3]
```

[A](https://stackoverflow.com/a/7798060)



