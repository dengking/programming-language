# Swap、reset and release

一、对ownership的操作:

1、swap、release

二、

## cppreference [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)

| Modifiers                                                    |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [release](https://en.cppreference.com/w/cpp/memory/unique_ptr/release) | returns a pointer to the managed object and releases the ownership (public member function) |
| [reset](https://en.cppreference.com/w/cpp/memory/unique_ptr/reset) | replaces the managed object (public member function)         |
| [swap](https://en.cppreference.com/w/cpp/memory/unique_ptr/swap) | swaps the managed objects (public member function)           |



## cppreference [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)

| Modifiers                                                    |                                                      |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [reset](https://en.cppreference.com/w/cpp/memory/shared_ptr/reset) | replaces the managed object (public member function) |
| [swap](https://en.cppreference.com/w/cpp/memory/shared_ptr/swap) | swaps the managed objects (public member function)   |

### [std::shared_ptr<T>::reset](https://en.cppreference.com/w/cpp/memory/shared_ptr/reset) 通过 `void reset() noexcept;` 来 release ownership

stackoverflow [Does using .reset() on a std::shared_ptr delete all instances](https://stackoverflow.com/questions/21589595/does-using-reset-on-a-stdshared-ptr-delete-all-instances) # [A](https://stackoverflow.com/a/21589725)

When you use `.reset()`, you are eliminating *one* owner of the pointer, but all of the other owners are still around. Here is an example:

```cpp
#include <memory>
#include <cstdio>

class Test { public: ~Test() { std::puts("Test destroyed."); } };

int main()
{
    std::shared_ptr<Test> p = std::make_shared<Test>();
    std::shared_ptr<Test> q = p;
    std::puts("p.reset()...");
    p.reset();
    std::puts("q.reset()...");
    q.reset();
    std::puts("done");
    return 0;
}
```

The program output:

```
p.reset()...
q.reset()...
Test destroyed.
done
```

Note that `p` and `q` are both owners of the object, and once both `p` and `q` are reset, **then** the instance is destroyed.