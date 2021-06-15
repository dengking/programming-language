# Compile-time and run-time in C++ 总结

1、C++是充分发挥compile-time、static的语言，这在 "What-is-new-in-C++" 中也进行了讨论

2、CppCoreGuidelines [Philosophy](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#p-philosophy) 

[P.4: Ideally, a program should be statically type safe](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p4-ideally-a-program-should-be-statically-type-safe)

[P.5: Prefer compile-time checking to run-time checking](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p5-prefer-compile-time-checking-to-run-time-checking)

因此，我们应该prefer compile time。



## [`static_assert`](https://en.cppreference.com/w/cpp/language/static_assert) VS [`assert`](https://en.cppreference.com/w/cpp/error/assert) 

### [static_assert](https://en.cppreference.com/w/cpp/language/static_assert) 

`static_assert` 是compile-time；

syntax如下：

```c++
static_assert ( bool_constexpr , message )
```

它的第一个入参是：`bool_constexpr`，解释是：

> a contextually converted constant expression of type `bool`

`static_assert` 的第一个入参往往是和 `type_traits` 中的 各种关于 type trait的函数一起使用的，也就是compile time的，所以是constant expression。



### `assert` 

run-time

## Static array VS dynamic array

在 https://stackoverflow.com/a/4810676 中提及了；



## Compile-time concept

下面总结了C++中的compile-time concept。

### Constant expression

关于constant expression，参见`C-family-language\C++\Language-reference\Expressions\Operators\Other\Constant-expressions`章节。

### Template

关于template，参见`C-family-language\C++\Language-reference\Template`章节。

### Enum

参见`C++\Language-reference\Enum`章节。

## Compile-time context

下面是总结的（部分）compile-time context：

- template-meta programming
- array sizes，参见[Array declaration](https://en.cppreference.com/w/cpp/language/array)
- [switch statement](https://en.cppreference.com/w/cpp/language/switch)

## Run-time concept

`virtual`

## Static polymorphism VS dynamic polymorphism 

参见`C++\Guide\Implementation-of-polymorphism\`章节。



## Static type and dynamic type

参见`C++\Language-reference\Basic-concept\Type-system\Type-system\OOP-class-type\Polymorphic-type`。

### [`static_cast`](https://en.cppreference.com/w/cpp/language/static_cast) VS [`dynamic_cast`](https://en.cppreference.com/w/cpp/language/dynamic_cast) 

`static_cast` 是 compile-time type conversion

`dynamic_cast` 是 run-time type conversion

### `decltype` VS `typeid`

参见 `C++\Language-reference\Basic-concept\Type-system\Type-operation\Query-type` 章节。



## static reflection VS dynamic reflection

参见 `C++\Guide\Type-相关\Reflection` 章节



## static bind VS dynamic bind



### 静态绑定和动态绑定是怎么实现的？

静态类型和动态类型：

1、静态类型：变量在声明时的类型，是在编译阶段确定的。静态类型不能更改。

2、动态类型：目前所指对象的类型，是在运行阶段确定的。动态类型可以更改。

静态绑定和动态绑定：

1、静态绑定是指程序在 编译阶段 确定对象的类型（静态类型）。

2、动态绑定是指程序在 运行阶段 确定对象的类型（动态类型）。

静态绑定和动态绑定的区别：

发生的时期不同：如上。

对象的静态类型不能更改，动态类型可以更改。

注：对于类的成员函数，只有虚函数是动态绑定，其他都是静态绑定。



```C++
#include <iostream>

using namespace std;

class Base
{
public:
	virtual void fun() { cout << "Base::fun()" << endl;
     }
};
class Derive : public Base
{
public:
	void fun() { cout << "Derive::fun()"; 
    }
};


int main()
{
	Base *p = new Derive(); // p 的静态类型是 Base*，动态类型是 Derive*
    p->fun(); // fun 是虚函数，运行阶段进行动态绑定
	return 0;
}

```

```
/*
运行结果：
Derive::fun()
*/

```

