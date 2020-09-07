# 关于本章

在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism`中讨论了各种polymorphism，本章讨论c++中如何实现polymorphism，这是一个较大的话题。c++中提供了多种polymorphism，下面根据static/dynamic进行分类：

static

- overload
- template

dynamic

- virtual method

当对它们进行实现的时候，无论是哪种实现方式，都需要考虑one-to-many的问题，即需要考虑使用set  of candidates中的哪一个来作为实现，那到底选择哪一个呢？显然这有一个比较的过程，通俗来讲，选择最“合适” 的那一个，那到底哪个最合适呢？不同的polymorphism有不同的标准。

c++ overload 、template specialization、virtual method和Python attribute find的过程类似，都是实现polymorphsim的基础。

Overload、specialization、subclass都是对某种“特殊情况的说明”（即**最最特殊的实现**），compiler在进行选择/resolve的时候，应该选择**最最特殊情况**。



## Static polymorphism

### Static polymorphism and template metaprogramming

c++的static polymorphism是由compiler来实现，C++的实现允许programmer通过template metaprogramming来对static polymorphism的过程进行控制，这是C++的核心特性，主要的实现方式：

- trait
- sfinae
- `enable_if`

上诉三者在`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if`中总结了。





### Implementation of template

在`C++\Language-reference\Template\Implementation`章节对此进行了深入分析。



### Implementation of overload 

需要根据function name、type of argument来从set  of candidates中找到对应的实现。

#### 实现方式

cppreference [Overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)对此进行了描述。



[Name mangling](https://en.wikipedia.org/wiki/Name_mangling)



匹配过程在下面的文章中有提及：

文章：[SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/)

这篇文章中给出的例子非常好，“matches perfectly”、“overload candidate selection phase”

维基百科文章：[Function overloading](https://en.wikipedia.org/wiki/Function_overloading)

这篇文章中，将匹配过程描述为resolution。

与此相关的问题有：

- [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)

### Overload + template

当将两者结合起来，就使得问题比较复杂了：

- Variadic function template

- Specialization of function template，即涉及到function overload resolution、template specialization


#### resolution过程

function template的resolution算法，在文章[SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/)中就总结地较好：

> while looking for the best overload, all candidates have to be considered. 

正如这篇文章中所讲述的，为了寻找到best overload，compiler采用了SFINAE策略，即它会尝试所有的candidate，关于SFINAE，参见`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if`章节。

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

##### TODO

上述分析的resolution过程，没有涉及SFINAE，后续需要结合具体的例子来进行说明。

TO READ:

- https://www.fluentcpp.com/2017/08/15/function-templates-partial-specialization-cpp/

#### Variadic function template

在文章[Variadic templates in C++](https://eli.thegreenplace.net/2014/variadic-templates-in-c/)中讨论了variadic function template，下面是其中的例子：

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



## Dynamic polymorphism



### Implementation of virtual method

需要考虑，使用哪个实现方式，因为可能的实现方式是多个的。

#### 实现方式

[Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)



most derived class

在https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019中提及了

> For example, if a class has virtual functions, all the instances of that class might share a single virtual function table



需要对比c++ runtime polymorphysim的实现和Python的runtime polymorphysim的实现，两者都需要存储 映射关系。c++的vtbl和Python的`__dict__`类似的。Python仅仅有reference semantic的。



这本质是是对function call的实现，即根据function name，调用对应的function。compiler编译过程中，的scope resolution过程，与上述过程也是非常类似的，都是逐级搜索；我应该根据compiler-time、runtime来进行区分，对两者的实现逻辑进行对比；它们本质上都是建立起映射关系：function name到function的映射；它们的搜索过程，都是逐级进行的。



在文章artima ["Pure Virtual Function Called": An Explanation](https://www.artima.com/cppsource/pure_virtual.html)中对此进行了解释。