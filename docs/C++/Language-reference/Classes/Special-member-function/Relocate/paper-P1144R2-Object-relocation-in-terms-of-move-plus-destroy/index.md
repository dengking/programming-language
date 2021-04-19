# open-std [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html)

> NOTE:
>
> 一、这篇论文主要讨论了如下内容:
>
> 1、relocate
>
> 其实，它就是一个复合操作，这是非常容易理解的；从目前来看，我觉得它的重要意义是 "github [remove move constructor for not_null. #842](https://github.com/microsoft/GSL/pull/842)  #**[hsutter](https://github.com/hsutter)** commented [on 5 Feb 2020](https://github.com/microsoft/GSL/pull/842#issuecomment-582074941) "中提及的: 不break class invariant
>
> 2、 **trivially relocatable**
>
> 这是作者着重讨论的内容，其中作者着重讨论了"1.1. Optimizations enabled by trivial relocatability"

## Abstract

We define a new verb, "relocate," which is equivalent to a move and a destroy (analogous to the existing verb "swap," which is equivalent to a move, two move-assignments, and a destroy). For many C++ types, the "relocate" operation is implementable as a single `memcpy`. We provide a standard trait to detect types which are **trivially relocatable**, for the benefit of library writers. Finally, we provide a portable way for a user-defined type (e.g. `boost::shared_ptr`) to warrant(保证) to the implementation that it is trivially relocatable.

