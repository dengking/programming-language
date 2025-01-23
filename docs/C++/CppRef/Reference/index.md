# Reference

## What is reference?

C++的reference本质上是**alias**，不是**object**。想要理解reference的本质，可以思考它和pointer的差别，这在 `Pointer-VS-reference`章节进行了讨论。

> 关于alias，参见`C++\Language-reference\Alias`。

cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference) 

> References are not objects; they do not necessarily occupy storage, although the compiler may allocate storage if it is necessary to implement the desired semantics (e.g. a non-static data member of reference type usually increases the size of the class by the amount necessary to store a memory address).

## Referenceable and non-referenceable type



### [cppreference Metaprogramming library (since C++11)](https://en.cppreference.com/w/cpp/meta) # Definitions

> NOTE: 这是 cppreference 中对"*referenceable types*"的正式定义

The following types are collectively called *referenceable types*:

- [object types](https://en.cppreference.com/w/cpp/language/type#Object_type)
- [function types](https://en.cppreference.com/w/cpp/language/function#Function_type) without *cv* and *ref* (在 stackoverflow [Is only “void” a non-referenceable type?](https://stackoverflow.com/questions/59367341/is-only-void-a-non-referenceable-type) # [A](https://stackoverflow.com/a/59367376/23877800) 中给出了验证)
- [reference types](https://en.cppreference.com/w/cpp/language/reference) (需要注意的是这是通过 [reference collapsing](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing) 来实现的，根据 cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference)  章节的内容可知，这仅仅在"It is permitted to form references to references through type manipulations in templates or typedefs, in which case the *reference collapsing* rules apply")

For any referenceable type `T`, a reference to it can be created[[1\]](https://en.cppreference.com/w/cpp/meta#cite_note-1).

1. [↑](https://en.cppreference.com/w/cpp/meta#cite_ref-1) For reference types, this can be done via [reference collapsing](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing).



### Non-referenceable type

#### void

因为void就是表示无，stackoverflow [Is only “void” a non-referenceable type?](https://stackoverflow.com/questions/59367341/is-only-void-a-non-referenceable-type) # [A](https://stackoverflow.com/a/78410179/23877800)  

We will also need the definition of *object types* (link above):

> - object types are (possibly cv-qualified) types that are not function types, reference types, or possibly cv-qualified `void` (see also [`std::is_object`](https://en.cppreference.com/w/cpp/types/is_object));

| Type                                 | Object type? |
| ------------------------------------ | ------------ |
| fundamental types / `void`           | void type    |
| fundamental types / `std::nullptr_t` | object type  |



### Type trait: `is_referenceable` 

参见 `Type-traits-for-reference` 章节



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



## cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference)  

Declares a named variable as a reference, that is, an alias to an already-existing object or function.

### Syntax

A reference is required to be initialized to refer to a valid object or function: see [reference initialization](https://en.cppreference.com/w/cpp/language/reference_initialization).

> NOTE: 在声明的时候就需要初始化

The type “reference to (possibly cv-qualified) void” cannot be formed.

> NOTE: void是non-referenceable type

**Reference types** cannot be [cv-qualified](https://en.cppreference.com/w/cpp/language/cv) at the top level; there is no syntax for that in declaration, and if a qualification(限制) is added to a typedef-name or [`decltype`](https://en.cppreference.com/w/cpp/language/decltype) specifier, or type template parameter, it is ignored.

> NOTE: 上面这段话如何理解呢？
>
> This means that you cannot declare a **reference type** as `const T&` or `volatile T&` in a way that the reference itself is considered `const` or `volatile`. Instead, the `const` and `volatile` qualifiers apply to the type being referenced, not to the reference itself.
>
> When you declare a **reference type**, such as `int&`, the **reference** itself cannot be `const` or `volatile`. For example, `const int&` means that the object being referred to cannot be modified through that **reference**, but the reference itself is not `const`.
>
> 下面是一个例子，需要注意: `void func(const T &param)`
>
> ```c++
> #include <iostream>
> template<typename T>
> void func(const T &param) // T cannot be cv-qualified at the top level
> {
>     std::cout << param << std::endl;
>     // param += 1;
> }
> 
> template<typename T>
> void func(T &param) // T cannot be cv-qualified at the top level
> {
>     std::cout << param << std::endl;
>     param += 1;
> }
> 
> int main() {
>     int a = 0;
>     func(a);
>     std::cout << a << std::endl;
> }
> 
> ```
>
> 输出如下:
>
> ```
> 0
> 1
> ```
>
> 



References are not **objects**; they do not necessarily occupy storage, although the compiler may allocate storage if it is necessary to implement the desired semantics (e.g. a **non-static data member** of reference type usually increases the size of the class by the amount necessary to store a memory address).

> NOTE: 不同的compiler有不同的implementation

Because references are not **objects**, there are no **arrays** of references, no **pointers** to references, and no **references** to references:

```C++
int& a[3]; // error: **arrays** of references
int&* p;   // error: **pointers** to references
int& &r;   // error: **references** to references
```

> NOTE: 上面这段话中的"no **references** to references"和"[cppreference Metaprogramming library (since C++11)](https://en.cppreference.com/w/cpp/meta) # Definitions"中的" *referenceable types* "定义不同



### Reference collapsing(since C++11)

It is permitted to form references to references through type manipulations in templates or typedefs, in which case the *reference collapsing* rules apply: rvalue reference to rvalue reference collapses to rvalue reference, all other combinations form **lvalue reference**(后面有主记符来帮助记录):

```C++
typedef int&  lref;
typedef int&& rref;
int n;
lref&  r1 = n; // type of r1 is int&
lref&& r2 = n; // type of r2 is int&
rref&  r3 = n; // type of r3 is int&
rref&& r4 = 1; // type of r4 is int&&
```

(This, along with special rules for [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction) when `T&&` is used in a function template(这其实是forwarding reference), forms the rules that make [std::forward](https://en.cppreference.com/w/cpp/utility/forward) possible.)

> NOTE: 上面括号中的这段话这段话揭示了 [std::forward](https://en.cppreference.com/w/cpp/utility/forward) 的实现原理，在thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 中对这个topic进行了讨论
>

> NOTE: 
>
> **Mnemonic**: 在 thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 中给出了很好的助记符:
>
> > The result is the *reference collapsing* rule. The rule is very simple. `&` always wins. So `& &` is `&`, and so are `&& &` and `& &&`. The only case where `&&` emerges(出现) from collapsing is `&& &&`. You can think of it as a logical-OR, with `&` being 1 and `&&` being 0.
>



### Lvalue references

Lvalue references can be used to alias an existing object (optionally with different cv-qualification):

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



## Type traits

参见 `Type-traits-for-reference` 章节

## See also

isocpp faq [References](https://isocpp.org/wiki/faq/references) 

