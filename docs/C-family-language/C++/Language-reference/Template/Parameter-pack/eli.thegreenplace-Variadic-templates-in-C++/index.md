# eli.thegreenplace [Variadic templates in C++](https://eli.thegreenplace.net/2014/variadic-templates-in-c/)

> NOTE: 这篇文章展示了如下例子：
>
> - Variadic function template
> - Variadic class template
> - Perfect forwarding



Prior to C++11, the only way to write functions that take an arbitrary number of arguments was to use **variadic functions** like `printf`, with the ellipsis syntax (`...`) and the accompanying `va_` family of macros. If you've ever written code using this approach you know how cumbersome(麻烦) it is. In addition to being type unsafe (all **type resolution** has to be done explicitly with casts in `va_arg`, at runtime), it's also tricky to get right. The `va_`macros perform low-level memory manipulation, and I've seen a lot of code that segfaults(段错误) because it isn't using them carefully enough.

> NOTE: 
>
> 1、interpretation model
>
> 2、static type VS dynamic type

But what always bothered me most with this approach is leaving something that is clearly known at **compile-time**, to **run-time**. Yes, when we write a variadic function we don't know all the ways it's going to be used. But when the compiler puts the whole program together, *it does know*. It sees perfectly well all the invocations of the function throughout the program, and all the possible argument types it gets passed (types are, after all, resolved at compile-time in C++).

> NOTE: C和`C++`的type system的对比

## Variadic templates

One of the new features of `C++11` is *variadic templates*. Finally, there's a way to write functions that take an arbitrary number of arguments in a type-safe way and have all the argument handling logic resolved at compile-time, rather than run-time. Variadic templates can be used for much more than just functions that take an arbitrary number of arguments; in this article I want to demonstrate some of these capabilities.

## Basic example

Let's dive in, by implementing a function that adds all of its arguments together:

```c++
template<typename T>
T adder(T v) {
  return v;
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
  return first + adder(args...);
}
```

And here are a couple of ways we could call it:

```c++
long sum = adder(1, 2, 3, 8, 7);

std::string s1 = "x", s2 = "aa", s3 = "bb", s4 = "yy";
std::string ssum = adder(s1, s2, s3, s4);
```



> NOTE: 下面是完整的可执行程序
>

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
// g++ --std=c++11 test.cpp
```



`adder` will accept any number of arguments, and will compile properly as long as it can apply the `+` operator to them. This checking is done by the compiler, at compile time. There's nothing magical about it - it follows C++'s usual template and overload resolution rules.



`typename... Args` is called a *template parameter pack*, and `Args... args` is called a *function parameter pack*(`Args` is, of course, a completely arbitrary name and could be anything else). Variadic templates are written just the way you'd write recursive code - you need a **base case** (the `adder(T v)` declaration above) and a **general case** which "recurses" [[1\]](https://eli.thegreenplace.net/2014/variadic-templates-in-c/#id7). The recursion itself happens in the call `adder(args...)`. Note how the general `adder`is defined - the first argument is **peeled off**(剥离) the template parameter pack into type `T` (and accordingly, argument `first`). So with each call, the parameter pack gets shorter by one parameter. Eventually, the **base case** is encountered.

> NOTE: 上诉Basic example中给出的例子是典型的template+overload，compiler对它的resolution的过程是比较复杂的，在`C-family-language\C++\Guide\Implementation-of-polymorphism\index.md`的《Overload + template》章节中对它的resolution过程进行了非常详细的说明。
>
> 上述 **base case** 是recursion的 **ending condition** 。

To get a better feel for the process, one can use the `__PRETTY_FUNCTION__` macro [[2\]](https://eli.thegreenplace.net/2014/variadic-templates-in-c/#id8). If we insert the following as the first line in both versions of `adder` above:

```c++
std::cout << __PRETTY_FUNCTION__ << "\n";
```

And then execute `adder(1, 2, 3, 8, 7)`, we'll see:

```c++
T adder(T, Args...) [T = int, Args = <int, int, int, int>]
T adder(T, Args...) [T = int, Args = <int, int, int>]
T adder(T, Args...) [T = int, Args = <int, int>]
T adder(T, Args...) [T = int, Args = <int>]
T adder(T) [T = int]
```



## Some simple variations

When reading about `C++` **template meta-programming**, one often hears about "**pattern matching**" and how this part of the language constitutes(构成) a fairly complete compile-time functional language.

The example shown above is very basic - template arguments are **peeled off**（剥离） one by one until the **base case** is hit. Here's a somewhat more interesting display of **pattern matching**:

```c++
template<typename T>
bool pair_comparer(T a, T b) {
  // In real-world code, we wouldn't compare floating point values like
  // this. It would make sense to specialize this function for floating
  // point types to use approximate comparison.
  return a == b;
}

