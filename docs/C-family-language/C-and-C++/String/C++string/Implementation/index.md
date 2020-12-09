# `std::tring` implementation

## Small String Optimization

这是一种memory相关的performance optimization，参见工程software-engineering的`Software-analysis\Performance\Memory`章节。

### stackoverflow [Meaning of acronym SSO in the context of std::string](https://stackoverflow.com/questions/10315041/meaning-of-acronym-sso-in-the-context-of-stdstring)

In [a C++ question about optimization and code style](https://stackoverflow.com/questions/10231349/are-the-days-of-passing-const-stdstring-as-a-parameter-over), several answers referred to "SSO" in the context of optimizing copies of `std::string`. What does SSO mean in that context?

Clearly not "single sign on". "Shared string optimization", perhaps?

#### [A](https://stackoverflow.com/a/10319672)

##### Background / Overview

Operations on **automatic variables** ("from the stack", which are variables that you create without calling `malloc` / `new`) are generally much faster than those involving the free store ("the heap", which are variables that are created using `new`). 

However, the size of **automatic arrays** is fixed at compile time, but the size of arrays from the **free store** is not. 

Moreover, the stack size is limited (typically a few MiB), whereas the **free store** is only limited by your system's memory.

SSO is the **Short / Small String Optimization**. A `std::string` typically stores the string as a pointer to the **free store** ("the heap"), which gives similar performance characteristics as if you were to call `new char [size]`. This prevents a **stack overflow** for very large strings, but it can be slower, especially with **copy operations**. As an optimization, many implementations of `std::string` create a **small automatic array**, something like `char [20]`. If you have a string that is 20 characters or smaller (given this example, the actual size varies), it stores it directly in that array. This avoids the need to call `new` at all, which speeds things up a bit.

EDIT:

I wasn't expecting this answer to be quite so popular, but since it is, let me give a more realistic implementation, with the caveat that I've never actually read any implementation of SSO "in the wild".

##### Implementation details

At the minimum, a `std::string` needs to store the following information:

- The size
- The capacity
- The location of the data

The size could be stored as a `std::string::size_type` or as a pointer to the end. The only difference is whether you want to have to subtract two pointers when the user calls `size` or add a `size_type` to a pointer when the user calls `end`. The capacity can be stored either way as well.

##### You don't pay for what you don't use.

First, consider the naive implementation based on what I outlined above:

```cpp
class string {
public:
    // all 83 member functions
private:
    std::unique_ptr<char[]> m_data;
    size_type m_size;
    size_type m_capacity;
    std::array<char, 16> m_sso;
};
```

For a 64-bit system, that generally means that `std::string` has 24 bytes of 'overhead' per string, plus another 16 for the SSO buffer (16 chosen here instead of 20 due to padding requirements). It wouldn't really make sense to store those three data members plus a local array of characters, as in my simplified example. If `m_size <= 16`, then I will put all of the data in `m_sso`, so I already know the capacity and I don't need the pointer to the data. If `m_size > 16`, then I don't need `m_sso`. There is absolutely no overlap where I need all of them. A smarter solution that wastes no space would look something a little more like this (untested, example purposes only):

```cpp
class string {
public:
    // all 83 member functions
private:
    size_type m_size;
    union {
        class {
            // This is probably better designed as an array-like class
            std::unique_ptr<char[]> m_data;
            size_type m_capacity;
        } m_large;
        std::array<char, sizeof(m_large)> m_small;
    };
};
```

I'd assume that most implementations look more like this.

> NOTE: 上述union的使用是值得借鉴的



#### [A](https://stackoverflow.com/a/51796541)

> NOTE: 这个回答讨论的是如何Benchmarks进行验证，在`Software-analysis\Performance\Memory`中收录了这个回答。



## stackoverflow [Why does libc++'s implementation of std::string take up 3x memory as libstdc++?](https://stackoverflow.com/questions/27631065/why-does-libcs-implementation-of-stdstring-take-up-3x-memory-as-libstdc)



### [A](https://stackoverflow.com/a/28003328)

Here is a short program to help you explore both kinds of memory usage of `std::string`: stack and heap.

```cpp
#include <string>
#include <new>
#include <cstdio>
#include <cstdlib>

std::size_t allocated = 0;

void* operator new(size_t sz)
{
	void *p = std::malloc(sz);
	allocated += sz;
	return p;
}

void operator delete(void *p) noexcept
{
	return std::free(p);
}

int main()
{
	allocated = 0;
	std::string s("hi");
	std::printf("stack space = %zu, heap space = %zu, capacity = %zu\n", sizeof(s), allocated, s.capacity());
}
// g++ test.cpp

```

Using http://melpon.org/wandbox/ it is easy to get output for different compiler/lib combinations, for example:

**gcc 4.9.1:**

```cpp
stack space = 8, heap space = 27, capacity = 2
```

**gcc 5.0.0:**

```cpp
stack space = 32, heap space = 0, capacity = 15
```

**clang/libc++:**

```cpp
stack space = 24, heap space = 0, capacity = 22
```

**VS-2015:**

```cpp
stack space = 32, heap space = 0, capacity = 15
```

(the last line is from [http://webcompiler.cloudapp.net](http://webcompiler.cloudapp.net/))

The above output also shows `capacity`, which is a measure of how many `char`s the string can hold before it has to allocate a new, larger buffer from the heap. For the gcc-5.0, libc++, and VS-2015 implementations, this is a measure of the *short string buffer*. That is, the size buffer allocated on the stack to hold short strings, thus avoiding the more expensive **heap allocation**.

It appears that the libc++ implementation has the smallest (stack usage) of the short-string implementations, and yet contains the largest of the short string buffers. And if you count *total* memory usage (stack + heap), libc++ has the smallest total memory usage for this 2-character string among all 4 of these implementations.

It should be noted that all of these measurements were taken on 64 bit platforms. On 32 bit, the libc++ stack usage will go down to 12, and the small string buffer goes down to 10. I don't know the behavior of the other implementations on 32 bit platforms, but you can use the above code to find out.