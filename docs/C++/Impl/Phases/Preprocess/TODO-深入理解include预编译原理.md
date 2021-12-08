# 20180318
原文链接：https://www.cnblogs.com/liushui-sky/p/6423553.html

# [深入理解include预编译原理](http://www.cnblogs.com/liushui-sky/p/6423553.html)

你了解 #include 某个 .h 文件后，**编译器**做了哪些操作么？ 你清楚为什么在 .h文件中定义函数实现的话需要在函数前面加上 **static** 修饰么？你知道 #ifndef……#define……#endif 这种防止头文件重复包含的精髓所在么？本文就是来探讨这些问题，并给出我的理解和思考，欢迎大家留言交流。

## 1. include 命令的作用

### 1.1  什么情况不使用 include

```c
//a.c文件 
 
void test_a() 
{ 
    return;  
} 
 
 
//b.c文件 
 
void test_a();  // 函数声明 
 
void test_b() 
{ 
    test_a();    // 由于上面已经声明了，所以可以使用 
}
```



其实，这样的工程，可以不用使用 **include 预编译命令**。

> 我刚刚在我的环境中写了一个小测试，发现在b.c中不声明函数test_a也是能够编译通过的，所以我需要思考的是编译器对函数声明的处理是什么？？

 

### 1.2  什么情况使用 include

如果工程里面的函数特别多，那么按照上面的做法，则必须在每一个 .c 文件的开头列出所有本文件调用过的**函数的声明**，这样很不高效，而且一旦某个函数的形式发生变化，又得一个一个改 .c 开头的函数声明。 

因此，#include 预编译命令诞生。

```c
//a.c文件 
 
void test_a() 
{ 
     return;  
} 
 
//a.h文件 
 
void test_a(); 
 
//b.c文件 
 
#include "a.h"    // 包含含有 test_a() 函数声明的头文件 
 
void test_b() 
{ 
    test_a();         
}
```

### 1.3  #include 起到什么效果

上述代码在编译器进行预编译的时候，**遇到 #include "a.h" ，则会把整个 a.h 文件都copy到 b.c 的开头**，因此，在实际编译 b.c 之前，b.c 已经被修改为了如下形式：



```c
//b.c 预编译后的临时文件 
 
void test_a(); 
 
void test_b() 
{ 
    test_a();         
}
```



由此可见，得到的效果和手动加 test_a() 函数声明时的效果相同。

> tips: 在Linux下，可以使用 gcc -E b.c 来查看预编译 b.c 后的效果。

 

## 2. static 关键词的使用

### 2.1  什么叫函数重复定义

我们经常会遇到报错，说变量或者函数重复定义。那么，在此，首先我举例说明一下什么叫**函数的重复定义**。

```c
//a.c文件 
 
void test() 
{ 
    return; 
} 
 
//b.c文件 
 
void test() 
{ 
    return; 
}
```

那么，在编译的时候是不会报错的，但是，在链接的时候，会出现报错：
multiple definition of `test'，因为在同一个工程里面出现了两个test函数的定义。

> 此处需要了解一下编译器编译的完整过程，因为编译器是分步骤进行编译的。先进行预编译，再进行编译，在进行汇编，在进行链接。

### 2.2  在.h里面写函数实现

如果在 .h 里面写了函数实现，会出现什么情况？

```c
//a.h文件 
 
void test_a() 
{ 
   return;     
} 
 
//b.c文件 
 
#include "a.h" 
 
void test_b() 
{ 
    test_a(); 
}
```



预编译后，会发现，b.c 被修改为如下形式：



```c 
//b.c 预编译后的临时文件 
 
void test_a() 
{ 
   return;     
} 
 
void test_b() 
{ 
    test_a(); 
}
```

当然，这样目前是没有什么问题的，可以正常编译链接成功。但是，如果有一个 c.c 也包含的 a.h 的话，怎么办？



```c
//c.c文件 
 
#include "a.h" 
 
void test_c() 
{ 
    test_a(); 
}
```



同上，c.c 在预编译后，也形成了如下代码：



```c
// c.c 预编译后的临时文件 
 
void test_a() 
{ 
    return;     
} 
 
void test_c() 
{ 
    test_a(); 
}
```

那么，在链接器进行链接（link）的时候，会报错：
```shell
multiple definition of `test_a'
```
因此，在 .h 里面写函数实现的弊端就暴露出来了。但是，经常会有这样的需求，将一个**函数**设置为 内联（inline） 函数，并且放在 .h 文件里面，那么，怎样才能防止出现上述 重复定义的报错呢？

