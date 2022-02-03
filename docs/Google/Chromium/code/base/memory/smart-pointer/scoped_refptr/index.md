# github [chromium](https://github.com/chromium/chromium/tree/master)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/[**scoped_refptr.h**](https://github.com/chromium/chromium/blob/master/base/memory/scoped_refptr.h)

## doc

```C++
// A smart pointer class for reference counted objects.  Use this class instead
// of calling AddRef and Release manually on a reference counted object to
// avoid common memory leaks caused by forgetting to Release an object
// reference. 
```

一、如何理解上面这段话？

1、首先需要清楚的是 "reference counted objects" 的含义: 它是指继承了在[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[ref_counted.h](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted.h)** 中定义的 `RefCounted`、`RefCountedThreadSafe` 之一的，典型的例子:

a、下面的 `class MyFoo`

b、[chromium](https://github.com/chromium/chromium/tree/master)/[base](https://github.com/chromium/chromium/tree/master/base)/[**callback_internal.h**](https://github.com/chromium/chromium/blob/master/base/callback_internal.h) 

```C++
class BASE_EXPORT BindStateBase
    : public RefCountedThreadSafe<BindStateBase, BindStateBaseRefCountTraits>
```



2、`scoped_refptr` 本质上是RAII wrapper for `RefCounted`、`RefCountedThreadSafe`: 

```C++
// Use this class instead
// of calling AddRef and Release manually on a reference counted object to
// avoid common memory leaks caused by forgetting to Release an object
// reference. 
```

3、`scoped_refptr` 是smart pointer，它的usage和`std::shared_ptr`是非常类似的。

### example

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

需要注意的是: `RefCounted` 提供了无惨的default constructor，compiler会自动调用。

### example: exchange references

```c++
{
    scoped_refptr<MyFoo> a = MakeRefCounted<MyFoo>();
    scoped_refptr<MyFoo> b;

    b.swap(a); // now, |b| references the MyFoo object, and |a| references nullptr.
}
```



### example: assignment

```C++
{
    scoped_refptr<MyFoo> a = MakeRefCounted<MyFoo>();
    scoped_refptr<MyFoo> b;

    b = a; // now, |a| and |b| each own a reference to the same MyFoo object.
}
```



## code

需要处理如下类型的转换:

1、raw pointer

2、`scoped_ptr` of other type

3、`scoped_ptr` of same type



```C++
template <class T>
class TRIVIAL_ABI scoped_refptr {
public:
  typedef T element_type;
protected:
  T* ptr_ = nullptr;
};  
```



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

### object generator 



## `shared_ptr` vs `scoped_ptr`

参见:

1、stackoverflow [shared_ptr vs scoped_ptr](https://stackoverflow.com/questions/1770636/shared-ptr-vs-scoped-ptr)

