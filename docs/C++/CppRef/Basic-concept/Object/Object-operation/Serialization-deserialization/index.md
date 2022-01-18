# Serialization and deserialization

序列化与反序列化。按照C++中的说法，deserialization也可以叫做reinterpret。

| 概念            | 解释                                                | 需要考虑的问题                                               |
| --------------- | --------------------------------------------------- | ------------------------------------------------------------ |
| serialization   | 给定一个object，得到它的object representation       | 一般使用**byte type**                                        |
| deserialization | 给定一个memory region，按照指定type进行interpretion | - memory address是否满足type的[Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment) requirement<br>- [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing) |

C++中，serialization and deserialization都是可以通过`reinterpret_cast`来实现的。

从上面可以看出，这些内容是密切相关的：

- deserialization
- `reinterpret_cast`
- [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)
- [Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment) 

可以这样来总结它们之间的关联：在C++中，一般通过`reinterpret_cast`来进行deserialization，在进行deserialization的时候，需要考虑[Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)，strict aliasing的目的是：满足type的[Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment) requirement。



参见：

- byte type：`C-family-language\C++\Language-reference\Basic-concept\Type-system\Type-system\Byte-type.md`
- strict aliasing：`C-family-language\C-and-C++\Pointer-array-alias\Alias`





## TODO

https://isocpp.org/wiki/faq/serialization

https://stackoverflow.com/questions/234724/is-it-possible-to-serialize-and-deserialize-a-class-in-c