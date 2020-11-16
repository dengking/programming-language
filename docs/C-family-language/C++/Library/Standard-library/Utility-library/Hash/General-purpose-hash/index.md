# General purpose hash in C++

在前面我们已经知道了了C++ standard library并没有提供general purpose hash function，但是有的时候我们的确是需要的，本文给出一些样例实现。

## Fowler-Noll-Vo hash function

参见: wikipedia [Fowler-Noll-Vo hash function](https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function)

### stackoverflow [std::hash value on char* value and not on memory address?](https://stackoverflow.com/questions/34597260/stdhash-value-on-char-value-and-not-on-memory-address)

[A](https://stackoverflow.com/a/34597785)

There is of course the trivial (and slow) solution of creating a temporary `std::string` and hashing that one. If you don't want to do this, I'm afraid you will have to implement your own hash function. Sadly enough, the current C++ standard library doesn't provide general purpose hash algorithms disentangled from object-specific hash solutions. (But there is [some hope](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) this could change in the future.)

> NOTE: 总结的非常好

Suppose you had a function

```cpp
std::size_t
hash_bytes(const void * data, std::size_t size) noexcept;
```

that would take an address and a size and return you a hash computed from the that many bytes following that address. With the help of that function, you could easily write

```cpp
template <typename T>
struct myhash
{
  std::size_t
  operator()(const T& obj) const noexcept
  {
    // Fallback implementation.
    auto hashfn = std::hash<T> {};
    return hashfn(obj);
  }
};
```

and then specialize it for the types you're interested in.

```cpp
template <>
struct myhash<std::string>
{
  std::size_t
  operator()(const std::string& s) const noexcept
  {
    return hash_bytes(s.data(), s.size());
  }
};

template <>
struct myhash<const char *>
{
  std::size_t
  operator()(const char *const s) const noexcept
  {
    return hash_bytes(s, std::strlen(s));
  }
};
```

This leaves you only with the exercise of implementing `hash_bytes`. Fortunately, there are some fairly good hash functions that are rather easy to implement. My go-to algorithm for simple hashing is the [Fowler-Noll-Vo hash function](https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function). You can implement it in five lines of code; see the linked Wikipedia article.

If you want to get a bit fancy, consider the following implementation. First, I define a generic `template` that can be specialized for any version of the FNV-1a hash function.

```cpp
template <typename ResultT, ResultT OffsetBasis, ResultT Prime>
class basic_fnv1a final
{

  static_assert(std::is_unsigned<ResultT>::value, "need unsigned integer");

public:

  using result_type = ResultT;

private:

  result_type state_ {};

public:

  constexpr
  basic_fnv1a() noexcept : state_ {OffsetBasis}
  {
  }

  constexpr void
  update(const void *const data, const std::size_t size) noexcept
  {
    const auto cdata = static_cast<const unsigned char *>(data);
    auto acc = this->state_;
    for (auto i = std::size_t {}; i < size; ++i)
      {
        const auto next = std::size_t {cdata[i]};
        acc = (acc ^ next) * Prime;
      }
    this->state_ = acc;
  }

  constexpr result_type
  digest() const noexcept
  {
    return this->state_;
  }

};
```

Next, I provide aliases for the 32 and 64 bit versions. The parameters were taken from [Landon Curt Noll's website](http://www.isthe.com/chongo/tech/comp/fnv/index.html).

```cpp
using fnv1a_32 = basic_fnv1a<std::uint32_t,
                             UINT32_C(2166136261),
                             UINT32_C(16777619)>;

using fnv1a_64 = basic_fnv1a<std::uint64_t,
                             UINT64_C(14695981039346656037),
                             UINT64_C(1099511628211)>;
```

Finally, I provide type meta-functions to select a version of the algorithm given the wanted number of bits.

```cpp
template <std::size_t Bits>
struct fnv1a;

template <>
struct fnv1a<32>
{
  using type = fnv1a_32;
};

template <>
struct fnv1a<64>
{
  using type = fnv1a_64;
};

template <std::size_t Bits>
using fnv1a_t = typename fnv1a<Bits>::type;
```

And with that, we're good to go.

```cpp
constexpr std::size_t
hash_bytes(const void *const data, const std::size_t size) noexcept
{
  auto hashfn = fnv1a_t<CHAR_BIT * sizeof(std::size_t)> {};
  hashfn.update(data, size);
  return hashfn.digest();
}
```

Note how this code automatically adapts to platforms where `std::size_t` is 32 or 64 bits wide.

> NOTE: 完整代码如下:
>
> ```C++
> #include <type_traits>
> #include <cstdint>
> #include <climits>
> /**
>  * @brief fnv1a算法的实现
>  *
>  * @tparam ResultT
>  * @tparam OffsetBasis
>  * @tparam Prime
>  */
> template<typename ResultT, ResultT OffsetBasis, ResultT Prime>
> class basic_fnv1a final
> {
> 	/**
> 	 * 要求结果是unsigned type
> 	 */
> 	static_assert(std::is_unsigned<ResultT>::value, "need unsigned integer");
> public:
> 	using result_type = ResultT;
> private:
> 	result_type state_ { };
> public:
> 
> 	constexpr basic_fnv1a() noexcept :
> 					state_ { OffsetBasis }
> 	{
> 	}
> 
> 	constexpr void update(const void *const data, const std::size_t size) noexcept
> 	{
> 		const auto cdata = static_cast<const unsigned char*>(data);
> 		auto acc = this->state_;
> 		for (auto i = std::size_t { }; i < size; ++i)
> 		{
> 			const auto next = std::size_t { cdata[i] };
> 			acc = (acc ^ next) * Prime;
> 		}
> 		this->state_ = acc;
> 	}
> 
> 	constexpr result_type digest() const noexcept
> 	{
> 		return this->state_;
> 	}
> 
> };
> using fnv1a_32 = basic_fnv1a<std::uint32_t,
> UINT32_C(2166136261),
> UINT32_C(16777619)>;
> 
> using fnv1a_64 = basic_fnv1a<std::uint64_t,
> UINT64_C(14695981039346656037),
> UINT64_C(1099511628211)>;
> 
> constexpr std::size_t hash_bytes(const void *const data, const std::size_t size) noexcept
> {
> 	auto hashfn = fnv1a_t<CHAR_BIT * sizeof(std::size_t)> { };
> 	hashfn.update(data, size);
> 	return hashfn.digest();
> }
> 
> ```
>
> 





## BKDR hash algorithm

### stackoverflow [C++ unordered_map with char* as key](https://stackoverflow.com/questions/20649864/c-unordered-map-with-char-as-key)

> 在 bugsdb [C++ unordered_map with char* as key](https://bugsdb.com/_en/debug/272c82a107ca800554a208494707cc6e) 中也描述了相同的内容

```C++
#include <unordered_map>
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

template<class _Tp>
struct my_equal_to: public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp &__x, const _Tp &__y) const
	{
		return strcmp(__x, __y) == 0;
	}
};

typedef unordered_map<char*, unsigned int, ::std::tr1::hash<char*>, my_equal_to<char*> > my_unordered_map;
//typedef unordered_map<string, unsigned int > my_unordered_map;

my_unordered_map location_map;

int main()
{
	char a[10] = "ab";
	location_map.insert(my_unordered_map::value_type(a, 10));
	char b[10] = "abc";
	location_map.insert(my_unordered_map::value_type(b, 20));

	char c[10] = "abc";
	location_map.insert(my_unordered_map::value_type(c, 20));

	printf("map size: %d\n", location_map.size());
	my_unordered_map::iterator it;
	if ((it = location_map.find("abc")) != location_map.end())
	{
		printf("found!\n");
	}

	return 0;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序在Linux是无法编译通过的

I insert the same C string `abc` twice and look it up. The second insertion should fail and there will be only one `abc` in the unordered_map. However, the output size is 3. It seems that the compare function does not work properly here.



Edit: After defining a hash function for `char*` by my own, the program works properly. The full program code is listed below. Thank you all.

```C++
#include <unordered_map>
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

template<class _Tp>
struct my_equal_to: public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp &__x, const _Tp &__y) const
	{
		return strcmp(__x, __y) == 0;
	}
};

struct Hash_Func
{
	//BKDR hash algorithm
	int operator()(char *str) const
	{
		int seed = 131; //31  131 1313 13131131313 etc//
		int hash = 0;
		while (*str)
		{
			hash = (hash * seed) + (*str);
			str++;
		}

		return hash & (0x7FFFFFFF);
	}
};

typedef unordered_map<char*, unsigned int, Hash_Func, my_equal_to<char*> > my_unordered_map;

int main()
{
	my_unordered_map location_map;

	char a[10] = "ab";
	location_map.insert(my_unordered_map::value_type(a, 10));
	char b[10] = "abc";
	location_map.insert(my_unordered_map::value_type(b, 20));

	char c[10] = "abc";
	location_map.insert(my_unordered_map::value_type(c, 20));

	printf("map size: %d\n", location_map.size());
	my_unordered_map::iterator it;
	if ((it = location_map.find("abc")) != location_map.end())
	{
		printf("found!\n");
	}

	return 0;
}
// g++ --std=c++11 test.cpp

```



## [Java's String hash function](https://en.wikipedia.org/wiki/Java_hashCode()#The_java.lang.String_hash_function)

stackoverflow [std::hash value on char* value and not on memory address?](https://stackoverflow.com/questions/34597260/stdhash-value-on-char-value-and-not-on-memory-address) # [A](https://stackoverflow.com/a/34597485) : 

I've had to do this before and ended up writing a function to do this, with essentially the same implementation as [Java's String hash function](https://en.wikipedia.org/wiki/Java_hashCode()#The_java.lang.String_hash_function):

```cpp
size_t hash_c_string(const char* p, size_t s) {
    size_t result = 0;
    const size_t prime = 31;
    for (size_t i = 0; i < s; ++i) {
        result = p[i] + (result * prime);
    }
    return result;
}
```

Mind you, this is NOT a cryptographically secure hash, but it is fast enough and yields good results.



## gcc libstdc++ `std::_Hash_bytes`

stackoverflow [std::hash value on char* value and not on memory address?](https://stackoverflow.com/questions/34597260/stdhash-value-on-char-value-and-not-on-memory-address) # [A](https://stackoverflow.com/a/64616123) 中提及: 

For example, libstdc++ (which is what GCC uses by default) provides `std::_Hash_bytes` which can be called like this:

```cpp
#include <functional>
// -> which finally includes /usr/include/c++/$x/bits/hash_bytes.h
#include <cstring>

static size_t hash_cstr_gnu(const char *s)
{
    const size_t seed = 0;
    return std::_Hash_bytes(s, std::strlen(s), seed);
}
```