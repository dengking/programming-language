# chromium reference counting

一些使用C++ atomic library的code。



## github [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[atomic_ref_count.h](https://github.com/chromium/chromium/blob/master/base/atomic_ref_count.h)**



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
	constexpr AtomicRefCount() :
					ref_count_(0)
	{
	}
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

## [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[ref_counted.h](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted.h)**

一、**[ref_counted.h](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted.h)** 中的内容是需要结合 [**scoped_refptr.h**](https://github.com/chromium/chromium/blob/master/base/memory/scoped_refptr.h) 来进行阅读的，因为其中引用了很多在  [**scoped_refptr.h**](https://github.com/chromium/chromium/blob/master/base/memory/scoped_refptr.h) 中定义的constant。

二、

1、`RefCountedBase` 

2、`RefCountedThreadSafe` 



### `class BASE_EXPORT base::subtle::RefCountedBase`



```C++
class BASE_EXPORT RefCountedBase {
  // non-copyable
  RefCountedBase(const RefCountedBase&) = delete;
    
  RefCountedBase& operator=(const RefCountedBase&) = delete;
  // 
  explicit RefCountedBase(StartRefCountFromZeroTag) { }

  explicit RefCountedBase(StartRefCountFromOneTag) : ref_count_(1) { }
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

### `class BASE_EXPORT base::subtle::RefCountedThreadSafeBase`

一、**Thread-safe**，基于`class AtomicRefCount`的。

二、需要注意，它是 `DISALLOW_COPY_AND_ASSIGN` 的，为什么要这样设计？



三、为什么 `Release()` 的返回值类型是`bool`？

它的返回值的含义是:

> Returns true if the object should self-delete.



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

`if (!ref_count_.Decrement())` 

当 `ref_count_.Decrement()` 返回为 false时，表示 `ref_count_` 为 0 了，此时 `!ref_count_.Decrement()` 为 TRUE，说应该release resource了，所以 `ReleaseImpl()` 应该返回TRUE。

四、

在 [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[ref_counted.h](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted.h)** 中，有如下定义: 

```C++
#if defined(ARCH_CPU_X86_FAMILY)
  // Returns true if the object should self-delete.
  bool Release() const { return ReleaseImpl(); }
  void AddRef() const { AddRefImpl(); }
  void AddRefWithCheck() const { AddRefWithCheckImpl(); }
#else
  // Returns true if the object should self-delete.
  bool Release() const;
  void AddRef() const;
  void AddRefWithCheck() const;
#endif
```

那 `#if !defined(ARCH_CPU_X86_FAMILY)` 时，`Release()` 等的implementation在哪里呢？在 [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[ref_counted.cc](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted.cc)**  中有如下implementation:

```C++
#if !defined(ARCH_CPU_X86_FAMILY)
bool RefCountedThreadSafeBase::Release() const {
  return ReleaseImpl();
}
void RefCountedThreadSafeBase::AddRef() const {
  AddRefImpl();
}
void RefCountedThreadSafeBase::AddRefWithCheck() const {
  AddRefWithCheckImpl();
}
#endif
```



五、`friend scoped_refptr<U> base::AdoptRef(U*);`

它和`refptr`的关联是什么？



### CRTP mixin from above + friend base class

```c++
template <class T, typename Traits = DefaultRefCountedTraits<T>>
class RefCounted : public subtle::RefCountedBase
template <class T, typename Traits = DefaultRefCountedThreadSafeTraits<T> >
class RefCountedThreadSafe : public subtle::RefCountedThreadSafeBase
template<typename T>
class RefCountedData
```



### Make destructor private

> 1、You should always make your destructor non-public, to avoid any code deleting the object accidentally while there are references to it.
>
> 2、You should always make the ref-counted base class a friend of your class, so that it can access the destructor.

典型的采用 "CRTP mixin from above + friend base class"，通过 "make the ref-counted base class a friend of your class, so that it can access the destructor"，从而保证当reference count为0的时候，private destructor能够被调用。



### `DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafeBase);`

一、因为它是需要reference count来进行管理的，因此对象本身是不能够copy、assign的，但是pointer to object是能够被copy、assign的，并且pointer to object是能够调用它的函数的；

二、对于需要reference count的object，显然它是需要提供new来进行创建的，一般是不能够在heap上创建的。

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



## [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[ref_counted_memory.h](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted_memory.h)**

```c++
class BASE_EXPORT RefCountedMemory
    : public RefCountedThreadSafe<RefCountedMemory> 
class BASE_EXPORT RefCountedStaticMemory : public RefCountedMemory
class BASE_EXPORT RefCountedBytes : public RefCountedMemory
class BASE_EXPORT RefCountedString : public RefCountedMemory
class BASE_EXPORT RefCountedString16 : public base::RefCountedMemory 
class BASE_EXPORT RefCountedSharedMemoryMapping : public RefCountedMemory
```