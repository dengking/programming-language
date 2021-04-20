# stackoverflow [Implementation C++14 make_integer_sequence](https://stackoverflow.com/questions/17424477/implementation-c14-make-integer-sequence)



## [A](https://stackoverflow.com/a/17426611)

Here's a `log N` implementation that doesn't even need an increased max-depth for template instantiations and compiles pretty fast:

```C++
// using aliases for cleaner syntax
template<class T> using Invoke = typename T::type;

/**
 * @brief 它的type就是它本身
 *
 * @tparam
 */
template<unsigned...>
struct seq
{
	using type = seq;
};

template<class S1, class S2> 
struct concat;

template<unsigned ... I1, unsigned ... I2>
struct concat<seq<I1...>, seq<I2...>> : seq<I1..., (sizeof...(I1)+I2)...>
{
};

template<class S1, class S2>
using Concat = Invoke<concat<S1, S2>>;

template<unsigned N> struct gen_seq;
template<unsigned N> using GenSeq = Invoke<gen_seq<N>>;

template<unsigned N>
struct gen_seq: Concat<GenSeq<N / 2>, GenSeq<N - N / 2>>
{
};

template<> struct gen_seq<0> : seq<>
{
};
template<> struct gen_seq<1> : seq<0>
{
};

```

> NOTE: 
>
> 1、递归关系:
>
> seq(N) = seq(N/2) + seq(N-N/2)
>
> 2、`(sizeof...(I1)+I2)...` 要如何理解？
>
> 取`I1`的长度`L1`，然后将将`I2`中每个元素都加上`L1`
>
> 3、推演如下:
>
> ```C++
> GenSeq<5>=Concat<GenSeq<2>, GenSeq<3>>;
> 
> GenSeq<2>=Concat<GenSeq<1>, GenSeq<1>>
> =Concat<Seq<0>, Seq<0>>
> =Seq<0, 1>;
> 
> GenSeq<3>=Concat<GenSeq<1>, GenSeq<2>>
> =Concat<Seq<0>, Seq<0, 1>>;
> =Seq<0, 1, 2>;
> 
> GenSeq<5>=Concat<Seq<0, 1>, Seq<0, 1, 2>>
> =Seq<0, 1, 2, 3, 4>;
> ```
>
> 

### Comments

@Xeo I would read `Concat` as "take two lists and put them one after the other". Adding "and add the the length of the leftmost list to the contents of the rightmost list" to what `Concat` does would surprise me. 

```C++
template<class S, unsigned I = 1> struct inc;
template<unsigned ... Is, unsigned I> struct inc<seq<Is...>, I> : seq<(Is+I)...>
{
};
template<class S, unsigned I = 1> using Inc=Invoke<inc<S,I>>;

```

then

```C++
template<unsigned N> struct gen_seq: Concat<GenSeq<N / 2>, Inc<GenSeq<N - N / 2>, N / 2>>
{
};

```

, where `Concat` doesn't add anything to second list, would decouple that operation from concatenation. 

## [A](https://stackoverflow.com/a/32223343)

This is basically me hacking around Xeo's solution: Making community wiki - if appreciative, ***please upvote Xeo***.

...just modified until I felt it couldn't get any simpler, renamed and added `value_type` and `size()` per the Standard (but only doing `index_sequence` not `integer_sequence`), and code working with GCC 5.2 `-std=c++14` could run otherwise unaltered under older/other compilers I'm stuck with. Might save someone some time / confusion.

```C++
// based on http://stackoverflow.com/a/17426611/410767 by Xeo
namespace std  // WARNING: at own risk, otherwise use own namespace
{
    template <size_t... Ints>
    struct index_sequence
    {
        using type = index_sequence;
        using value_type = size_t;
        static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
    };

    // --------------------------------------------------------------

    template <class Sequence1, class Sequence2>
    struct _merge_and_renumber;

    template <size_t... I1, size_t... I2>
    struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
      : index_sequence<I1..., (sizeof...(I1)+I2)...>
    { };

    // --------------------------------------------------------------

    template <size_t N>
    struct make_index_sequence
      : _merge_and_renumber<typename make_index_sequence<N/2>::type,
                            typename make_index_sequence<N - N/2>::type>
    { };

    template<> struct make_index_sequence<0> : index_sequence<> { };
    template<> struct make_index_sequence<1> : index_sequence<0> { };
}
```

Notes:

1、the "magic" of Xeo's solution is in the declaration of `_merge_and_renumber` (`concat` in his code) with exactly two parameters, while the specialization effectively exposes their individual parameter packs

2、the `typename`...`::type` in...

```C++
struct make_index_sequence
  : _merge_and_renumber<typename make_index_sequence<N/2>::type,
                        typename make_index_sequence<N - N/2>::type>
```

avoids the error:

```C++
invalid use of incomplete type 'struct std::_merge_and_renumber<std::make_index_sequence<1ul>, std::index_sequence<0ul> >'
```

