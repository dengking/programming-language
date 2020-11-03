# eclipse

在 https://recomm.cnblogs.com/blogpost/4431964 中对相关问题进行了总结。

## Eclipse IDE for C/C++ Developers下载地址

[Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/)

## 如何使用doxygen风格的注释？

在Window->Preferences中搜索“doxygen"，然后会搜索到”C/C++“->"Editor"界面，将workspace default设置为doxygen。



## Formmater

目前，对于C++，我采用的formatter是`./my-bsd-formatter.xml`，它是基于BSD的。



## 搭建Eclipse Linux C++开发环境

### cnblogs [Eclipse开发C/C++ 安装配置图文详解](https://www.cnblogs.com/loongqiang/p/3767672.html)

> 这篇文章写得不错

### cnblogs [ubuntu11.10搭建eclipse C++开发环境](https://www.cnblogs.com/ylan2009/archive/2012/04/16/2451080.html)

> 写的不多

### Eclipse + MinGW

如何在eclipse中配置MinGW？参见:

1) https://www.eclipse.org/4diac/documentation/html/installation/minGW.html



## Eclipse C++/C header and symbol

### cnblogs [eclipse添加C/C++头文件跳转](https://www.cnblogs.com/skywalker0011/p/4431964.html)

 Eclipse 中新建C 或C ++到项目时，头文件报警，显示“Unresolved inclusion:<stdio.h>” 虽然不影响项目到编译和运行，确也无法查看头文件，让人感觉实在不爽。

在eclipse中菜单Window --> Preferences --> C/C++ --> Build --> Environment添加环境变量

```C++
C_INCLUDE_PATH =/usr/lib/gcc/x86_64-linux-gnu/4.9/include:/usr/local/include:/usr/lib/gcc/x86_64-linux-gnu/4.9/include-fixed:/usr/include/x86_64-linux-gnu:/usr/include

CPLUS_INCLUDE_PATH =/usr/include/c++/4.9:/usr/include/x86_64-linux-gnu/c++/4.9:/usr/include/c++/4.9/backward:/usr/lib/gcc/x86_64-linux-gnu/4.9/include:/usr/local/include:/usr/lib/gcc/x86_64-linux-gnu/4.9/include-fixed:/usr/include/x86_64-linux-gnu:/usr/include
```

记得选中下面的Append variables to native environment.

添加完后重启Eclipse 新建项目即可。

### cnblogs [Eclipse CDT配置c++标准库目录（Ubuntu 11.10）](https://www.cnblogs.com/windroc/archive/2011/10/25/2237024.html)



### cnblogs [eclipse C开发添加自己的头文件搜索路径](https://www.cnblogs.com/lidabo/p/6802040.html)