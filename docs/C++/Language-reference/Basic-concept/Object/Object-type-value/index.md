# Object、type、value

本节讨论object、type、value。

## Object and value

需要搞清楚object 和 value 这两个概念，相关章节如下:

1、`Enum`章节

enumerator 是 value 而不是 object。

2、`cppreference-object` 章节

## Type determines everything

在"Object property"节中描述了object的属性，很多都是由type决定的，这验证了“Type determines everything”（参见`Theory\Type-system\index.md`）。

这些属性之间是存在着一定的关系的，下面对此进行说明

### Type determines the interpretion of memory representation, and further determine value

**type** 决定了 **interpretation** of **memory representation**，进而决定了 **value**。关于这个观点，在cppreference [reinterpret_cast conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)中的描述是可以佐证的：

> Converts between **types** by **reinterpreting** the underlying **bit** pattern.

在工程hardware的，`CPU\Endianess\Endianness`中，我们将会进一步看到，“interpretion of memory representation”还涉及到endian，这在大多数情况下，programmer无需关注endian。



#### draft: Object、type、value

关于三种的关系，在akrzemi1 [Value semantics](https://akrzemi1.wordpress.com/2012/02/03/value-semantics/)中有一定的阐述；

**Object**的是标准中定义的一个抽象概念，它是比较底层的，它对应的是memory、storage，在实际交流、表述中，我们更多地是使用value，即value semantic，value是比object更加具体一些的概念，它能够表达type，并且对于实际的application，我们关注的是value，而不是像object这样非常底层的；[如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118)就是一个典型的例子；这也是为什么：Value semantic is default；



#### Aliase to an existing object

C++、C非常灵活，对于同一个object，允许

- 使用多种type进行interpret，即reinterpret（重解释），一般通过pointer + `rereinterpret_cast`来实现
- 使用多个name进行refer to，一般通过reference来实现

对于aliase to an existing object，C++、C标准都进行了严格的定义，这在strict aliasing中进行了详细介绍。如果programmer不遵循strict aliasing，则会导致undefined behavior。

C++中，reinterpret由`rereinterpret_cast`来实现，所它将strict aliasing的内容和`rereinterpret_cast`放到了一起。

需要注意的是：上述两种方式，都仅仅是alias，都不会重新创建一个新的object（原object的副本）。

参见：

- alias: `C++\Language-reference\Alias`
- `rereinterpret_cast`: `C++\Language-reference\Basic-concept\Type-system\Type-conversion\Cast-operator`
- reference: `C++\Language-reference\Reference`
- pointer: `C-family-language\C-and-C++\Pointer-array\Pointer`

### Type determines size and alignment

type决定了object的size、alignment；

