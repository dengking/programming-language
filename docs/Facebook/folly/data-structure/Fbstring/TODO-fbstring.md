# `fbstring`



```C++
/**
 * This is the core of the string. The code should work on 32- and
 * 64-bit and both big- and little-endianan architectures with any
 * Char size.
 *
 * The storage is selected as follows (assuming we store one-byte
 * characters on a 64-bit machine): (a) "small" strings between 0 and
 * 23 chars are stored in-situ without allocation (the rightmost byte
 * stores the size); (b) "medium" strings from 24 through 254 chars
 * are stored in malloc-allocated memory that is copied eagerly; (c)
 * "large" strings of 255 chars and above are stored in a similar
 * structure as medium arrays, except that the string is
 * reference-counted and copied lazily. the reference count is
 * allocated right before the character array.
 *
 * The discriminator between these three strategies sits in two
 * bits of the rightmost char of the storage:
 * - If neither is set, then the string is small. Its length is represented by
 *   the lower-order bits on little-endian or the high-order bits on big-endian
 *   of that rightmost character. The value of these six bits is
 *   `maxSmallSize - size`, so this quantity must be subtracted from
 *   `maxSmallSize` to compute the `size` of the string (see `smallSize()`).
 *   This scheme ensures that when `size == `maxSmallSize`, the last byte in the
 *   storage is \0. This way, storage will be a null-terminated sequence of
 *   bytes, even if all 23 bytes of data are used on a 64-bit architecture.
 *   This enables `c_str()` and `data()` to simply return a pointer to the
 *   storage.
 *
 * - If the MSb is set, the string is medium width.
 *
 * - If the second MSb is set, then the string is large. On little-endian,
 *   these 2 bits are the 2 MSbs of MediumLarge::capacity_, while on
 *   big-endian, these 2 bits are the 2 LSbs. This keeps both little-endian
 *   and big-endian fbstring_core equivalent with merely different ops used
 *   to extract capacity/category.
 */
```

一、"rightmost char of the storage" 的含义是什么？

```c++
  Category category() const {
    // works for both big-endian and little-endian
    return static_cast<Category>(bytes_[lastChar] & categoryExtractMask);
  }
```



二、对于small string，它的size只需要6 bits即可表示出来

三、对于如何区分small、medium、large，还没有搞清楚。

四、endianness

在大端中，MSB放在低地址，LSB放在高地址；

在小端中，MSB放在高地址，LSB放在低地址；

参见 baike [大小端模式](https://baike.baidu.com/item/%E5%A4%A7%E5%B0%8F%E7%AB%AF%E6%A8%A1%E5%BC%8F/6750542?fr=aladdin) ，其中给出的例子非常好，当使用序列化的时候，就需要考虑endianness。



## 内存模型

参考:

1、cnblogs [漫步Facebook开源C++库folly(1)：string类的设计](https://www.cnblogs.com/promise6522/archive/2012/06/05/2535530.html)

2、zhiqiang [字符串类的实现 folly::fbstring](https://zhiqiang.org/coding/folly-fbstring.html)

```C++
  struct MediumLarge {
    Char* data_;
    size_t size_;
    size_t capacity_;

    size_t capacity() const {
      return kIsLittleEndian ? capacity_ & capacityExtractMask : capacity_ >> 2;
    }

    void setCapacity(size_t cap, Category cat) {
      capacity_ = kIsLittleEndian
          ? cap | (static_cast<size_t>(cat) << kCategoryShift)
          : (cap << 2) | static_cast<size_t>(cat);
    }
  };

  union {
    uint8_t bytes_[sizeof(MediumLarge)]; // For accessing the last byte.
    Char small_[sizeof(MediumLarge) / sizeof(Char)];
    MediumLarge ml_;
  };
```

一、`size_t`、指针类型，长度都是8字节，因此，可以认为 `sizeof(struct MediumLarge) == 24`。

二、$2^8 = 256$，因此一个字节即可表示它的`size`。

三、其中 `kIsLittleEndian` 是在 [folly/Portability.h](https://gitee.com/mirrors/folly/blob/master/folly/Portability.h) 中定义的。

四、上述 `union` 显然是为了支持对 `MediumLarge` 的byte-by-byte access。

五、需要注意的是: `ml_` 是数据成员，并且是唯一的数据成员





在 cnblogs [漫步Facebook开源C++库folly(1)：string类的设计](https://www.cnblogs.com/promise6522/archive/2012/06/05/2535530.html) 中给出了示意图:

> NOTE: 
>
> fbstring内存模型示意图（使用[LucidChart](https://www.lucidchart.com/)绘制）：
> ![img](https://pic002.cnblogs.com/images/2012/337251/2012060423542240.png)





```c++
  constexpr static size_t lastChar = sizeof(MediumLarge) - 1; // 在 64 bit 机器上，为 23

