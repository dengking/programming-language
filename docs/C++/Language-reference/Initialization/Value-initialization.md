# Value initialization



## akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)

Some time ago, I showed how `boost::value_initialized` can be used to value-initialize objects in generic components in C++03. In C++11 it is practically not necessary. C++11 gives you a way to force **value-initialization** of your objects in any possible initialization context.

> NOTE: 这就是C++11的uniform initialization，参见`C++\Language-reference\Initialization\Uniform-initialization.md`

### Value initialization

> NOTE: 本节其实重点是区分value initialization和default initialization。

Are you familiar with term “value initialization”? It is sometimes confused with “default-initialization”. In the following declaration:

```C++
#include <string>
#include <mutex>
#include <iostream>

struct widget
{
	int x;
	int y;
	std::string title;
};

void default_initialization()
{
	int i;         // default-initialize i
	std::cout << "i:" << i << std::endl;
	std::mutex m;  // default-initialize m
	widget w;      // default-initialize w
	std::cout << "w.x:" << w.x << std::endl;
	std::cout << "w.y:" << w.y << std::endl;
	std::cout << "w.title:" << w.title << std::endl;
	int a[9];      // default-initialize a
	std::cout << "a[0]:" << a[0] << std::endl;
	std::cout << "a[1]:" << a[1] << std::endl;
	std::cout << "a[2]:" << a[2] << std::endl;
	std::cout << "a[3]:" << a[3] << std::endl;
}

int main()
{
	default_initialization();

	return 0;
}
// g++ --std=c++11 test.cpp


```

> NOTE: 上述程序的输出如下:
>
> ```C++
> i:0
> w.x:2
> w.y:0
> w.title:
> a[0]:256
> a[1]:64
> a[2]:4196512
> a[3]:0
> 
> ```

`i` is default-initialized. Its value is **indeterminate** (no initialization is performed). This is so for **performance** reasons, because sometimes we do not need an initial value. For instance:

```C++
int i;         // uninitialized
std::cin >> i; // read initial value
```

Let’s continue with our example: `m` is also **default initialized**. But because `std::mutex` provides a **default constructor**, this constructor is chosen for **default-initialization**.
For `w`, because type `widget` does not provide a **custom default constructor**, an **implicit default constructor** is generated (and called here) by the compiler: it calls **default constructors** for member sub-objects, which perform no initialization for `x` and `y` (as they are `int`s) and calls **user-provided default constructor** for `title`. For `a`, each element of the array is default-initialized (left in **indeterminate state**).

As we can see, for **built-in types**, **default-initialization** leaves the objects in **indeterminate state**. **Value-initialization** enables us to tell that for **built-in types**, objects should be initialized with value `0`. 

> NOTE: 上面已经说明了default-initialization和value-initialization的一个重要差异: 对于built-in type:
>
> | Initialization         |                                                              |
> | :--------------------- | ------------------------------------------------------------ |
> | Default-initialization | **default-initialization** leaves the objects in **indeterminate state** |
> | Value-initialization   | objects should be initialized with value `0`                 |
>
> 

#### Initialization list of the constructor

We were able to use it in C++03 in certain contexts:

```C++
#include <string>
#include <mutex>
#include <iostream>

struct widget
{
	int x;
	int y;
	std::string title;
};

struct value_initialization
{
	int i;
	std::mutex m;
	widget w;
	int a[9];

	value_initialization() :
			i() // value-initialize i
			, m() // value-initialize m
			, w() // value-initialize w
			, a() // value initialize a
	{
	}
};

int main()
{
	value_initialization v;
	std::cout << "i:" << v.i << std::endl;
	std::cout << "w.x:" << v.w.x << std::endl;
	std::cout << "w.y:" << v.w.y << std::endl;
	std::cout << "w.title:" << v.w.title << std::endl;
	std::cout << "a[0]:" << v.a[0] << std::endl;
	std::cout << "a[1]:" << v.a[1] << std::endl;
	std::cout << "a[2]:" << v.a[2] << std::endl;
	std::cout << "a[3]:" << v.a[3] << std::endl;
	return 0;
}
// g++ --std=c++11 test.cpp


```

> NOTE: 上述程序的输出如下:
>
> ```C++
> i:0
> w.x:0
> w.y:0
> w.title:
> a[0]:0
> a[1]:0
> a[2]:0
> a[3]:0
> ```

In **initialization list of the constructor** `()` means “value-initialize”. That is: initialize `i` with value 0. For `m`, call its **default constructor**. For `w`, its members `x` and `y` are initialized with value `0`, and for `title` we call `string`’s default constructor. Integers in `a` are initialized with value `0`.

#### Automatic objects

If we wanted to trigger such **value initialization** for **automatic objects** in C++03, it is possible, but we have to select the appropriate **initialization syntax** for each object:

