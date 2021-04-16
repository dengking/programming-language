# cppreference [Replacing text macros](https://en.cppreference.com/w/cpp/preprocessor/replace)



## `#` and `##` operators

> NOTE: 
>
> 1、原文的内容， 非常冗长，不易阅读理解



## `#` stringify and `##` concatenate

关于此，参见:

### [gsl-lite](https://github.com/gsl-lite/gsl-lite)/[include](https://github.com/gsl-lite/gsl-lite/tree/master/include)/[gsl](https://github.com/gsl-lite/gsl-lite/tree/master/include/gsl)/[gsl-lite.hpp](https://github.com/gsl-lite/gsl-lite/blob/master/include/gsl/gsl-lite.hpp)

```C++
#define  gsl_lite_MAJOR  0
#define  gsl_lite_MINOR  38
#define  gsl_lite_PATCH  0

#define  gsl_lite_VERSION  gsl_STRINGIFY(gsl_lite_MAJOR) "." gsl_STRINGIFY(gsl_lite_MINOR) "." gsl_STRINGIFY(gsl_lite_PATCH)

#define gsl_STRINGIFY(  x )  gsl_STRINGIFY_( x )
#define gsl_STRINGIFY_( x )  #x
#define gsl_CONCAT_(  a, b )  gsl_CONCAT2_( a, b )
#define gsl_CONCAT2_( a, b )  a##b
```



## stackoverflow [# and ## in macros](https://stackoverflow.com/questions/4364971/and-in-macros)

> NOTE: 
>
> 1、非常好的例子

```C++
#include <stdio.h>
#define f(a,b) a##b
#define g(a)   #a
#define h(a) g(a)

int main()
{
	printf("%s\n", h(f(1,2)));
	printf("%s\n", g(f(1,2)));
	return 0;
}

```

> NOTE: 
>
> 下面是我的测试:
>
> ```C++
> #define f(a,b) a##b
> #define g(a)   #a
> #define h(a) g(a)
> 
> int main()
> {
> 	printf("%s\n", h(f(1,2)));
> 	printf("%s\n", g(f(1,2)));
> 	return 0;
> }
> 
> ```
>
> `gcc -E test.cpp`
>
> ```C++
> # 1 "test.cpp"
> # 1 "<built-in>"
> # 1 "<命令行>"
> # 1 "/usr/include/stdc-predef.h" 1 3 4
> # 1 "<命令行>" 2
> # 1 "test.cpp"
> 
> 
> 
> 
> int main()
> {
>  printf("%s\n", "12");
>  printf("%s\n", "f(1,2)");
>  return 0;
> }
> ```
>
> 

### [A](https://stackoverflow.com/a/4368983)

> NOTE: 
>
> 1、这个解释非常好

An occurrence of a parameter in a function-like macro, unless it is the operand of `#` or `##`, is **expanded** before **substituting** it and rescanning the whole for further expansion. Because `g`'s parameter *is* the operand of `#`, the argument is not expanded but instead immediately stringified (`"f(1,2)"`). Because `h`'s parameter *is not* the operand of `#` nor `##`, the argument is first expanded (`12`), then substituted (`g(12)`), then rescanning and further expansion occurs (`"12"`).

### [A](https://stackoverflow.com/a/4365341)

Because that is how the preprocessor works.

A single '#' will create a string from the given argument, regardless of what that argument contains, while the double '##' will create a new token by concatenating the arguments.

Try looking at the preprocessed output (for instance with `gcc -E`) if you want to understand better how the macros are evaluated.

## Example

1、[SergiusTheBest](https://github.com/SergiusTheBest)/[ScopeExit](https://github.com/SergiusTheBest/ScopeExit)

2、More C++ Idioms/[Member Detector](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector)

