# enable_if



## boost [enable_if](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html)

> NOTE: 这篇文章讲到了精髓



## thegreenplace [SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/)

```C++
#include <iostream>
void foo(unsigned i) {
  std::cout << "unsigned " << i << "\n";
}

template <typename T>
void foo(const T& t) {
  std::cout << "template " << t << "\n";
}

int main()
{
    foo(42);
    return 0;
}
```

> NOTE: 编译`g++ test.cpp`，输出为：
>
> template 42

What do you think a call to `foo(42)` would print? The answer is `"template 42"`, and the reason for this is that integer literals are signed by default (they only become unsigned with the `U` suffix).

### SFINAE

The standard states:

> If a substitution results in an invalid type or expression, type deduction fails. An invalid type or expression is one that would be ill-formed if written using the **substituted arguments**. Only invalid types and expressions in the **immediate context** of the function type and its template parameter types can result in a **deduction failure**.

> NOTE: 上面这段话的意思是：如果使用模板实参（**substituted arguments**）替换模板形参而生成的code是ill-formed的，那么这就是invalid type or expression。

And then goes on to list the possible scenarios that are deemed invalid, such as using a type that is not a class or enumeration type in a qualified name, attempting to create a reference to `void`, and so on.

But wait, what does it mean by the last sentence about "immediate context"? Consider this (non-sensical) example:

```c++
template <typename T>
void negate(const T& t) {
  typename T::value_type n = -t();
}
```

If **type deduction** matches this overload for some fundamental type, we'll actually get a compile error due to the `T::value_type` inside the function body. This is outside of the "immediate context of the function type and its template parameter types" mentioned by the standard. The lesson here is that if we want to write a template that only makes sense for some types, we must make it fail deduction for invalid types right in the declaration, to cause substitution failure. If the invalid type sneaks past the overload candidate selection phase, the program won't compile.

> NOTE: 上面这段话没有搞得很懂，但是它的意思大概是：Only invalid types and expressions in the **immediate context** of the function type and its template parameter types can result in a **deduction failure**，所以如果we want to write a template that only makes sense for some types, we must make it fail deduction for invalid types right in the declaration, to cause substitution failure. If the invalid type sneaks past the overload candidate selection phase, the program won't compile. 关于如何实现这个，在下一段中对此进行了介绍：

### enable_if - a compile-time switch for templates

SFINAE has proved so useful that programmers started to *explicitly* rely on it very early on in the history of C++. One of the most notable tools used for this purpose is `enable_if`. 

> NOTE: 这段话说了SFINAE额`enable_if`之间的关系

It can be defined as follows:

```c++
template <bool, typename T = void>
struct enable_if
{};

template <typename T>
struct enable_if<true, T> {
  typedef T type;
};
```



