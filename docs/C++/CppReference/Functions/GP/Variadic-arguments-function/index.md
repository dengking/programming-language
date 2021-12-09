# Variadic functions



## C++中实现variadic function的方式

总的来说，有如下三种方式:

1、[parameter pack](https://en.cppreference.com/w/cpp/language/parameter_pack)  C++11

2、[std::initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list)  C++11

3、C [Variadic arguments](https://en.cppreference.com/w/cpp/language/variadic_arguments)

### stackoverflow [Variable number of arguments in C++?](https://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c)

#### [A](https://stackoverflow.com/a/1657924)

You probably shouldn't, and you can probably do what you want to do in a safer and simpler way. Technically to use variable number of arguments in C you include `stdarg.h`. From that you'll get the `va_list` type as well as three functions that operate on it called `va_start()`, `va_arg()` and `va_end()`.

```cpp
#include<stdarg.h>

int maxof(int n_args, ...)
{
    va_list ap;
    va_start(ap, n_args);
    int max = va_arg(ap, int);
    for(int i = 2; i <= n_args; i++) {
        int a = va_arg(ap, int);
        if(a > max) max = a;
    }
    va_end(ap);
    return max;
}
```

If you ask me, this is a mess. It looks bad, it's unsafe, and it's full of technical details that have nothing to do with what you're conceptually trying to achieve. Instead, consider using overloading or inheritance/polymorphism, builder pattern (as in `operator<<()` in streams) or default arguments etc. These are all safer: the compiler gets to know more about what you're trying to do so there are more occasions it can stop you before you blow your leg off.

#### [A](https://stackoverflow.com/a/16338804)

In *C++11* you have two new options, as the [Variadic functions](http://en.cppreference.com/w/cpp/utility/variadic) reference page in the *Alternatives section* states:

> 1、Variadic templates can also be used to create functions that take variable number of arguments. They are often the better choice because they do not impose restrictions on the types of the arguments, do not perform integral and floating-point promotions, and are type safe. (since C++11)
>
> 2、If all variable arguments share a common type, a `std::initializer_list` provides a convenient mechanism (albeit with a different syntax) for accessing variable arguments.

Below is an example showing both alternatives (*see it live*):

```cpp
#include <iostream>
#include <string>
#include <initializer_list>

template <typename T>
void func(T t) 
{
    std::cout << t << std::endl ;
}

template<typename T, typename... Args>
void func(T t, Args... args) // recursive variadic function
{
    std::cout << t <<std::endl ;

    func(args...) ;
}

template <class T>
void func2( std::initializer_list<T> list )
{
    for( auto elem : list )
    {
        std::cout << elem << std::endl ;
    }
}

int main()
{
    std::string
        str1( "Hello" ),
        str2( "world" );

    func(1,2.5,'a',str1);

    func2( {10, 20, 30, 40 }) ;
    func2( {str1, str2 } ) ;
} 
```

If you are using `gcc` or `clang` we can use the [**PRETTY_FUNCTION** *magic variable*](https://gcc.gnu.org/onlinedocs/gcc/Function-Names.html) to display the type signature of the function which can be helpful in understanding what is going on. For example using:

```cpp
std::cout << __PRETTY_FUNCTION__ << ": " << t <<std::endl ;
```

would results int following for variadic functions in the example (*see it live*):

```cpp
void func(T, Args...) [T = int, Args = <double, char, std::basic_string<char>>]: 1
void func(T, Args...) [T = double, Args = <char, std::basic_string<char>>]: 2.5
void func(T, Args...) [T = char, Args = <std::basic_string<char>>]: a
void func(T) [T = std::basic_string<char>]: Hello
```

In Visual Studio you can use [**FUNCSIG**](https://msdn.microsoft.com/en-us/library/b0084kay.aspx).

**Update Pre C++11**

Pre *C++11* the alternative for [std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list) would be [std::vector](http://en.cppreference.com/w/cpp/container/vector) or one of the other [standard containers](http://en.cppreference.com/w/cpp/container):

```cpp
#include <iostream>
#include <string>
#include <vector>

template <class T>
void func1( std::vector<T> vec )
{
    for( typename std::vector<T>::iterator iter = vec.begin();  iter != vec.end(); ++iter )
    {
        std::cout << *iter << std::endl ;
    }
}

int main()
{
    int arr1[] = {10, 20, 30, 40} ;
    std::string arr2[] = { "hello", "world" } ; 
    std::vector<int> v1( arr1, arr1+4 ) ;
    std::vector<std::string> v2( arr2, arr2+2 ) ;

    func1( v1 ) ;
    func1( v2 ) ;
}
```

and the alternative for *variadic templates* would be [variadic functions](http://en.cppreference.com/w/cpp/utility/variadic) although they are not *type-safe* and in general [error prone and can be unsafe to use](http://robertseacord.sys-con.com/node/158825/mobile) but the only other potential alternative would be to use *default arguments*, although that has limited use. The example below is a modified version of the sample code in the linked reference:

```cpp
#include <iostream>
#include <string>
#include <cstdarg>

void simple_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt != '\0') {
        if (*fmt == 'd') {
            int i = va_arg(args, int);
            std::cout << i << '\n';
        } else if (*fmt == 's') {
            char * s = va_arg(args, char*);
            std::cout << s << '\n';
        }
        ++fmt;
    }

    va_end(args);
}


int main()
{
    std::string
        str1( "Hello" ),
        str2( "world" );

    simple_printf("dddd", 10, 20, 30, 40 );
    simple_printf("ss", str1.c_str(), str2.c_str() ); 

    return 0 ;
} 
```

Using *variadic functions* also comes with restrictions in the arguments you can pass which is detailed in the [draft C++ standard](http://www.open-std.org/Jtc1/sc22/wg21/docs/papers/2005/n1804.pdf) in section `5.2.2` *Function call* paragraph *7*:

> When there is no parameter for a given argument, the argument is passed in such a way that the receiving function can obtain the value of the argument by invoking `va_arg` (18.7). The lvalue-to-rvalue (4.1), array-to-pointer (4.2), and function-to-pointer (4.3) standard conversions are performed on the argument expression. After these conversions, if the argument does not have arithmetic, enumeration, pointer, pointer to member, or class type, the program is ill-formed. If the argument has a non-POD class type (clause 9), the behavior is undefined. [...]





## cppreference [Utility library](https://en.cppreference.com/w/cpp/utility) # [Variadic functions](https://en.cppreference.com/w/cpp/utility/variadic)

> NOTE: 
>
> 主要讲述了 [va_start](https://en.cppreference.com/w/cpp/utility/variadic/va_start) 、[va_arg](https://en.cppreference.com/w/cpp/utility/variadic/va_arg) 、[va_list](https://en.cppreference.com/w/cpp/utility/variadic/va_list) 等等



## cppreference [Variadic arguments](https://en.cppreference.com/w/cpp/language/variadic_arguments)

> NOTE: 
>
> 1、需要注意的是，variadic arguments 和 parameter pack是两个概念，它是传统的C的方式，即前面的 "cppreference [Utility library](https://en.cppreference.com/w/cpp/utility) # [Variadic functions](https://en.cppreference.com/w/cpp/utility/variadic)" 中介绍的方式
>
> 2、本文总结了C++中各种实现 variadic argument function的方式



### [va_arg](https://en.cppreference.com/w/cpp/utility/variadic/va_arg) 方式

```C++
// the function declared as follows
int printx(const char* fmt...);
// may be called with one or more arguments:
printx("hello world");
printx("a=%d b=%d", a, b);
 
int printx(const char* fmt, ...); // same as above (extraneous comma is allowed 
                                  // for C compatibility)
int printy(..., const char* fmt); // error: ... cannot appear as a parameter
int printz(...); // valid, but the arguments cannot be accessed portably
```

#### Default conversions

Because variadic parameters have the lowest rank for the purpose of [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution), they are commonly used as the catch-all fallbacks in [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae).



Because variadic parameters have the lowest rank for the purpose of [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution), they are commonly used as the catch-all fallbacks in [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae).

> NOTE: 
>
> 1、上述catch-all，非常形象

### Alternatives (since C++11)

1、[Variadic templates](https://en.cppreference.com/w/cpp/language/parameter_pack) 

2、[std::initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list) 

### Notes

In the C programming language, at least one named parameter must appear before the ellipsis parameter, so `printz(...);` is not valid. In C++, this form is allowed even though the arguments passed to such function are not accessible, and is commonly used as the fallback overload in [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae), exploiting the lowest priority of the ellipsis conversion in [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution).

> NOTE: 
>
> 1、这段总结非常好，它是SFINAE的总结

## Variadic template VS ellipsis

本质上是"static type VS dynamic type": 

C++ veridical template相比于C的优势，可以作为static type VS dynamic type的案例

在 eli.thegreenplace [Variadic templates in C++](https://eli.thegreenplace.net/2014/variadic-templates-in-c/) 中，也对这个话题进行了讨论。





## `Recursive-variadic-function`

参见 `Recursive-variadic-function` 章节。