template<typename T, typename... Args>
bool pair_comparer(T a, T b, Args... args) {
  return a == b && pair_comparer(args...);
}
```

`pair_comparer` accepts any number of arguments and returns `true` if and only if they are **pair-wise** equal. The types are not enforced - everything that can be compared goes. For example:

```c++
#include <iostream>
template<typename T>
bool pair_comparer(T a, T b) {
  // In real-world code, we wouldn't compare floating point values like
  // this. It would make sense to specialize this function for floating
  // point types to use approximate comparison.
  return a == b;
}

template<typename T, typename... Args>
bool pair_comparer(T a, T b, Args... args) {
  return a == b && pair_comparer(args...);
}
int main()
{
std::cout<<pair_comparer(1.5, 1.5, 2, 2, 6, 6)<<std::endl;    
}
```

> NOTE: 编译指令`g++ --std=c++11 test.cpp`

Returns `true`. But if we change the second argument to just `1`, this won't compile since a `double` and `int` are not the same type.

More interestingly, `pair_comparer` will only work for an even number of arguments because they are peeled off in **pairs** and the base case compares two. The following:

```c++
pair_comparer(1.5, 1.5, 2, 2, 6, 6, 7)
```

Does not compile; the compiler complains that the base case expects 2 arguments but only 1 is provided. To fix this, we can add another variation of the function template:

```c++
template<typename T>
bool pair_comparer(T a) {
  return false;
}
```



```c++
#include <iostream>
template<typename T>
bool pair_comparer(T a, T b) {
  // In real-world code, we wouldn't compare floating point values like
  // this. It would make sense to specialize this function for floating
  // point types to use approximate comparison.
  return a == b;
}

template<typename T>
bool pair_comparer(T a) {
  return false;
}

template<typename T, typename... Args>
bool pair_comparer(T a, T b, Args... args) {
  return a == b && pair_comparer(args...);
}
int main()
{
std::cout<<pair_comparer(1.5, 1.5, 2, 2, 6, 7)<<std::endl;    
}

