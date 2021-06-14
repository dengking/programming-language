# 编译内存相关

本章是 C++ 面经的开章之作，其中涉及 C++ 程序的编译过程、内存以及头文件的一些知识点，重点在内存方面进行展开，包括内存的分区、内存对齐、内存泄漏、内存泄漏的防止方法、现有的检测内存泄漏的工具等等。由于问题之间的关联性，可能有些问题并非是本章相关的知识点，例如一些问题涉及到了类中的虚函数、创建类的对象的底层原理等等，但为了保持问题上下的连贯性，也放在了本章，便于问题的理解。



## A C++ 程序编译过程

> NOTE: 
>
> compiler principle、龙书、from-source-to-exec
>
> 一、主要考察C++的implementation，参见 `C++\C++implementation` 章节
>
> 二、programming language的design and implementation
>
> design: grammar
>
> implementation: from source code to excutable
>
> 三、Separate compilation model、translation unit，参见 `C++\C++implementation\Phases-of-translation\Separate-compilation-model` 章节
>
> 涉及到的问题:
>
> 1、Linkage 是建立在 translation unit 之间的一个概念
>
> 2、Initialization order of no-local object with static storage duration
>
> 四、分步骤进行描述
>
> 1、Preprocess，参见 `C++\C++implementation\Preprocess` 章节
>
> macro、include guard
>
> cppreference [Preprocessor](https://en.cppreference.com/w/cpp/preprocessor)
>
> 2、Compile，参见 `C++\C++implementation\Compile` 章节
>
> 3、Link，参见 `C++\C++implementation\Link` 章节



编译过程分为四个过程：编译（编译预处理、编译、优化），汇编，链接。

编译预处理：处理以 # 开头的指令；

编译、优化：将源码 .cpp 文件翻译成 .s 汇编代码；

汇编：将汇编代码 .s 翻译成机器指令 .o 文件；

链接：汇编程序生成的目标文件，即 .o 文件，并不会立即执行，因为可能会出现：.cpp 文件中的函数引用了另一个 .cpp 文件中定义的符号或者调用了某个库文件中的函数。那链接的目的就是将这些文件对应的目标文件连接成一个整体，从而生成可执行的程序 .exe 文件。

![](./1612676946-HVvkdR-image.png)

### 链接分为两种：

静态链接：代码从其所在的静态链接库中拷贝到最终的可执行程序中，在该程序被执行时，这些代码会被装入到该进程的虚拟地址空间中。

动态链接：代码被放到动态链接库或共享对象的某个目标文件中，链接程序只是在最终的可执行程序中记录了共享对象的名字等一些信息。在程序执行时，动态链接库的全部内容会被映射到运行时相应进行的虚拟地址的空间。

### 二者的优缺点：

静态链接：浪费空间，每个可执行程序都会有目标文件的一个副本，这样如果目标文件进行了更新操作，就需要重新进行编译链接生成可执行程序（更新困难）；优点就是执行的时候运行速度快，因为可执行程序具备了程序运行的所有内容。

动态链接：节省内存、更新方便，但是动态链接是在程序运行时，每次执行都需要链接，相比静态链接会有一定的性能损失。



## TODO: C++ 内存管理

> NOTE: 
>
> ELF
>
> C++ data model、object、object lifetime and storage duration
>
> C++ 给予了 programmer对memory的控制，这是Java所无法提供的，参见 `C++\CppReference\Basic-concept\Object\cppreference-object` 章节





## 栈和堆的区别

> NOTE: 
>
> 一、
>
> 参见工程Linux-OS 的`Multitasking\Process-memory\Process-address-space\Segment\Stack-VS-heap\Operation-on-stack-VS-on-heap` 章节
>
> 结合process run model来进行解答，参见 `Multitasking\Process-model\Process-run-model` 章节
>
> 二、private、shared
>
> stack对应的是call stack，每个thread都一个一个call stack
>
> heap是所有的thread shared的
>
> 三、storage duration、存放的内容
>
> automatic storage duration、automatic object、function argument
>
> dynamic storage duration、dynamic object
>
> 四、size
>
> call stack的size是可以控制的，参见: `man-3-pthread_create`
>
> heap的size是hardware决定的
>
> 五、C++ prefer automatic storage duration over dynamic storage duration
>
> 六、效率
>
> stack 远高于 heap
>
> 如何进行optimization:
>
> 1、memory pool
>
> 2、small buffer optimization

申请方式：栈是系统自动分配，堆是程序员主动申请。

申请后系统响应：分配栈空间，如果剩余空间大于申请空间则分配成功，否则分配失败栈溢出；申请堆空间，堆在内存中呈现的方式类似于链表（记录空闲地址空间的链表），在链表上寻找第一个大于申请空间的节点分配给程序，将该节点从链表中删除，大多数系统中该块空间的首地址存放的是本次分配空间的大小，便于释放，将该块空间上的剩余空间再次连接在空闲链表上。

栈在内存中是连续的一块空间（向低地址扩展）最大容量是系统预定好的，堆在内存中的空间（向高地址扩展）是不连续的。

申请效率：栈是有系统自动分配，申请效率高，但程序员无法控制；堆是由程序员主动申请，效率低，使用起来方便但是容易产生碎片。

存放的内容：栈中存放的是局部变量，函数的参数；堆中存放的内容由程序员控制。



## 变量的区别

> NOTE: 
>
> 一、C++ data model，参见
>
> `C++\CppReference\Basic-concept\Object`
>
> 
>
> 二、从C++的四种storage duration为基准来展开，在 `C++\CppReference\Basic-concept\Object\Lifetime&&storage-duration\Storage-duration` 中的table就是这个问题的答案
>
> 
>
> 

