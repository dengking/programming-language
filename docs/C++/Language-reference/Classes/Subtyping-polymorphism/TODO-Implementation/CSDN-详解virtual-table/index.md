# csdn [详解virtual table](https://blog.csdn.net/wangweixaut061/article/details/7019994) 

> NOTE: 
>
> 1、C++并没有统一实现，因此不同的compiler的实现方式是不同的，本文所介绍的只是一种可能的实现方式，并不能够代表所有

C++中的虚函数的作用主要是实现了**多态**的机制。关于多态，简而言之就是用父类型别的指针指向其子类的实例，然后通过父类的指针调用实际子类的成员函数。这种技术可以让父类的指针有“多种形态”，这是一种泛型技术。所谓泛型技术，说白了就是试图使用不变的代码来实现可变的算法。比如：模板技术，RTTI技术，虚函数技术，要么是试图做到在编译时决议，要么试图做到运行时决议。

> NOTE: 
>
> 1、上面这段话中所述的"泛型技术"，并不准确



## 虚函数表

对C++ 了解的人都应该知道虚函数（Virtual Function）是通过一张虚函数表（Virtual Table）来实现的。简称为V-Table。 在这个表中，主是要一个类的**虚函数的地址表**，这张表解决了继承、覆盖的问题，保证其能真实反应实际的函数。这样，在有虚函数的类的实例中这个表被分配在了 这个实例的内存中，所以，当我们用父类的指针来操作一个子类的时候，这张虚函数表就显得由为重要了，它就像一个地图一样，指明了实际所应该调用的函数。

这里我们着重看一下这张虚函数表。在C++的标准规格说明书中说到，编译器必需要保证**虚函数表的指针**存在于对象实例中最前面的位置（这是为了保证正确取到**虚函数的偏移量**）。 这意味着我们通过对象实例的地址得到这张虚函数表，然后就可以遍历其中函数指针，并调用相应的函数。

假设我们有这样的一个类：

```C++
class Base {

public:

virtual void f() { cout << "Base::f" << endl; }

virtual void g() { cout << "Base::g" << endl; }

virtual void h() { cout << "Base::h" << endl; }

};
```

按照上面的说法，我们可以通过`Base`的实例来得到虚函数表。 下面是实际例程：

```C++
#include <iostream>
using namespace std;
class Base
{

public:

	virtual void f()
	{
		cout << "Base::f" << endl;
	}

	virtual void g()
	{
		cout << "Base::g" << endl;
	}

	virtual void h()
	{
		cout << "Base::h" << endl;
	}

};
int main()
{
	typedef void (*Fun)(void);

	Base b;

	Fun pFun = NULL;

	cout << "虚函数表地址：" << (int*) (&b) << endl;

	cout << "虚函数表的第一个函数地址:" << (int*) *(int*) (&b) << endl; 

	// Invoke the first virtual function

	pFun = (Fun) *((int*) *(int*) (&b));

	pFun();
}

```

