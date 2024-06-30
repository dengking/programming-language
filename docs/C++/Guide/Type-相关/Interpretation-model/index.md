# Interpretion model

1. [ISA](https://en.wikipedia.org/wiki/Instruction_set_architecture) serves as the [interface](https://en.wikipedia.org/wiki/Interface_(computing)) between [software](https://en.wikipedia.org/wiki/Software) and [hardware](https://en.wikipedia.org/wiki/Computer_hardware). 

2. 在advanced programming language中有着严格的type system

3. 那么advanced programming language中的type和机器指令之间的对应关系是什么呢？

   

本文就对这个问题进行探讨:

在硬件级别，一切都是`01`，没有在高级programming language中的各种概念，比如type。在硬件级别，通过instruction来决定对`01`数据进行何种操作，比如:

1) **加法指令**决定了`01`数据进行加法运算

2) **浮点运算指令**决定了`01`数据进行浮点运算

在高级programming language中，有type的概念，compiler根据type来汇编生成instruction:

1) compiler根据type来决定对应的object的memory size，进而决定storage allocation

2) 根据type来决定使用何种运算指令

3) type决定了数据存储的格式、运算器; CPU对于不同类型的数据采用不同的存储方式([Computer number format](https://en.wikipedia.org/wiki/Computer_number_format))、使用不同的运算单元([ALU](https://en.wikipedia.org/wiki/Arithmetic_logic_unit)、[FPU](https://en.wikipedia.org/wiki/Floating-point_unit));显然，对于浮点类型，compiler会选择浮点运算指令，对于整形，则会选择简单指令。

上述分析非常重要 ，它是理解C，C++中pointer conversion、aliasing、type punning的基础，这个model非常重要，它可以帮助分析：

- object representation

- union

- aliasing



我将此作成为**interpretation model**，这个model非常重要，它融会贯通了如下内容:

- 高级programming language
- compiler
- CPU

## wikipedia [Type system](https://en.wikipedia.org/wiki/Type_system) 中关于"interpretion model"的内容

关于interpretation model，在wikipedia [Type system](https://en.wikipedia.org/wiki/Type_system)中有着解释:

### wikipedia [Type system](https://en.wikipedia.org/wiki/Type_system) # [Usage overview](https://en.wikipedia.org/wiki/Type_system#Usage_overview)

A [compiler](https://en.wikipedia.org/wiki/Compiler) may also use the static type of a value to optimize the storage it needs and the choice of algorithms for operations on the value. In many [C](https://en.wikipedia.org/wiki/C_(programming_language)) compilers the *float* [data type](https://en.wikipedia.org/wiki/Data_type), for example, is represented in 32 [bits](https://en.wikipedia.org/wiki/Bit), in accord with the [IEEE specification for single-precision floating point numbers](https://en.wikipedia.org/wiki/IEEE_754-2008). They will thus use floating-point-specific [microprocessor operations](https://en.wikipedia.org/wiki/Instruction_set) on those values (floating-point addition, multiplication, etc.).

> NOTE: 
>
> 一、这段话以float类型的数据为例来说明：编译器在汇编阶段可以根据value的static type来优化对该value的存储以及选择处理该value的指令，简而言之: compiler根据static type来决定:
>
> 1) storage
>
> 2) "the choice of algorithms for operations on the value"，其实就是选择合适的instruction。

### wikipedia [Type system](https://en.wikipedia.org/wiki/Type_system) # [Fundamentals](https://en.wikipedia.org/wiki/Type_system#Fundamentals)

Assigning a data type, termed *typing*, gives meaning to a sequence of [bits](https://en.wikipedia.org/wiki/Bit) such as a value in [memory](https://en.wikipedia.org/wiki/Computer_memory) or some [object](https://en.wikipedia.org/wiki/Object_(computer_science)) such as a [variable](https://en.wikipedia.org/wiki/Variable_(computer_science)). The hardware of a [general purpose computer](https://en.wikipedia.org/wiki/General_purpose_computer) is unable to discriminate(区分、辨别) between for example a [memory address](https://en.wikipedia.org/wiki/Memory_address) and an [instruction code](https://en.wikipedia.org/wiki/Instruction_code_(programming)), or between a [character](https://en.wikipedia.org/wiki/Character_(computing)), an [integer](https://en.wikipedia.org/wiki/Integer), or a [floating-point number](https://en.wikipedia.org/wiki/Floating-point_number), because it makes no intrinsic distinction between any of the possible values that a sequence of bits might *mean*.[[note 1\]](https://en.wikipedia.org/wiki/Type_system#cite_note-Burroughs-4) Associating a sequence of bits with a type conveys that [meaning](https://en.wiktionary.org/wiki/meaning) to the programmable hardware to form a *[symbolic system](https://en.wikipedia.org/wiki/Symbolic_system)* composed of that hardware and some program.

> NOTE: 
>
> Type决定了sequence of bits的含义。





## wikipedia [Memory address # Contents of each memory location](https://en.wikipedia.org/wiki/Memory_address#Contents_of_each_memory_location)

See also: [binary data](https://en.wikipedia.org/wiki/Binary_data)

Each memory location in a [stored-program computer](https://en.wikipedia.org/wiki/Stored-program_computer) holds a [binary number](https://en.wikipedia.org/wiki/Binary_number) or [decimal number](https://en.wikipedia.org/wiki/Decimal_number) *of some sort*. Its interpretation, as data of some [data type](https://en.wikipedia.org/wiki/Data_type) or as an instruction, and use are determined by the [instructions](https://en.wikipedia.org/wiki/Instruction_(computer_science)) which retrieve and manipulate it.

> NOTE: 上面这段话，对interpretation model的总结是非常好的。



## chinaunix [一直有个疑问是关于数据类型的](http://bbs.chinaunix.net/thread-3996911-1-1.html) 

高级语言中有**数据类型**，高级语言翻译成**机器指令**没有**数据类型**之说。比如说一条指令是访问一个内存地址也就是高级语言中的一个变量，那`cpu`怎么知道这个地址以后的几个字节都是属于这个变量的呢？也就是**数据类型**在**机器语言**级别是如何实现的？是不是有额外的表来记录程序每个需要用到的地址是几个字节，这个问题一直困扰着我，还请高人能解答。



> NOTE: 编译器根据对于不同的数据类型选择不同的机器指令；

### A

每条指令都包含着到底要运算、搬移具体到多少个字节的信息

### A

可以这么说，但也不全是，大家支持的很微妙。简单说，大多数RISC处理器在数据load的时候，可以通过**指令**区分load位宽和符号（0扩展或者符号扩展），但对于**数据处理指令**则并不一定区分，典型的`add指令`就不会区分**数据类型**。

**类型信息**并不会被显示存储，而是编译器根据高级语言中**数据类型信息**，来编排**汇编指令**，比如：

1、对`char`变量，编译器会在`load`的时候，通过专用的**字节load指令**并进行符号扩展，完成**内存**到**CPU内部寄存器**的载入；

2、再比如`unsigned short`变量，编译器也会在数据读取的时候安排一个0扩展的半字load；

而对于寄存器向外部存储器写入，则只控制位宽，不控制符号（你可以想想为什么）。

以上主要针对RISC，类似x86，则更为灵活，你可以深入研究研究。  





## Aliasing: 按照指定类型来进行解释



### 从底层来看type conversion

从一种类型到另外一种类型，是否是将另外一种类型的值拷贝到新数据内存空间中呢？


比如下面的这段代码：
```c
static uint64_t load64_le(uint8_t const* V)
{
#if !defined(__LITTLE_ENDIAN__)
#error This code only works with little endian systems
#endif
  uint64_t Ret = *((uint64_t const*)V);
  return Ret;
}
```
`uint64_t Ret = *((uint64_t const*)V);`的执行过程到底是什么？

`(uint64_t const*)V`会创建一个`uint64_t const*`临时变量，这个临时变量的值取自变量`V`，`*((uint64_t const*)V)`从这个临时变量所指向的内存空间中读取值，然后保存到变量`Ret`中。

在 [cppreference cast operator](https://en.cppreference.com/w/c/language/cast) 章节中有如下内容：

> Any pointer to object can be cast to any other pointer to object. If the value is not correctly aligned for the target type, the behavior is undefined. Otherwise（表示符合alignment requirement of the target type）, if the value is converted back to the original type, it compares equal to the original value. If a pointer to object is cast to pointer to any character type, the result points at the lowest byte of the object and may be incremented up to `sizeof` the target type (in other words, can be used to examine [object representation](https://en.cppreference.com/w/c/language/object) or to make a copy via [memcpy](https://en.cppreference.com/w/c/string/byte/memcpy) or [memmove](https://en.cppreference.com/w/c/string/byte/memmove)).

前面这段话的意思结合上述代码的实例来看的话是这样的：将`uint8_t const* V`的变量`V` cast 到`uint64_t const*`类型，此时会创建一个`uint64_t const*`临时变量，这个临时变量的值就是`V`的值，即某个指向`uint8_t const`类型变量的地址；这种操作是存在风险的：`V`的类型是`uint8_t const*`，所以它的值是符合`uint8_t `类型的alignment requirement的；但是它并一定符合`uint64_t const`的alignment requirement。一旦不符合，则这就是一个undefined behavior；



###  指针type conversion

1、geeksforgeeks [Little and Big Endian Mystery](https://www.geeksforgeeks.org/little-and-big-endian-mystery/)

这篇文章的这段代码非常好地展示处理type conversion的底层

```c
#include <stdio.h> 
int main() 
{ 
    unsigned char arr[2] = {0x01, 0x00}; 
    unsigned short int x = *(unsigned short int *) arr; 
    printf("%d", x); 
    getchar(); 
    return 0; 
} 
```

2、[Output from arbitrary dereferenced pointer](https://stackoverflow.com/questions/12451230/output-from-arbitrary-dereferenced-pointer)

```c
char buf[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
char *c_ptr;
unsigned long *u_ptr;

c_ptr = buf;
for (int i=0;i<5;i++)
{
    u_ptr = (unsigned long *)c_ptr;
    printf("%X\n",*u_ptr);
    c_ptr++;
}
```


## TODO

c-jump [Data Types and Memory Allocation](http://www.c-jump.com/CIS77/ASM/DataTypes/lecture.html)

csdn [汇编语言数据类型以及数据定义详解](https://blog.csdn.net/Javaxuxuexi/article/details/93312804)

## draft

### 20201224

关于alias to an existing object，在`C++\Language-reference\Basic-concept\Data-model\Object\Object.md`中进行了说明，本段只是进行总结。

C++支持两种方式来实现alias to an existing object：

- reference
- pointer

参见: 

- `rereinterpret_cast`: `C++\Language-reference\Basic-concept\Type-system\Type-conversion\Cast-operator`
- reference: `C++\Language-reference\Reference`

