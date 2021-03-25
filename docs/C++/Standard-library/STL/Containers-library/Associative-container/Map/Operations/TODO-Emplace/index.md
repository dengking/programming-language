# emplace

## C++ map emplace `class`  `mapped_type` 

对于`mapped_type`是`class`的，如何实现emplace呢？

### stackoverflow [std::map emplace without copying value](https://stackoverflow.com/questions/27960325/stdmap-emplace-without-copying-value)

```C++
struct Foo
{
   Foo(double d, string s) {}
   Foo(const Foo&) = delete;
   Foo(Foo&&) = delete;
}

map<int,Foo> m;
m.emplace(1, 2.3, string("hello")); // invalid
```

> NOTE: 在C++11中，还不支持这种写法，C++17支持了

#### [A](https://stackoverflow.com/a/27960637)

The arguments you pass to `map::emplace` get forwarded to the constructor of `map::value_type`, which is `pair<const Key, Value>`. So you can use the [piecewise construction constructor](http://en.cppreference.com/w/cpp/utility/pair/pair) of `std::pair` to avoid intermediate copies and moves.

```c++
std::map<int, Foo> m;

m.emplace(std::piecewise_construct,
          std::forward_as_tuple(1),
          std::forward_as_tuple(2.3, "hello"));
```

[Live demo](http://coliru.stacked-crooked.com/a/c475c316b77f542b)

#### [A](https://stackoverflow.com/a/39881701)

In C++17 this can more easily be achieved with the [`try_emplace`](http://en.cppreference.com/w/cpp/container/map/try_emplace) method.

```c++
map<int,Foo> m;
m.try_emplace(1, 2.3, "hello");
```

This addition to the standard library was covered in [paper N4279](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4279.html) and should already be supported in [Visual Studio 2015](https://blogs.msdn.microsoft.com/vcblog/2016/01/22/vs-2015-update-2s-stl-is-c17-so-far-feature-complete/), [GCC 6.1](https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.201z) and [LLVM 3.7](http://libcxx.llvm.org/cxx1z_status.html) (the libc++ library).

**Comments**

Yes, `try_emplace()` is really the best solution. In particular, `emplace()` always constructs a Key-Value pair on the heap. So, if the Key is actually found in the table, `emplace()` will delete that just newly constructed Key-Value pair again. `try_emplace` on the contrary does everything in the expected order: Check, if they Key exists, and if yes, return an iterator to that Key-Value pair. If not, then it emplaces the new Key and Value into the container. – [Kai Petzke](https://stackoverflow.com/users/2528436/kai-petzke) [Sep 12 '19 at 0:06](https://stackoverflow.com/questions/27960325/stdmap-emplace-without-copying-value#comment102217158_39881701)

## cppreference [std::unordered_map<Key,T,Hash,KeyEqual,Allocator>::emplace](https://en.cppreference.com/w/cpp/container/unordered_map/emplace)



## cppreference [try_emplace](http://en.cppreference.com/w/cpp/container/map/try_emplace)