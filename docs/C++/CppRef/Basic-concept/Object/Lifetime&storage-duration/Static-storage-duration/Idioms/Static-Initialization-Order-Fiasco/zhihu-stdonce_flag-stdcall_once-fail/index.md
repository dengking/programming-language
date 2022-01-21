# zhihu [用std::once_flag来std::call_once，运行时却执行了2次？](https://zhuanlan.zhihu.com/p/408838350)

> NOTE: 
>
> 一、这篇文章的例子非常好的演示了static initialization order fiasco，并且它还给出了debug 方法

## 一、问题描述

**1.简化后的代码段（**[原始代码](https://link.zhihu.com/?target=https%3A//github.com/alibaba/MNN/blob/master/source/core/Backend.cpp%23L20)**）**

```c++
// File1.h
#include <iostream>
class A
{
public:
	A() {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
static A& GetExtraCreator();

```



```C++
// File1.cpp
#include <thread>
#include <mutex>
#include <iostream>
#include "File1.h"

static std::once_flag gInitFlag;

static A& GetExtraCreator()
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	static A* gExtraCreator;
	std::call_once(gInitFlag, [&]() { gExtraCreator = new A; });
	return *gExtraCreator;
}

```



```C++
// File2.cpp
#include "File1.h"
static bool gResistor = []() {
	A& obj_A = GetExtraCreator();
	return false;
}( );

int main()
{
	GetExtraCreator();
	GetExtraCreator();    
}

```

> NOTE: 
>
> 上述程序是典型的static initialization order fiasco:
>
> 在`File2.cpp`中定义的static variable `gResistor`，它的initialization是依赖于在 `File1.cpp` 中定义的static variable  `gInitFlag`。

不难看出，这是一个单例（**饿汉模式**）的思路：

1、`gExtraCreator`作为静态变量，存放`Class A`的对象地址。

2、`gInitFlag`也是静态变量，用来标记是否已经执行过`init`操作。

3、`gResistor`是一个静态变量，用来在`main`函数之前初始化相关资源。

4、`GetExtraCreator`作为对外接口，首次调用会执行`call_once`来初始化`gExtraCreator`，同时把`gInitFlag`置为非零。

5、后续再调用该接口时，判断到`gInitFlag`不为零，就不会再执行`call_once`，直接返回`gExtraCreator`。

**2.预期现象**

不管调用多少次`GetExtraCreator`，`call_once`都只执行一次。

**3.实际现象**

> **操作系统：Windows 10, 64 bit**
> **编译器：Microsoft Visual Studio 2017**
> **模式：Debug/Release**

->Debug模式：
**第二次调用GetExtraCreator，call_once执行了第二次。以后再调用GetExtraCreator，call_once也不再执行。**
->Release模式：
不管调用多少次`GetExtraCreator`，`call_once`都只执行一次。



## 二、背景知识

**1.Debug模式和Release模式的初始值**

Debug 版本：
`/MDd` `/MLd` 或 `/MTd` 使用 Debug runtime library(调试版本的运行时刻函数库)

`/Od` 关闭优化开关

`/D "_DEBUG" `相当于 `#define _DEBUG`,打开编译调试代码开关(主要针对assert函数)

`/ZI` 创建 Edit and continue(编辑继续)数据库，这样在调试过程中如果修改了源代码不需重新编译

`/GZ` 可以帮助捕获内存错误

`/Gm` 打开最小化重链接开关，减少链接时间

Release 版本：

`/MD` /ML 或 `/MT` 使用发布版本的运行时刻函数库

`/O1` 或 `/O2` 优化开关，使程序最小或最快

`/D "NDEBUG"` 关闭条件编译调试代码开关(即不编译`assert`函数)

`/GF` 合并重复的字符串，并将字符串常量放到只读内存，防止被修改



**2.静态变量的初始化顺序**

`Debug`模式在编译时，多了`/GZ`的flag。

`/GZ`的其中一个作用是：用 `0xCC` 初始化所有**自动变量**。`0xCD` ( Cleared Data ) 初始化堆中分配的内存（即动态分配的内存，例如 new ）

[为什么在C语言中静态变量的初值是0？](https://www.zhihu.com/question/49111720)

**2.静态变量的初始化顺序**

在C++标准['ISOIEC14882-1998 3.6.2 Initialization of nonlocal objects'](https://link.zhihu.com/?target=https%3A//cdn.standards.iteh.ai/samples/25845/dfc85a0e535a4c7c946fc811fc35132e/ISO-IEC-14882-1998.pdf)里：

1、在同一个编译单元内，静态变量初始化的顺序就是定义的顺序；

2、跨编译单元的静态变量的初始化顺序未定义，具体的初始化顺序取决于编译器的实现。

附blog：
[Static变量在Debug和Release模式下的初始化顺序偶有差异](https://link.zhihu.com/?target=https%3A//blog.csdn.net/sallay/article/details/6770441)
[C++静态变量的初始化](https://link.zhihu.com/?target=https%3A//www.jianshu.com/p/dd34cee5242c)

**3.initialization**



## 三、问题分析

#### 1、首先聚焦在Debug模式和Release模式现象不同上

1、根据背景知识1，对于**静态变量**，Debug和Release模式的初始化值是一样的，都为0，所以和初始值无关。

2、根据背景知识2，Debug和Release模式下的初始化顺序有差异，可以单步调试看看。

#### 2、其次单步调试，重点关注静态变量的初始化

1、在`gInitFlag`上打断点，添加**监视**，观察值第一次改变的时机。

2、在`gInitFlag`值第一次被修改后，设置**改变时自动触发中断**。

3、继续运行，触发断点，查看调用堆栈，提示执行了**dynamic initializer**。

#### 3、最后分析为何会执行dynamic initializer

## 四、结论

#### 1.问题

1、VS错误的把**静态变量**的初始化置为了动态，使得`gInitFlag`初始化的顺序落后于`gResistor`。

2、两个**静态变量**处在不同的文件中，导致**初始化顺序**不固定。如果`gResistor`初始化在前，先置位了`gInitFlag`，接着`gInitFlag`初始化，又被重新清零，导致下次调用时又要重新置位。

3、Release模式之所以没问题，推测是`gResistor`的初始化顺序被安排在了`gInitFlag`之后，并没有被清零。

#### 2.修改办法

1、把`gInitFlag`放到`GetExtraCreator()`中。

2、因为VS对**静态局部变量**的初始化是正确的，所以不会执行**动态初始化**。

```C++
static A & GetExtraCreator()
{
    static std::once_flag gInitFlag;
    static A * gExtraCreator;
    std::call_once(gInitFlag, [&]() { gExtraCreator = new A; });
    return *gExtraCreator;
}
```

## 评论

我感觉你结论错了。。运行两次是因为**跨文件初始化顺序**

register初始化的时候执行了一次

regiater执行完后，`once_flag`又被初始化了一次

才导致的call once可以被调用两次

您说得对，我结论里描述的因果关系不准确。我修改一下

1、根本原因是初始化顺序不严谨导致flag有被清除的风险。

2、“静态变量的初始化置为了动态”这个bug即使修复了，也是避免不了这个问题的。