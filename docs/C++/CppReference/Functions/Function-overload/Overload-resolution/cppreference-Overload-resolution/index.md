
# cppreference [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)

In order to compile a function call, the compiler must first perform [name lookup](https://en.cppreference.com/w/cpp/language/lookup), which, for functions, may involve [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl), and for function templates may be followed by [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction). If these steps produce more than one *candidate function*, then *overload resolution* is performed to select the function that will actually be called.

> NOTE: overload resolution非常复杂，基本上涵盖了C++的大部分内容；原文中，对于template function的部分涉及得不够多，下面的“Overload resolution and template function“章节进行了补充。

In general, the candidate function whose parameters match the arguments most closely is the one that is called.

> NOTE: 按照`C++\Guide\Implementation-of-polymorphism\index.md`中的说法，其实就是 most matched。

If a function cannot be selected by overload resolution (e.g. it is a [templated entity](https://en.cppreference.com/w/cpp/language/templates#Templated_entity) with a failed [constraint](https://en.cppreference.com/w/cpp/language/constraints)), it cannot be named or otherwise used.

> NOTE: 这段话其实所说明的就是：
>
> - SFINA
> - constrain

## Details

> NOTE: 
>
> *implicit object parameter*，参见 cppreference [this pointer](https://en.cppreference.com/w/cpp/language/this)

### Member function

> NOTE: 本节的小标题是我添加上去的，原文并没有，我添加的目的是对内容进行梳理总结，原文原文的中的下面这些段落都是对member function的描述，其中重点描述的是implicit object parameter和implied object argument。

If any candidate function is a [member function](https://en.cppreference.com/w/cpp/language/member_functions) (static or non-static), but not a constructor, it is treated as if it has an extra parameter (*implicit object parameter*) which represents the object for which they are called and appears before the first of the actual parameters.

Similarly, the object on which a member function is being called is prepended to the argument list as the *implied object argument*.

> NOTE: 原文的介绍是非常难懂的，因为它并没有像读者说明清楚这样做的意图是什么，所以造出读者：不知其所以然，后者说，它没有站在设计者的角度来向读者阐明这样做的目的。在accu [**Overload Resolution - Selecting the Function**](https://accu.org/journals/overload/13/66/kilpelainen_268/)的中则对此进行了说明：
>
> For overload resolution, member functions are considered as free functions with an extra parameter taking the object itself. This is called the *implicit object parameter* . The cv-qualification [ [1 ](https://accu.org/journals/overload/13/66/kilpelainen_268/#ftn.d0e82)] of the implicit parameter is the same as the cv-qualification of the specified member function. The object is matched to the implicit object parameter to make the overload resolution possible. This is an easy way to make the overloading rules **uniform** for the member functions and free functions. 
>
> 显然，最后一句话是点睛之笔。
>
> 但是毕竟 *implicit object parameter* 与其他的parameter是有所不同的，所以原文的下面对此它的特别之处进行了详细说明：

For member functions of class `X`, the type of the implicit object parameter is affected by cv-qualifications and ref-qualifications of the member function as described in [member functions](https://en.cppreference.com/w/cpp/language/member_functions).

> NOTE: 在cppreference [member functions](https://en.cppreference.com/w/cpp/language/member_functions)的“const- and volatile-qualified member functions”段，对这个问题进行了深入分析。

For the rest of overload resolution, the *implied object argument* is indistinguishable from other arguments, but the following special rules apply to the *implicit object parameter*:

> NOTE: 上面这段话中的“indistinguishable”的意思其实就是“same”，即相同的。下面是 *implied object argument* 的特别之处

1) user-defined conversions cannot be applied to the **implicit object parameter**

```c++
#include <iostream>

struct B
{
	void f(int)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
struct A
{
	operator B&()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

int main()
{
	A a;
//	a.B::f(1); // Error: user-defined conversions cannot be applied
//			   // to the implicit object parameter
	static_cast<B&>(a).f(1); // OK
    ((B&)a).f(1); // OK
}
// g++ test.cpp

```

> NOTE: 上述程序的输出为:
>
> ```C++
> A::operator B&()
> void B::f(int)
> A::operator B&()
> void B::f(int)
> ```

### Function template

If any candidate is a **function template**, its specializations are generated using [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction), and such specializations are treated just like **non-template functions** except where specified otherwise in the **tie-breaker rules**. If a name refers to one or more function templates and also to a set of overloaded non-template functions, those functions and the specializations generated from the templates are all candidates.

> NOTE: 上面这段话中的“tie-breaker rules“的意思是”打破平局规则“，"tiebreakers"的意思是“决胜局”，关于这个词，参见下面内容:
>
> baidu [tie-breaker-rule是什么意思](https://zhidao.baidu.com/question/560880697101891604.html)
>
> > 打破bai平局规则；打破平局的规则；判定准则
>
> wikipedia [Tennis scoring system](https://en.wikipedia.org/wiki/Tennis_scoring_system)
>
> > However, if the players reach 12–12, a 7-point tie-breaker is played to determine the winner. 



## Candidate functions

The set of candidate functions and the list of arguments is prepared in a unique way for each of the **contexts** where overload resolution is used:

### Call to a named function

### Call to a class object

### Call to an overloaded operator

### Initialization by constructor

### Copy-initialization by conversion

### Non-class initialization by conversion

### Reference initialization by conversion

### List-initialization

## Viable functions

## Best viable function

## Ranking of implicit conversion sequences

### Rank rule of standard conversion sequence

Each [type of standard conversion sequence](https://en.cppreference.com/w/cpp/language/implicit_conversion) is assigned one of three ranks:

| rank               | explanation                                                  | 注解 |
| ------------------ | ------------------------------------------------------------ | ---- |
| 1) **Exact match** | - no conversion required, <br>- lvalue-to-rvalue conversion, <br>- qualification conversion, <br>- function pointer conversion, (since C++17) <br>- user-defined conversion of class type to the same class |      |
| 2) **Promotion**   | - integral promotion <br>- floating-point promotion          |      |
| 3) **Conversion**  | - integral conversion, <br>- floating-point conversion, <br/>- floating-integral conversion, <br/>- pointer conversion, <br/>- pointer-to-member conversion, <br/>- boolean conversion, <br/>- user-defined conversion of a derived class to its base |      |

The rank of the standard conversion sequence is the worst of the ranks of the standard conversions it holds (there may be up to [three conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion))

> NOTE: 这段话的意思是: rank of the standard conversion sequence是由the worst of the ranks of the standard conversions it holds而决定的

> NOTE: 前面介绍了standard conversion sequence的rank rule，下面开始介绍implicit conversion sequence的rank rule

### Rank rule of implicit conversion sequences

| rule                                                         | 注解                     |
| ------------------------------------------------------------ | ------------------------ |
| A standard conversion sequence is always *better* than a user-defined conversion sequence or an ellipsis conversion sequence. |                          |
| A user-defined conversion sequence is always *better* than an [ellipsis conversion](https://en.cppreference.com/w/cpp/language/variadic_arguments) sequence |                          |
| A standard conversion sequence `S1` is *better* than a standard conversion sequence `S2` if | 下面回答对此进行详细介绍 |



> NOTE: 当compiler根据rank rule无法进行全排序的时候，则就会出现ambiguous overload的compile error，这是我们平时非常容易遇到的



```C++
int f(const int&) // overload #1
{
	return 0;
}

int f(int&)       // overload #2 (both references)
{
	return 0;
}
int g(const int&) // overload #1
{
	return 0;
}
int g(int)         // overload #2
{
	return 0;
}
int main()
{
	int i;
	int j = f(i); // lvalue i -> int& is better than lvalue int -> const int&
				  // calls f(int&)
	int k = g(i); // lvalue i -> const int& ranks Exact Match
				  // lvalue i -> rvalue int ranks Exact Match
				  // ambiguous overload: compilation error
}
// g++ test.cpp


```

> NOTE: 上述程序是典型的ambiguous overload，它被收录到了[Ambiguous-overload](./Ambiguous-overload.md) 中
>
> ```C++
> test2.cpp: In function ‘int main()’:
> test2.cpp:23:13: error: call of overloaded ‘g(int&)’ is ambiguous
>   int k = g(i); // lvalue i -> const int& ranks Exact Match
>              ^
> test2.cpp:23:13: note: candidates are:
> test2.cpp:10:5: note: int g(const int&)
>  int g(const int&) // overload #1
>      ^
> test2.cpp:14:5: note: int g(int)
>  int g(int)         // overload #2
> 
> ```







```c++
class B;
class A
{
	A(B&)
	{
	}
};
// converting constructor
class B
{
	operator A()
	{
	}
};
// user-defined conversion function
class C
{
	C(B&)
	{
	}
};
// converting constructor
void f(A)
{
} // overload #1
void f(C)
{
} // overload #2

int main()
{
	B b;
	f(b); // B -> A via ctor or B -> A via function (ambiguous conversion)
		  // b -> C via ctor (user-defined conversion)
		  // the conversions for overload #1 and for overload #2
		  // are indistinguishable; compilation fails
}
// g++ test.cpp


```

> NOTE:  上述程序编译报错如下:
>
> ```c++
> test3.cpp: In function ‘int main()’:
> test3.cpp:33:5: error: call of overloaded ‘f(B&)’ is ambiguous
>   f(b); // B -> A via ctor or B -> A via function (ambiguous conversion)
>      ^
> test3.cpp:33:5: note: candidates are:
> test3.cpp:23:6: note: void f(A)
>  void f(A)
>       ^
> test3.cpp:26:6: note: void f(C)
>  void f(C)
> ```
>
> 

## Implicit conversion sequence in list-initialization

