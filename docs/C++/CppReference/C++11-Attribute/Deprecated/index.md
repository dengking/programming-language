# `deprecated`





## cppreference [C++ attribute: deprecated (since C++14)](https://en.cppreference.com/w/cpp/language/attributes/deprecated)



## Implementation

在C++14之前如何实现呢？本节讨论这个问题。

### stackoverflow [C++ mark as deprecated](https://stackoverflow.com/questions/295120/c-mark-as-deprecated)

I have a method in an interface that I want to deprecate with portable C++. When I Googled for this all I got was a Microsoft specific solution; [#pragma deprecated](https://docs.microsoft.com/en-us/cpp/preprocessor/deprecated-c-cpp?view=vs-2017) and [__declspec(deprecated)](https://docs.microsoft.com/en-us/cpp/cpp/deprecated-cpp?view=vs-2017).

A second prize solution would be to ifdef a MSVC and a GCC solution.
Thanks



[A](https://stackoverflow.com/a/21192071)

> NOTE: 讨论了C++14的写法

In C++14, you can mark a function as deprecated using the `[[deprecated]]` attribute (see section 7.6.5 [dcl.attr.deprecated]).

> The *attribute-token* `deprecated` can be used to mark names and entities whose use is still allowed, but is discouraged for some reason.

For example, the following function `foo` is deprecated:

```cpp
[[deprecated]]
void foo(int);
```

It is possible to provide a message that describes why the name or entity was deprecated:

```cpp
[[deprecated("Replaced by bar, which has an improved interface")]]
void foo(int);
```

The message must be a string literal.

For further details, see [“Marking as deprecated in C++14”](http://josephmansfield.uk/articles/marking-deprecated-c++14.html).

[A](https://stackoverflow.com/a/295229)

This should do the trick:

```cpp
#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

...

//don't use me any more
DEPRECATED(void OldFunc(int a, float b));

//use me instead
void NewFunc(int a, double b);
```

However, you will encounter problems if a function return type has a commas in its name e.g. `std::pair<int, int>` as this will be interpreted by the preprocesor as passing 2 arguments to the DEPRECATED macro. In that case you would have to typedef the return type.

Edit: simpler (but possibly less widely compatible) version [here](https://stackoverflow.com/a/21265197/2651243).

[A](https://stackoverflow.com/a/21265197)

Here's a simplified version of my [2008 answer](https://stackoverflow.com/a/295229/2651243):

```cpp
#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

//...

//don't use me any more
DEPRECATED void OldFunc(int a, float b);

//use me instead
void NewFunc(int a, double b);
```

See also:

- [MSVC documentation for `__declspec(deprecated)`](http://msdn.microsoft.com/en-us/library/044swk7y.aspx)
- [GCC documentation for `__attribute__((deprecated))`](http://gcc.gnu.org/onlinedocs/gcc-4.7.1/gcc/Type-Attributes.html#Type-Attributes)
- [Clang documentation for `__attribute__((deprecated))`](http://clang.llvm.org/docs/LanguageExtensions.html#deprecated)





