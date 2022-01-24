# `std::in_place`

1、是在阅读 [chromium](https://github.com/chromium/chromium/tree/master)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/[**ref_counted.h**](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted.h)  时发现的，它是典型的construct in place。

2、这有些类似于tag dispatch

## cppreference [std::in_place](https://en.cppreference.com/w/cpp/utility/in_place)

`std::in_place`, `std::in_place_type`, and `std::in_place_index` are disambiguation tags that can be passed to the constructors of [std::optional](https://en.cppreference.com/w/cpp/utility/optional), [std::variant](https://en.cppreference.com/w/cpp/utility/variant), and [std::any](https://en.cppreference.com/w/cpp/utility/any) to indicate that the contained object should be constructed in-place, and (for the latter two) the type of the object to be constructed.



## 素材

https://stackoverflow.com/questions/40923097/stdin-place-t-and-friends-in-c17

https://www.cppstories.com/2018/07/in-place-cpp17/

http://coliru.stacked-crooked.com/a/00976d6c9c1b0c5f



