# cppreference [Function template](https://en.cppreference.com/w/cpp/language/function_template)

> NOTE: 
>
> 1、原文的内容是非常庞杂的，目前还没有完成阅读

## Abbreviated function template(since C++20)

> NOTE: 目的是simplify C++

```C++
void f1(auto); // same as template<class T> void f(T)
void f2(C1 auto); // same as template<C1 T> void f2(T), if C1 is a concept
void f3(C2 auto...); // same as template<C2... Ts> void f3(Ts...), if C2 is a concept
void f4(const C3 auto*, C4 auto&); // same as template<C3 T, C4 U> void f4(const T*, U&);
template <class T, C U>
void g(T x, U y, C auto z); // same as template<class T, C U, C W> void g(T x, U y, W z);
```



## [Function template instantiation](https://en.cppreference.com/w/cpp/language/function_template#Function_template_instantiation)



### Explicit instantiation

```c++
#include <iostream>
template<typename T>
void f(T s)
{
	std::cout << s << '\n';
}

template void f<double>(double); // instantiates f<double>(double)
template void f<>(char); // instantiates f<char>(char), template argument deduced
template void f(int); // instantiates f<int>(int), template argument deduced
int main()
{
	f(1); // calls #3, even though specialization of #1 would be a perfect match
}
// g++ test.cpp

```



### Implicit instantiation

