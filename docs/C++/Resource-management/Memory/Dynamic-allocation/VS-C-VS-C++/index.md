# 关于本章

本章讨论C++、C在Dynamic memory management之间的差异。

## draft

C++中的dynamic memory management主要是通过[new](https://en.cppreference.com/w/cpp/language/new)、[delete](https://en.cppreference.com/w/cpp/language/delete)，它们都是operator，是语言内置的，用户无法替换实现，C++标准库提供的[Dynamic memory management library](https://en.cppreference.com/w/cpp/memory)，可以实现更加高级的dynamic memory management。

C中的dynamic memory management是通过标准库所提供[dynamic memory management library](https://en.cppreference.com/w/c/memory)，如[malloc](https://en.cppreference.com/w/c/memory/malloc)、[free](https://en.cppreference.com/w/c/memory/free)，显然它们都是function。这样的安排，就给用户提供使用替换方案的机会，比如redis就没有使用C标准库所提供的malloc和free，而是使用的[jemalloc](http://jemalloc.net/)库，这在[redis](https://github.com/antirez/redis)中进行了介绍。

## new VS malloc

### [Out of memory](https://en.wikipedia.org/wiki/Out_of_memory) 的处理

the [C standard function](https://en.wikipedia.org/wiki/C_standard_function) for allocating memory, `malloc()`, will return [NULL](https://en.wikipedia.org/wiki/Null_pointer) and a well-behaved application should handle this situation. `malloc`通过返回值是否为NULL来判断是否分配成功

c++中的`new`呢？通过Google `c++ new return null`可以得到如下有文章：

1、[Why doesn't new in C++ return NULL on failure](https://stackoverflow.com/questions/26419786/why-doesnt-new-in-c-return-null-on-failure)

2、http://www.cplusplus.com/reference/new/bad_alloc/

3、[Will new return NULL in any case?](https://stackoverflow.com/questions/550451/will-new-return-null-in-any-case)



以下是demo

```c
void alloc_resource(int len) {
		try {
			/// https://stackoverflow.com/a/2204380
			///  initialise memory with new operator in C++
			array_p_ = new ElementT[len]();
		} catch (std::bad_alloc& ba) {
			std::string msg = "bad_alloc caught: " + ba.what();
			std::cerr << msg << '\n';
			LogError(msg);
			std::abort();
		}
		len_ = len;
	}
```

