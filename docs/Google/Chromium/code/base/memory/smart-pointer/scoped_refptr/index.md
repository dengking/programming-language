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

C++允许指针的declaration type和它实际所指向的object的真实type不同，上述函数首先使用 `std::is_convertible<U*, T*>`进行校验，在具体实现上，它使用C++11 delegating constructor特性来调用`scoped_refptr(r.ptr_)`，`r.ptr_` 的类型是 `U*`，而constructor `scoped_refptr(T* p)`的入参类型是 `T*`，因此此处就涉及pointer conversion，由于前面做过了static check，因此这个conversion是可以成功的，下面是简化的例子:

```C++
#include <algorithm>
#include <iostream>
#include <type_traits>
template <class T>
class scoped_refptr
{
public:
	scoped_refptr(T* p) : ptr_(p) {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	template<typename U, typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
	scoped_refptr(const scoped_refptr<U>& p) : scoped_refptr(p.ptr_) {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}

	~scoped_refptr() {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		delete ptr_;
	}
protected:
	T* ptr_ = nullptr;
private:
	// Friend required for move constructors that set r.ptr_ to null.
	// 如果不加这个friend声明，则会产生如下编译报错:
	// test.cpp:12:68: error: ‘Derived* scoped_refptr<Derived>::ptr_’ is protected within this context
	// 12 | st scoped_refptr<U>&p) : scoped_refptr(p.ptr_)
	template <typename U>
	friend class scoped_refptr;
};
class Base {};
class Derived :public Base {};
int main()
{
	auto ptr0 = scoped_refptr<Derived>(new Derived);
	auto ptr1 = scoped_refptr<Base>(ptr0);
}
// g++ test.cpp --std=c++11 -pedantic -Wall 
```

输出如下:

```C++
scoped_refptr<T>::scoped_refptr(T*) [with T = Derived]
scoped_refptr<T>::scoped_refptr(T*) [with T = Base]
scoped_refptr<T>::scoped_refptr(const scoped_refptr<U>&) [with U = Derived; <template-parameter-2-2> = void; T = Base]
scoped_refptr<T>::~scoped_refptr() [with T = Base]
scoped_refptr<T>::~scoped_refptr() [with T = Derived]
free(): double free detected in tcache 2
Aborted (core dumped)
```

上述导致double free的原因非常简单。

#### Move constructor

```C++
// Move constructor. This is required in addition to the move conversion
// constructor below.
scoped_refptr(scoped_refptr&& r) noexcept : ptr_(r.ptr_) { r.ptr_ = nullptr; }
```

可以看到，move constructor的实现非常简单，就是简单的pointer assignment: `ptr_(r.ptr_)`。

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

"Move conversion constructor"的实现也是非常简单的，它在move constructor的基础上增加了`std::is_convertible`的检查，需要注意的是: 它本质上也是直接的pointer assignment。

### Assignment operator

"assignment"其实也就意味着要将current object替换为(replace)为它右边的，current object管理着resource的时候，往往需要分配、deep copy、release，这就诞生了"unified assignment operator"，它是典型的使用copy-and-swap-idiom的，下面是对`scoped_refptr`的简化，以更好地理解它的运行原理:

```C++
#include <algorithm>
#include <iostream>
template <class T>
class scoped_refptr
{
public:
	scoped_refptr(T* p) : ptr_(p) {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	// Move constructor.
	scoped_refptr(scoped_refptr&& r) noexcept : ptr_(r.ptr_) {
		r.ptr_ = nullptr;
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	scoped_refptr& operator=(T* p) {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return *this = scoped_refptr(p);
	}
	// Unified assignment operator.
	scoped_refptr& operator=(scoped_refptr r) noexcept {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		swap(r);
		return *this;
	}
	void swap(scoped_refptr& r) noexcept {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		std::swap(ptr_, r.ptr_);
	}
	~scoped_refptr() {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		delete ptr_;
	}
protected:
	T* ptr_ = nullptr;
};
int main()
{
	auto ptr = scoped_refptr<int>(new int{ 1 });
	ptr = new int{ 2 };
}
// g++ test.cpp --std=c++11 -pedantic -Wall 
```

它的输出如下:

```C++
scoped_refptr<T>::scoped_refptr(T*) [with T = int]
scoped_refptr<T>& scoped_refptr<T>::operator=(T*) [with T = int]
scoped_refptr<T>::scoped_refptr(T*) [with T = int]
scoped_refptr<T>& scoped_refptr<T>::operator=(scoped_refptr<T>) [with T = int]
void scoped_refptr<T>::swap(scoped_refptr<T>&) [with T = int]
scoped_refptr<T>::~scoped_refptr() [with T = int]
scoped_refptr<T>::~scoped_refptr() [with T = int]
```

1、通过上述输出可以看到 `scoped_refptr& operator=(T* p)` 的实现是依赖于 `scoped_refptr& operator=(scoped_refptr r) noexcept` 的。

2、另外一个值得关注的是: `scoped_refptr& operator=(T* p)` 在调用 `scoped_refptr& operator=(scoped_refptr r) noexcept` ，第二个函数的入参是不涉及move constructor的。为了验证这，我特地加了move constructor的，从输出来看，它是未被调用的。

#### Unified assignment operator

```c++
// Unified assignment operator.
scoped_refptr& operator=(scoped_refptr r) noexcept {
    swap(r);
    return *this;
}
```

1、它是典型的使用copy-and-swap-idiom的

2、第二个值得关注的是: 它是 `noexcept` 的

#### from raw pointer

```C++
scoped_refptr& operator=(T* p) { return *this = scoped_refptr(p); }
```

#### from `std::nullptr_t`

```C++
  scoped_refptr& operator=(std::nullptr_t) {
    reset();
    return *this;
  }
```

### `reset`

````C++
  // Sets managed object to null and releases reference to the previous managed
  // object, if it existed.
  void reset() { scoped_refptr().swap(*this); }
````

这个实现也是比较巧妙的: "default constructor + swap"。

### `release()`: release ownership

```C++
// Returns the owned pointer (if any), releasing ownership to the caller. The
// caller is responsible for managing the lifetime of the reference.
T* release() WARN_UNUSED_RESULT;
template <typename T>
T* scoped_refptr<T>::release() {
  T* ptr = ptr_;
  ptr_ = nullptr;
  return ptr;
}
```



### `friend class scoped_refptr`

在类中有如下声明: 

```C++
  // Friend required for move constructors that set r.ptr_ to null.
  template <typename U>
  friend class scoped_refptr;
```

它是做什么的呢？一开始我并没有明白，在编译"Copy conversion constructor"节的例子的时候我找到了答案:

在 copy conversion constructor、move conversion constructor中，入参的类型是 `scoped_refptr<U>` 而不是 `scoped_refptr<T>` ，显然它们不是相同的类型，而在 `scoped_refptr<T>` 中要访问 `scoped_refptr<U>` 的protected member `ptr_`，如果不添加上述friend 声明的话，显然是会导致编译报错的。

## `shared_ptr` vs `scoped_ptr`

参见:

1、stackoverflow [shared_ptr vs scoped_ptr](https://stackoverflow.com/questions/1770636/shared-ptr-vs-scoped-ptr)

