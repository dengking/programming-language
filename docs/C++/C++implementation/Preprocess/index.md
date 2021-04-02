# Preprocessor 

## What is preprocessor?

参见: 

1、wikipedia [Preprocessor](https://en.wikipedia.org/wiki/Preprocessor)

2、microsoft [C/C++ preprocessor reference](C/C++ preprocessor reference)

3、[gcc The C Preprocessor](https://gcc.gnu.org/onlinedocs/cpp/)



## cppreference Preprocessor

The preprocessor is executed at [translation phase 4](https://en.cppreference.com/w/cpp/language/translation_phases), before the compilation. The result of preprocessing is a single file which is then passed to the actual compiler.

### Directives and Capabilities

preprocessing instruction

> NOTE: 预编译指令，它们都是以`#`打头的，下面是对它们的分类

| Capabilities                                                 | Directive                                                    |      |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ---- |
| [Conditional inclusion](https://en.cppreference.com/w/cpp/preprocessor/conditional) | `#if`, `#ifdef`, `#ifndef`, <br/>`#else`, `#elif` and <br/>`#endif` |      |
| [Replacing text macros](https://en.cppreference.com/w/cpp/preprocessor/replace) | 1) `#define` and `#undef` <br/>2) `#` and `##                |      |
| [Source file inclusion](https://en.cppreference.com/w/cpp/preprocessor/include) | `include`, <br/>`__has_include`(since C++17)                 |      |
| [Error directive](https://en.cppreference.com/w/cpp/preprocessor/error) | `error`,                                                     |      |
| [Implementation defined behavior control](https://en.cppreference.com/w/cpp/preprocessor/impl) | `pragma`, <br/>`_Pragma`(since C++11)                        |      |
| [Filename and line information](https://en.cppreference.com/w/cpp/preprocessor/line) | `line`,                                                      |      |

[[1\]](https://en.cppreference.com/w/cpp/preprocessor#cite_note-1)






