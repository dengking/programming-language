# Object storage duration and lifetime

storage duration和lifetime是[object](https://en.cppreference.com/w/cpp/language/object)的重要属性，这两个属性是密切相关的，在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)对此进行了介绍，本文讨论object的storage duration和lifetime。

object的storage duration和lifetime是两个非常重要的概念，是理解后续很多内容的基础：

- allocation

- initialization（在cppreference中，对initialization有着专门的描述）
- deallocation（destruction）

## cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)

> NOTE: 原文的内容是比较杂乱的，既包含了**storage duration**又包含了**linkage**，实际上它们两者是independent property of object，所以应该分开来进行讨论，linkage的讨论，参见`C-family-language\C-and-C++\From-source-code-to-exec\Link\Linkage`章节；
>
> 原文之所以将它们放到一起是因为：C++和C并没有提供专门分别描述这两种property的specifier，而是提供的合并的specifier，关于这一点，在`C-family-language\C-and-C++\From-source-code-to-exec\Link\LinkageLinkage.md#linkage and storage duration specifiers`中进行了详细的讨论。
>
> 我们按照在`Theory\Programming-language\How-to-master-programming-language.md#`中总结的：首先学习property，然后学习描述这些property的specifier的方式来进行学习。
>
> 本文仅仅关注原文中object storage duration相关的内容，我们以如下的思路来组织内容：
>
> - C++ object有哪几种storage duration
> - C++ language提供了哪些specifier来供programmer对storage duration进行控制/描述，即C++ language中，有哪些Storage class specifiers
>
> 所以，本文与原文的组织有所差异

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

| specifier                   | storage duration                                             | linkage    | C++ version                                                  | Explanation                                                  |
| --------------------------- | ------------------------------------------------------------ | ---------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `auto` (until C++11)        | *automatic*                                                  | no linkage | 在[Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)的Notes有说明: Since C++11, `auto` is no longer a storage class specifier; it is used to indicate type deduction. |                                                              |
| `register` (until C++17)    | *automatic*                                                  | no linkage |                                                              |                                                              |
| `static`                    | *static* or *thread*                                         | *internal* |                                                              |                                                              |
| `extern`                    | *static* or *thread*                                         | *external* |                                                              | It specifies **external linkage**, and does not technically affect **storage duration**, but it cannot be used in a **definition** of an **automatic storage duration** object, so all `extern` objects have **static** or **thread** durations. In addition, a variable declaration that uses `extern` and has no initializer is not a [definition](https://en.cppreference.com/w/cpp/language/definition).<br> 上面这段话的意思是：`extern` variable只能够link to **object with static storage** or **object with `thread_local` storage**。 |
| `thread_local`(since C++11) | *thread*                                                     |            |                                                              |                                                              |
| `mutable`                   | does not affect storage duration or linkage. See [const/volatile](https://en.cppreference.com/w/cpp/language/cv) for the explanation. |            |                                                              |                                                              |





## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)



