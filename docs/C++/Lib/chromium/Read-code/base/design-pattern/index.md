# Design pattern

本章总结在chromium中广泛采用的design pattern。



## Resource handle

cross plateform

handle to system resource

如何进行封装？



### Abstract resource handle、plateform resource handle



### RAII、scoped resource handle



#### scoped move-only resource handle

参见: 

1、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[writable_shared_memory_region.h](https://github.com/chromium/chromium/blob/master/base/memory/writable_shared_memory_region.h)**





## observer pattern



## Cross plateform

如何根据不同的plateform从source code中选择对应plateform的source code进行编译。很显然，是不能够将所有的plateform的implementation source code全部都compile到target file中的。