> NOTE: 
>
> 1、在  github [remove move constructor for not_null. #842](https://github.com/microsoft/GSL/pull/842)  #**[hsutter](https://github.com/hsutter)** commented [on 5 Feb 2020](https://github.com/microsoft/GSL/pull/842#issuecomment-582074941)  中，提出了"The correct long-term answer for this would be if C++ gets something along the lines of the **relocation / destructive move semantics proposals**, where roughly "relocation/destructive-move leaves an object that is guaranteed to be no longer used" "
>
> 问题: 如何保证" object that is guaranteed to be no longer used"？

## 1. Introduction and motivation

### Relocate

> NOTE: 
>
> 1、`memcpy`、`realloc`

C++17 knows the verbs "move," "copy," "destroy," and "swap," where "swap" is a higher-level operation composed of several lower-level operations. To this list we propose to add the verb "relocate," which is a higher-level operation composed of exactly two lower-level operations. Given an object type `T` and memory addresses `src` and `dst`, the phrase "***relocate*** a `T` from `src` to `dst`" means no more and no less than "*move-construct* `dst` from `src`, and then immediately *destroy* the object at `src`."

### Relocatable and [trivially relocatable](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#abstract-opdef-trivially-relocatable)

Just as the verb "swap" produces the adjective "swappable," the verb "relocate" produces the adjective "relocatable." Any type which is both move-constructible and destructible is ***relocatable***. The notion can be modified by adverbs: we say that a type is ***nothrow relocatable*** if its relocation operation is `noexcept`, and we say that a type is ***[trivially relocatable](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#abstract-opdef-trivially-relocatable)*** if its relocation operation is trivial (which, just like trivial move-construction and trivial copy-construction, means "the operation is tantamount(等同于) to a `memcpy`").



Almost all relocatable types are trivially relocatable: `std::unique_ptr<int>`, `std::vector<int>`, `std::string`. Non-trivially relocatable types exist but are rare; see [Appendix C: Examples of non-trivially relocatable class types](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#non-trivial-samples).

### 1.1. Optimizations enabled by trivial relocatability

#### 1.1.1. Vector resize

If we have a reliable way of detecting "trivial relocatability," we can optimize any routine that performs the moral equivalent of `realloc`, including

```c++
    std::vector<R>::resize
    std::vector<R>::reserve
    std::vector<R>::emplace_back
    std::vector<R>::push_back
```

[[Bench\]](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-bench) (presented at C++Now 2018) shows a 3x speedup on `std::vector<std::unique_ptr<int>>::resize`. [This Reddit thread](https://www.reddit.com/r/cpp/comments/9wj4vt/trivially_relocatable_in_san_diego_call_for/e9p76i4/) demonstrates a similar 3x speedup using the online tool Quick-Bench.

As observed in [[CppChat\]](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-cppchat) (@21:55): Just as with C++11 move semantics, you can write benchmarks to show whatever speedup you like. The more complicated your types' move-constructors and destructors, the more time you save by eliminating calls to them.

> NOTE: 
>
> 1、性能的提升源自于消除对"move-constructors and destructors"的调用，因此最终的实现，就相当于memory copy，实现上可以直接使用类似于`realloc`的操作，关于`realloc`，参见 cppreference [realloc](https://en.cppreference.com/w/c/memory/realloc)。
>
> 其实，这体现了C VS C++，当一个type是trivial的时候，可以直接使用C的memory API对其进行操作

#### 1.1.2. Swap

> NOTE: 
>
> 1、这段话没有理解

Given a reliable way of detecting **trivial relocatability**, we can optimize any routine that uses the moral equivalent of `std::swap`, such as

```c++
    std::swap
    std::sort
    std::vector<R>::insert (arguably)
```

#### 1.1.3. More efficient small-buffer type-erasure

Given a reliable way of detecting trivial relocatability, we can de-duplicate the code generated by small-buffer-optimized (SBO) type-erasing wrappers such as `std::function` and `std::any`. For these types, a *move* of the wrapper object is implemented in terms of a *relocation* of the contained object. (See for example [libc++'s std::any](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-libcxxany), where the function that performs the relocation operation is confusingly named `__move`.) In general, the *relocate* operation for a contained type `C` must be uniquely codegenned for each different `C`, leading to code bloat. But a single instantiation suffices(足够) to relocate every *trivially* relocatable `C` in the program. A smaller number of instantiations means faster compile times, a smaller text section, and "hotter" code (because a relatively higher proportion of your code now fits in icache).

> NOTE: 
>
> 1、"codegenned"的含义是code generate
>
> 2、没有理解这段话的含义

### 1.2. The most important benefit

> NOTE: 
>
> 1、理解这段话的关键是: 读懂 [[FollyIssue889\]](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-follyissue889) ，其中在其中已经对本节的内容进行了深入讨论了。

Many real-world codebases already contain templates which require trivial relocatability of their template parameters, but currently have no way to *verify* trivial relocatability. For example, [[Folly\]](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-folly) requires the programmer to warrant the trivial relocatability of any type stored in a `folly::fbvector`:

```c++
    class Widget {
        std::vector<int> lst_;
    };

    folly::fbvector<Widget> vec;  // FAILS AT COMPILE TIME for lack of warrant
```

But this merely encourages the programmer to add the warrant and continue. An incorrect warrant will be discovered only at runtime, via undefined behavior. (See [Allocated memory contains pointer to self](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#non-trivial-sample-list) and [[FollyIssue889\]](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-follyissue889).)

```c++
    class Gadget {
        std::list<int> lst_;
    };
    // sigh, add the warrant on autopilot
    template<> struct folly::IsRelocatable<Gadget> : std::true_type {};

    folly::fbvector<Gadget> vec;  // CRASHES AT RUNTIME due to fraudulent warrant
```

If this proposal is adopted, then Folly can start using `static_assert(std::is_trivially_relocatable_v<T>)` in the implementation of `fbvector`, and the programmer can stop writing explicit warrants. Finally, the programmer can start writing assertions of correctness, which aids maintainability and can even find real bugs. [Example:](https://p1144.godbolt.org/z/dax9BR)

```c++
    class Widget {
        std::vector<int> lst_;
    };
    static_assert(std::is_trivially_relocatable_v<Widget>);  // correctly SUCCEEDS

    class Gadget {
        std::list<int> lst_;
    };
    static_assert(std::is_trivially_relocatable_v<Gadget>);  // correctly ERRORS OUT
```

The improvement in user experience for real-world codebases (such as [[Folly\]](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-folly), [[EASTL\]](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-eastl), BDE, Qt, etc.) is the most important benefit to be gained by this proposal.

> NOTE: 
>
> 一、上面这段关于folly中`folly::IsRelocatable`的讨论，让我想到了:
>
> CppCoreGuidelines [P.4: Ideally, a program should be statically type safe](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p4-ideally-a-program-should-be-statically-type-safe)
>
> CppCoreGuidelines [P.5: Prefer compile-time checking to run-time checking](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p5-prefer-compile-time-checking-to-run-time-checking)



## Appendix C: Examples of non-trivially relocatable class types

> NOTE: 
>
> 一、有data member的值，是依赖于object的，那么这就导致了新的object的value，不能够直接copy自源object，而是要重新计算
>
> 1、Class contains pointer to self
>
> 2、Allocated memory contains pointer to self
>
> 3、Class invariant depends on `this`
>
> 上述三种，都是属于这种情况的

### Class contains pointer to self

This fictional `short_string` illustrates a mechanism that can apply to any small-buffer-optimized class. [libc++'s std::function](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-libcxxfunction) uses this mechanism (on a 24-byte buffer) and is thus not trivially relocatable.

However, different mechanisms for small-buffer optimization exist. [libc++'s std::any](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-libcxxany) also achieves small-buffer optimization on a 24-byte buffer, without (necessarily) sacrificing trivial relocatability.



```C++
#include <string.h>
#include <stdlib.h>
#include <utility>

struct short_string
{
	char *data_ = buffer_; // pointer to self
	size_t size_ = 0;
	char buffer_[8] = { };

	const char* data() const
	{
		return data_;
	}

	short_string() = default;
	short_string(const char *s) :
					size_(strlen(s))
	{
		if (size_ < sizeof buffer_)
			strcpy(buffer_, s);
		else
			data_ = strdup(s);
	}
	short_string(short_string &&s)
	{
		memcpy(this, &s, sizeof(*this));
		// 这里进行修正，这是非常重要的
		if (s.data_ == s.buffer_)
			data_ = buffer_;
		else
			s.data_ = nullptr;
	}
	~short_string()
	{
		if (data_ != buffer_)
			free(data_);
	}
};

int main()
{
	short_string s("hello world");
	short_string s2(std::move(s));
	return 0;
}
// g++ --std=c++11 test.cpp

```



> NOTE: 
>
> 1、当构造一个新的object的时候，这个object无法直接使用`memcpy` source object的内容而构造，这是因为新的object中的pointer的value还是source object的，那么一旦source object end，则这就导致dangling
>
> 2、stackoverflow [c++ type trait to say “trivially movable” - examples of](https://stackoverflow.com/questions/45747932/c-type-trait-to-say-trivially-movable-examples-of) # [A](https://stackoverflow.com/a/45748834) 中，也展示了类似的例子:
>
> ```C++
> #include <memory>
> #include <type_traits>
> 
> struct A
> {
>     int a;
>     int b;
>     int* p{&a};
> };
> 
> int main()
> {
>    auto p = std::make_unique<A>();
>    A a = std::move(*p.get());  // gets moved here, a.p is dangling.
> 
>    return  std::is_move_assignable<A>::value;  // <-- yet, this returns true.
> }
> ```
>
> 



### Allocated memory contains pointer to self

`std::list` needs somewhere to store its "past-the-end" node, commonly referred to as the "sentinel node," whose `prev` pointer points to the list’s last node. If the sentinel node is allocated on the heap, then `std::list` can be trivially relocatable; but if the sentinel node is placed within the `list` object itself (as happens on libc++ and libstdc++), then relocating the `list` object requires fixing up the list’s last node’s `next` pointer so that it points to the new sentinel node inside the destination `list` object. This fixup of an arbitrary heap object cannot be simulated by `memcpy`.



Traditional implementations of `std::set` and `std::map` also store a "past-the-end" node inside themselves and thus also fall into this category.

```C++
struct node
{
	node *prev_ = nullptr;
	node *next_ = nullptr;
};
struct list
{
	node n_; // sentinel node
	iterator begin()
	{
		return iterator(n_.next_);
	}
	iterator end()
	{
		return iterator(&n_);
	}
	list(list &&l)
	{
		if (l.n_.next_)
			l.n_.next_->prev_ = &n_;  // fixup
		if (l.n_.prev_)
			l.n_.prev_->next_ = &n_;  // fixup
		n_ = l.n_;
		l.n_ = node { };
	}
	// ...
};
// g++ test.cpp --std=c++11 -pedantic -Wall

```



### Class invariant depends on `this`

The `offset_ptr` provided by [[Boost.Interprocess\]](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html#biblio-boostinterprocess) is an example of this category.

```C++
#include <stdint.h>
#include <iostream>

struct offset_ptr
{
	uintptr_t value_;

	uintptr_t here() const
	{
		return uintptr_t(this);
	}
	uintptr_t distance_to(void *p) const
	{
		return uintptr_t(p) - here();
	}
	void* get() const
	{
		return (void*) (here() + value_);
	}

	offset_ptr() :
					value_(distance_to(nullptr))
	{
	}
	offset_ptr(void *p) :
					value_(distance_to(p))
	{
	}
	offset_ptr(const offset_ptr &rhs) :
					value_(distance_to(rhs.get()))
	{
	}
	offset_ptr& operator=(const offset_ptr &rhs)
	{
		value_ = distance_to(rhs.get());
		return *this;
	}
	~offset_ptr() = default;
};

int main()
{
	offset_ptr p;
	std::cout << p.value_ << std::endl;
	return 0;
}
// g++ test.cpp --std=c++11 -pedantic -Wall

```





> NOTE: 
>
> 1、上述程序中的class invariant是什么？`value_`
>
> 2、需要在constructor中，establish class invariant



### Program invariant depends on `this`

In the following snippet, `struct Widget` is relocatable, but not trivially relocatable, because the relocation operation of destroying a `Widget` at point A and constructing a new `Widget` at point B has behavior that is observably different from a simple `memcpy`.



```C++
#include <stdint.h>
#include <iostream>
#include <set>

std::set<void*> registry;

struct registered_object
{
	registered_object()
	{
		registry.insert(this);
	}
	registered_object(registered_object&&) = default;
	registered_object(const registered_object&) = default;
	registered_object& operator=(registered_object&&) = default;
	registered_object& operator=(const registered_object&) = default;
	~registered_object()
	{
		registry.erase(this);
	}
};

struct Widget: registered_object
{
};

int main()
{
}
// g++ test.cpp --std=c++11 -pedantic -Wall

```