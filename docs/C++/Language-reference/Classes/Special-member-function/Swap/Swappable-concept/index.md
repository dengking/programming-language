# Swappable

本文描述的是，在generic programming中，如何**使用**`swap`，关于如何**实现**`swap`，参见`C++\Idiom\OOP\Non-throwing-swap`。

## cppreference [C++ named requirements: *Swappable*](https://en.cppreference.com/w/cpp/named_req/Swappable)

Any lvalue or rvalue of this type can be swapped with any lvalue or rvalue of some other type, using unqualified function call `swap()` in the context where both [std::swap](https://en.cppreference.com/w/cpp/algorithm/swap) and the user-defined `swap()`s are **visible**.

> NOTE: 
>
> 1、上面这段话中的**visible**是指compiler的name lookup能够找到这些函数，关于name lookup，参见`C++\Language-reference\Basic-concept\Organization\Name-lookup`。
>
> 2、ADL

### Requirements

Type U is swappable with type T if, for any object u of type U and any object t of type T,

> NOTE: 通过上面的描述可以看到，**swappable**所描述的是两个类型之间的性质



Many standard library functions (for example, many algorithms) expect their arguments to satisfy *Swappable*, which means that any time the standard library performs a swap, it uses the equivalent of using [std::swap](http://en.cppreference.com/w/cpp/algorithm/swap); swap(t, u);.

> NOTE: 这是generic programming

### Implementation

Typical implementations either

1) Define a non-member `swap` in the enclosing namespace, which may forward to a member swap if access to non-public data members is required

2) Define a [friend function](https://en.cppreference.com/w/cpp/language/friend) in-class (this approach hides the class-specific swap from name lookup other than ADL)

> NOTE:
>
> 1、 **swappable**的实现是非常依赖于ADL的，关于ADL，参加`C++\Language-reference\Basic-concept\Organization\Name-lookup\ADL`
>
> 2、需要注意的是，我们应该使用**The Interface Principle**来看待`swap`函数，它是对应的type的interface，关于**The Interface Principle**，参见`C++\Language-reference\Classes\The-interface-principle.md`，在这篇文章中，以**swappable**为例。

### Example1: 较复杂的例子

> NOTE: 
>
> 1、其实下面这个例子是不够直接、明了的，它其实暗含了一些机关
>
> a、`class IntVector` 是 non assignable的，这是因为
>
> ```C++
> IntVector& operator=(IntVector) = delete; // not assignable
> ```
>
> 这就意味着，它是:
>
> non copy assignable、non move assignable
>
> 这就造成 `std::swap(v1, v2); // compiler error! std::swap requires MoveAssignable`
>
> 2、[`std::iter_swap`](https://en.cppreference.com/w/cpp/algorithm/iter_swap) 的 implementation:
>
> ```C++
> template<class ForwardIt1, class ForwardIt2>
> constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b) // constexpr since C++20
> {
>    using std::swap;
>    swap(*a, *b);
> }
> ```
>
> 因此，compiler就会选择use define swap: `void swap(IntVector &v1, IntVector &v2)`
>
> 3、由于 [`std::iter_swap`](https://en.cppreference.com/w/cpp/algorithm/iter_swap) 使用了 ADL，因此下面的这个写法其实和"Example 2"中的写法类似的
>
> 

```c++
#include <iostream>
#include <vector>

class IntVector
{
	std::vector<int> v;
	IntVector& operator=(IntVector) = delete; // not assignable
public:
	void swap(IntVector &other)
	{
		v.swap(other.v);
	}
};
void swap(IntVector &v1, IntVector &v2)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	v1.swap(v2);
}

int main()
{
	IntVector v1, v2;
//  std::swap(v1, v2); // compiler error! std::swap requires MoveAssignable
	std::iter_swap(&v1, &v2); // OK: library calls unqualified swap()
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```

> NOTE: 
>
> 1、输出如下:
>
> ```C++
> void swap(IntVector&, IntVector&)
> ```
>
> 通过上述例子可知，通过ADL找到了user-defined swap
>
> 2、"`std::swap` requires MoveAssignable "是从C++17开始的

### Example2: 最最典型的例子

```C++
#include <iostream>
#include <vector>

class IntVector
{
	std::vector<int> v;
	IntVector& operator=(IntVector) = delete; // not assignable
public:
	void swap(IntVector &other)
	{
		v.swap(other.v);
	}
};
void swap(IntVector &v1, IntVector &v2)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	v1.swap(v2);
}

int main()
{
	IntVector v1, v2;
	using std::swap;
	swap(v1, v2);
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```

1、输出如下:

```C++
void swap(IntVector&, IntVector&)
```

2、从输出可以看出，compiler选择了use define swap: `void swap(IntVector &v1, IntVector &v2)`

### Example3: 反例

```C++
#include <iostream>
#include <vector>
#include <utility>

class IntVector
{
	std::vector<int> v;
public:
	void swap(IntVector &other)
	{
		v.swap(other.v);
	}
};
void swap(IntVector &v1, IntVector &v2)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	v1.swap(v2);
}

int main()
{
	IntVector v1, v2;
	std::swap(v1, v2);
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```

1、上述程序的输出为空，这说明compiler没有选择use define swap: `void swap(IntVector &v1, IntVector &v2)`，而是使用的`std::swap`

2、上述例子是一个反例

## cppreference [C++ named requirements: *ValueSwappable*](https://en.cppreference.com/w/cpp/named_req/ValueSwappable)





## cpppatterns [Swap values](https://cpppatterns.com/patterns/swap-values.html)



```c++
#include <utility>
#include <string>
#include <iostream>

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
// g++ --std=c++11 test.cpp -Wall -pedantic

```

On [line 9](https://cpppatterns.com/patterns/swap-values.html#line9), we use a [using-declaration](http://en.cppreference.com/w/cpp/language/using_declaration) to make `std::swap` visible and then, on [line 10](https://cpppatterns.com/patterns/swap-values.html#line10), we call unqualified `swap` (not `std::swap`) to swap the values of the two objects. This allows a user-defined specialization of `swap` to be found via [argument-dependent lookup (ADL)](http://en.cppreference.com/w/cpp/language/adl), which may provide a more efficient implementation of the swap operation, before falling back to the generic `std::swap` function. This approach is particularly useful when swapping two generic objects (such as in a template).

> NOTE:
>
> 1、"falling back"这个此用得非常好