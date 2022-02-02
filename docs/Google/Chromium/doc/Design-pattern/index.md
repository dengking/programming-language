# Design pattern

本章总结在chromium中广泛采用的design pattern。

## doc

[chromium](https://chromium.googlesource.com/?format=HTML) / [chromium](https://chromium.googlesource.com/chromium/) / [src](https://chromium.googlesource.com/chromium/src/) / [HEAD](https://chromium.googlesource.com/chromium/src/+/HEAD) / [.](https://chromium.googlesource.com/chromium/src/+/HEAD/) / [docs](https://chromium.googlesource.com/chromium/src/+/HEAD/docs) / **[patterns](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/patterns/)**

### test

1、[The TestApi Pattern](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/patterns/testapi.md)

2、[The Friend-the-tests Pattern](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/patterns/friend-the-tests.md)

3、[The ForTesting Methods Pattern](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/patterns/fortesting-methods.md)

## Resource handle

cross plateform

handle to system resource

如何进行封装？



### Abstract resource handle、plateform resource handle



### RAII、scoped resource handle

#### Example

一、scoped move-only resource handle

参见: 

1、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[writable_shared_memory_region.h](https://github.com/chromium/chromium/blob/master/base/memory/writable_shared_memory_region.h)**

二、doc [Smart Pointer Guidelines](https://www.chromium.org/developers/smart-pointer-guidelines)



## Delegate pattern + virtual class

这是在chromium base中，广泛采用的模式。

要搞清楚所delegate的是什么、将什么的implementation(实现)给delegate出去了；本质上来说delegate所定义的是interface，它需要由其他class来进行实现，然后在host class中，使用delegate的implementation。

### Example

比如`Thread`的`Delegate`所delegate的是它的task queue的implementation。

