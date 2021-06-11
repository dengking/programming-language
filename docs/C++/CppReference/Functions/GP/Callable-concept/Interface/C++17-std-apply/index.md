# std::apply



## cppreference [std::apply](https://en.cppreference.com/w/cpp/utility/apply)



### Possible implementation

```c++
namespace detail
{
template<class F, class Tuple, std::size_t ... I>
constexpr decltype(auto) apply_impl(F &&f, Tuple &&t, std::index_sequence<I...>)
{
	// This implementation is valid since C++20 (via P1065R2)
	// In C++17, a constexpr counterpart of std::invoke is actually needed here
	return std::invoke(std::forward < F > (f), std::get<I>(std::forward<Tuple>(t))...);
}
}  // namespace detail

template<class F, class Tuple>
constexpr decltype(auto) apply(F &&f, Tuple &&t)
{
	return detail::apply_impl(std::forward < F > (f), std::forward < Tuple > (t), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>> { });
}

```

