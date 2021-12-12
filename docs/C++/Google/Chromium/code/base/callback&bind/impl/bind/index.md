# bind

一、通过`bind`来创建callback，而不是直接调用对应的constructor。

联系`bind` 和 `callback`的是bind state

## [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[bind.h](https://github.com/chromium/chromium/blob/main/base/bind.h)**

### factory method

#### `BindOnce`



#### `BindRepeating`



### wrapper



#### `Unretained`, `RetainedRef`, `Owned`, `Passed`, `IgnoreResult`

"retain" 的意思是 "保留"；显然 "Unretained" 的意思是 "不保留"

参考内容:

1、docs [Important Abstractions and Data Structures](https://www.chromium.org/developers/coding-style/important-abstractions-and-data-structures)

这篇文章做了非常详细的介绍

2、docs [OnceCallback<> and BindOnce(), RepeatingCallback<> and BindRepeating()](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/callback.md)	

3、geek-share [Chromium中base::Unretained的用法](https://www.geek-share.com/detail/2577300816.html)







## [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[bind_internal.h](https://github.com/chromium/chromium/blob/main/base/bind_internal.h)**

主要描述`BindStateBase`的implementation `BindState` 。

### `struct BindState final : BindStateBase` 

```C++
template <typename Functor, typename... BoundArgs>
struct BindState final : BindStateBase 
```

#### 成员变量

```C++
  Functor functor_;
  std::tuple<BoundArgs...> bound_args_;
```

可以看到，它使用 `std::tuple` 来存储bound args。

### `BindImpl`

