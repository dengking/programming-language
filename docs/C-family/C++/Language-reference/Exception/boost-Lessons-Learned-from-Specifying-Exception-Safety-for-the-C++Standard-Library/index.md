# boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html)

## Abstract

This paper represents the knowledge accumulated in response to a real-world need: that the C++ **Standard Template Library**(STL) exhibit useful and well-defined interactions with exceptions, the error-handling mechanism built-in to the core C++ language. It explores the meaning of exception-safety, reveals surprising myths(谬见) about exceptions and genericity(泛型), describes valuable tools for reasoning about program correctness, and outlines an automated testing procedure for verifying exception-safety.

> NOTE: 这段话的大意是: 从STL中学习"interactions with exceptions"、"error-handling mechanism"经验，并将这些经验应用于"real-world need"。

**Keywords:** exception-safety, exceptions, STL, C++

## 1 What is exception-safety?

Informally, **exception-safety** in a component means that it exhibits reasonable(合理的) behavior when an exception is thrown during its execution. 

> NOTE: 即使在exception出现的情况下，program仍然能够保证"reasonable behavior" ，显然这是非常吸引人的一个性质

For most people, the term “reasonable” includes all the usual expectations for error-handling: 

1、that resources should not be leaked, and 

2、that the program should remain in a well-defined state so that execution can continue. 

3、For most components, it also includes the expectation that when an error is encountered, it is reported to the caller.

More formally, we can describe a component as minimally(最低限度的) exception-safe if, when exceptions are thrown from within that component, its invariants are intact(原封不动的；未受损伤的). Later on we'll see that at least three different levels of exception-safety can be usefully distinguished. These distinctions can help us to describe and reason about the behavior of large systems.

### Exception-neutrality

> NOTE: 
>
> 1、"neutrality"的意思是: 中立性
>
> 2、下面这段话中的"generic"，应该指代的是generic programming，在C++中，它所指的是template
>
> 3、这段话让我想起了: exception 和 generic programming

In a generic component, we usually have an additional expectation of *exception-neutrality*, which means that exceptions thrown by a component's type parameters should be propagated, unchanged, to the component's caller.

## 2 Myths and Superstitions

> NOTE: 
>
> 1、本节标题的含义是: 神话 和 迷信
>
> 2、本节作者的书写思路是: 逐一列举"widely-held superstitions"，并逐一进行分析

Exception-safety seems straightforward so far: it doesn't constitute(构成) anything more than we'd expect from code using more traditional error-handling techniques. It might be worthwhile, however, to examine the term from a psychological viewpoint. Nobody ever spoke of “error-safety” before C++ had exceptions.

It's almost as though exceptions are viewed as a *mysterious attack* on otherwise correct code, from which we must protect ourselves. Needless to say, this doesn't lead to a healthy relationship with **error handling**! During standardization, a democratic(民主的) process which requires broad support for changes, I encountered many widely-held superstitions(迷信). In order to even begin the discussion of exception-safety in generic components, it may be worthwhile confronting(面对) a few of them.

> NOTE: 翻译如下:
>
> "似乎异常被视为一种对正确代码的神秘攻击，我们必须保护自己。
> 不用说，这不会导致错误处理的健康关系!
> 在标准化这一需要广泛支持变革的民主过程中，我遇到了许多广为流传的迷信。
> 为了开始讨论泛型组件中的异常安全性，可能有必要面对其中的一些。"
>
> 这段话是承上启下

### Myth 1

*“Interactions between templates and exceptions are not well-understood.”* This myth(神话), often heard from those who consider these both new language features, is easily disposed of: there simply are no interactions. A template, once instantiated, works in all respects like an ordinary class or function. A simple way to reason about the behavior of a template with exceptions is to think of how a specific instantiation of that template works. Finally, the genericity of templates should not cause special concern. Although the component's client supplies part of the operation (which may, unless otherwise specified, throw arbitrary exceptions), the same is true of operations using familiar virtual functions or simple function pointers.

