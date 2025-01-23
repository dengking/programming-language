

## Rvalue references (since C++11)

### Rvalue references  VS lvalue references to `const` 

> NOTE: 
>
> 这个标题是我添加的，原文并没有，其实原文的内容主要是从"rvalue references  VS lvalue references to `const` "的角度来展开的，下面的两个也是我添加的，对原文的内容进行了一下概括
>
> 

### [Extend the lifetimes](https://en.cppreference.com/w/cpp/language/reference_initialization#Lifetime_of_a_temporary) of temporary objects 

Rvalue references can be used to [extend the lifetimes](https://en.cppreference.com/w/cpp/language/reference_initialization#Lifetime_of_a_temporary) of temporary objects (note, lvalue references to `const` can extend the lifetimes of temporary objects too, but they are not modifiable through them):

```C++
#include <iostream>
#include <string>
 
int main() {
    std::string s1 = "Test";
//  std::string&& r1 = s1;           // error: can't bind to lvalue
 
    const std::string& r2 = s1 + s1; // okay: lvalue reference to const extends lifetime
//  r2 += "Test";                    // error: can't modify through reference to const
 
    std::string&& r3 = s1 + s1;      // okay: rvalue reference extends lifetime
    r3 += "Test";                    // okay: can modify through reference to non-const
    std::cout << r3 << '\n';
}

// g++ --std=c++11 test.cpp
```

> NOTE: 运行结果如下:
>
> ```
> TestTestTest
> ```



### Move semantic/ Rvalue reference and lvalue reference overloads

> NOTE: 
>
> 这一点，主要是为了实现move semantic

More importantly, when a function has both **rvalue reference** and **lvalue reference** [overloads](https://en.cppreference.com/w/cpp/language/overload_resolution), the rvalue reference overload binds to **rvalues** (including both **prvalues** and **xvalues**), while the lvalue reference overload binds to **lvalues**.

[cppreference std::move#Notes](https://en.cppreference.com/w/cpp/utility/move#Notes):

> The functions that accept rvalue reference parameters (including [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor), [move assignment operators](https://en.cppreference.com/w/cpp/language/move_operator), and regular member functions such as `std::vector::push_back`) are selected, by [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution), when called with [rvalue](https://en.cppreference.com/w/cpp/language/value_category) arguments (either [prvalues](https://en.cppreference.com/w/cpp/language/value_category) such as a temporary object or [xvalues](https://en.cppreference.com/w/cpp/language/value_category) such as the one produced by `std::move`).

```c++
#include <iostream>
#include <utility> // std::move

void f(int& x)
{
	std::cout << "lvalue reference overload f(" << x << ")\n";
}

void f(const int& x)
{
	std::cout << "lvalue reference to const overload f(" << x << ")\n";
}

void f(int&& x)
{
	std::cout << "rvalue reference overload f(" << x << ")\n";
}

int main()
{
	int i = 1;
	const int ci = 2;
	f(i);  // calls f(int&)
	f(ci); // calls f(const int&)
	f(3);  // calls f(int&&)
		   // would call f(const int&) if f(int&&) overload wasn't provided
	f(std::move(i)); // calls f(int&&)

	// rvalue reference variables are lvalues when used in expressions
	int&& x = 1;
	f(x);            // calls f(int& x)
	f(std::move(x)); // calls f(int&& x)
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序输入如下：
>
> ```
> lvalue reference overload f(1)
> lvalue reference to const overload f(2)
> rvalue reference overload f(3)
> rvalue reference overload f(1)
> lvalue reference overload f(1)
> rvalue reference overload f(1)
> ```

This allows [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor), [move assignment](https://en.cppreference.com/w/cpp/language/move_assignment) operators, and other move-aware functions (e.g. [std::vector::push_back()](https://en.cppreference.com/w/cpp/container/vector/push_back)) to be automatically selected when suitable.

#### Move semantic transfer ownership

This makes it possible to move out of an object in scope that is no longer needed:

```C++
std::vector<int> v{1,2,3,4,5};
std::vector<int> v2(std::move(v)); // binds an rvalue reference to v
assert(v.empty());
```



### Rvalue references bind to xvalues

Because rvalue references can bind to **xvalues**, they can refer to **non-temporary objects**:

```c++
#include <iostream>
#include <utility> // std::move

void f(int& x)
{
	std::cout << "lvalue reference overload f(" << x << ")\n";
}

void f(const int& x)
{
	std::cout << "lvalue reference to const overload f(" << x << ")\n";
}

void f(int&& x)
{
	std::cout << "rvalue reference overload f(" << x << ")\n";
}

int main()
{
	int i2 = 42;
	int&& rri = std::move(i2); // binds directly to i2
	f(rri);
}

// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序输出：
>
> ```
> lvalue reference overload f(42)
> ```

[cppreference std::move#Notes](https://en.cppreference.com/w/cpp/utility/move#Notes):

> Names of [rvalue reference](https://en.cppreference.com/w/cpp/language/reference) variables are [lvalues](https://en.cppreference.com/w/cpp/language/value_category) and have to be converted to [xvalues](https://en.cppreference.com/w/cpp/language/value_category) to be bound to the function overloads that accept rvalue reference parameters, which is why [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) and [move assignment operators](https://en.cppreference.com/w/cpp/language/move_operator) typically use `std::move`:

```C++
// Simple move constructor
A(A&& arg) : member(std::move(arg.member)) // the expression "arg.member" is lvalue
{} 
// Simple move assignment operator
A& operator=(A&& other) {
     member = std::move(other.member);
     return *this;
}
```

---

### A named rvalue reference is an lvalue-just like any other variable

> NOTE: 
>
> 这是我补充的

```c++
#include <iostream>
#include <utility> // std::move

void f(int& x)
{
	std::cout << "lvalue reference overload f(" << x << ")\n";
}

void f(const int& x)
{
	std::cout << "lvalue reference to const overload f(" << x << ")\n";
}

void f(int&& x)
{
	std::cout << "rvalue reference overload f(" << x << ")\n";
}

int main()
{
	// rvalue reference variables are lvalues when used in expressions
	int&& x = 1;
	f(x);            // calls f(int& x)
	f(std::move(x)); // calls f(int&& x)
}
// g++ --std=c++11 test.cpp
```

> NOTE:
>
> 1、上述例子告诉我们：
>
> "rvalue reference variables are **lvalues** when used in expressions"
>
> 也就是说：rvalue reference variables is lvalue；所以，对于入参类型为rvalue reference的function的argument，它的argument就是典型的rvalue reference variables，也就是说function body中，argument为lvalue。比如在下面例子中，`x`是lvalue：
>
> ```C++
> void f(int&& x)
> {
> 	std::cout << "rvalue reference overload f(" << x << ")\n";
> }
> ```
>
> 
>
> 我们需要更加深入地思考：为什么"rvalue reference variables are **lvalues** when used in expressions"？回答这个问题，需要我们理解rvalue reference的本质：本质上来说，rvalue reference是reference，是alias，所以我们可以认为rvalue reference就是一个alias to temporary object，显然它表示的就是temporary object，我们可以将它看做是temporary object，所以rvalue reference object是一个rvlaue。



## Forwarding references (since C++11)

> NOTE: 
>
> forwarding reference是function generic programming的基础:
>
> `auto&&`解决的是函数的返回值
>
> `template &&`解决的函数的参数
>
> 因此，原文的这部分内容放到了 `C++\CppReference\Functions\GP` 章节。
>
> 





## Dangling references

> NOTE: 关于 [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) ，参见`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration`章节。

