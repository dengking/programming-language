# Value initialization

1、value initialization和default initialization一样，也是不带参数的，value initialization比default initialization多做了一点: 对于non-class type，对它们执行zero initialization。



## cppreference [Value initialization(since C++03)](https://en.cppreference.com/w/cpp/language/value_initialization)

### Syntax

| syntax                                                       | explanation                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `T()` 、<br>`T{}`                                            | a **nameless temporary object** is created <br>with the **initializer** consisting of an empty pair of parentheses or braces (since C++11); |
| `new T ()`、<br>`new T {}`                                   | an object with dynamic storage duration is created by a [new-expression](https://en.cppreference.com/w/cpp/language/new) <br>with the **initializer** consisting of an empty pair of parentheses or braces (since C++11); |
| `Class::Class(...) : member() { ... }`、<br>`Class::Class(...) : member{} { ... }` | a non-static data member or a base class is initialized using a **member initializer** <br>with an empty pair of parentheses or braces (since C++11); |
| `T object {};`                                               | a named variable (automatic, static, or thread-local) is declared with the initializer consisting of a pair of braces. (since C++11) |


> NOTE: 
>
> 1、`{}`是C++11引入的uniform initialization特性，参见  [list-initialization](https://en.cppreference.com/w/cpp/language/list_initialization)
>
> 2、我没有按照原文的方式，而是将相同内容进行了合并
>
> 3、需要注意的是，对于value initialization，它是不支持 `T object();` syntax的，因为compiler将它作为function declaration，参见后面的"Notes"章节

**Aggregate-initialization**

In all cases, if the empty pair of braces `{}` is used and `T` is an *aggregate type*, [aggregate-initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization) is performed instead of **value-initialization**.

> NOTE:
>
> 1、[aggregate-initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization) 和 [list-initialization](https://en.cppreference.com/w/cpp/language/list_initialization) 一致

**List-initialization**

If `T` is a class type that has no **default constructor** but has a constructor taking [std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list), [list-initialization](https://en.cppreference.com/w/cpp/language/list_initialization) is performed.

> NOTE: 如果既有default constructor，又有constructor taking [std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list)，那compiler如何选择呢？

### Effect

> NOTE: 按照在`C++\Language-reference\Initialization\Initialization`中总结的思路来进行整理。

#### Class type

> NOTE: 关于class type的initialization规则是最最冗杂的，下面结合了具体的例子来进行了说明。

**(until C++11)**

1) if `T` is a class type with at least one user-provided constructor of any kind, the [default constructor](https://en.cppreference.com/w/cpp/language/default_constructor) is called;

> NOTE: 这条规则和C++11中的相同

2) if `T` is a non-union class type without any user-provided constructors, every non-static data member and base-class component of T is value-initialized;

> NOTE: 和C++11中有什么差异呢？下面的2)中给出的例子就体现了两者的差异。

**(since C++11)**

1) if `T` is a class type  

a) with no **default constructor** or 

b) with a user-provided (**default constructor**) or

c) with a deleted **default constructor** 

the object is [default-initialized](https://en.cppreference.com/w/cpp/language/default_initialization);

> NOTE: 结合下面的具体例子来看，上述a)、c)都会导致编译报错，只有b)是能够编译通过的。这条规则其实描述的是当b)的时候，compiler只会这些user-define default constructor，而无法做任何事情，因为此时的default constructor是user-provided，compiler没有控制权，所以它不敢执行 [zero-initialization](https://en.cppreference.com/w/cpp/language/zero_initialization)。

2) if `T` is a class type with a **default constructor** that is neither user-provided nor deleted <br>(that is, it may be a class with an implicitly-defined (**default constructor**) or defaulted **default constructor**), the object is [zero-initialized](https://en.cppreference.com/w/cpp/language/zero_initialization) and then it is [default-initialized](https://en.cppreference.com/w/cpp/language/default_initialization) if it has a **non-trivial default constructor**;

> NOTE: 关于2)中的最后一句，在原文的Notes章节中有专门的解释，下面将它整理到了这一段中。

Since C++11, value-initializing a class without a user-provided constructor, which has a member of a class type with a user-provided constructor,  zeroes out the member before calling its constructor:

