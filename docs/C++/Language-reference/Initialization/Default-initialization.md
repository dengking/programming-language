# Default initialization



## cppreference [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)

This is the initialization performed when a variable is constructed with **no initializer**.

> NOTE: [Value initialization (since C++03)](https://en.cppreference.com/w/cpp/language/value_initialization)是**empty initializer**。

### Syntax

> NOTE: 从non-class type 和 class type来进行区分

Default initialization is performed in three situations:

| syntax                               | situation                                                    |
| ------------------------------------ | ------------------------------------------------------------ |
| `T object ;`                         | a variable with <br>- automatic, <br>- static <br/>- thread-local [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) <br>is declared with no **initializer**; |
| `new T` <br>`new T ( )`(until C++03) | an object with **dynamic storage duration** <br>- is created by a [new-expression](https://en.cppreference.com/w/cpp/language/new) with no initializer or<br>- is created by a new-expression with the initializer consisting of an empty pair of parentheses (until C++03); |
|                                      | a base class or a **non-static data member** is not mentioned in a constructor [initializer list](https://en.cppreference.com/w/cpp/language/initializer_list) and that **constructor** is called. |



| 注解                                                         |
| ------------------------------------------------------------ |
| 需要注意的是，关于static、thread-local object，原文的NOTE段有补充说明，需要以它为准 |
| 需要注意的是，`new T ( )` (until C++03)，在C++03中，被认为是 [value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)，原文的后面进行了专门说明 |
| 意思是:对于**non-static data member**，如果在constructor [initializer list](https://en.cppreference.com/w/cpp/language/initializer_list)没有提起它，它的default constructor会被compiler调用 |



### Effect

The effects of **default initialization** are:

#### Class type

the constructors are considered and subjected to [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution) against the empty argument list. The constructor selected (which is one of the [default constructors](https://en.cppreference.com/w/cpp/language/default_constructor)) is called to provide the initial value for the new object;

> NOTE: 说白了就是default constructor被调用

#### Array type

every element of the array is default-initialized

#### Other type

nothing is done: the objects with automatic storage duration (and their subobjects) are initialized to indeterminate values.





> NOTE: 原文中的解释其实是非常难以理解的，在下面文章中有非常好的解释:
>
> akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)，收录在`C++\Language-reference\Initialization\Value-initialization.md`中了。
>
> 在原文的NOTE段，对上面的内容进行了比较直观易懂的解释:

### Notes

Default initialization of **non-class variable**s with **automatic** and **dynamic** storage duration produces objects with indeterminate values (static and thread-local objects get [zero initialized](https://en.cppreference.com/w/cpp/language/zero_initialization))

> NOTE: 也就是上面说的 nothing is done，为什么这样做呢？在akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)中给出的解释是:
>
> > Its value is **indeterminate** (no initialization is performed). This is so for **performance** reasons, because sometimes we do not need an initial value.

> NOTE: 关于static、thread-local object，下面的例子中有着更加详细的说明。



> NOTE: 也就是built-in type，是不允许default-initialized的。

Reference cannot be default-initialized.

```C++
#include <string>
#include <iostream>

struct T1
{
	int mem;
};

struct T2
{
	int mem;
	T2() // "mem" is not in the initializer list
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

int n; // static non-class, a two-phase initialization is done:
// 1) zero initialization initializes n to zero
// 2) default initialization does nothing, leaving n being zero

int main()
{
	int n;            // non-class, the value is indeterminate
	std::string s;    // class, calls default ctor, the value is "" (empty string)
	std::string a[2]; // array, default-initializes the elements, the value is {"", ""}
//  int& r;           // error: a reference
//  const int n;      // error: a const non-class
//  const T1 t1;      // error: const class with implicit default ctor
	T1 t1;            // class, calls implicit default ctor
	std::cout << t1.mem << std::endl;
	const T2 t2;      // const class, calls the user-provided default ctor
					  // t2.mem is default-initialized (to indeterminate value)
	std::cout << t2.mem << std::endl;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```C++
> 1963938480
> T2::T2()
> 125
> ```

#### Default initialization and `const`

> NOTE: 这是我添加的标题

If `T` is a const-qualified type, it must be a **class type** with a **user-provided default constructor**.

> NOTE: 这段话的意思是非常明显的，如果`T`是一个const-qualified type，那么它必须是a **class type** with a **user-provided default constructor**，否则就不能够使用default initialization。

```C++
#include <string>
#include <iostream>

struct T1
{
	int mem;
};

struct T2
{
	int mem;
	T2() // "mem" is not in the initializer list
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

int main()
{
	const int n2; // error: a const non-class
	const T1 t1; // error: const class with implicit default ctor
	const T2 t2;      // const class, calls the user-provided default ctor
					  // t2.mem is default-initialized (to indeterminate value)
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序编译报错如下:
>
> ```C++
> test.cpp: In function ‘int main()’:
> test.cpp:20:12: error: uninitialized const ‘n2’ [-fpermissive]
>   const int n2; // error: a const non-class
>             ^
> test.cpp:21:11: error: uninitialized const ‘t1’ [-fpermissive]
>   const T1 t1; // error: const class with implicit default ctor
>            ^
> test.cpp:4:8: note: ‘const struct T1’ has no user-provided default constructor
>  struct T1
>         ^
> test.cpp:6:6: note: and the implicitly-defined constructor does not initialize ‘int T1::mem’
>   int mem;
> 
> ```
>
> 

### Indeterminate value and UB (since C++14)

> NOTE: 原文并没有这样的标题，这个标题是我添加上的，意在表明本段的含义

Use of an indeterminate value obtained by default-initializing a non-class variable of any type is [undefined behavior](https://en.cppreference.com/w/cpp/language/ub) (in particular, it may be a [trap representation](https://en.cppreference.com/w/cpp/language/object#Object_representation_and_value_representation)), except in the following cases:

> NOTE: 在实际生产中这种UB发生的情况不多，因为一般的programmer都有着比较好的initialization意识。



## Summary

对于default initialization，直观感受是，它和default constructor对应；它不对built-in type进行 [zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization)，这是它和value initialization的主要差别。