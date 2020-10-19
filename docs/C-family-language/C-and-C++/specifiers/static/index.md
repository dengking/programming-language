# 关于本章

本章描述在C和C++语言中都会涉及到的`static`，本章是一个专题总结。



## Static storage duration and keyword `static`

在C family language中，提及static，我们应该要有上述认知，即存在:

1) static storage duration，参见: `C++\Language-reference\Basic-concept\Data-model\Object\Storage-duration-and-lifetime.md`

2) keyword `static`，在下面会对它进行详细说明。

## 描述思路

在`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Storage-duration`的[Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)小节所总结的: 

> 无论是`C++`还是C，都没有专门描述linkage的specifier，而是将描述**storage duration**和描述**linkage**的specifier合并在一起，对于linkage，并没有单独描述它的specifier，但是，compiler提供了default linkage；关于这一点，我们需要仔细阅读cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration) 和 creference [Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)

因此，对于keyword `static`的讨论是离不开static storage duration和linkage的，下面是专门描述这两种属性的章节:

1) static storage duration: `C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Storage-duration`

2) linkage: `C-and-C++\From-source-code-to-exec\Link\Linkage`

## keyword  `static`用法总结

对于keyword  `static`的用法，可以采用如下两种分类方法: 

### 1) programming paradigm

|             | object               | function                 |
| ----------- | -------------------- | ------------------------ |
| **Non-OOP** | - static object      | - static function        |
| **OOP**     | - static data member | - static member function |

上面的分类方法是采用的在`C++\Language-reference\Basic-concept\index.md#Extend to OOP`中总结的思路:

> 首先描述Non-OOP，然后描述OOP

虽然上面将两者分开，但是实际编程中，可能会出现两者之的重叠：static object in static function。



#### Non-OOP

使用keyword `static`修饰的object，具备:

1) storage duration: static storage duration

2) linkage: internal linkage



使用keyword static修饰的function，具备:

1) storage duration: 在`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Storage-duration`中，已经讨论过了这个问题:

> 对于function而言，它没有**storage duration** property，只有**linkage** property，对于function而言，讨论它的storage duration是没有意义的

2) linkage: internal

在`./Static-function`中，对它进行了专门分析。

#### OOP

参见: `C++\Language-reference\Classes\Members\Static-member`。

### 2) cppreference [C++ keywords: static](https://en.cppreference.com/w/cpp/keyword/static)

下面是 cppreference [C++ keywords: static](https://en.cppreference.com/w/cpp/keyword/static) 总结的static的用法，我觉得总结得是比较好的。

|       | 用法                                                         | 说明                     |
| ----- | ------------------------------------------------------------ | ------------------------ |
| **1** | [declarations of namespace members with static storage duration and internal linkage](https://en.cppreference.com/w/cpp/language/storage_duration) | Non-OOP，namespace scope |
| **2** | [definitions of block scope variables with static storage duration and initialized once](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables) | Non-OOP，block scope     |
| 3     | [declarations of class members not bound to specific instances](https://en.cppreference.com/w/cpp/language/static) | 对应的就是1)中的OOP      |

和1)对比可以发现，其实2)中的1和2是对1)中的Non-OOP的细分；



## Static object

对于static object，需要考虑的问题有：

1) storage duration / lifetime

2) 唯一性



### Static local object in static function

对于function而言，添加`static`修饰，只能够改变其linkage。对于位于static function中的static local object，它的lifetime并不受static function的影响。

### 唯一性

#### Nifty counter idiom

参见`C++\Language-reference\Initialization\Initialization-of-Static-Variables`

#### Function-local static objects in inline function

在`cppreference Storage class specifiers#Static local variables`中对这个问题进行了说明:

> Function-local static objects in all definitions of the same [inline function](inline.html) (which may be implicitly inline) all refer to the same object defined in one translation unit.

TO READ:

https://stackoverflow.com/questions/185624/static-variables-in-an-inlined-function





### static virtual?

No，参见: https://stackoverflow.com/questions/1820477/c-static-virtual-members 

## Static in header file

对于static object 和 static function，如果将它们定义于header file中，则会产生一些列问题，这在`./Static-in-header-file.md`中进行了讨论。



## 相关内容

### Singleton pattern

参见: `C++\Pattern\Singleton`