> 这让我想起了static inline函数，现在看来这篇文章才知道这样进行声明的原因。

### 2.3  static 关键词

应对上面的情况，static关键词很好地解决了这个问题。

用static修饰函数，则表明该函数**只能**在**本文件**中使用，因此，当不同的文件中有**相同的函数名**被static修饰时，不会产生**重复定义**的报错。例如：

> TODO:上面对static的解释和我之前对static的认知是不相同的，所以需要在了解一下static的相关知识。

```c
//a.c文件 
 
static void test() 
{ 
    return; 
} 
 
void test_a() 
{ 
    test(); 
} 
 
//b.c文件 
 
static void test() 
{ 
    return; 
} 
 
void test_b() 
{ 
    test(); 
}
```

编译工程时不会报错，但是test()函数只能被 a.c 和 b.c 中的函数调用，不能被 c.c 等其他文件中的函数调用。
那么，用static修饰 .h 文件中定义的函数，会有什么效果呢？

```c
//a.h文件 
 
static void test() 
{ 
    return; 
} 
 
//b.c文件 
 
#include "a.h" 
 
void test_b() 
{ 
    test(); 
} 
 
//c.c文件 
 
#include "a.h" 
 
void test_c() 
{ 
    test(); 
}
```



这样的话，在预编译后，b.c 和 c.c 文件中，由于 #include "a.h" ，故在这两个文件开头都会定义 static void test() 函数，因此，test_b() 和 test_c() 均调用的是自己文件中的 static void test() 函数 ， 因此不会产生重复定义的报错。
因此，**结论，在 .h 文件中定义函数的话，建议一定要加上 static 关键词修饰，这样，在被多个文件包含时，才不会产生重复定义的错误**。

 

## 3. 防止头文件重复包含

经常写程序的人都知道，我们在写 .h 文件的时候，一般都会加上

```c
#ifndef    XXX 
#define   XXX  
…… 
#endif
```

这样做的目的是为了防止头文件的重复包含，具体是什么意思呢？
它不是为了防止多个文件包含某一个头文件，而是为了防止一个头文件被**同一个文件**包含**多次**。具体说明如下：



```c
//a.h文件 
 
static void test_a() 
{ 
    return; 
} 
 
//b.c文件 
 
#include "a.h" 
 
void test_b() 
{ 
    test_a(); 
} 
 
//c.c 
 
#include "a.h" 
 
void test_c() 
{ 
    test_a(); 
}
```

这样是没有问题的，但下面这种情况就会有问题。

```c
//a.h文件 
 
static void test_a() 
{ 
    return; 
} 
 
//b.h文件 
 
#include "a.h" 
 
//c.h文件 
 
#include "a.h" 
 
//main.c文件 
 
#include "b.h" 
#include "c.h" 
 
void main() 
{ 
    test_a(); 
}
```

这样就不小心产生问题了，因为 b.h 和 c.h 都包含了 a.h，那么，在预编译main.c 文件的时候，会展开为如下形式：

```c
//main.c 预编译之后的临时文件 
 
static void test_a() 
{ 
    return; 
} 
 
static void test_a() 
{ 
    return; 
} 
 
void main() 
{ 
    test_a(); 
}
```

在同一个 .c 里面，出现了两次 test_a() 的定义，因此，会出现重复定义的报错。
但是，如果在 a.h 里面加上了

```
#ifndef    XXX 
#define   XXX  
…… 
#endif
```

 的话，就不会出现这个问题了。
例如，上面的 a.h 改为：



```c
//a.h 文件 
 
#ifndef  A_H 
#define A_H 
 
static void test_a() 
{ 
    return; 
} 
 
#endif
```



预编译展开main.c则会出现：



```c
//main.c 预编译后的临时文件 
 
#ifndef A_H 
#define A_H 
 
static void test_a() 
{ 
    return; 
} 
 
#endif 
 
#ifndef A_H 
#define A_H 
 
static void test_a() 
{ 
    return; 
} 
 
#endif 
 
void main() 
{ 
    test_a(); 
}

```

在编译main.c时，当遇到第二个 #ifndef  A_H ，由于前面已经定义过 A_H，故此段代码被跳过不编译，因此，不会产生重复定义的报错。这就是 #ifndef……#define……#endif 的精髓所在。

转自：http://ticktick.blog.51cto.com/823160/596179/

参考：http://blog.csdn.net/softmanfly/article/details/41699511
