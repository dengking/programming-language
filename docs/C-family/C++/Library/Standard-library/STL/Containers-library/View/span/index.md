# `std::span`

在阅读 [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#main) 的时候，可以发现，`gsl::span` 贯穿始终，下面是直接使用`gsl::span` 的一些guideline: 

[P.3: Express intent](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#p3-express-intent)

[I.13: Do not pass an array as a single pointer](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#i13-do-not-pass-an-array-as-a-single-pointer)

[F.24: Use a `span<T>` or a `span_p<T>` to designate a half-open sequence](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f24-use-a-spant-or-a-span_pt-to-designate-a-half-open-sequence)

[R.14: Avoid `[]` parameters, prefer `span`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-ap)

[ES.42: Keep use of pointers simple and straightforward](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-ap)





## stackoverflow [What is a “span” and when should I use one?](https://stackoverflow.com/questions/45723819/what-is-a-span-and-when-should-i-use-one)

Recently I've gotten suggestions to use `span<T>`'s in my code, or have seen some answers here on the site which use `span`'s - supposedly some kind of container. But - I can't find anything like that in the C++17 standard library.

So what is this mysterious `span<T>`, and why (or when) is it a good idea to use it if it's non-standard?

**comments**

1、`std::span` was proposed in 2017. It applies to C++17 or C++20. Also see [P0122R5, span: bounds-safe views for sequences of objects](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0122r5.pdf). Do you really want to target that language? It will be years before compilers catch up. – [jww](https://stackoverflow.com/users/608639/jww) [Jan 24 '18 at 0:29](https://stackoverflow.com/questions/45723819/what-is-a-span-and-when-should-i-use-one#comment83815969_45723819)

### [A](https://stackoverflow.com/a/45723820)

#### What is it?

A `span<T>` is:

1、A very lightweight abstraction of a contiguous sequence of values of type `T` somewhere in memory.

2、Basically a `struct { T * ptr; std::size_t length; }` with a bunch of convenience methods.

3、A non-owning type (i.e. a ["reference-type"](https://stackoverflow.com/questions/24827592/what-is-definition-of-reference-type) rather than a "value type"): It never allocates nor deallocates anything and does not keep smart pointers alive.

It was formerly known as an [`array_view`](https://stackoverflow.com/q/34832090/1593077) and even earlier as [`array_ref`](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3334.html).

#### When should I use it?

First, when *not* to use it:

1、Don't use it in code that could just take any pair of start `&` end iterators, like `std::sort`, `std::find_if`, `std::copy` and all of those super-generic templated functions.

> NOTE: why？

2、Don't use it if you have a standard library container (or a Boost container etc.) which you know is the right fit for your code. It's not intended to supplant(替代) any of them.



Now for when to actually use it:

> Use `span<T>` (respectively, `span<const T>`) instead of a free-standing `T*` (respectively `const T*`) when the allocated length or size also matter. So, replace functions like:
>
> ```cpp
>   void read_into(int* buffer, size_t buffer_size);
> ```
>
> with:
>
> ```cpp
>   void read_into(span<int> buffer);
> ```

#### Why should I use it? Why is it a good thing?

Oh, spans are awesome! Using a `span`...

1、means that you can work with that pointer+length / start+end pointer combination like you would with a fancy, pimped-out standard library container, e.g.:

- `for (auto& x : my_span) { /* do stuff */ }`

- `std::find_if(my_span.cbegin(), my_span.cend(), some_predicate);`

- `std::ranges::find_if(my_span, some_predicate);` (in C++20)

    

... but with absolutely none of the overhead most container classes incur.

2、lets the compiler do more work for you sometimes. For example, this:

```cpp
  int buffer[BUFFER_SIZE];
  read_into(buffer, BUFFER_SIZE);
```

becomes this:

```cpp
  int buffer[BUFFER_SIZE];
  read_into(buffer);
```

... which will do what you would want it to do. See also [Guideline P.5](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p5-prefer-compile-time-checking-to-run-time-checking).

3、is the reasonable alternative to passing `const vector<T>&` to functions when you expect your data to be contiguous in memory. No more getting scolded by high-and-mighty C++ gurus!

> NOTE: 最后一句的意思是: 再也不会被那些高高在上的c++大师骂了!

4、facilitates static analysis, so the compiler might be able to help you catch silly bugs.

5、allows for debug-compilation instrumentation for runtime bounds-checking (i.e. `span`'s methods will have some bounds-checking code within `#ifndef NDEBUG` ... `#endif`)

6、indicates that your code (that's using the span) doesn't own the pointed-to memory.

There's even more motivation for using `span`s, which you could find in the [C++ core guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) - but you catch the drift.

#### But is it in the standard library?

**edit:** Yes, [`std::span`](https://en.cppreference.com/w/cpp/container/span) was added to C++ with the C++20 version of the language!

Why only in C++20? Well, While the idea is not new - its current form was conceived in conjunction with the [C++ core guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) project, which only started taking shape in 2015. So it took a while.

#### So how do I use it if I'm writing C++17 or earlier?

It's part of the [Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p3-express-intent)'s Support Library (GSL). Implementations:

1、Microsoft / Neil Macintosh's [GSL](https://github.com/Microsoft/GSL) contains a standalone implementation: [`gsl/span`](https://github.com/Microsoft/GSL/blob/master/include/gsl/span)

2、[GSL-Lite](https://github.com/martinmoene/gsl-lite) is a single-header implementation of the whole GSL (it's not that big, don't worry), including `span<T>`.

The GSL implementation does generally assume a platform that implements C++14 support [[11](https://github.com/microsoft/GSL/blob/7d78b743e43ecba06ca47426d03d9d16076dec16/README.md#L9)]. These alternative single-header implementations do not depend on GSL facilities:

1、[`martinmoene/span-lite`](https://github.com/martinmoene/span-lite) requires C++98 or later

2、[`tcbrindle/span`](https://github.com/tcbrindle/span) requires C++11 or later

Note that these different span implementations have some differences in what methods/support functions they come with; and they may also differ somewhat from the version adopted into the standard library in C++20.

------

**Further reading:** You can find all the details and design considerations in the final official proposal before C++17, P0122R7: [span: bounds-safe views for sequences of objects](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0122r7.pdf) by Neal Macintosh and Stephan J. Lavavej. It's a bit long though. Also, in C++20, the span comparison semantics changed (following [this short paper](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1085r2.md) by Tony van Eerd).

## Implementation



### [microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)**/[include](https://github.com/microsoft/GSL/tree/master/include)/[gsl](https://github.com/microsoft/GSL/tree/master/include/gsl)/[**span**](https://github.com/Microsoft/GSL/blob/master/include/gsl/span)

### martinmoene [GSL-Lite](https://github.com/martinmoene/gsl-lite) 