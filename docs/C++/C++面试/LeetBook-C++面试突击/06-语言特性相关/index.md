# 语言特性相关

## 左值和右值的区别？左值引用和右值引用的区别，如何将左值转换成右值？

> NOTE: 
>
> 一、参见 `C++\CppReference\Expressions\Value-categories`
>
> 下面的解释是不好的
>
> 

左值：指表达式结束后依然存在的持久对象。

右值：表达式结束就不再存在的临时对象。

左值和右值的区别：左值持久，右值短暂

右值引用和左值引用的区别：

1、左值引用不能绑定到要转换的表达式、字面常量或返回右值的表达式。右值引用恰好相反，可以绑定到这类表达式，但不能绑定到一个左值上。

2、右值引用必须绑定到右值的引用，通过 && 获得。右值引用只能绑定到一个将要销毁的对象上，因此可以自由地移动其资源。
`std::move` 可以将一个左值强制转化为右值，继而可以通过右值引用使用该值，以用于移动语义。

```C++
#include <iostream>
using namespace std;

void fun1(int& tmp) 
{ 
  cout << "fun1(int& tmp):" << tmp << endl; 
} 

void fun2(int&& tmp) 
{ 
  cout << "fun2(int&& tmp)" << tmp << endl; 
} 

int main() 
{ 
  int var = 11; 
  fun1(12); // error: cannot bind non-const lvalue reference of type 'int&' to an rvalue of type 'int'
  fun1(var);
  fun2(1); 
}

```

## `std::move()` 函数的实现原理

> NOTE: 
>
> 参见 `C++\CppReference\Classes\Special-member-function\C++11-Move` 章节

`std::move()` 函数原型：

```C++
template <typename T>
typename remove_reference<T>::type&& move(T&& t)
{
	return static_cast<typename remove_reference<T>::type &&>(t);
}
```

说明：引用折叠原理

右值传递给上述函数的形参 T&& 依然是右值，即 T&& && 相当于 T&&。

左值传递给上述函数的形参 T&& 依然是左值，即 T&& & 相当于 T&。

小结：通过引用折叠原理可以知道，move() 函数的形参既可以是左值也可以是右值。

remove_reference 具体实现：

```C++
//原始的，最通用的版本
template <typename T> struct remove_reference{
    typedef T type;  //定义 T 的类型别名为 type
};
 
//部分版本特例化，将用于左值引用和右值引用
template <class T> struct remove_reference<T&> //左值引用
{ typedef T type; }
 
template <class T> struct remove_reference<T&&> //右值引用
{ typedef T type; }   
  
//举例如下,下列定义的a、b、c三个变量都是int类型
int i;
remove_refrence<decltype(42)>::type a;             //使用原版本，
remove_refrence<decltype(i)>::type  b;             //左值引用特例版本
remove_refrence<decltype(std::move(i))>::type  b;  //右值引用特例版本 


```



## 什么是指针？指针的大小及用法？

> NOTE: 
>
> 参见 `C++\Resource-management\Memory\Pointer&&array\Pointer` 章节
>
> 

**指针：** 指向另外一种类型的复合类型。

**指针的大小：** 在 64 位计算机中，指针占 8 个字节空间。

```C++
#include<iostream>

using namespace std;

int main(){
    int *p = nullptr;
    cout << sizeof(p) << endl; // 8

    char *p1 = nullptr;
    cout << sizeof(p1) << endl; // 8
    return 0;
}

```

### 指针的用法：

1、指向普通对象的指针

```C++
#include <iostream>

using namespace std;

class A
{
};

int main()
{
    A *p = new A();
    return 0;
}

```

2、指向常量对象的指针：常量指针

```C++
#include <iostream>
using namespace std;

int main(void)
{
    const int c_var = 10;
    const int * p = &c_var;
    cout << *p << endl;
    return 0;
}

```

3、指向函数的指针：函数指针

```C++
#include <iostream>
using namespace std;

int add(int a, int b){
    return a + b;
}

int main(void)
{
    int (*fun_p)(int, int);
    fun_p = add;
    cout << fun_p(1, 6) << endl;
    return 0;
}

```

