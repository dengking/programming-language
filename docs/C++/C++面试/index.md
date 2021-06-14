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

