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





## Template pointer to member function

1、github [macmade](https://github.com/macmade)/**[CPPTraits](https://github.com/macmade/CPPTraits)**

```C++
#define XS_TYPE_TRAITS_DEF_HAS_PUBLIC_METHOD( _N_, _F_ )                                                        \
    template< typename _T_, typename _R_, typename ... _A_ >                                                    \
    class _N_                                                                                                   \
    {                                                                                                           \
        private:                                                                                                \
                                                                                                                \
            struct _Y { char _[ 1 ]; };                                                                         \
            struct _N { char _[ 2 ]; };                                                                         \
                                                                                                                \
            template< typename _U_, _U_ > struct _S;                                                            \
                                                                                                                \
            template< typename _C_ > static _Y & _T( _S< _R_ ( _C_::* )( _A_ ... ), ( &_C_::_F_ ) > * );        \
            template< typename _C_ > static _N & _T( ... );                                                     \
                                                                                                                \
        public:                                                                                                 \
                                                                                                                \
            enum { value = sizeof( _T< _T_ >( nullptr ) ) == sizeof( _Y ) };                                    \
    }

```



2、cppreference [std::is_member_function_pointer](https://en.cppreference.com/w/cpp/types/is_member_function_pointer)

```C++
#include<type_traits>
template<class T>
struct is_member_function_pointer_helper: std::false_type
{
};

template<class T, class U>
struct is_member_function_pointer_helper<T U::*> : std::is_function<T>
{
};

template<class T>
struct is_member_function_pointer: is_member_function_pointer_helper<typename std::remove_cv<T>::type>
{
};

```



3、sourcemaking [Adapter in C++: External Polymorphism](https://sourcemaking.com/design_patterns/adapter/cpp/2)

4、[Check if a class has a member function of a given signature](https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature) # [A](https://stackoverflow.com/a/87846)

```C++
template<typename T>
struct HasUsedMemoryMethod
{
    template<typename U, size_t (U::*)() const> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, &U::used_memory>*);
    template<typename U> static int Test(...);
    static const bool Has = sizeof(Test<T>(0)) == sizeof(char);
};

template<typename TMap>
void ReportMemUsage(const TMap& m, std::true_type)
{
        // We may call used_memory() on m here.
}
template<typename TMap>
void ReportMemUsage(const TMap&, std::false_type)
{
}
template<typename TMap>
void ReportMemUsage(const TMap& m)
{
    ReportMemUsage(m, 
        std::integral_constant<bool, HasUsedMemoryMethod<TMap>::Has>());
}
```

这篇文章，收录在了`Detection-idiom`章节了。