4、指向对象成员的指针，包括指向对象成员函数的指针和指向对象成员变量的指针。

特别注意：定义指向成员函数的指针时，要标明指针所属的类。

```C++
#include <iostream>

using namespace std;

class A
{
public:
    int var1, var2; 
    int add(){
        return var1 + var2;
    }
};

int main()
{
    A ex;
    ex.var1 = 3;
    ex.var2 = 4;
    int *p = &ex.var1; // 指向对象成员变量的指针
    cout << *p << endl;

    int (A::*fun_p)();
    fun_p = A::add; // 指向对象成员函数的指针 fun_p
    cout << (ex.*fun_p)() << endl;
    return 0;
}

```

5、`this` 指针：指向类的当前对象的指针常量。

```C++
#include <iostream>
#include <cstring>
using namespace std;

class A
{
public:
    void set_name(string tmp)
    {
        this->name = tmp;
    }
    void set_age(int tmp)
    {
        this->age = age;
    }
    void set_sex(int tmp)
    {
        this->sex = tmp;
    }
    void show()
    {
        cout << "Name: " << this->name << endl;
        cout << "Age: " << this->age << endl;
        cout << "Sex: " << this->sex << endl;
    }

private:
    string name;
    int age;
    int sex;
};

int main()
{
    A *p = new A();
    p->set_name("Alice");
    p->set_age(16);
    p->set_sex(1);
    p->show();

    return 0;
}

```



## 什么是野指针和悬空指针？

悬空指针：

若指针指向一块内存空间，当这块内存空间被释放后，该指针依然指向这块内存空间，此时，称该指针为“悬空指针”。

举例：

```C++
void *p = malloc(size);
free(p); 
// 此时，p 指向的内存空间已释放， p 就是悬空指针。
```

野指针：

“野指针”是指不确定其指向的指针，未初始化的指针为“野指针”。

```C++
void *p; 
// 此时 p 是“野指针”。
```

## C++ 11 `nullptr` 比 `NULL` 优势

> NOTE: 
>
> 参见 `C++\Resource-management\Memory\Pointer&&array\Pointer\Null-pointer\nullptr-idiom` 章节

1、`NULL`：预处理变量，是一个宏，它的值是 0，定义在头文件 `<cstdlib>` 中，即 `#define NULL 0`。

2、`nullptr`：C++ 11 中的关键字，是一种特殊类型的字面值，可以被转换成任意其他类型。

`nullptr` 的优势：

1、有类型，类型是 `typdef decltype(nullptr) nullptr_t;`，使用 `nullptr` 提高代码的健壮性。

2、函数重载：因为 `NULL` 本质上是 0，在函数调用过程中，若出现函数重载并且传递的实参是 `NULL`，可能会出现，不知和哪一个函数匹配的情况；但是传递实参 `nullptr` 就不会出现这种情况。

```C++
#include <iostream>
#include <cstring>
using namespace std;

void fun(char const *p)
{
    cout << "fun(char const *p)" << endl;
}

void fun(int tmp)
{
    cout << "fun(int tmp)" << endl;
}

int main()
{
    fun(nullptr); // fun(char const *p)
    /*
    fun(NULL); // error: call of overloaded 'fun(NULL)' is ambiguous
    */
    return 0;
}

```



## 指针和引用的区别？

> NOTE: 
>
> 参见 `C++\CppReference\Reference\Pointer-VS-reference` 章节

1、指针所指向的内存空间在程序运行过程中可以改变，而引用所绑定的对象一旦绑定就不能改变。（是否可变）

2、指针本身在内存中占有内存空间，引用相当于变量的别名，在内存中不占内存空间。（是否占内存）

3、指针可以为空，但是引用必须绑定对象。（是否可为空）

4、指针可以有多级，但是引用只能一级。（是否能为多级）



## 常量指针和指针常量的区别

### 常量指针

> NOTE: 
>
> 典型的例子就是C++: 
>
> ```C++
> const char *
> ```
>
> 

常量指针本质上是个指针，只不过这个指针指向的对象是常量。