> NOTE: 翻译如下:
>
> "模板和异常之间的交互没有被很好地理解。
> 这个迷思，经常从那些认为这两种语言都是新的特性的人那里听到，很容易就被抛弃了:根本没有交互作用。
> 模板一旦实例化，在各个方面都像普通类或函数一样工作。
> 推断有异常的模板行为的一种简单方法是考虑该模板的特定实例化是如何工作的。
> 最后，模板的通用性不应该引起特别关注。
> 尽管组件的客户端提供了部分操作(除非特别指定，否则可能抛出任意异常)，但使用熟悉的虚函数或简单函数指针的操作也是如此。"
>
> 作者提出的观点是: exception 和 template之间压根就没有 interaction，因为template一旦被"instantiated"，就会像普通class、function一样工作。作者给出了理解 "the behavior of a template with exceptions" 的方法: 根据 "how a specific instantiation of that template works"  来进行理解。

### Myth 2

*“It is well known to be impossible to write an exception-safe generic container.”* This claim is often heard with reference to an article by Tom Cargill [[4\]](https://www.boost.org/community/exception_safety.html#reference4) in which he explores the problem of exception-safety for a generic stack template. In his article, Cargill raises many useful questions, but unfortunately fails to present a solution to his problem.[1](https://www.boost.org/community/exception_safety.html#footnote1) He concludes by suggesting that a solution may not be possible. Unfortunately, his article was read by many as “proof” of that speculation. Since it was published there have been many examples of exception-safe generic components, among them the C++ standard library containers.

> NOTE: 翻译如下:
>
> "众所周知，编写异常安全的泛型容器是不可能的。
> 这种说法经常在Tom Cargill的一篇文章中听到，在这篇文章中，他探讨了通用堆栈模板的异常安全问题。
> 在他的文章中，Cargill 提出了许多有用的问题，但不幸的是，没有提出一个解决他的问题的解决方案。
> 不幸的是，他的文章被许多人视为这种猜测的“证据”。
> 自从它发布以来，已经出现了许多异常安全的泛型组件的例子，其中包括c++标准库容器。"
>
> 作者给出的观点是: 是可以实现 "exception-safe generic components"

### Myth 3

*“Dealing with exceptions will slow code down, and templates are used specifically to get the best possible performance.”* A good implementation of C++ will not devote a single instruction cycle to dealing with exceptions until one is thrown, and then it can be handled at a speed comparable with that of calling a function [[7\]](https://www.boost.org/community/exception_safety.html#reference7). That alone gives programs using exceptions performance equivalent to that of a program which ignores the possibility of errors. Using exceptions can actually result in faster programs than “traditional” error handling methods for other reasons. First, a catch block clearly indicates(表明) to the compiler which code is devoted to error-handling; it can then be separated from the usual execution path, improving **locality of reference**. Second, code using “traditional” error handling must typically test a return value for errors after every single function call; using exceptions completely eliminates that overhead.

> NOTE: 翻译如下:
>
> "“处理异常会降低代码速度，模板被专门用来获得可能的最佳性能。“
> 一个好的c++实现不会用一个指令周期来处理异常，直到一个异常被抛出，然后它可以以与调用函数相当的速度处理。
> 这就使使用异常的程序的性能等同于忽略错误可能性的程序的性能。
> 由于其他原因，使用异常实际上可以比“传统”错误处理方法更快地生成程序。
> 首先，catch块清楚地告诉编译器哪些代码是专门用于错误处理的;
> 然后，它可以从通常的执行路径中分离出来，改善引用的局部性。
> 其次，使用“传统”错误处理的代码通常必须在每次函数调用后测试返回值是否存在错误;
> 使用异常完全消除了这种开销。"
>
> 作者给出的观点是: Dealing with exceptions不会slow code down

### Myth 4

*“Exceptions make it more difficult to reason about a program's behavior.”* Usually cited(引用) in support of this myth is the way “hidden” execution paths are followed during stack-unwinding. Hidden execution paths are nothing new to any C++ programmer who expects local variables to be destroyed upon returning from a function:

> NOTE: 翻译如下:
>
> "异常使得推断程序的行为变得更加困难。
> 通常被引用来支持这种说法的是在堆栈展开过程中遵循的“隐藏”执行路径。
> 对于任何c++程序员来说，隐藏的执行路径并不是什么新鲜事，因为他们希望在从函数返回时销毁局部变量:"
>
> 作者的观点是: exception不会增加"reason about a program's behavior"的难度，下面作者列举了例子来进行说明: 

```c++
ErrorCode f( int& result )         // 1 
{                                  // 2 
    X x;                           // 3 
    ErrorCode err = x.g( result ); // 4 
    if ( err != kNoError )         // 5 
        return err;                // 6 
    // ...More code here... 
    return kNoError;               // 7 
}
```

In the example above, there is a “hidden” call to `X::~X()` in lines 6 and 7. Granted, using exceptions, there is no code devoted to error handling visible:

```c++
int f()                 // 1 
{                       // 2 
    X x;                // 3 
    int result = x.g(); // 4 
    // ...More code here... 
    return result;      // 5 
} 
```

For many programmers more familiar with exceptions, the second example is actually more readable and understandable than the first. The “hidden” code paths include the same calls to destructors of local variables. In addition, they follow a simple pattern which acts *exactly* as though there were a potential return statement after each function call in case of an exception. Readability is enhanced because the normal path of execution is unobscured(不隐藏的) by error-handling, and return values are freed up to be used in a natural way.

There is an even more important way in which exceptions can enhance correctness: by allowing simple **class invariants**. In the first example, if `x`'s constructor should need to allocate resources, it has no way to report a failure: in C++, constructors have no return values. The usual result when exceptions are avoided is that classes requiring resources must include a separate initializer function which finishes the job of construction. The programmer can therefore never be sure, when an object of class `X` is used, whether he is handling a full-fledged `X` or some abortive attempt to construct one (or worse: someone simply forgot to call the initializer!)

> NOTE: 翻译如下:
>
> "还有一种更重要的方法可以增强异常的正确性:通过允许简单的类不变量。
> 在第一个例子中，如果x的构造函数需要分配资源，它没有办法报告失败:在c++中，构造函数没有返回值。
> 避免异常时，通常的结果是，需要资源的类必须包含一个单独的初始化函数来完成构造工作。
> 因此，程序员永远无法确定，当使用类X的对象时，他是在处理一个成熟的X，还是在尝试构造一个失败的X(或者更糟:有人只是忘记调用初始化器!)"
>
> 1、这段话是符合 CppCoreGuidelines [C.42: If a constructor cannot construct a valid object, throw an exception](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c42-if-a-constructor-cannot-construct-a-valid-object-throw-an-exception)
>
> 2、上面这段话中提及了class invariant，在CppCoreGuidelines 中也使用 class invariant:
>
> a、[invariant](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-struct) : C.2: Use `class` if the class has an invariant; use `struct` if the data members can vary independently
>
> b、[establish invariant](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-ctor) : C.40: Define a constructor if a class has an invariant
>
> 显然，这段话是如何 "C.40" 的

## 3 A contractual basis for exception-safety

> NOTE: 
>
> 1、"contractual"的含义是"契约的"；
>
> 2、在进行generic programming的时候，是允许client指定template argument的，这就是下面这段话中的"configurability by client code"的含义，因此"exception-safety in a generic component"通常取决于它的template argument的，在进行generic programming的时候，就需要generic component和client之间约定关于exception safety的contract，这就是下面这段话中的"exception-safety in a generic component usually depends on a contract between the component and its clients"；由于有exception safety的contract，因此需要将exception-safety进行分级。

A non-generic component can be described as exception-safe in isolation, but because of its configurability by client code, exception-safety in a generic component usually depends on a contract between the component and its clients. For example, the designer of a generic component might require that an operation which is used in the component's destructor not throw any exceptions.[2](https://www.boost.org/community/exception_safety.html#footnote2) The generic component might, in return, provide one of the following guarantees:

1、The *basic* guarantee: that the invariants of the component are preserved, and no resources are leaked.

2、The *strong* guarantee: that the operation has either completed successfully or thrown an exception, leaving the program state exactly as it was before the operation started.

3、The *no-throw* guarantee: that the operation will not throw an exception.

### Basic guarantee 

The basic guarantee is a simple minimum standard for exception-safety to which we can hold all components. It says simply that after an exception, the component can still be used as before. Importantly, the preservation of invariants allows the component to be destroyed, potentially as part of stack-unwinding. This guarantee is actually less useful than it might at first appear. If a component has many valid states, after an exception we have no idea what state the component is in; only that the state is valid. The options for recovery in this case are limited: either destruction or resetting the component to some known state before further use. 

> NOTE: 
>
> 1、通过上面这段话可知作者的最终结论: "This guarantee is actually less useful than it might at first appear"，简而言之是: "Basic guarantee"是非常鸡肋的，在实际中，可能没有太多价值。这在最后一段话中进行了详细说明:  "If a component has many valid states, after an exception we have no idea what state the component is in; only that the state is valid. The options for recovery in this case are limited: either destruction or resetting the component to some known state before further use. "
>
> 简而言之: 它是unpredictable，因此是uncomputational，因此非常鸡肋
>
> 2、"It says simply that after an exception, the component can still be used as before. Importantly, the preservation of invariants allows the component to be destroyed, potentially as part of stack-unwinding"
>
> 这段话的意思是: 在出现exception后，component 是仍然能够使用的，这意味中它能够实现"preservation of invariants"，实现"preservation of invariants"能够"allows the component to be destroyed, potentially as part of stack-unwinding"，显然这意味中，它能够避免resource leak。

Consider the following example:

```C++
template <class X> 
void print_random_sequence() 
{ 
    std::vector<X> v(10); // A vector of 10 items 
    try { 
        // Provides only the basic guarantee 
        v.insert( v.begin(), X() ); 
    } 
    catch(...) {} // ignore any exceptions above 
    // print the vector's contents 
    std::cout "(" << v.size() << ") "; 
    std::copy( v.begin(), v.end(), 
    std::ostream_iterator<X>( std::cout, " " ) ); 
} 
```

Since all we know about `v` after an exception is that it is valid, the function is allowed to print any random sequence of `X`s.[3](https://www.boost.org/community/exception_safety.html#footnote3) It is “safe” in the sense that it is not allowed to crash, but its output may be unpredictable.

> 3 In practice of course, this function would make an extremely poor random sequence generator!

### Strong guarantee 

> NOTE: 
>
> 1、 “commit-or-rollback” 让我想到了 "transaction "
>
> 2、strong guarantee能够保证"the program state is simple and predictable in case of an exception"，显然这是computational的，这就决定了Strong guarantee有着非常高的价值，在实际开发中，是programmer的追求
>
> 3、最后一段话的总结是非常好的 

The *strong* guarantee provides full “commit-or-rollback” semantics. In the case of C++ standard containers, this means, for example, that if an exception is thrown all iterators remain valid. We also know that the container has exactly the same elements as before the exception was thrown. A transaction that has no effects if it fails has obvious benefits: the program state is simple and predictable in case of an exception. In the C++ standard library, nearly all of the operations on the node-based containers list, set, multiset, map, and multimap provide the *strong* guarantee.[4](https://www.boost.org/community/exception_safety.html#footnote4)).

### No-throw guarantee

The *no-throw* guarantee is the strongest of all, and it says that an operation is guaranteed not to throw an exception: it always completes successfully. This guarantee is necessary for most destructors, and indeed the destructors of C++ standard library components are all guaranteed not to throw exceptions. The *no-throw* guarantee turns out to be important for other reasons, as we shall see.[5](https://www.boost.org/community/exception_safety.html#footnote5)

> 5 All type parameters supplied by clients of the C++ standard library are required not to throw from their destructors. In return, all components of the C++ standard library provide at least the *basic* guarantee.

> NOTE: 
>
> 1、现实情况中，No-throw guarantee可能无法实现，这在下面的"5 What level of exception-safety should a component specify?"中进行了说明。

## 4 Legal Wrangling

> NOTE: 本节标题的含义是: 法律纠纷

Inevitably(不可避免地), the contract can get more complicated: a quid pro quo arrangement is possible. Some components in the C++ Standard Library give one guarantee for arbitrary type parameters, but give a stronger guarantee in exchange for additional promises from the client type that no exceptions will be thrown. For example, the standard container operation `vector<T>::erase` gives the *basic* guarantee for any `T`, but for types whose copy constructor and copy assignment operator do not throw, it gives the *no-throw* guarantee.[6](https://www.boost.org/community/exception_safety.html#footnote6)

> NOTE: 翻译如下:
>
> "不可避免的是，合同可能会变得更加复杂:双方可能会达成一项交换安排。
> c++标准库中的一些组件为任意类型参数提供了一种保证，但又提供了更强的保证，以交换客户端类型的额外承诺，即不会抛出异常。
> 例如，标准容器操作' vector<T>::erase '为任何' T '提供了*basic*保证，但对于复制构造函数和复制赋值操作符不抛出的类型，它提供了no-throw保证。"
>
> 1、这段话论述了C++ STL的exception safety和template argument的exception safety之间的关联，简单概括起来就是: 如果template argument的exception safety越高，则C++ STL的exception safety也越高
>
> 2、"Some components in the C++ Standard Library give one guarantee for arbitrary type parameters, but give a stronger guarantee in exchange for additional promises from the client type that no exceptions will be thrown."
>
> 这段话是难以理解的，难点在第二句: "that no exceptions will be thrown"引导的是"promises"的同位语从句；前面的翻译基本上是比较准确的。



## 5 What level of exception-safety should a component specify?

> NOTE: 
>
> 1、这段话的`insert`例子，体现了performance和exception safety的tradeoff
>
> 2、copy and swap来实现strong exception safety
>
> 3、在这一段中，作者提出了 "“natural”(本质的) level of safety "，即一些operation，它的本质的level of safety，显然，它是programmer无法逾越的，它就相当于天花板

From a client's point-of-view, the strongest possible level of safety would be ideal. Of course, the *no-throw* guarantee is simply impossible for many operations, but what about the *strong* guarantee? For example, suppose we wanted atomic behavior for `vector<T>::insert`. Insertion into the middle of a vector requires copying elements after the insertion point into later positions, to make room for the new element. If copying an element can fail, rolling back the operation would require “undoing” the previous copies...which depends on copying again. If copying back should fail (as it likely would), we have failed to meet our guarantee.

One possible alternative would be to redefine `insert` to build the new array contents in a fresh piece of memory each time, and only destroy the old contents when that has succeeded. Unfortunately, there is a non-trivial cost if this approach is followed: insertions near the end of a vector which might have previously caused only a few copies would now cause every element to be copied. The *basic* guarantee is a “natural”(本质的) level of safety for this operation, which it can provide without violating its performance guarantees. In fact all of the operations in the library appear to have such a “natural”(本质的) level of safety.

Because performance requirements were already a well-established part of the draft standard and because performance is a primary goal of the STL, there was no attempt to specify more safety than could be provided within those requirements. Although not all of the library gives the *strong* guarantee, almost any operation on a standard container which gives the *basic* guarantee can be made *strong* using the “make a new copy” strategy described above:

```C++
template <class Container, class BasicOp> 
void MakeOperationStrong( Container& c, const BasicOp& op ) 
{ 
    Container tmp(c); // Copy c 
    op(tmp); // Work on the copy 
    c.swap(tmp); // Cannot fail7
}
```

This technique can be folded into a wrapper class to make a similar container which provides stronger guarantees (and different performance characteristics).[8](https://www.boost.org/community/exception_safety.html#footnote8)

> 8 This suggests another potential use for the oft-wished-for but as yet unseen `container_traits<>` template: automated container selection to meet exception-safety constraints.

## 6 Should we take everything we can get?

By considering a particular implementation, we can hope to discern(识别、辨别) a natural(本质的) level of safety. The danger in using this to establish requirements for a component is that the implementation might be restricted. If someone should come up with a more-efficient implementation which we'd like to use, we may find that it's incompatible with our exception-safety requirements. One might expect this to be of no concern in the well-explored domains of data structures and algorithms covered by the STL, but even there, advances are being made. A good example is the recent *introsort* algorithm [[6\]](https://www.boost.org/community/exception_safety.html#reference6), which represents a substantial improvement in worst-case complexity over the well-established *quicksort*.

> NOTE: 翻译如下:
>
> "通过考虑一种特殊的执行方式，我们可以希望辨别出一种本质的安全水平。
> 使用它来建立组件需求的危险在于，实现可能受到限制。
> 如果有人想出了我们想要使用的更高效的实现，我们可能会发现它与我们的异常安全需求不兼容。
> 人们可能认为这在STL所涵盖的数据结构和算法领域中是无关的，但即使在那里，也取得了进展。
> 最近推出的introsort算法就是一个很好的例子，它在最坏情况的复杂度方面比成熟的快速排序有了很大的改进。"

To determine exactly how much to demand of the standard components, I looked at a typical real-world scenario. The chosen test case was a “composite container.” Such a container, built of two or more standard container components, is not only commonly needed, but serves as a simple representative case for maintaining invariants in larger systems:

```c++
// SearchableStack - A stack which can be efficiently searched 
// for any value. 
template <class T> 
class SearchableStack 
{ 
 public: 
    void push(const T& t); // O(log n) 
    void pop(); // O(log n) 
    bool contains(const T& t) const; // O(log n) 
    const T& top() const; // O(1) 
 private: 
    std::set<T> set_impl; 
    std::list<std::set<T>::iterator> list_impl; 
}; 
```

The idea is that the list acts as a stack of set iterators: every element goes into the set first, and the resulting position is pushed onto the list. The invariant is straightforward: the set and the list should always have the same number of elements, and every element of the set should be referenced by an element of the list. The following implementation of the push function is designed to give the *strong* guarantee within the natural levels of safety provided by set and list:

```c++
template <class T>                                // 1 
void SearchableStack<T>::push(const T& t)         // 2 
{                                                       // 3 
    set<T>::iterator i = set_impl.insert(t);      // 4 
    try                                                 // 5 
    {                                                   // 6 
        list_impl.push_back(i);                         // 7 
    }                                                   // 8 
    catch(...)                                          // 9 
    {                                                   // 10 
        set_impl.erase(i);                              // 11 
        throw;                                          // 12 
    }                                                   // 13 
}                                                       // 14 
```

What does our code actually require of the library? We need to examine the lines where non-const operations occur:

- Line 4: if the insertion fails but `set_impl` is modified in the process, our invariant is violated. We need to be able to rely on the *strong* guarantee from `set<T>::insert`.
- Line 7: likewise, if `push_back` fails, but `list_impl` is modified in the process, our invariant is violated, so we need to be able to rely on the *strong* guarantee from list<T>::insert.
- Line 11: here we are “rolling back” the insertion on line 4. If this operation should fail, we will be unable to restore our invariant. We absolutely depend on the *no-throw* guarantee from `set<T>::erase`.[9](https://www.boost.org/community/exception_safety.html#footnote9)
- Line 11: for the same reasons, we also depend on being able to pass the `i` to the `erase` function: we need the *no-throw* guarantee from the copy constructor of `set<T>::iterator`.

I learned a great deal by approaching the question this way during standardization. First, the guarantee specified for the composite container actually depends on stronger guarantees from its components (the *no-throw* guarantees in line 11). Also, I took advantage of all of the natural level of safety to implement this simple example. Finally, the analysis revealed a requirement on iterators which I had previously overlooked when operations were considered on their own. The conclusion was that we should provide as much of the natural level of safety as possible. Faster but less-safe implementations could always be provided as extensions to the standard components. [10](https://www.boost.org/community/exception_safety.html#footnote10)

## 7 Automated testing for exception-safety

As part of the standardization process, I produced an exception-safe reference implementation of the STL. Error-handling code is seldom rigorously tested in real life, in part because it is difficult to cause error conditions to occur. It is very common to see error-handling code which crashes the first time it is executed ...in a shipping product! To bolster confidence that the implementation actually worked as advertised, I designed an automated test suite, based on an exhaustive technique due to my colleague Matt Arnold.

The test program started with the basics: reinforcement and instrumentation, especially of the global operators `new` and `delete`.[11](https://www.boost.org/community/exception_safety.html#footnote11)Instances of the components (containers and algorithms) were created, with type parameters chosen to reveal as many potential problems as possible. For example, all type parameters were given a pointer to heap-allocated memory, so that leaking a contained object would be detected as a memory leak.

Finally, a scheme was designed that could cause an operation to throw an exception at each possible point of failure. At the beginning of every client-supplied operation which is allowed to throw an exception, a call to `ThisCanThrow` was added. A call to `ThisCanThrow` also had to be added everywhere that the generic operation being tested might throw an exception, for example in the global operator `new`, for which an instrumented replacement was supplied.

```c++
// Use this as a type parameter, e.g. vector<TestClass> 
struct TestClass 
{ 
    TestClass( int v = 0 ) 
        : p( ThisCanThrow(), new int( v ) ) {} 
    TestClass( const TestClass& rhs ) 
        : p( ThisCanThrow(), new int( *rhs.p ) ) {} 
    const TestClass& operator=( const TestClass& rhs ) 
        { ThisCanThrow(); *p = *rhs.p; } 
    bool operator==( const TestClass& rhs ) const
        { ThisCanThrow(); return *p == *rhs.p; } 
    ...etc... 
    ~TestClass() { delete p; } 
};
```

`ThisCanThrow` simply decrements a “throw counter” and, if it has reached zero, throws an exception. Each test takes a form which begins the counter at successively higher values in an outer loop and repeatedly attempts to complete the operation being tested. The result is that the operation throws an exception at each successive step along its execution path that can possibly fail. For example, here is a simplified version of the function used to test the *strong* guarantee: [12](https://www.boost.org/community/exception_safety.html#footnote12)

```c++
extern int gThrowCounter; // The throw counter
void ThisCanThrow() 
{ 
    if (gThrowCounter-- == 0) 
        throw 0; 
} 
 
template <class Value, class Operation> 
void StrongCheck(const Value& v, const Operation& op) 
{ 
    bool succeeded = false; 
    for (long nextThrowCount = 0; !succeeded; ++nextThrowCount) 
    { 
        Value duplicate = v; 
        try 
        { 
            gThrowCounter = nextThrowCount; 
            op( duplicate ); // Try the operation 
            succeeded = true; 
        } 
        catch(...) // Catch all exceptions 
        { 
            bool unchanged = duplicate == v; // Test strong guarantee 
            assert( unchanged ); 
        } 
        // Specialize as desired for each container type, to check 
        // integrity. For example, size() == distance(begin(),end()) 
        CheckInvariant(v); // Check any invariant 
    } 
}
```

Notably, this kind of testing is much easier and less intrusive with a generic component than with non-generics, because testing-specific type parameters can be used without modifying the source code of the component being tested. Also, generic functions like `StrongCheck` above were instrumental in performing the tests on a wide range of values and operations.

## 8 Further Reading

To my knowledge, there are currently only two descriptions of STL exception-safety available. The original specification [[2\]](https://www.boost.org/community/exception_safety.html#reference2) for the reference exception-safe implementation of the STL is an informal specification, simple and self-explanatory (also verbose), and uses the *basic-* and *strong-*guarantee distinctions outlined in this article. It explicitly forbids leaks, and differs substantively from the final C++ standard in the guarantees it makes, though they are largely identical. I hope to produce an updated version of this document soon.

The description of exception-safety in the C++ Standard [[1\]](https://www.boost.org/community/exception_safety.html#reference1) is only slightly more formal, but relies on hard-to-read “standardese” and an occasionally subtle web of implication.[13](https://www.boost.org/community/exception_safety.html#footnote13) In particular, leaks are not treated directly at all. It does have the advantage that it *is* the standard.

The original reference implementation [[5\]](https://www.boost.org/community/exception_safety.html#reference5) of the exception-safe STL is an adaptation of an old version of the SGI STL, designed for C++ compilers with limited features. Although it is not a complete STL implementation, the code may be easier to read, and it illustrates a useful base-class technique for eliminating exception-handling code in constructors. The full test suite [[3\]](https://www.boost.org/community/exception_safety.html#reference3) used to validate the reference implementation has been used successfully to validate all recent versions of the SGI STL, and has been adapted to test one other vendor's implementation (which failed). As noted on the documentation page, it also seems to have the power to reveal hidden compiler bugs, particularly where optimizers interact with exception-handling code.

## References

1. International Standard ISO/IEC 14882, *Information Technology-Programming Languages-C++*, Document Number ISO/IEC 14882-1998, available from http://webstore.ansi.org/ansidocstore/default.asp.
2. D. Abrahams, *Exception Safety in STLport*, available at http://www.stlport.org/doc/exception_safety.html.
3. D. Abrahams and B. Fomitchev, *Exception Handling Test Suite*, available at http://www.stlport.org/doc/eh_testsuite.html.
4. Tom Cargill, “Exception Handling: A False Sense of Security,” C++ Report, Nov-Dec 1994, also available at [http://www.awprofessional.com/content/images/020163371x/supplements/Exception_Handling_Article.html](http://www.informit.com/content/images/020163371x/supplements/Exception_Handling_Article.html).
5. B. Fomitchev, *Adapted SGI STL Version 1.0*, with exception handling code by D. Abrahams, available at [http://www.stlport.org](http://www.stlport.org/).
6. D. R. Musser, “Introspective Sorting and Selection Algorithms,” *Software-Practice and Experience* 27(8):983-993, 1997.
7. Bjarne Stroustrup, *The Design And Evolution of C++*. Addison Wesley, Reading, MA, 1995, ISBN 0-201-54330-3, Section 16.9.1.

## Footnotes

1 Probably the greatest impediment to a solution in Cargill's case was an unfortunate combination of choices on his part: the interface he chose for his container was incompatible with his particular demands for safety. By changing either one he might have solved the problem.

2 It is usually inadvisable to throw an exception from a destructor in C++, since the destructor may itself be called during the stack-unwinding caused by another exception. If the second exception is allowed to propagate beyond the destructor, the program is immediately terminated.

3 In practice of course, this function would make an extremely poor random sequence generator!

4 It is worth noting that mutating algorithms usually cannot provide the *strong* guarantee: to roll back a modified element of a range, it must be set back to its previous value using `operator=`, which itself might throw. In the C++ standard library, there are a few exceptions to this rule, whose rollback behavior consists only of destruction: `uninitialized_copy`, `uninitialized_fill`, and `uninitialized_fill_n`.

5 All type parameters supplied by clients of the C++ standard library are required not to throw from their destructors. In return, all components of the C++ standard library provide at least the *basic* guarantee.

6 Similar arrangements might have been made in the C++ standard for many of the mutating algorithms, but were never considered due to time constraints on the standardization process.

7 Associative containers whose `Compare` object might throw an exception when copied cannot use this technique, since the swap function might fail.

8 This suggests another potential use for the oft-wished-for but as yet unseen `container_traits<>` template: automated container selection to meet exception-safety constraints.

9 One might be tempted to surround the erase operation with a `try`/`catch` block to reduce the requirements on `set<T>` and the problems that arise in case of an exception, but in the end that just begs the question. First, erase just failed and in this case there are no viable alternative ways to produce the necessary result. Second and more generally, because of the variability of its type parameters a generic component can seldom be assured that any alternatives will succeed.

10 The prevalent philosophy in the design of STL was that functionality that wasn't essential to all uses should be left out in favor of efficiency, as long as that functionality could be obtained when needed by adapting the base components. This departs from that philosophy, but it would be difficult or impossible to obtain even the *basic* guarantee by adapting a base component that doesn't already have it.

11 An excellent discussion on how to fortify memory subsystems can be found in: Steve Maguire, Writing Solid Code, Microsoft Press, Redmond, WA, 1993, ISBN 1-55615- 551-4.

12 Note that this technique requires that the operation being tested be exception-neutral. If the operation ever tries to recover from an exception and proceed, the throw counter will be negative, and subsequent operations that might fail will not be tested for exception-safety.

13 The changes to the draft standard which introduced exception-safety were made late in the process, when amendments were likely to be rejected solely on the basis of the number of altered words. Unfortunately, the result compromises clarity somewhat in favor of brevity. Greg Colvin was responsible for the clever language-lawyering needed to minimize the extent of these changes.