```c++
void manual_value_initialization()
{
  int i = 0;     // "=0" for scalars
  std::mutex m;  // nothing here
  widget w = {}; // aggregate initialization
  int a[9] = {}; // aggregate initialization
}
```

But now, what do you do if you are in a function template:

```C++
template <typename T>
void manual_value_initialization()
{
  T t = /* TYPE WHAT ? */;
}
```

Since `T` can be `int`, `std::mutex`, `widget` or `int[9]`, which syntax do you pick for **value-initialization**? None will work for all four types.

### Value-initialization syntax

In C++11 the last problem is solved with the “brace” syntax:

```C++
template <typename T>
void cpp11_value_initialization()
{
  T t {}; // value-initialize
}
```

`{}` just means “value-initialize”, but unlike `()`, it works in any initialization context.

Going back to our example with class called `value_initialization`, it is risky to put built-in types as members in a class, because you may forget to call their **value initialization** in the constructor(s). Especially, when you add a new member, and the body of the constructor is in another “cpp” file, you may forget to add **value-initialization** in all constructors. In C++11, you can fix that problem:

```C++
struct cpp11_value_initialization
{
  int i {};
  std::mutex m {};
  widget w {};
  int a[9] {};
 
  // no default constructor required
};
```

> NOTE: if we used `()` to “initialize” `member` in the last example, we would be in fact declaring a member function. This is why initialization with `()` is not uniform.
>
> 显然，使用`()`无法实现initialization grammar的uniform。

Syntax `int i {};` here means, “unless specified otherwise in the constructor, value-initialize member `i` upon initialization.” Similarly, in “generic context”:

```C++
template <typename T>
class C
{
  T member {};
  // ...
};
```

Note that if a class defines (implicitly or not) a **default constructor**, **empty braces** are *not* interpreted as **zero-element initializer list** (see [this post](https://akrzemi1.wordpress.com/2013/05/14/empty-list-initialization/) for more details).

> NOTE: 下面是作者添加的更新:
>
> **Update.** I made an error in the original text: empty braces are sometimes treated as a zero-size initializer list.

This is why the braced initialization syntax is called “uniform initialization”: because it works the same in different (all) initialization contexts: in constructor initialization list, for declaring automatic, temporary, global, heap-allocated variables, for class members. Note that if we used `()` to “initialize” `member` in the last example, we would be in fact declaring a member function. This is why initialization with `()` is not uniform.

“Uniform” does not mean that `()` and `{}` will do the same thing. For an example, see [this post](https://akrzemi1.wordpress.com/2013/06/05/intuitive-interface-part-i/).

The empty brace syntax enables a funny-looking way of creating *tags*. A tag is an empty structure that does not store any value but whose type is used for selecting overloaded constructors of functions. You can find them in the Standard Library, for instance `std::allocator_arg`, `std::nothrow`, `std::piecewise_construct`, `std::defer_lock`. Such tags can be declared as one-liners:

```c++
constexpr struct allocator_arg_t{} allocator_arg{};
```

The first empty braces indicate that class `allocator_arg_t` has empty definition; the second indicates that we are **value-initializing** a global constant.



## cppreference [Value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)

### Syntax

| syntax                                                       | explanation                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `T()` 、<br>`T{}`                                            | a **nameless temporary object** is created <br>with the **initializer** consisting of an empty pair of parentheses or braces (since C++11); |
| `new T ()`、<br>`new T {}`                                   | an object with dynamic storage duration is created by a [new-expression](https://en.cppreference.com/w/cpp/language/new) <br>with the **initializer** consisting of an empty pair of parentheses or braces (since C++11); |
| `Class::Class(...) : member() { ... }`、<br>`Class::Class(...) : member{} { ... }` | a non-static data member or a base class is initialized using a **member initializer** <br>with an empty pair of parentheses or braces (since C++11); |
| `T object {};`                                               | a named variable (automatic, static, or thread-local) is declared with the initializer consisting of a pair of braces. (since C++11) |


> NOTE: `{}`是C++11引入的uniform initialization特性，参见`C++\Language-reference\Initialization\Uniform-initialization.md`。

In all cases, if the empty pair of braces `{}` is used and `T` is an *aggregate type*, [aggregate-initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization) is performed instead of **value-initialization**.

If `T` is a class type that has no **default constructor** but has a constructor taking [std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list), [list-initialization](https://en.cppreference.com/w/cpp/language/list_initialization) is performed.

> NOTE: 如果既有default constructor，又有constructor taking [std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list)，那compiler如何选择呢？

### Effect

> NOTE: 按照在`C++\Language-reference\Initialization\Initialization.md`中总结的思路来进行整理。

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



## SUMMARY

对于value initialization，直观的感觉是，它重要描述的是compiler的default behavior。