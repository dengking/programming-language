# akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)

> NOTE:
>
> 1、这篇文章对比了value initialization 和 default initialization
>
> 2、遵循 CppCoreGuidelines [ES.20: Always initialize an object](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-always)，阅读完成后，我的一个想法是: 我们总是需要value initialization，C++11 的 uniform initialization `{}` 让我们以uniform、consistent syntax来实现 value initialization。

Some time ago, I showed how `boost::value_initialized` can be used to value-initialize objects in generic components in C++03. In C++11 it is practically not necessary. C++11 gives you a way to force **value-initialization** of your objects in any possible initialization context.

> NOTE: 这就是C++11的uniform initialization，参见`C++11-List-initialization`

## Value initialization

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

> NOTE: 
>
> 1、对于non-class type（比如int），它们的default initialization(或者说 default constructor)是do-nothing，显然这是和C兼容的

As we can see, for **built-in types**, **default-initialization** leaves the objects in **indeterminate state**. **Value-initialization** enables us to tell that for **built-in types**, objects should be initialized with value `0`. 

> NOTE: 上面已经说明了default-initialization和value-initialization的一个重要差异: 对于built-in type:
>
> | Initialization         |                                                              |
> | :--------------------- | ------------------------------------------------------------ |
> | Default-initialization | **default-initialization** leaves the objects in **indeterminate state** |
> | Value-initialization   | objects should be initialized with value `0`                 |
>
> 

### Initialization list of the constructor

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

### Automatic objects

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

## Value-initialization syntax

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

