# shaharmike [C++ vtables - Part 1 - Basics](https://shaharmike.com/cpp/vtable-part1/)

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

`NonVirtualClass` has a size of 1 because in C++ `class`es can’t have zero size. However, this is not important right now.

`VirtualClass`’s size is 8 on a 64 bit machine. Why? Because there’s a hidden pointer inside it pointing to a `vtable`. `vtable`s are static translation tables, created for each virtual-class. This post series is about their content and how they are used.

To get some deeper understanding on how `vtables` look let’s explore the following code with gdb to find out how the memory is laid out:

```C++
#include <iostream>

class Parent {
 public:
  virtual void Foo() {}
  virtual void FooNotOverridden() {}
};

class Derived : public Parent {
 public:
  void Foo() override {}
};

int main() {
  Parent p1, p2;
  Derived d1, d2;

  std::cout << "done" << std::endl;
}
```





Here’s what we learned from the above:

1、Even though the classes have no data members, there’s a hidden pointer to a vtable;

2、vtable for `p1` and `p2` is the same. vtables are static data per-type;

3、`d1` and `d2` inherit a vtable-pointer from `Parent` which points to `Derived`’s vtable;

> NOTE: 
> 1、这要如何理解？

4、All vtables point to an offset of 16 (`0x10`) bytes into the vtable. We’ll also discuss this later.