特点：`const` 的位置在指针声明运算符 `*` 的左侧。只要 `const` 位于 `*` 的左侧，无论它在类型名的左边或右边，都表示指向常量的指针。（可以这样理解`，*` 左侧表示指针指向的对象，该对象为常量，那么该指针为常量指针。）

```C++
const int * p;
int const * p;
```

注意 1：指针指向的对象不能通过这个指针来修改，也就是说常量指针可以被赋值为变量的地址，之所以叫做常量指针，是限制了通过这个指针修改变量的值。

例如：

```C++
#include <iostream>
using namespace std;

int main()
{
    const int c_var = 8;
    const int *p = &c_var; 
    *p = 6;            // error: assignment of read-only location '* p'
    return 0;
}

```

注意 2：虽然常量指针指向的对象不能变化，可是因为常量指针本身是一个变量，因此，可以被重新赋值。

例如：

```C++
#include <iostream>
using namespace std;

int main()
{
    const int c_var1 = 8;
    const int c_var2 = 8;
    const int *p = &c_var1; 
    p = &c_var2;
    return 0;
}

```

### 指针常量

> NOTE: 
>
> "A reference can be thought of as a constant pointer"
>
> 

指针常量的本质上是个常量，只不过这个常量的值是一个指针。

特点：const 位于指针声明操作符右侧，表明该对象本身是一个常量，`*` 左侧表示该指针指向的类型，即以 `*` 为分界线，其左侧表示指针指向的类型，右侧表示指针本身的性质。

```c++
const int var;
int * const c_p = &var; 
```

注意 1：指针常量的值是指针，这个值因为是常量，所以指针本身不能改变。

```C++
#include <iostream>
using namespace std;

int main()
{
    int var, var1;
    int * const c_p = &var;
    c_p = &var1; // error: assignment of read-only variable 'c_p'
    return 0;
}

```

注意 2：指针的内容可以改变。

```C++
#include <iostream>
using namespace std;

int main()
{
    int var = 3;
    int * const c_p = &var;
    *c_p = 12; 
    return 0;
}

```



## 函数指针和指针函数的区别

### 指针函数

指针函数本质是一个函数，只不过该函数的返回值是一个指针。相对于普通函数而言，只是返回值是指针。

```C++
#include <iostream>
using namespace std;

struct Type
{
  int var1;
  int var2;
};

Type * fun(int tmp1, int tmp2){
    Type * t = new Type();
    t->var1 = tmp1;
    t->var2 = tmp2;
    return t;
}

int main()
{
    Type *p = fun(5, 6);
    return 0;
}


```

### 函数指针

函数指针本质是一个指针变量，只不过这个指针指向一个函数。函数指针即指向函数的指针。

```C++
#include <iostream>
using namespace std;
int fun1(int tmp1, int tmp2)
{
  return tmp1 * tmp2;
}
int fun2(int tmp1, int tmp2)
{
  return tmp1 / tmp2;
}

int main()
{
  int (*fun)(int x, int y); 
  fun = fun1;
  cout << fun(15, 5) << endl; 
  fun = fun2;
  cout << fun(15, 5) << endl; 
  return 0;
}
/*
运行结果：
75
3
*/

```

函数指针和指针函数的区别：

1、本质不同

指针函数本质是一个函数，其返回值为指针。

函数指针本质是一个指针变量，其指向一个函数。
2、定义形式不同

指针函数：`int* fun(int tmp1, int tmp2);` ，这里`*` 表示函数的返回值类型是指针类型。

函数指针：`int (*fun)(int tmp1, int tmp2);`，这里* 表示变量本身是指针类型。

3、用法不同



## 强制类型转换有哪几种？

> NOTE: 
>
> 参见 `C++\CppReference\Basic-concept\Type-system\Type-conversion\Cast-operator` 章节

### `static_cast`

`static_cast`：用于数据的强制类型转换，强制将一种数据类型转换为另一种数据类型。

1、用于基本数据类型的转换。

2、用于类层次之间的基类和派生类之间 指针或者引用 的转换（不要求必须包含虚函数，但必须是有相互联系的类），进行上行转换（派生类的指针或引用转换成基类表示）是安全的；进行下行转换（基类的指针或引用转换成派生类表示）由于没有动态类型检查，所以是不安全的，最好用 dynamic_cast 进行下行转换。

