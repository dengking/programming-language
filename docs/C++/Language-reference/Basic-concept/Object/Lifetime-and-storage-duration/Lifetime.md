# Lifetime

## Guide

本节是我对cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 总结、梳理，能够帮助读者快速地掌握cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 中的内容。

本节内容(Object lifetime)是非常重要的，正如在`C++\Guide\Order`章节所总结的:

> C++中的很多问题，如果我们使用"order"即"次序"来进行分析，能够以清晰的、鸟瞰的视角对问题形成全面的理解。

Object lifetime就是典型的以"order"来对object进行描述；object是C family language的核心概念，C family language对object lifetime进行了非常详细的定义，通过它，我们能够将涉及到的内容有机地串联起来，形成一个统一的整体。

### Lifetime of an object

下面是以表格的形式展示的Lifetime of an object，相比于cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) ，它更加的清晰、直观。

| activity                     | 说明                                               | 章节                                                         |
| ---------------------------- | -------------------------------------------------- | ------------------------------------------------------------ |
| allocation                   | 这其实是**object creation**，会为object分配storage | `C++\Language-reference\Basic-concept\Data-model\Object\Object.md`的"Object creation"章节 |
| initialization/construction  | 初始化object                                       | `C++\Language-reference\Initialization`                      |
| deinitialization/destruction | 反初始化object                                     |                                                              |
| deallocation                 | 回收object的storage                                |                                                              |

需要注意的是：上面是按照发生顺序进行排列的，即：allocation->initialization->deinitialization->deallocation。

上述仅仅是大的流程，每个activity会涉及到的问题有: 

| 问题        | 说明                                                         |
| ----------- | ------------------------------------------------------------ |
| 1) 发生时间 | 对于不同的object，它的上述四个activity发生的时间是不同的；<br>比如不同的storage duration，它的上述activity发生的时间截然不同 |
| 2) 具体流程 | 对于不同类型的object，它的上述四个activity的具体流程是不同的；<br>不同类型的object，它的initialization流程是完全不同的，<br>比如OOP object，它的initialization涉及到了很多问题; |

C++语言对这些问题，都进行了详细的规定，后面我们将以此为思路，组织内容。对于比较特殊的object，我们会进行单独地、专门的描述，下面是汇总:

| 章节                                                         | 说明                                                    |
| ------------------------------------------------------------ | ------------------------------------------------------- |
| `C++\Language-reference\Classes\Lifetime`                    | 描述OOP object的lifetime                                |
| `C++\Language-reference\Basic-concept\Data-model\Object-with-static-storage-duration` | 描述object with static storage duration的lifetime       |
| `C++\Language-reference\Basic-concept\Data-model\Object-with-thread-local-storage-duration` | 描述object with thread local storage duration的lifetime |



### 内容简介

cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 的内容是比较杂乱的，因此有必要梳理清楚: 

| 主题                  | 注解                                                         |
| --------------------- | ------------------------------------------------------------ |
| Lifetime of object    | - [explicitly created object](https://en.cppreference.com/w/cpp/language/object#Object_creation) <br>- [implicitly created objects](https://en.cppreference.com/w/cpp/language/object#Object_creation) of implicit-lifetime types <br>原文对于下面两种特殊的object进行专门的说明: <br>- Temporary object <br>- Lifetime of objects of OOP class type(C++是支持OOP的，所以它需要对objects of OOP class type的initialization、deinitialization进行准确的定义) |
| Lifetime of reference |                                                              |

通过上述表格可以看出，C++对各种可能的object的lifetime都进行了详细的定义。

关于"OOP class type"，参见`C++\Language-reference\Basic-concept\Type-system\Type-system\OOP-class-type`。

| 主题                       | 注解                                                         |
| -------------------------- | ------------------------------------------------------------ |
| Object and storage         | 原文中并没有"Object and storage"这样的标题，它源于在`C++\Language-reference\Basic-concept\Object\Object.md`，在cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)中，其实也围绕着这个主题展开了讨论:<br>- Storage reuse: 同一个storage，可以用于多个object <br>- 对lifetime of object和storageduration的关系进行了说明 <br>所以，为了内容的连贯性，我沿用`Object.md`中的小标题来对内容进行组织 |
| Access outside of lifetime | 这是一种常见的error，这种error是和lifetime相关的，非常有必要学习 |



## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)



### Lifetime of an explicitly created object

> NOTE: 仅仅描述了大体的流程，并不涉及具体细节

#### Begin

| activity       | explanation                                                  | 注解                     |
| -------------- | ------------------------------------------------------------ | ------------------------ |
| allocation     | storage with the proper alignment and size for its type is obtained |                          |
| initialization | its initialization (if any) is complete                      | 原文对此有更加详细的讨论 |



#### End

The lifetime of an object ends when:

1) if it is of a **non-class type**, the object is destroyed (maybe via a pseudo-destructor call) (since C++20), or

2) if it is of a **class type**, the [destructor](https://en.cppreference.com/w/cpp/language/destructor) call starts, or

> NOTE: 上面两段话，简单来说: 
>
> 对于Non-OOP object，它们一般没有deinitialization;
>
> 对于OOP object，它的deinitialization对应的是它的destructor；
>
> 

3) the storage which the object occupies is released, or is reused by an object that is not nested within it.

> NOTE: 参见下面的"Storage reuse"段
>
> 关于non-class type、class type，参见`C++\Language-reference\Basic-concept\Type-system\Type-system`

### Lifetime of an implicitly created object

