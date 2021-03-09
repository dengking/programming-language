# `size_t`

## `size_t` is an abstraction

1、`size_t` 表达的是size、符合intentional programming

2、它要求的是能够容纳下所有的size，具体的implementation，由implementation决定

3、`size_t`"提高代码的可移植性、有效性或者可读性，或许同时提高这三者"，参见 jeremybai [为什么size_t重要？（Why size_t matters）](https://jeremybai.github.io/blog/2014/09/10/size-t) 。



## creference [`size_t`](https://en.cppreference.com/w/c/types/size_t)



## stackoverflow [What is size_t in C?](https://stackoverflow.com/questions/2550774/what-is-size-t-in-c)



## [A](https://stackoverflow.com/a/2550799)

[From Wikipedia](http://en.wikipedia.org/wiki/Size_t):

> According to the 1999 ISO C standard (C99), `size_t` is an unsigned integer type of at least 16 bit (see sections 7.17 and 7.18.3).
>
> `size_t`is an unsigned data type defined by several C/C++ standards, e.g. the C99 ISO/IEC 9899 standard, that is defined in `stddef.h`.[1](http://en.wikipedia.org/wiki/Size_t) It can be further imported by inclusion of `stdlib.h` as this file internally sub includes `stddef.h`.
>
> This type is used to represent the size of an object. Library functions that take or return sizes expect them to be of type or have the return type of `size_t`. Further, the most frequently used compiler-based operator `sizeof` should evaluate to a constant value that is compatible with`size_t`.

As an implication, `size_t` is a type guaranteed to hold any **array index**.



## cplusplus [size_t](http://www.cplusplus.com/reference/cstring/size_t/)

Unsigned integral type

Alias of one of the fundamental unsigned integer types.

It is a type able to represent the size of any object in bytes: size_t is the type returned by the `sizeof` operator and is widely used in the standard library to represent sizes and counts.

In `<cstring>`, it is used as the type of the parameter *num* in the functions [memchr](http://www.cplusplus.com/memchr), [memcmp](http://www.cplusplus.com/memcmp), [memcpy](http://www.cplusplus.com/memcpy), [memmove](http://www.cplusplus.com/memmove), [memset](http://www.cplusplus.com/memset), [strncat](http://www.cplusplus.com/strncat), [strncmp](http://www.cplusplus.com/strncmp), [strncpy](http://www.cplusplus.com/strncpy) and [strxfrm](http://www.cplusplus.com/strxfrm), which in all cases it is used to specify the maximum number of bytes or characters the function has to affect.

It is also used as the return type for [strcspn](http://www.cplusplus.com/strcspn), [strlen](http://www.cplusplus.com/strlen), [strspn](http://www.cplusplus.com/strspn) and [strxfrm](http://www.cplusplus.com/strxfrm) to return sizes and lengths.







## jeremybai [为什么size_t重要？（Why size_t matters）](https://jeremybai.github.io/blog/2014/09/10/size-t)

之前在《内存拷贝的注意事项》一文中提到过size_t，可能许多人对这个类型不太熟悉没有用过或者根本不敢去用，最近看到一篇文章对这个类型讲的比较详细，便翻译过来让不熟悉的同学可以知道它产生的原因以及如何使用。
　　原文地址： [Why size_t matters](http://web.archive.org/web/20101209143037/http://www.eetimes.com/discussion/programming-pointers/4026076/Why-size-t-matters?)

---



前言：使用`size_t`可能会提高代码的可移植性、有效性或者可读性，或许同时提高这三者。

在标准C库中的许多函数使用的参数或者返回值都是表示的用字节表示的**对象大小**，比如说`malloc(n)` 函数的参数n指明了需要申请的空间大小，还有`memcpy(s1, s2, n)`的最后一个参数，表明需要复制的内存大小，`strlen(s)`函数的返回值表明了以`’\0’`结尾的字符串的长度（不包括`’\0’`），其返回值并不是该字符串的实际长度，因为要去掉`’\0’`。

　　或许你会认为这些参数或者返回值应该被申明为int类型（或者long或者unsigned），但是事实上并不是。C标准中将他们定义为`size_t`。标准中记载`malloc`的申明应该出现在，定义为：

```C
void *malloc(size_t n);
```

　　`memcpy`和`strlen`的申明应该出现在中：

```C
void *memcpy(void *s1, void const *s2, size_t n);
size_t strlen(char const *s);
```

　　size_t还经常出现在C++标准库中，此外，C++库中经常会使用一个相似的类型size_type，用的可能比size_t还要多。
　　据我所知，大部分的C和C++程序员害怕这些库使用size_t，因为他们不知道size_t代表什么或者为什么这些库需要使用它，归根结底，原因在于他们什么时候什么地方需要用到它。

### 可移植性问题

　　早期的C语言（由Brian Kernighan 和 Dennis Ritchie 在The C Programming Language书中所写，Prentice-Hall, 1978）并没有提供size_t类型，C标准委员会为了解决移植性问题将size_t引入，举例如下：
　　让我们来写一个可移植的标准memcpy函数，我们将会看到一些不同的申明和它们在不同平台不同大小的地址空间上编译下的情况。
　　回忆memcpy(s1, s2, n)函数，它将s2指向地址开始的n个字节拷贝到s2指向的地址，返回s1，这个函数可以拷贝任何数据类型，所以参数和返回值的类型应该为可以指向任何类型的`void*`，同时，源地址不应该被改变，所以第二个参数s2类型应该为`const void*`，这些都不是问题。
　　真正的问题在于我们如何申明第三个参数，它代表了源对象的大小，我相信大部分程序员都会选择int：

```
void *memcpy(void *s1, void const *s2, int n);
```

　　使用int类型在大部分情况下都是可以的，但是并不是所有情况下都可以。int是有符号的，它可以表示负数，但是，大小不可能是复数。所以我们可以使用unsigned int代替它让第三个参数表示的范围更大。
　　在大部分机器上，unsigned int的最大值要比int的最大值大两倍，比如说再也给16位的机器上，unsigned int的最大值为65535，int的最大值为32767。
　　尽管int类型的大小依赖于C编译器的实现，但是在给定的平台上int对象的大小和unsigned int对象的大小是一样的。因此，使用unsigned int修饰第三个参数的代价与int是相同的：

```
void *memcpy(void *s1, void const *s2, unsigned int n);  
```

　　这样似乎没有问题了，unsigned int可以表示最大类型的对象大小了，这种情况只有在整形和指针类型具有相同大小的情况下，比如说在IP16中，整形和指针都占2个字节（16位），而在IP32上面，整形和指针都占4个字节（32位）。（参见下面C数据模型表示法）

```
C数据模型表示法
　　最近，我偶然发现几篇文章，他们使用简明的标记来表述不同目标平台下c语言数据的实现。我还没有找到这个标记的来源，正式的语法，甚至连名字都没有，但他似乎很简单，即使没有正规的定义也可以很容易使用起来。这些标记的一边形式形如：
　　I nI L nL LL nLL P nP。   
　　其中每个大写字母（或成对出现）代表一个C的数据类型，每一个对应的n是这个类型包含的位数。I代表int，L代表long，LL代表long long，以及P代表指针（指向数据，而不是函数）。每个字母和数字都是可选的。   
　　例如，I16P32架构支持16位int和32位指针类型，没有指明是否支持long或者long long。如果两个连续的类型具有相同的大小，通常省略第一个数字。例如，你可以将I16L32P32写为I16LP32，这是一个支持16位int，32位long，和32位指针的架构。  
　　标记通常把字母分类在一起，所以可以按照其对应的数字升序排列。例如，IL32LL64P32表示支持32位int，32位long，64位long long和32位指针的架构；然而，通常写作ILP32LL64。  
```

　　不幸的是，这种memcpy的申明在I16LP32架构上（整形是16-bit 长整形和指针类型时32-bits）显得不够用了，比如说摩托罗拉第一代处理器68000，在这种情况下，处理器可能拷贝的数据大于65535个字节，但是这个函数第三个参数n不能处理这么大的数据。
　　什么？你说很容易就可以改正？只需要把`memcpy`的第三个参数的类型修改一下：

```
void *memcpy(void *s1, void const *s2, unsigned long  n); 
```

　　你可以在I16LP32目标架构上使用这个函数了，它可以处理更大的数据。而且在IP16和IP32平台上效果也还行，说明它确实给出了memcpy的一种移植性较好的申明。但是，在IP16平台上相比于使用`unsigned int`，你使用`unsigned long`可能会使你的代码运行效率大打折扣（代码量变大而且运行变慢）。
　　在标准C中规定，长整形（无论无符号或者有符号）至少占用32位，因此在IP16平台上支持标准C的话，那么它一定是IP16L32 平台。这些平台通常使用一对16位的字来实现32位的长整形。在这种情况下，移动一个长整形需要两条机器指令，每条移动一个16位的块。事实上，这个平台上的大部分的32位操作都需要至上两条指令。
　　因此，以可移植性为名将memcpy的第三个参数申明为unsigned long而降低某些平台的性能是我们所不希望看到的。使用size_t可以有效避免这种情况。
　　size_t类型是一个类型定义，通常将一些无符号的整形定义为size_t，比如说unsigned int或者unsigned long，甚至unsigned long long。每一个标准C实现应该选择足够大的无符号整形来代表该平台上最大可能出现的对象大小。

### 使用`size_t`

　　`size_t`的定义在`<stddef.h>, <stdio.h>, <stdlib.h>, <string.h>, <time.h>和<wchar.h>`这些标准C头文件中，也出现在相应的`C++`头文件, 等等中，你应该在你的头文件中至少包含一个这样的头文件在使用size_t之前。 　　包含以上任何C头文件（由C或C++编译的程序）表明将size_t作为全局关键字。包含以上任何C++头文件（当你只能在C++中做某种操作时）表明将size_t作为std命名空间的成员。 　　根据定义，size_t是sizeof关键字（注：sizeof是关键字，并非运算符）运算结果的类型。所以，应当通过适当的方式声明n来完成赋值：

```
n = sizeof(thing);
```

　　考虑到可移植性和程序效率，n应该被申明为size_t类型。类似的，下面的foo函数的参数也应当被申明为sizeof：

```
foo(sizeof(thing));
```

　　参数中带有size_t的函数通常会含有局部变量用来对数组的大小或者索引进行计算，在这种情况下，size_t是个不错的选择。 
　　适当地使用size_t还会使你的代码变得如同自带文档。当你看到一个对象声明为size_t类型，你马上就知道它代表**字节大小**或**数组索引**，而不是错误代码或者是一个普通的算术值。 
　　在我接下来的一些文章的例子中会使用size_t，敬请期待！