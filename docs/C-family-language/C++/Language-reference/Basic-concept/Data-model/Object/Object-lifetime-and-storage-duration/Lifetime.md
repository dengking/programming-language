# Lifetime

## Guide

本节是我对cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 总结、梳理，能够帮助读者快速地掌握cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 中的内容。

本节内容(Object lifetime)是非常重要的，正如在`C++\Guide\Order`章节所总结的:

> C++中的很多问题，如果我们使用"order"即"次序"来进行分析，能够以清晰的、鸟瞰的视角对问题形成全面的理解。

Object lifetime就是典型的以"order"来对object进行描述；object是C family language的核心概念，C family language对object lifetime进行了非常详细的定义，通过它，我们能够将涉及到的内容有机地串联起来，形成一个统一的整体。

### Lifetime of an object

下面是以表格的形式展示的Lifetime of an object，相比于cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) ，它更加的清晰、直观。

| activity         | 说明                                               | 章节                                                         |
| ---------------- | -------------------------------------------------- | ------------------------------------------------------------ |
| allocation       | 这其实是**object creation**，会为object分配storage | `C++\Language-reference\Basic-concept\Data-model\Object\Object.md`的"Object creation"章节 |
| initialization   | 初始化object                                       | `C++\Language-reference\Initialization`                      |
| deinitialization | 反初始化object                                     |                                                              |
| deallocation     | 回收object的storage                                |                                                              |

需要注意的是：上面是按照发生顺序进行排列的，即：allocation->initialization->deinitialization->deallocation。

上述仅仅是大的流程，每个activity会涉及到的问题有: 

| 问题        | 说明                                                         |
| ----------- | ------------------------------------------------------------ |
| 1) 发生时间 | 对于不同的object，它的上述四个activity发生的时间是不同的；<br>比如不同的storage duration，它的上述activity发生的时间截然不同 |
| 2) 具体流程 | 对于不同类型的object，它的上述四个activity的具体流程是不同的；<br>不同类型的object，它的initialization流程是完全不同的，<br>比如OOP object，它的initialization涉及到了很多问题; |

C++语言对这些问题，都进行了详细的规定，后面我们将以此为思路，组织内容。

### 内容简介

cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 的内容是比较杂乱的，因此有必要梳理清楚: 

| 主题                  | 注解                                                         |
| --------------------- | ------------------------------------------------------------ |
| Lifetime of object    | - [explicitly created object](https://en.cppreference.com/w/cpp/language/object#Object_creation) <br>- [implicitly created objects](https://en.cppreference.com/w/cpp/language/object#Object_creation) of implicit-lifetime types <br>- temporary object <br>- Lifetime of objects of OOP class type(C++是支持OOP的，所以它需要对objects of OOP class type的initialization、deinitialization进行准确的定义) |
| Lifetime of reference |                                                              |

通过上述表格可以看出，C++对各种可能的object的lifetime都进行了详细的定义。

关于"OOP class type"，参见`C++\Language-reference\Basic-concept\Type-system\Type-system\OOP-class-type`。

| 主题                       | 注解                              |
| -------------------------- | --------------------------------- |
| Storage reuse              | 同一个storage，可以用于多个object |
| Access outside of lifetime |                                   |



## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)



### Lifetime of an explicitly created object

#### Begin

| activity                                                     | 注解                     |
| ------------------------------------------------------------ | ------------------------ |
| storage with the proper alignment and size for its type is obtained |                          |
| its initialization (if any) is complete                      | 原文对此有更加详细的讨论 |



#### End

The lifetime of an object ends when:

1) if it is of a non-class type, the object is destroyed (maybe via a pseudo-destructor call) (since C++20), or

2) if it is of a class type, the [destructor](https://en.cppreference.com/w/cpp/language/destructor) call starts, or

3) the storage which the object occupies is released, or is reused by an object that is not nested within it.

> NOTE: 参见下面的"Storage reuse"段

### Lifetime of an implicitly created object

Some operations [implicitly create objects](https://en.cppreference.com/w/cpp/language/object#Object_creation) of implicit-lifetime types (see below) in given region of storage and start their lifetime.

> 中文意思: 一些操作隐式地在给定的存储区域中创建隐含生命期类型的对象(见下面)，并启动它们的生命期。

### Lifetime of reference

The lifetime of a [reference](https://en.cppreference.com/w/cpp/language/reference) begins when its initialization is complete and ends as if it were a scalar object.

Note: the lifetime of the referred object may end before the end of the lifetime of the reference, which makes [dangling references](https://en.cppreference.com/w/cpp/language/reference#Dangling_references) possible.



### Lifetime of objects of OOP class type

> NOTE: 关于

Lifetimes of non-static data members and base subobjects **begin** and **end** following [class initialization order](https://en.cppreference.com/w/cpp/language/initializer_list#Initialization_order).



### Implicit-lifetime types

> NOTE: 是否是trivially-copyable type？



### Temporary object lifetime

> NOTE: 理解原文的内容的一个非常关键的点是: 理解C++17 [Temporary materialization](https://en.cppreference.com/w/cpp/language/implicit_conversion#Temporary_materialization) 特性，这个特性改变了temporary的规则。因此，原文中关于"Temporary object lifetime"的讨论分为了两部分:
>
> 1) C++17前
>
> 2) C++17
>
> 本文就采取的这种描述方式。

Temporary objects are created in the following situations:

[binding a reference to a prvalue](https://en.cppreference.com/w/cpp/language/reference_initialization)

[initializing](https://en.cppreference.com/w/cpp/language/list_initialization) an object of type [std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list)`<T>` from a braced-init-list (since C++11)





All **temporary objects** are destroyed as the last step in evaluating the **full-expression** that (lexically) contains the point where they were created, and if multiple **temporary objects** were created, they are destroyed in the order opposite to the order of creation. This is true even if that evaluation ends in throwing an exception.