> NOTE:
>
> 在`C++\Idiom\Generic-programming\CRTP`中，就使用了`static_cast`

3、可以将空指针转化成目标类型的空指针。

4、可以将任何类型的表达式转化成 `void` 类型。

### `const_cast`

`const_cast`：强制去掉常量属性，不能用于去掉变量的常量性，只能用于去除指针或引用的常量性，将常量指针转化为非常量指针或者将常量引用转化为非常量引用（注意：表达式的类型和要转化的类型是相同的）。

> NOTE: 
>
> "tag-prevents C++ programmers from shooting themselves in the foot"

### `reinterpret_cast`

`reinterpret_cast`：改变指针或引用的类型、将指针或引用转换为一个足够长度的整型、将整型转化为指针或引用类型。



### `dynamic_cast`

> NOTE: 
>
> polymorphic type、依赖于RTTI

1、其他三种都是编译时完成的，动态类型转换是在程序运行时处理的，运行时会进行类型检查。

2、只能用于带有虚函数的基类或派生类的指针或者引用对象的转换，转换成功返回指向类型的指针或引用，转换失败返回 NULL；不能用于基本数据类型的转换。

3、在向上进行转换时，即派生类类的指针转换成基类类的指针和 static_cast 效果是一样的，（注意：这里只是改变了指针的类型，指针指向的对象的类型并未发生改变）。

```C++
#include <iostream>
#include <cstring>

using namespace std;

class Base
{
};

class Derive : public Base
{
};

int main()
{
    Base *p1 = new Derive();
    Derive *p2 = new Derive();

    //向上类型转换
    p1 = dynamic_cast<Base *>(p2);
    if (p1 == NULL)
    {
        cout << "NULL" << endl;
    }
    else
    {
        cout << "NOT NULL" << endl; //输出
    }

    return 0;
}

```

4、在下行转换时，基类的指针类型转化为派生类类的指针类型，只有当要转换的指针指向的对象类型和转化以后的对象类型相同时，才会转化成功。

```C++
#include <iostream>
#include <cstring>

using namespace std;

class Base
{
public:
    virtual void fun()
    {
        cout << "Base::fun()" << endl;
    }
};

class Derive : public Base
{
public:
    virtual void fun()
    {
        cout << "Derive::fun()" << endl;
    }
};

int main()
{
    Base *p1 = new Derive();
    Base *p2 = new Base();
    Derive *p3 = new Derive();

    //转换成功
    p3 = dynamic_cast<Derive *>(p1);
    if (p3 == NULL)
    {
        cout << "NULL" << endl;
    }
    else
    {
        cout << "NOT NULL" << endl; // 输出
    }

    //转换失败
    p3 = dynamic_cast<Derive *>(p2);
    if (p3 == NULL)
    {
        cout << "NULL" << endl; // 输出
    }
    else
    {
        cout << "NOT NULL" << endl;
    }

    return 0;
}

```

## 如何判断结构体是否相等？能否用 `memcmp` 函数判断结构体相等？

> NOTE: 
>
> 参见 `C++\CppReference\Basic-concept\Object\cppreference-object`
>
> object representation 和 value representation

需要重载操作符 == 判断两个结构体是否相等，不能用函数 `memcmp` 来判断两个结构体是否相等，因为 `memcmp` 函数是逐个字节进行比较的，而结构体存在内存空间中保存时存在字节对齐，字节对齐时补的字节内容是随机的，会产生垃圾值，所以无法比较。

利用运算符重载来实现结构体对象的比较：

```c++
#include <iostream>

using namespace std;

struct A
{
    char c;
    int val;
    A(char c_tmp, int tmp) : c(c_tmp), val(tmp) {}

    friend bool operator==(const A &tmp1, const A &tmp2); //  友元运算符重载函数
};

bool operator==(const A &tmp1, const A &tmp2)
{
    return (tmp1.c == tmp2.c && tmp1.val == tmp2.val);
}

int main()
{
    A ex1('a', 90), ex2('b', 80);
    if (ex1 == ex2)
        cout << "ex1 == ex2" << endl;
    else
        cout << "ex1 != ex2" << endl; // 输出
    return 0;
}

```

