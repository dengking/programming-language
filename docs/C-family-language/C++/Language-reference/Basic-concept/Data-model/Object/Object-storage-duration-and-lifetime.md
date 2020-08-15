# Object storage duration and lifetime

storage duration和lifetime是[object](https://en.cppreference.com/w/cpp/language/object)的重要属性，这两个属性是密切相关的，在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)对此进行了介绍，本文讨论object的storage duration和lifetime。

object的storage duration和lifetime是两个非常重要的概念，是理解后续很多内容的基础：

- allocation
- initialization（在cppreference中，对initialization有着专门的描述）
- destruction
- deallocation

需要注意的是：上面是按照发生顺序进行排列的，即：allocation->initialization->destruction->deallocation。

## cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)

> NOTE: 原文的内容是比较杂乱的，既包含了**storage duration**又包含了**linkage**，实际上它们两者是independent property of object，所以应该分开来进行讨论，linkage的讨论，参见`C-family-language\C-and-C++\From-source-code-to-exec\Link\Linkage`章节；
>
> 原文之所以将它们放到一起是因为：C++和C并没有提供专门分别描述这两种property的specifier，而是提供的合并的specifier，关于这一点，在[Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)中进行了详细的讨论。
>
> 我们按照在`Theory\Programming-language\How-to-master-programming-language.md#`中总结的：首先学习property，然后学习描述这些property的specifier的方式来进行学习。
>
> 本文仅仅关注原文中object storage duration相关的内容，我们以如下的思路来组织内容：
>
> - C++ object有哪几种storage duration
> - C++ language提供了哪些specifier来供programmer对storage duration进行控制/描述，即C++ language中，有哪些Storage class specifiers
>
> 所以，本文与原文的组织有所差异。

### [Storage duration](https://en.cppreference.com/w/cpp/language/storage_duration#Storage_duration)

> NOTE: 下面描述该表格的组织以及各列的含义
>
> 对于每种storage duration，都有对应的“allocation time point”（何时分配）和 “dealloaction time point”（何时被回收）。
>
> 另外一个非常重要的问题是：判断object具备哪种storage duration，这在“objects”列中说明。
>
> scope是我们从OS的角度来分析storage duration。

| storage duration         | allocation time point                                        | dealloaction time point                                      | objects                                                      | scope    | explanation                  |
| ------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | -------- | ---------------------------- |
| automatic                | the object is allocated at the beginning of the enclosing code block | `C++`: deallocated at the end <br>C: deallocated when it is exited by any means ([goto](https://en.cppreference.com/w/c/language/goto), [return](https://en.cppreference.com/w/c/language/return), reaching the end) | 1. local objects, **except** those declared `static` （**static object**）, `extern`（**extern object**） or `thread_local`. | function | 与此相关的一个主要概念是RAII |
| thread <br>(since C++11) | the object is allocated when the thread begins               | deallocated when the thread ends                             | 1. objects declared `thread_local` have this storage duration | thread   |                              |
| static                   | the object is allocated when the **program** begins          | deallocated when the **program** ends                        | 1. objects declared at namespace scope (including **global namespace**) <br>2. those declared with `static` or `extern` （包括**static local object**、**extern local object**） | process  |                              |
| dynamic                  | the object is allocated by using [dynamic memory allocation](https://en.cppreference.com/w/cpp/memory) function | deallocated by using [dynamic memory deallocation](https://en.cppreference.com/w/cpp/memory) function |                                                              |          |                              |

> NOTE:
>
> 上述storage duration的分类中，并不包含对 temporary object的说明，关于temporary object，参见cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) `#`  “Temporary object lifetime” 章节

#### Example: ***automatic*** storage duration: extern local object

来源：[How does linker handle variables with different linkages?](https://stackoverflow.com/questions/51737002/how-does-linker-handle-variables-with-different-linkages) `#` [A](https://stackoverflow.com/a/51737215)

```c++
#include <iostream>

static int x; // a namespace scope, so `x` has internal linkage, static storage duration

int f()
{
	extern int x; // static storage duration
	++x;
}

int g()
{
	extern int x; // static storage duration
	std::cout << x << '\n';
}

int main()
{
	g();
	f();
	g();
}
// g++ test.cpp

```



### [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)

> NOTE: 本节描述描述storage duration的specifier

| specifier                   | storage duration                                             | linkage    | C++ version                                                  | Explanation                                                  | function         |
| --------------------------- | ------------------------------------------------------------ | ---------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ---------------- |
| `auto` (until C++11)        | *automatic*                                                  | no linkage | 在[Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)的Notes有说明: Since C++11, `auto` is no longer a storage class specifier; it is used to indicate type deduction. |                                                              | 不可修饰function |
| `register` (until C++17)    | *automatic*                                                  | no linkage |                                                              |                                                              | 不可修饰function |
| `static`                    | *static* or *thread*                                         | *internal* |                                                              |                                                              | 可以修饰function |
| `extern`                    | *static* or *thread*                                         | *external* |                                                              | It specifies **external linkage**, and does not technically affect **storage duration**, but it cannot be used in a **definition** of an **automatic storage duration** object, so all `extern` objects have **static** or **thread** durations. In addition, a variable declaration that uses `extern` and has no initializer is not a [definition](https://en.cppreference.com/w/cpp/language/definition).<br> 上面这段话的意思是：`extern` variable只能够link to **object with static storage** or **object with `thread_local` storage**，所以`extern` variable的storage duration是static的，这是因为显然它需要compiler和linker在编译阶段就能够找到这个object。 | 可以修饰function |
| `thread_local`(since C++11) | *thread*                                                     |            |                                                              |                                                              | 不可修饰function |
| `mutable`                   | does not affect storage duration or linkage. See [const/volatile](https://en.cppreference.com/w/cpp/language/cv) for the explanation. |            |                                                              |                                                              | 不可修饰function |

> NOTE: 
>
> 无论是`C++`还是C，都没有专门描述linkage的specifier，而是将描述**storage duration**和描述**linkage**的specifier合并在一起，对于linkage，并没有单独描述它的specifier，但是，compiler提供了default linkage；关于这一点，我们需要仔细阅读cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration) 和 creference [Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)：
>
> cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration) 中，对specifiers的描述如下：
>
> > The **storage class specifiers** are a part of the *decl-specifier-seq* of a name's [declaration syntax](https://en.cppreference.com/w/cpp/language/declarations). Together with the [scope](https://en.cppreference.com/w/cpp/language/scope) of the name, they control two independent properties of the name: its *storage duration* and its *linkage*.
>
> 在`C++`中，将这些specifier称为 storage class specifier。
>
> creference [Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration) 中，对specifiers的描述如下：
>
> > Specify *storage duration* and *linkage* of objects and functions
>
> 在`C`中，将这些specifier称为 storage class specifier。
>
> 
>
> 我们需要深入思考：为什么将linkage和storage duration的specifier合并？
>
> 关于此的原因之一可以参看上述table中`extern`的Explanation，原因之二则是出于语言设计者出于对语言简便性的考虑（在`Theory\Programming-languageDesign-of-programming-language.md\#Design of specifier`中进行了讨论）



#### linkage and storage duration of function

需要注意的是：对于function而言，它没有**storage duration** property，只有**linkage** property，对于function而言，讨论它的storage duration是没有意义的。对于object而言，它既有**storage duration** property，又有**linkage** property。

`static` 、 `extern` 也可以 修饰 function，linkage





#### `static` specifier and static storage duration

使用`static` specifier修饰的object具有static storage duration，但是具有static storage duration的object，不一定要使用`static` specifier来修饰。

### Static local variables

> NOTE: 对于static local variable，

Variables declared at **block scope** with the specifier `static` or `thread_local` (since C++11) have static or thread (since C++11) storage duration but are initialized the first time control passes through their declaration (unless their initialization is [zero-](https://en.cppreference.com/w/cpp/language/zero_initialization) or [constant-initialization](https://en.cppreference.com/w/cpp/language/constant_initialization), which can be performed before the block is first entered). On all further calls, the declaration is skipped.

> NOTE: 上面这段话关于initialization的描述是不易理解的？它的意思是：对于static local variable，它的initialization的发生时间如下：
>
> - [zero-](https://en.cppreference.com/w/cpp/language/zero_initialization) or [constant-initialization](https://en.cppreference.com/w/cpp/language/constant_initialization) can be performed before the block is first entered
> - others are initialized the first time control passes through their declaration



#### Initialization of static local variable concurrently (since C++11)

> NOTE: 这种情况的典型就是：线程执行函数中声明了一个static local variable。

If multiple threads attempt to initialize the same **static local variable** concurrently, the **initialization** occurs exactly once (similar behavior can be obtained for arbitrary functions with [std::call_once](../thread/call_once.html)).

Note: usual implementations of this feature use variants of the double-checked locking pattern, which reduces runtime overhead for already-initialized local statics to a single non-atomic boolean comparison.

> NOTE: double-checked locking pattern在工程parallel-computing的`Synchronization\Lock`章节描述。



## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)



### Temporary object lifetime



### Storage reuse



### Access outside of lifetime



