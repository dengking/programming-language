# `std::enable_if`



## boost [enable_if](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html) 

> NOTE: 这篇文章讲到了精髓

### [Introduction](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#core.enable_if.introduction)

The `enable_if` family of templates is a set of tools to allow a **function template** or a **class template specialization** to include or exclude itself from a set of **matching functions** or **specializations** based on properties of its template arguments. For example, one can define **function templates** that are only enabled for, and thus only match, an arbitrary set of types defined by a traits class. The `enable_if` templates can also be applied to enable **class template specializations**. Applications of `enable_if` are discussed in length in [[1\]](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#REF1) and [[2\]](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#REF2).

> NOTE: 通过 `enable_if` 来实现对SFINAE的控制

#### [Background](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#core.enable_if.introduction.background) 

Sensible operation of **template function overloading** in C++ relies on the ***SFINAE* (substitution-failure-is-not-an-error)** principle [[3\]](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#REF3): if an invalid argument or return type is formed during the instantiation of a function template, the instantiation is removed from the overload resolution set instead of causing a compilation error. The following example, taken from [[1\]](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#REF1), demonstrates why this is important:

```c++
int negate(int i) { return -i; }

template <class F>
typename F::result_type negate(const F& f) { return -f(); }
```

Suppose the compiler encounters the call `negate(1)`. The first definition is obviously a better match, but the compiler must nevertheless consider (and instantiate the prototypes) of both definitions to find this out. Instantiating the latter definition with `F` as `int` would result in:

```
int::result_type negate(const int&);
```

where the return type is invalid. If this were an error, adding an unrelated function template (that was never called) could break otherwise valid code. Due to the **SFINAE principle** the above example is not, however, erroneous. The latter definition of `negate` is simply removed from the overload resolution set.

The `enable_if` templates are tools for controlled creation of the **SFINAE conditions**.

> NOTE: 这段话的思想在下一篇中表述为“enable_if - a compile-time switch for templates”

### [The enable_if templates](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#core.enable_if.the_enable_if_templates)

```c++
template<bool B, class T = void>
struct enable_if_c {
    typedef T type;
};

template<class T>
struct enable_if_c<false, T> {
};

template<class Cond, class T = void>
struct enable_if : public enable_if_c<Cond::value, T> {
};
```

An instantiation of the `enable_if_c` template with the parameter `B` as `true` contains a member type `type`, defined to be `T`. If `B` is `false`, no such member is defined. Thus `enable_if_c::type` is either a valid or an invalid **type expression**, depending on the value of `B`. When valid, `enable_if_c::type` equals `T`. The `enable_if_c` template can thus be used for controlling when functions are considered for **overload resolution** and when they are not.

> NOTE: 下面这篇文章中也提及了type expression

```c++
template<class T>
typename enable_if_c<boost::is_arithmetic<T>::value, T>::type
foo(T t) { return t; }
```

The `disable_if_c` template is provided as well, and has the same functionality as `enable_if_c` except for the negated condition. The following function is enabled for all non-arithmetic types.

```c++
template <class T>
typename disable_if_c<boost::is_arithmetic<T>::value, T>::type
bar(T t) { return t; }
```

### [Using enable_if](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#core.enable_if.using_enable_if)

> NOTE:  原文的本地总结了enable_if的几种用法，参看目录可知，`enable_if`的用途是广泛的

With respect to function templates, `enable_if` can be used in multiple different ways:

1、As the **return type** of an instantiatied function

2、As an **extra parameter** of an instantiated function

3、As an **extra template parameter** (useful only in a compiler that supports C++0x default arguments for function template parameters, see [Enabling function templates in C++0x](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#core.enable_if.using_enable_if.enable_if_0x) for details.

As an example of using the form of `enable_if` that works via an extra function parameter, the `foo` function in the previous section could also be written as:

```c++
template <class T>
T foo(T t,
    typename enable_if<boost::is_arithmetic<T> >::type* dummy = 0);
```

Hence, an **extra parameter** of type `void*` is added, but it is given a default value to keep the parameter hidden from client code. Note that the second template argument was not given to `enable_if`, as the default `void` gives the desired behavior.

> NOTE: 给一个default value是非常重要的。

Which way to write the enabler is largely a matter of taste, but for certain functions, only a subset of the options is possible:

- Many operators have a fixed number of arguments, thus `enable_if` must be used either in the **return type** or in an **extra template parameter**.
- Functions that have a **variadic parameter list** must use either the **return type** form or an **extra template parameter**.
- Constructors do not have a return type so you must use either an **extra function parameter** or an **extra template parameter**.
- Constructors that have a **variadic parameter list** must an **extra template parameter**.
- Conversion operators can only be written with an **extra template parameter**.

> NOTE: 这段总结的非常好。有一点需要强调的是，上述几种情况中添加**extra template parameter**时，目的是为了使用`enable_if`来控制SFINAE，而不是真的为了添加一个parameter，所以往往会为extra template parameter指定一个default value，这就是前面所说的“keep the parameter hidden from client code”。下面的[Enabling function templates in C++0x](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#core.enable_if.using_enable_if.enable_if_0x)段，会对这一技巧进行更加深入的分析。



#### [Enabling function templates in C++0x](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#core.enable_if.using_enable_if.enable_if_0x)

```c++
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/utility/enable_if.hpp>

class test
{
public:
    // A constructor that works for any argument list of size 10
    template< class... T,
        typename boost::enable_if_c< sizeof...( T ) == 10,
            int >::type = 0>
    test( T&&... );

    // A conversion operation that can convert to any arithmetic type
    template< class T,
        typename boost::enable_if< boost::is_arithmetic< T >,
            int >::type = 0>
    operator T() const;

    // A conversion operation that can convert to any pointer type
    template< class T,
        typename boost::enable_if< boost::is_pointer< T >,
            int >::type = 0>
    operator T() const;
};

int main()
{
    // Works
    test test_( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 );

    // Fails as expected
    test fail_construction( 1, 2, 3, 4, 5 );

    // Works by calling the conversion operator enabled for arithmetic types
    int arithmetic_object = test_;

    // Works by calling the conversion operator enabled for pointer types
    int* pointer_object = test_;

    // Fails as expected
    struct {} fail_conversion = test_;
}
```

> NOTE: 上述代码在都给你了extra template parameter一个default value，具体原因在前面已经分析了

#### [Enabling template class specializations](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html#core.enable_if.using_enable_if.enabling_template_class_speciali)

```c++
template <class T, class Enable = void>
class A { ... };

template <class T>
class A<T, typename enable_if<is_integral<T> >::type> { ... };

template <class T>
class A<T, typename enable_if<is_float<T> >::type> { ... };
```

The `enable_if_has_type` template is usable this scenario but instead of using a type traits to enable or disable a specialization, it use a SFINAE context to check for the existence of a dependent type inside its parameter. For example, the following structure extracts a dependent `value_type` from T if and only if `T::value_type` exists.

```c++
template <class T, class Enable = void>
class value_type_from
{
  typedef T type;
};

template <class T>
class value_type_from<T, typename enable_if_has_type<typename T::value_type>::type>
{
  typedef typename T::value_type type;
};
```



## cppreference [std::enable_if](https://en.cppreference.com/w/cpp/types/enable_if)



### Possible implementation

前面我们在介绍 `std::enable_if` 的时候提到，如果 condition 不满足的话，会生成一个无效的类型，此处由于SFINAE 机制的存在，只要 call 存在一个匹配的话，就不会报错（只是简单的丢弃该函数）。

```c++
template<bool B, class T = void>
struct enable_if {
};

template<class T>
struct enable_if<true, T> {
    typedef T type;
};

```



`std::enable_if<>`的实现机制如下代码所示：

```c++
template<bool Cond, typename T = void>
struct enable_if {
};

template<typename T>
struct enable_if<true, T> {
    typedef T type;
};
```

从上面的代码可以看到：

在 condition 为真的时候，由于偏特化机制，第二个结构体模板明显是一个更好的匹配，所以 `std::enable_if<>::type` 就是有效的。

当 condition 为假的时候，只有第一个结构体模板能够匹配，所以`std::enable_if<>::type` 是无效的，会被丢弃，编译器会报错：error: no type named ‘type’ in ‘struct std::enable_if<false, bool>。



### Notes

A common mistake is to declare two function templates that differ only in their **default template arguments**. This does not work because the declarations are treated as redeclarations of the same **function template** (**default template arguments** are not accounted for in [function template equivalence](https://en.cppreference.com/w/cpp/language/function_template#Function_template_overloading)).

```c++
#include <type_traits>

/* WRONG */

struct T {
    enum {
        int_t, float_t
    } type; // 成员变量，类型是enum

    // constructor
    template<typename Integer,
            typename = std::enable_if_t<std::is_integral<Integer>::value>>
    T(Integer) : type(int_t) {}

    // constructor
    template<typename Floating,
            typename = std::enable_if_t<std::is_floating_point<Floating>::value>>
    T(Floating) : type(float_t) {} // error: treated as redefinition
};

/* RIGHT */

struct T {
    enum {
        int_t, float_t
    } type; // 成员变量，类型是enum

    // constructor
    template<typename Integer,
            std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
    T(Integer) : type(int_t) {}

    // constructor
    template<typename Floating,
            std::enable_if_t<std::is_floating_point<Floating>::value, bool> = true>
    T(Floating) : type(float_t) {} // OK
};
```





Care should be taken when using `enable_if` in the type of a **template non-type parameter** of a **namespace-scope function template**. Some ABI specifications like the Itanium ABI do not include the instantiation-dependent portions of **non-type template parameters** in the mangling, meaning that specializations of two distinct function templates might end up with the same **mangled name** and be erroneously linked together. For example:

```c++
#include <type_traits>
// first translation unit

struct X
{
    enum { value1 = true, value2 = true };
};

template<class T, std::enable_if_t<T::value1, int> = 0>
void func() {} // #1

template void func<X>(); // #2

// second translation unit

struct X
{
    enum { value1 = true, value2 = true };
};

template<class T, std::enable_if_t<T::value2, int> = 0>
void func() {} // #3

template void func<X>(); // #4
```

The function templates `#1` and `#3` have different signatures and are distinct templates. Nonetheless, `#2` and `#4`, despite being instantiations of different function templates, have the same mangled name [in the Itanium C++ ABI](https://github.com/itanium-cxx-abi/cxx-abi/issues/20) (`_Z4funcI1XLi0EEvv`), meaning that the linker will erroneously consider them to be the same entity.

## Applications&examples

[std::enable_if to conditionally compile a member function](https://stackoverflow.com/questions/6972368/stdenable-if-to-conditionally-compile-a-member-function) 



### [cplusplus std::enable_if<> example](https://cplusplus.com/reference/type_traits/enable_if/#google_vignette) 



```c++
// enable_if example: two ways of using enable_if
#include <iostream>
#include <type_traits>

// 1. the return type (bool) is only valid if T is an integral type:
template <class T>
typename std::enable_if<std::is_integral<T>::value,bool>::type
  is_odd (T i) {return bool(i%2);}

// 2. the second template argument is only valid if T is an integral type:
template < class T,
           class = typename std::enable_if<std::is_integral<T>::value>::type>
bool is_even (T i) {return !bool(i%2);}

int main() {

  short int i = 1;    // code does not compile if type of i is not integral

  std::cout << std::boolalpha;
  std::cout << "i is odd: " << is_odd(i) << std::endl;
  std::cout << "i is even: " << is_even(i) << std::endl;

  return 0;
}
```

### LLVM   

LLVM相关源码: 

```c++
/*----------llvm/include/llvm/ADT/Hashing.h------------*/
/// \brief Compute a hash_code for any integer value.
///
/// Note that this function is intended to compute the same hash_code for
/// a particular value without regard to the pre-promotion type. This is in
/// contrast to hash_combine which may produce different hash_codes for
/// differing argument types even if they would implicit promote to a common
/// type without changing the value.

template <typename T>
typename std::enable_if<is_integral_or_enum<T>::value, hash_code>::type
hash_value(T value);//此函数的返回值是typename std::enable_if<is_integral_or_enum<T>::value, hash_code>::type
```





## See also

cplusplus [std::enable_if](http://www.cplusplus.com/reference/type_traits/enable_if/) 
