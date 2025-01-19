# SFINAE

“SFINAE”是“Substitution failure is not an error”的缩写。"substitution"所表示是使用template argument来**substitute** template parameter。

## Why need SFINAE ?

本节讨论为什么C++ 会采用 SFINAE。在`C++\Guide\Polymorphism`中，我们已经知道:

1) C++  template [**Parametric polymorphism**](https://en.wikipedia.org/wiki/Parametric_polymorphism)，是一种static polymorphism

2) SFINAE是C++实现"通过**template metaprogramming**(编写metaprogram)来对**static polymorphism**的过程进行控制"的基础。

> NOTE: custom polymorphism

本节对这个问题进行深入分析。

### 遵循try my best principle

> SFINAE遵循try my best原则

"try my best原则"是实现polymorphism的主要原则，在`Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation`中对它进行了讨论。下面结合SFINAE来进行说明: 

SFINAE 是 compiler 编译机制(或者说: 原理)，它保证了compiler会对每个candidate都执行替换(校验)，中途出现错误则将当前candidate给drop调，并不会终止，而是去尝试下一个candidate，直至尝试了所有的candidate。这个过程是遵循在`Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation`中提出的"try my best原则"原则的，显然这样做的目标地是: 保证了compiler能够选择最合适的实现(implementation/concrete)；

### SFINAE是实现对**static polymorphism**的过程进行控制的基础

参见 `Custom-static-polymorphism\SFINAE-based` 章节。



## SFINAE的前提是有substitution

1、SFINAE的前提是必要有substitution，如果没有substitution，那么就无法使用SFINAE，这是在使用SFINAE-base custom static polymorphism的时候，非常容易犯下的错误，在 `Custom-static-polymorphism\SFINAE-based` 章节中，对此进行了总结。



## wikipedia [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error)