The list of template arguments does not have to be supplied if it can be [deduced](https://en.cppreference.com/w/cpp/language/template_argument_deduction) from context.

> NOTE: 
>
> 1、在下面的"Template argument deduction"对 "deduction" 进行了专门的介绍

```C++
#include <iostream>

template<typename T>
void f(T s)
{
    std::cout << s << '\n';
}

int main()
{
    f<double>(1); // instantiates and calls f<double>(double)
    f<>('a'); // instantiates and calls f<char>(char)
    f(7); // instantiates and calls f<int>(int)
    void (*ptr)(std::string) = f; // instantiates f<string>(string)
}
// g++ test.cpp

```



## Template argument deduction

> NOTE: 
>
> 1、非常重要的一个性质
>
> 2、C++ function template是支持"Template argument deduction"，这在一定程度上simplify C++
>
> 3、Object generator idiom就是利用了这个特性

In order to instantiate a function template, every template argument must be known, but not every template argument has to be specified. When possible, the compiler will deduce the missing template arguments from the function arguments. This occurs when a function call is attempted and when an address of a function template is taken.



## Explicit template arguments

> NOTE: 主要讲述如何获得template argument

Template arguments of a function template may be obtained from

1、template argument deduction

2、default template arguments

3、specified explicitly

> NOTE: template argument只有如上三种来源

There is no way to explicitly specify template arguments to [overloaded operators](https://en.cppreference.com/w/cpp/language/operators), [conversion functions](https://en.cppreference.com/w/cpp/language/cast_operator), and constructors, because they are called without the use of the function name.

> NOTE: magic function的局限性

## Template argument substitution

> NOTE: 前面讲述了如何获得template argument，后续就是执行substitution

### Substitution

When all **template arguments** have been specified, deduced or obtained from default template arguments, every use of a template parameter in the function parameter list is replaced with the corresponding template arguments.

### [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae) 

Substitution failure (that is, failure to replace template parameters with the deduced or provided template arguments) of a function template removes the function template from the [overload set](https://en.cppreference.com/w/cpp/language/overload_resolution). This allows a number of ways to manipulate overload sets using template metaprogramming: see [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae) for details.

### Decay to pointer

After substitution, all function parameters of array and function type are adjusted to pointers and all top-level cv-qualifiers are dropped from function parameters (as in a regular [function declaration](https://en.cppreference.com/w/cpp/language/function#Function_declaration)).

## [Function template overloading](https://en.cppreference.com/w/cpp/language/function_template#Function_template_overloading)

Function templates and non-template functions may be overloaded.

> NOTE: 下面这篇文章对此有着非常好的描述。
>
> 1、ibm [Overloading function templates (C++ only)](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.2.0/com.ibm.zos.v2r2.cbclx01/overloading_function_templates.htm)
>
> You may overload a function template either by a non-template function or by another function template.
>
> Non-template functions take precedence over template functions. The following example describes this:
>
> ```C++
> #include <iostream>
> using namespace std;
> 
> template<class T> void f(T x, T y)
> {
> 	cout << "Template" << endl;
> }
> 
> void f(int w, int z)
> {
> 	cout << "Non-template" << endl;
> }
> 
> int main()
> {
> 	f(1, 2);
> 	f('a', 'b');
> 	f(1, 'b');
> }
> // g++ test.cpp
> 
> ```
>
> The following is the output of the above example:
>
> ```C++
> Non-template
> Template
> Non-template
> ```
>
> The function call `f(1, 2)` could match the argument types of both the template function and the non-template function. The non-template function is called because a non-template function takes precedence in overload resolution.

A non-template function is always distinct from a **template specialization** with the same type. Specializations of different **function templates** are always distinct from each other even if they have the same type. Two function templates with the same return type and the same parameter list are distinct and can be distinguished with explicit template argument list.

> NOTE: 
>
> 1、基于 "[Function overloads vs function specializations](https://en.cppreference.com/w/cpp/language/function_template#Function_overloads_vs_function_specializations)" 段中关于"overload 和 specialization"的讨论来理解上面这段话是非常容易的



## [Function overloads vs function specializations](https://en.cppreference.com/w/cpp/language/function_template#Function_overloads_vs_function_specializations)

Note that only **non-template** and **primary template overloads** participate in **overload resolution**. The **specializations** are not overloads and are not considered. Only after the **overload resolution** selects the best-matching **primary function template**, its specializations are examined to see if one is a better match.

> NOTE: 
>
> 1、显然，这种设计让整体都变得简单，简而言之：compiler首先执行overload resolution，然后执行specializaiton resolution。
>
> 2、C++严格区分overload 和 specialization，这个观点在 "stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading) # [A](https://stackoverflow.com/a/7108123)" 中给出了
>
> 3、这种设计验证了在`C++\Language-reference\Template\Implementation`中总结的“Partial template specialization是primary template的附庸”观点。

```c++
#include <iostream>
// #1: overload for all types
template<class T> void f(T)
{
	std::cout << __LINE__<<" " <<__PRETTY_FUNCTION__ << std::endl;
}
// #2: specialization of #1 for pointers to int
template<> void f(int*)
{
	std::cout << __LINE__<<" " <<__PRETTY_FUNCTION__ << std::endl;
}
// #3: overload for all pointer types
template<class T> void f(T*)
{
	std::cout << __LINE__<<" " <<__PRETTY_FUNCTION__ << std::endl;
}
int main()
{
	f(new int(1)); // calls #3, even though specialization of #1 would be a perfect match
}
// g++ test.cpp

```

> NOTE: 
>
> 上述程序的输出如下:
>
> ```C++
> 15 void f(T*) [with T = int]
> ```
>
> 

It is important to remember this rule while ordering the header files of a translation unit. For more examples of the interplay(相互影响) between **function overloads** and **function specializations**, expand below:

> NOTE: 
>
> 1、原文这里给出了非常多的内容，暂时没有阅读
>
> 

### 下面是一些补充内容:

### Example: stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading)

[A](https://stackoverflow.com/a/7108123)

> 在stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading)的回答[A](https://stackoverflow.com/a/7108123)中，对overload + template的resolution过程进行了更加详细的介绍，下面是对其中内容的整理：

Short story: overload when you can, specialise when you need to.

Long story: C++ treats specialisation and overloads very differently. This is best explained with an example.

```c++
#include <iostream>
template <typename T> void foo(T)
{
    std::cout<<"foo(T)"<<std::endl;
}
template <typename T> void foo(T*) // overload of foo(T)
{
    std::cout<<"foo(T*)"<<std::endl;
}
template <> void foo<int>(int*) // specialisation of foo(T*)
{
    std::cout<<"foo(int*)"<<std::endl;
}

int main()
{
foo(new int); // calls foo<int>(int*);
}

```

> NOTE:
>
> 对于上述三个`foo`之间的关系，需要准确判断：
>
> 1、`template <typename T> void foo(T)`
>
> 2、`template <typename T> void foo(T*) // overload of foo(T)` 
>
> 上述两个function template之间的关系是overload，两者是不同的template。
>
> 1、`template <> void foo<int>(int*) // specialisation of foo(T*)`
>
> 是对`template <typename T> void foo(T*)`的specialisation ，它和`template <typename T> void foo(T)`之间没有关联。
>
> **resolution过程**：
>
> Compiler 首先进行“overload resolution“，所以它首先找到`template <typename T> void foo(T*)`；然后进行”specialisation resolution“，所以它找到`template <> void foo<int>(int*)`，所以最终的输出为：`foo(int*)`
>
> 

```C++
#include <iostream>
template <typename T> void foo(T)
{
    std::cout<<"foo(T)"<<std::endl;
}
template<> void foo<int*>(int*) // specialisation of foo(T)
{
    std::cout<<"foo<int*>(int*)"<<std::endl;
}
template<typename T> void foo(T*) // overload of foo(T*)
{
    std::cout<<"foo(T*)"<<std::endl;
}

int main()
{
foo(new int); // calls foo<int>(int*);
}

```

> NOTE:
>
> 对于上述三个`foo`之间的关系，需要准确判断：
>
> 1、`template <typename T> void foo(T)`
>
> 2、`template<> void foo<int*>(int*) // specialisation of foo(T)` 
>
> 上述两个function template之间的关系是specialisation 。
>
> 1、`template<typename T> void foo(T*) // overload of foo(T*)`
>
> **resolution过程**：
>
> compiler 首先进行“overload resolution“，所以它首先找到`template <typename T> void foo(T*)`，由于没有对它的specialisation，所以它最终输出：`foo(T*)`

The compiler does **overload resolution** before it even looks at **specialisations**. So, in both cases, overload resolution chooses `foo(T*)`. However, only in the first case does it find `foo<int>(int*)` because in the second case the `int*` specialisation is a specialisation of `foo(T)`, not `foo(T*)`.



## Function template specialization

> NOTE: 原文省略了