# RVO and NRVO

## 内容简介

在如下文章中介绍了RVO和NRVO

1、cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)

2、stackoverflow [What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization)

3、wikipedia [Copy elision](https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization) # Return value optimization

### compiler执行optimization的步骤/优先级

总的来说，compiler执行optimization的步骤/优先级大致如下: 

1、copy elision

compiler会优先执行copy elision，如果copy elision不能执行，则会执行下一步；

2、*two-stage overload resolution*(两阶段overload resolution)

compiler会优先选择move construction，最后才选择copy construction；

compiler是会严格遵循optimization principle的，它会主动将这部分optimization完成，无需由programmer来承担，这在一定程度上simplify C++了。

> NOTE: 关于此，在后面的move-and-return中会结合move来进行讨论。



## cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision) # [Notes](https://en.cppreference.com/w/cpp/language/copy_elision#Notes) (since C++11)

In a return statement or a throw-expression, if the compiler cannot perform copy elision but the conditions for copy elision are met or would be met, except that the source is a function parameter, the compiler will attempt to use the **move constructor** even if the object is designated by an lvalue; see [return statement](https://en.cppreference.com/w/cpp/language/return#Notes) for details.

> NOTE: 这其实描述的是RVO

## cppreference [`return` statement](https://en.cppreference.com/w/cpp/language/return) # RVO

> NOET: cppreference [`return` statement](https://en.cppreference.com/w/cpp/language/return) 中并没有RVO章节，它是我添加上去的。

**Returning by value** may involve construction and copy/move of a **temporary object**, unless [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision) is used. 


> NOTE: 上面这段话的意思是: 如果没有使用  [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision) ，则"**Returning by value** may involve construction and copy/move of a **temporary object**, "

Specifically, the conditions for copy/move are as follows:

### Automatic move from local variables and parameters(since C++11)

If *`expression`* is a (possibly parenthesized) [id-expression](https://en.cppreference.com/w/cpp/language/identifiers) that names a variable whose type is either

> NOTE: "id-expression"意味着: 它是lvalue。

1、a non-volatile object type or

2、a non-volatile rvalue reference to object type(since C++20)

> NOTE: 2、暂时不懂

and that variable is declared

1、in the body or

2、as a parameter of the innermost enclosing function or lambda expression,

> NOTE: 2、暂时不懂

then [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution) to select the constructor to use for initialization of the returned value or, for `co_return`, to select the overload of `promise.return_value()` (since C++20) is performed *twice*:

> NOTE: *two-stage overload resolution*(两阶段overload resolution)

#### first

1、first as if *expression* were an **rvalue expression** (thus it may select the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor)), and

a、if the first overload resolution failed or

b、it succeeded, but did not select the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) (formally, the first parameter of the selected constructor was not an rvalue reference to the (possibly cv-qualified) type of *expression*)(until C++20)

> NOTE: b、没有搞懂

#### second

2、then overload resolution is performed as usual, with *expression* considered as an lvalue (so it may select the [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor)).

### Guaranteed copy elision(since C++17)

If *expression* is a prvalue, the result object is initialized directly by that expression. This does not involve a copy or move constructor when the types match (see [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)).







## Example

### RVO and NRVO

```C++
#include <iostream>
#include <utility>

class MoveOnly
{
public:
	MoveOnly()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~MoveOnly()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	MoveOnly(const MoveOnly&) = delete;
	MoveOnly(MoveOnly&&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
MoveOnly foo()
{
	return MoveOnly();
}

MoveOnly bar()
{
	MoveOnly m;
	return m;
}


MoveOnly foobar()
{
	MoveOnly m;
	return std::move(m);
}
int main()
{
	MoveOnly m1 = foo();
	MoveOnly m2 = bar();
	MoveOnly m3 = foobar();
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

运行结果如下:

```C++
MoveOnly::MoveOnly()
MoveOnly::MoveOnly()
MoveOnly::~MoveOnly()
MoveOnly::~MoveOnly()
```

可以看到，compiler执行了RVO、NRVO，即使在使用默认的optimization的情况下。



### moving a local object in a return statement prevents copy elision [`-Wpessimizing-move`]

```C++
#include <iostream>
#include <utility>

class MoveOnly
{
public:
	MoveOnly()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~MoveOnly()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	MoveOnly(const MoveOnly&) = delete;
	MoveOnly(MoveOnly&&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

MoveOnly foobar()
{
	MoveOnly m;
	return std::move(m);
}
int main()
{

	MoveOnly m3 = foobar();
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

编译、运行结果如下:

```C++
main.cpp: In function 'MoveOnly foobar()':
main.cpp:25:18: warning: moving a local object in a return statement prevents copy elision [-Wpessimizing-move]
   25 |  return std::move(m);
      |         ~~~~~~~~~^~~
main.cpp:25:18: note: remove 'std::move' call
MoveOnly::MoveOnly()
MoveOnly::MoveOnly(MoveOnly&&)
MoveOnly::~MoveOnly()
MoveOnly::~MoveOnly()
```







## wikipedia [Copy elision](https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization) # Return value optimization

In the context of the [C++](https://en.wikipedia.org/wiki/C%2B%2B) [programming language](https://en.wikipedia.org/wiki/Programming_language), **return value optimization** (**RVO**) is a [compiler optimization](https://en.wikipedia.org/wiki/Compiler_optimization) that involves eliminating the [temporary object](https://en.wikipedia.org/wiki/Temporary_variable) created to hold a [function](https://en.wikipedia.org/wiki/Subroutine)'s return value.[[6\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-moreeffcpp-6) **RVO** is particularly notable for being allowed to change the observable behaviour of the resulting [program](https://en.wikipedia.org/wiki/Computer_program) by the [C++ standard](https://en.wikipedia.org/wiki/ISO/IEC_14882).[[7\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-andrei-7)

### Summary

In general, the C++ standard allows a [compiler](https://en.wikipedia.org/wiki/Compiler) to perform any optimization, provided the resulting [executable](https://en.wikipedia.org/wiki/Executable) exhibits the same observable behaviour *as if* (i.e. pretending) all the requirements of the standard have been fulfilled. This is commonly referred to as the "[as-if rule](https://en.wikipedia.org/wiki/As-if_rule)".[[8\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-C++03_1.9/1-8) The term *return value optimization* refers to a special clause in the [C++ standard](https://en.wikipedia.org/wiki/ISO/IEC_14882) that goes even further than the "as-if" rule: an implementation may omit a **copy operation** resulting from a [return statement](https://en.wikipedia.org/wiki/Return_statement), even if the [copy constructor](https://en.wikipedia.org/wiki/Copy_constructor) has [side effects](https://en.wikipedia.org/wiki/Side_effect_(computer_science)).[[1\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-C++03_12.8/15-1)

The following example demonstrates a scenario where the implementation may eliminate one or both of the copies being made, even if the **copy constructor** has a visible side effect (printing text).[[1\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-C++03_12.8/15-1) The first copy that may be eliminated is the one where a nameless temporary `C` could be copied into the function `f`'s [return value](https://en.wikipedia.org/wiki/Return_value). The second copy that may be eliminated is the copy of the temporary object returned by `f` to `obj`(下面这个程序涉及到的两处copy constructor).

```cpp
#include <iostream>

struct C {
  C() {}
  C(const C&) { std::cout << "A copy was made.\n"; }
};

C f() {
  return C();
}//此函数设计copy constructor，因为该函数的是by-value返回的，所以函数内构造的temporary object需要被copy出来

int main() {
  std::cout << "Hello World!\n";
  C obj = f();
  return 0;
}
```

Depending upon the [compiler](https://en.wikipedia.org/wiki/Compiler), and that compiler's settings, the resulting [program](https://en.wikipedia.org/wiki/Computer_program) may display any of the following outputs:

```cpp
Hello World!
A copy was made.
A copy was made.
```

```cpp
Hello World!
A copy was made.
```

```cpp
Hello World!
```

### Background

Returning an object of [built-in type](https://en.wikipedia.org/wiki/Built-in_type) from a [function](https://en.wikipedia.org/wiki/Subroutine) usually carries little to no overhead(开销), since the object typically fits in a [CPU register](https://en.wikipedia.org/wiki/Processor_register). Returning a larger object of [class type](https://en.wikipedia.org/wiki/Class_(computing)) may require more expensive copying from one memory location to another. To avoid this, an implementation may create a **hidden object** in the caller's [stack frame](https://en.wikipedia.org/wiki/Stack_frame), and pass the address of this object to the function. The function's return value is then copied into the **hidden object**.[[9\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-efficient-9) Thus, code such as this:

```cpp
struct Data { 
  char bytes[16]; 
};

Data f() {
  Data result = {};
  // generate result
  return result;
}

int main() {
  Data d = f();
}
```

May generate code equivalent to this:

```cpp
struct Data { 
  char bytes[16]; 
};

Data * f(Data * _hiddenAddress) {
  Data result = {};
  // copy result into hidden object
  *_hiddenAddress = result;
  return _hiddenAddress;
}

int main() {
  Data _hidden; // create hidden object
  Data d = *f(&_hidden); // copy the result into d
}
```

which causes the `Data` object to be copied twice.

In the early stages of the evolution of [C++](https://en.wikipedia.org/wiki/C%2B%2B), the language's inability to efficiently return an object of [class type](https://en.wikipedia.org/wiki/Class_(computing)) from a function was considered a weakness.[[10\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-lippman-10) Around 1991, [Walter Bright](https://en.wikipedia.org/wiki/Walter_Bright) implemented a technique to minimize copying, effectively replacing the **hidden object** and the **named object** inside the function with the object used for holding the result(使用object used for holding the result来代替hidden object和named object):[[11\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-d20-11)

```cpp
struct Data { 
  char bytes[16]; 
};

void f(Data *p) {
  // generate result directly in *p
}

int main() {
  Data d;
  f(&d);
}
```

Bright implemented this optimization in his [Zortech C++](https://en.wikipedia.org/wiki/Digital_Mars) compiler.[[10\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-lippman-10) This particular technique was later coined "**Named return value optimization**", referring to the fact that the copying of a **named object** is elided.[[11\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-d20-11)

### Compiler support

**Return value optimization** is supported on most compilers.[[6\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-moreeffcpp-6)[[12\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-vc8-12)[[13\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-gcc-13) There may be, however, circumstances where the compiler is unable to perform the optimization. One common case is when a function may return different named objects depending on the path of execution:[[9\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-efficient-9)[[12\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-vc8-12)[[14\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-n1377-14)

```cpp
#include <string>
std::string f(bool cond = false) {
  std::string first("first");
  std::string second("second");
  // the function may return one of two named objects
  // depending on its argument. RVO might not be applied
  return cond ? first : second;
}

int main() {
  std::string result = f();
}
```


