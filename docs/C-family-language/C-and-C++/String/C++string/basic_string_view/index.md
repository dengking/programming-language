# `string_view`

`string_view`是c++17中增加的功能。

## modernescpp [C++17 - Avoid Copying with std::string_view](https://www.modernescpp.com/index.php/c-17-avoid-copying-with-std-string-view)

> NOTE: 这篇完整是非常好的，推荐首先阅读



## cppreference [std::basic_string_view](https://en.cppreference.com/w/cpp/string/basic_string_view)



## stackoverflow [How exactly is std::string_view faster than const std::string&?](https://stackoverflow.com/questions/40127965/how-exactly-is-stdstring-view-faster-than-const-stdstring)

### [A](https://stackoverflow.com/a/40129198)

> NOTE:这篇文章非常好，它不仅描述了`std::string_view`的优势，也描述了应当使用`std::string`的场景。

`std::string_view` is faster in a few cases.

First, `std::string const&` requires the data to be in a `std::string`, and not a raw C array, a `char const*` returned by a C API, a `std::vector<char>` produced by some deserialization engine, etc. The avoided format conversion avoids copying bytes, and (if the string is longer than the SBO¹ for the particular `std::string` implementation) avoids a memory allocation.

> NOTE: 上面这段话的意思是: `std::string const&`要求string位于`std::string`中，显然一旦使用了`std::string`，则就会导致一次allocation、copy。这还会导致后续一系列的allocation、copy，显然这就大大降低了性能。

```cpp
void foo( std::string_view bob ) {
  std::cout << bob << "\n";
}
int main(int argc, char const*const* argv) {
  foo( "This is a string long enough to avoid the std::string SBO" );
  if (argc > 1)
    foo( argv[1] );
}
```

No allocations are done in the `string_view` case, but there would be if `foo` took a `std::string const&` instead of a `string_view`.

The second really big reason is that it permits working with substrings without a copy. Suppose you are parsing a 2 gigabyte json string (!)². If you parse it into `std::string`, each such parse node where they store the name or value of a node *copies* the original data from the 2 gb string to a local node.

Instead, if you parse it to `std::string_view`s, the nodes *refer* to the original data. This can save millions of allocations and halve memory requirements during parsing.

The speedup you can get is simply ridiculous.

This is an extreme case, but other "get a substring and work with it" cases can also generate decent speedups with `string_view`.

An important part to the decision is what you lose by using `std::string_view`. It isn't much, but it is something.

You lose implicit null termination, and that is about it. So if the same string will be passed to 3 functions all of which require a null terminator, converting to `std::string` once may be wise. Thus if your code is known to need a null terminator, and you don't expect strings fed from C-style sourced buffers or the like, maybe take a `std::string const&`. Otherwise take a `std::string_view`.

If `std::string_view` had a flag that stated if it was null terminated (or something fancier) it would remove even that last reason to use a `std::string const&`.

There is a case where taking a `std::string` with no `const&` is optimal over a `std::string_view`. If you need to own a copy of the string indefinitely after the call, taking by-value is efficient. You'll either be in the SBO case (and no allocations, just a few character copies to duplicate it), or you'll be able to *move* the heap-allocated buffer into a local `std::string`. Having two overloads `std::string&&` and `std::string_view` might be faster, but only marginally, and it would cause modest code bloat (which could cost you all of the speed gains).

------

¹ Small Buffer Optimization

² Actual use case.



## Custom implementation

### [redis-plus-plus](https://github.com/sewenew/redis-plus-plus)/[src](https://github.com/sewenew/redis-plus-plus/tree/master/src)/[sw](https://github.com/sewenew/redis-plus-plus/tree/master/src/sw)/[redis++](https://github.com/sewenew/redis-plus-plus/tree/master/src/sw/redis%2B%2B)/[utils.h](https://github.com/sewenew/redis-plus-plus/blob/master/src/sw/redis%2B%2B/utils.h)

