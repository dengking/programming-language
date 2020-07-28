# Swappable

本文描述的是，在generic programming中，如何**使用**`swap`，关于如何**实现**`swap`，参见`C++\Idiom\OOP\Non-throwing-swap`。

## cppreference [C++ named requirements: *Swappable*](https://en.cppreference.com/w/cpp/named_req/Swappable)

Any lvalue or rvalue of this type can be swapped with any lvalue or rvalue of some other type, using unqualified function call `swap()` in the context where both [std::swap](https://en.cppreference.com/w/cpp/algorithm/swap) and the user-defined `swap()`s are **visible**.

> NOTE: 上面这段话中的**visible**是指compiler的name lookup能够找到这些函数，关于name lookup，参见`C++\Language-reference\Basic-concept\Organization\Name-lookup`。

### Requirements

Type U is swappable with type T if, for any object u of type U and any object t of type T,

> NOTE: 通过上面的描述可以看到，**swappable**所描述的是两个类型之间的性质



Many standard library functions (for example, many algorithms) expect their arguments to satisfy *Swappable*, which means that any time the standard library performs a swap, it uses the equivalent of using [std::swap](http://en.cppreference.com/w/cpp/algorithm/swap); swap(t, u);.

> NOTE: 这是generic programming

Typical implementations either

1) Define a non-member `swap` in the enclosing namespace, which may forward to a member swap if access to non-public data members is required

2) Define a [friend function](https://en.cppreference.com/w/cpp/language/friend) in-class (this approach hides the class-specific swap from name lookup other than ADL)

> NOTE: **swappable**的实现是非常依赖于ADL的，关于ADL，参加`C++\Language-reference\Basic-concept\Organization\Name-lookup\ADL`，需要注意的是，我们应该使用**The Interface Principle**来看待`swap`函数，它是对应的type的interface，关于**The Interface Principle**，参见`C++\Language-reference\Classes\The-interface-principle.md`，在这篇文章中，以**swappable**为例。

### Example

```c++
#include <iostream>
#include <vector>
 
class IntVector {
    std::vector<int> v;
    IntVector& operator=(IntVector) = delete; // not assignable
 public:
    void swap(IntVector& other) {
        v.swap(other.v);
    }
};
void swap(IntVector& v1, IntVector& v2) {
    v1.swap(v2);
}
 
int main()
{
    IntVector v1, v2;
//  std::swap(v1, v2); // compiler error! std::swap requires MoveAssignable
    std::iter_swap(&v1, &v2); // OK: library calls unqualified swap()
}
// g++ --std=c++11 test.cpp 
```



## cppreference [C++ named requirements: *ValueSwappable*](https://en.cppreference.com/w/cpp/named_req/ValueSwappable)





## cpppatterns [Swap values](https://cpppatterns.com/patterns/swap-values.html)



```c++
#include <utility>
#include <string>
int main()
{
  std::string s1 = "Hello";
  std::string s2 = "World";
  using std::swap;
  std::cout << "Before swap:" << std::endl;
  std::cout << s1 << " " << s2 << std::endl;
  swap(s1, s2);
  std::cout << "After swap:" << std::endl;
  std::cout << s1 << " " << s2 << std::endl;
}
```