constexpr static size_t maxSmallSize = lastChar / sizeof(Char);
  constexpr static size_t maxMediumSize = 254 / sizeof(Char);
  constexpr static uint8_t categoryExtractMask = kIsLittleEndian ? 0xC0 : 0x3;
  constexpr static size_t kCategoryShift = (sizeof(size_t) - 1) * 8;
  constexpr static size_t capacityExtractMask = kIsLittleEndian
      ? ~(size_t(categoryExtractMask) << kCategoryShift)
      : 0x0 /* unused */;
```



## category 

根据capacity来进行划分的。

24 `0001 1000`

256 `0100 0000`



### bit mask

```c++
 constexpr static uint8_t categoryExtractMask = kIsLittleEndian ? 0xC0 : 0x3;
```



```C++
0xC0: 11000000 小端
 0x3: 00000011 大端
```



### `category_type`



```C++
  typedef uint8_t category_type;

  enum class Category : category_type {
    isSmall = 0,
    isMedium = kIsLittleEndian ? 0x80 : 0x2,
    isLarge = kIsLittleEndian ? 0x40 : 0x1,
  };
```



```
isMedium = kIsLittleEndian ? 0x80(1000 0000) : 0x2(0000 0010),

isLarge = kIsLittleEndian ? 0x40(0100 0000) : 0x1(0000 0001),
```





|        |      |      |
| ------ | ---- | ---- |
| 23     |      |      |
| 24-256 |      |      |
| 257-   |      |      |

$2^5 = 32$

$2^6 = 64$

$2^7 = 128$

$2^8 = 256$

正好一个字节。



## `initSmall`

`ml` 的长度是3 word。

```C++
// Small strings are bitblitted
template <class Char>
inline void fbstring_core<Char>::initSmall(const Char* const data, const size_t size) 
{
  // Layout is: Char* data_, size_t size_, size_t capacity_
  static_assert(sizeof(*this) == sizeof(Char*) + 2 * sizeof(size_t), "fbstring has unexpected size"); // `ml_` 是唯一的数据成员，因此上述assertion是成立的
  static_assert(sizeof(Char*) == sizeof(size_t), "fbstring size assumption violation");
  // sizeof(size_t) must be a power of 2
  static_assert((sizeof(size_t) & (sizeof(size_t) - 1)) == 0, "fbstring size assumption violation");

// If data is aligned, use fast word-wise copying. Otherwise,
// use conservative memcpy. "conservative"的含义是"保守的"
// The word-wise path reads bytes which are outside the range of
// the string, and makes ASan unhappy, so we disable it when
// compiling with ASan.
#ifndef FOLLY_SANITIZE_ADDRESS
  // "reinterpret_cast<size_t>(data)"的前提是: sizeof(Char*) == sizeof(size_t) 
  if ((reinterpret_cast<size_t>(data) & (sizeof(size_t) - 1)) == 0) {
    const size_t byteSize = size * sizeof(Char); // size的单位是Char
    constexpr size_t wordWidth = sizeof(size_t);
    switch ((byteSize + wordWidth - 1) / wordWidth) { // Number of words.
      case 3:
        ml_.capacity_ = reinterpret_cast<const size_t*>(data)[2];
        FOLLY_FALLTHROUGH;
      case 2:
        ml_.size_ = reinterpret_cast<const size_t*>(data)[1];
        FOLLY_FALLTHROUGH;
      case 1:
        ml_.data_ = *reinterpret_cast<Char**>(const_cast<Char*>(data)); // 写法和上面的有些差异，它为什么这样写呢？这是因为 `ml_.data_` 的类型是 `Char*` ，而 `ml_.capacity_`、`ml_.size_` 的类型是 `size_t` 
        FOLLY_FALLTHROUGH;
      case 0:
        break;
    }
  } else
#endif
  {
    if (size != 0) {
      fbstring_detail::podCopy(data, data + size, small_);
    }
  }
  setSmallSize(size);
}
```

一、"If data is aligned, use fast word-wise copying"

这段话是难以理解的，要写搞清楚它，需要从下面两个方面入手:

1、"word-wise copying"的含义是什么？

下面就是"word-wise copying":

```C++
      case 3:
        ml_.capacity_ = reinterpret_cast<const size_t*>(data)[2];
        FOLLY_FALLTHROUGH;
      case 2:
        ml_.size_ = reinterpret_cast<const size_t*>(data)[1];
        FOLLY_FALLTHROUGH;
      case 1:
        ml_.data_ = *reinterpret_cast<Char**>(const_cast<Char*>(data));
        FOLLY_FALLTHROUGH;
      case 0:
        break;
