# chromium reference counting

## github [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[atomic_ref_count.h](https://github.com/chromium/chromium/blob/master/base/atomic_ref_count.h)**

### `class AtomicRefCount`

```C++
// This is a low level implementation of atomic semantics for reference
// counting.  Please use base/memory/ref_counted.h directly instead.
#ifndef BASE_ATOMIC_REF_COUNT_H_
#define BASE_ATOMIC_REF_COUNT_H_
#include <atomic>
namespace base
{
class AtomicRefCount
{
public:
	constexpr AtomicRefCount() : ref_count_(0)	{	}
	explicit constexpr AtomicRefCount(int initial_value) :
					ref_count_(initial_value)
	{
	}

	// Increment a reference count.
	// Returns the previous value of the count.
	int Increment()
	{
		return Increment(1);
	}

	// Increment a reference count by "increment", which must exceed 0.
	// Returns the previous value of the count.
	int Increment(int increment)
	{
		return ref_count_.fetch_add(increment, std::memory_order_relaxed);
	}

	// Decrement a reference count, and return whether the result is non-zero.
	// Insert barriers to ensure that state written before the reference count
	// became zero will be visible to a thread that has just made the count zero.
	bool Decrement()
	{
		// TODO(jbroman): Technically this doesn't need to be an acquire operation
		// unless the result is 1 (i.e., the ref count did indeed reach zero).
		// However, there are toolchain issues that make that not work as well at
		// present (notably TSAN doesn't like it).
		return ref_count_.fetch_sub(1, std::memory_order_acq_rel) != 1;
	}

	// Return whether the reference count is one.  If the reference count is used
	// in the conventional way, a refrerence count of 1 implies that the current
	// thread owns the reference and no other thread shares it.  This call
	// performs the test for a reference count of one, and performs the memory
	// barrier needed for the owning thread to act on the object, knowing that it
	// has exclusive access to the object.
	bool IsOne() const
	{
		return ref_count_.load(std::memory_order_acquire) == 1;
	}

	// Return whether the reference count is zero.  With conventional object
	// referencing counting, the object will be destroyed, so the reference count
	// should never be zero.  Hence this is generally used for a debug check.
	bool IsZero() const
	{
		return ref_count_.load(std::memory_order_acquire) == 0;
	}

	// Returns the current reference count (with no barriers). This is subtle, and
	// should be used only for debugging.
	int SubtleRefCountForDebug() const
	{
		return ref_count_.load(std::memory_order_relaxed);
	}
private:
	std::atomic_int ref_count_;
};
}  // namespace base

#endif  // BASE_ATOMIC_REF_COUNT_H_

```

一、`fetch_sub` 的返回值是之前的值，因此当 `fetch_sub` 的返回值是 `1` 的时候，说明现在它的值是 `0`。

二、`IsZero`、`IsZero` 显然都是read acquire

三、它的constructor都是 `constexpr` 的

四、它的功能是reference counting

### `Decrement`

```C++
	// Decrement a reference count, and return whether the result is non-zero.
	// Insert barriers to ensure that state written before the reference count
	// became zero will be visible to a thread that has just made the count zero.
	bool Decrement()
	{
		// TODO(jbroman): Technically this doesn't need to be an acquire operation
		// unless the result is 1 (i.e., the ref count did indeed reach zero).
		// However, there are toolchain issues that make that not work as well at
		// present (notably TSAN doesn't like it).
		return ref_count_.fetch_sub(1, std::memory_order_acq_rel) != 1;
	}
```

一、注释的表明意思是:

"Insert barriers"的目的是保证在reference count成为0之前，被写入的state("state written before the reference count became zero")对另外一个刚刚使得reference count成为0的thread是可见的(visible)，本质上是 "ordering constrain"

