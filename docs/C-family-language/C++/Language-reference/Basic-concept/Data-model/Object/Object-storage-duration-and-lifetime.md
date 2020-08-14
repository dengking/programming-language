# Object storage duration and lifetime

storage duration和lifetime是[object](https://en.cppreference.com/w/cpp/language/object)的重要属性，这两个属性是密切相关的，在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)对此进行了介绍。

object的storage duration和lifetime是两个非常重要的概念，是理解后续很多内容的基础：

- initialization（在cppreference中，对initialization有着专门的描述）
- destruction

## cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)

> NOTE: 原文的内容是比较杂乱的，既包含了**storage duration**又包含了**linkage**，实际上它们两者是independent property of object，所以应该分开来进行讨论，linkage的讨论，参见`C-family-language\C-and-C++\From-source-code-to-exec\Link\Linkage`章节；
>
> 原文之所以将它们放到一起是因为：C++和C并没有提供专门描述这两种property的specifier，而是提供的合并的specifier，关于这一点，在`C-family-language\C-and-C++\From-source-code-to-exec\Link\LinkageLinkage.md#linkage and storage duration specifiers`中进行了详细的讨论。
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

> NOTE: 对于每种storage duration，都有对应的allocation time point 和 dealloaction time point。

#### automatic

> NOTE: *automatic* storage duration

#### static

All objects declared at namespace scope (including global namespace) have this storage duration, plus those declared with `static` or `extern`. See [Non-local variables](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables) and [Static local variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables) for details on initialization of objects with this storage duration.

#### thread (since C++11)



#### dynamic



### [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)

> NOTE: 本节描述描述storage duration的specifier

| specifier                   | storage duration                                             | linkage    | C++ version                                                  | Explanation                                                  |
| --------------------------- | ------------------------------------------------------------ | ---------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `auto` (until C++11)        | *automatic*                                                  | no linkage | 在[Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)的Notes有说明: Since C++11, `auto` is no longer a storage class specifier; it is used to indicate type deduction. |                                                              |
| `register` (until C++17)    | *automatic*                                                  | no linkage |                                                              |                                                              |
| `static`                    | *static* or *thread*                                         | *internal* |                                                              |                                                              |
| `extern`                    | *static* or *thread*                                         | *external* |                                                              | It specifies **external linkage**, and does not technically affect **storage duration**, but it cannot be used in a definition of an **automatic storage duration** object, so all `extern` objects have **static** or **thread** durations. In addition, a variable declaration that uses `extern` and has no initializer is not a [definition](https://en.cppreference.com/w/cpp/language/definition). |
| `thread_local`(since C++11) | *thread*                                                     |            |                                                              |                                                              |
| `mutable`                   | does not affect storage duration or linkage. See [const/volatile](https://en.cppreference.com/w/cpp/language/cv) for the explanation. |            |                                                              |                                                              |





## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)



