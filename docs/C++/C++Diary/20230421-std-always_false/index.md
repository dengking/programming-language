# std::always_false





### stackoverflow [Is it possible to implement always_false in the C++ standard library?](https://stackoverflow.com/questions/57787666/is-it-possible-to-implement-always-false-in-the-c-standard-library) 

There are cases where one uses an `always_false` helper to e.g. cause unconditional `static_assert` failure if instantiation of some template is attempted:

```c++
#include <string>
#include <cstddef>
#include <concepts>


template<class... T>
struct always_false : std::false_type {
};

template<class T>
struct UsingThisShouldBeAnError {
    static_assert(always_false<T>::value, "You should not use this!");
};

int main() {
    UsingThisShouldBeAnError<int>();
}
```