## 参数传递时，值传递、引用传递、指针传递的区别？

参数传递的三种方式：

1、值传递：形参是实参的拷贝，函数对形参的所有操作不会影响实参。

2、指针传递：本质上是值传递，只不过拷贝的是指针的值，拷贝之后，实参和形参是不同的指针，通过指针可以间接的访问指针所指向的对象，从而可以修改它所指对象的值。

3、引用传递：当形参是引用类型时，我们说它对应的实参被引用传递。

```C++
#include <iostream>
using namespace std;

void fun1(int tmp){ // 值传递
    cout << &tmp << endl;
}

void fun2(int * tmp){ // 指针传递
    cout << tmp << endl;
}

void fun3(int &tmp){ // 引用传递
    cout << &tmp << endl;
}

int main()
{
    int var = 5;
    cout << "var 在主函数中的地址：" << &var << endl;

    cout << "var 值传递时的地址：";
    fun1(var);

    cout << "var 指针传递时的地址：";
    fun2(&var);

    cout << "var 引用传递时的地址：";
    fun3(var);
    return 0;
}

/*
运行结果：
var 在主函数中的地址：0x23fe4c
var 值传递时的地址：0x23fe20
var 指针传递时的地址：0x23fe4c
var 引用传递时的地址：0x23fe4c
*/

```





---

> 后面开始讨论generic programming

---

## 什么是模板？如何实现？

> NOTE: 
>
> 一、C++使用template来实现generic programming
>
> 二、
>
> 参见 `C++\CppReference\Template` 章节
>
> 参见 `C++\Programming-paradigm\TMP` 章节

模板：创建类或者函数的蓝图或者公式，分为函数模板和类模板。

实现方式：模板定义以关键字 template 开始，后跟一个模板参数列表。

1、模板参数列表不能为空；

2、模板类型参数前必须使用关键字 `class` 或者 `typename`，在模板参数列表中这两个关键字含义相同，可互换使用。

```C++
template <typename T, typename U, ...>
```

函数模板：通过定义一个函数模板，可以避免为每一种类型定义一个新函数。

1、对于函数模板而言，模板类型参数可以用来指定返回类型或函数的参数类型，以及在函数体内用于变量声明或类型转换。

2、函数模板实例化：当调用一个模板时，编译器用函数实参来推断模板实参，从而使用实参的类型来确定绑定到模板参数的类型。

```C++
#include<iostream>

using namespace std;

template <typename T>
T add_fun(const T & tmp1, const T & tmp2){
    return tmp1 + tmp2;
}

int main(){
    int var1, var2;
    cin >> var1 >> var2;
    cout << add_fun(var1, var2);

    double var3, var4;
    cin >> var3 >> var4;
    cout << add_fun(var3, var4);
    return 0;
}

```

类模板：类似函数模板，类模板以关键字 template 开始，后跟模板参数列表。但是，编译器不能为类模板推断模板参数类型，需要在使用该类模板时，在模板名后面的尖括号中指明类型。

```C++
#include <iostream>

using namespace std;

template <typename T>
class Complex
{
public:
    //构造函数
    Complex(T a, T b)
    {
        this->a = a;
        this->b = b;
    }

    //运算符重载
    Complex<T> operator+(Complex &c)
    {
        Complex<T> tmp(this->a + c.a, this->b + c.b);
        cout << tmp.a << " " << tmp.b << endl;
        return tmp;
    }

private:
    T a;
    T b;
};

int main()
{
    Complex<int> a(10, 20);
    Complex<int> b(20, 30);
    Complex<int> c = a + b;

    return 0;
}

```



## 函数模板和类模板的区别？

1、实例化方式不同：函数模板实例化由编译程序在处理函数调用时自动完成，类模板实例化需要在程序中显示指定。

2、实例化的结果不同：函数模板实例化后是一个函数，类模板实例化后是一个类。

3、默认参数：类模板在模板参数列表中可以有默认参数。

