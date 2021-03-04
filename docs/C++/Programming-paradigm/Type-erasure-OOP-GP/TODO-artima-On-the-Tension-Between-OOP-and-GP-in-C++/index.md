# artima [On the Tension Between Object-Oriented and Generic Programming in C++](https://www.artima.com/cppsource/type_erasure.html)

and What Type Erasure Can Do About It
by Thomas Becker
October 15, 2007

> NOTE: 
>
> 1、其实 type erasure 所创造的是一个更大的abstraction
>
> 2、这篇文章提出了: C++ iterator type erasure: `any_iterator` 
>
> 3、本文提出了消除interface 和 implementation 之间的dependency、compiler firewall如下方法:
>
> a、pimpl idiom
>
> b、abstract base class
>
> c、type erasure
>
> 



---

## Summary

The author discusses how the use of generic programming in C++ can lead to conflicts with object-oriented design principles. He demonstrates how a technique known as **type erasure** can often be used to resolve these conflicts. An in-depth example is presented: `any_iterator`, a type-safe, heterogeneous C++ iterator.

---

In his glossary of terms[[1](https://www.artima.com/cppsource/type_erasure2.html#notes)], Bjarne Stroustrup has described the C++ programming language that he created as "a general-purpose programming language [...] that supports procedural programming, data abstraction, object-oriented programming, and generic programming." The fact that C++ supports these different programming paradigms makes it unique—and uniquely powerful—among today's programming languages. On the other hand, it should not come as a surprise that the close coexistence(共存) of such vastly different paradigms can cause considerable friction(摩擦力), especially in large software systems.

> NOTE: 
>
> 1、这就是所谓的multi-paradigm，参见 `Programming-paradigm\Common` 章节
>
> 2、上面提到了"friction"，虽然存在，但是知道思想是"博采众长"，参见:
>
> a、参见 `Programming-paradigm\Common` 章节
>
> b、CppCoreGuidelines-[T.5: Combine generic and OO techniques to amplify their strengths, not their costs](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t5-combine-generic-and-oo-techniques-to-amplify-their-strengths-not-their-costs)

In this article, I will focus on the tension that can occur when object-oriented programming (classes, objects, and runtime polymorphism come to mind) meets generic programming (algorithms, templates, and compile time polymorphism come to mind).

> NOTE: 
> 1、static polymorphism and dynamic polymorphism

The article consists of two parts. In the first part, I will demonstrate how the coexistence of OO and generic programming can cause serious friction in real-life software engineering. I will then explain how a technique known as **type erasure** can be used to alleviate these problems.

The second part explains how type erasure can be implemented in C++. Specifically, I will elaborate on an example used in the first part, namely, **C++ iterator type erasure**. I will discuss the design and implementation of a class template[[2](https://www.artima.com/cppsource/type_erasure2.html#notes)] `any_iterator` that provides type erasure for C++ iterators.

## The Trouble with Object-Oriented and Generic Programming

### A Little Trivia Quiz

> NOTE: 
>
> 1、"Quiz"的含义是: 考查；恶作剧；课堂测验
>
> 2、本节标题的含义是: 一个小测验

Let us start with a little trivia quiz. Who said the following things about object-oriented programming?

"I find OOP technically unsound."

"I find OOP philosophically unsound."

"I find OOP methodologically wrong."

"I have yet to see an interesting piece of code that comes from these OO people."

"I think that object orientedness is almost as much of a hoax(恶作剧、戏弄) as artificial intelligence."

All the quotes above are from an interview with Alexander Stepanov[[3](https://www.artima.com/cppsource/type_erasure2.html#notes)], the inventor of the STL and elder statesman(活动家) of generic programming. As a practicing software engineer who works on large commercial software projects, I know better than to hold such a negative view of OO programming. But when someone like Alexander Stepanov says such a thing, then I don't think it should be taken lightly.

> NOTE: 
>
> 1、作者这段话的意思是: Alexander Stepanov是一个牛人、顶级专家，因此他的话，有分量，不能忽视，因此不能忽视它对OOP的负面评价

My experience as a software engineer in the trenches(战壕) has taught me that there is much more tension, if not contradiction(矛盾) or incompatibility(不兼容), between OO programming and generic programming than many people care to admit. It is easy to dismiss Alexander Stepanov's rejection of OO programming as extreme and unrealistic. It is much harder to make the OO and generic programming paradigms coexist and cooperate in real-life software engineering.

> NOTE: 翻译如下:
>
> "我作为软件工程师的经验告诉我，在面向对象编程和泛型编程之间，如果不是矛盾或不兼容的话，存在着比许多人愿意承认的更多的紧张关系。
> 人们很容易将Alexander Stepanov对面向对象编程的拒绝斥为极端和不现实。
> 在现实的软件工程中，要使面向对象和通用编程范式共存并协作是非常困难的。"

In the next three sections, I will illustrate the problem with an example from the real world, and I will suggest a less radical(激进的) remedy(补救措施) than to disavow(完全否定) OO programming as a tool in software design altogether.

> NOTE: 翻译如下:
>
> "在接下来的三节中，我将用一个来自现实世界的例子来说明这个问题，并且我将建议一个不那么激进的补救办法，而不是完全否认OO编程是软件设计中的一种工具。"
>
> 简单的意思是: OOP虽然不完美，但是不至于将它完全否定，对于它的问题，C++提供了补救措施

### An Example from the Real World

> NOTE: 
>
> 1、这一段中给出的例子其实是有些牵强的，因为在实际中，应该很少有人会给用户提供generic programming的interface，因为这样的interface是有compile-time dependency，这就导致了implementation的更改需要client进行recompile

Suppose you have a class `number_cruncher` that calculates and stores a sequence of numbers. Further, suppose that `number_cruncher` wants to expose to its clients a way to iterate over the stored numbers and retrieve them.

In the old days, predating the STL, the interface of `number_cruncher` might have exposed a callback for retrieving the numbers, or perhaps some form of `get_first()`/`get_next()` idiom. In an evironment where the STL and generic programming are used, such an interface would raise more than a few eyebrows. Here, the interface of `number_cruncher` will expose a pair of iterators, like this:

```C++
class number_cruncher
{   
public:
  typedef std::vector<double>::const_iterator const_iterator;
  const_iterator begin() const;
  const_iterator end() const;
  [...]
};
```

> NOTE: 
>
> 1、iterator是generic programming的技术，因此上述code是使用了generic programming

Now suppose you change the type of the collection which internally holds the items from `std::vector` to some other kind of vector. (Yes, there are other kinds of vectors.) In the sense of OO programming, what have you just done? You have changed an implementation detail. And what happens? The world recompiles. By using a typedef for the iterator that you expose, you were able to ensure that your clients did not have to change their code. Nonetheless(尽管如此), their code depends on the type of the iterator, and thus on the type of the collection that you use to store your numbers. In the pre-STL design mentioned earlier, this dependency can easily be eliminated, e.g., by using the `pimpl` idiom, or by exposing the number cruncher's interface via an **abstract base class**. This option is now gone. By using the typedef for the iterators, you have introduced a **compile-time dependency** that cannot be eliminated. Depending on the context in which your class is used, that may or may not be a serious issue.

> NOTE: 
>
> 下面是对上面这段话的一些注解: 
>
> 1、iterator是generic programming的技术，因此上述code是使用了generic programming
>
> 2、上述iterator引入了"compile-time dependency"，这就导致一旦修改了implementation，就需要recompile
>
> 3、"Nonetheless(尽管如此), their code depends on the type of the iterator, and thus on the type of the collection that you use to store your numbers."
>
> client code 依赖于"type of iterator"，因此就间接依赖于 "the type of  the collection"
>
> 4、那如何避免上述的dependency呢？作者给出了两种方法:
>
> a、在OOP中，使用 `pimpl` idiom 是能够避免recompile的
>
> b、在OOP中，"exposing the number cruncher's interface via an **abstract base class**"

But it gets worse. Suppose that the implementation of `number_cruncher` changes in such a way that there are frequent insertions to the internal collection of numbers. Having anticipated(预期) a change of that kind, you have carefully specified `number_cruncher`'s interface to state that it will expose a pair of **input iterators**, dereferencing to something that converts to a double, with incrementing and dereferencing both having O(1) complexity. Therefore, you are within your rights(有权) when you change the internal type of your collection from `std::vector<double>` to `std::list<double>`. Again, your clients will have to recompile. But it is also possible that before, when you were exposing vector iterators, some of your clients may have said, gee, I wonder how many elements there are in this number cruncher. Let me do:

> NOTE: 最后一段话的意思是: 在修改为  `std::list<double>` 之前，你的client写了如下code:，显然如下code是需要iterator type是random access iterator的

```C++
size_t numItems = 
  my_number_cruncher.end() - my_number_cruncher.begin();
```

You did not mean for them to do that, but you could not prevent it either. Now, after you changed an implementation detail of `number_cruncher`, not only will the world recompile, the world will find that it has a broken build. By exposing functionality that you neither needed nor wanted to expose, you have broken encapsulation.

> NOTE: 前面两段话探讨了这样的一种问题:
>
> 将 `class number_cruncher` 的 "type of the collection which internally holds the items" 由 `std::vector` 修改为 `std::list`，因此它的iterator也有random access iterator修改为了input iterator。这个修改带来了两个后果:
>
> 1、client需要recompile
>
> 2、client中原来一些使用random access iterator支持但是input iterator不支持的code将无法编译通过

Well, that's not really an issue, you might say, because if your clients are STL savvy(悟性、理解、懂行), as they should be, then they were probably using `std::distance` to get the number of elements in the `number_cruncher`:

> NOTE: 
> 1、"Well, that's not really an issue, you might say, because if your clients are STL savvy" 的意思是: 因为如果你的客户精通STL
>
> 2、下面所演示的是使用`std::distance`能够实现兼容前面提及的修改

```c++
size_t numItems = std::distance(
  my_number_cruncher.begin(),
  my_number_cruncher.end()
);
```

If they were doing that, then your implementation change from vector to list causes the semantics of your number cruncher to change: the cost of getting the number of elements changes from constant to linear. This is probably even worse than the broken build, because it may not be noticed until way, way later, when something goes horribly(可怕的) awry(脱离既定路线的、错误) in the field. You never meant for anybody to get the number of elements in constant time, but you could not prevent it.

> NOTE: 翻译如下:
>
> "如果它们是这样做的，那么从vector到list的实现变化将导致数字运算机的语义发生变化:获取元素数量的成本从常量变为线性。
> 这可能比坏的构建更糟糕，因为它可能直到很久很久以后，在这个领域出现了可怕的错误时才会被注意到。
> 你从未打算让任何人在常数时间内获得元素的数量，但你无法阻止它。"
>
> 1、这一段作者的意思是: 使用`std::distance`虽然能够保证code编译通过，但是complexity却改变了，并且这种改变容易让人忽视的
>
> 2、上述code说明了使用`std::distance`的优势，概括起来说其实就是"program to abstraction and polymorphism"

Perhaps you think that none of these problems so far are very serious or very likely to happen in practice. Well, ok, I will admit that I made all this up. I have never changed an `std::vector` to a different kind of vector, and changing an `std::vector` to an `std::list` is extremely rare and has never caused me the kind of grief that I described above. But here's something quite similar that has actually happened to me.

Being a scientific programmer, I find myself writing classes that calculate sequences of numbers, store them internally, and expose them to clients. Hence the `number_cruncher` example above. In this case, my number cruncher class had to calculate several sequences of numbers, and there were certain fairly simple relationships between these sequences. For example, sequence D was a constant multiple of sequence A, sequence E was the pointwise difference between sequences B and C, sequence F was the quotient of sequences D and E, and so on and so forth. It would have been a silly time-space trade-off for me to store each one of these sequences in a collection. Instead, I stored only sequences A, B, and C. Sequences D, E, and F were exposed via suitable combinations of `boost::transform_iterator` and `boost::zip_iterator`. These iterators calculate the respective value on the fly during dereferencing. The interface of the `number_cruncher` class ended up looking like this:

> NOTE: 
>
> 1、"`boost::transform_iterator` and `boost::zip_iterator`. These iterators calculate the respective value on the fly during dereferencing. " 这说明这两种iterator是lazy的

```c++
class number_cruncher
{
public:
  typedef std::vector<double>::const_iterator a_iterator;
  typedef a_iterator b_iterator;
  typedef a_iterator c_iterator;
  typedef boost::transform_iterator<
    boost::function<double (double)>,
    std::vector<double>::const_iterator
  > d_iterator;
  [...]

  boost::range<a_iterator> get_a_sequence();
  boost::range<b_iterator> get_b_sequence();
  boost::range<c_iterator> get_c_sequence();
  boost::range<d_iterator> get_d_sequence();
  [...]
};
```

This design obviously suffers from compile-time dependencies just as the simpler examples that we considered earlier. Whenever I see fit to rearrange my sequences internally, e.g., by storing sequences C and E and have my iterators calculate B as the sum of C and E, then my clients have to recompile. But that wasn't really an issue. Still, I was faced with a full-scale revolt(违反) on the part of my clients, who accused me of delivering an abomination of an interface.

> NOTE: 翻译如下:
>
> "这种设计显然受到编译时依赖关系的影响，就像我们前面考虑的更简单的示例一样。
> 每当我认为适合重新安排我的序列内部，例如，通过存储序列C和E，并让我的迭代器计算B作为C和E的总和，然后我的客户端必须重新编译。
> 但这并不是真正的问题。
> 尽管如此，我还是遭到了客户的全面反对，他们指责我发布了一个令人厌恶的界面。"

I should first point out that, as it always happens in real-life software, the number of sequences involved started out rather small but soon reached several dozen. Also, each of these sequences had a domain-related meaning. Therefore, there already was an enum to identify each sequence. Now let us analyze what kind of an interface my clients can rightfully(正当的) expect of me from an object-oriented point of view. Please bear in mind that I am not talking about performance issues yet. I just want to analyze the situation from an OO perspective.

> NOTE: 翻译如下:
>
> "我首先要指出的是，正如在现实生活中的软件中经常发生的那样，涉及的序列数量一开始相当少，但很快就达到了几十个。
> 此外，每个序列都有与领域相关的含义。
> 因此，已经有一个枚举来标识每个序列。
> 现在让我们分析什么样的接口我的客户可以合法(正当的)希望我从面向对象的角度。
> 请记住，我现在还不是在讨论性能问题。
> 我只是想从面向对象的角度来分析这个情况。"

Remember, in the world of OO programming, an interface exposes *what an object does for its clients*, not what the object *is*, and not *how* it does what it does. What is my number cruncher supposed to do for its clients? They have an enum value, and they need the corresponding sequence of numbers in the form of a pair of input iterators that dereference to a double. There can be little doubt as to what kind of interface we should see on an object that provides this service:

> NOTE: 翻译如下:
>
> "请记住，在面向对象编程的世界中，接口公开了对象为它的客户端做了什么，而不是对象是什么，也不是它如何做它做的事情。
> 我的数字处理机应该为它的客户做什么?
> 它们有一个枚举值，并且需要对应的数字序列，它们是一对输入迭代器，对double对象进行解引用。
> 我们应该在提供此服务的对象上看到什么样的接口是毫无疑问的:"

```C++
class number_cruncher
{
public:
  boost::range<number_iterator> 
  get_sequence(sequence_type type);
};
```

Instead, my interface is bloated and cluttered with information that has a lot to do with *how* my number cruncher does what it does, and nothing with *what it does for its clients*. Focusing on the various iterator types in my interface, we see that they expose to the client what they *are*, when all that matters is *what they do for the client*. The standard solution in OO programming is to employ runtime polymorphism to treat these different types uniformly at runtime.

> NOTE: 翻译如下:
>
> "相反，我的界面是膨胀和杂乱的信息，这与我的数字处理机做什么有很大关系，而与它为客户做什么没有任何关系。
> 关注我的接口中的各种迭代器类型，我们看到它们向客户机公开它们的“身份”，而真正重要的是“它们为客户机做了什么”。
> 面向对象编程的标准解决方案是在运行时使用运行时多态性来统一地对待这些不同的类型。"

We see that from on OO perspective, my clients complaints are entirely justified: failure to employ runtime polymorphism has led to a poorly designed interface and strong compile-time dependencies.

There are, of course, many more examples for this kind of tension between **OO programming** and **generic programming**. The common deeper reason behind all those examples is the different role that type plays in OO programming and in generic programming. In OO programming, the judicious choice of user-defined types is a very important, if not the most important, design tool. Take one look at any high-level book on OO analysis and design, and you'll find that it's all about the proper choice of classes and the inheritance relationships between them. In generic programming, on the other hand, the type system has a tendency to get away from the software designer. Types often end up being unrelated despite the fact that the things they represent are very much related. In other words, the multitude of types is merely a side effect of some generic programming technique such as policy driven design or some such thing. Were it not for these techniques, one would have never designed the user-defined types in that way.

> NOTE: 翻译如下:
>
> "当然，对于**OO编程**和**泛型编程**之间的这种紧张关系，还有更多的例子。
> 所有这些例子背后的共同深层原因是类型在面向对象编程和泛型编程中扮演的不同角色。
> 在面向对象编程中，用户定义类型的明智选择即使不是最重要的设计工具，也是非常重要的。
> 看看任何关于OO分析和设计的高级书籍，你会发现它都是关于正确选择类以及它们之间的继承关系的。
> 另一方面，在泛型编程中，类型系统倾向于远离软件设计者。
> 类型通常最终是不相关的，尽管它们所代表的事物是非常相关的。
> 换句话说，大量的类型只是一些泛型编程技术的副作用，比如策略驱动设计或类似的东西。
> 如果没有这些技术，就不会以这种方式设计用户定义的类型。"

### Now What?

So now we know that generic programming idioms and OO design principles have a tendency to conflict. However, I don't believe that throwing out one or the other is a very practical solution. In order to successfully design and implement large software systems, we must avail ourselves of OO principles as well as generic programming techniques. What we—and by we I mean all of us, C++ gurus, authors, speakers, consultants as well as software engineers in the trenches—must understand is this:

> Good engineering involves compromise at every turn. A good, working, finished product is never pure by the standards of any one idiom or methodology. The art of good engineering is not the art of discovering and applying the one right idiom over all others. The art of good engineering is to know what your options are, and then to choose your trade-offs wisely rather than letting others choose them for you.

> NOTE: 翻译如下:
>
> "好的工程设计需要在每一个环节都做出妥协。
> 一个好的、可工作的、最终的产品，从任何一个习语或方法的标准来看，都不是纯粹的。
> 优秀工程的艺术并不是发现和应用一种正确的成语胜过其他所有成语的艺术。
> 好的工程的艺术是知道你有哪些选择，然后明智地做出权衡，而不是让别人替你选择。"

Stepping off my soapbox, what does this embrace(拥抱) of compromise imply for the examples of the previous section? Well, compromise is never easy. It's a pretty messy thing most of the time. That's why people love that pure, squeaky-clean, holier-than-thou absolute truth that is being preached at every street corner (and at many software development conferences as well, for that matter).

> NOTE: 翻译如下:
>
> "走出我的临时讲台，这种对妥协的拥抱对于上一节的例子意味着什么?
> 妥协从来都不是件容易的事。
> 大多数时候这是一件很麻烦的事情。
> 这就是为什么人们喜欢在每个街角(以及许多软件开发会议上)宣扬的纯粹、干净、比你更神圣的真理的原因。"

It is true that whenever you expose a pair of STL iterators such as `std::vector<double>::const_iterator` through a class interface, you are introducing a compile-time dependency. Moreover, as we saw in the previous section, you are getting precariously close to violating basic principles of OO programming such as encapsulation and the proper design of interfaces. But you also have an advantage on your side: when the compiler is through with its inlining, your vector iterators are quite likely to perform as efficiently as plain pointers. This performance gain may well justify the sacrifice of OO purity. The important thing to understand is that there is a trade-off to be made here. It is not uncommon in scientific programming that the performance gain afforded by generic programming techniques is indispensable. But it is equally common in large software systems that there are other performance bottlenecks that render this performance gain insignificant.

It seems to me that as far as the trade-off between OO purity and efficiency is concerned, the pendulum in the C++ community has swung to the efficiency extreme. In the 1990's, it was all about object-oriented analysis and design. Any mention of performance issues in connection with runtime polymorphism was dismissed as "the old C way of thinking." Today, it's the other way round: any proposal to replace or enhance a generic programming solution with something that involves an extra virtual function call will inevitably be met with the objection that it is "too slow." This kind of knee-jerk reaction is not useful in real-life software engineering. I always thought that the old adage concerning optimization and its relation to evil was enough guidance in this respect. But perhaps a more poignant one is needed:

> Optimization whose effect no user ever notices is the root of many evils, among them the failure of software projects and the subsequent failure of businesses.

So assume that you are in a similar situation as I was with my bad number cruncher interface. Your clients demand a more OO-compliant design, and they are perfectly willing to pay a performance penalty on the order of magnitude of a virtual function call for each iterator operation. This is the time to consider *type erasure* as a solution. So what is type erasure, and how can it help us out here?

### Type Erasure as the Glue between OO and Generic Programming

In their book on C++ template metaprogramming[[4](https://www.artima.com/cppsource/type_erasure2.html#notes)], Dave Abrahams and Aleksey Gurtovoy define **type erasure** as "the process of turning a wide variety of types with a common interface into one type with *that same interface*."

> NOTE: 
>
> 1、上述definition是非常好的

The most widely known and used examples of type erasure are `boost::any`[[5](https://www.artima.com/cppsource/type_erasure2.html#notes)] and `boost::function`[[6](https://www.artima.com/cppsource/type_erasure2.html#notes)]. I'll discuss `boost::any` in detail in the second part of this article. `boost::function` is a class template that takes one template argument, **a function type**. Choosing a function type amounts to(等价于) choosing a return type and a list of argument types for a function. Suppose we instantiate `boost::function` as follows:

```
boost::function<int (int)> foo;
```

The variable `foo` can now hold anything that's callable with an `int` as its only argument, and whose return type is convertible to `int`. This could be a **function pointer**, a **user-defined functor**, the result of a `boost::bind`, or what have you. Clearly, this matches the above definition of type erasure.

> NOTE: 此处关于 `boost::funciton` 是 type erasure的介绍是非常好的

The relevance(关联) of this in the context of object-oriented programming is that an interface can now say to the client programmer: "I need you to give me something that's callable as specified by this here function type. What it really is, I don't care. You can give me different things at run time. Also, you can change your client code so it gives me something other than it did before, and you won't have to recompile me." Or, referring to a return value rather than an argument, an interface could say: "I'll give you something that's callable as specified by this here function type. What it really is, you won't know. It could change at run time. I might also change it at compile time, but don't worry, you won't have to recompile because of that."

> NOTE: 
>
> 1、通过上面这段话，可以看出type erasure的优势: 
>
> a、消除了compiler-dependency，这样就避免了recompile
>
> b、generic: type erasure是非常generic的
>
> 显然作者的思路就是: 使用 **type-erasing iterator class** 来替代前面的iterator，从而解决前面描述的各种问题

It should be clear now that our problem with iterator types could be solved by a **type-erasing iterator class**, that is, an iterator that can hold concrete iterators of all manner of type, as long as they have a suitable **commonality**. An interface such as the one of our `number_cruncher` could then say to its clients: "I will give you a pair of input iterators which dereference to something that converts to `double`. If you insist on knowing, I can tell you that what you're getting is the result of a **type erasure**. But that's irrelevant to you, because you will never know what the real type was before the erasure. In fact, before the erasure, the iterator category may have been better than 'input.' But you won't know that, and you will only ever see the input iterator interface. Any change that I make to the actual, un-erased type of the iterator, be it at runtime or at compile time, will go completely unnoticed by you."

> NOTE: 
> 1、上面这段话中的commonality让我想到了 "commonality-and-variability analysis"
>
> 2、上面这段话让我想到: 使用type erasure也能够实现Compiler firewall 

In the second part of this article, I will discuss how type erasure can be implemented in C++. Specifically, I will present my `any_iterator`[[2](https://www.artima.com/cppsource/type_erasure2.html#notes)], a class template that provides type erasure for C++ iterators.

## Implementing Type Erasure in C++, with an Emphasis on Iterator Type Erasure



### Implementing Type Erasure in C++: A Closer Look at `boost::any`



### Beyond `boost::any`



### Implementing Iterator Type Erasure in C++: The `any_iterator`



### Determining the Granularity of the Iterator Type Erasure



### Conversion between Different `any_iterator` Types



### Mundane Uses of the `any_iterator`



### Other Attempts at C++ Iterator Type Erasure





### Outlook: C++ Concepts and the `any_iterator`



## Acknowledgements



## Share your opinion