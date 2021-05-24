# Function-try-block

是在阅读 cppreference [Scope](https://en.cppreference.com/w/cpp/language/scope) 的时候，发现的Function-try-block。

## cppreference [Function-try-block](https://en.cppreference.com/w/cpp/language/function-try-block)



```C++
#include <iostream>
#include <string>
struct S {
    std::string m;
    S(const std::string& str, int idx) try : m(str, idx) {
        std::cout << "S(" << str << ", " << idx << ") constructed, m = " << m << '\n';
    } catch(const std::exception& e) {
        std::cout << "S(" << str << ", " << idx << ") failed: " << e.what() << '\n';
    } // implicit "throw;" here
};
int main() {
    S s1{"ABC", 1}; // does not throw (index is in bounds)
    try {
        S s2{"ABC", 4}; // throws (out of bounds)
    } catch (std::exception& e) {
        std::cout << "S s2... raised an exception: " << e.what() << '\n';
    }
}
```

