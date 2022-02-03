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



```C++
template <class T>
class TRIVIAL_ABI scoped_refptr {
public:
  typedef T element_type;
protected:
  T* ptr_ = nullptr;
};  
```

`T` 的类型是 "ref counted type"。

### object generator 

#### `AdoptRef`

```C++
// Creates a scoped_refptr from a raw pointer without incrementing the reference
// count. Use this only for a newly created object whose reference count starts
// from 1 instead of 0.
template <typename T>
scoped_refptr<T> AdoptRef(T* obj) {
	using Tag = std::decay_t<decltype( T::kRefCountPreference )>;
	static_assert( std::is_same<subtle::StartRefCountFromOneTag, Tag>::value,
		"Use AdoptRef only if the reference count starts from one." );

	DCHECK(obj);
	DCHECK(obj->HasOneRef());
	obj->Adopted();
	return scoped_refptr<T>(obj, subtle::kAdoptRefTag);
}
```

可以看到: `AdoptRef`仅仅用于 `StartRefCountFromOneTag`。

#### `MakeRefCounted`

```c++
namespace subtle {
	template <typename T>
	scoped_refptr<T> AdoptRefIfNeeded(T* obj, StartRefCountFromZeroTag) {
		return scoped_refptr<T>(obj);
	}

	template <typename T>
	scoped_refptr<T> AdoptRefIfNeeded(T* obj, StartRefCountFromOneTag) {
		return AdoptRef(obj);
	}
}  // namespace subtle

// Constructs an instance of T, which is a ref counted type, and wraps the
// object into a scoped_refptr<T>.
template <typename T, typename... Args>
scoped_refptr<T> MakeRefCounted(Args&&... args) {
	T* obj = new T(std::forward<Args>(args)...);
	return subtle::AdoptRefIfNeeded(obj, T::kRefCountPreference);
}
```



#### `WrapRefCounted`

```C++
// Takes an instance of T, which is a ref counted type, and wraps the object
// into a scoped_refptr<T>.
template <typename T>
scoped_refptr<T> WrapRefCounted(T* t) {
  return scoped_refptr<T>(t);
}
```

### constructor

需要处理如下类型的转换:

1、raw pointer

2、`scoped_ptr` of other type

3、`scoped_ptr` of same type

4、`std::nullptr_t`

#### from raw pointer

```C++
// Constructs from a raw pointer. Note that this constructor allows implicit
// conversion from T* to scoped_refptr<T> which is strongly discouraged. If
// you are creating a new ref-counted object please use
// base::MakeRefCounted<T>() or base::WrapRefCounted<T>(). Otherwise you
// should move or copy construct from an existing scoped_refptr<T> to the
// ref-counted object.
scoped_refptr(T* p) : ptr_(p) {
    if (ptr_)
        AddRef(ptr_); // 注意，需要AddRef一下
}
```

通过comment可知，上述函数是不建议使用的，而是应该直接使用object generator function。

#### from `std::nullptr_t`

```C++
// Allow implicit construction from nullptr.
constexpr scoped_refptr(std::nullptr_t) {}
```

#### Copy constructor

```C++
// Copy constructor. This is required in addition to the copy conversion
// constructor below.
scoped_refptr(const scoped_refptr& r) : scoped_refptr(r.ptr_) {}
```

上述copy constructor会调用前面的"from raw pointer constructor"，这是使用的C++11 delegating constructor特性。

#### Copy conversion constructor

```C++
// Copy conversion constructor.
template <
	typename U,
	typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type
>
scoped_refptr(const scoped_refptr<U>& r) : scoped_refptr(r.ptr_) {}
```

一、上述copy constructor会调用前面的"from raw pointer constructor"，这是使用的C++11 delegating constructor特性。

二、上述代码虽然简单，但是我比较疑惑的是它会进行哪些conversion？

指针的declaration type和它实际所指向的object的真实type不同，这在C++中是允许的。

#### Move constructor

```C++
// Move constructor. This is required in addition to the move conversion
// constructor below.
scoped_refptr(scoped_refptr&& r) noexcept : ptr_(r.ptr_) { r.ptr_ = nullptr; }
```

#### Move conversion constructor

```C++
// Move conversion constructor.
template <
    typename U,
    typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type
>
scoped_refptr(scoped_refptr<U>&& r) noexcept : ptr_(r.ptr_) {
    r.ptr_ = nullptr;
}
```

### assignment operator



## `shared_ptr` vs `scoped_ptr`

参见:

1、stackoverflow [shared_ptr vs scoped_ptr](https://stackoverflow.com/questions/1770636/shared-ptr-vs-scoped-ptr)

