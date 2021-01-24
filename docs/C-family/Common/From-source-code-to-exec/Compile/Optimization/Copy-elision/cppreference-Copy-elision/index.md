# cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)

Omits copy and move (since C++11) constructors, resulting in zero-copy pass-by-value semantics.

> NOTE: 这是C++对value semantic的增强

## Mandatory elision of copy/move operations(C++17)

> NOTE: 强制消除

Under the following circumstances, the compilers are required to omit the copy and move construction of class objects, even if the copy/move constructor and the destructor have observable side-effects. 

> NOTE: 比较激进的optimization

The objects are constructed directly into the storage where they would otherwise be copied/moved to. 

> NOTE: 看了上面这段话，我想到了
>
> 1、construct in place
>
> 2、placement new

The copy/move constructors need not be present or accessible:

> NOTE: 需要注意的是:
>
> 1、上面仅仅说明了copy/move constructors need not be present or accessible，而没有说明destructor，在下面的"1、return"中，对destructor进行了说明。
>
> 2、这样的设计是否会存在: programmer并不想让这个object被copy，如何来保证呢？在 stackoverflow [What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization) # [A](https://stackoverflow.com/a/12953129) 中，提及了于此相关的内容。

### 1、return

> NOTE: 这段所描述的，其实就是RVO，在下面的"Non-mandatory elision of copy/move (since C++11) operations"中还会提及。

