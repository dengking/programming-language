# [C++ vtables - Part 1 - Basics](https://shaharmike.com/cpp/vtable-part1/)

In this mini post-series we’ll explore how [`clang`](https://clang.llvm.org/) implements **vtables** & **RTTI**. In this part we’ll start with some basic classes and later on cover **multiple inheritance** and **virtual inheritance**.

Please note that this mini-series will include some digging into the binary generated for our different pieces of code via [`gdb`](https://www.gnu.org/software/gdb/). This is somewhat low-level(ish), but I’ll do all the heavy lifting for you. I don’t believe many future posts will be this low-level.

> *Disclaimer*: everything written here is implementation specific, may change in any future version, and should not be relied on. We look into this for educational reasons only.
>
> ☑ I agree

cool, let’s start.

## Part 1 - vtables - Basics

Let’s examine the following code:

```cpp
#include <iostream>
using namespace std;

class NonVirtualClass
{
public:
	void foo()
	{
	}
};

class VirtualClass
{
public:
	virtual void foo()
	{
	}
};

int main()
{
	cout << "Size of NonVirtualClass: " << sizeof(NonVirtualClass) << endl;
	cout << "Size of VirtualClass: " << sizeof(VirtualClass) << endl;
}
// g++ test.cpp
```

```shell
$ # compile and run main.cpp
$ clang++ main.cpp && ./a.out
Size of NonVirtualClass: 1
Size of VirtualClass: 8
```

> NOTE: 上述是原文中，使用`clang`编译的结果，在我的环境中，没有`clang`，使用的`g++`进行编译。