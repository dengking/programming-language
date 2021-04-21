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

Because arrays can not be passed by value as a function parameter. When you pass them by value they decay into a pointer.

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



It is because *arrays* cannot be passed *by value* to a function. So in order to make it work, the array decays into a pointer which then gets passed to the function *by value*.

In other words, passing an array by value is akin to *initializing* an array with another array, but in C++ *an array* cannot be *initialized* with another array:

```cpp
char buff[3] = {0,0,0};
char x[3] = buff; //error 
```

So if an array appears on the right hand side of `=`, the left hand side has to be either `pointer` or `reference` type:

```cpp
char *y = buff; //ok - pointer
char (&z)[3] = buff; //ok - reference
```

Demo : http://www.ideone.com/BlfSv

It is exactly for the same reason `auto` is inferred differently in each case below (note that `auto` comes with C++11):

```cpp
auto a = buff; //a is a pointer - a is same as y (above)
std::cout << sizeof(a) << std::endl; //sizeof(a) == sizeof(char*)

auto & b = buff; //b is a reference to the array - b is same as z (above)
std::cout << sizeof(b) << std::endl; //sizeof(b) == sizeof(char[3])
```

Output:

```cpp
4 //size of the pointer
3 //size of the array of 3 chars
```

Demo : http://www.ideone.com/aXcF5



### C string 

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
	f("abc");
	f1("abc");
	return 0;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序的输出如下:
>
> ```C++
> void f(T) [with T = const char*]
> f:buff size:8
> void f1(T&) [with T = const char [4]]
> f:buff size:4
> ```
>
> 

### Pass-by-reference example

code-examples [Why is value taking setter member functions not recommended in Herb Sutter's CppCon 2014 talk(Back to Basics: Modern C++ Style)?](https://code-examples.net/en/q/190b60f) 

```C++
#include<array>
#include<string>

template<class C>
struct string_view
{
// known continuous memory containers:
	template<std::size_t N>
	string_view(const C (&arr)[N]) :
					string_view(arr, arr + N)
	{
	}
};	
```

这篇文章收录于`basic_string_view`章节了



[C++ Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure) # Erasing down to a contiguous buffer of T[#](https://sodocumentation.net/cplusplus/topic/2872/type-erasure#erasing-down-to-a-contiguous-buffer-of-t)

```C++
template<class T>
struct array_view
{
	// the core of the class:
	T *b = nullptr;
	T *e = nullptr;
	// array constructor:
	template<std::size_t N>
	array_view(T (&arr)[N]) :
					array_view(arr, N)
	{
	}

};

```