And now we can do things like [[2\]](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/#id6):

```c++
template <class T,
         typename std::enable_if<std::is_integral<T>::value,
                                 T>::type* = nullptr>
void do_stuff(T& t) {
  std::cout << "do_stuff integral\n";
    // an implementation for integral types (int, char, unsigned, etc.)
}

template <class T,
          typename std::enable_if<std::is_class<T>::value,
                                  T>::type* = nullptr>
void do_stuff(T& t) {
    // an implementation for class types
}
```

Note SFINAE at work here. When we make the call `do_stuff()`, the compiler selects the first overload: since the condition `std::is_integral` is `true`, the specialization of `struct enable_if` for `true` is used, and its internal `type` is set to `int`. The second overload is omitted because without the `true` specialization (`std::is_class` is `false`) the general form of `struct enable_if` is selected, and it doesn't have a `type`, so the type of the argument results in a **substitution failure**.

`enable_if` has been part of Boost for many years, and since C++11 it's also in the standard C++ library as `std::enable_if`. Its usage is somewhat verbose though, so C++14 adds this type alias for convenience:

```c++
template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;
```

With this, the examples above can be rewritten a bit more succinctly（简洁的）:

```c++
template <class T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
void do_stuff(T& t) {
    // an implementation for integral types (int, char, unsigned, etc.)
}

template <class T,
          typename std::enable_if_t<std::is_class<T>::value>* = nullptr>
void do_stuff(T& t) {
    // an implementation for class types
}
```

### Uses of enable_if

`enable_if` is an extremely useful tool. There are hundreds of references to it in the C++11 standard template library. It's so useful because it's a key part in using *type traits*, a way to restrict templates to types that have certain properties. Without `enable_if`, templates are a rather blunt "catch-all" tool. If we define a function with a template argument, this function will be invoked on all possible types. Type traits and `enable_if` let us create different functions that act on different kinds of types, while still remaining generic [[3\]](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/#id7).

One usage example I like is the two-argument constructor of `std::vector`:

```c++
// Create the vector {8, 8, 8, 8}
std::vector<int> v1(4, 8);

// Create another vector {8, 8, 8, 8}
std::vector<int> v2(std::begin(v1), std::end(v1));

// Create the vector {1, 2, 3, 4}
int arr[] = {1, 2, 3, 4, 5, 6, 7};
std::vector<int> v3(arr, arr + 4);
```

There are two forms of the two-argument constructor used here. Ignoring allocators, this is how these constructors could be declared:

```c++
template <typename T>
class vector {
    vector(size_type n, const T val);

    template <class InputIterator>
    vector(InputIterator first, InputIterator last);

    ...
}
```

Both constructors take two arguments, but the second one has the catch-all property of templates. Even though the template argument `InputIterator` has a **descriptive name**, it has no **semantic meaning** - the compiler wouldn't mind if it was called `ARG42` or `T`. The problem here is that even for `v1`, the *second* constructor would be invoked if we didn't do something special. This is because the type of `4` is `int` rather than `size_t`. So to invoke the first constructor, the compiler would have to perform a type conversion. The second constructor would fit perfectly though.

So how does the library implementor avoid this problem and make sure that the second constructor is only called for iterators? By now we know the answer - with `enable_if`.

Here is how the second constructor is *really* defined:

```c++
template <class _InputIterator>
vector(_InputIterator __first,
       typename enable_if<__is_input_iterator<_InputIterator>::value &&
                          !__is_forward_iterator<_InputIterator>::value &&
                          ... more conditions ...
                          _InputIterator>::type __last);
```

It uses `enable_if` to only enable this overload for types that are input iterators, though not forward iterators. For forward iterators, there's a separate overload, because the constructors for these can be implemented more efficiently.

As I mentioned, there are many uses of `enable_if` in the C++11 standard library. The `string::append` method has a very similar use to the above, since it has several overloads that take two arguments and a template overload for iterators.

A somewhat different example is `std::signbit`, which is supposed to be defined for all arithmetic types (integer or floating point). Here's a simplified version of its declaration in the `cmath` header:

```c++
template <class T>
typename std::enable_if<std::is_arithmetic<T>, bool>::type
signbit(T x)
{
    // implementation
}
```

Without using `enable_if`, think about the options the library implementors would have. One would be to overload the function for each of the known arithmetic type. That's very verbose. Another would be to just use an unrestricted template. But then, had we actually passed a wrong type into it, say `std::string`, we'd most likely get a fairly obscure error *at the point of use*. With `enable_if`, we neither have to write boilerplate, nor to produce bad error messages. If we invoke `std::signbit` as defined above with a bad type we'll get a fairly helpful error saying that a suitable function cannot be found.

## A more advanced version of enable_if

Admittedly, `std::enable_if` is clumsy, and even `enable_if_t` doesn't help much, though it's a bit less verbose. You still have to mix it into the declaration of a function in a way that often obscures the return type or an argument type. This is why some sources online suggest crafting more advanced versions that "get out of the way". Personally, I think this is the wrong tradeoff to make.

`std::enable_if` is a rarely used construct. So making it less verbose doesn't buy us much. On the other hand, making it more mysterious is detrimental, because every time we see it we have to think about how it works. The implementation shown here is fairly simple, and I'd keep it this way. Finally I'll note that the C++ standard library uses the verbose, "clumsy" version of `std::enable_if` without defining more complex versions. I think that's the right decision.





## `std::enable_if`

cppreference [std::enable_if](https://en.cppreference.com/w/cpp/types/enable_if)

cplusplus [std::enable_if](http://www.cplusplus.com/reference/type_traits/enable_if/)







## Usage

[std::enable_if to conditionally compile a member function](https://stackoverflow.com/questions/6972368/stdenable-if-to-conditionally-compile-a-member-function)

