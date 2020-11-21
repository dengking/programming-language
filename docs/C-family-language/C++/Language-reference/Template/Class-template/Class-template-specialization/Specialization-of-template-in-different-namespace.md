# Specialization of template in different namespace

在使用g++的时候，遇到过这样的compiler error。根据: 

- stackoverflow [warning: specialization of template in different namespace](https://stackoverflow.com/questions/25594644/warning-specialization-of-template-in-different-namespace)# [A](https://stackoverflow.com/a/25594741) (下面收录了这篇文章)
- stackoverflow [Specialization of template in different namespace](https://stackoverflow.com/questions/25311512/specialization-of-template-in-different-namespace) # [A](https://stackoverflow.com/a/25311919)

可知，这是g++ bug。

## stackoverflow [warning: specialization of template in different namespace](https://stackoverflow.com/questions/25594644/warning-specialization-of-template-in-different-namespace)

With the following piece of code I get the warning:

```C++
warning: specialization of ‘template<class _Iterator> struct std::iterator_traits’ in different namespace [-fpermissive]
template<> class std::iterator_traits<Token_ptr>{
public:
    typedef Word difference_type;
    typedef Word value_type;
    typedef Token_ptr pointer;
    typedef Word& reference ;
    typedef std::bidirectional_iterator_tag iterator_category ;
};
```

While everything works correctly, does any body know what exactly means and why is issued the warning. ( g++ issues the warning while clang++ doesn't ).

[A](https://stackoverflow.com/a/25594741)

Assuming that you are compiling this in C++11 mode (since clang gave no warning) and that this specialization is in the global namespace, then there's nothing wrong with your code. It's a g++ bug. §14.7.3 [temp.expl.spec]/p2:

> An explicit specialization shall be declared in a namespace enclosing the specialized template. An explicit specialization whose *declarator-id* is not qualified shall be declared in the nearest enclosing namespace of the template, or, if the namespace is inline (7.3.1), any namespace from its enclosing namespace set. Such a declaration may also be a definition. If the declaration is not a definition, the specialization may be defined later (7.3.1.2).

The global namespace is a "namespace enclosing the specialized template", and your *declarator-id* is qualified with `std::`, so the second sentence doesn't apply. As a workaround, you can do what [cdhowie](https://stackoverflow.com/a/25594681/2756719)'s answer suggests - i.e., opening a `namespace std` block and putting the specialization there.

See [CWG issue 374](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#374) and [GCC bug 56480](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480).

[A](https://stackoverflow.com/a/25594681)

There is one visible problem here, and one potentially invisible problem. The visible problem is that this code is not in a `namespace std` block. To specialize a template, you need to be within the namespace of the template you are specializing. Providing the namespace name as part of the type doesn't actually work in this case. So you need to do this:

```cpp
namespace std
{
    template<> class iterator_traits<Token_ptr>{
    public:
        typedef Word difference_type;
        typedef Word value_type;
        typedef Token_ptr pointer;
        typedef Word& reference ;
        typedef std::bidirectional_iterator_tag iterator_category ;
    };
}
```

The other potential problem is that this code may already be inside of another `namespace` block (I can't tell because this is presumably not the entire source file). If it is, you need to close that namespace block first so that `namespace std` is not nested within any other namespace blocks.

Further reading: [Specialization of 'template struct std::less' in different namespace](https://stackoverflow.com/q/2282349/501250)



## stackoverflow [Specialization of template in different namespace](https://stackoverflow.com/questions/25311512/specialization-of-template-in-different-namespace)

> NOTE: 下面是对原文的代码的整理，将它放到了同一个文件中

```C++
#include <iostream>
#include <map>
#include <cstring>
namespace MyLib
{

#define DECLARE_ENUM( type ) template<> std::map<const char*, type>  \
            MyLib::Enum<type>::mMap = std::map<const char*, type>(); \
            template<> MyLib::Enum<type>::Enum (void)

template<typename Type> class Enum
{
private:
    // constructor
	Enum(void);

public:
	static int Size(void)
	{ 
        /* ... */
		return 0;
	}

private:
	static std::map<const char*, Type> mMap;
};

}
enum MyEnum
{
	value1, value2, value3,
};

DECLARE_ENUM (MyEnum)
{
	mMap["value1"] = value1;
	mMap["value2"] = value2;
	mMap["value3"] = value3;
}

void SomeFunc(void)
{
	std::cout << MyLib::Enum<MyEnum>::Size();
}
int main()
{
}
// g++ --std=c++11 test.cpp


```

> NOTE: 上述代码的实现是非常值得借鉴的，每个enum都有一个对应的`Enum`类，这个类保存了enum的一个mapping: name to value

g++ gives me a "Specialization of template in different namespace" error. 



[A](https://stackoverflow.com/a/25311919)

This changed in C++11 as a result of [CWG issue 374](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#374) and [N3064](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2010/n3064.pdf). The current wording (§14.7.3 [temp.expl.spec]/p2) is:

> An explicit specialization shall be declared in a namespace enclosing the specialized template. An explicit specialization whose *declarator-id* is not qualified shall be declared in the nearest enclosing namespace of the template, or, if the namespace is inline (7.3.1), any namespace from its enclosing namespace set.

Since your *declarator-id* is in fact qualified with `MyLib::`, and the global namespace is a "namespace enclosing the specialized template", this looks like a GCC bug ([bug 56480](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480)). Your code compiles fine with [clang in C++11 mode](http://coliru.stacked-crooked.com/a/d27b20fe26d6fc21).

In C++98, however, the specialization must be put inside namespace in which the template is a member (see Mark B's comment below), and clang will produce a warning if [put in C++98 mode](http://coliru.stacked-crooked.com/a/1a12ca90d3c7033c).

> NOTE: 下面是正确版本的代码

```C++
#include <iostream>
#include <map>
#include <cstring>
namespace MyLib
{

#define DECLARE_ENUM( type ) template<> std::map<const char*, type>  \
            MyLib::Enum<type>::mMap = std::map<const char*, type>(); \
            template<> MyLib::Enum<type>::Enum (void)

template<typename Type> class Enum
{
public:
	Enum(void);
	static int Size(void)
	{ /* ... */
		return mMap.size();
	}

private:
	static std::map<const char*, Type> mMap;
};

}

enum MyEnum
{
	value1, value2, value3,
};

namespace MyLib
{
DECLARE_ENUM (MyEnum)
{
	mMap["value1"] = value1;
	mMap["value2"] = value2;
	mMap["value3"] = value3;
}
}
void SomeFunc(void)
{
	std::cout << MyLib::Enum<MyEnum>::Size() << std::endl;
}
// 触发构造
static MyLib::Enum<MyEnum> g_E;

int main()
{

	SomeFunc();
}
// g++ --std=c++11 test.cpp


```

> NOTE: 输出为:
>
> ```
> 3
> ```
>
> 

## Example code

下面是一个简单的完整的代码，源自: stackoverflow [Why aren't template specializations allowed to be in different namespaces?](https://stackoverflow.com/questions/3072248/why-arent-template-specializations-allowed-to-be-in-different-namespaces) : 

```C++
#include <iostream>
namespace first
{
template<class T>
class myclass
{
	T t;
public:
	void who_are_you() const
	{
		std::cout << "first::myclass";
	}
};
}
namespace second
{
using first::myclass;
template<>
class myclass<int>
{
	int i, j;
public:
	void who_are_you() const
	{
		std::cout << "second::myclass";
	}
};
}

int main()
{
	second::myclass c;
	c.who_are_you();
}
// g++ --std=c++11 test.cpp



```

compile error:

```C++
test.cpp:19:7: error: specialization of ‘template<class T> class first::myclass’ in different namespace [-fpermissive]
 class myclass<int>
       ^
test.cpp:5:7: error:   from definition of ‘template<class T> class first::myclass’ [-fpermissive]
 class myclass
       ^
test.cpp:23:21: error: definition of ‘void first::myclass<int>::who_are_you() const’ is not in namespace enclosing ‘first::myclass<int>’ [-fpermissive]
  void who_are_you() const
                     ^
test.cpp: In function ‘int main()’:
test.cpp:32:18: error: missing template arguments before ‘c’
  second::myclass c;
                  ^
test.cpp:32:18: error: expected ‘;’ before ‘c’
test.cpp:33:2: error: ‘c’ was not declared in this scope
  c.who_are_you();
  ^

```

