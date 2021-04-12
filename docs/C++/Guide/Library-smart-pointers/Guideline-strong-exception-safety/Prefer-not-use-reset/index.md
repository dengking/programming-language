# Prefer not use reset

其实本质上是CppCoreGuidelines [R.13: Perform at most one explicit resource allocation in a single expression statement](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-single-alloc)。

## stackoverflow [is it better to use shared_ptr.reset or operator =?](https://stackoverflow.com/questions/14836691/is-it-better-to-use-shared-ptr-reset-or-operator)

> NOTE: 
>
> 1、非常好的内容，它将本文的topic说得非常透彻；
>
> 2、应该从本质上来对问题进行分析:  
>
> 本质原因是: 直接使用  `new T()` 所带来的一系列问题。

I'm trying to wrap my head around the new idioms for C++11.

It seems that with `shared_ptr` at least, there is a substantive(本质的) difference between using `new T()` and `make_shared<T>()`.

But what of resetting a shared pointer to point to a new instance of something. Previously, I would typically use `reset(new T())` member. However, doesn't this suffer from the same problem as not using `make_shared()` in the first place? (i.e. it doesn't allow `make_shared` to allocate the object, therefore it is forced to place the ref count in a separate allocation instead of in the same allocation as the `T` itself?)

> NOTE: `reset`是否会出现和`new T()`相同的问题？

Is it simply better going forward to use:

```cpp
   mysharedptr = make_shared<T>(args...);
```

Or is there a better way?

And shouldn't `reset` offer variadic forwarding of arguments as `make_shared` does, so that one could write `mysharedptr.reset(args...);`?

### [A](https://stackoverflow.com/a/14837300)

There is indeed a substantial(本质的) difference between:

```cpp
shared_ptr<T> sp(new T());
```

And:

```cpp
shared_ptr<T> sp = make_shared<T>();
```

The first version performs an allocation for the `T` object, then performs a separate allocation to create the reference counter. The second version performs one single allocation for both the object and the reference counter, placing them in a contiguous region of memory, resulting in less memory overhead.

> NOTE: 如何实现呢?

Also, some implementations are able to perform further space optimizations in the case of `make_shared<>` (see the "We Know Where You Live" optimization done by MS's implementation).

However, that is not the only reason why `make_shared<>` exists. The version based on explicit `new T()` is not exception-safe in some situations, especially when invoking a function that accepts a `shared_ptr`.

```cpp
void f(shared_ptr<T> sp1, shared_ptr<T> sp2);

...

f(shared_ptr<T>(new T()), shared_ptr<T>(new T()))
```

Here, the compiler could evaluate the first `new T()` expression, then evaluate the second `new T()` expression, then construct the corresponding `shared_ptr<>` objects. But what if the second allocation causes an exception before the first allocated object is bound to its `shared_ptr<>`? It will be leaked. With `make_shared<>()`, this is not possible:

```cpp
f(make_shared<T>(), make_shared<T>())
```

Because allocated objects are bound to the respective `shared_ptr<>` objects inside each function call to `make_shared<>()`, this call is exception-safe. This is yet another reason why naked `new` should never be used unless you really know what you are doing. (*)

Considering your remark(评论) about `reset()`, you are right in observing that `reset(new T())` will perform separate allocations for the counter and the object, just as the construction of a new `shared_ptr<>` will perform a separate allocation when a raw pointer is passed as an argument. Therefore, an assignment using `make_shared<>` is preferable (or even a statement such as `reset(make_shared<T>())`).

Whether or not `reset()` should support a variadic argument list, this is probably more of a kind of open discussion for which StackOverflow is not a good fit.

`(*)` There are a few situations that still require it. For instance, the fact that the C++ Standard Library lacks a corresponding `make_unique<>` function for `unique_ptr`, so you'll have to write one yourself. Another situation is when you do not want the object and the counter to be allocated on a single memory block, because the presence of weak pointers to the object will prevent the *entire* block from being deallocated, even though no more owning pointers to the object exist.



### [A](https://stackoverflow.com/a/14837398)

Correct, `reset(new T...)` suffers all the issues of `shared_ptr(new T...)`; it will result in double allocation and also is non-exception-safe (there's not much chance of a leak, unless `bad_alloc` happens within `reset`).

`reset` is documented as equivalent to `shared_ptr<T>(ptr).swap(*this)`, so you could also write:

```cpp
make_shared<T>(args...).swap(mysharedptr);
```

Assignment from `make_shared<T>` is almost equivalent, the only difference being the relative order of the deletion of the old `T` and the destruction of the temporary `shared_ptr`, which is not observable.



## stackoverflow [std::shared_ptr: reset() vs. assignment](https://stackoverflow.com/questions/31438714/stdshared-ptr-reset-vs-assignment)

