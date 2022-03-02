# [cmake-language(7)](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id9)[¶](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#cmake-language-7)

CMake input files are written in the "CMake Language" in source files named `CMakeLists.txt` or ending in a `.cmake` file name extension.

> NOTE: 
>
> 两种CMake input files:
>
> 1、`CMakeLists.txt`
>
> 2、`.cmake` file name extension

CMake Language source files in a project are organized into:

1、[Directories](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#directories) (`CMakeLists.txt`),

2、[Scripts](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#scripts) (`<script>.cmake`), and

3、[Modules](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#modules) (`<module>.cmake`).

## [Organization](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id10)[¶](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#organization)

### [Directories](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id11)[¶](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#directories)

> NOTE: 
>
> 一、cmake的directory会包含对应的`CMakeLists.txt`，可以通过 [`add_subdirectory()`](https://cmake.org/cmake/help/latest/command/add_subdirectory.html#command:add_subdirectory) 来添加 subdirectory，基本上cmake directory和file system的directory是对应的
>
> 二、cmake中有如下概念:
>
> 1、project source tree
>
> 2、build tree

### [Scripts](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id12)[¶](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#scripts)

> NOTE: 
>
> 一、这是cmake的*script mode*
>
> 二、cmake中包含专门的 [Scripting Commands](https://cmake.org/cmake/help/v3.21/manual/cmake-commands.7.html#id3)[¶](https://cmake.org/cmake/help/v3.21/manual/cmake-commands.7.html#scripting-commands)，显然

### [Modules](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id13)[¶](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#modules)

> NOTE: 
>
> 一、module的目的是为了模块化

## [Syntax](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id14)

### [Source Files](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id16)[¶](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#source-files)

A CMake Language source file consists of zero or more [Command Invocations](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#command-invocations) separated by newlines and optionally spaces and [Comments](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#comments):

```bash
file         ::=  file_element*
file_element ::=  command_invocation line_ending |
                  (bracket_comment|space)* line_ending
line_ending  ::=  line_comment? newline
space        ::=  <match '[ \t]+'>
newline      ::=  <match '\n'>
```

> NOTE: 
>
> 一、上面是典型的CFG，它非常简单，其实这也体现了cmake的简单性
>
> 二、cmake source file其实主要是由它的command invocation组成

### [Command Invocations](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id17)[¶](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#command-invocations)

> NOTE: 
>
> 这是最最常用的

## [Variables](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id31)



Variables have dynamic scope. Each variable "set" or "unset" creates a binding in the current scope

> NOTE: 
>
> 一、"Each variable "set" or "unset" creates a binding in the current scope" 如何理解？

## [Environment Variables](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id32)[¶](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#environment-variables)

> NOTE: 
>
> 暂时还没有使用过

## [Lists](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#id33)[¶](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#lists)

> NOTE: 
>
> 下面是一个例子:
>
> ```cmake
> list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/")
> ```
>
> 