4、特化：函数模板只能全特化；而类模板可以全特化，也可以偏特化。、

5、调用方式不同：函数模板可以隐式调用，也可以显示调用；类模板只能显示调用。



## 什么是可变参数模板？

> NOTE: 
>
> 这是C++11的新特性，参见 `C++\CppReference\Template\Parameter-pack` 章节
>
> 

可变参数模板：接受可变数目参数的模板函数或模板类。将可变数目的参数被称为参数包，包括模板参数包和函数参数包。

1、模板参数包：表示零个或多个模板参数；

2、函数参数包：表示零个或多个函数参数。

用省略号来指出一个模板参数或函数参数表示一个包，在模板参数列表中，`class...` 或 `typename...` 指出接下来的参数表示零个或多个类型的列表；一个类型名后面跟一个省略号表示零个或多个给定类型的非类型参数的列表。当需要知道包中有多少元素时，可以使用 `sizeof...` 运算符。

```C++
template <typename T, typename... Args> // Args 是模板参数包
void foo(const T &t, const Args&... rest); // 可变参数模板，rest 是函数参数包
```



```C++
#include <iostream>

using namespace std;

template <typename T>
void print_fun(const T &t)
{
    cout << t << endl; // 最后一个元素
}

template <typename T, typename... Args>
void print_fun(const T &t, const Args &...args)
{
    cout << t << " ";
    print_fun(args...);
}

int main()
{
    print_fun("Hello", "wolrd", "!");
    return 0;
}
/*运行结果：
Hello wolrd !

*/

```

说明：可变参数函数通常是递归的，第一个版本的 `print_fun` 负责终止递归并打印初始调用中的最后一个实参。第二个版本的 `print_fun` 是可变参数版本，打印绑定到 `t` 的实参，并用来调用自身来打印函数参数包中的剩余值。

## 什么是模板特化？为什么特化？

> NOTE: 
>
> 一、显然，它是一种polymorphism
>
> 

模板特化的原因：模板并非对任何模板实参都合适、都能实例化，某些情况下，通用模板的定义对特定类型不合适，可能会编译失败，或者得不到正确的结果。因此，当不希望使用模板版本时，可以定义类或者函数模板的一个特例化版本。

模板特化：模板参数在某种特定类型下的具体实现。分为函数模板特化和类模板特化

1、函数模板特化：将函数模板中的全部类型进行特例化，称为函数模板特化。

2、类模板特化：将类模板中的部分或全部类型进行特例化，称为类模板特化。

特化分为全特化和偏特化：

1、全特化：模板中的模板参数全部特例化。

2、偏特化：模板中的模板参数只确定了一部分，剩余部分需要在编译器编译时确定。

说明：要区分下函数重载与函数模板特化

定义函数模板的特化版本，本质上是接管了编译器的工作，为原函数模板定义了一个特殊实例，而不是函数重载，函数模板特化并不影响函数匹配。

> NOTE: 
>
> 参见 `Specialization-and-overload` 章节
>
> 

```C++
#include <iostream>
#include <cstring>

using namespace std;
//函数模板
template <class T>
bool compare(T t1, T t2)
{
    cout << "通用版本：";
    return t1 == t2;
}

template <> //函数模板特化
bool compare(char *t1, char *t2)
{
    cout << "特化版本：";
    return strcmp(t1, t2) == 0;
}

int main(int argc, char *argv[])
{
    char arr1[] = "hello";
    char arr2[] = "abc";
    cout << compare(123, 123) << endl;
    cout << compare(arr1, arr2) << endl;

    return 0;
}
/*
运行结果：
通用版本：1
特化版本：0
*/

```

## include " " 和 <> 的区别

`include<文件名>` 和 `#include"文件名"` 的区别:

1、查找文件的位置：`include<文件名>` 在**标准库头文件**所在的目录中查找，如果没有，再到当前源文件所在目录下查找；`#include"文件名"` 在当前源文件所在目录中进行查找，如果没有；再到系统目录中查找。

2、使用习惯：对于标准库中的头文件常用 `include<文件名>`，对于自己定义的头文件，常用 `#include"文件名"`

