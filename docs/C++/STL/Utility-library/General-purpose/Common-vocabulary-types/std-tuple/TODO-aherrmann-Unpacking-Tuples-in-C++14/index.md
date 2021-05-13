# aherrmann [Unpacking Tuples in C++14](http://aherrmann.github.io/programming/2016/02/28/unpacking-tuples-in-cpp14/)

C++11 introduced tuples to the C++ standard library. As the [documentation](http://en.cppreference.com/w/cpp/utility/tuple) says they offer a *fixed-size collection of heterogeneous values*. 

Unfortunately, tuples can be a little bit tricky to deal with in a generic fashion. The C++14 standard introduced a few features that greatly reduce the necessary boilerplate. In this post I will discuss how to deal with tuples with very compact code.

If you would like to follow along you can find the code examples on [GitHub](https://github.com/aherrmann/unpacking_tuples_examples). Build instructions can be found in the Readme file.

## Introducing Tuples

The difficulty with tuples in `C++` is that they can only be indexed at **compile time**. The standard library function [`get`](http://en.cppreference.com/w/cpp/utility/tuple/get) accepts the index as a template parameter (i.e. at **compile time**) and returns a reference to the value at that index. The index has to be a **constant expression**. It cannot be dynamically generated as e.g. in a for-loop. Furthermore, since tuples can have heterogeneous values and C++ is a statically typed language there is no way to dynamically iterate over the values in a generic tuple. We wouldn’t know their types.

```C++
#include <tuple>
using namespace std;
int main()
{
	tuple<int, bool, char> t = make_tuple(1, true, 'a');

	int n = get<0>(t); // ok
	bool b = get<1>(t); // ok
	char c = get<2>(t); // ok

	for (int i = 0; i < 3; ++i)
	{
		get<i>(t); // error: `i` is not `constexpr`
	}
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述代码编译报错如下:
>
> ```C++
> /usr/include/c++/4.8.2/tuple:773:5: 附注：template<long unsigned int __i, class ... _Elements> constexpr typename std::__add_r_ref<typename std::tuple_element<__i, std::tuple<_Elements ...> >::type>::type std::get(std::tuple<_Elements ...>&&)
>      get(tuple<_Elements...>&& __t) noexcept
>      ^
> /usr/include/c++/4.8.2/tuple:773:5: 附注：  template argument deduction/substitution failed:
> test.cpp:13:11: 错误：the value of ‘i’ is not usable in a constant expression
>    get<i>(t); // error: `i` is not `constexpr`
>            ^
> test.cpp:11:11: 附注：‘int i’不是 const
>   for (int i = 0; i < 3; ++i)
>            ^
> test.cpp:13:11: 附注：in template argument for type ‘long unsigned int’ 
>    get<i>(t); // error: `i` is not `constexpr`
> ```
>
> 

The problem can be circumvented by exploiting [variadic templates, and parameter pack expansion](http://en.cppreference.com/w/cpp/language/parameter_pack). A feature that was also introduced in C++11. It allows to, in a way, iterate over tuple elements at compile time. First, we need to define a type that can hold a **parameter pack of indices** for a given tuple.

```C++
template <size_t... Is>
struct index_sequence;
```

Next, we can use parameter pack expansion to index into a generic tuple.

```C++
template<class Tuple, size_t ... Is>
void function(Tuple t, index_sequence<Is...>)
{
	some_func(get<Is>(t)...);
}

```

Which would call `some_func` with the tuple’s elements unpacked into its argument list.

What’s left is to construct an `index_sequence` that contains the appropriate parameter pack of indices. The C++14 standard introduced [`make_index_sequence`](http://en.cppreference.com/w/cpp/utility/integer_sequence) for that purpose. Before that C++ programmers had to implement it themselves or pick one of the many implementations available on the Internet. E.g. [this](http://stackoverflow.com/a/17426611/841562) $O(\log(N))$ implementation on Stack Overflow.



## Implementing Functions on Tuples

With all these tools available in the standard-library we can stop worrying, go ahead, and play with tuples to our heart’s content.

### `take_front`

Suppose we wanted to implement a function that takes an arbitrary tuple and returns a new tuple that holds the first `N` elements of the original tuple. Let’s call it `take_front`. Since tuples have fixed size the parameter `N` will have to be a template parameter.

```C++
template<class Tuple, size_t ... Is>
constexpr auto take_front_impl(Tuple t, index_sequence<Is...>)
{
	return make_tuple(get<Is>(t)...);
}

template<size_t N, class Tuple>
constexpr auto take_front(Tuple t)
{
	return take_front_impl(t, make_index_sequence<N> { });
}

```

The function `take_front_impl` takes the input tuple and an `index_sequence`. As before that second parameter is only there so that we can get our hands on a parameter pack of indices. We then use these indices to get the elements of the input tuple and pass them to `make_tuple` which will construct the result. However, at that point we haven’t actually defined, yet, which elements should be put into that new tuple. This happens within `take_front`, which constructs an index-sequence consisting of the indices `0` to `N-1` and passes it to `take_front_impl`.

We can use that function like so.

```C++
auto t = take_front < 2 > (make_tuple(1, 2, 3, 4));
assert(t == make_tuple(1, 2));

```

At this point I should mention that all the code in this article is optimized for readability. In production code you would probably want to qualify members of the `std` namespace, and use [perfect forwarding](http://en.cppreference.com/w/cpp/utility/forward#Example). You should also be aware that the function [`make_tuple`](http://en.cppreference.com/w/cpp/utility/tuple/make_tuple) applies non-trivial transformations to its arguments, such as converting references to values and [`reference_wrapper`](http://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) to references.

### `apply`

With that out of the way, let’s implement another function on tuples. A very useful function that we might want to implement is `apply`. It takes a tuple and a callable and calls the callable with the elements of the tuple as arguments. It could for example be used in the following way.

```C++
auto x = apply(make_tuple(1, 2), plus<> { });
assert(x == 3);

```

The implementation uses the same trick as `take_front` before. We construct a parameter pack of integers and use a helper function to extract all the tuple elements.

```C++
template<class Tuple, class F, size_t ... Is>
constexpr auto apply_impl(Tuple t, F f, index_sequence<Is...>)
{
	return f(get<Is>(t)...);
}

template<class Tuple, class F>
constexpr auto apply(Tuple t, F f)
{
	return apply_impl(t, f, make_index_sequence<tuple_size<Tuple> { }> { });
}

```

This function is actually part of the [library fundamentals technical specification](http://en.cppreference.com/w/cpp/experimental/apply). Note, however, that I swapped the order of the arguments. It’s a matter of taste but I prefer callable arguments in the end of the parameter list because it allows for more readable in-line lambda definitions.



## Don’t Split Your Functions

Both of the above functions `take_front`, and `apply` are implemented using the same pattern. We first call `make_index_sequence` to construct an `index_sequence` which holds a **parameter pack of indices**. Then we call a helper function that **accesses and unpacks that parameter pack**. Unfortunately, this splits the function’s body in two pieces which makes the code harder to read. It is often said that patterns **hint at**(暗示着) a missing language feature. One could argue that the inability to create(**parameter pack**) and immediately unpack **parameter packs** in the same place is a lacking language feature. However, here I want to discuss how to, at least, localize that pattern such that we don’t need to define **helper functions** outside of scope.

> NOTE: 前面描述的patter是"split"的含义是: 必须定义两个函数，第一个函数创建**parameter pack of indices**(`index_sequence<Is...>`)，将这个pack作为入参传入到第二个函数(**helper functions**)中，在第二个函数中"**accesses and unpacks that parameter pack**"，显然这就要求programmer "splits the function’s body in two pieces which makes the code harder to read"
>
> 在这一段，作者提出了一个新的更好的思路:  "create(**parameter pack**) and immediately unpack **parameter packs** in the same place"

C++14 introduced another great feature, namely, **variadic lambdas**. That feature allows to define a lambda that behaves like a **variadic template function**. For example the following lambda returns the smallest absolute value of all given parameters.

```C++
    auto minabs = [](auto... xs) {
        return min({abs(xs)...});
    };

    assert(1 == minabs(-1, 2, -3));
```

This implementation uses the initializer list overload of [`min`](http://en.cppreference.com/w/cpp/algorithm/min).

Now, how could we use **variadic lambdas** to avoid the `*_impl` pattern from above? 

### First, naive, approach 

A first, naive, approach follows. First, we try to separate the idea of constructing an **index sequence** in one place and **unpacking** it in another.

```C++
template <class F, size_t... Is>
constexpr auto index_apply_impl(F f,
                                index_sequence<Is...>) {
    return f(Is...);
}

template <size_t N, class F>
constexpr auto index_apply(F f) {
    return index_apply_impl(f, make_index_sequence<N>{});
}

```

The function `index_apply` expects a callable and passes it to a helper function alongside a parameter pack of indices from `0` to `N-1`. That helper function then passes these indices as arguments to the callable. We could now try to implement `take_front` as follows.

```c++
template <size_t N, class Tuple>
constexpr auto take_front(Tuple t) {
    return index_apply<N>([&](auto... Is) {
        return make_tuple(get<Is>(t)...);
    });
}
```

This already looks very promising. We have eliminated the need for a specific helper function and can instead rely on one general helper for (hopefully) all cases. However, unfortunately, that code will not compile. The `get` template takes the index as a **template parameter**. **Template parameters** can only be [constant expressions](http://en.cppreference.com/w/cpp/language/constant_expression). However, the arguments `Is...` to the lambda are ordinary (run-time) values of type `size_t`. Therefore, we cannot use them as template parameters.

### Second, complete, approach

Fortunately, there is an easy way around that problem. The standard library defines the template class [`integral_constant`](http://en.cppreference.com/w/cpp/types/integral_constant) which encapsulates a **static constant** of a specified type. Since it carries its value in a **template parameter** that value is a **constant expression** that can also be used as a parmeter to other templates. Conveniently, it also defines an implicit `constexpr` conversion operator such that we can use an `integral_constant` object in most places where we need a **constant expression** of the corresponding value type. With this little helper we can implement `index_apply` as follows.

```C++
template <class F, size_t... Is>
constexpr auto index_apply_impl(F f,
                                index_sequence<Is...>) {
    return f(integral_constant<size_t, Is> {}...);
}

template <size_t N, class F>
constexpr auto index_apply(F f) {
    return index_apply_impl(f, make_index_sequence<N>{});
}
```



This, finally, allows us to implement `take_front`, and `apply` without the need for any further helper functions.

```C++
template <size_t N, class Tuple>
constexpr auto take_front(Tuple t) {
    return index_apply<N>([&](auto... Is) {
        return make_tuple(get<Is>(t)...);
    });
}

template <class Tuple, class F>
constexpr auto apply(Tuple t, F f) {
    return index_apply<tuple_size<Tuple>{}>(
        [&](auto... Is) { return f(get<Is>(t)...); });
}
```

Both functions call `index_apply`, specifying how many elements we want to extract. Then they pass a variadic lambda that expects a parameter pack of indices. These indices are passed as instances of `integral_constant`. Therefore, they can be used right away as a template argument to `get`.

## A Few More Examples

Now that we have `index_apply` let’s write two more functions on tuples with its help. 

### Reverse a tuple

First, let’s write a function that takes a tuple and returns a new tuple that contains the original tuple’s elements in **reversed order**.

```c++
template<class Tuple>
constexpr auto reverse(Tuple t)
{
	return index_apply<tuple_size<Tuple>{ }>(
		[&](auto ... Is)
		{
			return make_tuple(get<tuple_size<Tuple>{} - 1 - Is>(t)...);
		}
	);
}

```

That function is nearly identical to `tuple_front` just that this time we take the full length, and count the indices that we pass to `get` backwards.

```C++
    auto t = reverse(make_tuple(1, 2, 3, 4));
    assert(t == make_tuple(4, 3, 2, 1));
```



### zip

Now, let’s move on to a more complex example. We will write a function that takes an arbitrary number of tuples and returns a tuple of tuples, where the first contains all the first elements of the input tuples, the second all the second elements, and so on. We’ll call this function `zip`. We also specify that when called with zero arguments it just returns an empty tuple. If the tuples are of differing length then we will crop all inputs to match the shortest one. All in all we expect `zip` to fulfill the following assertions.

```C++
assert(make_tuple() = zip());

auto t1 = zip(make_tuple(1, 2), make_tuple(3, 4), make_tuple(5, 6));
assert(t1 == make_tuple( make_tuple(1, 3, 5), make_tuple(2, 4, 6) ));

auto t2 = zip(make_tuple(1, 2, 3), make_tuple(4));
assert(t2 == make_tuple( make_tuple(1, 4) ));

```

It would also make sense to implement the function `transpose` in terms of `zip`, which takes a tuple of tuples, interprets it like a matrix, and returns a transposed tuple of tuples.

```C++
template<class Tuple>
constexpr auto transpose(Tuple t)
{
	return apply(t, [](auto ... ts)
	{	return zip(ts...);});
}

```

How could we implement `zip`? There are a few things that we need. First, we need the length of the shortest tuple. For that purpose we can use `min` again.

```C++
constexpr size_t len = min({tuple_size<Tuples>{}...});
```

Where we used the fact that `min` is a `constexpr` function since C++14.

Next, we need to find a way to go through every tuple and every tuple’s elements simultaneously. Unfortunately, the following, naive, implementation will not compile.

```C++
template<class ... Tuples>
constexpr auto zip(Tuples ... ts)
{
	constexpr size_t len = min( { tuple_size<Tuples> {}... });
	return index_apply<len>([&](auto ... Is)
	{
		return make_tuple(make_tuple(get<Is>(ts)...)...);
	});
}

```

We somehow need to nest two parameter pack expansions. However, in the above code it is ambiguous which pack to expand first. Instead of taking a guess for us the compiler will (thankfully) simply refuse to accept this code.

We can circumvent that problem by splitting the task in two. We can think of the result as a tuple of rows where each row contains the `I`-th elements of all input tuples. For each row the index is fixed. The outer tuple then contains all those rows. In code that reads as follows.

```C++
template <class... Tuples>
constexpr auto zip(Tuples... ts) {
    constexpr size_t len = min({tuple_size<Tuples>{}...});
    auto row =
        [&](auto I) { return make_tuple(get<I>(ts)...); };
    return index_apply<len>(
        [&](auto... Is) { return make_tuple(row(Is)...); });
}
```

The lambda `row` constructs a single row of `I`-th elements of all tuples. To that end it takes one `integral_constant` as an argument and uses it to extract one element from each tuple. Within `index_apply` we then construct a tuple of all rows.

Finally, to handle the empty case we simply provide the following overload.

```C++
constexpr auto zip() { return make_tuple(); }
```

With that we have implemented `zip` to the specification above.

## Conclusion

In this article we saw how to reduce the boilerplate when extracting elements out of generic tuples. The presented pattern allows to write the full implementation of a function that deals with generic patterns in one place without having to resort to external helper functions.

If you have any comments, thoughts, or criticism please leave a comment. In any case I hope you enjoyed the article. Thanks for reading!