In a [return statement](https://en.cppreference.com/w/cpp/language/return), when the operand is a [prvalue](https://en.cppreference.com/w/cpp/language/value_category) of the same class type (ignoring [cv-qualification](https://en.cppreference.com/w/cpp/language/cv)) as the function return type:

```C++
T f() {
    return T();
}
 
f(); // only one call to default constructor of T
```

> NOTE: 其实在C++17之前的compiler已经实现了这种optimization，参见下面的"Non-mandatory elision of copy/move (since C++11) operations"，但是两个版本之间还是存在差异的，下面会进行总结。

The **destructor** of the type returned must be accessible at the point of the return statement and non-deleted, even though no T object is destroyed.

> NOTE: constructor和"copy/move constructors "相反

### 2、initialization

In the initialization of an object, when the initializer expression is a [prvalue](https://en.cppreference.com/w/cpp/language/value_category) of the same class type (ignoring [cv-qualification](https://en.cppreference.com/w/cpp/language/cv)) as the variable type:

```C++
T x = T(T(f())); // only one call to default constructor of T, to initialize 
```



## Non-mandatory elision of copy/move (since C++11) operations

Under the following circumstances, the compilers are permitted, but not required to omit the copy and move (since C++11) construction of class objects even if the copy/move (since C++11) constructor and the destructor have observable side-effects. The objects are constructed directly into the storage where they would otherwise be copied/moved to. 

> NOTE: 对比前面的"Mandatory elision of copy/move operations(C++17)"可知，它们的目标效果是相同的

This is an optimization: even when it takes place and the copy/move (since C++11) constructor is not called, it still must be present and accessible (as if no optimization happened at all), otherwise the program is ill-formed:

> NOTE: 上面这段话的意思是: 在C++17之前的版本中，当compiler执行"elision of copy/move "时，即使"copy/move (since C++11) constructor"没有被调用，"copy/move (since C++11) constructor still must be present and accessible (as if no optimization happened at all), otherwise the program is ill-formed"
>
> 这样设计的目的是: 
>
> 1、在C++ programming language层，并没有规定elision of copy/move ，因此implementation的optimization，必须要保证即使执行了optimization，还是要遵循C++ programming language规范，不能够打破规范；
>
> 2、stackoverflow [What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization) # [A](https://stackoverflow.com/a/12953129) 中提及的:
>
> > If a call to a copy or move constructor is elided, that constructor must still exist and must be accessible. This ensures that copy elision does not allow copying objects which are not normally copyable, e.g. because they have a private or deleted copy/move constructor.

### 1、return

In a [return statement](https://en.cppreference.com/w/cpp/language/return), when the operand is the name of a non-volatile object with automatic storage duration, which isn't a function parameter or a catch clause parameter, and which is of the same class type (ignoring [cv-qualification](https://en.cppreference.com/w/cpp/language/cv)) as the function return type. 

> NOTE: 有name，显然是lvalue

This variant of **copy elision** is known as **NRVO**, "**named return value optimization**".



### 2、initialization 

#### (until C++17)

In the initialization of an object, when the source object is a **nameless temporary** and is of the same class type (ignoring [cv-qualification](https://en.cppreference.com/w/cpp/language/cv)) as the target object. 

> NOTE: "nameless temporary "，显然是rvalue。

#### RVO

When the nameless temporary is the operand of a return statement, this variant of copy elision is known as RVO, "**return value optimization**".

#### (since C++17)

Return value optimization is mandatory and no longer considered as copy elision; see above.

### 3、throw expression

> NOTE: 暂时未读

### 4、catch clause

> NOTE: 暂时未读

---

When **copy elision** occurs, the implementation treats the source and target of the omitted copy/move (since C++11) operation as simply two different ways of referring to the same object, and the destruction of that object occurs at the later of the times when the two objects would have been destroyed without the optimization (except that, if the parameter of the selected constructor is an rvalue reference to object type, the destruction occurs when the target would have been destroyed) (since C++17).

> NOTE: 上面这段话是什么意思？没有读懂

Multiple copy elisions may be chained to eliminate multiple copies.

> NOTE: 原文这里仅仅一段话，在 stackoverflow [What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization) # [A](https://stackoverflow.com/a/12953145) 中，对这段话进行了专门的注解:
>
> ```cpp
> #include <iostream>
> #include <utility>
> class Thing
> {
> public:
> 	Thing()
> 	{
> 		std::cout << __PRETTY_FUNCTION__ << std::endl;
> 	}
> 	~Thing()
> 	{
> 		std::cout << __PRETTY_FUNCTION__ << std::endl;
> 	}
> 	Thing(const Thing&)
> 	{
> 		std::cout << __PRETTY_FUNCTION__ << std::endl;
> 	}
> };
> Thing f()
> {
> 	Thing t;
> 	return t;
> }
> 
> int main()
> {
> 
> 	Thing t2 = f();
> }
> // g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out
> 
> ```
>
> > NOTE: 输出如下:
> >
> > ```
> > Thing::Thing()
> > Thing::~Thing()
> > 
> > ```
> >
> > 
>
> and explained:
>
> > Here the criteria for elision can be combined to eliminate two calls to the copy constructor of class `Thing`: 
> >
> > the copying of the local automatic object `t` into the temporary object for the return value of function `f()` and the copying of that temporary object into object `t2`. 
> >
> > Effectively, the construction of the local object `t` can be viewed as directly initializing the global object `t2`, and that object’s destruction will occur at program exit. Adding a move constructor to `Thing` has the same effect, but it is the move construction from the temporary object to `t2` that is elided.
>
> 即使是move construction，也会被elided。

### 5、RVO and NRVO in [constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) and [constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization) (since C++11)

In [constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) and [constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization), return value optimization (RVO) is guaranteed, however, named return value optimization (NRVO) is forbidden:

> NOTE: 通过下面的例子的编译结果可以看出:
>
> 1、named return value optimization (NRVO) 
>
> ```C++
> constexpr A g()
> {
> 	A a;
> 	return a;
> }
> ```
>
> 在g++中是无法编译通过的
>
> 

```C++
#include <iostream>
#include <utility>
struct A
{
	void *p;
	constexpr A() :
					p(this)
	{
	}
};
/**
 * @brief return named A object `a`
 *
 * @return
 */
constexpr A g()
{
	A a;
	return a;
}

constexpr A a;          // a.p points to a

// constexpr A b = g(); // error: b.p would be dangling and would point to a temporary
// with automatic storage duration

void h()
{
	A c = g();          // c.p may point to c or to an ephemeral temporary
}

extern const A d;
constexpr A f()
{
	A e;
	if (&e == &d)
		return A();
	else
		return e;
	// mandating NRVO in constant evaluation contexts would result in contradiction
	// that NRVO is performed if and only if it's not performed
}
// constexpr A d = f(); // error: d.p would be dangling
int main()
{

	g();
	h();
	f();
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

> NOTE: 上述程序，编译如下:
>
> ```C++
> main.cpp: In function 'constexpr A g()':
> main.cpp:20:1: error: body of 'constexpr' function 'constexpr A g()' not a return-statement
>    20 | }
>       | ^
> main.cpp: In function 'constexpr A f()':
> main.cpp:42:1: error: body of 'constexpr' function 'constexpr A f()' not a return-statement
>    42 | }
>       | ^
> ```
>
> 显然，在`g++`中，是不允许返回一个named object的



## Notes

Copy elision is the only allowed form of optimization (until C++14)one of the two allowed forms of optimization, alongside [allocation elision and extension](https://en.cppreference.com/w/cpp/language/new#Allocation), (since C++14) that can change the observable side-effects. 

> NOTE: 上面这段话，初读是难以理解的，下面是注解版本:
>
> Optimization that can change the observable side-effects: 
>
> 一、until C++14
>
> 1、Copy elision 
>
> 二、since C++14
>
> 1、Copy elision 
>
> 2、[Allocation elision and extension](https://en.cppreference.com/w/cpp/language/new#Allocation)
>
> "Optimization that can change the observable side-effects"意味中打破As-if-rule，因此，这段话总结了compiler打破As-if rule的optimization。在下面文章中，也涉及了这个topic:
>
> 1、stackoverflow [What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization) # [A](https://stackoverflow.com/a/12953129)
>
> 

Because some compilers do not perform copy elision in every situation where it is allowed (e.g., in debug mode), programs that rely on the side-effects of copy/move constructors and destructors are not portable.

> NOTE: 这是cross-plateform的建议

### (since C++11)

In a return statement or a throw-expression, if the compiler cannot perform copy elision but the conditions for copy elision are met or would be met, except that the source is a function parameter, the compiler will attempt to use the move constructor even if the object is designated by an lvalue; see [return statement](https://en.cppreference.com/w/cpp/language/return#Notes) for details.

> NOTE: 这其实描述的是RVO



