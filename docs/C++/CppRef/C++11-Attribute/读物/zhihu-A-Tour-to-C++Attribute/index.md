# zhihu [A Tour to C++ Attribute](https://zhuanlan.zhihu.com/p/64493524) 

## 内容概要

1. 什么是attribute
2. C++11到C++17的常用attribute介绍
3. 为名空间和枚举型添加attribute
4. C++17新特性：忽略未知的attribute
5. 使用using指令避免名空间的重复书写
6. Bjarne Stroustrup对attribute的使用建议

## 参考文献

1. [Attribute Syntax - Using the GNU Compiler Collection (GCC)](https://link.zhihu.com/?target=https%3A//gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html)
2. [Using the GNU Compiler Collection (GCC): Common Function Attributes](https://link.zhihu.com/?target=https%3A//gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html%23Common-Function-Attributes)
3. [__declspec | Microsoft Docs](https://link.zhihu.com/?target=https%3A//docs.microsoft.com/pl-pl/cpp/cpp/declspec)
4. [Attributes in Clang — Clang 5 documentation](https://link.zhihu.com/?target=https%3A//clang.llvm.org/docs/AttributeReference.html)
5. [C++17 attribute](https://link.zhihu.com/?target=https%3A//www.bfilipek.com/2017/07/cpp17-in-details-attributes.html)
6. [cppreference : attribute](https://link.zhihu.com/?target=https%3A//en.cppreference.com/w/cpp/language/attributes)
7. [modern c attribute](https://link.zhihu.com/?target=https%3A//arne-mertz.de/2016/12/modern-c-features-attributes/)
8. [common attribute intro](https://link.zhihu.com/?target=https%3A//kheresy.wordpress.com/2018/07/04/c-attribute)
9. [godbolt](https://link.zhihu.com/?target=https%3A//www.godbolt.org/)
10. [C++11 FAQ](https://link.zhihu.com/?target=http%3A//stroustrup.com/C%2B%2B11FAQ.html%23attributes)
11. [C++ Memory Order 与 Atomic 学习小记](https://zhuanlan.zhihu.com/p/31386431)
12. [atomic/compare_exchange](https://link.zhihu.com/?target=https%3A//en.cppreference.com/w/cpp/atomic/atomic/compare_exchange)
13. [使用C++原子量实现自旋锁](https://link.zhihu.com/?target=https%3A//www.cnblogs.com/FateTHarlaown/p/9170474.html)





## 1. 什么是attribute?

attribute是在基本程序代码中加入的辅助信息，编译器可以利用这些辅助信息来帮助自己CG（code generation），譬如用来优化或者产生特定代码（DLL，OpenMP等）。相比于其他语言（e.g. C#)，C++中的Meta information是由编译器决定的，你没有办法添加自己制定的attributes。而在C#中，我们可以从`System.Attribute`派生出来。

## 2. 一些你可能面熟的老朋友

在使用gcc作为compiler的代码中，经常出现的attribute

```cpp
struct S { short f[3]; } __attribute__ ((aligned (8)));

void fatal () __attribute__ ((noreturn));
```

MSCV中import/export DLL时出现出现的attribute

```cpp
#if COMPILING_DLL
    #define DLLEXPORT __declspec(dllexport)
#else
    #define DLLEXPORT __declspec(dllimport)
#endif
```

## 3. Modern C++是如何处理attribute的？

1. [Attribute Syntax - Using the GNU Compiler Collection (GCC)](https://link.zhihu.com/?target=https%3A//gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html)
2. [Using the GNU Compiler Collection (GCC): Common Function Attributes](https://link.zhihu.com/?target=https%3A//gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html%23Common-Function-Attributes)
3. [__declspec | Microsoft Docs](https://link.zhihu.com/?target=https%3A//docs.microsoft.com/pl-pl/cpp/cpp/declspec)
4. [Attributes in Clang — Clang 5 documentation](https://link.zhihu.com/?target=https%3A//clang.llvm.org/docs/AttributeReference.html)

在C++11之前，各个编译器对attribute的写法有着自己的一套规则，很混乱。自C++11开始，我们有了越来越多的标准attribute，也就是说对于attribute有着统一的写法，而不用使用编译器特定的标记了。

## 4. 常用attribute介绍

使用attribute的基本语法是`[attribute]`和`[[namespace::attr]]`,你几乎可以在任何东西上使用attribute，例如类型、函数、对象等。

## 4.1 `[[noreturn]]`

`[[noreturn]]`只能用于函数声明，其告诉编译器这个函数不会返回，也就是说，这个函数要么抛出异常，要么在所有执行路径上调用类似`exit`、`terminate`这些不会返回的函数。`[[noreturn]]`可以让编译器在CG的时候进行优化，也可以用来抑制编译器的warning。

```cpp
void fatal_error() {
    throw "error";
}

int sw( int v) {
    switch (v) {
    case 1:
        return 0;
    default:
        fatal_error();
    }
}
```

上面的程序中，函数`sw`在`switch`语句的`default`的分支没有返回值，所以编译器会出现以下警告

```text
In function 'int sw(int)':
warning: control reaches end of non-void function [-Wreturn-type]
```

但是只要`fatal_error()`改成

```cpp
[[noreturn]] void fatal_error() {
    throw "error";
}
```

警告就消失了。



## 4.2 `[[nodiscard]]`

如果函数被声明为`[[nodiscard]]`，或者函数by value地返回了一个被声明为`[[nodiscard]]`的enumeration/class的对象。那么，当这个函数的返回值被抛弃时，编译器会给出警告。

```cpp
struct [[nodiscard]] error_info {
    int id;
    char *message;
};
error_info enable_missile_safety_mode();
void launch_missiles();
void test_missiles() {
   enable_missile_safety_mode(); //编译器在舍弃 nodiscard 值时发布警告
   launch_missiles();
}
```

假设把`error_info enable_missile_safety_mode();`改成`error_info& enable_missile_safety_mode();`,函数就不是按值返回的了，便不会发出警告。

`[[nodiscard]]`可以提醒使用函数的人，一定要接住这个返回值（比如说`operator new`的返回值），或者一定要检查这个返回值（比如判断是否执行成功）。

## 4.3 `[[deprecated]]/[[deprecated("reason")]]`

`[[deprecated]]/[[deprecated("reason")]]`是告诉编译器和developer，这个东西已经被废弃了，虽然还可以用，但是最好不要用。比较常见的情况是版本更新后，整个一大块代码会被抽掉。

```cpp
[[deprecated("Will remove in next release")]] void test() {}
void run{
    test();
}
```

使用上面的代码的时候，编译器就会给出警告

```text
In function 'void run()':
warning: 'void test()' is deprecated: Will remove in next release [-Wdeprecated-declarations]
```

## 4.3 `[[fallthrough]]`

```cpp
switch(something) {
    case SPECIAL_CASE:
        prepareForSpecialCase();
        [[fallthrough]]
    default:
        handleAllCases();
}
```

`[[fallthrough]]`放在一个`case`块的末尾，表明你是故意没写`break`语句。不然的话编译器可以给你一个warning，static analyzer和code reviewer也会警告你。

## 4.3 `[[maybe_unused]]`

```cpp
static void impl1() { ... } // Compilers may warn about this
[[maybe_unused]] static void impl2() { ... } // Warning suppressed

void foo() {
   int x = 42; // Compilers may warn about this
   [[maybe_unused]] int y = 42; // Warning suppressed
}
```

有时候你声明/定义了一个函数或者变量暂时还没有用到，编译器就会warning，你可以`[[maybe_unused]]`来supress这个warning。

## 5. 为名空间和枚举型添加attribute

```cpp
namespace [[deprecated]] old_stuff{
    void legacy();
}

old_stuff::legacy(); // Emits warning
```

我们也可以为名空间增加attribute，例如添加`[[deprecated]]`使得名空间中的所有function，type等都处于`[[deprecated]]`状态。

```cpp
enum Algorithm {
  FPM = 0,
  FP [[deprecated]] = FPM
};

Algorithm e = FP; // Emits warning
```

假设我们定义了一个`Algorithm`的枚举类型，其中一个可选值最初为`FP=0`，但后面想用`FPM`代替`FP`这个名字，就可以像上面这个代码这样改。

## 6. 忽略未知的属性

在C++17之前你可以使用一些编译器特定的attribute，但是当你用另外一个不支持这些attribute的编译器去编译你的代码的时候，你就会得到error。但是现在若你的编译器支持C++17,它就只会报一个warning或者都不报。

```cpp
// compilers which don't 
// support MyCompilerSpecificNamespace will ignore this attribute
[[MyCompilerSpecificNamespace::do_special_thing]] 
void foo();
```

例如，GCC 8.3 就只会报这样一个warning

```text
warning: 'MyCompilerSpecificNamespace::do_special_thing' scoped attribute directive ignored [-Wattributes]
void foo();
```



## 7. 使用using指令避免名空间的重复书写

```cpp
void f() {
    [[rpr::kernel, rpr::target(cpu,gpu)]] // repetition
    do-task();
}
```

当你使用多个同一名空间中的属性时，可以using指令简化代码

```cpp
void f() {
    [[using rpr: kernel, target(cpu,gpu)]]
    do-task();
}
```

## 8. Bjarne Stroustrup对attribute的使用建议

```text
There is a reasonable fear that attributes will be used to create language dialects. The recommendation is to use attributes to only control things that do not affect the meaning of a program but might help detect errors (e.g. [[noreturn]]) or help optimizers (e.g. [[carries_dependency]])
```

## 9. [[carries_dependency]]

`[[carries_dependency]]`涉及的知识点稍微多一点，所以我们放在最后讲。
