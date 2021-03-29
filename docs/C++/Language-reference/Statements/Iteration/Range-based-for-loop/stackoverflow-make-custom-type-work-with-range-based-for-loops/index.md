# stackoverflow [How to make my custom type to work with “range-based for loops”?](https://stackoverflow.com/questions/8164567/how-to-make-my-custom-type-to-work-with-range-based-for-loops)



## [A](https://stackoverflow.com/a/31457319)

The standard has been changed since the question (and most answers) were posted [in the resolution of this defect report](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1442).

The way to make a `for(:)` loop work on your type `X` is now one of two ways:

- Create member `X::begin()` and `X::end()` that return something that acts like an iterator
- Create a free function `begin(X&)` and `end(X&)` that return something that acts like an iterator, in the same namespace as your type `X`.¹

And similar for `const` variations. This will work both on compilers that implement the defect report changes, and compilers that do not.

### C++11 range-for

The objects returned do not have to actually be iterators. The `for(:)` loop, unlike most parts of the C++ standard, is [specified to expand to something equivalent to](http://en.cppreference.com/w/cpp/language/range-for):

```cpp
for( range_declaration : range_expression )
```

becomes:

```cpp
{
  auto && __range = range_expression ;
  for (auto __begin = begin_expr,
            __end = end_expr;
            __begin != __end; ++__begin) {
    range_declaration = *__begin;
    loop_statement
  }
}
```

where the variables beginning with `__` are for exposition only, and `begin_expr` and `end_expr` is the magic that calls `begin`/`end`.²

#### Requirements on the `begin`/`end` return value 

The requirements on the `begin`/`end` return value are simple: You must overload pre-`++`, ensure the initialization expressions are valid, binary `!=` that can be used in a boolean context, unary `*` that returns something you can assign-initialize `range_declaration` with, and expose a public destructor.

Doing so in a way that isn't compatible with an iterator is probably a bad idea, as future iterations of C++ might be relatively cavalier about breaking your code if you do.

### C++17 range-for

As an aside, it is reasonably likely that a future revision of the standard will permit `end_expr` to return a different type than `begin_expr`. This is useful in that it permits "lazy-end" evaluation (like detecting null-termination) that is easy to optimize to be as efficient as a hand-written C loop, and other similar advantages.

------

¹ Note that `for(:)` loops store any temporary in an `auto&&` variable, and pass it to you as an lvalue. You cannot detect if you are iterating over a temporary (or other rvalue); such an overload will not be called by a `for(:)` loop. See [stmt.ranged] 1.2-1.3 from n4527.

² Either call the `begin`/`end` method, or ADL-only lookup of free function `begin`/`end`, *or* magic for C-style array support. Note that `std::begin` is not called unless `range_expression` returns an object of type in `namespace std` or dependent on same.

------

In [c++17](https://stackoverflow.com/questions/tagged/c%2b%2b17) the range-for expression has been updated

```cpp
{
  auto && __range = range_expression ;
  auto __begin = begin_expr;
  auto __end = end_expr;
  for (;__begin != __end; ++__begin) {
    range_declaration = *__begin;
    loop_statement
  }
}
```

with the types of `__begin` and `__end` have been decoupled.

This permits the end iterator to not be the same type as begin. Your end iterator type can be a "sentinel" which only supports `!=` with the begin iterator type.

### C++17 range-for Example: C string

A practical example of why this is useful is that your `end` iterator can read "check your `char*` to see if it points to `'0'`" when `==` with a `char*`. This allows a C++ range-for expression to generate optimal code when iterating over a null-terminated `char*` buffer.

```cpp
struct null_sentinal_t {
  template<class Rhs,
    std::enable_if_t<!std::is_same<Rhs, null_sentinal_t>{},int> =0
  >
  friend bool operator==(Rhs const& ptr, null_sentinal_t) {
    return !*ptr;
  }
  template<class Rhs,
    std::enable_if_t<!std::is_same<Rhs, null_sentinal_t>{},int> =0
  >
  friend bool operator!=(Rhs const& ptr, null_sentinal_t) {
    return !(ptr==null_sentinal_t{});
  }
  template<class Lhs,
    std::enable_if_t<!std::is_same<Lhs, null_sentinal_t>{},int> =0
  >
  friend bool operator==(null_sentinal_t, Lhs const& ptr) {
    return !*ptr;
  }
  template<class Lhs,
    std::enable_if_t<!std::is_same<Lhs, null_sentinal_t>{},int> =0
  >
  friend bool operator!=(null_sentinal_t, Lhs const& ptr) {
    return !(null_sentinal_t{}==ptr);
  }
  friend bool operator==(null_sentinal_t, null_sentinal_t) {
    return true;
  }
  friend bool operator!=(null_sentinal_t, null_sentinal_t) {
    return false;
  }
};
```

[live example](http://coliru.stacked-crooked.com/a/bc8daa9b041c4283) of this.

Minimal test code is:

```cpp
struct cstring {
  const char* ptr = 0;
  const char* begin() const { return ptr?ptr:""; }// return empty string if we are null
  null_sentinal_t end() const { return {}; }
};

cstring str{"abc"};
for (char c : str) {
    std::cout << c;
}
std::cout << "\n";
```

#### 完整测试程序

```C++
#include <utility>
#include <iterator>
#include <iostream>

struct null_sentinal_t
{
	template<class Rhs, std::enable_if_t<!std::is_same<Rhs, null_sentinal_t> { }, int> = 0>
	friend bool operator==(Rhs const &ptr, null_sentinal_t)
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return !*ptr;
	}
	template<class Rhs, std::enable_if_t<!std::is_same<Rhs, null_sentinal_t> { }, int> = 0>
	friend bool operator!=(Rhs const &ptr, null_sentinal_t)
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return !(ptr == null_sentinal_t { });
	}
	template<class Lhs, std::enable_if_t<!std::is_same<Lhs, null_sentinal_t> { }, int> = 0>
	friend bool operator==(null_sentinal_t, Lhs const &ptr)
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return !*ptr;
	}
	template<class Lhs, std::enable_if_t<!std::is_same<Lhs, null_sentinal_t> { }, int> = 0>
	friend bool operator!=(null_sentinal_t, Lhs const &ptr)
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return !(null_sentinal_t { } == ptr);
	}
	friend bool operator==(null_sentinal_t, null_sentinal_t)
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return true;
	}
	friend bool operator!=(null_sentinal_t, null_sentinal_t)
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return false;
	}
};

struct cstring
{
	const char *ptr = 0;
	const char* begin() const
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return ptr;
	}
	null_sentinal_t end() const
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return
		{};
	}
};

int main()
{
	cstring str { "abc" };
	for (char c : str)
	{
		std::cout << c;
	}
	std::cout << "\n";
}
// g++ --std=c++17 test.cpp

```

输出如下:

```C++
[48]begin
[53]end
[16]operator!=
[10]operator==
a[16]operator!=
[10]operator==
b[16]operator!=
[10]operator==
c[16]operator!=
[10]operator==
```

分析:

1、在range-for中，会调用如下函数:

```C++
	template<class Rhs, std::enable_if_t<!std::is_same<Rhs, null_sentinal_t> { }, int> = 0>
	friend bool operator!=(Rhs const &ptr, null_sentinal_t)
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return !(ptr == null_sentinal_t { });
	}
```

2、这个函数，会调用如下函数:

```C++
	template<class Rhs, std::enable_if_t<!std::is_same<Rhs, null_sentinal_t> { }, int> = 0>
	friend bool operator==(Rhs const &ptr, null_sentinal_t)
	{
		std::cout << "[" << __LINE__ << "]" << __FUNCTION__ << std::endl;
		return !*ptr;
	}
```



------



### C++17 range-for Example

Here is a simple example.

```cpp
namespace library_ns {
  struct some_struct_you_do_not_control {
    std::vector<int> data;
  };
}
```

Your code:

```cpp
namespace library_ns {
  int* begin(some_struct_you_do_not_control& x){ return x.data.data(); }
  int* end(some_struct_you_do_not_control& x){ return x.data.data()+x.data.size(); }
  int const* cbegin(some_struct_you_do_not_control const& x){ return x.data.data(); }
  int* cend(some_struct_you_do_not_control const& x){ return x.data.data()+x.data.size(); }
  int const* begin(some_struct_you_do_not_control const& x){ return cbegin(x); }
  int const* end(some_struct_you_do_not_control const& x){ return cend(x); }
}
```

this is an example how you can augment a type you don't control to be iterable.



### C++17 range-for Example

Here I return pointers-as-iterators, hiding the fact I have a vector under the hood.

For a type you do own, you can add methods:

```cpp
struct egg {};
struct egg_carton {
  auto begin() { return eggs.begin(); }
  auto end() { return eggs.end(); }
  auto cbegin() const { return eggs.begin(); }
  auto cend() const { return eggs.end(); }
  auto begin() const { return eggs.begin(); }
  auto end() const { return eggs.end(); }
private:
  std::vector<egg> eggs;
};
```

here I reuse the `vector`'s iterators. I use `auto` for brevity; in [c++11](https://stackoverflow.com/questions/tagged/c%2b%2b11) I'd have to be more verbose.

#### 完整测试程序

```C++
#include <utility>
#include <iterator>
#include <iostream>
#include <vector>

struct egg
{
	int id { 0 };
	egg(int i) :
					id { i }
	{
	}
};
struct egg_carton
{
	auto begin()
	{
		return eggs.begin();
	}
	auto end()
	{
		return eggs.end();
	}
	auto cbegin() const
	{
		return eggs.begin();
	}
	auto cend() const
	{
		return eggs.end();
	}
	auto begin() const
	{
		return eggs.begin();
	}
	auto end() const
	{
		return eggs.end();
	}
	void add(int id)
	{
		eggs.emplace_back(id);
	}
private:
	std::vector<egg> eggs;
};
int main()
{
	egg_carton ec;
	for (int i = 0; i < 10; ++i)
	{
		ec.add(i);
	}
	for (auto &&e : ec)
	{
		std::cout << e.id << std::endl;
	}
	std::cout << "\n";
}
// g++ --std=c++14 test.cpp

```

输出如下:

```
0
1
2
3
4
5
6
7
8
9
```



### C++17 range-for Example

Here is a quick and dirty iterable range-view:

```cpp
template<class It>
struct range_t {
  It b, e;
  It begin() const { return b; }
  It end() const { return e; }
  std::size_t size() const { return end()-begin(); }
  bool empty() const { return begin()==end(); }
 
  range_t without_back( std::size_t n = 1 ) const {
    n = (std::min)(n, size());
    return {begin(), end()-n};
  }
  range_t without_front( std::size_t n = 1 ) const {
    n = (std::min)(n, size());
    return {begin()+n, end()};
  }
  decltype(auto) front() const { return *begin(); }
  decltype(auto) back() const { return *(std::prev(end())); }
};
template<class C>
auto make_range( C&& c ) {
  using std::begin; using std::end;
  return range_t{ begin(c), end(c) };
}
```

using [c++17](https://stackoverflow.com/questions/tagged/c%2b%2b17) template class deduction.

```cpp
std::vector<int> v{1,2,3,4,5};
for (auto x : make_range(v).without_front(2) ) {
  std::cout << x << "\n";
}
```

prints 3 4 5, skipping first 2.

#### 完整测试程序

```C++
#include <utility>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>

template<class It>
struct range_t
{
	It b, e;
	It begin() const
	{
		return b;
	}
	It end() const
	{
		return e;
	}
	std::size_t size() const
	{
		return end() - begin();
	}
	bool empty() const
	{
		return begin() == end();
	}

	range_t without_back(std::size_t n = 1) const
	{
		n = (std::min)(n, size());
		return
		{	begin(), end()-n};
	}
	range_t without_front(std::size_t n = 1) const
	{
		n = (std::min)(n, size());
		return
		{	begin()+n, end()};
	}
	decltype(auto) front() const
	{
		return *begin();
	}
	decltype(auto) back() const
	{
		return *(std::prev(end()));
	}
};
template<class C>
auto make_range(C &&c)
{
	using std::begin;
	using std::end;
	return range_t<decltype(begin(c))> { begin(c), end(c) };
}
int main()
{
	std::vector<int> v { 1, 2, 3, 4, 5 };
	for (auto x : make_range(v).without_front(2))
	{
		std::cout << x << "\n";
	}
}
// g++ --std=c++14 test.cpp

```

输出如下:

```C++
3
4
5
```

## [A](https://stackoverflow.com/a/28926968)

I write my answer because some people might be more happy with simple real life example without STL includes.

I have my own plain only data array implementation for some reason, and I wanted to use the range based for loop. Here is my solution:

```cpp
template<typename DataType>
class PodArray
{
public:
	class iterator
	{
	public:
		iterator(DataType *ptr) :
						ptr(ptr)
		{
		}
		iterator operator++()
		{
			++ptr;
			return *this;
		}
		bool operator!=(const iterator &other) const
		{
			return ptr != other.ptr;
		}
		const DataType& operator*() const
		{
			return *ptr;
		}
	private:
		DataType *ptr;
	};
private:
	unsigned len;
	DataType *val;
public:
	iterator begin() const
	{
		return iterator(val);
	}
	iterator end() const
	{
		return iterator(val + len);
	}

	// rest of the container definition not related to the question ...
};

```

Then the usage example:

```cpp
PodArray<char> array;
// fill up array in some way
for(auto& c : array)
  printf("char: %c\n", c);
```