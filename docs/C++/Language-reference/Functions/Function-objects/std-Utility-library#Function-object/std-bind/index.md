# `std::bind`

1、partial function，即会留下一些形式参数

2、完全bind，比如在 [progschj](https://github.com/progschj)/**[ThreadPool](https://github.com/progschj/ThreadPool)** 中:

```C++
std::bind(std::forward<F>(f), std::forward<Args>(args)...)
```





## stackoverflow [std::function and std::bind: what are they, and when should they be used?](https://stackoverflow.com/questions/6610046/stdfunction-and-stdbind-what-are-they-and-when-should-they-be-used)



### A

`std::bind` is for [partial function application](http://en.wikipedia.org/wiki/Partial_application).

That is, suppose you have a function object `f` which takes 3 arguments:

```c++
f(a,b,c);
```

You want a new function object which only takes two arguments, defined as:

```c++
g(a,b) := f(a, 4, b);
```

`g` is a "partial application" of the function `f`: the middle argument has already been specified, and there are two left to go.

You can use `std::bind` to get `g`:

```
auto g = bind(f, _1, 4, _2);
```

This is more concise than actually writing a functor class to do it.

There are further examples in the article you link to. You generally use it when you need to pass a functor to some algorithm. You have a function or functor that *almost* does the job you want, but is more configurable (i.e. has more parameters) than the algorithm uses. So you bind arguments to some of the parameters, and leave the rest for the algorithm to fill in:

```
// raise every value in vec to the power of 7
std::transform(vec.begin(), vec.end(), some_output, std::bind(std::pow, _1, 7));
```

Here, `pow` takes two parameters and can raise to *any* power, but all we care about is raising to the power of 7.

As an occasional use that isn't partial function application, `bind` can also re-order the arguments to a function:

```
auto memcpy_with_the_parameters_in_the_right_flipping_order = bind(memcpy, _2, _1, _3);
```

I don't recommend using it just because you don't like the API, but it has potential practical uses for example because:

```
not2(bind(less<T>, _2, _1));
```

is a less-than-or-equal function (assuming a total order, blah blah). This example normally isn't necessary since there already is a `std::less_equal` (it uses the `<=` operator rather than `<`, so if they aren't consistent then you might need this, and you might also need to visit the author of the class with a cluestick). It's the sort of transformation that comes up if you're using a functional style of programming, though.



## cppreference [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind)





### Notes

> NOTE: 
>
> 1、这段`Notes`回答了不少问题

#### Bind to object

As described in [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable), when invoking a pointer to non-static member function or pointer to non-static data member, the first argument has to be a reference or pointer (including, possibly, smart pointer such as [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) and [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)) to an object whose member will be accessed.

#### Argument passing

The arguments to bind are copied or moved, and are never passed by reference unless wrapped in [std::ref](https://en.cppreference.com/w/cpp/utility/functional/ref) or [std::cref](https://en.cppreference.com/w/cpp/utility/functional/ref).

> NOTE: 
>
> 1、关于pass-by-reference，参见下面的 "stackoverflow [How to bind function to an object by reference?](https://stackoverflow.com/questions/26187192/how-to-bind-function-to-an-object-by-reference)"

#### Duplicate placeholders

Duplicate placeholders in the same bind expression (multiple `_1`'s for example) are allowed, but the results are only well defined if the corresponding argument (`u1`) is an lvalue or non-movable rvalue.





## stackoverflow [How to bind function to an object by reference?](https://stackoverflow.com/questions/26187192/how-to-bind-function-to-an-object-by-reference)

A