```

> NOTE: 编译指令`g++ --std=c++11 test.cpp`





Here, we force all odd-numbered sequences of arguments to return `false`, because when only a single argument is left this version is matched.

Note that `pair_comparer` forces both members of the compared pair to be of the exact same type. A simple variation would be to allow different types, as long as they can be compared. I'll leave this an an exercise to the interested reader.

## Performance

If you're concerned with the performance of code that relies on variadic templates, worry not. As there's no actual recursion involved, all we have is a sequence of function calls **pre-generated** at **compile-time**. This sequence is, in practice, fairly short (variadic calls with more than 5-6 arguments are rare). Since modern compilers are aggressively **inlining code**, it's likely to end up being compiled to machine code that has absolutely no function calls. What you end up with, actually, is not unlike **loop unrolling**.

> NOTE: 上述loop unrolling的实现是依赖于compiler的

Compared to the C-style variadic functions, this is a marked win, because C-style variadic arguments have to be resolved at runtime. The `va_` macros are literally manipulating the runtime stack. Therefore, variadic templates are often a performance optimization for variadic functions.

## Type-safe variadic functions

I have mentioned `printf` in the beginning of the article, as an example of a variadic function that doesn't use templates. However, as we all know, `printf` and its kin are not *type safe*. If you pass a number into a `%s`format, bad things may happen and the compiler won't warn you about it [[3\]](https://eli.thegreenplace.net/2014/variadic-templates-in-c/#id9).

It's pretty obvious how **variadic templates** enable us to write **type safe** functions. In the case of `printf`, when the implementation reaches a new formatting directive it can actually assert the type of the argument passed. This assertion won't fire at compile-time, but it *will* fire - and a nice error message can be generated instead of **undefined behavior**.

> NOTE: type safe通过compile time完成

> NOTE: 下面是cppreference [Parameter pack(since C++11)](https://en.cppreference.com/w/cpp/language/parameter_pack)中给出的例子，它类似于一个type safe `printf`
>
> ```c++
> #include <iostream>
>  
> void tprintf(const char* format) // base function
> {
>     std::cout << format;
> }
>  
> template<typename T, typename... Targs>
> void tprintf(const char* format, T value, Targs... Fargs) // recursive variadic function
> {
>     for ( ; *format != '\0'; format++ ) {
>         if ( *format == '%' ) {
>            std::cout << value;
>            tprintf(format+1, Fargs...); // recursive call
>            return;
>         }
>         std::cout << *format;
>     }
> }
>  
> int main()
> {
>     tprintf("% world% %\n","Hello",'!',123);
>     return 0;
> }
> ```
>
> 

I will not discuss the implementation of a type-safe `printf` further - it has been rehashed many times already. For some good examples see Stroustrup's new edition of "The C++ Programming Language", or Alexandrescu's "[Variadic templates are funadic](https://channel9.msdn.com/Events/GoingNative/GoingNative-2012/Variadic-Templates-are-Funadic)" talk.

## Varidic data structures

This use-case is much more interesting, IMHO, because it was something that just wasn't possible prior to introduction of `C++11`, at least without considerable hackery.

Custom data structures (`struct`s since the times of C and `class`es in C++) have compile-time defined fields. They can represent types that grow at runtime (`std::vector`, for example) but if you want to add new fields, this is something the compiler has to see. **Variadic templates** make it possible to define data structures that could have an arbitrary number of fields, and have this number configured per use. The prime example of this is a `tuple` class, and here I want to show how to construct one [[4\]](https://eli.thegreenplace.net/2014/variadic-templates-in-c/#id10).

For the full code that you can play with and compile on your own: [variadic-tuple.cpp](https://github.com/eliben/code-for-blog/blob/master/2014/variadic-tuple.cpp).

Let's start with the type definition:

```c++
template <class... Ts> struct tuple {};

template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...> {
  tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {}

  T tail;
};
```

> NOTE: 上述是Varidic class template。

We start with the **base case** - the definition of a class template named `tuple`, which is empty. The specialization that follows peels off(剥离) the first type from the parameter pack, and defines a member of that type named `tail`. It also derives from(源于) the tuple instantiated with the rest of the pack. This is a recursive definition that stops when there are no more types to peel off, and the base of the hierarchy is an empty `tuple`. To get a better feel for the resulting data structure, let's use a concrete example:

```c++
tuple<double, uint64_t, const char*> t1(12.2, 42, "big");
```

Ignoring the constructor, here's a pseudo-trace of the `tuple` structs created:

```c++
struct tuple<double, uint64_t, const char*> : tuple<uint64_t, const char*> {
  double tail;
}

struct tuple<uint64_t, const char*> : tuple<const char*> {
  uint64_t tail;
}

struct tuple<const char*> : tuple {
  const char* tail;
}

struct tuple {
}
```

The layout of **data members** in the original 3-element `tuple` will be:

```c++
[const char* tail, uint64_t tail, double tail]
```

Note that the empty base consumes no space, due to [empty base optimization](http://en.cppreference.com/w/cpp/language/ebo). Using Clang's [layout dump feature](http://eli.thegreenplace.net/2012/12/17/dumping-a-c-objects-memory-layout-with-clang/), we can verify this:

```
*** Dumping AST Record Layout
   0 | struct tuple<double, unsigned long, const char *>
   0 |   struct tuple<unsigned long, const char *> (base)
   0 |     struct tuple<const char *> (base)
   0 |       struct tuple<> (base) (empty)
   0 |       const char * tail
   8 |     unsigned long tail
  16 |   double tail
     | [sizeof=24, dsize=24, align=8
     |  nvsize=24, nvalign=8]
```

Indeed, the size of the data structure and the internal layout of members is as expected.

So, the `struct` definition above lets us create tuples, but there's not much else we can do with them yet. The way to access tuples is with the `get` function template [[5\]](https://eli.thegreenplace.net/2014/variadic-templates-in-c/#id11), so let's see how it works. First, we'll have to define a helper type that lets us access the type of the k-th element in a tuple:

```c++
template <size_t, class> struct elem_type_holder;

template <class T, class... Ts>
struct elem_type_holder<0, tuple<T, Ts...>> {
  typedef T type;
};

