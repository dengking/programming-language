# Non-static member functions

## Member method is a kind of function

在阅读下面文章时:

- accu [**Overload Resolution - Selecting the Function**](https://accu.org/journals/overload/13/66/kilpelainen_268/)
- cppreference [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)

其中都有关于member method、implicit object parameter、implied object argument的讨论，并且在accu [**Overload Resolution - Selecting the Function**](https://accu.org/journals/overload/13/66/kilpelainen_268/)中说明了这样做的原因:

> This is an easy way to make the overloading rules **uniform** for the member functions and free functions.

显然，从实现层面来看，Member method is a kind of function，这样我们可以使用一个统一的模型来描述free function、member method。这是符合在 `C++\Language-reference\Basic-concept\index.md` 中提出的“Uniform function model”的。

## cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions)



### Explanation

Within the body of a non-static member function of `X`, any [id-expression](https://en.cppreference.com/w/cpp/language/expressions) `E` (e.g. an identifier) that resolves to a non-type non-static member of `X` or of a base class of `X`, is transformed to a member access expression `(*this).E` (unless it's already a part of a member access expression). This does not occur in template definition context, so a name may have to be prefixed with `this->` explicitly to become [dependent](https://en.cppreference.com/w/cpp/language/dependent_name).

> NOTE: 关于上面这段话中的“non-type non-static member of `X` or of a base class of `X`”的理解，其实它就是表示普通的data member、member method。
>
> 关于上面这段话中的id-expression，参见`C++\Language-reference\Expressions`，



### const- and volatile-qualified member functions

Differently cv-qualified functions have different types and so may overload each other.

> NOTE: CV修饰的是不同的type，关于overload，参见`C++\Language-reference\Functions\Function-overload`章节

In the body of a cv-qualified function, `*this` is cv-qualified, e.g. in a **const member function**, only other **const member functions** may be called normally. (A non-const member function may still be called if [`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) is applied or through an access path that does not involve [`this`](https://en.cppreference.com/w/cpp/language/this).)

> NOTE: 这样的涉及保证了C++的type safety.
>
> 在cppreference [const_cast conversion](https://en.cppreference.com/w/cpp/language/const_cast)中给出了例子。

```c++
#include <iostream>
#include <vector>

struct Array {
    std::vector<int> data;
    Array(int sz) : data(sz) {}
    // const member function
    int operator[](int idx) const {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      // this has type const Array*
      return data[idx];  // transformed to (*this).data[idx];
    }
    // non-const member function
    int& operator[](int idx) {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      // this has type Array*
      return data[idx];  // transformed to (*this).data[idx]
    }
};

int main() {
  Array a(10);
  /**
   * OK: the type of a[1] is int&
   * 选择非const版本
   */
  a[1] = 1;
  const Array ca(10);
  // ca[1] = 2; // Error: the type of ca[1] is int
  /**
   * 选择const版本
   */
  ca[1];
}
// g++ --std=c++11 test.cpp
/**
上述程序的输出如下:
int& Array::operator[](int)
int Array::operator[](int) const
**/
```



> NOTE: 上述对CV-qualified member function的讨论中，仅仅涉及到了member function，但是并没有涉及到**data member**。在**const-qualified member function**，由于this是const-qualified，所以所有的data member都是const-qualified的（在member function中，所有的member access都会被转换为`this->`），因此在const-qualified member function中compiler会进行CV的检查，一旦违背了CV，则会报编译错误，下面是一些例子:
>
> 1) Example1: 来源: cppreference [this#Example](https://en.cppreference.com/w/cpp/language/this#Example)
>
> ```C++
> class T {
>     int x;
>     int y;
>     void foo() {
>       x = 6;        // same as this->x = 6;
>       this->x = 5;  // explicit use of this->
>     }
> 
>     void foo() const {
>       x = 7;  // Error: *this is constant
>     }
> 
>   public:
>     T(int x)
>         : x(x),       // uses parameter x to initialize member x
>           y(this->x)  // uses member x to initialize member y
>     {}
> };
> 
> int main() { T t(3); }
> // g++ test.cpp
> 
> ```
> 
> 上述程序编译报错如下:
> 
> ```C++
> test.cpp: 在成员函数‘void T::foo() const’中:
> test.cpp:13:5: 错误：assignment of member ‘T::x’ in read-only object
>   x = 7; // Error: *this is constant
> ```
>
> 1) Example2: 来源: 我自创的
> 
> ```C++
> #include <vector>
> struct X
>{
> };
>
> class T
> {
> 	std::vector<X> m;
> 	void foo()
> 	{
> 		X &x = m[0]; // 非const，所有使用普通reference即可
> 	}
> 
> 	void foo() const
> 	{
> 		X &x = m[0]; // const，需要使用const reference
> 	}
> 
> public:
> 	T()
> 	{
> 	}
> };
> 
> int main()
> {
> 	T t;
> }
> // g++ test.cpp
> 
> ```
> 
> 上述程序编译报错如下:
> 
> ```C++
> test.cpp: 在成员函数‘void T::foo() const’中:
> test.cpp:16:13: 错误：将类型为‘X&’的引用初始化为类型为‘const X’的表达式无效
>   X &x = m[0]; // const，需要使用const reference
> ```
>
> 修正版本如下:
> 
> ```C++
> #include <vector>
> struct X
>{
> };
>
> class T
> {
> 	std::vector<X> m;
> 	void foo()
> 	{
> 		X &x = m[0]; // 非const，所有使用普通reference即可
> 	}
> 
> 	void foo() const
> 	{
> 		const X &x = m[0]; // const，需要使用const reference
> 	}
> 
> public:
> 	T()
> 	{
> 	}
> };
> 
> int main()
> {
> 	T t;
> }
> 
> ```
> 
> 
> 
> 





### ref-qualified member functions(since C++11)

| ref-qualifier        | syntax        | [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution) |
| -------------------- | ------------- | ------------------------------------------------------------ |
| no ref-qualifier     |               |                                                              |
| lvalue ref-qualifier | `void f() &`  | the implicit object parameter has type **lvalue reference** to cv-qualified `X` |
| rvalue ref-qualifier | `void f() &&` | the implicit object parameter has type **rvalue reference** to cv-qualified `X` |



```c++
#include <iostream>
struct S {
    void f() & { std::cout << "lvalue\n"; }
    void f() && { std::cout << "rvalue\n"; }
};

int main() {
  S s;
  s.f();             // prints "lvalue"
  std::move(s).f();  // prints "rvalue"
  S().f();           // prints "rvalue"
}
// g++ --std=c++11 test.cpp

```

#### gpt-4o-mini Overloading Based on Value Category

Overloading based on value category in C++ is a powerful feature that allows you to define different behaviors for member functions depending on whether the object is an **lvalue** or an **rvalue**. This is particularly useful in the context of **move semantics** and **resource management**, where you may want to handle **temporary objects** (rvalues) differently from **named objects** (lvalues).

##### Overloading Member Functions

You can overload member functions based on whether they are called on lvalues or rvalues by using the following syntax:

- `int func() &` for lvalue references
- `int func() &&` for rvalue references

##### Example of Overloading Based on Value Category

Here’s an example that demonstrates how to overload member functions based on value category:

```cpp
#include <iostream>
#include <string>

class StringWrapper {
public:
    StringWrapper(const std::string& str) : data(str) {
        std::cout << "Lvalue constructor called: " << data << std::endl;
    }

    StringWrapper(std::string&& str) : data(std::move(str)) {
        std::cout << "Rvalue constructor called: " << data << std::endl;
    }

    // Overloaded member function for lvalues
    void print() & {
        std::cout << "Lvalue print: " << data << std::endl;
    }

    // Overloaded member function for rvalues
    void print() && {
        std::cout << "Rvalue print: " << data << std::endl;
    }

private:
    std::string data;
};

int main() {
    StringWrapper sw1("Hello"); // Calls lvalue constructor
    sw1.print(); // Calls lvalue version of print()

    StringWrapper sw2(std::string("World")); // Calls rvalue constructor
    sw2.print(); // Calls lvalue version of print()

    // Calling print on a temporary object (rvalue)
    StringWrapper("Temporary").print(); // Calls rvalue version of print()

    return 0;
}
/**
输出如下:
Lvalue constructor called: Hello
Lvalue print: Hello
Rvalue constructor called: World
Lvalue print: World
Rvalue print: Temporary
**/
```



##### Benefits of Overloading Based on Value Category

- **Performance Optimization**: You can optimize the behavior of your class for **temporary objects (rvalues)** and **named objects (lvalues)**, which can lead to better performance, especially in resource management scenarios.
  
- **Clearer Semantics**: By providing different implementations for lvalues and rvalues, you can make the intent of your code clearer and avoid unintended side effects.

- **Flexibility**: This approach allows you to define how your class behaves in different contexts, making it more versatile.



### qualified方式总结

| qualified方式                 | 对`this`pointer的影响                                        | 对overload resolution的影响                    |
| ----------------------------- | ------------------------------------------------------------ | ---------------------------------------------- |
| const- and volatile-qualified | In the body of a cv-qualified function, `*this` is cv-qualified | CV-qualified member function是不同的function   |
| ref-qualified                 | ref-qualification does not change the properties of the [this](https://en.cppreference.com/w/cpp/language/this) pointer: within a rvalue ref-qualified function, `*this` remains an lvalue expression. | ref-qualified member functions是不同的function |



### Special member functions

> NOTE: 这在`C++\Language-reference\Classes\Special-member-functions`章节进行介绍

