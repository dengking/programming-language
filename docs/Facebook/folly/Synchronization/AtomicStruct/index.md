# `AtomicStruct`



## lrita [folly utility 简明摘要](https://lrita.github.io/2021/06/11/cpp-folly-utility/) # `folly::AtomicStruct`

```C++
// AtomicStruct 可以原子地操作一个大小小于等于8byte的对象，其内部原理就是把对象转换成对应大小的int类型，
// 使用std::atmoic<int>来操作。是一个比较有助的封装。对象大小大于8byte的则无法使用该封装。
#include <folly/synchronization/AtomicStruct.h>

struct A {
  int32_t a;
};

folly::AtomicStruct<A> a;
auto xx = a.load();
```

> NOTE: 
>
> 上面的注释，已经将它的用法描述清楚了



## [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[synchronization](https://github.com/facebook/folly/tree/main/folly/synchronization)/[**AtomicStruct.h**](https://github.com/facebook/folly/blob/main/folly/synchronization/AtomicStruct.h)

> NOTE: 
>
> 它的implementation还是比较简单的



```C++
using Raw = _t<detail::AtomicStructRaw<constexpr_log2_ceil(sizeof(T))>>;
```

`_t` 是在 [**Traits.h**](https://github.com/facebook/folly/blob/main/folly/Traits.h) 中定义的:

```C++
template <typename T>
using _t = typename T::type;
```



## Application

1、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[synchronization](https://github.com/facebook/folly/tree/main/folly/synchronization)/[**LifoSem.h**](https://github.com/facebook/folly/blob/main/folly/synchronization/LifoSem.h)

2、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[**IndexedMemPool.h**](https://github.com/facebook/folly/blob/main/folly/IndexedMemPool.h)

3、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[detail](https://github.com/facebook/folly/tree/main/folly/detail)/[**MemoryIdler.h**](https://github.com/facebook/folly/blob/main/folly/detail/MemoryIdler.h)

4、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[**AtomicUnorderedMap.h**](https://github.com/facebook/folly/blob/main/folly/AtomicUnorderedMap.h)

5、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[**ThreadPoolExecutor.h**](https://github.com/facebook/folly/blob/main/folly/executors/ThreadPoolExecutor.h)