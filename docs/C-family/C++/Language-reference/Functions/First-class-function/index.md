# 关于本章

Function as first class citizen是一个非常有用的特性，但是C++并不直接支持这个特性，本章介绍C++中如何实现类似于function as first class citizen效果。

## 两种实现方式

在 stackoverflow [What is ScopeGuard in C++?](https://stackoverflow.com/questions/31365013/what-is-scopeguard-in-c) # [A](https://stackoverflow.com/a/31365171) 中，提出了两种实现思路:

> The rôle of the `function` here is to avoid templating so that `Scope_guard` instances can be declared as such, and passed around. An alternative, slightly more complex and with slightly constrained usage, but possibly marginally more efficient, is to have a class templated on a functor type, and use C++11 `auto` for declarations, with the scope guard instance created by a factory function. 

下面是总结: 

|                                | 优势                    | 劣势 |
| ------------------------------ | ----------------------- | ---- |
| `std::function`                | 1、can be passed around |      |
| function as template parameter |                         |      |

### Function as template parameter

参见 `../Generic-programming/Function-as-template-parameter` 章节。



### `std::function`

Example:

1、[spdlog](https://github.com/gabime/spdlog)/[include](https://github.com/gabime/spdlog/tree/v1.x/include)/[spdlog](https://github.com/gabime/spdlog/tree/v1.x/include/spdlog)/[details](https://github.com/gabime/spdlog/tree/v1.x/include/spdlog/details)/[**thread_pool.h**](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/details/thread_pool.h)

2、[spdlog](https://github.com/gabime/spdlog)/[include](https://github.com/gabime/spdlog/tree/v1.x/include)/[spdlog](https://github.com/gabime/spdlog/tree/v1.x/include/spdlog)/[details](https://github.com/gabime/spdlog/tree/v1.x/include/spdlog/details)/[**periodic_worker.h**](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/details/periodic_worker.h)



## TO RAED

### [cwrap](https://github.com/corelight/cwrap)

Auto wrap C and C++ functions with instrumentation