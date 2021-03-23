# Check incomplete type

1、由于delete incomplete type是undefined的behavior，因此我们需要考虑如何判断是否是incomplete type。

2、与本节内容相关的是`Detect-whether-a-type-is-defined`章节

## stackoverflow [Delete objects of incomplete type](https://stackoverflow.com/questions/4325154/delete-objects-of-incomplete-type) # [A](https://stackoverflow.com/a/4325900)

It is undefined behavior.

However, you can make the compiler check for incomplete types, like boost:

```cpp
// verify that types are complete for increased safety
template<class T>
inline void checked_delete(T *x)
{
	// intentionally complex - simplification causes regressions
	typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
	(void) sizeof(type_must_be_complete);
	delete x;
}

```

Applying `sizeof` to an incomplete type should trigger an error, and I suppose if that passes with some compiler, then an array of negative size would trigger an error.



## `std::unique_ptr`

本地文件位置: `/usr/include/c++/4.8.2/bits/unique_ptr.h`

URL: https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.4/a01404.html

```C++
  /// Primary template, default_delete.
  template<typename _Tp>
    struct default_delete
    {
      constexpr default_delete() noexcept = default;

      template<typename _Up, typename = typename
               enable_if<is_convertible<_Up*, _Tp*>::value>::type>
        default_delete(const default_delete<_Up>&) noexcept { }

      void
      operator()(_Tp* __ptr) const
      {
        static_assert(sizeof(_Tp)>0,
                      "can't delete pointer to incomplete type");
        delete __ptr;
      }
    };
```



### stackoverflow [std::unique_ptr with an incomplete type won't compile](https://stackoverflow.com/questions/9954518/stdunique-ptr-with-an-incomplete-type-wont-compile)

> NOTE: 从提问者贴出的编译报错信息来看，也是采用的上面描述的那种方式。