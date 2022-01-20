# github [thelink2012](https://github.com/thelink2012)/**[any](https://github.com/thelink2012/any)**

简单地看了一下它的implementation:

1、custom virtual table，关于此，参见 `Small-Object-Optimization-for-Polymorphic-Types` 章节

2、small object optimization

```C++
union storage_union
{
    using stack_storage_t = typename std::aligned_storage<2 * sizeof(void*), std::alignment_of<void*>::value>::type;

    void*               dynamic;
    stack_storage_t     stack;      // 2 words for e.g. shared_ptr
};
```

3、type erasure