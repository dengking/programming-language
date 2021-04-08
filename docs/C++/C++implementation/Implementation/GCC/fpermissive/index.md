# fpermissive 

## 起因

在使用命令编译`cpp`文件的时候: 

```C++
gcc -I. -I../../../ -I../../../../ -I../../../../../ -I../../../../../../ -I../../../../cres/Include/ -I../../../../ldp/include/ -I../../../include/ -I../../include/ -I../common/new_logger/ -I./api_app -I./linenoise -I./sds -I./yaml-cpp-yaml-cpp-0.6.3/include/ -Wp,-MT,build/main.o -Wp,-MMD,build/main.o.d -c -fvisibility=hidden -std=c++11  -Wno-unknown-pragmas -Wno-deprecated -Wno-write-strings  -g  -DOS_COMMON_EXT -DNDEBUG -D_FILE_OFFSET_BITS=64 -c -o build/main.o main.cpp
```

遇到了如下编译报错:

```C++
sds/sds.h: 在函数‘size_t sdsavail(sds)’中:
sds/sds.h:83:87: 错误：从类型‘void*’到类型‘sdshdr8*’的转换无效 [-fpermissive]
 #define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
                                                                                       ^
sds/sds.h:114:13: 附注：in expansion of macro ‘SDS_HDR_VAR’
             SDS_HDR_VAR(8,s);
             ^
sds/sds.h:83:87: 错误：从类型‘void*’到类型‘sdshdr16*’的转换无效 [-fpermissive]
 #define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
                                                                                       ^
sds/sds.h:118:13: 附注：in expansion of macro ‘SDS_HDR_VAR’
             SDS_HDR_VAR(16,s);
             ^
sds/sds.h:83:87: 错误：从类型‘void*’到类型‘sdshdr32*’的转换无效 [-fpermissive]
 #define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
                                                                                       ^
sds/sds.h:122:13: 附注：in expansion of macro ‘SDS_HDR_VAR’
             SDS_HDR_VAR(32,s);
             ^
sds/sds.h:83:87: 错误：从类型‘void*’到类型‘sdshdr64*’的转换无效 [-fpermissive]
 #define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
                                                                                       ^
sds/sds.h:126:13: 附注：in expansion of macro ‘SDS_HDR_VAR’
             SDS_HDR_VAR(64,s);
             ^
In file included from main.cpp:1:0:
api_shell.h: 在函数‘void freeHintsCallback(void*)’中:
api_shell.h:50:13: 错误：从类型‘void*’到类型‘sds {aka char*}’的转换无效 [-fpermissive]
  sdsfree(ptr);
             ^
In file included from util.h:16:0,
                 from api_shell.h:13,
                 from main.cpp:1:
sds/sds.h:225:6: 错误：  初始化‘void sdsfree(sds)’的实参 1 [-fpermissive]
 void sdsfree(sds s);

```

1、`sds.h`是[antirez](https://github.com/antirez)/**[sds](https://github.com/antirez/sds)**中的source code，它是使用C编写的，在C中，是允许void pointer到其它类型的implicit conversion的，但是这在C++中是非法的，所以报了上述错误。

2、上述报错中的`-fpermissive`引起了我的注意，"permissive"的含义是允许的、自由的。

3、参见 thegreenplace [void* and casts, in C and C++](https://eli.thegreenplace.net/2009/11/16/void-and-casts-in-c-and-c)

4、显然，我的上述问题的来源是mix C and C++

5、后来，我在编译指令中添加了`-fpermissive`，然后上述报错全部变成了warning:

```C++
./api_app/../sds/sds.h: 在函数‘size_t sdsavail(sds)’中:
./api_app/../sds/sds.h:83:87: 警告：从类型‘void*’到类型‘sdshdr8*’的转换无效 [-fpermissive]
 #define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
                                                                                       ^
./api_app/../sds/sds.h:114:13: 附注：in expansion of macro ‘SDS_HDR_VAR’
             SDS_HDR_VAR(8,s);
             ^
./api_app/../sds/sds.h:83:87: 警告：从类型‘void*’到类型‘sdshdr16*’的转换无效 [-fpermissive]
 #define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
                                                                                       ^
./api_app/../sds/sds.h:118:13: 附注：in expansion of macro ‘SDS_HDR_VAR’
             SDS_HDR_VAR(16,s);
             ^
./api_app/../sds/sds.h:83:87: 警告：从类型‘void*’到类型‘sdshdr32*’的转换无效 [-fpermissive]
 #define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
                                                                                       ^
./api_app/../sds/sds.h:122:13: 附注：in expansion of macro ‘SDS_HDR_VAR’
             SDS_HDR_VAR(32,s);
             ^
./api_app/../sds/sds.h:83:87: 警告：从类型‘void*’到类型‘sdshdr64*’的转换无效 [-fpermissive]
 #define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
                                                                                       ^
./api_app/../sds/sds.h:126:13: 附注：in expansion of macro ‘SDS_HDR_VAR’
             SDS_HDR_VAR(64,s);
```



## stackoverflow [What does the fpermissive flag do?](https://stackoverflow.com/questions/8843818/what-does-the-fpermissive-flag-do)



[A](https://stackoverflow.com/a/8843848)

Right from [the docs](http://gcc.gnu.org/onlinedocs/gcc-4.0.4/gcc/C_002b_002b-Dialect-Options.html#index-fpermissive-140):

> `-fpermissive`
> Downgrade some diagnostics about nonconformant code from errors to warnings. Thus, using `-fpermissive` will allow some nonconforming code to compile.

Bottom line: **don't use** it unless you know what you are doing!

> NOTE: 
>
> 1、简而言之，将error变成waring



## gcc [3.5 Options Controlling C++ Dialect # -fpermissive](https://gcc.gnu.org/onlinedocs/gcc-4.0.4/gcc/C_002b_002b-Dialect-Options.html#index-fpermissive-140)

`-fpermissive`

Downgrade some diagnostics about nonconformant code from errors to warnings. Thus, using -fpermissive will allow some nonconforming code to compile.