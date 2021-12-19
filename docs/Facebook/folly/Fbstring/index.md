# FBstring



# zhihu 张雅宸 [C++ folly库解读（一） Fbstring —— 一个完美替代std::string的库](https://zhuanlan.zhihu.com/p/348614098)

在引入**[fbstring](https://link.zhihu.com/?target=https%3A//github.com/facebook/folly/blob/master/folly/docs/FBString.md)**之前，我们首先再回顾一下 string 常见的三种实现方式。

## string 常见的三种实现方式



### eager copy



### COW



### SSO



## Fbstring 介绍

### Storage strategies

### Implementation highlights



## Benchmark

在**[FBStringBenchmark.cpp](https://link.zhihu.com/?target=https%3A//github.com/facebook/folly/blob/master/folly/test/FBStringBenchmark.cpp)**中。



## 主要类

![img](https://pic1.zhimg.com/80/v2-3150b44077cbdaad89a6a7715f492388_720w.jpg)

```C++
template <
    typename E,
    class T = std::char_traits<E>,
    class A = std::allocator<E>,
    class Storage = fbstring_core<E>>
class basic_fbstring;
```

`fbstring_core` 负责字符串的存储及字符串相关的操作，例如 `init`、`copy`、`reserve`、`shrink` 等等。

## 字符串存储数据结构





```C++
  constexpr static size_t lastChar = sizeof(MediumLarge) - 1;
  constexpr static size_t maxSmallSize = lastChar / sizeof(Char);
```

size : 利用 small_的一个字节来存储 size，**而且具体存储的不是 size，而是`maxSmallSize - s`**（maxSmallSize=23，再转成 char 类型），因为这样可以 SSO 多存储一个字节，具体原因后面详细讲。

> NOTE: 
>
> **`maxSmallSize - s`** 中的`s`指的是`size`



### small strings :

​	

![img](https://pic4.zhimg.com/80/v2-ed0b6ace553c33d63a0b24fd8be505e7_720w.jpg)

### medium strings :

![img](https://pic4.zhimg.com/80/v2-e847108e5f7bad48b66e963e6c6cc4d7_720w.jpg)

### large strings :

![img](https://pic1.zhimg.com/80/v2-53eaf77c4a8fc0dc1a53d094b8047dc4_720w.jpg)

## 如何区分字符串类型 category

字符串的 small/medium/large 类型对外部透明，而且针对字符串的各种操作例如 `copy`、`shrink`、`reserve`、赋值等等，三种类型的处理方式都不一样，**所以，我们需要在上面的数据结构中做些“手脚”，来区分不同的字符串类型。**

因为只有三种类型，所以只需要 2 个 bit 就能够区分。相关的数据结构为：

```cpp
typedef uint8_t category_type;

enum class Category : category_type {
    isSmall = 0,
    isMedium = kIsLittleEndian ? 0x80 : 0x2,       //  10000000 , 00000010
    isLarge = kIsLittleEndian ? 0x40 : 0x1,        //  01000000 , 00000001
};
```

`kIsLittleEndian` 为判断当前平台的大小端，大端和小端的存储方式不同。

### small strings

> NOTE: 
>
> category占用2 bit
>
> size最大为23，因此只需要4 bit即可



## 字符串初始化

首先 `fbstring_core` 的构造函数中，根据字符串的长度，调用 3 种不同类型的初始化函数：

```cpp
fbstring_core(
    const Char* const data,
    const size_t size,
    bool disableSSO = FBSTRING_DISABLE_SSO) {
  if (!disableSSO && size <= maxSmallSize) {
    initSmall(data, size);
  } else if (size <= maxMediumSize) {
    initMedium(data, size);
  } else {
    initLarge(data, size);
  }
}
```

### initSmall



### initMedium



### initLarge



## 特殊的构造函数 —— 不拷贝用户传入的字符串
