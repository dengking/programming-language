# csdn [C++ 虚函数和虚函数表详解（底层分析、代码测试）](https://blog.csdn.net/qq_24447809/article/details/110825780)

## 概述

C++编译器是保证虚函数表的指针存在于对象实例中最前面的位置(是为了保证取到虚函数表的最高的性能)，这样我们就能通过已经实例化的对象的地址得到这张虚函数表，再遍历其中的函数指针，并调用相应的函数。

> NOTE: 
>
> 1、这段话的内容是和 wikipedia Virtual method table的内容相矛盾的:
>
> > Many compilers place the virtual table pointer as the last member of the object; other compilers place it as the first; portable source code works either way.[[2\]](https://en.wanweibaike.com/wiki-Virtual table#cite_note-2) For example, [g++](https://en.wanweibaike.com/wiki-G%2B%2B) previously placed the pointer at the end of the object.[[3\]](https://en.wanweibaike.com/wiki-Virtual table#cite_note-3)
>
> 

**需要注意的几点：**
（1）同一个类的对象共享一个虚函数表。

（2）如果类存在虚函数表（有虚函数），那么这个类地址的起始位存放**虚函数表的地址**。

（3）子类继承父类虚函数表中的虚函数地址数据，不是同一张表，如果在子类中重写继承的虚函数，会覆盖虚函数表中相应的函数地址。

以上证明见下面分析：

## Visual调试分析

### （1）创建一个基类

```C++
class person
{
public:
	virtual void work();
	virtual void eat();
	virtual void money();
	void sleep();  //只是声明静态成员，函数存放在代码段，并不占用这个类的空间
private:
	int id=2;
};

```

输入指令，此指令可以查看person类的内存分配

```SHELL
cl /d1 reportSingleClassLayoutperson test.cpp
```



#### Memory layout

测试结果：

![在这里插入图片描述](E:\github\programming-language\docs\C++\Language-reference\Classes\Subtyping-polymorphism\TODO-Implementation\CSDN-C++虚函数和虚函数表详解\20201207202727940.png)



可以看出`person`类有一个虚函数表指针`vfptr`，指向的虚函数表vftable中有我们定义好的3个虚函数，注意每个虚函数函数前面加了作用域`person::`。

#### 直观图

直观的关系如图所示：

![在这里插入图片描述](E:\github\programming-language\docs\C++\Language-reference\Classes\Subtyping-polymorphism\TODO-Implementation\CSDN-C++虚函数和虚函数表详解\20201207211501232.png)

### （2）再创建一个子类teacher，重写了work虚函数

```C++
class teacher :public person
{
public:
	virtual void work();
};
```



输入指令查看teacher子类内存分配

```SHELL
cl /d1 reportSingleClassLayoutteacher test.cpp
```



测试结果：

![在这里插入图片描述](E:\github\programming-language\docs\C++\Language-reference\Classes\Subtyping-polymorphism\TODO-Implementation\CSDN-C++虚函数和虚函数表详解\20201207211338653.png)如图，



子类继承了父类的id和虚函数表的函数指针数据（注意，并不是和父类同一个虚函数表，见下面代码测试），因为重写了work函数，work函数前面作用域变为了`teacher::`。



## 代码测试

```C++
#include<iostream>
using namespace std;

class person
{
public:
	person(int n = 0)
	{
		id = n;
	}
	;
	virtual void work();
	virtual void eat();
	virtual void money();
	void sleep();  //只是声明静态成员，函数存放在代码段，并不占用这个类的空间
private:
	int id = 2;
};

void person::money()
{
	cout << "person_money" << endl;
}
void person::sleep()
{
	std::cout << "person_sleep" << std::endl;
}
void person::work()
{
	std::cout << "person_work" << std::endl;
}
void person::eat()
{
	std::cout << "person_eat" << std::endl;
}

class teacher: public person
{
public:
	virtual void work();
};
void teacher::work()
{
	cout << "teacher_work" << endl;
}

class student: public person
{
public:
	void stufunc();
};
void student::stufunc()
{
	cout << "student func" << endl;
}

int main()
{
	person p1;
	int *person_addr = (int*) &p1;
	cout << "基类对象地址" << person_addr << endl;

	int *virTableptr = (int*) *(person_addr + 0); //对象地址最开始一位指向虚函数表
	cout << "虚函数表地址：" << virTableptr << endl;

	/*虚函数表中每一位存有函数指针，即函数入口地址*/
	int *func1 = (int*) *(virTableptr + 0);    //虚函数指针强转为int* ，方便显示
	void (*func1_ac)() = (void(*)()) func1;	  //可执行函数指针

	int *func2 = (int*) *(virTableptr + 1);
	void (*func2_ac)() = (void(*)()) func2;

	int *func3 = (int*) *(virTableptr + 2);
	void (*func3_ac)() = (void(*)()) func3;

	cout << "person_func:" << endl;
	cout << "work_addr:" << func1 << endl;  //打印函数地址
	func1_ac();  //函数执行

	cout << "eat_addr:" << func2 << endl;
	func2_ac();

	cout << "money_addr:" << func3 << endl;
	func3_ac();

	/*下面测试teacher类*/
	cout << endl;
	cout << "---------------------------" << endl;
	cout << "覆盖虚函数子类teacher测试" << endl;
	cout << "---------------------------" << endl;
	teacher p2;
	int *teacher_addr = (int*) &p2;
	cout << "子类teacher对象地址" << teacher_addr << endl;

	int *teacher_virTable = (int*) *(teacher_addr + 0);
	cout << "子类teacher虚函数表地址" << teacher_virTable << endl;

	cout << "teacher func" << endl;

	int *tfunc1 = (int*) *(teacher_virTable + 0);
	void (*tfunc1_ac)() = (void(*)())tfunc1;
	cout << "teacher_work_addr:" << tfunc1 << endl;
	tfunc1_ac();

	int *tfunc2 = (int*) *(teacher_virTable + 1);
	void (*tfunc2_ac)() = (void(*)())tfunc2;
	cout << "teacher_eat_addr:" << tfunc2 << endl;
	tfunc2_ac();

	int *tfunc3 = (int*) *(teacher_virTable + 2);
	void (*tfunc3_ac)() = (void(*)())tfunc3;
	cout << "teacher_money_addr:" << tfunc3 << endl;
	tfunc3_ac();

	/*测试student类*/
	cout << endl;
	cout << "---------------------------" << endl;
	cout << "未覆盖虚函数子类student测试" << endl;
	cout << "---------------------------" << endl;
	student p3;
	int *stu_addr = (int*) &p3;
	cout << "子类student对象地址:" << stu_addr << endl;

	int *stu_virtable = (int*) *(stu_addr + 0);
	cout << "子类student虚函数表地址:" << stu_virtable << endl;

	/*测试person同类*/
	cout << endl;
	cout << "---------------------------" << endl;
	cout << "同类person对象测试" << endl;
	cout << "---------------------------" << endl;
	person p4;
	int *p4_addr = (int*) &p4;
	cout << "同类person对象地址:" << p4_addr << endl;

	int *p4_virtable = (int*) *(p4_addr + 0);
	cout << "同类person虚函数表地址:" << p4_virtable << endl;

	return 0;
}
// g++ -Wall -pedantic test.cpp -g


```

