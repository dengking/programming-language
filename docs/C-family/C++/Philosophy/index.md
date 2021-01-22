# Philosophy



## CppCoreGuidelines [Philosophy](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#p-philosophy)

### P.1: Express ideas directly in code

**Reason** Compilers don’t read comments (or design documents) and neither do many programmers (consistently). What is expressed in code has defined semantics and can (in principle) be checked by compilers and other tools.

> NOTE: programming language也是一种[语言](https://dengking.github.io/Post/Language/Language/)，我们使用它来描述万事万物；我们需要使用programming language提供的feature来完整地的“描述”我们的意图，而不是将意图隐式地在comments 、document、甚至口头 说明。
>
> 这一点与[zen of python](https://www.python.org/dev/peps/pep-0020/)的
>
> ```
> Explicit is better than implicit.
> ```
>
> 一致。
>
> 这一点和P.3: Express intent相关；



#### Example

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



#### 补充

`concept` 在c++20中，programmer可以使用concept feature来formally描述对type的期望，这种方式能够让compiler来检查type，是典型的express ideas directly in code，关于此参见：

- cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)，本地路径: `C++\Named-requirements`

`std::chrono::seconds`

`std::unique_lock`、`std::unique_ptr`、`std::shared_lock`、`std::shared_ptr`



### P.3: Express intent

**Reason** Unless the intent of some code is stated (e.g., in names or comments), it is impossible to tell whether the code does what it is supposed to do.

> NOTE: 上述“stated”的含义是“说明、描述、表述”
>
> 在阅读维基百科[smart pointer](https://en.wikipedia.org/wiki/Smart_pointer)时，其中提及了smart pointer属于[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)，显然smart pointer是典型的express intent。

#### 补充

cast operator: C++提供了丰富的cast operator来让programmer准确的表达其cast intent，关于此，参见:

- `C++\Language-reference\Basic-concept\Type-system\Type-conversion\Cast-operator`



### P.4: Ideally, a program should be statically type safe

**Reason** Ideally, a program would be completely statically (compile-time) type safe. Unfortunately, that is
not possible. Problem areas:

- unions
- casts
- array decay
- range errors
- narrowing conversions

> NOTE: 上述这些问题，C++基于C努力地进行着改进，这部分能否在`C-family-language\C-and-C++\VS-C-VS-c++.md`中有描述。
>
> C++的meta-programming 技术，赋予了programmer非常强大的、灵活的对类型进行描述、检查的能力，一旦类型不符合预期，则立即给出编译报错，终止编译，典型特性包括：
>
> - concept
> - detection idiom
> - trait（一种reflection）
>
> 另外一個例子是：`static_assert`





## C++语言的设计者的设计原则

### C++是一门安全、自由、灵活的语言

它给予programmer极大的自由，此处所谓的**自由**是指programmer有着很大的**选择**空间，或者说它并没有给予programmer过多的限制。

[正如在如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎]( https://www.zhihu.com/question/22111546/answer/31929118) 中作者所援引的：

正如Bjarne Stroustrup所说，

> "C++的许多设计决策根源于我对强迫人按某种特定方式行事的极度厌恶。"

> "当我试图去取缔一个我不喜欢的语言特征时，我总抑制住这样的欲望，因为我认为我无权把个人观点强加给别人。"

选择是自主权的表现，向往自由的人一定不会认为有选择是件坏事。

在http://thbecker.net/articles/rvalue_references/section_04.html中有这样的总结：

> As we all know, the First Amendment to the C++ Standard states: "The committee shall make no rule that prevents C++ programmers from shooting themselves in the foot." Speaking less facetiously, when it comes to choosing between giving programmers more control and saving them from their own carelessness, C++ tends to err on the side of giving more control. Being true to that spirit, C++11 allows you to use move semantics not just on rvalues, but, at your discretion, on lvalues as well. A good example is the `std` library function `swap`. As before, let `X` be a class for which we have overloaded the copy constructor and copy assignment operator to achieve move semantics on rvalues.

验证上面这段话所表达的思想的另外一个例子是：[`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) 

C++的安全性体现在compiler进行了严格的类型检查，对于不符合规范的program，compiler会给出报错提示。同时C++基于了programmer执行危险操作的权利，不过C++采用的“explicit”的策略，即需要由programmer“explicit”地来执行，而不是由compiler“implicit”地执行。

C++语言就是在这样的发展中，逐渐变大庞杂。

### C++是兼容并包的

c++提供了极大的选择空间，这就意味着它需要提供各种各样的program technique。一个典型的例子是：c++语言的概念比其他的programming language要多得多：比如在python、java，它们仅仅有reference semantic，而c++囊括了：value semantic、reference semantic。



### Feature of C++

在c和c++中，需要由programmer来管理[object](https://en.cppreference.com/w/cpp/language/object ) 的lifetime、storage duration。object的lifetime有：

- global

- stack

- heap

c++的smart pointer library就是用来帮助programmer管理位于heap的object的。

在带有GC的programming language中，programmer是无需关注这些内容的。

c++囊括了：

#### Value semantic and reference semantic

在`Language-reference\Basic-concept\Reference-and-Value-Semantics`中对此进行了说明。

#### Compile-time and run-time

在`C-and-C++\Compile-time-and-run-time`中对此进行描述

#### Static and dynamic 

##### static type and dynamic type

对于polymorphism object，它有static type和dynamic type，关于此，在如下章节中对此进行了说明：

- `C++\Language-reference\Basic-concept\Data-model\Object\Object.md#`[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)
- `C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system.md#Static type and dynamic type`

对于polymorphic object，如何获得它的static type？如何获得它的static type？这些C++都是支持的。

#### Programming paradigm

- OOP
- template meta programming
- generic programming

#### Explicit and implicit

`explicit`供programmer对conversion进行控制；



#### Automatic

为了提供C++的易用性，C++ compiler automatic地执行了很多操作，同时它也提供了供programmer将automatic操作取消掉的权利。如下是一些例子：

- C++允许programmer控制是否进行template argument deduction；



### Prefer introducing new features via the standard library

c++语言是在不断地演进的，那c++标准委员会的专家们是以什么样的原则来不断地更新这么语言呢？这个问题在isocpp What specific design goals guided the committee? [¶](https://isocpp.org/wiki/faq/cpp11#cpp11-specific-goals) [Δ](https://isocpp.org/wiki/faq/cpp11#)中给出了答案，在维基百科[C++11](https://en.wikipedia.org/wiki/C%2B%2B11)的[Design goals](https://en.wikipedia.org/wiki/C%2B%2B11#Design_goals)章节也引用了这个答案，我觉得其中非常重要的一点是：

> Prefer introducing new features via the standard library, rather than extending the core language

上述standard library，更加具体来说就是[Language support library](https://en.cppreference.com/w/cpp/utility#Language_support)。

例子包括:

1 `std::swap`

2 `std::declval`

3 `std::invoke`

> NOTE: tag: prefer standard library over extending the core language