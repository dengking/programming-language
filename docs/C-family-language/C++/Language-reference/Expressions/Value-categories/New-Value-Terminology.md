# stroustrup [“New” Value Terminology](http://www.stroustrup.com/terminology.pdf)

## Background 

The terms “lvalue” and “rvalue” are deep in C++’s genes. They were introduced by Christopher Strachey for [CPL](http://en.wikipedia.org/wiki/CPL_(programming_language)) [Strachey,196?], the ancestor to BCPL. [Dennis Ritchie](http://en.wikipedia.org/wiki/Dennis_Ritchie) used “lvalue” to describe C (e.g. see [K&R,1978]), but left out “rvalue”, considering “lvalue” and “not lvalue” sufficient. I did the same for early definitions of `C++` (e.g. see [Stroustrup,1986] and [Ellis,1989]). The terms “lvalue” and “rvalue” are “all over” the draft C++ standard. Clearly, this is not terminology to mess with without serious reason and great care. So – despite appearances and rumors to the contrary – we didn’t. Key terms, such as “lvalue” and “rvalue” retain their conventional meaning in the **FCD**. Furthermore, I claim that the terminology used in the FCD [Becker,2010] and presented in [Miller,2010a] does not represent “random personal preferences” but is derived from an analysis of fundamental properties and selected from a very constrained set of options. Anyone doing a proper analysis should come to a very similar set of concepts and names for those. Precise wording for specification is hard, so to aid precision and to resolve some known specification problems (related to rvalue references) Mike Miller wrote a paper with the unpromising title Rvalue References as ‘Funny’ Lvalues proposing a changed terminology [Miller,2010]. I did not consider “messing with terminology” late in the standards process a good idea and when Daniel Krügler pointed out that the definition of “rvalue” in that proposal differed from the use of “rvalue” in the standard library, I considered the proposal dead.

> NOTE： “FCD”是 Final Committee Draft 的 意思，参见：http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2011/n3249.html

## The model

Now I was seriously worried. Clearly we were headed for an impasse（僵局） or a mess or both. I spent the lunchtime doing an analysis to see which of the properties (of values) were **independent**. There were only two independent properties:
- “has identity” – i.e. and address, a pointer, the user can determine whether two copies are identical, etc.
- “can be moved from” – i.e. we are allowed to leave to source of a “copy” in some **indeterminate**, but **valid** state(即我们被允许在一些不确定但有效的状态下留下”副本“的来源)

> NOTE: 上面这段话将 “has identity”  和 “can be moved from” 描述为 two independent properties，为什么他们是两个是independent的？它们之间是否存在互斥性？要回答这个问题，需要我搞清楚这两个property的含义。其中"has identity"是非常好理解的，但是"can be moved from"的含义我就不清楚了。经过Google，[这篇文章](https://stackoverflow.com/a/3109981)对move semantics的解释非常好。

This led me to the conclusion that there are exactly three kinds of **values** (using the regex notational trick of using a capital letter to indicate a negative – I was in a hurry):

- iM: has identity and cannot be moved from
- im: has identity and can be moved from (e.g. the result of casting an **lvalue** to a **rvalue reference**)
- Im: does not have identity and can be moved from

The fourth possibility (“**IM**”: doesn’t have identity and cannot be moved) is not useful in `C++` (or, I think) in any other language.

In addition to these three fundamental classifications of values, we have two obvious **generalizations** that correspond to the two independent properties:
- i: has identity
- m: can be moved from

This led me to put this diagram on the board (my handwriting):

```cpp
iM      im      Im
 \     /  \     /
  \   /    \   /
   \ /      \ /
    i        m
```

Please note that the labels on the diagram (representing the three fundamental kinds of **values** and the two **generalizations**) are not **names**; they are abbreviations(缩写) of lists of properties claimed to be primitive(原始) (in the mathematical sense).

```cpp
lvalue       xvalue           prvalue
 \           /    \           /
  \         /      \         /
   \       /        \       /
    \     /          \     /
     \   /            \   /
    glvalue           rvalue
```

> NOTE: 这张图是下一段分析后的结果，将它放到这里是为了便于和上一张图进行对比

## Naming

> NOTE: 这段描述如何来为这几种value进行命名

I observed that we had only limited freedom to name: The two points to the left (labeled “iM” and “i”) are what people with more or less formality have called “**lvalues**” and the two points on the right (labeled
“m” and “Im”) are what people with more or less formality have called “**rvalues**.”(注意这里有一个more or less，说明不是百分之百) This must be reflected in our naming. That is, the left “leg” of the `W` should have names related to “**lvalue**” and the right “leg” of the `W` should have names related to “**rvalue**.” I note that this whole discussion/problem arise from the introduction of **rvalue references** and **move semantics**(这个问题是由于**rvalue references** and **move semantics**的引入而引发的). These notions simply don’t exist in Strachey’s world consisting of just **rvalues** and **lvalues**. Someone observed that the ideas that

- Every value is either an lvalue or an rvalue
- An lvalue is not an rvalue and an rvalue is not an lvalue

are deeply embedded in our consciousness, very useful properties, and traces of this **dichotomy**(非此即彼) can be found all over the draft standard(这种二分法的痕迹可以在标准草案中找到). We all agreed that we ought to preserve those properties (and make them precise). This further constrained our naming choices. I observed that the standard library wording(措辞) uses “rvalue” to mean “m” (the generalization), so that to preserve the expectation and text of the standard library the right-hand bottom point of the W should be named “**rvalue**.”

> NOTE: 这里描述了命名为**rvalue**的原因：“m”

This led to a focused discussion of naming. First, we needed to decide on “**lvalue**.” Should “lvalue” mean “iM” or the generalization “i”? Led by Doug Gregor, we listed the places in the core language wording where the word “lvalue” was qualified(有资格) to mean the one or the other. A list was made and in most cases and in the most tricky/brittle text “lvalue” currently means “iM”. This is the classical meaning of lvalue because “in the old days” nothing was moved; “**move**” is a novel notion in C++0x. Also, naming the topleft point of the W “**lvalue**” gives us the property that every value is an lvalue or an rvalue, but not both.

> NOTE : 这段描述了命名为**lvalue**的原因：与convention兼容

So, the top left point of the W is “**lvalue**” and the bottom right point is “**rvalue**.” What does that make the bottom left and top right points? The bottom left point is a generalization of the **classical lvalue**, allowing for move. So it is a “generalized lvalue.” We named it “**glvalue**.”(这段描述了命名为**glvalue**的原因) You can quibble about the abbreviation, but (I think) not with the logic. We assumed that in serious use “generalized lvalue” would somehow be abbreviated anyway, so we had better do it immediately (or risk confusion). The top right point of the W is less general than the bottom right (now, as ever, called “rvalue”). That point represent the original pure notion of an object you can **move** from because it cannot be referred to again (except by a destructor)(这一点代表了您可以移动的对象的原始纯概念，因为它不能再次引用（除了析构函数）). I liked the phrase “**specialized rvalue**” in contrast to “**generalized lvalue**” but “**pure rvalue**” abbreviated to “**prvalue**” won out (and probably rightly so)(与“generalized lvalue”相比，我喜欢短语“specialized rvalue”，但缩写为“prvalue”的“pure rvalue”胜出（并且可能是正确的）). So, the left leg of the W is “**lvalue**” and “**glvalue**” and the right leg is “**prvalue**” and “**rvalue**.” Incidentally, every value is either a **glvalue** or a **prvalue**, but not both.

> NOTE: 这段描述了命名为**glvalue**、**prvalue**的原因。

This leaves the top middle of the W: “im”; that is, values that have identity and can be moved. We really don’t have anything that guides us to a good name for those esoteric（深奥难懂的） beasts. They are important to people working with the (draft) standard text, but are unlikely to become a household name. We didn’t find any real constraints on the naming to guide us, so we picked ‘**x**’ for the center, the unknown, the strange, the xpert only, or even x-rated. Here is Steve showing off the final product:

```c++
lvalue       xvalue           prvalue
 \           /    \           /
  \         /      \         /
   \       /        \       /
    \     /          \     /
     \   /            \   /
    glvalue           rvalue
```

> NOTE: 关于上图的注解，参见下面的explanation章节。

Note the classification of current uses of “lvalue” (with various classifications) in the pre-Pittsburgh WP to the left (mostly in Doug’s handwriting).

In Pittsburgh (and a few places later), I had the opportunity to explain this terminology to several people. It can be done in a few minutes. I think it has prvalue (Groan!). Members of the CWG reports that the new terminology has already helped clarify and resolve issues.

## Summary
The (supposedly new) terminology in the FCD (and presented in [Miller,2010a]) is logical (derived from fundamental language properties), in accordance with the history of C++ and all of its ancestors, in accordance with the pre-Pittsburgh wording, consistent with the terminology used to specify the standard library, and solves a few specification problems that several members of the CWG deemed important.



## Explanation

本节对下面这张图进行详细说明

```
lvalue       xvalue           prvalue
 \           /    \           /
  \         /      \         /
   \       /        \       /
    \     /          \     /
     \   /            \   /
    glvalue           rvalue
```

### 名称解释

| 缩写    | 全拼                                                         | 中文含义 |
| ------- | ------------------------------------------------------------ | -------- |
| glvalue | generalized lvalue                                           | 广义左值 |
| prvalue | pure rvalue                                                  | 纯右值   |
| xvalue  | eXpiring value，在下面文章中提及了这个名称: <br>- cppreference [Value categories](https://en.cppreference.com/w/cpp/language/value_category) <br>- stackoverflow [What is move semantics?](https://stackoverflow.com/questions/3106110/what-is-move-semantics) `#` [A](https://stackoverflow.com/a/11540204) | 将亡值   |

### 如何理解上述图？

在 https://stackoverflow.com/a/11540204 中，使用的是下面这种图

```c++
     expressions
       /     \
      /       \
     /         \
 glvalues   rvalues
   /  \       /  \
  /    \     /    \
 /      \   /      \
lvalues   xvalues   prvalues
```

相比而言，这张图是更加易懂的：expression分为两大类：

- glvalues
- rvalues

上述两大类包含第三层的对应元素：

- glvalues：lvalues、xvalues
- rvalues：prvalues、xvalues

按照cppreference [Value categories](https://en.cppreference.com/w/cpp/language/value_category)中的说法，“the three primary value categories”是:

- lvalues
- xvalues
- prvalues

lvalues、prvalues是历史遗留的，通过上面的描述可以知道，它们是遵循convention的，所以在此不再进行赘述；新增的是：xvalues。

### xvalues

令人费解的是：xvalues它既可以归入glvalues也可以归入rvalues；我们需要思考：C++为什么引入xvalues这个新的概念？答案是：move semantic（关于move semantic，在`C++\Language-reference\Reference\Move-semantic`中进行了介绍），下面对这个答案进行详细说明：

C++11给予programmer可以引用prvalue的权利，这就是rvalue reference，对于prvalue，programmer是可以安全地将其move走的，这是在C++语言级别支持的（compiler能够识别）。同时C++还给予了programmer将一些**可以安全地移走的glvalue**也move走的权利，这些**可以安全地移走的glvalue**就是**xvalue**，显然，这些**可以安全地移走的glvalue**即具备 **im** 属性。

需要注意：与prvalue必然可以安全地move不同的是，对于**可以安全地移走的glvalue** 的判断是完全由programmer 来决定的，也就是xvalue是由programmer来决定的。

为了支持这个，C++语言做了如下变动：

引入了xvalue的概念，xvalue既可以归入glvalue，也可以归入rvalue，通过`std::move`，programmer告诉compiler将其当做rvalue来使用，以充分发挥move semantic。

为了支持上述的转换：将xvalue作为rvalue来使用，C++添加了reference collapsing规则（在`C++\Language-reference\Reference`中对这些规则进行了详细介绍）；

需要注意的是：C++中，只允许programmer**引用**rvalue。

### Literal类prvalue

在`C++\Language-reference\Expressions\Value-categories\Value-categories\Value-categories.md`中总结了literal类prvalue，感觉使用stroustrup [“New” Value Terminology](http://www.stroustrup.com/terminology.pdf)中的分类法无法将它们也归入prvalue，但是使用传统的rvalue不能处于assignment operator的左边则可以。在cnblogs [Lvalues and Rvalues](https://www.cnblogs.com/areliang/archive/2011/11/16/2251480.html)中，对此有着更加好的解释:

> rvalue是不可以赋值的，它不是一个变量，在内存中没有存在，没有地址。它要么是存在于CPU的寄存器中，要么是存在于指令中（立即数）。所以只要对rvalue取地址，那么就一定是错误的（编译器会抱怨的）。
>
> 访问rvalue不会导致CPU访问存储器（对立即数和寄存器的访问很快）。
>
> rvalue的例子：
>
> ```C++
> int a;  
> a = 10; // 10是rvalue，它没有地址，&10就是错误的表达式。从汇编语言的角度来看，10是直接存在于MOV指令中的立即数。   
> 10 = a; // 错误，10是rvalue，不可赋值。   
> //函数返回值属于rvalue，因为返回值通常用CPU寄存器传递，没有地址。   
> int foo()  
> {  
>     return 0;  
> }  
> int b = foo(); //没问题，函数返回值是rvalue。   
> int* p = &foo(); //错误，rvalue没有地址。   
> void bar(int& i)  
> {  
> }  
> bar(foo()); //错误，bar函数参数需要的是lvalue。 
> ```
>
> 