# 关于本章

本章描述function template。



## cppreference [Function template](https://en.cppreference.com/w/cpp/language/function_template)



### [Function template instantiation](https://en.cppreference.com/w/cpp/language/function_template#Function_template_instantiation)



#### Explicit instantiation

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



#### Implicit instantiation

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



### [Function template overloading](https://en.cppreference.com/w/cpp/language/function_template#Function_template_overloading)

Function templates and non-template functions may be overloaded.

A non-template function is always distinct from a template specialization with the same type. Specializations of different function templates are always distinct from each other even if they have the same type. Two function templates with the same return type and the same parameter list are distinct and can be distinguished with explicit template argument list.

> NOTE: 
>
> C++的overload resolution本就复杂，现在加上template就使得问题更加复杂了，所以原文的这一段非常难以理解也是可想而知的。
>

### [Function overloads vs function specializations](https://en.cppreference.com/w/cpp/language/function_template#Function_overloads_vs_function_specializations)

Note that only non-template and **primary template overloads** participate in overload resolution. The **specializations** are not overloads and are not considered. Only after the **overload resolution** selects the best-matching **primary function template**, its specializations are examined to see if one is a better match.

> NOTE: 显然，这种设计让整体都变得简单，简而言之：compiler首先执行overload resolution，然后执行specializaiton resolution。
>
> 这种设计验证了在`C++\Language-reference\Template\Implementation\index.md`中总结的“Partial template specialization是primary template的附庸”观点。

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

上述程序的输出如下:

```C++
15 void f(T*) [with T = int]
```



#### Example: stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading)

在stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading)的回答[A](https://stackoverflow.com/a/7108123)中，对overload + template的resolution过程进行了更加详细的介绍，下面是对其中内容的整理：

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

对于上述三个`foo`之间的关系，需要准确判断：

- `template <typename T> void foo(T)`
- `template <typename T> void foo(T*) // overload of foo(T)` 

上述两个function template之间的关系是overload，两者是不同的template。

- `template <> void foo<int>(int*) // specialisation of foo(T*)`

是对`template <typename T> void foo(T*)`的specialisation ，它和`template <typename T> void foo(T)`之间没有关联。

**resolution过程**：compiler 首先进行“overload resolution“，所以它首先找到`template <typename T> void foo(T*)`；然后进行”specialisation resolution“，所以它找到`template <> void foo<int>(int*)`，所以最终的输出为：`foo(int*)`



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

对于上述三个`foo`之间的关系，需要准确判断：

- `template <typename T> void foo(T)`
- `template<> void foo<int*>(int*) // specialisation of foo(T)` 

上述两个function template之间的关系是specialisation 。

- `template<typename T> void foo(T*) // overload of foo(T*)`

**resolution过程**：compiler 首先进行“overload resolution“，所以它首先找到`template <typename T> void foo(T*)`，由于没有对它的specialisation，所以它最终输出：`foo(T*)`

总结：

The compiler does **overload resolution** before it even looks at specialisations. So, in both cases, overload resolution chooses `foo(T*)`. However, only in the first case does it find `foo<int>(int*)` because in the second case the `int*` specialisation is a specialisation of `foo(T)`, not `foo(T*)`.





#### 准确表达overload and specialization

C++的语法是比较冗杂的，有时候稍微的差异，表达的含义却是天差地别，这一点在template + overload的时候尤其明显，下面结合如下两篇文章中的内容进行说明：

- stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading)
- accu [An introduction to C++ Traits](https://accu.org/index.php/journals/442)

在stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading)中给出了如下例子：

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



```c++
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

再来看一个accu [An introduction to C++ Traits](https://accu.org/index.php/journals/442)中给出的例子：

`is_pointer`

Now, while **fully specialized templates** are useful and in my experience, the most common sort of trait class specialization, I think that it is worth quickly looking at **partial specialization**, in this case, `boost::is_pointer` [[boost](https://accu.org/index.php/journals/442#boost)]. Again, a default template class is defined:

```c++
template< typename T > 
struct is_pointer{ 
  static const bool value = false; 
};
```

And a **partial specialization** for all pointer types is added:

```c++
template< typename T > 
struct is_pointer< T* >{ 
  static const bool value = true; 
};
```

简而言之：

- `template< typename T > struct is_pointer`
- `template< typename T > struct is_pointer< T* >//partial specialization`

#### Variadic function template

在文章thegreenplace [Variadic templates in C++](https://eli.thegreenplace.net/2014/variadic-templates-in-c/)中讨论了variadic function template，下面是其中的例子：

```c++
#include <string>
#include <iostream>
template<typename T>
T adder(T v) {
  std::cout << __PRETTY_FUNCTION__ << "\n";
  return v;
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
  std::cout << __PRETTY_FUNCTION__ << "\n";
  return first + adder(args...);
}

int main()
{
long sum = adder(1, 2, 3, 8, 7);
std::cout<<sum<<std::endl;
    
std::string s1 = "x", s2 = "aa", s3 = "bb", s4 = "yy";
std::string ssum = adder(s1, s2, s3, s4);
std::cout<<ssum<<std::endl;
}
```

> 编译:`g++ --std=c++11 test.cpp`

下面是compiler生成的code：

```c++
T adder(T, Args...) [T = int, Args = <int, int, int, int>]
T adder(T, Args...) [T = int, Args = <int, int, int>]
T adder(T, Args...) [T = int, Args = <int, int>]
T adder(T, Args...) [T = int, Args = <int>]
T adder(T) [T = int]
```

compiler生成上述代码的过程是值的推敲的，一个值的推敲的点是：base condition，即`T adder(T v)`的到达。


