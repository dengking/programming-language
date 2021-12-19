# `scoped_refptr`



## usage

```C++
class MyFoo : public RefCounted<MyFoo> {
  ...
  private:
  friend class RefCounted<MyFoo>;  // Allow destruction by RefCounted<>.
  ~MyFoo();                        // Destructor must be private/protected.
};

void some_function() {
  scoped_refptr<MyFoo> foo = MakeRefCounted<MyFoo>();
  foo->Method(param);
  // |foo| is released when this function returns
}
//
void some_other_function() {
  scoped_refptr<MyFoo> foo = MakeRefCounted<MyFoo>();
  ...
  foo.reset();  // explicitly releases |foo|
  ...
  if (foo)
    foo->Method(param);
}
```



## [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[scoped_refptr.h](https://github.com/chromium/chromium/blob/master/base/memory/scoped_refptr.h)**

需要处理如下类型的转换:

1、raw pointer

2、`scoped_ptr` of other type

3、`scoped_ptr` of same type



```C++
  // Constructs from a raw pointer. Note that this constructor allows implicit
  // conversion from T* to scoped_refptr<T> which is strongly discouraged. If
  // you are creating a new ref-counted object please use
  // base::MakeRefCounted<T>() or base::WrapRefCounted<T>(). Otherwise you
  // should move or copy construct from an existing scoped_refptr<T> to the
  // ref-counted object.
  scoped_refptr(T* p) : ptr_(p) {
    if (ptr_)
      AddRef(ptr_);
  }
```

通过comment可知，上述函数是不建议使用的；



## `shared_ptr` vs `scoped_ptr`

参见:

1、stackoverflow [shared_ptr vs scoped_ptr](https://stackoverflow.com/questions/1770636/shared-ptr-vs-scoped-ptr)

