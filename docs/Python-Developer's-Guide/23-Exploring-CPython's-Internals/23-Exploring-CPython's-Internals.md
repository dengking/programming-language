[TOC]

# 23. Exploring CPython’s Internals[¶](https://devguide.python.org/exploring/#exploring-cpython-s-internals)



## 23.1. CPython Source Code Layout[¶](https://devguide.python.org/exploring/#cpython-source-code-layout)

This guide gives an overview of CPython’s code structure. It serves as a summary of file locations for modules and builtins.

For Python **modules**, the typical layout is:

- `Lib/<module>.py`
- `Modules/_<module>.c` (if there’s also a C accelerator module)
- `Lib/test/test_<module>.py`
- `Doc/library/<module>.rst`

For extension-only modules, the typical layout is:

- `Modules/<module>module.c`
- `Lib/test/test_<module>.py`
- `Doc/library/<module>.rst`

For builtin types, the typical layout is:

- `Objects/<builtin>object.c`
- `Lib/test/test_<builtin>.py`
- `Doc/library/stdtypes.rst`

For builtin functions, the typical layout is:

- `Python/bltinmodule.c`
- `Lib/test/test_builtin.py`
- `Doc/library/functions.rst`

Some exceptions:

- builtin type `int` is at `Objects/longobject.c`
- builtin type `str` is at `Objects/unicodeobject.c`
- builtin module `sys` is at `Python/sysmodule.c`
- builtin module `marshal` is at `Python/marshal.c`
- Windows-only module `winreg` is at `PC/winreg.c`



## 23.2. Additional References[¶](https://devguide.python.org/exploring/#additional-references)

> NOTE: 在CPython的官方文档中并没有发现专门对CPython的架构进行专门介绍的，下面的文章虽然不是官方文档，但是内容非常好，读完后，基本可以掌握CPython的架构。

For over 20 years the CPython code base has been changing and evolving. Here’s a sample of resources about the architecture of CPython aimed at building your understanding of both the 2.x and 3.x versions of CPython:

**Current references**[¶](https://devguide.python.org/exploring/#id1)

| Title                                                        | Brief                                                | Author           | Version | 评价                     |
| :----------------------------------------------------------- | :--------------------------------------------------- | :--------------- | :------ | ------------------------ |
| [A guide from parser to objects, observed using GDB](https://hackmd.io/s/ByMHBMjFe) | Code walk from Parser, AST, Sym Table and Objects    | Louie Lu         | 3.7.a0  |                          |
| [Green Tree Snakes](https://greentreesnakes.readthedocs.io/en/latest/) | The missing Python AST docs                          | Thomas Kluyver   | 3.6     |                          |
| [Yet another guided tour of CPython](https://paper.dropbox.com/doc/Yet-another-guided-tour-of-CPython-XY7KgFGn88zMNivGJ4Jzv) | A guide for how CPython REPL works                   | Guido van Rossum | 3.5     | 中国大陆无法访问         |
| [Python Asynchronous I/O Walkthrough](http://pgbovine.net/python-async-io-walkthrough.htm) | How CPython async I/O, generator and coroutine works | Philip Guo       | 3.5     |                          |
| [Coding Patterns for Python Extensions](https://pythonextensionpatterns.readthedocs.io/en/latest/) | Reliable patterns of coding Python Extensions in C   | Paul Ross        | 3.4     |                          |
| [Your Guide to the CPython Source Code](https://realpython.com/cpython-source-code-guide/) | Your Guide to the CPython Source Code                | Anthony Shaw     | 3.8     | 非常好，非常适合于入门。 |

> 如下是一些补充：
>
> - [Guided tour to the CPython source code](https://github.com/pyladies/pyladies-maintainers/issues/6)
> - [运行cpython](https://cloud.tencent.com/developer/information/%E8%BF%90%E8%A1%8Ccpython)

**Historical references**[¶](https://devguide.python.org/exploring/#id2)

| Title                                                        | Brief                                             | Author          | Version | 评价                             |
| :----------------------------------------------------------- | :------------------------------------------------ | :-------------- | :------ | -------------------------------- |
| [Python’s Innards Series](https://tech.blog.aknin.name/category/my-projects/pythons-innards/) | ceval, objects, pystate and miscellaneous topics  | Yaniv Aknin     | 3.1     |                                  |
| [Eli Bendersky’s Python Internals](https://eli.thegreenplace.net/tag/python-internals) | Objects, Symbol tables and miscellaneous topics   | Eli Bendersky   | 3.x     |                                  |
| [A guide from parser to objects, observed using Eclipse](https://docs.google.com/document/d/1nzNN1jeNCC_bg1LADCvtTuGKvcyMskV1w8Ad2iLlwoI/) | Code walk from Parser, AST, Sym Table and Objects | Prashanth Raghu | 2.7.12  |                                  |
| [CPython internals: A ten-hour codewalk through the Python interpreter source code](http://pgbovine.net/cpython-internals.htm) | Code walk from source code to generators          | Philip Guo      | 2.7.8   | 主要是视频，但是中国大陆无法访问 |