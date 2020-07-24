# Reference 

c++的reference是alias。

- `&`是左值的alias

- `&&`是右值的alias

它可以：

- alias to **already-existing** object
- alias to **already-existing** function

## cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference)

### Syntax

**Reference types** cannot be [cv-qualified](https://en.cppreference.com/w/cpp/language/cv) at the top level; there is no syntax for that in declaration, and if a qualification is added to a typedef-name or [`decltype`](https://en.cppreference.com/w/cpp/language/decltype) specifier, or type template parameter, it is ignored.

> NOTE: 理解上面这段话的关键是理解"**Reference types**"，**reference type**不是reference，下面是**reference type**的一个例子：
>
> ```c++
> typedef int&  lref;
> typedef int&& rref;
> ```
>
> `lref`、`rref`就是**reference type**。



References are not **objects**; they do not necessarily occupy storage, although the compiler may allocate storage if it is necessary to implement the desired semantics (e.g. a **non-static data member** of reference type usually increases the size of the class by the amount necessary to store a memory address).

> NOTE: 不同的compiler有不同的implementation

Because references are not **objects**, there are no **arrays** of references, no **pointers** to references, and no **references** to references:

### Reference collapsing

> NOTE: 支持reference collapsing的目的是什么？



### Lvalue references



#### Reference to `const`

```C++
#include <iostream>
#include <string>
 
int main() {
    std::string s = "Ex";
    std::string& r1 = s;
    const std::string& r2 = s;
 
    r1 += "ample";           // modifies s
//  r2 += "!";               // error: cannot modify through reference to const
    std::cout << r2 << '\n'; // prints s, which now holds "Example"
}
```

#### Pass-by-reference semantics



```C++
#include <iostream>
#include <string>
 
void double_string(std::string& s) {
    s += s; // 's' is the same object as main()'s 'str'
}
 
int main() {
    std::string str = "Test";
    double_string(str);
    std::cout << str << '\n';
}
```



### Rvalue references (since C++11)



#### Rvalue references  VS lvalue references to `const` 

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





#### Rvalue reference variables



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

上述例子告诉我们：

"rvalue reference variables are **lvalues** when used in expressions"

也就是说：rvalue reference variables is lvalue；所以，对于入参类型为rvalue reference的function的argument，它的argument就是典型的rvalue reference variables，也就是说function body中，argument为lvalue。比如在下面例子中，`x`是lvalue：

```c++
void f(int&& x)
{
	std::cout << "rvalue reference overload f(" << x << ")\n";
}
```

我们需要更加深入地思考：为什么"rvalue reference variables are **lvalues** when used in expressions"？回答这个问题，需要我们理解rvalue reference的本质：本质上来说，rvalue reference是reference，是alias，所以我们可以认为rvalue reference就是一个alias to temporary object，显然它表示的就是temporary object，我们可以将它看做是temporary object，所以rvalue reference object是一个rvlaue。

#### Rvalue references bind to xvalues

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



#### Rvalue reference and lvalue reference overloads

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



### Forwarding references (since C++11)

**Forwarding references** are a special kind of references that preserve the value category of a function argument, making it possible to *forward* it by means of [std::forward](https://en.cppreference.com/w/cpp/utility/forward). Forwarding references are either:

#### 1)

1) function parameter of a function template declared as **rvalue reference** to **cv-unqualified** [type template parameter](https://en.cppreference.com/w/cpp/language/template_parameters) of that same function template:

```c++
#include <utility> // std::forward
#include <iostream>

template<class T>
int g(const T&& x) // x is not a forwarding reference: const T is not cv-unqualified
{
	std::cout << __PRETTY_FUNCTION__ << " " << " " << x << std::endl;
}

template<class T>
int g(const T& x)
{
	std::cout << __PRETTY_FUNCTION__ << " " << " " << x << std::endl;
}

template<class T>
int f(T&& x)
{                    // x is a forwarding reference
	return g(std::forward<T>(x)); // and so can be forwarded
}

int main()
{
	int i = 100;
	f(i); // argument is lvalue, calls f<int&>(int&), std::forward<int&>(x) is lvalue
	f(0); // argument is rvalue, calls f<int>(int&&), std::forward<int>(x) is rvalue
}



// g++ --std=c++11 test.cpp
```

> NOTE: 输出如下:
>
> ```c++
> int g(const T&) [with T = int]  100
> int g(const T&&) [with T = int]  0
> ```



```c++
template<class T> struct A {
    template<class U>
    A(T&& x, U&& y, int* p); // x is not a forwarding reference: T is not a
                             // type template parameter of the constructor,
                             // but y is a forwarding reference
};
```

#### 2)

2) `auto&&` except when deduced from a brace-enclosed initializer list:

```c++
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <vector> // std::vector

std::vector<int> foo()
{
	return
	{	1,2,3,4};
}
std::vector<int> f()
{
	return
	{	1,2,3,4};
}
template<class T>
int g(T&& x)
{                    // x is a forwarding reference
	return 0;
}

int main()
{
	auto&& vec = foo();       // foo() may be lvalue or rvalue, vec is a forwarding reference
	auto i = std::begin(vec); // works either way
	(*i)++;                   // works either way
	g(std::forward<decltype(vec)>(vec)); // forwards, preserving value category

	for (auto&& x : f())
	{
		// x is a forwarding reference; this is the safest way to use range for loops
	}

	auto&& z = { 1, 2, 3 }; // *not* a forwarding reference (special case for initializer lists)
}

// g++ --std=c++11 test.cpp
```

See also [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Deduction_from_a_function_call) and [std::forward](https://en.cppreference.com/w/cpp/utility/forward).



### Rvalue reference VS forwarding reference

[cppreference std::move#Notes](https://en.cppreference.com/w/cpp/utility/move#Notes):

> One exception is when the type of the function parameter is rvalue reference to type template parameter ("forwarding reference" or "universal reference"), in which case **std::forward** is used instead.

上面这段话给我的感觉是：forwarding reference是一种特殊的rvalue reference。

| reference类型        |                                                              |
| -------------------- | ------------------------------------------------------------ |
| rvalue reference     | [std::move](https://en.cppreference.com/w/cpp/utility/move)  |
| forwarding reference | [std::forward](https://en.cppreference.com/w/cpp/utility/forward) |



### Dangling references









## isocpp [References](https://isocpp.org/wiki/faq/references) 



## Thoughts on reference


如果一个函数的说明如下:
```c++
Sub(int ReSubTimes, std::chrono::seconds& ReSubInterval)
```

它的第二个入参的类型是左值引用

下面两种调用方式

```c++
#include<chrono>
void Sub(int ReSubTimes, std::chrono::seconds& ReSubInterval)
{


}

int main(){
std::chrono::seconds t(3);
Sub(3,t);
Sub(3,std::chrono::seconds(3));

}
```
上述`Sub(3,std::chrono::seconds(3));`会导致如下编译报错
```c++
test.cpp: 在函数‘int main()’中:
test.cpp:10:30: 错误：用类型为‘std::chrono::seconds {aka std::chrono::duration<long int>}’的右值初始化类型为‘std::chrono::seconds& {aka std::chrono::duration<long int>&}’的非常量引用无效
 Sub(3,std::chrono::seconds(3)); 
                              ^
test.cpp:2:6: 错误：在传递‘void Sub(int, std::chrono::seconds&)’的第 2 个实参时
 void Sub(int ReSubTimes, std::chrono::seconds& ReSubInterval){

```
这是因为`Sub(3,std::chrono::seconds(3));`的第二个入参是右值，无法通过左值引用进行传参。