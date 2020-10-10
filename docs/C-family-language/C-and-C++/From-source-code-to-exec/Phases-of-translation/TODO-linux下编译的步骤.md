最近一段时间做了关于一些软件的交叉编译工作，由于觉得并没有学到许多东西，所以抽时间看了关于静态链接方面的内容，读了一部分《程序员的自我修养——链接、装载与库》，记录一些读书笔记及自己的总结。

### 一、GCC做了什么？

我们在[Linux](http://lib.csdn.net/base/linux)中经常使用

```
gcc  源文件名.c  -o  目标可执行文件名
```

对一个编写好的C文件进行编译生成可执行文件，那么这之间的细节呢？ 
这里源文件名为 hello.c ，生成可执行文件名为 hello 为例子 

![图片](https://github.com/dengking/learn-cpp/tree/master/res/step-to-get-exec.png)

gcc一个C源文件时对过程进行了隐藏，实际为：预编译 cpp、编译 cc1、汇编 as、链接 ld，其中每一步后的英文名是所使用的工具的名称。

那么各自的步骤到底干了什么呢？

##### 1、预编译：处理.c文件中以“#”开头的预编译指令。

这些**预编译指令**包括了**宏定义**、**条件预编译指令**等等，此步中也会进行删除掉所有的注释等操作。当无法判断一个**宏定义**是否正确或者**头文件**是否正确包含的话，可以直接打开此步生成的文件进行检查。

```shell
gcc  -E  hello.c  -o  hello.i
```

##### 2、编译：进行词法分析、语法分析、语义分析等工作，此步所生成的文件是以汇编代码构成的。

这一步是由编译器cc1完成（gcc中），主要分为词法分析、语法分析、语义分析、中间语言生成、目标代码生成与优化几步。

> a.词法分析：将所有的[C语言](http://lib.csdn.net/base/c)源代码分割为一系列的token记号，这些token记号主要为关键字、标识符、常量及特殊符号，比如表达式 a+b 在这步中就会被拆分为 a 和 b 两个标识符及 + 这个特殊符号。
>
> b.语法分析：产生**语法树**，关于这步需要有一些数理逻辑的知识，即生成以表达式为节点的树，对应上面 a+b 的情况是 + 为一个节点，而 a 和 b 分别为左右子树的节点。
>
> c.语义分析：确定每个节点的类型，比如整型、字符型等。 
> 可以理解为在前一步的树的基础上在每个节点上都标示好**类型**，对于一些隐式转换及强制类型转换都会在这步中进行处理。
>
> d.中间语言生成：进行两步操作，首先将**语法树**转化为**中间代码**，然后在**中间代码**中对已经能够确定值的表达式进行求值。 
> 其中中间代码一般为三地址码，即 `x = y op z`的形式，其中op代表特殊符号，然后如果有些表达式能够确定其值，比如 t1=5+6 这种两个常量相加的语句就直接进行计算。
>
> e.目标代码生成与优化：如字面意思，进行目标代码的生成与优化。 
> 关于目标代码的生成与具体的硬件平台有关，而优化部分有部分操作，比如合适的寻址方式、对于乘法运算使用位移进行代替，这些如果有接触汇编代码会比较了解。

编译器流程图： 

![这里写图片描述](https://github.com/dengking/learn-cpp/tree/master/res/step-of-compile.png)

此步中生成的文件如果懂得汇编代码的话仍然是可读的

```
gcc  -S  hello.c  -o  hello.s
```

##### 3、汇编：将汇编代码转变成机器可以执行的指令。

此步中是根据**汇编指令**与**机器指令**的**对照表**进行一一翻译，基本上一个汇编语句对应一条机器指令。 
此步生成的文件已经没法读了，打开后全部是乱码，因为已经全部机制指令了。

```
gcc  -c  hello.s  -o  hello.o    
或
gcc  -c  hello.c  -o hello.o      
或
as  hello.s  -o  hello.o
```
>这一步生成的文件也叫做**relocatable file**

##### 4、链接：将几个输入的目标文件加工后合并为一个输出文件。

简单来说，可以看作拼**拼图**的过程，每个.o文件都是一块拼图碎片，链接就是将它们拼接成一幅图（可执行文件）的过程。

[linux](http://lib.csdn.net/base/linux)中可执行文件都称为ELF文件，Windows中为PE文件，之后都会这样称呼

##### 总结：

经过上面的几步我们大致了解了编译的过程，尤其是gcc的编译器cc1对源文件的操作步骤。

下面是在32位的Ubuntu中对 hello.c 文件进行动态编译显示的详细信息，静态编译与之类似：

```
gcc  --verbose  -fno-builtin  hello.c   //--verbose 表示显示编译的详细信息，-fno-builtin表示关闭内置函数优化选项，因为hello.c文件中用printf只对一个字符串进行输出，此时GCC会将其自动替换为puts函数。
```

```
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/i686-linux-gnu/4.8/lto-wrapper
Target: i686-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 4.8.4-2ubuntu1~14.04.1' --with-bugurl=file:///usr/share/doc/gcc-4.8/README.Bugs --enable-languages=c,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-4.8 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.8 --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --enable-gnu-unique-object --disable-libmudflap --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-4.8-i386/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-4.8-i386 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-4.8-i386 --with-arch-directory=i386 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-objc-gc --enable-targets=all --enable-multiarch --disable-werror --with-arch-32=i686 --with-multilib-list=m32,m64,mx32 --with-tune=generic --enable-checking=release --build=i686-linux-gnu --host=i686-linux-gnu --target=i686-linux-gnu
Thread model: posix
gcc version 4.8.4 (Ubuntu 4.8.4-2ubuntu1~14.04.1) 
COLLECT_GCC_OPTIONS='-v' '-fno-builtin' '-mtune=generic' '-march=i686'
 /usr/lib/gcc/i686-linux-gnu/4.8/cc1 -quiet -v -imultiarch i386-linux-gnu hello.c -quiet -dumpbase hello.c -mtune=generic -march=i686 -auxbase hello -version -fno-builtin -fstack-protector -Wformat -Wformat-security -o /tmp/ccM5JRqF.s
GNU C (Ubuntu 4.8.4-2ubuntu1~14.04.1) version 4.8.4 (i686-linux-gnu)
    compiled by GNU C version 4.8.4, GMP version 5.1.3, MPFR version 3.1.2-p3, MPC version 1.0.1
GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
ignoring nonexistent directory "/usr/local/include/i386-linux-gnu"
ignoring nonexistent directory "/usr/lib/gcc/i686-linux-gnu/4.8/../../../../i686-linux-gnu/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/i686-linux-gnu/4.8/include
 /usr/local/include
 /usr/lib/gcc/i686-linux-gnu/4.8/include-fixed
 /usr/include/i386-linux-gnu
 /usr/include
End of search list.
GNU C (Ubuntu 4.8.4-2ubuntu1~14.04.1) version 4.8.4 (i686-linux-gnu)
    compiled by GNU C version 4.8.4, GMP version 5.1.3, MPFR version 3.1.2-p3, MPC version 1.0.1
GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
Compiler executable checksum: aaaa08c6fefb26a9d107c18441ada1ef
COLLECT_GCC_OPTIONS='-v' '-fno-builtin' '-mtune=generic' '-march=i686'
 as -v --32 -o /tmp/ccs3h630.o /tmp/ccM5JRqF.s
GNU汇编版本 2.24 (i686-linux-gnu) 使用BFD版本 (GNU Binutils for Ubuntu) 2.24
COMPILER_PATH=/usr/lib/gcc/i686-linux-gnu/4.8/:/usr/lib/gcc/i686-linux-gnu/4.8/:/usr/lib/gcc/i686-linux-gnu/:/usr/lib/gcc/i686-linux-gnu/4.8/:/usr/lib/gcc/i686-linux-gnu/
LIBRARY_PATH=/usr/lib/gcc/i686-linux-gnu/4.8/:/usr/lib/gcc/i686-linux-gnu/4.8/../../../i386-linux-gnu/:/usr/lib/gcc/i686-linux-gnu/4.8/../../../../lib/:/lib/i386-linux-gnu/:/lib/../lib/:/usr/lib/i386-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/i686-linux-gnu/4.8/../../../:/lib/:/usr/lib/
COLLECT_GCC_OPTIONS='-v' '-fno-builtin' '-mtune=generic' '-march=i686'
 /usr/lib/gcc/i686-linux-gnu/4.8/collect2 --sysroot=/ --build-id --eh-frame-hdr -m elf_i386 --hash-style=gnu --as-needed -dynamic-linker /lib/ld-linux.so.2 -z relro /usr/lib/gcc/i686-linux-gnu/4.8/../../../i386-linux-gnu/crt1.o /usr/lib/gcc/i686-linux-gnu/4.8/../../../i386-linux-gnu/crti.o /usr/lib/gcc/i686-linux-gnu/4.8/crtbegin.o -L/usr/lib/gcc/i686-linux-gnu/4.8 -L/usr/lib/gcc/i686-linux-gnu/4.8/../../../i386-linux-gnu -L/usr/lib/gcc/i686-linux-gnu/4.8/../../../../lib -L/lib/i386-linux-gnu -L/lib/../lib -L/usr/lib/i386-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/i686-linux-gnu/4.8/../../.. /tmp/ccs3h630.o -lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed /usr/lib/gcc/i686-linux-gnu/4.8/crtend.o /usr/lib/gcc/i686-linux-gnu/4.8/../../../i386-linux-gnu/crtn.o
```

其中最重要的三句已经用红字表示了出来，可以看到一个最简单的helloworld程序都需要链接许多.o目标文件才能够得到ELF文件。 
从上看出，gcc实际上是对**cpp、cc1、as、ld**几个后台程序的包装，执行它时会根据具体的参数要求去调用这几个后台程序，从而得到我们所需要编译出的ELF文件。
