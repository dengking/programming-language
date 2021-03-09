# Dump class memory layout

dump class memory layout是后续探索的一个重要工具。

## clang



### thegreenplace [Dumping a C++ object's memory layout with Clang](https://eli.thegreenplace.net/2012/12/17/dumping-a-c-objects-memory-layout-with-clang)



## g++



### stackoverflow [Print layout of C++ object with g++ compiler](https://stackoverflow.com/questions/2979606/print-layout-of-c-object-with-g-compiler)

Is there a way to print the layout of a C++ object using the g++ compiler or any other means. A simplified example (assuming int takes 4 bytes)

A

Looking at the man pages, `-fdump-class-hierarchy` maybe?



### 测试1: 单个polymorphic class

```c++
class person
{
public:
        virtual void work(){}
        virtual void eat(){}
        virtual void money(){}
        void sleep(){} //只是声明静态成员，函数存放在代码段，并不占用这个类的空间
private:
        int id=2;
};
int main()
{

}
// g++ test.cpp -fdump-class-hierarchy

```

输出如下:

```C++
Vtable for person
person::_ZTV6person: 5u entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI6person)
16    (int (*)(...))person::work
24    (int (*)(...))person::eat
32    (int (*)(...))person::money

Class person
   size=16 align=8
   base size=12 base align=8
person (0x0x7f15f84874e0) 0
    vptr=((& person::_ZTV6person) + 16u)
```

一、`ZTV`、`ZTI` 是什么含义？

在 shaharmike [C++ vtables - Part 1 - Basics](https://shaharmike.com/cpp/vtable-part1/) 中，对此进行了回答

> Note: we’re looking at demangled symbols. If you really want to know, _ZTV is a prefix for vtable, _ZTS is a prefix for type-string (name) and _ZTI is for type-info.

可以看到:

1、`ZTV` 中 `V` 的含义是 **v**table

2、`ZTI` 中 `I` 的含义是 type-**i**nfo

3、`ZTS` 中 `S` 的含义是 type-**s**tring

二、`vptr=((& person::_ZTV6person) + 16u)`中，为什么要`+ 16u`？

这要结合如下内容来看:

```C++
Vtable for person
person::_ZTV6person: 5u entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI6person)
16    (int (*)(...))person::work
24    (int (*)(...))person::eat
32    (int (*)(...))person::money
```

可以看到 `person::_ZTV6person` 的前两个entry不是virtual function的，所以需要 `+16` 以pass这两个entry。

关于这一点，在 shaharmike [C++ vtables - Part 1 - Basics](https://shaharmike.com/cpp/vtable-part1/) 中，也进行了分析



---

关于上述输出的解释可以参见:

1、shaharmike [C++ vtables - Part 1 - Basics](https://shaharmike.com/cpp/vtable-part1/)

### 测试2: polymorphic class inheritance

```C++
class person
{
public:
	virtual void work()
	{
	}
	virtual void eat()
	{
	}
	virtual void money()
	{
	}
	void sleep()
	{
	} //只是声明静态成员，函数存放在代码段，并不占用这个类的空间
private:
	int id = 2;
};

class teacher: public person
{
public:
	virtual void work() override
	{
	}
};
int main()
{

}
// g++ test.cpp -fdump-class-hierarchy

```

输出如下:

```C++
Vtable for person
person::_ZTV6person: 5u entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI6person)
16    (int (*)(...))person::work
24    (int (*)(...))person::eat
32    (int (*)(...))person::money

Class person
   size=16 align=8
   base size=12 base align=8
person (0x0x7f4e75a604e0) 0
    vptr=((& person::_ZTV6person) + 16u)

Vtable for teacher
teacher::_ZTV7teacher: 5u entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI7teacher)
16    (int (*)(...))teacher::work
24    (int (*)(...))person::eat
32    (int (*)(...))person::money

Class teacher
   size=16 align=8
   base size=12 base align=8
teacher (0x0x7f4e75a55138) 0
    vptr=((& teacher::_ZTV7teacher) + 16u)
  person (0x0x7f4e75a60540) 0
      primary-for teacher (0x0x7f4e75a55138)
```



## MSCV

参见: 

1、csdn [C++ 虚函数和虚函数表详解（底层分析、代码测试）](https://blog.csdn.net/qq_24447809/article/details/110825780)



## gdb

参见:

1、 [**C++ vtables - Part 1 - Basics**](https://shaharmike.com/cpp/vtable-part1/)