> NOTE: 显然先 [zero-initialization](https://en.cppreference.com/w/cpp/language/zero_initialization) 然后再 [default-initialization](https://en.cppreference.com/w/cpp/language/default_initialization) 是非常安全的，既能够保证zero-initialization又能够安全地执行default-initialization。

```C++
#include <string>
#include <vector>
#include <iostream>

struct A
{
	int i;
	A()
	{
	} // user-provided default ctor, does not initialize i
};

struct B
{
	A a;
};
// implicitly-defined default ctor

int main()
{
	std::cout << B().a.i << '\n'; // value-initializes a B temporary
								  // leaves b.a.i uninitialized in C++03
								  // sets b.a.i to zero in C++11
}
// g++ --std=c++11 test.cpp


```

> NOTE: 输出如下:
>
> ```
> 0
> ```





> NOTE: 下面例子对上面两条规则中描述的各种情况进行了说明:
>
> | default constructor                            | example     | effect                                                       |
> | ---------------------------------------------- | ----------- | ------------------------------------------------------------ |
> | with no **default constructor**                | `struct T2` | 编译报错，因为没有default constructor，所以不允许 `T2 t2{ };`<br>compile error: class with no default constructor |
> | with a user-provided (**default constructor**) | `struct T3` | 符合上述`1)`中的[default-initialized](https://en.cppreference.com/w/cpp/language/default_initialization)；从输出可以看到，user-provided default constructor被调用<br>由于在user-provided default constructor中没有初始化`mem1`，<br>所以它的value是indeterminate |
> | with a deleted **default constructor**         | `struct T5` | 编译报错，因为没有default constructor，所以不允许 `T5 t5{ };`<br/> |
> | implicitly-defined (**default constructor**)   | `struct T1` | 符合上述`2)`                                                 |
> | defaulted **default constructor**              | `struct T4` | 符合上述`2)`                                                 |
>
> 

```C++
#include <string>
#include <vector>
#include <iostream>

// implicit default constructor
struct T1
{
	int mem1;
	std::string mem2;
};

struct T2
{
	int mem1;
	std::string mem2;
	T2(const T2&) // user-provided copy constructor、no default constructor
	{
	}
};

struct T3
{
	int mem1;
	std::string mem2;
	T3() // user-provided default constructor
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

struct T4
{
	int mem1;
	std::string mem2;
	T4() = default;
};

struct T5
{
	int mem1;
	std::string mem2;
	T5() = delete;
};
int main()
{
	T1 t1 { }; // class with implicit default constructor =>
			   //     t1.mem1 is zero-initialized, the value is 0
			   //     t1.mem2 is default-initialized, the value is ""
//  T2 t2{ }; // error: class with no default constructor
	T3 t3 { }; // class with user-provided default constructor =>
			   //     t3.mem1 is default-initialized to indeterminate value
			   //     t3.mem2 is default-initialized, the value is ""
	T4 t4 { };

//	T5 t5 { }; // error: use of deleted function ‘T5::T5()’
	std::cout << t1.mem1 << ' ' << t3.mem1 << ' ' << t4.mem1 << '\n';
}
// g++ --std=c++11 test.cpp


```

> NOTE: 输出如下:
>
> ```
> T3::T3()
> 0 720930048 0
> ```







#### Array type

3) if `T` is an array type, each element of the array is value-initialized;



#### Other type

4) otherwise, the object is [zero-initialized](https://en.cppreference.com/w/cpp/language/zero_initialization).

> NOTE: 这是value initialization和default initialization的重要差异所在。

### Notes

> NOTE: 原文这一段中关于`()`和`{}`的讨论，都收录到了`C++\Language-reference\Initialization\Uniform-initialization.md`中。

A constructor is *user-provided* if it is user-declared and not explicitly defaulted on its first declaration.

> NOTE: “not explicitly defaulted on its first declaration”的含义是:
>
> ```C++
> class C
> {
> public:
> C()=default;
> }
> ```
>
> 这是C++11引入的新特性。





#### Syntax ambiguity in value initialization and function declaration

> NOTE:
>
> 这部分内容移到了"Syntax-ambiguity-in-initialization-with-round-parenthese"章节







## Example 

```c++
#include <iostream>

struct Test
{
	int a;
	int b;
};

class C
{

public:
	Test t;
};

int main()
{
	C c;
	std::cout << c.t.a << std::endl;
	std::cout << c.t.b << std::endl;
}
```

上述例子是不使用zero initialization，可以看到`c.t.a`、`c.t.b`都是随机值。



```c++
#include <iostream>

struct Test
{
	int a;
	int b;
};

class C
{

public:
	Test t { };
};

int main()
{
	C c;
	std::cout << c.t.a << std::endl;
	std::cout << c.t.b << std::endl;
}
```

上述例子是使用zero initialization，可以看到`c.t.a`、`c.t.b`都是默认值0。

作为对比，可以看看下面的例子：

```c++
#include <iostream>

struct Test
{
int a;
int b;
};


class C
{

public:
    Test t {1};

};

int main()
{
    C c;
    std::cout<<c.t.a<<std::endl;
    std::cout<<c.t.b<<std::endl;
}

```

输出如下：

```
1
0
```



作为对比，看看涉及[Value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)的例子：

```c++
// C++ code to demonstrate zero initialisation 

#include <iostream> 
#include <string> 

struct foo { 
	int x, y, z; 
}; 

double f[3]; // zero-initialized to three 0.0's 

int* p; // zero-initialized to null pointer value 

// zero-initialized to indeterminate value 
// then default-initialized to "" 
std::string s; 

int main(int argc, char* argv[]) 
{ 
	foo x = foo(); 
	
	std::cout << x.x << x.y << x.z << '\n'; 
	
	return 0; 
} 

```

输出为:

```
000
```

