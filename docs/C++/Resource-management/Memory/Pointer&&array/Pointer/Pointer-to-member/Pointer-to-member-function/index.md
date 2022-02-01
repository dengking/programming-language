# Pointers to Member Functions



## isocpp FAQ Pointers to Member Functions [¶](https://isocpp.org/wiki/faq/pointers-to-members) [Δ](https://isocpp.org/wiki/faq/pointers-to-members#)



## Example

下面的例子展示了pointers to Member Functions的用法。

### sourcemaking [Adapter in C++: External Polymorphism](https://sourcemaking.com/design_patterns/adapter/cpp/2)

1. Specify the new desired interface
2. Design a "wrapper" class that can "impedance match" the old to the new
3. The client uses (is coupled to) the new interface
4. The adapter/wrapper "maps" to the legacy implementation

```C++
#ifdef _MSC_VER
#include "stdafx.h"
#endif

#include <iostream>

using namespace std;

class ExecuteInterface
{
public:
	// 1. Specify the new interface
	virtual ~ExecuteInterface()
	{
	}
	virtual void execute() = 0;
};

// 2. Design a "wrapper" or "adapter" class
template<class TYPE>
class ExecuteAdapter: public ExecuteInterface
{
public:
	ExecuteAdapter(TYPE *o, void (TYPE::*m)())
	{
		object = o;
		method = m;
	}
	~ExecuteAdapter()
	{
		delete object;
	}

	// 4. The adapter/wrapper "maps" the new to the legacy implementation
	void execute()
	{ /* the new */
		(object->*method)();
	}
private:
	TYPE *object; // ptr-to-object attribute

	void (TYPE::*method)(); /* the old */     // ptr-to-member-function attribute
};

// The old: three totally incompatible classes
// no common base class,
class Fea
{
public:
	// no hope of polymorphism
	~Fea()
	{
		cout << "Fea::dtor" << endl;
	}
	void doThis()
	{
		cout << "Fea::doThis()" << endl;
	}
};

class Feye
{
public:
	~Feye()
	{
		cout << "Feye::dtor" << endl;
	}
	void doThat()
	{
		cout << "Feye::doThat()" << endl;
	}
};

class Pheau
{
public:
	~Pheau()
	{
		cout << "Pheau::dtor" << endl;
	}
	void doTheOther()
	{
		cout << "Pheau::doTheOther()" << endl;
	}
};

/* the new is returned */
ExecuteInterface** initialize()
{
	ExecuteInterface **array = new ExecuteInterface*[3];

	/* the old is below */
	array[0] = new ExecuteAdapter<Fea>(new Fea(), &Fea::doThis);
	array[1] = new ExecuteAdapter<Feye>(new Feye(), &Feye::doThat);
	array[2] = new ExecuteAdapter<Pheau>(new Pheau(), &Pheau::doTheOther);
	return array;
}

int main()
{
	ExecuteInterface **objects = initialize();
	for (int i = 0; i < 3; i++)
	{
		objects[i]->execute();
	}

	// 3. Client uses the new (polymporphism)
	for (int i = 0; i < 3; i++)
	{
		delete objects[i];
	}
	delete objects;
	return 0;
}
// g++ -Wall -pedantic test.cpp && ./a.out

```

> NOTE: 
>
> 1、上面是修正版，原文的版本是存在错误的
>
> 2、运行结果如下:
>
> ```C++
> fax & print something
> print internal printJob
> ```
>
> 3、上述例子是不及 informit [Design Patterns with Signature-Based Polymorphism: A C++ Approach](https://www.informit.com/articles/article.aspx?p=700594) # External Polymorphism 的
>
> 4、上述例子运用了: 
>
> a、generic  invoke、call pointer to member function
>
> b、OOP interface + template implementation




