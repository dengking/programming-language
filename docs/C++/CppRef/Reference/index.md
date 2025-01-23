# Reference

## What is reference?

C++的reference本质上是**alias**，不是**object**。想要理解reference的本质，可以思考它和pointer的差别，这在 `Pointer-VS-reference`章节进行了讨论。

> 关于alias，参见`C++\Language-reference\Alias`。



## Referenceable and non-referenceable type



### [cppreference Metaprogramming library (since C++11)](https://en.cppreference.com/w/cpp/meta) # Definitions

> NOTE: 这是 cppreference 中对"*referenceable types*"的正式定义

The following types are collectively called *referenceable types*:

- [object types](https://en.cppreference.com/w/cpp/language/type#Object_type)
- [function types](https://en.cppreference.com/w/cpp/language/function#Function_type) without *cv* and *ref* (在 stackoverflow [Is only “void” a non-referenceable type?](https://stackoverflow.com/questions/59367341/is-only-void-a-non-referenceable-type) # [A](https://stackoverflow.com/a/59367376/23877800) 中给出了验证)
- [reference types](https://en.cppreference.com/w/cpp/language/reference) (创建引用的引用)

For any referenceable type `T`, a reference to it can be created[[1\]](https://en.cppreference.com/w/cpp/meta#cite_note-1).

1. [↑](https://en.cppreference.com/w/cpp/meta#cite_ref-1) For reference types, this can be done via [reference collapsing](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing).



### Type trait: `is_referenceable` 

写法1:

```c++
#include <iostream>
#include <type_traits>

// Primary template for is_referenceable
template<typename T, typename = void>
struct is_referenceable : std::false_type {};

// Specialization for lvalue references
template<typename T>
struct is_referenceable<T, std::void_t<T &>> : std::true_type {};

// Specialization for rvalue references
template<typename T>
struct is_referenceable<T, std::void_t<T &&>> : std::true_type {};

// Helper variable template
template<typename T>
inline constexpr bool is_referenceable_v = is_referenceable<T>::value;

// Test function
template<typename T>
void test_referenceable() {
    std::cout << "Is referenceable: " << std::boolalpha << is_referenceable_v<T> << "\n";
}

int main() {
    test_referenceable<int>(); // Should be true
    test_referenceable<int &>(); // Should be true
    test_referenceable<int &&>(); // Should be true
    test_referenceable<void>(); // Should be false
    test_referenceable<void *>(); // Should be true
    test_referenceable<void (&)()>(); // Should be false (function type)
    test_referenceable<int (*)()>(); // Should be false (function pointer)
    test_referenceable<int[5]>(); // Should be false (array type)
    test_referenceable<int[]>(); // Should be false (incomplete type)

    return 0;
}

```

编译报错:

```c++
Struct is_referenceable<T, std::void_t<T &&>> has already been defined or declare
```

写法2:

尝试加上reference

```c++
#include <iostream>
#include <type_traits>
namespace detail {
    template<class T>
    struct type_identity {
        using type = T;
    }; // or use std::type_identity (since C++20)

    template<class T> // Note that “cv void&” is a substitution failure
    auto try_add_lvalue_reference(int) -> type_identity<T &>;
    template<class T> // Handle T = cv void case
    auto try_add_lvalue_reference(...) -> type_identity<T>;

    template<class T>
    auto try_add_rvalue_reference(int) -> type_identity<T &&>;
    template<class T>
    auto try_add_rvalue_reference(...) -> type_identity<T>;
} // namespace detail

template<class T>
struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)) {};

template<class T>
struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {};


// Helper variable template
template<typename T>
inline constexpr bool is_l_referenceable_v = !std::is_same_v<std::add_lvalue_reference_t<T>, T>;

template<typename T>
inline constexpr bool is_r_referenceable_v = !std::is_same_v<std::add_rvalue_reference_t<T>, T>;


// Helper variable template
template<typename T>
inline constexpr bool is_referenceable_v = is_l_referenceable_v<T> || is_r_referenceable_v<T>;

// Test function
template<typename T>
void test_referenceable() {
    std::cout << "Is referenceable: " << std::boolalpha << is_referenceable_v<T> << "\n";
}

int main() {
    test_referenceable<int>(); // Should be true
    test_referenceable<int &>(); // Should be true
    test_referenceable<int &&>(); // Should be true
    test_referenceable<void>(); // Should be false
    test_referenceable<void *>(); // Should be true
    test_referenceable<void (&)()>(); // Should be false (function type)
    test_referenceable<int (*)()>(); // Should be false (function pointer)
    test_referenceable<int[5]>(); // Should be false (array type)
    test_referenceable<int[]>(); // Should be false (incomplete type)

    return 0;
}

```





### Example

stackoverflow [Is only “void” a non-referenceable type?](https://stackoverflow.com/questions/59367341/is-only-void-a-non-referenceable-type) # [A](https://stackoverflow.com/a/59367376/23877800) 

```c++
#include <iostream>
using std::add_lvalue_reference_t;
using std::add_rvalue_reference_t;
using std::boolalpha;
using std::cout;
using std::endl;
using std::is_reference_v;

int main(void) {

    cout << boolalpha << is_reference_v<add_rvalue_reference_t<int>> << endl; // true
    cout << boolalpha << is_reference_v<add_rvalue_reference_t<int &>> << endl; // true
    cout << boolalpha << is_reference_v<add_rvalue_reference_t<int &&>> << endl; // true
    cout << boolalpha << is_reference_v<add_rvalue_reference_t<void>> << endl; // false

    cout << boolalpha << is_reference_v<add_lvalue_reference_t<void()>> << endl; // true
    cout << boolalpha << is_reference_v<add_rvalue_reference_t<void() const>> << endl; // false
    cout << boolalpha << is_reference_v<add_rvalue_reference_t<void() volatile>> << endl; // false
    cout << boolalpha << is_reference_v<add_rvalue_reference_t<void() const volatile>> << endl; // false
    cout << boolalpha << is_reference_v<add_rvalue_reference_t<void() &>> << endl; // false
    cout << boolalpha << is_reference_v<add_rvalue_reference_t<void() &&>> << endl; // false

    return 0;
}

```



## Reference classification

### 分类方式1: value category 

let's clarify the types of references in `C++`:

1. **Lvalue Reference**: A reference that can bind to an lvalue (an object that has a persistent address).

   ```c++
   int x = 10;
   int& lref = x; // lref is an lvalue reference to x
   ```

2. **Rvalue Reference**: A reference that can bind to an rvalue (a temporary object).

   ```c++
   int&& rref = 20; // rref is an rvalue reference to a temporary integer
   ```


3. **Universal Reference (Forwarding Reference)**: A reference that can bind to both **lvalues** and **rvalues**, typically found in template code. It is declared as `T&&` where `T` is a template type that is deduced.

   ```c++
   template<typename T>
   void func(T&& arg); // arg is a universal reference
   ```

简而言之:

- lvalue reference
- rvalue reference
- forwarding reference/universal reference

使用准确的value category语言来进行描述:

1) `&`是左值(glvalues)的alias

2) `&&`是右值(rvalues)的alias



#### Reference & value category & const

在 stackoverflow [What is move semantics?](https://stackoverflow.com/questions/3106110/what-is-move-semantics) # [A](https://stackoverflow.com/a/11540204) 中的总结：

```c++
            lvalue   const lvalue   rvalue   const rvalue
---------------------------------------------------------              
X&          yes
const X&    yes      yes            yes      yes
X&&                                 yes
const X&&                           yes      yes
```

> In practice, you can forget about `const X&&`. Being restricted to read from **rvalues** is not very useful.

上述表格总结得非常好，基本上涵盖了reference、value category and const的所有可能的组合，下面结合具体的例子来对它进行仔细的说明:

##### 1) `const X&`  to lvalue

来源: csdn [【C++数据类型】const 引用的几点用法](https://blog.csdn.net/hyman_c/article/details/52700094)

这个例子中的`ra`就是典型的`const X&`  to lvalue，后续对`ra`的使用需要保持相同的CV，否则会编译报错，下面是一个典型的例子: 

```C++
#include<iostream>

using namespace std;

void print(int &a) {
    cout << a << endl;
}

int main() {
    int a = 10;
    const int &ra = a;
    print(ra); //传入常左值引用
    return 0;
}
// g++ test.cpp

```

上述代码编译报错如下:

```C++
test.cpp: 在函数‘int main()’中:
test.cpp:12:10: 错误：将类型为‘int&’的引用初始化为类型为‘const int’的表达式无效
  print(ra); //传入常左值引用
          ^
test.cpp:4:6: 错误：在传递‘void print(int&)’的第 1 个实参时
 void print(int &a)

```

上述报错提示中的`const int`所指为`ra`，显然上述程序就违背了CV原则，修改版本如下:

```C++
#include<iostream>

using namespace std;

void print(const int &a) {
    cout << a << endl;
}

int main() {
    int a = 10;
    const int &ra = a;
    print(ra); //传入常左值引用
    return 0;
}
// g++ test.cpp

```

##### 2) `const X&` to const lvalue

关于此的一个典型的例子就是在const-qualified member function中，所有的member data都是const-qualified的，如果想要reference，必须要使用`const X&` ，在`C++\Language-reference\Classes\Members\Non-static-member\Function-member`中给出了典型的例子。

##### 3) `const X&`  to rvalue

下面是一个小例子: 


如果一个函数的说明如下:

```c++
Sub(int ReSubTimes, std::chrono::seconds& ReSubInterval)
```

它的第二个入参的类型是左值引用

下面两种调用方式

```c++
#include<chrono>

void Sub(int ReSubTimes, std::chrono::seconds &ReSubInterval) {}

int main() {
    std::chrono::seconds t(3);
    Sub(3, t);
    Sub(3, std::chrono::seconds(3));
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



#### Rvalue reference VS forwarding reference

这两个特性，为C++语言带来了较大的改变: 

1、**rvalue reference**用于**move semantic**；

2、**forwarding reference**用于**perfect forwarding**；



| reference类型        |                                                              |
| -------------------- | ------------------------------------------------------------ |
| rvalue reference     | [std::move](https://en.cppreference.com/w/cpp/utility/move)  |
| forwarding reference | [std::forward](https://en.cppreference.com/w/cpp/utility/forward) |



[cppreference std::move#Notes](https://en.cppreference.com/w/cpp/utility/move#Notes):

> One exception is when the type of the function parameter is rvalue reference to type template parameter ("forwarding reference" or "universal reference"), in which case **std::forward** is used instead.

上面这段话给我的感觉是：forwarding reference是一种特殊的rvalue reference。



### 分类方式2: referenced

Declares a named variable as a reference, that is, an alias to an already-existing **object** or **function**.

1) alias to **already-existing** object

2) alias to **already-existing** function



## Type traits



[std::is_lvalue_reference](https://en.cppreference.com/w/cpp/types/is_lvalue_reference) 

```c++
#include <type_traits>
template<class T>
struct is_lvalue_reference : std::false_type {};
template<class T>
struct is_lvalue_reference<T &> : std::true_type {};

```



[std::is_rvalue_reference](https://en.cppreference.com/w/cpp/types/is_rvalue_reference) 

```c++
#include <type_traits>

template<class T>
struct is_rvalue_reference : std::false_type {};
template<class T>
struct is_rvalue_reference<T &&> : std::true_type {};

```



[std::is_reference](https://en.cppreference.com/w/cpp/types/is_reference) 

```c++
#include <type_traits>
template<class T>
struct is_reference : std::false_type {};
template<class T>
struct is_reference<T &> : std::true_type {};
template<class T>
struct is_reference<T &&> : std::true_type {};

```



## See also

isocpp faq [References](https://isocpp.org/wiki/faq/references) 