二、[`fetch_sub`](https://en.cppreference.com/w/cpp/atomic/atomic/fetch_sub) 的返回值是之前的值，因此当 `fetch_sub` 的返回值是 `1` 的时候，说明现在它的值是 `0`。

三、对于返回值是`bool`的function，需要read acquire，因此，需要使用 `std::memory_order_acquire`

四、这个函数的返回值表示: 当前的reference count的值是否是非0的，如果当前reference count的值是非0，则返回为TRUE；否则返回为false，即当前的值为0了。

## github [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[ref_counted.h](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted.h)**

一、**[ref_counted.h](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted.h)** 中的内容是需要结合 [**scoped_refptr.h**](https://github.com/chromium/chromium/blob/master/base/memory/scoped_refptr.h) 来进行阅读的，因为其中引用了很多在  [**scoped_refptr.h**](https://github.com/chromium/chromium/blob/master/base/memory/scoped_refptr.h) 中定义的constant。

二、

1、`RefCountedBase`、 `RefCounted`

2、`RefCountedThreadSafeBase`、 `RefCountedThreadSafe`

### Tag dispatching: constructor

`RefCountedBase`、`RefCountedThreadSafeBase` 两个函数都提供了如下模式的constructor:

```C++
  explicit RefCountedBase(StartRefCountFromZeroTag) { }
  explicit RefCountedBase(StartRefCountFromOneTag) : ref_count_(1) { }
```

这是典型的tag dispatch用法，在它们的子类 `RefCounted`、`RefCountedThreadSafe` 的constructor中，会调用上述constructor中的一个。

### `class base::subtle::RefCountedBase`



```C++
class BASE_EXPORT RefCountedBase {
  // 
  explicit RefCountedBase(StartRefCountFromZeroTag) { }
  explicit RefCountedBase(StartRefCountFromOneTag) : ref_count_(1) { }
  // non-copyable
  RefCountedBase(const RefCountedBase&) = delete;
  RefCountedBase& operator=(const RefCountedBase&) = delete;
  // 下面是主要的API
  bool HasOneRef() const;
  bool HasAtLeastOneRef() const;
  void AddRef() const;
  // Returns true if the object should self-delete.
  bool Release() const;
private:    
  mutable uint32_t ref_count_ = 0;
  static_assert(std::is_unsigned<decltype(ref_count_)>::value,
                "ref_count_ must be an unsigned type.");
};
```

一、典型的:

1、tag-dispatch

2、explicit single parameter constructor

二、它是非线程安全的

三、显然是non-copyable

#### How to trap unsafe cross thread usage?

它使用的是 `DFAKE_MUTEX(add_release_);`

### `class base::subtle::RefCountedThreadSafeBase`

```C++
class BASE_EXPORT RefCountedThreadSafeBase {
  // constructor
  explicit constexpr RefCountedThreadSafeBase(StartRefCountFromZeroTag) {}
  explicit constexpr RefCountedThreadSafeBase(StartRefCountFromOneTag) {}    
  //
  bool HasOneRef() const;
  bool HasAtLeastOneRef() const;
  // Returns true if the object should self-delete.
  bool Release() const;
  void AddRef() const;
private:
  mutable AtomicRefCount ref_count_{0};
};
```

一、**Thread-safe**，基于`class AtomicRefCount`的。

二、需要注意，它是 `DISALLOW_COPY_AND_ASSIGN` 的，为什么要这样设计？

三、为什么 `Release()` 的返回值类型是`bool`？

它的返回值的含义是:

> Returns true if the object should self-delete.

#### `ReleaseImpl`

```C++
  ALWAYS_INLINE bool ReleaseImpl() const {
#if DCHECK_IS_ON()
    DCHECK(!in_dtor_);
    DCHECK(!ref_count_.IsZero());
#endif
    if (!ref_count_.Decrement()) {
#if DCHECK_IS_ON()
      in_dtor_ = true;
#endif
      return true;
    }
    return false;
  }
```

1、当 `ref_count_.Decrement()` 返回为 false时，表示 `ref_count_` 为 0 了，此时 `!ref_count_.Decrement()` 为 TRUE，说应该release resource了，所以 `ReleaseImpl()` 应该返回TRUE。



### Usage

`class base::RefCounted`、`class base::RefCountedThreadSafe` 的usage模式是类似的，下面是总结:

#### CRTP mixin from above + friend base class

在 `base::RefCounted` 的document中给出了它们的usage pattern

```C++
class MyFoo : public base::RefCounted<MyFoo> {
	//...
private:
	friend class base::RefCounted<MyFoo>;
	~MyFoo(); // "make destructor private"，后面会有说明
};
```



#### Make destructor private

在 `base::RefCounted` 的document中有如下描述:

> 1、You should always make your destructor non-public, to avoid any code deleting the object accidentally while there are references to it.
>
> 2、You should always make the ref-counted base class a friend of your class, so that it can access the destructor.

典型的采用 "CRTP mixin from above + friend base class"，通过 "make the ref-counted base class a friend of your class, so that it can access the destructor"，从而保证当reference count为0的时候，private destructor能够被调用，并且避免被外部意外调用，前面的例子就是对此的展示

### Reference count 初始值

```C++
// The reference count starts from zero by default, and we intended to migrate
// to start-from-one ref count. Put REQUIRE_ADOPTION_FOR_REFCOUNTED_TYPE() to
// the ref counted class to opt-in.
//
// If an object has start-from-one ref count, the first scoped_refptr need to be
// created by base::AdoptRef() or base::MakeRefCounted(). We can use
// base::MakeRefCounted() to create create both type of ref counted object.
//
// The motivations to use start-from-one ref count are:
//  - Start-from-one ref count doesn't need the ref count increment for the
//    first reference.
//  - It can detect an invalid object acquisition for a being-deleted object
//    that has zero ref count. That tends to happen on custom deleter that
//    delays the deletion.
//    TODO(tzik): Implement invalid acquisition detection.
//  - Behavior parity to Blink's WTF::RefCounted, whose count starts from one.
//    And start-from-one ref count is a step to merge WTF::RefCounted into
//    base::RefCounted.
```

关于第一段话，在后面的"Static polymorphism: `T::kRefCountPreference`"中会进行解释。后面两段话解释了使用"start-from-one ref count"的原因。

### Policy-based design: Traits

模板参数 `Traits` 是典型的使用policy-based design，用于对object lifetime进行控制。

```c++
template <class T, typename Traits = DefaultRefCountedTraits<T>>
class RefCounted : public subtle::RefCountedBase
template <class T, typename Traits = DefaultRefCountedThreadSafeTraits<T> >
class RefCountedThreadSafe : public subtle::RefCountedThreadSafeBase
template<typename T>
class RefCountedData
```

### tag dispatching static polymorphism: `T::kRefCountPreference`

`class base::RefCounted`、`class base::RefCountedThreadSafe` 在调用base class的constructor的时候，采用的写法如下:

```C++
RefCounted() : subtle::RefCountedBase(T::kRefCountPreference) {}
```

 这样做的原因是: 

1、在前面我们已经知道`RefCountedBase` 提供了两个constructor，提供tag来进行区分，这是场景的tag dispatching玩法

2、"preference"的含义是偏好，`T::kRefCountPreference`的值是`RefCountedBase` 的两个tag之一，sub class `T`可以通过定义`kRefCountPreference`来进行选择，显然这是static polymorphism，在原文的document中，对此的说法是"opt-in"

```C++
// The reference count starts from zero by default, and we intended to migrate
// to start-from-one ref count. Put REQUIRE_ADOPTION_FOR_REFCOUNTED_TYPE() to
// the ref counted class to opt-in.
```

我之前在恒生公司开发UST API的时候，就用过这个technique。

`class base::RefCounted`、`class base::RefCountedThreadSafe`中都有定义static inline member `kRefCountPreference`，这样做的目的是:

1、可以由subclass `T`来指定 kRefCountPreference 的具体值进行override，它们的definition是兜底的，因为它会被子类继承，因此如果base class没有进行override，那么可以使用base class的

### 主要接口

1、`AddRef()`

2、`Release()`

### `class base::RefCounted`

```C++
template <typename T>
struct DefaultRefCountedTraits {
	static void Destruct(const T* x) {
		RefCounted<T, DefaultRefCountedTraits>::DeleteInternal(x);
	}
};

template <class T, typename Traits = DefaultRefCountedTraits<T>>
class RefCounted : public subtle::RefCountedBase {
public:
    /// 这是一个static inline member，它是用于tag dispatch的: 
	static constexpr subtle::StartRefCountFromZeroTag kRefCountPreference = subtle::kStartRefCountFromZeroTag; 
    /// 需要注意第一个入参，它是用于override
	RefCounted() : subtle::RefCountedBase(T::kRefCountPreference) {}

	RefCounted(const RefCounted&) = delete;
	RefCounted& operator=(const RefCounted&) = delete;

	void AddRef() const {
		subtle::RefCountedBase::AddRef();
	}
    /// 
	void Release() const {
		if (subtle::RefCountedBase::Release()) {
			// Prune the code paths which the static analyzer may take to simulate
			// object destruction. Use-after-free errors aren't possible given the
			// lifetime guarantees of the refcounting system.
			ANALYZER_SKIP_THIS_PATH();
			Traits::Destruct(static_cast<const T*>( this )); // 这是CRTP的常规做法
		}
	}
protected:
	~RefCounted() = default;
private:
	friend struct DefaultRefCountedTraits<T>;
	template <typename U>
	static void DeleteInternal(const U* x) {
		delete x;
	}
};
```



### `class base::RefCountedThreadSafe`

```C++
template <class T, typename Traits = DefaultRefCountedThreadSafeTraits<T> >
class RefCountedThreadSafe : public subtle::RefCountedThreadSafeBase {
public:
	static constexpr subtle::StartRefCountFromZeroTag kRefCountPreference = subtle::kStartRefCountFromZeroTag;

	explicit RefCountedThreadSafe()
		: subtle::RefCountedThreadSafeBase(T::kRefCountPreference) {}

	RefCountedThreadSafe(const RefCountedThreadSafe&) = delete;
	RefCountedThreadSafe& operator=(const RefCountedThreadSafe&) = delete;

	void AddRef() const { AddRefImpl(T::kRefCountPreference); }

	void Release() const {
		if (subtle::RefCountedThreadSafeBase::Release()) {
			ANALYZER_SKIP_THIS_PATH();
			Traits::Destruct(static_cast<const T*>( this ));
		}
	}

protected:
	~RefCountedThreadSafe() = default;

private:
	friend struct DefaultRefCountedThreadSafeTraits<T>;
	template <typename U>
	static void DeleteInternal(const U* x) {
		delete x;
	}

	void AddRefImpl(subtle::StartRefCountFromZeroTag) const {
		subtle::RefCountedThreadSafeBase::AddRef();
	}

	void AddRefImpl(subtle::StartRefCountFromOneTag) const {
		subtle::RefCountedThreadSafeBase::AddRefWithCheck();
	}
};
```

### `class base::RefCountedData`

```C++
// A thread-safe wrapper for some piece of data so we can place other
// things in scoped_refptrs<>.
template<typename T>
class RefCountedData : public base::RefCountedThreadSafe< base::RefCountedData<T> > {
public:
	RefCountedData() : data() {}
	RefCountedData(const T& in_value) : data(in_value) {}
	RefCountedData(T&& in_value) : data(std::move(in_value)) {}
	template <typename... Args>
	explicit RefCountedData(in_place_t, Args&&... args)
		: data(std::forward<Args>(args)...) {}

	T data;

private:
	friend class base::RefCountedThreadSafe<base::RefCountedData<T> >;
	~RefCountedData() = default;
};
```

它是典型的contained object，这一点和`std::any`是非常类似的。

### Adopt ref

`StartRefCountFromOneTag`

```C++
  explicit constexpr RefCountedThreadSafeBase(StartRefCountFromOneTag)
      : ref_count_(1) {
#if DCHECK_IS_ON()
    needs_adopt_ref_ = true;
#endif
  }
```

在成员方法 `Adopted()` 中会将 `needs_adopt_ref_` 的值设置为`false`。

#### friend `base::AdoptRef`

因为成员方法 `void Adopted() const` 是private的，因此需要添加上述声明



五、`friend scoped_refptr<U> base::AdoptRef(U*);`

它和`refptr`的关联是什么？



### `DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafeBase);`

一、因为它是需要reference count来进行管理的，因此对象本身是不能够copy、assign的，但是pointer to object是能够被copy、assign的，并且pointer to object是能够调用它的函数的；

二、对于需要reference count的object，显然它是需要提供new来进行创建的，一般是不能够在heap上创建的。