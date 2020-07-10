# Philosophy



## CppCoreGuidelines [Philosophy](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#p-philosophy)

### P.1: Express ideas directly in code

What is expressed in code has defined semantics and can (in principle) be checked by compilers and other tools.

> NOTE: programming language也是一种[语言](https://dengking.github.io/Post/Language/Language/)，我们使用它来描述万事万物；
>
> 我们需要使用programming language提供的feature来使我们的“描述”简洁易懂（可读性）、准确、含义丰富；
>
> prefer explicit over implicit



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





## C++语言的设计者的设计原则

### C++是一门自由的语言

它给予programmer极大的自由，此处所谓的自由是指programmer有着很大的**选择**空间，或者说它并没有给予programmer过多的限制。

[正如在如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎]( https://www.zhihu.com/question/22111546/answer/31929118) 中作者所援引的：

正如Bjarne Stroustrup所说，

> "C++的许多设计决策根源于我对强迫人按某种特定方式行事的极度厌恶。"

> "当我试图去取缔一个我不喜欢的语言特征时，我总抑制住这样的欲望，因为我认为我无权把个人观点强加给别人。"

选择是自主权的表现，向往自由的人一定不会认为有选择是件坏事。

在http://thbecker.net/articles/rvalue_references/section_04.html中有这样的总结：

> As we all know, the First Amendment to the C++ Standard states: "The committee shall make no rule that prevents C++ programmers from shooting themselves in the foot." Speaking less facetiously, when it comes to choosing between giving programmers more control and saving them from their own carelessness, C++ tends to err on the side of giving more control. Being true to that spirit, C++11 allows you to use move semantics not just on rvalues, but, at your discretion, on lvalues as well. A good example is the `std` library function `swap`. As before, let `X` be a class for which we have overloaded the copy constructor and copy assignment operator to achieve move semantics on rvalues.

c++语言就是在这样的发展中，逐渐变大庞杂。

### c++是兼容并包的

c++提供了极大的选择空间，这就意味着它需要提供各种各样的program technique。一个典型的例子是：c++语言的概念比其他的programming language要多得多：比如在python、java，它们仅仅有reference semantic，而c++囊括了：value semantic、reference semantic。

在c和c++中，需要由programmer来管理object https://en.cppreference.com/w/cpp/language/object 的lifetime、Storage duration。object的lifetime有：

- global

- stack

- heap

c++的smart pointer library就是用来帮助programmer管理位于heap的object的。

在带有GC的programming language中，programmer是无需关注这些内容的。