**Substitution failure is not an error** (**SFINAE**) refers to a situation in [C++](https://en.wikipedia.org/wiki/C%2B%2B) where an invalid substitution of [template](https://en.wikipedia.org/wiki/Template_(C%2B%2B)) parameters is not in itself an error.

> NOTE: SFINAE遵循try my best原则，在上面的"遵循try my best原则"段对这样做的原因进行了解释。



### Example

```c++
#include <iostream>

struct Test {
    typedef int foo;
};

// Definition #1
template<typename T>
void f(typename T::foo) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// Definition #2
template<typename T>
void f(T) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main() {
    f<Test>(10);  // Call #1.
    f<int>(10);   // Call #2. Without error (even though there is no int::foo)
    // thanks to SFINAE.
}
// g++ test.cpp
```

> NOTE: 上述程序的输出如下:
>
> ```c++
> void f(typename T::foo) [with T = Test; typename T::foo = int]
> void f(T) [with T = int]
> 
> ```
>
> 

Although SFINAE was initially introduced to avoid creating ill-formed programs when unrelated template declarations were visible (e.g., through the inclusion of a header file), many developers later found the behavior useful for **compile-time introspection**. Specifically, it allows a template to determine certain properties of its **template arguments** at **instantiation** time.

> NOTE: 
>
> 1、**compile-time introspection**是C++20的[concept](https://en.cppreference.com/w/cpp/language/constraints)所要解决的。
>
> 2、static reflection
>
> 3、发生在template instantiation?

```c++
#include <iostream>

template<typename T>
struct has_typedef_foobar {
    // Types "yes" and "no" are guaranteed to have different sizes,
    // specifically sizeof(yes) == 1 and sizeof(no) == 2.
    typedef char yes[1];
    typedef char no[2];

    template<typename C>
    static yes &test(typename C::foobar *);

    template<typename>
    static no &test(...);

    // If the "sizeof" of the result of calling test<T>(nullptr) is equal to
    // sizeof(yes), the first overload worked and T has a nested type named
    // foobar.
    static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

struct foo {
    typedef float foobar;
};

int main() {
    std::cout << std::boolalpha;
    std::cout << has_typedef_foobar<int>::value << std::endl;  // Prints false
    std::cout << has_typedef_foobar<foo>::value << std::endl;  // Prints true
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述代码是detection idiom 

When `T` has the nested type `foobar` defined, the instantiation of the first `test` works and the null pointer constant is successfully passed. (And the resulting type of the expression is `yes`.) If it does not work, the only available function is the second `test`, and the resulting type of the expression is `no`. An **ellipsis** is used not only because it will accept any argument, but also because its **conversion rank** is lowest, so a call to the first function will be preferred if it is possible; this removes ambiguity.

> NOTE: 这个技巧需要好好学习，关于 ellipsis parameter，参见`C++\Language-reference\Expressions\Operators\Other\Catch-all-operator`章节。

### C++11 simplification

In [C++11](https://en.wikipedia.org/wiki/C%2B%2B11), the above code could be simplified to:

```c++
#include <iostream>
#include <type_traits>

template<typename ... Ts>
using void_t = void;

template<typename T, typename = void>
struct has_typedef_foobar : std::false_type {
};

template<typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : std::true_type {
};

struct foo {
    using foobar = float;
};

int main() {
    std::cout << std::boolalpha;
    std::cout << has_typedef_foobar<int>::value << std::endl;
    std::cout << has_typedef_foobar<foo>::value << std::endl;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 
>
> 运行结果如下:
>
> ```C++
> true
> true
> ```
>
> 通过上述运行结果来看，`void_t`并没有生效，具体原因在cppreference [std::void_t](https://en.cppreference.com/w/cpp/types/void_t) 中给出了解释。
>
> 关于正确的实现，参见
>
> 1) `C++\Library\Standard-library\Utility-library\Language-support\Type-support\Type-traits\Type-transformations\void_t`章节。
>
> 2) `C++\Language-reference\Template\Programming-paradigm\Idioms\Detection`章节

With the standardisation of the detection idiom in the [Library fundamental v2 (n4562)](http://en.cppreference.com/w/cpp/experimental/lib_extensions_2) proposal, the above code could be re-written as follows:

> NOTE: 参见 `C++\Library\Standard-library\Extensions\Version-2\The-C++detection-idiom` 章节。

## cppreference [sfinae](https://en.cppreference.com/w/cpp/language/sfinae)

> NOTE: 
>
> 一、原文的结构是不明晰的，初读容易无法掌握清楚脉络，掌握原文内容的一个前提是:
>
> SFINAE既可以作用于overload也可以作用于specialization。
>
> 下面是我梳理后的脉络:
>
> 1、"This rule applies during overload resolution of function templates" 
>
> 首先描述的是 SFINAE 用于overload resolution
>
> 2、"Type SFINAE"
>
> 描述的是哪些error导致failure
>
> 3、"Expression SFINAE"
>
> 这是C++11引入的一种新的写法
>
> 4、"SFINAE in partial specializations"
>
> 描述的是 SFINAE 用于 specialization
>
> 5、"Library support"
>
> 标准库提供的对SFINAE的支持
>
> 6、"Alternatives"



### [Explanation](https://en.cppreference.com/w/cpp/language/sfinae#Explanation)



### [Type SFINAE](https://en.cppreference.com/w/cpp/language/sfinae#Type_SFINAE)

- attempting to use a type that is not a class or enumeration on the left of a scope resolution operator `**::**`

    > NOTE: 
    >
    > 1、下面的`is_class`其实用到了这一点

- attempting to create a reference to void

    > NOTE: 
    >
    > 1、void是non-referenceable type，关于此，是我在阅读 `SFINAE in partial specializations` 段时发现的
    >
    > 2、在`SFINAE in partial specializations` 段中，给出了trait来判断是否是non-referenceable type

- attempting to create pointer to member of `T`, where `T` is not a class type

```C++
template<typename T>
class is_class
{
	typedef char yes[1];
	typedef char no[2];
	template<typename C> static yes& test(int C::*); // selected if C is a class type
	template<typename C> static no& test(...);      // selected otherwise
public:
	static bool const value = sizeof(test<T>(0)) == sizeof(yes);
};

```

> NOTE: 
>
> 1、上述是使用detection idiom
>
> 2、它实现了一个class concept
>
> 3、它的实现是非常值得借鉴的



### [Expression SFINAE](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE)

> NOTE: 
>
> 1、参见`Expression-SFINAE`章节。



### SFINAE in partial specializations

> NOTE: 
>
> 1、参见 `Class-template-SFINAE` 章节



### [Library support](https://en.cppreference.com/w/cpp/language/sfinae#Library_support)

> NOTE: 
>
> 一、原文介绍了 [std::enable_if](https://en.cppreference.com/w/cpp/types/enable_if) 、 [std::void_t](https://en.cppreference.com/w/cpp/types/void_t) ，但是其中的内容是任意误解的，下面是澄清:
>
> 1、 [std::enable_if](https://en.cppreference.com/w/cpp/types/enable_if) 是可以用于partial specialization SFINAE的
>
> 二、关于这两个function的描述，参见 `Custom-static-polymorphism\SFINAE-based` 章节

### [Alternatives](https://en.cppreference.com/w/cpp/language/sfinae#Alternatives)

Where applicable, [tag dispatch](https://en.cppreference.com/w/cpp/iterator/iterator_tags#Example), [static_assert](https://en.cppreference.com/w/cpp/language/static_assert), and, if available, [concepts](https://en.cppreference.com/w/cpp/language/constraints), are usually preferred over direct use of SFINAE.

> NOTE: 
>
> 1、上述这些都是custom static polymorphism，在`Custom-static-polymorphism`章节对它们进行了介绍



## TO READ



https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html

https://www.fluentcpp.com/2018/05/18/make-sfinae-pretty-2-hidden-beauty-sfinae/


https://www.modernescpp.com/index.php/c-20-concepts-the-details

https://jguegant.github.io/blogs/tech/sfinae-introduction.html