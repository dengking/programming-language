# Type Erasure technique

1、本质上是为了实现consistent interface

2、Type Erasure 是实现 polymorphic value type 的基础

## Implementation techniques

本节对各种实现type erasure的technique进行总结。

### 总结地非常好的文章

下面是对type erasure technique总结得非常好的文章:

一、sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

其中给出了各种实现方式，非常详细。



二、groups.google [Safe 'type erasure' without runtime memory allocation](https://groups.google.com/a/isocpp.org/g/std-proposals/c/eWl57Y96_iI)

> Generally, the 'type erasure' pattern involves the use of:
>
> \1. boost/std any - which is inefficient because of the need to query the object or it forces use of the visitor pattern which scatters the logic all over the place.
>
> \2. A custom abstract interface - which leads to runtime memory allocation.
>
> 

上述总结是非常好的，其中的第二条，其实就是下面的"OOP interface"。



###  OOP interface + template implementation

一、最常见的实现方式是: 

声明一个abstract base class，在其中定义[pure]virtual method(consistent interface)，然后让各个[sub|implementation]class[template]来implement(通过inheritance)这个abstract base class

二、实现了consistent type、consistent interface:

consistent interface: behavior

consistent type: type、data

三、这种方式是符合C++的，它能够实现type safety

四、它是符合 CppCoreGuidelines [T.84: Use a non-template core implementation to provide an ABI-stable interface](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t84-use-a-non-template-core-implementation-to-provide-an-abi-stable-interface)

五、在下面文章中，给出了样例code

1、stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) 

2、sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

六、sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

> Type erasure is a set of techniques for creating a type that can provide a **uniform interface** to various underlying types, while hiding the underlying type information from the client

program to abstraction and polymorphism。

#### 我的实践

对于像`CTable`这样的class template，我创建了多个specialization，同时我想要创建table name 到 object 的映射关系。显然，实现"table name 到 object 的映射关系"的做好的做法是: 将它们存到一个map中，但是由于各个specialization的type是不一致的，因此无法定义这样的map，那有没有解决方案呢？我的做法是: 声明一个abstract base class，在其中定义[pure]virtual method(consistent interface)，然后让各个class template来implement(通过inheritance)这个abstract base class。

> NOTE: 其实我的上述实现思路，即使用map来进行映射，是属于典型的custom virtual table-generic function pattern，这种实现方式是不推荐的。



### Custom virtual table

一、参见 `Custom-virtual-table` 章节

二、这种方式中，一般需要使用`void` pointer for data，因此，在type safety方面不及第一种方式 



### `shared_ptr<void>`、`unique_ptr<void>`

1、这是在  stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) # [A](https://stackoverflow.com/a/6044720)  中提出的。

2、在 sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure) 中，也使用了

### Optimization: small buffer optimization

Small buffer optimization是一种场景的optimization technique，由于type erasure中，通常涉及dynamic allocation，因此可以使用small buffer optimization来进行优化。

参见 `Small-object-buffer-optimization` 章节。



## C++ standard library support for type erasure

1、[std::any](https://en.cppreference.com/w/cpp/utility/any)

2、[std::variant](https://en.cppreference.com/w/cpp/utility/variant)

> NOTE: 貌似不涉及，需要验证



## TODO

1、modernescpp [C++ Core Guidelines: Type Erasure](https://www.modernescpp.com/index.php/c-core-guidelines-type-erasure)

2、quuxplusone [ What is Type Erasure?](https://quuxplusone.github.io/blog/2019/03/18/what-is-type-erasure/)

3、cplusplus [C++ type erasure](http://www.cplusplus.com/articles/oz18T05o/)

4、davekilian [C++ 'Type Erasure' Explained](https://davekilian.com/cpp-type-erasure.html)

5、lucadavidian [Type erasure in C++](http://www.lucadavidian.com/2019/08/26/type-erasure-in-c/)



### wikipedia [Type erasure](http://en.wikipedia.org/wiki/Type_erasure)