template <size_t k, class T, class... Ts>
struct elem_type_holder<k, tuple<T, Ts...>> {
  typedef typename elem_type_holder<k - 1, tuple<Ts...>>::type type;
};
```

> NOTE: `elem_type_holder`的模板参数`template <class T, class... Ts>`是`tuple`的模板参数。模板参数`k`用于控制我们要**剥离**几层，其实它非常类似于在维基百科[Template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming)的Compile-time class generation段中列举的`factorial`例子。上诉例子只不过是添加了一个**剥离**的机制。

`elem_type_holder` is yet another variadic class template. It takes a number `k` and the `tuple` type we're interested in as template parameters. Note that this is a compile-time template metaprogramming construct - it acts on **constants** and **types**, not on runtime objects. For example, given `elem_type_holder<2, some_tuple_type>`, we'll get the following pseudo expansion:

```c++
struct elem_type_holder<2, tuple<T, Ts...>> {
  typedef typename elem_type_holder<1, tuple<Ts...>>::type type;
}

struct elem_type_holder<1, tuple<T, Ts...>> {
  typedef typename elem_type_holder<0, tuple<Ts...>>::type type;
}

struct elem_type_holder<0, tuple<T, Ts...>> {
  typedef T type;
}
```



So the `elem_type_holder<2, some_tuple_type>` peels off two types from the beginning of the tuple, and sets its `type` to the type of the third one, which is what we need. Armed with this, we can implement `get`:

```c++
template <size_t k, class... Ts>
typename std::enable_if<
    k == 0, typename elem_type_holder<0, tuple<Ts...>>::type&>::type
get(tuple<Ts...>& t) {
  return t.tail;
}

template <size_t k, class T, class... Ts>
typename std::enable_if<
    k != 0, typename elem_type_holder<k, tuple<T, Ts...>>::type&>::type
get(tuple<T, Ts...>& t) {
  tuple<Ts...>& base = t;
  return get<k - 1>(base);
}
```

Here, [enable_if](http://eli.thegreenplace.net/2014/sfinae-and-enable_if/) is used to select between two template overloads of `get` - one for when `k` is zero, and one for the general case which peels off the first type and recurses, as usual with variadic function templates.

Since it returns a reference, we can use `get` to both read tuple elements and write to them:

```c++
tuple<double, uint64_t, const char*> t1(12.2, 42, "big");

std::cout << "0th elem is " << get<0>(t1) << "\n";
std::cout << "1th elem is " << get<1>(t1) << "\n";
std::cout << "2th elem is " << get<2>(t1) << "\n";

get<1>(t1) = 103;
std::cout << "1th elem is " << get<1>(t1) << "\n";
```

## Variadic templates for catch-all functions

Here is another example I find interesting. It's different from the ones already shown in the article, because it doesn't really use the traditional recursive approach of implementing variadic templates. Rather, it uses them to express the "any template parameters can go here" concept.

Say we want to write a function that can print out standard library containers. We want it to work for any container, and we also want the user to type as little as possible, so we don't want to act on iterators. We just want `print_container(c)` to work for any container `c`. Here's a first approach:

```c++
template <template <typename, typename> class ContainerType,
          typename ValueType,
          typename AllocType>
void print_container(const ContainerType<ValueType, AllocType>& c) {
  for (const auto& v : c) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
}
```

Many of the STL containers are templates that can be parameterized by the value type and an allocator type; for example `vector`, `list`, `deque`, and so on. So we can write:

```c++
std::vector<double> vd{3.14, 8.1, 3.2, 1.0};
print_container(vd);

std::list<int> li{1, 2, 3, 5};
print_container(li);
```

And this works as expected. However, if we try to use it for `map`, we get a compile error:

```
std::map<std::string, int> msi{{"foo", 42}, {"bar", 81}, {"bazzo", 4}};
print_container(msi);
^~~~~~~~~~~~~~~
error: no matching function for call to 'print_container'
note: candidate template ignored: substitution failure :
      template template argument has different template
      parameters than its corresponding template template parameter
```

This is because `map` is a template parameterized by 4 template arguments, not 2. The same problem would occur for a `set`, which has 3 template arguments. This is annoying - while the contents of the `print_container`function would be the same for all these containers, the signature has to be different. What can we do without duplicating code? Variadic templates for the rescue:

```c++
template <template <typename, typename...> class ContainerType,
          typename ValueType, typename... Args>
