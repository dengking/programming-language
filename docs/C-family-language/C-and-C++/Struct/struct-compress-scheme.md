# struct压缩与解压方案

## 问题描述

`struct`存在padding，导致`struct`对象实际所占用的内存空间 大于 它的value所占用的内存空间。

> 参见：
>
> - [Object representation and value representation](https://en.cppreference.com/w/cpp/language/object#Object_representation_and_value_representation)
>
> ```c++
> struct S {
>     char c;  // 1 byte value
>              // 3 bytes padding (assuming alignof(float) == 4)
>     float f; // 4 bytes value (assuming sizeof(float) == 4)
>     bool operator==(const S& arg) const { // value-based equality
>         return c == arg.c && f == arg.f;
>     }
> };
> ```



UST系统使用`struct`来作为消息协议，所以实际发送的网络消息中也包含了padding这样的无用字节，进而导致网络包偏大。通过性能测试发现：网络包的大小对实际的网络通信时延有较大影响。所以尽可能地降低网络包的大小对于提高UST系统整体性能有所裨益。

## UST系统中struct的作用

struct作为protocol，描述接口，具备描述功能；

## 构思

### 压缩与解压

本方案提成一种**压缩**和**解压**struct object的方法：

发送方**压缩**struct object，思路：剔除struct object 中的padding bytes，仅仅发送：**struct object values**，发送方通过压缩struct object进而可以实现降低网络包的大小的目的。

接收方**解压**网络包，思路：根据**struct object values**构造struct object。

为了使上面描述的压缩和解压过程通用，而不是为每个struct都编写一次压缩、解压，需要借助RTTI来实现通用的压缩、解压。

### struct RTTI

RTTI即run-time type info，struct的RTTI需要包含如下信息：

- 字段个数

- 每个字段的类型、长度、offset

#### struct RTTI的生成

使用parser来解析定义struct的header file来自动生成struct对应的RTTI，将生成的RTTI保存到内存中。



