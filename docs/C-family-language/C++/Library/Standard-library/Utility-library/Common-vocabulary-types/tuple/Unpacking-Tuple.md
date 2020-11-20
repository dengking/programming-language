# Unpacking Tuples



## aherrmann.github [Unpacking Tuples in C++14](http://aherrmann.github.io/programming/2016/02/28/unpacking-tuples-in-cpp14/)

C++11 in­tro­duced tu­ples to the C++ stan­dard li­brary. As the [doc­u­men­ta­tion](http://en.cppreference.com/w/cpp/utility/tuple) says they offer a *fixed-size col­lec­tion of het­ero­ge­neous values*. 

Un­for­tu­nately, tu­ples can be a little bit tricky to deal with in a generic fash­ion. The C++14 stan­dard in­tro­duced a few fea­tures that greatly re­duce the neces­sary boil­er­plate. In this post I will dis­cuss how to deal with tu­ples with very com­pact code.

If you would like to follow along you can find the code ex­am­ples on [GitHub](https://github.com/aherrmann/unpacking_tuples_examples). Build in­struc­tions can be found in the Readme file.

### In­tro­ducing Tu­ples

The dif­fi­culty with tu­ples in `C++` is that they can only be in­dexed at **com­pile time**. The stan­dard li­brary func­tion [`get`](http://en.cppreference.com/w/cpp/utility/tuple/get) ac­cepts the index as a tem­plate pa­ra­meter (i.e. at **com­pile time**) and re­turns a ref­er­ence to the value at that in­dex. The index has to be a **con­stant ex­pres­sion**. It cannot be dy­nam­i­cally gen­er­ated as e.g. in a for-loop. Fur­ther­more, since tu­ples can have het­ero­ge­neous values and C++ is a sta­t­i­cally typed lan­guage there is no way to dynam­i­cally it­erate over the values in a generic tu­ple. We wouldn’t know their types.

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

The problem can be cir­cum­vented by ex­ploiting [vari­adic tem­plates, and pa­ra­meter pack ex­pan­sion](http://en.cppreference.com/w/cpp/language/parameter_pack). A fea­ture that was also in­tro­duced in C++11. It al­lows to, in a way, it­erate over tuple el­e­ments at com­pile time. First, we need to de­fine a type that can hold a **pa­ra­meter pack of in­dices** for a given tu­ple.

```C++
template <size_t... Is>
struct index_sequence;
```

Next, we can use pa­ra­meter pack ex­pan­sion to index into a generic tu­ple.

```C++
template<class Tuple, size_t ... Is>
void function(Tuple t, index_sequence<Is...>)
{
	some_func(get<Is>(t)...);
}

```

Which would call `some_func` with the tu­ple’s el­e­ments un­packed into its ar­gu­ment list.

What’s left is to con­struct an `index_sequence` that con­tains the ap­pro­priate pa­ra­meter pack of in­dices. The C++14 stan­dard in­tro­duced [`make_index_sequence`](http://en.cppreference.com/w/cpp/utility/integer_sequence) for that pur­pose. Be­fore that C++ pro­gram­mers had to im­ple­ment it them­selves or pick one of the many im­ple­men­ta­tions avail­able on the In­ter­net. E.g. [this](http://stackoverflow.com/a/17426611/841562) $O(\log(N))$ im­ple­men­ta­tion on Stack Over­flow.



### Im­ple­menting Func­tions on Tu­ples

With all these tools avail­able in the stan­dard-li­brary we can stop wor­ry­ing, go ahead, and play with tu­ples to our heart’s con­tent.

#### `take_front`

Sup­pose we wanted to im­ple­ment a func­tion that takes an ar­bi­trary tuple and re­turns a new tuple that holds the first `N` el­e­ments of the orig­inal tu­ple. Let’s call it `take_front`. Since tu­ples have fixed size the pa­ra­meter `N` will have to be a tem­plate pa­ra­me­ter.

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

The func­tion `take_front_impl` takes the input tuple and an `index_sequence`. As be­fore that second pa­ra­meter is only there so that we can get our hands on a pa­ra­meter pack of in­dices. We then use these in­dices to get the el­e­ments of the input tuple and pass them to `make_tuple` which will construct the re­sult. How­ever, at that point we haven’t ac­tu­ally de­fined, yet, which el­e­ments should be put into that new tu­ple. This hap­pens within `take_front`, which con­structs an index-se­quence con­sisting of the in­dices `0` to `N-1` and passes it to `take_front_impl`.

We can use that func­tion like so.

```C++
auto t = take_front < 2 > (make_tuple(1, 2, 3, 4));
assert(t == make_tuple(1, 2));

```

At this point I should men­tion that all the code in this ar­ticle is op­ti­mized for read­abil­ity. In pro­duc­tion code you would prob­ably want to qualify mem­bers of the `std` name­space, and use [per­fect for­warding](http://en.cppreference.com/w/cpp/utility/forward#Example). You should also be aware that the func­tion [`make_tuple`](http://en.cppreference.com/w/cpp/utility/tuple/make_tuple) ap­plies non-trivial trans­for­ma­tions to its argu­ments, such as con­verting ref­er­ences to values and [`reference_wrapper`](http://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) to ref­er­ences.

#### `apply`

With that out of the way, let’s im­ple­ment an­other func­tion on tu­ples. A very useful func­tion that we might want to im­ple­ment is `apply`. It takes a tuple and a callable and calls the callable with the el­e­ments of the tuple as ar­gu­ments. It could for ex­ample be used in the fol­lowing way.

```C++
auto x = apply(make_tuple(1, 2), plus<> { });
assert(x == 3);

```

The im­ple­men­ta­tion uses the same trick as `take_front` be­fore. We con­struct a pa­ra­meter pack of in­te­gers and use a helper func­tion to ex­tract all the tuple el­e­ments.

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

This func­tion is ac­tu­ally part of the [li­brary fun­da­men­tals tech­nical spec­i­fi­ca­tion](http://en.cppreference.com/w/cpp/experimental/apply). Note, how­ever, that I swapped the order of the ar­gu­ments. It’s a matter of taste but I prefer callable ar­gu­ments in the end of the pa­ra­meter list be­cause it al­lows for more read­able in-line lambda de­f­i­n­i­tions.



### Don’t Split Your Func­tions

Both of the above func­tions `take_front`, and `apply` are im­ple­mented using the same pat­tern. We first call `make_index_sequence` to con­struct an `index_sequence` which holds a **pa­ra­meter pack of in­dices**. Then we call a helper func­tion that **ac­cesses and un­packs that pa­ra­meter pack**. Un­for­tu­nately, this splits the func­tion’s body in two pieces which makes the code harder to read. It is often said that pat­terns **hint at**(暗示着) a missing lan­guage fea­ture. One could argue that the in­ability to create(**parameter pack**) and im­me­di­ately un­pack **pa­ra­meter packs** in the same place is a lacking lan­guage fea­ture. How­ever, here I want to dis­cuss how to, at least, lo­calize that pat­tern such that we don’t need to de­fine **helper func­tions** out­side of scope.

> NOTE: 前面描述的patter是"split"的含义是: 必须定义两个函数，第一个函数创建**pa­ra­meter pack of in­dices**(`index_sequence<Is...>`)，将这个pack作为入参传入到第二个函数(**helper func­tions**)中，在第二个函数中"**ac­cesses and un­packs that pa­ra­meter pack**"，显然这就要求programmer "splits the func­tion’s body in two pieces which makes the code harder to read"
>
> 在这一段，作者提出了一个新的更好的思路:  "create(**parameter pack**) and im­me­di­ately un­pack **pa­ra­meter packs** in the same place"

C++14 in­tro­duced an­other great fea­ture, namely, **vari­adic lamb­das**. That fea­ture al­lows to de­fine a lambda that be­haves like a **vari­adic tem­plate function**. For ex­ample the fol­lowing lambda re­turns the smallest ab­solute value of all given pa­ra­me­ters.

```C++
    auto minabs = [](auto... xs) {
        return min({abs(xs)...});
    };

    assert(1 == minabs(-1, 2, -3));
```

This im­ple­men­ta­tion uses the ini­tial­izer list over­load of [`min`](http://en.cppreference.com/w/cpp/algorithm/min).

Now, how could we use **vari­adic lambdas** to avoid the `*_impl` pat­tern from above? 

#### First, naive, ap­proach 

A first, naive, ap­proach fol­lows. First, we try to sep­a­rate the idea of con­structing an **index se­quence** in one place and **un­packing** it in an­other.

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

The func­tion `index_apply` ex­pects a callable and passes it to a helper func­tion along­side a pa­ra­meter pack of in­dices from `0` to `N-1`. That helper func­tion then passes these in­dices as ar­gu­ments to the callable. We could now try to im­ple­ment `take_front` as fol­lows.

```c++
template <size_t N, class Tuple>
constexpr auto take_front(Tuple t) {
    return index_apply<N>([&](auto... Is) {
        return make_tuple(get<Is>(t)...);
    });
}
```

This al­ready looks very promis­ing. We have elim­i­nated the need for a spe­cific helper func­tion and can in­stead rely on one gen­eral helper for (hope­fully) all cases. How­ever, un­for­tu­nately, that code will not com­pile. The `get` tem­plate takes the index as a **tem­plate pa­ra­me­ter**. **Tem­plate pa­ra­me­ters** can only be [con­stant ex­pres­sions](http://en.cppreference.com/w/cpp/language/constant_expression). How­ever, the ar­gu­ments `Is...` to the lambda are or­di­nary (run-time) values of type `size_t`. There­fore, we cannot use them as tem­plate pa­ra­me­ters.

#### Second, complete, approach

For­tu­nately, there is an easy way around that prob­lem. The stan­dard li­brary de­fines the tem­plate class [`integral_constant`](http://en.cppreference.com/w/cpp/types/integral_constant) which en­cap­su­lates a **static con­stant** of a spec­i­fied type. Since it car­ries its value in a **tem­plate pa­ra­meter** that value is a **con­stant ex­pres­sion** that can also be used as a pa­r­meter to other tem­plates. Con­ve­niently, it also de­fines an im­plicit `constexpr` con­ver­sion op­er­ator such that we can use an `integral_constant` ob­ject in most places where we need a **con­stant ex­pres­sion** of the cor­re­sponding value type. With this little helper we can im­ple­ment `index_apply` as fol­lows.

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



This, fi­nally, al­lows us to im­ple­ment `take_front`, and `apply` without the need for any fur­ther helper func­tions.

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

Both func­tions call `index_apply`, spec­i­fying how many el­e­ments we want to ex­tract. Then they pass a vari­adic lambda that ex­pects a pa­ra­meter pack of in­dices. These in­dices are passed as in­stances of `integral_constant`. There­fore, they can be used right away as a tem­plate ar­gu­ment to `get`.

### A Few More Ex­am­ples

Now that we have `index_apply` let’s write two more func­tions on tu­ples with its help. 

#### Reverse a tuple

First, let’s write a func­tion that takes a tuple and re­turns a new tuple that con­tains the orig­inal tu­ple’s el­e­ments in **re­versed or­der**.

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

That func­tion is nearly iden­tical to `tuple_front` just that this time we take the full length, and count the in­dices that we pass to `get` back­wards.

```C++
    auto t = reverse(make_tuple(1, 2, 3, 4));
    assert(t == make_tuple(4, 3, 2, 1));
```



#### zip

Now, let’s move on to a more com­plex ex­am­ple. We will write a func­tion that takes an ar­bi­trary number of tu­ples and re­turns a tuple of tu­ples, where the first con­tains all the first el­e­ments of the input tu­ples, the second all the second el­e­ments, and so on. We’ll call this func­tion `zip`. We also specify that when called with zero ar­gu­ments it just re­turns an empty tu­ple. If the tu­ples are of dif­fering length then we will crop all in­puts to match the shortest one. All in all we ex­pect `zip` to ful­fill the fol­lowing as­ser­tions.

```C++
assert(make_tuple() = zip());

auto t1 = zip(make_tuple(1, 2), make_tuple(3, 4), make_tuple(5, 6));
assert(t1 == make_tuple( make_tuple(1, 3, 5), make_tuple(2, 4, 6) ));

auto t2 = zip(make_tuple(1, 2, 3), make_tuple(4));
assert(t2 == make_tuple( make_tuple(1, 4) ));

```

It would also make sense to im­ple­ment the func­tion `transpose` in terms of `zip`, which takes a tuple of tu­ples, in­ter­prets it like a ma­trix, and re­turns a trans­posed tuple of tu­ples.

```C++
template<class Tuple>
constexpr auto transpose(Tuple t)
{
	return apply(t, [](auto ... ts)
	{	return zip(ts...);});
}

```

How could we im­ple­ment `zip`? There are a few things that we need. First, we need the length of the shortest tu­ple. For that pur­pose we can use `min` again.

```C++
constexpr size_t len = min({tuple_size<Tuples>{}...});
```

Where we used the fact that `min` is a `constexpr` func­tion since C++14.

Next, we need to find a way to go through every tuple and every tu­ple’s el­e­ments si­mul­ta­ne­ously. Un­for­tu­nately, the fol­low­ing, naive, im­ple­men­ta­tion will not com­pile.

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

We somehow need to nest two pa­ra­meter pack ex­pan­sions. How­ever, in the above code it is am­biguous which pack to ex­pand first. In­stead of taking a guess for us the com­piler will (thank­fully) simply refuse to ac­cept this code.

We can cir­cum­vent that problem by split­ting the task in two. We can think of the re­sult as a tuple of rows where each row con­tains the `I`-th el­e­ments of all input tu­ples. For each row the index is fixed. The outer tuple then con­tains all those rows. In code that reads as fol­lows.

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

The lambda `row` con­structs a single row of `I`-th el­e­ments of all tu­ples. To that end it takes one `integral_constant` as an ar­gu­ment and uses it to ex­tract one el­e­ment from each tu­ple. Within `index_apply` we then con­struct a tuple of all rows.

Fi­nally, to handle the empty case we simply pro­vide the fol­lowing over­load.

```C++
constexpr auto zip() { return make_tuple(); }
```

With that we have im­ple­mented `zip` to the spec­i­fi­ca­tion above.

### Con­clu­sion

In this ar­ticle we saw how to re­duce the boil­er­plate when ex­tracting el­e­ments out of generic tu­ples. The pre­sented pat­tern al­lows to write the full im­ple­men­ta­tion of a func­tion that deals with generic pat­terns in one place without having to re­sort to ex­ternal helper func­tions.

If you have any com­ments, thoughts, or crit­i­cism please leave a com­ment. In any case I hope you en­joyed the ar­ti­cle. Thanks for read­ing!