Some operations [implicitly create objects](https://en.cppreference.com/w/cpp/language/object#Object_creation) of implicit-lifetime types (see below) in given region of storage and start their lifetime.

> 中文意思: 一些操作隐式地在给定的存储区域中创建隐含生命期类型的对象(见下面)，并启动它们的生命期。

### Lifetime of reference

The lifetime of a [reference](https://en.cppreference.com/w/cpp/language/reference) begins when its initialization is complete and ends as if it were a **scalar object**.

> NOTE: 关于scalar type，参见`C++\Language-reference\Basic-concept\Type-system\Type-system`。

Note: the lifetime of the referred object may end before the end of the lifetime of the reference, which makes [dangling references](https://en.cppreference.com/w/cpp/language/reference#Dangling_references) possible.

> NOTE: 关于dangling reference，参见`C++\Language-reference\Reference\Reference`章节。

### Lifetime of objects of OOP class type

Lifetimes of non-static data members and base subobjects **begin** and **end** following [class initialization order](https://en.cppreference.com/w/cpp/language/initializer_list#Initialization_order).

> NOTE: 原文关于此，仅仅是简单的一笔带过，其实这其中包含着发出多的内容，在`C++\Language-reference\Classes\Lifetime`章节对它进行了具体的描述。



### Implicit-lifetime types

> NOTE: 是否是trivially-copyable type？



### Temporary object lifetime

> NOTE: 
>
> 理解原文的内容的一个非常关键的点是: 理解C++17 [Temporary materialization](https://en.cppreference.com/w/cpp/language/implicit_conversion#Temporary_materialization) 特性，这个特性改变了Temporary object lifetime的规则。因此，原文中关于"Temporary object lifetime"的讨论分为了两部分:
>
> 1) C++17前
>
> 2) C++17
>
> 本文就采取的这种描述方式。
>
> 另外一个非常重要的是: C++是支持reference to temporary的，所以可以结合cppreference [Reference initialization](https://en.cppreference.com/w/cpp/language/reference_initialization)来理解本节内容。

#### Begin

Temporary objects are created in the following situations:

1) [binding a reference to a prvalue](https://en.cppreference.com/w/cpp/language/reference_initialization)

> NOTE: 
>
> Example: 来源cppreference [Reference initialization#Explanation](https://en.cppreference.com/w/cpp/language/reference_initialization#Explanation) : 
>
> ```c++
> const std::string& rs = "abc"; // rs refers to temporary copy-initialized from char array
> const double& rcd2 = 2;        // rcd2 refers to temporary with value 2.0
> ```
>
> 

2) [initializing](https://en.cppreference.com/w/cpp/language/list_initialization) an object of type [std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list)`<T>` from a braced-init-list (since C++11)

> NOTE: Example: 源自cppreference [Functions](https://en.cppreference.com/w/cpp/language/functions):  
>
> ```C++
> #include <iostream>
> // function name: "isodd"
> // parameter list has one parameter, with name "n" and type int
> // the return type is bool
> bool isodd(int n)
> {                      // the body of the function begins
> 	return n % 2;
> }                      // the body of the function ends
> int main()
> {
> 	for (int arg : { -3, -2, -1, 0, 1, 2, 3 })
> 	{
> 		std::cout << isodd(arg) << ' '; // isodd called 7 times, each
> 										// time n is copy-initialized from arg
> 	}
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 

##### until C++17

3) returning a prvalue from a function

> NOTE: 没有例子，不好理解

4) [conversion](https://en.cppreference.com/w/cpp/language/expressions#Conversions) that creates a prvalue ([including](https://en.cppreference.com/w/cpp/language/explicit_cast) `T(a,b,c)` and `T{}`)

5) [lambda expression](https://en.cppreference.com/w/cpp/language/lambda) (since C++11)

> NOTE: 为什么lambda expression是temporary？

6) [copy-initialization](https://en.cppreference.com/w/cpp/language/copy_initialization) that requires conversion of the initializer

> NOTE: 转换过程中，会创建一个temporary

7) [reference-initialization](https://en.cppreference.com/w/cpp/language/reference_initialization) to a different but convertible type or to a bitfield.

> NOTE: 转换过程中，会创建一个temporary

##### since C++17 

> TODO: 这部分内容暂时不考虑

#### End

All **temporary objects** are destroyed as the last step in evaluating the **full-expression** that (lexically) contains the point where they were created, and if multiple **temporary objects** were created, they are **destroyed in the order opposite to the order of creation**. This is true even if that evaluation ends in throwing an exception.

There are two exceptions from that:

1) The lifetime of a **temporary object** may be extended by binding to a **const lvalue reference** or to an **rvalue reference** (since C++11), see [reference initialization](https://en.cppreference.com/w/cpp/language/reference_initialization#Lifetime_of_a_temporary) for details.

2) The lifetime of a temporary object created when evaluating the default arguments of a **default constructor** used to initialize an element of an array ends before the next element of the array begins initialization. (since C++11)

> NOTE: 没有读懂

### Object and storage

本节的标题继承自`C++\Language-reference\Basic-concept\Object\Object.md`中的同名章节，本节结合cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 中的内容，从lifetime的角度对Object and storage之间的关系进行论述: 

> Lifetime of an object is equal to or is nested within the lifetime of its storage, see [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration).

"nested"是因为: [Storage reuse](https://en.cppreference.com/w/cpp/language/lifetime#Storage_reuse): 同一个storage，可以用于多个object，即存在object已经end了，但是storage还存在。

#### TODO [Storage reuse](https://en.cppreference.com/w/cpp/language/lifetime#Storage_reuse)

> NOTE: 这段内容是值得一读的

### [Access outside of lifetime](https://en.cppreference.com/w/cpp/language/lifetime#Access_outside_of_lifetime)

> NOTE: 这是一种常见的memory error，它和 object lifetime相关的，这部分内容放到了 `Ordering-and-object-lifetime\Access-outside-of-object-lifetime` 中。

