# docs [Clang command line argument reference](https://clang.llvm.org/docs/ClangCommandLineReference.html)

## [Compilation flags](https://clang.llvm.org/docs/ClangCommandLineReference.html#id6)[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#compilation-flags)

Flags controlling the behavior of Clang during compilation. These flags have no effect during actions that do not perform compilation.



### `-Xclang <arg>`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-xclang)

Pass `<arg>` to the clang compiler

> NOTE:
>
> 一、example:
>
> 下面的例子来自chromium: 
>
> ```shell
>     -Xclang -add-plugin
>     -Xclang find-bad-constructs 
>     -Xclang -plugin-arg-find-bad-constructs 
>     -Xclang raw-ptr-template-as-trivial-member 
>     -Xclang -plugin-arg-find-bad-constructs 
>     -Xclang use-classify-type 
> ```
>
> 上述例子中，使用到了 `-fplugin-arg-<name>-<arg>`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fplugin-arg-name-arg) 



### plugin

#### `-fpass-plugin=<dsopath>`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fpass-plugin)

Load pass plugin from a dynamic shared object file (only with new pass manager).

#### `-fplugin-arg-<name>-<arg>`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fplugin-arg-name-arg)

Pass `<arg>` to plugin `<name>`



#### `-fplugin=<dsopath>`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fplugin)

Load the named plugin (dynamic shared object)



### `nostdinc`

> NOTE: 
>
> 一、关于它的usage: stackoverflow [gcc: How to ignore standard include paths?](https://stackoverflow.com/questions/2988779/gcc-how-to-ignore-standard-include-paths)
>
> > I need to compile some files with a usage of modified versions of standard library headers. With Visual C++ compiler I will do this by usage of */X (Ignore Standard Include Paths)* and */I (Additional Include Directories)* parameters. How this should be done with gcc?



`-nostdinc, --no-standard-includes`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-nostdinc)

`-nostdinc++`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang1-nostdinc)



### [Preprocessor flags](https://clang.llvm.org/docs/ClangCommandLineReference.html#id7)[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#preprocessor-flags)

Flags controlling the behavior of the Clang **preprocessor**.

#### [Include path management](https://clang.llvm.org/docs/ClangCommandLineReference.html#id8)[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#include-path-management)

Flags controlling how `#include`s are resolved to files.

> NOTE: 
>
> |                    | flag                                                         |      |
> | ------------------ | ------------------------------------------------------------ | ---- |
> | search path        | `-I<dir>, --include-directory<arg>, --include-directory=<arg>`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-i-dir) |      |
> | system search path | `-isystem<directory>`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-isystem-directory) |      |
> |                    |                                                              |      |
>
> 

#### [Dependency file generation](https://clang.llvm.org/docs/ClangCommandLineReference.html#id9)[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#dependency-file-generation)

> NOTE: 
>
> 一、"dependency file"是什么？
>
> 在stackoverflow [How to print dependencies from llvm / clang (equivalent of gcc -MD)?](https://stackoverflow.com/questions/5584435/how-to-print-dependencies-from-llvm-clang-equivalent-of-gcc-md) # [A](https://stackoverflow.com/a/5584472) 中进行了解释:
>
> > An example:
> >
> > ```c++
> > $ cat > example.c 
> > #include <stdio.h>
> > 
> > int main(int argc, char **argv)
> > {
> >   printf("Hello, world!\n");
> >   return 0;
> > }
> > 
> > $ clang -MD -MF example-clang.d -c -o example-clang.o example.c
> > 
> > $ gcc -MD -MF example-gcc.d -c -o example-gcc.o example.c
> > 
> > $ cat example-clang.d 
> > example-clang.o: example.c /usr/include/stdio.h /usr/include/_types.h \
> >   /usr/include/sys/_types.h /usr/include/sys/cdefs.h \
> >   /usr/include/machine/_types.h /usr/include/i386/_types.h \
> >   /usr/include/secure/_stdio.h /usr/include/secure/_common.h
> > 
> > $ cat example-gcc.d 
> > example-gcc.o: example.c /usr/include/stdio.h /usr/include/_types.h \
> >   /usr/include/sys/_types.h /usr/include/sys/cdefs.h \
> >   /usr/include/machine/_types.h /usr/include/i386/_types.h \
> >   /usr/include/secure/_stdio.h /usr/include/secure/_common.h
> > 
> > $ diff example-clang.d example-gcc.d 
> > 1c1
> > < example-clang.o: example.c /usr/include/stdio.h /usr/include/_types.h \
> > ---
> > > example-gcc.o: example.c /usr/include/stdio.h /usr/include/_types.h \
> > ```
>
> 二、dependency file的作用是什么？
>
> 在下面进行了介绍
>
> 三、example
>
> ```shell
> ../../third_party/llvm-build/Release+Asserts/bin/clang++  -MMD -MF obj/apps/apps/app_lifetime_monitor.o.d 
> ```
>
> 

Flags controlling generation of a dependency file for `make`-like build systems.

### [Target-independent compilation options](https://clang.llvm.org/docs/ClangCommandLineReference.html#id12)[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#target-independent-compilation-options)

> NOTE: 
>
> 内容非常多



`-fdelete-null-pointer-checks, -fno-delete-null-pointer-checks`[¶](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fdelete-null-pointer-checks)

> NOTE: 
>
> 一、原文对它的介绍说非常少的，相比之下，在gcc doc [3.11 Options That Control Optimization](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html) 这对这两个option的介绍说更加详细的





