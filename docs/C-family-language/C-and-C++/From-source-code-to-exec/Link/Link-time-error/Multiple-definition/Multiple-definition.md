# Multiple definition



## TODO

[c++ linking error of multiple definition](https://ubuntuforums.org/showthread.php?t=1660856)

[Solving multiple definition](http://www.cplusplus.com/forum/unices/5784/)

[Error with multiple definitions of function](https://stackoverflow.com/questions/17904643/error-with-multiple-definitions-of-function)

[问题解决： multiple definition of XXX](https://blog.csdn.net/liyuefeilong/article/details/44071053)



## 问题分析

当多个source file/translation unit中，包含有相同name的definition时，就会出现这个错误，这是因为c和c++中，遵循[ODR](https://en.cppreference.com/w/cpp/language/definition)。



TO READ:

https://stackoverflow.com/questions/17904643/error-with-multiple-definitions-of-function



## Example

main.cpp

```
#include <iostream>
#include <string>
#include "testA.h"
#include "testB.h"
 
using namespace std;
 
int main()
{
    testA();
    testB();
    return 0;
}
```
testA.h

```
#ifndef TESTA_H
#define TESTA_H
void testA(void);
#endif
```
testA.cpp

```
#include <iostream>
#include "testA.h"
#include "testC.h"
 
using namespace std;
 
void testA(void)
{
    cout<<"A"<<endl;
    testC();
}
```
testB.h

```
#ifndef TESTB_H
#define TESTB_H
void testB(void);
#endif
```
testB.cpp

```
#include <iostream>
#include "testB.h"
#include "testC.h"
 
using namespace std;
 
void testB(void)
{
    cout<<"B"<<endl;
    testC();
}

```

testC.h

```
#ifndef TESTC_H
#define TESTC_H
void testC(void);
#endif
```
testC.cpp

```
#include <iostream>
using namespace std;
 
void testC(void)
{
    cout<<"C"<<endl;
}

namespace CombiIndexes
{
	typedef int type;
	enum
	{
		FirstLeg = 1,
		SecondLeg = 2
	};
	//判断是否为第一腿
	bool IsFirstLeg(const type& CombiIndex)
	{
		return CombiIndex == FirstLeg;
	}

	//判断是否为第一腿
	bool IsSecondLeg(const type& CombiIndex)
	{
		return CombiIndex == SecondLeg;
	}
}

```

Makefile

```
main:main.cpp testA.o testB.o testC.o
    g++ main.cpp testA.o testB.o testC.o -o main
    rm *.o
 
testA.o:testA.cpp
    g++ testA.cpp -c
 
testB.o:testB.cpp
    g++ testB.cpp -c
 
testC.o:testC.cpp
    g++ testC.cpp -c
```
make之后就报了如下错误：

```
g++ testA.cpp -c
g++ testB.cpp -c
g++ testC.cpp -c
g++ main.cpp testA.o testB.o testC.o -o main
testB.o: In function `CombiIndexes::IsFirstLeg(int const&)':
testB.cpp:(.text+0x0): multiple definition of `CombiIndexes::IsFirstLeg(int const&)'
testA.o:testA.cpp:(.text+0x0): first defined here
testB.o: In function `CombiIndexes::IsSecondLeg(int const&)':
testB.cpp:(.text+0x16): multiple definition of `CombiIndexes::IsSecondLeg(int const&)'
testA.o:testA.cpp:(.text+0x16): first defined here
collect2: ld 返回 1
make: *** [main] 错误 1
```
关于multiple definition of编译错误，可以参考以下文章的分析：

[multiple definition of `xxxx`问题解决及其原理](http://blog.csdn.net/mantis_1984/article/details/53571758)

[自己调用自己定义的函数，编译时出现"multiple definition of"错误！](http://bbs.csdn.net/topics/390032683)

我最终是按照如下方式来修改好的：

```
namespace CombiIndexes
{
	typedef int type;
	enum
	{
		FirstLeg = 1,
		SecondLeg = 2
	};
	//判断是否为第一腿
	static inline bool IsFirstLeg(const type& CombiIndex)
	{
		return CombiIndex == FirstLeg;
	}

	//判断是否为第一腿
	static inline bool IsSecondLeg(const type& CombiIndex)
	{
		return CombiIndex == SecondLeg;
	}
}
```
将那两个方法修改为static方法。

### 深入分析
从上面的错误信息中可以看出来：在单独编译testA.cpp，testB.cpp，testC.cpp完全正常，出现错误的地方是在g++ main.cpp testA.o testB.o testC.o -o main，也就是最后在编译器链接testA.o，testB.o，testC.o生成main的时候报错了。

 一般在.h文件中定义一个变量声明时，在其他文件中只要包含了这个.h文件，编译的时候就会独立被编译器解释，然后每个.C文件会生成独立的标识符和符号表，所以上述代码在单独编译的时候并不会报错，语法是合法的。但是，最后在编译器链接的时候，就会将工程中所有的符号整合在一起，由于文件中有重复的变量，于是就会出现重复定义的错误,系统就是提示你“multiple definition of `xxx`”。这是可以通过查看testA.o和testB.o的符号表来进行核实的。

 TODO：核实修改前后testA.o和testB.o，testC.o的差异

 除了上面描述的这种修改方法，其实还有一些其他的修改方法，我需要详细分析这些修改方法的异同。