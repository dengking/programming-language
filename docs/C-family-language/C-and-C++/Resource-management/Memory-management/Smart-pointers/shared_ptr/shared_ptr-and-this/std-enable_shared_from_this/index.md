# `std::enable_shared_from_this`

在阅读 [gabime](https://github.com/gabime)/**[spdlog](https://github.com/gabime/spdlog)** 的 [async_logger.h](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/async_logger.h) 时，其中使用了 `std::enable_shared_from_this`:

```C++
class SPDLOG_API async_logger final : public std::enable_shared_from_this<async_logger>, public logger
{

};

```

## CRTP

`std::enable_shared_from_this`是典型的使用CRTP的，它是mixin。

## cppreference [std::enable_shared_from_this](https://en.cppreference.com/w/cpp/memory/enable_shared_from_this)



### Example

```c++
#include <memory>
#include <iostream>

struct Good: std::enable_shared_from_this<Good> // note: public inheritance
{
    std::shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};

```

