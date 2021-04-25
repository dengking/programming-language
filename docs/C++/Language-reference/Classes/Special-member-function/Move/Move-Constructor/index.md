# Move Constructor

已经在C++11中标准化了。

## cppreference [Move constructors](https://en.cppreference.com/w/cpp/language/move_constructor)



### Notes

To make the [strong exception guarantee](https://en.cppreference.com/w/cpp/language/exceptions#Exception_safety) possible, user-defined move constructors should not throw exceptions. For example, [std::vector](https://en.cppreference.com/w/cpp/container/vector) relies on [std::move_if_noexcept](https://en.cppreference.com/w/cpp/utility/move_if_noexcept) to choose between move and copy when the elements need to be relocated.