在阅读[redis-plus-plus](https://github.com/sewenew/redis-plus-plus)时，发现其中给出了`StringView`的自定义实现。



```C++
// Refer to https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005finclude.html
#if __cplusplus >= 201703L
#  if defined __has_include
#    if __has_include(<string_view>)
#      define REDIS_PLUS_PLUS_HAS_STRING_VIEW
#    endif
#    if __has_include(<optional>)
#      define REDIS_PLUS_PLUS_HAS_OPTIONAL
#    endif
#  endif
#endif

#include <cstring>
#include <string>
#include <type_traits>

#if defined REDIS_PLUS_PLUS_HAS_STRING_VIEW
#include <string_view>
#endif
namespace sw
{

namespace redis
{

#if defined REDIS_PLUS_PLUS_HAS_STRING_VIEW

using StringView = std::string_view;

#else

// By now, not all compilers support std::string_view,
// so we make our own implementation.
class StringView
{
public:
	constexpr StringView() noexcept = default;

	constexpr StringView(const char *data, std::size_t size) :
					_data(data), _size(size)
	{
	}

	StringView(const char *data) :
					_data(data), _size(std::strlen(data))
	{
	}

	StringView(const std::string &str) :
					_data(str.data()), _size(str.size())
	{
	}

	constexpr StringView(const StringView&) noexcept = default;

	StringView& operator=(const StringView&) noexcept = default;

	constexpr const char* data() const noexcept
	{
		return _data;
	}

	constexpr std::size_t size() const noexcept
	{
		return _size;
	}

private:
	const char *_data = nullptr;
	std::size_t _size = 0;
};

#endif

```

上述实现是非常简陋的，没有保护operator的实现。

### [fmt](https://github.com/fmtlib/fmt)/[include](https://github.com/fmtlib/fmt/tree/master/include)/[fmt](https://github.com/fmtlib/fmt/tree/master/include/fmt)/[core.h](https://github.com/fmtlib/fmt/blob/master/include/fmt/core.h)

```C++
/**
 An implementation of ``std::basic_string_view`` for pre-C++17. It provides a
 subset of the API. ``fmt::basic_string_view`` is used for format strings even
 if ``std::string_view`` is available to prevent issues when a library is
 compiled with a different ``-std`` option than the client code (which is not
 recommended).
 */
template<typename Char> class basic_string_view
{
private:
	const Char *data_;
	size_t size_;

public:
	using value_type = Char;
	using iterator = const Char*;

	constexpr basic_string_view() FMT_NOEXCEPT : data_(nullptr), size_(0)
	{}

	/** Constructs a string reference object from a C string and a size. */
	constexpr basic_string_view(const Char *s, size_t count) FMT_NOEXCEPT
	: data_(s),
	size_(count)
	{}

	/**
	 \rst
	 Constructs a string reference object from a C string computing
	 the size with ``std::char_traits<Char>::length``.
	 \endrst
	 */
#if __cplusplus >= 201703L  // C++17's char_traits::length() is constexpr.
  FMT_CONSTEXPR
#endif
	basic_string_view(const Char *s) :
					data_(s), size_(std::char_traits < Char > ::length(s))
	{
	}

	/** Constructs a string reference from a ``std::basic_string`` object. */
	template <typename Traits, typename Alloc>
	FMT_CONSTEXPR basic_string_view(const std::basic_string<Char, Traits, Alloc> &s) FMT_NOEXCEPT
	: data_(s.data()),
	size_(s.size())
	{}

	template <typename S, FMT_ENABLE_IF(std::is_same<
					S, detail::std_string_view<Char>>::value)>
	FMT_CONSTEXPR basic_string_view(S s) FMT_NOEXCEPT : data_(s.data()),
	size_(s.size())
	{}

	/** Returns a pointer to the string data. */
	constexpr const Char* data() const
	{
		return data_;
	}

	/** Returns the string size. */
	constexpr size_t size() const
	{
		return size_;
	}

	constexpr iterator begin() const
	{
		return data_;
	}
	constexpr iterator end() const
	{
		return data_ + size_;
	}

	constexpr const Char& operator[](size_t pos) const
	{
		return data_[pos];
	}

	FMT_CONSTEXPR
	void remove_prefix(size_t n)
	{
		data_ += n;
		size_ -= n;
	}

	// Lexicographically compare this string reference to other.
	int compare(basic_string_view other) const
	{
		size_t str_size = size_ < other.size_ ? size_ : other.size_;
		int result = std::char_traits < Char > ::compare(data_, other.data_, str_size);
		if (result == 0)
			result = size_ == other.size_ ? 0 : (size_ < other.size_ ? -1 : 1);
		return result;
	}

	friend bool operator==(basic_string_view lhs, basic_string_view rhs)
	{
		return lhs.compare(rhs) == 0;
	}
	friend bool operator!=(basic_string_view lhs, basic_string_view rhs)
	{
		return lhs.compare(rhs) != 0;
	}
	friend bool operator<(basic_string_view lhs, basic_string_view rhs)
	{
		return lhs.compare(rhs) < 0;
	}
	friend bool operator<=(basic_string_view lhs, basic_string_view rhs)
	{
		return lhs.compare(rhs) <= 0;
	}
	friend bool operator>(basic_string_view lhs, basic_string_view rhs)
	{
		return lhs.compare(rhs) > 0;
	}
	friend bool operator>=(basic_string_view lhs, basic_string_view rhs)
	{
		return lhs.compare(rhs) >= 0;
	}
};

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;

```

上述实现是非常完整的。



## TODO

stackoverflow [What is string_view?](https://stackoverflow.com/questions/20803826/what-is-string-view)

