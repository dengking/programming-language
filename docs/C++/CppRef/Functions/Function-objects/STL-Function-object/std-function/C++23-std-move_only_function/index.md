# C++23 `std::move_only_function`

一、它是为了解决`std::function` copyable、non-movable而实现的，关于此，可以参见:

1、sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

2、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[docs](https://github.com/facebook/folly/tree/main/folly/docs)/[**Function.md**](https://github.com/facebook/folly/blob/main/folly/docs/Function.md)

## cppreference [std::move_only_function](https://en.cppreference.com/w/cpp/utility/functional/move_only_function)