```

[folly库 fbstring实现的疑问？ - ubsan的回答 - 知乎](https://www.zhihu.com/question/461029555/answer/1902221386)  中，有着非常好的总结:

> 所以 `ml_.capacity_` 只是表示高位的 8bytes 数据而已，这也太 tricky 了吧

2、这样做有什么风险？

这在 zhihu [folly库 fbstring实现的疑问？](https://www.zhihu.com/question/461029555) 中，进行了非常好的分析，这里不在赘述了。

 

### zhihu [folly库 fbstring实现的疑问？](https://www.zhihu.com/question/461029555)





## leeshine [漫谈C++ string（3）：FBString的实现](https://leeshine.github.io/2019/06/08/cpp-string3/)

> NOTE: 
>
> 这篇文章介绍地还不错，其中对"字符串的类型（small、medium、large）是存储在哪的？"解释地还不错
>
> 

由此可见，`basic_string`栈中最后一个字节的高两位被用来存储字符串类型，在短字符串中，该字节存储的是字符串的`size`，由于此时`size`最多为22，所以高两位是不需要的，所以可以用来存类别，而在中大型字符串中，这两位对应于`capacity`中的最高两位，现有主机的内存没有这么大，所以这高两位也不可能用到，因此可以用来存储字符串类型。由此可见，FBString对内存的利用控制得多精细。



```C++
// kCategoryShift = (sizeof(size_t) - 1) * 8;
    void setCapacity(size_t cap, Category cat) {
      capacity_ = kIsLittleEndian
          ? cap | (static_cast<size_t>(cat) << kCategoryShift)
          : (cap << 2) | static_cast<size_t>(cat);
    }
```

> NOTE: 
>
> 要将category 和 capacity一起编码放到`capacity_`字段中
>
> 对于big endian:
>
> ```C++
> (cap << 2) | static_cast<size_t>(cat);
> ```
>
> 它是非常简单的，它直接将`cap`左移动2 bit，然后将cat放到最低位



```C++
constexpr static size_t kCategoryShift = (sizeof(size_t) - 1) * 8;
```

> NOTE:
>
> `capacity_` 的类型就是 `size_t`
>
> `static_cast<size_t>(cat) << kCategoryShift`
>
> `isMedium = kIsLittleEndian ? 0x80 : 0x2,`
>
> ```
> 1000 0000,
> 0000 0000,0000 0000,0000 0000,0000 0000,0000 0000,0000 0000,0000 0000,1000 0000
> 1000 0000,0000 0000,0000 0000,0000 0000,0000 0000,0000 0000,0000 0000,0000 0000
> ```
>
> 



## Big endian、small endian

它非常好的体现了大小端对跨平台程序的影响



```c++
// kCategoryShift = (sizeof(size_t) - 1) * 8;
    void setCapacity(size_t cap, Category cat) {
      capacity_ = kIsLittleEndian
          ? cap | (static_cast<size_t>(cat) << kCategoryShift)
          : (cap << 2) | static_cast<size_t>(cat);
    }
```

上述code非常好地体现了big endian 和 little endian直接的差异:

1、big endian: `(cap << 2) | static_cast<size_t>(cat)` 最高MSB放在低地址，因为目前计算机系统中，它一般不会被用到，因此可以`<< 2`，从而将整体向左(低地址方向)移动了两位，从而将最高位的bit(LSB)给空出来了；对于 `static_cast<size_t>(cat)`，它的LSB在高地址，因此 `(cap << 2) | static_cast<size_t>(cat)` 就实现了将cat打到高地址的两位了。

2、little endian: 它的MSB在高地址，为空，



总结:

原则是放到MSB端，因为MSB端肯定是用不满的，因此可以充分地运用它来存储category。

对于big endian，因为它的MSB放在低地址，因此它的低地址是空闲的，对于little endian，因为它的MSB放在高地址，因此它的高地址是空闲的。

## Reference counting

`struct RefCounted`

一、有一个疑问:

1、`Char data_[1];`

为什么这样声明？

`data_`就是地址、指针，使用"数组名就是地址"来进行理解

那将它修改为如下方式有何不同？

`Char* data_`

能够节省一个指针的空间

二、它的实现方式是非常类似于 flexible array member，关于此参见 

1、creference [Struct declaration](https://en.cppreference.com/w/c/language/struct)

2、stackoverflow [Are flexible array members valid in C++?](https://stackoverflow.com/questions/4412749/are-flexible-array-members-valid-in-c) # [A](https://stackoverflow.com/a/4413035)

## Memory

`goodMallocSize` 是在 [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/master/folly)/[memory](https://github.com/facebook/folly/tree/master/folly/memory)/**[Malloc.h](https://github.com/facebook/folly/blob/master/folly/memory/Malloc.h)** 中定义的

`checkedMalloc` 是在 [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/master/folly)/[memory](https://github.com/facebook/folly/tree/master/folly/memory)/**[Malloc.h](https://github.com/facebook/folly/blob/master/folly/memory/Malloc.h)** 中定义的

