# Argument wrapper unwrap

## 为什么需要argument wrapper

我觉得使用argument wrapper，显然是引入了一层控制，这样这个system就更加灵活了

表达了argument的:

1、ownership 

2、constness

### wrap and unwrap

一、`bind`的时候需要对argument进行wrap

二、在进行`Run`的时候，需要对argument进行unwrap。

在 `Invoker::RunImpl` 中，有如下实现:

```C++
  template <typename Functor, typename BoundArgsTuple, size_t... indices>
  static inline R RunImpl(Functor&& functor,
                          BoundArgsTuple&& bound,
                          std::index_sequence<indices...>,
                          UnboundArgs&&... unbound_args) {
    static constexpr bool is_method = MakeFunctorTraits<Functor>::is_method;

    using DecayedArgsTuple = std::decay_t<BoundArgsTuple>;
    static constexpr bool is_weak_call =
        IsWeakMethod<is_method,
                     std::tuple_element_t<indices, DecayedArgsTuple>...>();

    return InvokeHelper<is_weak_call, R>::MakeItSo(
        std::forward<Functor>(functor),
        Unwrap(std::get<indices>(std::forward<BoundArgsTuple>(bound)))...,
        std::forward<UnboundArgs>(unbound_args)...);
  }
};
```

其中 `Unwrap(std::get<indices>(std::forward<BoundArgsTuple>(bound)))...` 就是对argument进行unwrap，这是调用的地方，关于具体的实现，参见后面的章节。


## wrapper
有着多种wrapper
### `UnretainedWrapper`

```C++
class UnretainedWrapper 
```
### `UnretainedRefWrapper`

```C++
class UnretainedRefWrapper 
```
### `OwnedWrapper` 

```C++
template <typename T, typename Deleter = std::default_delete<T>>
class OwnedWrapper {
 public:
  explicit OwnedWrapper(T* o) : ptr_(o) {}
  explicit OwnedWrapper(std::unique_ptr<T, Deleter>&& ptr)
      : ptr_(std::move(ptr)) {}
  T* get() const { return ptr_.get(); }

 private:
  std::unique_ptr<T, Deleter> ptr_;
};
```

### `PassedWrapper`





### `Unretained`

```C++
// Unretained() allows binding a non-refcounted class, and to disable
// refcounting on arguments that are refcounted objects.
//
// EXAMPLE OF Unretained():
//
//   class Foo {
//    public:
//     void func() { cout << "Foo:f" << endl; }
//   };
//
//   // In some function somewhere.
//   Foo foo;
//   OnceClosure foo_callback =
//       BindOnce(&Foo::func, Unretained(&foo));
//   std::move(foo_callback).Run();  // Prints "Foo:f".
//
// Without the Unretained() wrapper on |&foo|, the above call would fail
// to compile because Foo does not support the AddRef() and Release() methods.
template <typename T>
inline internal::UnretainedWrapper<T> Unretained(T* o) {
  return internal::UnretainedWrapper<T>(o);
}

template <typename T>
inline internal::UnretainedWrapper<T> Unretained(const raw_ptr<T>& o) {
  return internal::UnretainedWrapper<T>(o.get());
}


```

### `RetainedRefWrapper`

```C++
// RetainedRef() accepts a ref counted object and retains a reference to it.
// When the callback is called, the object is passed as a raw pointer.
//
// EXAMPLE OF RetainedRef():
//
//    void foo(RefCountedBytes* bytes) {}
//
//    scoped_refptr<RefCountedBytes> bytes = ...;
//    OnceClosure callback = BindOnce(&foo, base::RetainedRef(bytes));
//    std::move(callback).Run();
//
// Without RetainedRef, the scoped_refptr would try to implicitly convert to
// a raw pointer and fail compilation:
//
//    OnceClosure callback = BindOnce(&foo, bytes); // ERROR!
template <typename T>
inline internal::RetainedRefWrapper<T> RetainedRef(T* o) {
  return internal::RetainedRefWrapper<T>(o);
}
template <typename T>
inline internal::RetainedRefWrapper<T> RetainedRef(scoped_refptr<T> o) {
  return internal::RetainedRefWrapper<T>(std::move(o));
}
```




## Unwrap
基本上是和wrapper相对应。


```C++
template <typename>
struct BindUnwrapTraits;

template <typename T>
using Unwrapper = BindUnwrapTraits<std::decay_t<T>>;

template <typename T>
decltype(auto) Unwrap(T&& o) {
  return Unwrapper<T>::Unwrap(std::forward<T>(o));
}
```

