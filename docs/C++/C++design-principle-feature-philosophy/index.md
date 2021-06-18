# Design、feature、principle of C++

1、本文所讨论的都是一些理念、原则层面的，它能够帮助从大方向上理解、运用C++



## C++语言的设计者的设计原则

### C++是一门安全、自由、灵活的语言

它给予programmer极大的自由，此处所谓的**自由**是指programmer有着很大的**选择**空间，或者说它并没有给予programmer过多的限制。

[正如在如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎]( https://www.zhihu.com/question/22111546/answer/31929118) 中作者所援引的：

> 正如Bjarne Stroustrup所说，
>
> > "C++的许多设计决策根源于我对强迫人按某种特定方式行事的极度厌恶。"
>
> > "当我试图去取缔一个我不喜欢的语言特征时，我总抑制住这样的欲望，因为我认为我无权把个人观点强加给别人。"
>
> 选择是自主权的表现，向往自由的人一定不会认为有选择是件坏事。

在http://thbecker.net/articles/rvalue_references/section_04.html中有这样的总结：

> As we all know, the First Amendment to the C++ Standard states: "The committee shall make no rule that prevents C++ programmers from shooting themselves in the foot." Speaking less facetiously, when it comes to choosing between giving programmers more control and saving them from their own carelessness, C++ tends to err on the side of giving more control. Being true to that spirit, C++11 allows you to use move semantics not just on rvalues, but, at your discretion, on lvalues as well. A good example is the `std` library function `swap`. As before, let `X` be a class for which we have overloaded the copy constructor and copy assignment operator to achieve move semantics on rvalues.

验证上面这段话所表达的思想的另外的例子是

1、[`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) 

2、stackoverflow [Conversion function for error checking considered good?](https://stackoverflow.com/questions/6242296/conversion-function-for-error-checking-considered-good) # [A](https://stackoverflow.com/a/6242355)

```c++
int x = my_object; // does not compile because there's no explicit conversion
bool y = bool(my_object); // an explicit conversion does the trick
```



C++的安全性体现在compiler进行了严格的类型检查，对于不符合规范的program，compiler会给出报错提示。同时C++基于了programmer执行危险操作的权利，不过C++采用的“explicit”的策略，即需要由programmer“explicit”地来执行，而不是由compiler“implicit”地执行。

C++语言就是在这样的发展中，逐渐变大庞杂。

### C++是兼容并包的

c++提供了极大的选择空间，这就意味着它需要提供各种各样的program technique。下面是典型的例子：

1、C++语言的概念比其他的programming language要多得多：比如在python、java，它们仅仅有reference semantic，而c++囊括了：value semantic、reference semantic。



### Prefer introducing new features via the standard library

c++语言是在不断地演进的，那c++标准委员会的专家们是以什么样的原则来不断地更新这么语言呢？这个问题在isocpp What specific design goals guided the committee? [¶](https://isocpp.org/wiki/faq/cpp11#cpp11-specific-goals) [Δ](https://isocpp.org/wiki/faq/cpp11#)中给出了答案，在维基百科[C++11](https://en.wikipedia.org/wiki/C%2B%2B11)的[Design goals](https://en.wikipedia.org/wiki/C%2B%2B11#Design_goals)章节也引用了这个答案，我觉得其中非常重要的一点是：

> Prefer introducing new features via the standard library, rather than extending the core language

上述standard library，更加具体来说就是[Language support library](https://en.cppreference.com/w/cpp/utility#Language_support)。

例子包括:

1 `std::swap`

2 `std::declval`

3 `std::invoke`

> NOTE: tag: prefer standard library over extending the core language



### C++只需要最小限度的runtime

> NOTE: 
>
> "tag-C++principle-minimize runtime"

参见: 

1、[C/C++有没有类似能实现反射、内省和代理之类的技术？ - 左轻侯的回答 - 知乎](C/C++有没有类似能实现反射、内省和代理之类的技术？ - 左轻侯的回答 - 知乎 )

>  C++是一门面向系统编程的语言，也许这样说不合适，那么，它是一门支持系统编程，实际上也被广泛地应用于系统编程的语言。C++程序员往往要尽可能地直接和操作系统甚至硬件打交道。这个定位决定了**C++只需要最小限度的runtime**，同样地，**只需要最小限度的RTTI支持**。一个庞大的runtime会在性能和可移植性方面付出很大的成本，这是和C++的宗旨相违背的。另一方面，C++的风格是尽可能通过库来实现功能，而在语法和编译器标准上非常谨慎。C++不喜欢编译器魔法。



## Feature of C++



在c和c++中，需要由programmer来管理[object](https://en.cppreference.com/w/cpp/language/object ) 的lifetime、storage duration。object的lifetime有：

- global

- stack

- heap

c++的smart pointer library就是用来帮助programmer管理位于heap的object的。

在带有GC的programming language中，programmer是无需关注这些内容的。

c++囊括了：

### Value semantic and reference semantic

在`Language-reference\Basic-concept\Reference-and-Value-Semantics`中对此进行了说明。

### Compile-time and run-time

在`C-and-C++\Compile-time-and-run-time`中对此进行描述

### Static and dynamic 

#### static type and dynamic type

对于polymorphism object，它有static type和dynamic type，关于此，在如下章节中对此进行了说明：

- `C++\Language-reference\Basic-concept\Data-model\Object\Object.md#`[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)
- `C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system.md#Static type and dynamic type`

对于polymorphic object，如何获得它的static type？如何获得它的static type？这些C++都是支持的。

### Programming paradigm

C++是一门multiple-paradigm的programming language，它支持: 

1、OOP

2、generic programming

3、template meta programming

4、......

#### Explicit and implicit

`explicit`供programmer对conversion进行控制；



### Automatic

为了提供C++的易用性，C++ compiler automatic地执行了很多操作，同时它也提供了供programmer将automatic操作取消掉的权利。如下是一些例子：

- C++允许programmer控制是否进行template argument deduction；





## CppCoreGuidelines [Philosophy](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#p-philosophy)



> NOTE: 
>
> P.1 和 P.3 的含义比较接近，可以对两者进行总结，下面是我的一些想法: 
>
> 一、programming language也是一种[语言](https://dengking.github.io/Post/Language/Language/)，我们使用它来描述万事万物；我们需要使用programming language提供的feature来完整地的“描述”我们的意图，而不是将意图隐式地在comments 、document、甚至口头 说明。
>
> 这一点与[zen of python](https://www.python.org/dev/peps/pep-0020/)的
>
> ```
> Explicit is better than implicit.
> ```
>
> 二、How to do？
>
> 原文既给出了例子，也给处理how to do guideline，下面是我结合我的经验对how to do guideline的一些总结
>
> 典型例子: `std::chrono::seconds`
>
> 参见 `Strong-type` 章节
>
> 2、使用good abstraction
>
> 典型例子: `std::span` 等等
>
> 3、prefer library over hand-write code
>
> 尽可能地使用library；
>
> "Prefer algorithm over hand-write loop principle"，参见 `STL\Idiom\Guidelines`；
>
> 4、Use specifier to make it explicit principle
>
> C++ 提供了丰富的specifier，programmer应该使用它来准确地表达"intent"
>
> 5、Use language construct properly
>
> 合理地运用language construct，能够使得program更加准确、少些code

### P.1: Express ideas directly in code

**Reason** Compilers don’t read comments (or design documents) and neither do many programmers (consistently). What is expressed in code has defined semantics and can (in principle) be checked by compilers and other tools.



#### Example: strong type +Use specifier

```C++
class Date {
public:
    Month month() const;  // do
    int month();          // don't
    // ...
};
```

The first declaration of `month` is explicit about returning a `Month` and about not modifying the state of the `Date` object. The second version leaves the reader guessing and opens more possibilities for uncaught bugs.

> NOTE: 
>
> 上述example说明了: 
>
> 1、strong type
>
> 2、use specifier
>
> C++提供了丰富的specifier，programmer应该使用这些specifier来使program更加准确

#### Example: Prefer algorithm over hand-write loop

##### Example, bad

 This loop is a restricted form of `std::find`:

```c++
void f(vector<string>& v)
{
    string val;
    cin >> val;
    // ...
    int index = -1;                    // bad, plus should use gsl::index
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] == val) {
            index = i;
            break;
        }
    }
    // ...
}
```

##### Example, good

 A much clearer expression of intent would be:

```C++
void f(vector<string>& v)
{
    string val;
    cin >> val;
    // ...
    auto p = find(begin(v), end(v), val);  // better
    // ...
}
```

A well-designed library expresses intent (what is to be done, rather than just how something is being done) far better than direct use of language features.

> NOTE: 
>
> 1、上述对比的例子体现了: Prefer algorithm over hand-write loop
>
> 2、推广: prefer library over hand-write code

#### Example: strong type 

```c++
change_speed(double s);  // bad: what does s signify?
// ...
change_speed(2.3);
```

```c++
change_speed(Speed s);  // better: the meaning of s is specified
// ...
change_speed(2.3);    // error: no unit
change_speed(23m / 10s); // meters per second
```



### P.3: Express intent

**Reason** Unless the intent of some code is stated (e.g., in names or comments), it is impossible to tell whether the code does what it is supposed to do.

> NOTE: 
>
> 1、上述“stated”的含义是“说明、描述、表述”
>
> 2、在阅读维基百科[smart pointer](https://en.wikipedia.org/wiki/Smart_pointer)时，其中提及了smart pointer属于[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)，显然smart pointer是典型的express intent。

#### Example: Use language construct properly + prefer library

> NOTE: 
>
> 1、虽然 while 也能够实现 iteration，但是当仅仅是表达for each的时候，后面的range-for、`std::for_each`显然更加合适，显然programmer需要"Use language construct properly"

```C++
gsl::index i = 0;
while (i < v.size()) {
    // ... do something with v[i] ...
}
```

The intent of “just” looping over the elements of `v` is not expressed here. The implementation detail of an index is exposed (so that it might be misused), and `i` outlives the scope of the loop, which might or might not be intended. The reader cannot know from just this section of code.

Better:

```
for (const auto& x : v) { /* do something with the value of x */ }
```

Now, there is no explicit mention of the iteration mechanism, and the loop operates on a reference to `const` elements so that accidental modification cannot happen. If modification is desired, say so:

```
for (auto& x : v) { /* modify x */ }
```

For more details about for-statements, see [ES.71](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-for-range). Sometimes better still, use a named algorithm. This example uses the `for_each` from the Ranges TS because it directly expresses the intent:

```
for_each(v, [](int x) { /* do something with the value of x */ });
for_each(par, v, [](int x) { /* do something with the value of x */ });
```

The last variant makes it clear that we are not interested in the order in which the elements of `v` are handled.

A programmer should be familiar with

- [The guidelines support library](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-gsl)
- [The ISO C++ Standard Library](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-stdlib)
- Whatever foundation libraries are used for the current project(s)



#### Example: strong type + create good abstraction

 If two `int`s are meant to be the coordinates of a 2D point, say so:

```C++
draw_line(int, int, int, int);  // obscure
draw_line(Point, Point);        // clearer
```



### P.2: Write in ISO Standard C++

> NOTE: 
>
> 1、避免undefined behavior、implementation-defined behavior

### P.4: Ideally, a program should be statically type safe

**Reason** Ideally, a program would be completely statically (compile-time) type safe. Unfortunately, that is
not possible. Problem areas:

1、unions

2、casts

3、array decay

> NOTE: 
>
> 1、array decay就导致失去了长度信息，关于此的非常典型的例子是: "stackoverflow [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c) # 5. Common pitfalls when using arrays # 5.3 Pitfall: Using the C idiom to get number of elements" 中给出的:
>
> Main pitfall: the C idiom is not typesafe. For example, the code …
>
> ```c++
> #include <stdio.h>
> 
> #define N_ITEMS( array ) (sizeof( array )/sizeof( *array ))
> 
> void display(int const a[7])
> {
> 	int const n = N_ITEMS(a);          // Oops.
> 	printf("%d elements.\n", n);
> }
> 
> int main()
> {
> 	int const moohaha[] = { 1, 2, 3, 4, 5, 6, 7 };
> 
> 	printf("%d elements, calling display...\n", N_ITEMS(moohaha));
> 	display(moohaha);
> }
> // gcc test.c
> ```
>
> > NOTE: 上述程序，传递array的方式为：Pass by pointer
>
> > NOTE: 上述程序输出如下：
> >
> > ```
> > 7 elements, calling display...
> > 2 elements.
> > 
> > ```
>
> 1、The compiler rewrites `int const a[7]` to just `int const a[]`.
>
> 2、The compiler rewrites `int const a[]` to `int const* a`.
>
> 3、`N_ITEMS` is therefore invoked with a pointer.
>
> 4、For a 32-bit executable `sizeof(array)` (size of a pointer) is then 4.
>
> 5、`sizeof(*array)` is equivalent to `sizeof(int)`, which for a 32-bit executable is also 4.
>
> 

4、range errors

5、narrowing conversions

#### Note

These areas are sources of serious problems (e.g., crashes and security violations). We try to provide alternative techniques.

#### Enforcement

We can ban, restrain, or detect the individual problem categories separately, as required and feasible for individual programs. Always suggest an alternative. For example:

1、unions – use `variant` (in C++17)

2、casts – minimize their use; templates can help

> NOTE: 
>
> C++的meta-programming 技术，赋予了programmer非常强大的、灵活的对类型进行描述、检查的能力，一旦类型不符合预期，则立即给出编译报错。

3、array decay – use `span` (from the GSL)

4、range errors – use `span`

5、narrowing conversions – minimize their use and use `narrow` or `narrow_cast` (from the GSL) where they are necessary

> NOTE:
>
> 1、make it explicit principle

> NOTE: 
>
> 一、上述这些问题，C++基于C努力地进行着改进，这在 `Comparison-of-programming-language\C++VS-C\Type-system` 章节 中进行了总结。
>
> 二、关于type safety，参见 `Theory\Type-system\Type-safety` 章节



### [P.5: Prefer compile-time checking to run-time checking](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p5-prefer-compile-time-checking-to-run-time-checking)

#### Reason

Code clarity and performance. You don't need to write error handlers for errors caught at compile time.

> NOTE: 
>
> 1、这是compile time的优势，它总结得非常好
>
> 2、另外在 paper [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html) 的"1.2. The most important benefit"章节中，讨论了这个topic相关的内容。





### [P.6: What cannot be checked at compile time should be checkable at run time](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p6-what-cannot-be-checked-at-compile-time-should-be-checkable-at-run-time)

> NOTE: 
>
> 1、关于此的典型案例: "stackoverflow [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c) # 5. Common pitfalls when using arrays # 5.3 Pitfall: Using the C idiom to get number of elements" 
>
> In order to detect this error at **run time** you can do …
>
> ```c++
> #include "assert.h"
> #include "stdio.h"
> #include <iostream>
> #include <typeinfo>
> 
> #define N_ITEMS( array )       (                               \
>     assert((                                                    \
>         "N_ITEMS requires an actual array as argument",        \
>         typeid( array ) != typeid( &*array )                    \
>         )),                                                     \
>     sizeof( array )/sizeof( *array )                            \
>     )
> 
> void display(int const a[7])
> {
> 	int const n = N_ITEMS(a);          // Oops.
> 	std::cout << typeid( a ).name() << std::endl;
> 	std::cout << typeid( &*a ).name() << std::endl;
> 	printf("%d elements.\n", n);
> }
> 
> int main()
> {
> 	int const moohaha[] = { 1, 2, 3, 4, 5, 6, 7 };
> 	printf("%d elements, calling display...\n", N_ITEMS(moohaha));
> 	display(moohaha);
> }
> 
> ```
>
> The **runtime error detection** is better than **no detection**, but it wastes a little processor time, and perhaps much more programmer time. Better with **detection at compile time**! 

### P.8: Don’t leak any resources

> NOTE: 
>
> 参见: 
>
> 1、CppCoreGuidelines [The resource management section](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource)
>
> 2、`Resource-management` 章节

#### Note

 Enforcing [the lifetime safety profile](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#SS-lifetime) eliminates leaks. When combined with resource safety provided by [RAII](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-raii), it eliminates the need for “garbage collection” (by generating no garbage). Combine this with enforcement of [the type and bounds profiles](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#SS-force) and you get complete type- and resource-safety, guaranteed by tools.

> NOTE: 
>
> 1、我是因为看到了"  [the lifetime safety profile](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#SS-lifetime) " 这段话而将上面这段话摘抄下来的，关于object lifetime，一种典型的错误就是access outside of object lifetime，这在"Access-outside-of-object-lifetime"章节进行了总结。

### P.9: Don’t waste time or space

> NOTE: 
>
> 1、optimization principle
>
> 2、Zero-overhead principle-You don't pay for what you don't use

### P.11: Encapsulate messy constructs, rather than spreading through the code

> NOTE: 
>
> 1、其实就是合理的encapsulation