# Object storage duration and lifetime



## cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)

> NOTE: 原文的内容是比较杂乱的，既包含了**storage duration**又包含了**linkage**，实际上它们两者是independent property of object，所以应该分开来进行讨论，linkage的讨论，参见`C-family-language\C-and-C++\From-source-code-to-exec\Link\Linkage`章节；
>
> 原文之所以将它们放到一起是因为：C++和C并没有提供专门描述这两种property的specifier，而是提供的合并的specifier，关于这一点，在`C-family-language\C-and-C++\From-source-code-to-exec\Link\LinkageLinkage.md#linkage and storage duration specifiers`中进行了详细的讨论。
>
> 我们按照在中卖对于language的学习，我们应该首先学习property，然后学习描述这些property的specifier。
>
> 本文仅仅关注原文中object storage duration相关的内容，我们以如下的思路来组织内容：
>
> - C++ object有哪几种storage duration
> - C++ language提供了哪些specifier来供programmer对storage duration进行控制/描述，即C++ language中，有哪些Storage class specifiers
>
> 所以，本文与原文的组织有所差异

### [Storage duration](https://en.cppreference.com/w/cpp/language/storage_duration#Storage_duration)

#### ***automatic***



#### ***static***



#### ***thread***



#### ***dynamic***

### [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)

`auto`



`register`



`static`



`extern`



`thread_local`



`mutable`





## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)