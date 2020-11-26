# `std::integer_sequence`



## cppreference [std::integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence)







## Before C++14



### stackoverflow [Implementation C++14 make_integer_sequence](https://stackoverflow.com/questions/17424477/implementation-c14-make-integer-sequence)



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



@Xeo I would read `Concat` as "take two lists and put them one after the other". Adding "and add the the length of the leftmost list to the contents of the rightmost list" to what `Concat` does would surprise me. `template<class S, unsigned I=1> struct inc; template<unsigned... Is, unsigned I> struct inc<seq<Is...>, I>:seq<(Is+I)...> {}; template<class S, unsigned I=1> using Inc=Invoke<inc<S,I>>;` then `template<unsigned N>struct gen_seq:Concat<GenSeq<N/2>, Inc<GenSeq<N-N/2>,N/2>>{};`, where `Concat` doesn't add anything to second list, would decouple that operation from concatenation. 