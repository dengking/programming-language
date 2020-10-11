# 关于本章

本章描述在C和C++语言中都会涉及到的`static`，本章是一个专题总结。



keyword `static` 

static storage duration

## `static`用法总结

对于`static`的用法，可以采用如下两种分类方法: 

### 1) programming paradigm

|             | object               | function                 |
| ----------- | -------------------- | ------------------------ |
| **Non-OOP** | - static object      | - static function        |
| **OOP**     | - static data member | - static member function |

上面的分类方法是采用的在`C++\Language-reference\Basic-concept\index.md#Extend to OOP`中总结的思路:

> 首先描述Non-OOP，然后描述OOP

虽然上面将两者分开，但是实际编程中，可能会出现两者之的重叠：static object in static function。

### 2) cppreference [C++ keywords: static](https://en.cppreference.com/w/cpp/keyword/static)

下面是 cppreference [C++ keywords: static](https://en.cppreference.com/w/cpp/keyword/static) 总结的static的用法，我觉得总结得是比较好的。

|       | 用法                                                         | 说明                     |
| ----- | ------------------------------------------------------------ | ------------------------ |
| **1** | [declarations of namespace members with static storage duration and internal linkage](https://en.cppreference.com/w/cpp/language/storage_duration) | Non-OOP，namespace scope |
| **2** | [definitions of block scope variables with static storage duration and initialized once](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables) | Non-OOP，block scope     |
| 3     | [declarations of class members not bound to specific instances](https://en.cppreference.com/w/cpp/language/static) | 对应的就是1)中的OOP      |

和1)对比可以发现，其实2)中的1和2是对1)中的Non-OOP的细分；

## 相关章节

- `C++\Language-reference\Basic-concept\Data-model\Object\Object-storage-duration-and-lifetime.md`
- `C++\Language-reference\Initialization\Initialization-of-Static-Variables`



## static function

对于static function，需要考虑的问题有：

- linkage

### What is static function

首先描述static function的概念。

https://stackoverflow.com/questions/558122/what-is-a-static-function-in-c

### static function in header



https://stackoverflow.com/questions/780730/c-c-static-function-in-header-file-what-does-it-mean



### static inline function in header

https://stackoverflow.com/questions/47819719/static-inline-functions-in-a-header-file

https://stackoverflow.com/questions/22102919/static-vs-inline-for-functions-implemented-in-header-files?noredirect=1

https://stackoverflow.com/questions/12836171/difference-between-an-inline-function-and-static-inline-function

## static object

对于static object，需要考虑的问题有：

- storage duration / lifetime
- initialization
- 唯一性






### c++ static const members

https://cookierobotics.com/032/

https://stackoverflow.com/questions/3531060/how-to-initialize-a-static-const-member-in-c

https://en.cppreference.com/w/cpp/language/static#Constant_static_members

https://stackoverflow.com/questions/29822181/prevent-static-initialization-order-fiasco-c

https://stackoverflow.com/questions/12247912/extern-vs-singleton-class


https://isocpp.org/wiki/faq/ctors#static-init-order


https://isocpp.org/wiki/faq/ctors#construct-on-first-use-v2


https://stackoverflow.com/questions/14495536/how-to-initialize-const-member-variable-in-a-class

### Initialization

参见`C++\Language-reference\Initialization\Initialization-of-Static-Variables`



### static local object in static function



### 唯一性

#### Nifty counter idiom

参见`C++\Language-reference\Initialization\Initialization-of-Static-Variables`

#### Function-local static objects in inline function

在`cppreference Storage class specifiers#Static local variables`中对这个问题进行了说明:

> Function-local static objects in all definitions of the same [inline function](inline.html) (which may be implicitly inline) all refer to the same object defined in one translation unit.

TO READ:

https://stackoverflow.com/questions/185624/static-variables-in-an-inlined-function



## static member

参见: `C++\Language-reference\Classes\Members\Static-member`。



### static virtual?

No，参见: https://stackoverflow.com/questions/1820477/c-static-virtual-members 



## TODO

### singleton pattern

参见: `C++\Pattern\Singleton`