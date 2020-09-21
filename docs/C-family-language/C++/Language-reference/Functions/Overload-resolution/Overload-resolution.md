# Overload resolution



## cppreference [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)

In order to compile a function call, the compiler must first perform [name lookup](https://en.cppreference.com/w/cpp/language/lookup), which, for functions, may involve [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl), and for function templates may be followed by [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction). If these steps produce more than one *candidate function*, then *overload resolution* is performed to select the function that will actually be called.

> NOTE: overload resolution非常复杂，基本上涵盖了C++的大部分内容；原文中，对于template function的部分涉及得不够多，下面的“Overload resolution and template function“章节进行了补充。

In general, the candidate function whose parameters match the arguments most closely is the one that is called.

> NOTE: 按照`C++\Guide\Implementation-of-polymorphism\index.md`中的说法，其实就是 most matched。

If a function cannot be selected by overload resolution (e.g. it is a [templated entity](https://en.cppreference.com/w/cpp/language/templates#Templated_entity) with a failed [constraint](https://en.cppreference.com/w/cpp/language/constraints)), it cannot be named or otherwise used.

> NOTE: 这段话其实所说明的就是：
>
> - SFINA
> - constrain

### Details

> NOTE: 
>
> *implicit object parameter*，参见 cppreference [this pointer](https://en.cppreference.com/w/cpp/language/this)

#### Member function

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

#### Function template

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



### Candidate functions

The set of candidate functions and the list of arguments is prepared in a unique way for each of the **contexts** where overload resolution is used:

#### Call to a named function

#### Call to a class object

#### Call to an overloaded operator

#### Initialization by constructor

#### Copy-initialization by conversion

#### Non-class initialization by conversion

#### Reference initialization by conversion

#### List-initialization

### Viable functions

### Best viable function

### Ranking of implicit conversion sequences

### Implicit conversion sequence in list-initialization



## Overload resolution and template function

需要梳理清楚overload resolution、template function之间的关系；substitution和overload resolution的发生顺序。

通过name lookup（对于function，还会涉及ADL），如果发现的function是一个function template，则需要对primary function template进行specializaiton: 首先进行[template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction)，然后进行substitution；在进行substitution的时候，compiler还需要实现SFINAE，如果fail，则将这个specialization 丢弃。然后进行specializaiton resolution，还是进行overload resolution？按照之前的描述来看，是完成了overload resolution后，才进行specializaiton resolution。



### draft: cppreference [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)

This rule applies during overload resolution of function templates: When substituting the explicitly specified or deduced type for the template parameter fails, the specialization is discarded from the **overload set** instead of causing a compile error.



### draft: [Notes on C++ SFINAE#Overload Resolution](https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#overload-resolution)

When the compiler tries to compile a function call (simplified):

Perform a name lookup
For function templates the template argument values are deduced from the types of the actual arguments passed into the function.
All occurrences of the template parameter (in the return type and parameters types) are substituted with those deduced types.
When this process leads to invalid type (like `int::internalType`) the particular function is removed from the overload resolution set. (SFINAE)
At the end, we have a list of viable functions that can be used for the specific call. If this set is empty, then the compilation fails. If more than one function is chosen, we have an ambiguity. In general, the candidate function, whose parameters match the arguments most closely is the one that is called.



### draft: [SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/) 

Function template的resolution算法，在文章 [SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/) 中就总结地较好：

> while looking for the best overload, all candidates have to be considered. 

正如这篇文章中所讲述的，为了寻找到best overload，compiler采用了SFINAE策略，即它会尝试所有的candidate，关于SFINAE，参见`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if`章节。





## 控制overload resolution

[More C++ Idioms/Tag Dispatching](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Tag_Dispatching)中的preference ordering就是一个典型的例子。



## Name mangling

Name mangling发生于哪个阶段？



### wikipedia [Name mangling](https://en.wikipedia.org/wiki/Name_mangling)





## TO DO



https://akrzemi1.wordpress.com/2015/11/19/overload-resolution/


https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#overload-resolution

https://akrzemi1.wordpress.com/2015/11/19/overload-resolution/

http://foonathan.github.io/blog/2015/11/30/overload-resolution-4.html

https://www.fluentcpp.com/2017/08/15/function-templates-partial-specialization-cpp/


