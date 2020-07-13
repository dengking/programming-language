# Tag Dispatching



## [More C++ Idioms/Tag Dispatching](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Tag_Dispatching)

### Intent

Simplify writing multiple SFINAE-constrained overloads

> NOTE: 这段话说明了tag dispatching和SFINAE-constrained overloads之间的关联，显然两者能够实现相同的目的

### Motivation

It can also be used in conjunction with trailing return type and `decltype`. ([expression SFINAE](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE))

```c++
namespace detail { // tags for dispatching
	struct pick_3 {}
	struct pick_2 : pick_3 {}
	struct pick_1 : pick_2 {}
	static pick_1 selector;

	// first choice - member preferred if exists
	template<typename Cont, typename Op>
	auto remove_if(pick_1, Cont& cont, Op&& op)
		-> decltype(cont.remove_if(op), void())
	{
		cont.remove_if(std::forward<Op>(op));
	}
	
	// second choice - erase remove idiom
	template<typename Cont, typename Op>
	auto remove_if(pick_2, Cont& cont, Op&& op)
		-> decltype(cont.erase(std::remove_if(std::begin(cont), std::end(cont), std::forward<Op>(op)), std::end(cont)), void())
	{
		cont.erase(std::remove_if(std::begin(cont), std::end(cont), std::forward<Op>(op)), std::end(cont));
	}
	
	// last choice - manual looping
	template<typename Cont, typename Op>
	auto remove_if(pick_3, Cont& cont, Op&& op)
		-> void
	{
		auto it = std::begin(cont);
		while (it != std::end(cont))
		{
			if (op(*it))
				it = cont.erase(it);
			else
				++it;
		}
	}
}

template<typename Cont, typename Op>
auto remove_if(Cont& cont, Op&& op)
{
	detail::remove_if(detail::selector, cont, std::forward<Op>(op));
}
```