void print_container(const ContainerType<ValueType, Args...>& c) {
  for (const auto& v : c) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
}
```

What this says is - `ContainerType` is a template template parameter with any amount of template parameters itself. We don't care really, as long as the compiler can type-deduce them at the call. This version of the function will work for `map`, `set`, `unordered_map` and other containers [[6\]](https://eli.thegreenplace.net/2014/variadic-templates-in-c/#id12). One small addition we have to make to support mappings is:

```c++
// Implement << for pairs: this is needed to print out mappings where range
// iteration goes over (key, value) pairs.
template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& p) {
  out << "[" << p.first << ", " << p.second << "]";
  return out;
}
```



> NOTE: 完整例子如下：

```c++
#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<string>

template <template <typename, typename...> class ContainerType,
          typename ValueType, typename... Args>
void print_container(const ContainerType<ValueType, Args...>& c) {
  for (const auto& v : c) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
}

// Implement << for pairs: this is needed to print out mappings where range
// iteration goes over (key, value) pairs.
template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& p) {
  out << "[" << p.first << ", " << p.second << "]";
  return out;
}

int main()
{
std::vector<double> vd{3.14, 8.1, 3.2, 1.0};
print_container(vd);
std::list<int> li{1, 2, 3, 5};
print_container(li);
std::map<std::string, int> msi{{"foo", 42}, {"bar", 81}, {"bazzo", 4}};
print_container(msi);
}
```

> NOTE: 编译：`g++ --std=c++11 test.cpp`
>
> 输出：
>
> ```
> 3.14 8.1 3.2 1 
> 1 2 3 5 
> [bar, 81] [bazzo, 4] [foo, 42]
> ```
>
> 

> NOTE: 上诉程序的另外一种写法如下：

```c++
#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<string>
template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& p) {
  out << "[" << p.first << ", " << p.second << "]";
  return out;
}
template<typename CT>
void print_container(const CT& c) {
  for (const auto& v : c) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
}

int main()
{
std::vector<double> vd{3.14, 8.1, 3.2, 1.0};
print_container(vd);
std::list<int> li{1, 2, 3, 5};
print_container(li);
std::map<std::string, int> msi{{"foo", 42}, {"bar", 81}, {"bazzo", 4}};
print_container(msi);

}

```



## Variadic templates for forwarding

A somewhat related example is templates that don't do much on their own, but have to forward all their arguments to some other template or function. This turns out to be very useful because C++ has a commonly used construct that is inherently "variadic" when viewed from a template parameter point of view - the constructor. Given a generic type `T`, to invoke the constructor of `T`, we may need to pass in an arbitrary number of arguments. Unlike function types that specify their arguments at compile time, given just a generic type `T` we don't know which constructor(s) it has and how many arguments the constructor accepts.

A very important example of this is the `std::make_unique` function, available in the standard library since C++14. We want to be able to use it as follows:

```c++
std::unique_ptr<FooType> f = std::make_unique<FooType>(1, "str", 2.13);
```

`FooType` is an arbitrary type and can be constructed in arbitrary ways. How does `make_unique` know the signature of its constructor? With variadic templates, it doesn't have to know! Here's how `make_unique` is typically implemented:

```c++
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

Ignore the `&&` syntax and `std::forward` for now; I will cover them in a future article. What's important for the sake of our current discussion is the use of a variadic template to convey "any amount of arguments can go here" and passing them through to the constructor of `c` in the `new` expression.

## Links to resources

I found a number of resources useful while preparing this article:

1. The 4th edition of "The C++ Programming Language" (by Bjarne Stroustrup) has a good discussion of variadic templates in chapter 28.
2. [This StackOverflow thread](http://stackoverflow.com/a/14311714/8206) for the `print_container` example and also for mentioning `__PRETTY_FUNCTION__`in the context of variadic templates.
3. [C++ variadic templates, for the doubters](http://lbrandy.com/blog/2013/03/variadic_templates/) by Louis Brandy.
4. Andrei Alexandrescu's "Variadic templates are funadic" talk from Going Native 2012 was very useful - it's also the basis for my `tuple` example implementation.
5. Finally, if recursive use of variadic templates reminds you of pattern matching in functional languages, you're right! [Bartosz Milewski's article](http://bartoszmilewski.com/2009/10/21/what-does-haskell-have-to-do-with-c/) dives into more details about this.

------

