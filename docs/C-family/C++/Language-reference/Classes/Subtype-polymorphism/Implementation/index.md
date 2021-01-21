# 关于本章

本章描述C++ Subtype polymorphism的实现。



## draft

需要考虑，使用哪个实现方式，因为可能的实现方式是多个的。

#### 实现方式

wikipedia [Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)



most derived class

在https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019中提及了

> For example, if a class has virtual functions, all the instances of that class might share a single virtual function table



需要对比c++ runtime polymorphysim的实现和Python的runtime polymorphysim的实现，两者都需要存储 映射关系。c++的vtbl和Python的`__dict__`类似的。Python仅仅有reference semantic的。



这本质是是对function call的实现，即根据function name，调用对应的function。compiler编译过程中，的scope resolution过程，与上述过程也是非常类似的，都是逐级搜索；我应该根据compiler-time、runtime来进行区分，对两者的实现逻辑进行对比；它们本质上都是建立起映射关系：function name到function的映射；它们的搜索过程，都是逐级进行的。



在文章artima ["Pure Virtual Function Called": An Explanation](https://www.artima.com/cppsource/pure_virtual.html)中对此进行了解释。



## draft2

这个draft，是从`C++\Language-reference\Basic-concept\Type-system\Type-system\OOP-class-type\Polymorphic-type.md`中迁移过来的。

### Implementation of dynamic type

在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)`#`[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)中给出了这个问题的解答：

> Within each polymorphic object, the implementation stores additional information (in every existing implementation, it is one pointer unless optimized out), which is used by [virtual function](https://en.cppreference.com/w/cpp/language/virtual) calls and by the RTTI features ([dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast) and [typeid](https://en.cppreference.com/w/cpp/language/typeid)) to determine, at run time, the type with which the object was created, regardless of the expression it is used in.

#### RTTI of polymorphic type

对于polymorphic type，目前的实现普遍会使用RTTI，这在`C++\Language-reference\Basic-concept\Type-system\RTTI.md`中进行了描述。

#### Virtual function table of polymorphic type

Virtual function table参见`C-and-C++\From-source-code-to-exec\ABI\Itanium-Cpp-ABI\Virtual-method-table.md`。



## draft3

在wikipedia [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))中有如下总结，我觉得描述地非常好: 

[Object-oriented programming languages](https://en.wikipedia.org/wiki/Object-oriented_programming_language) offer subtype polymorphism using *[subclassing](https://en.wikipedia.org/wiki/Subclass_(computer_science))* (also known as *[inheritance](https://en.wikipedia.org/wiki/Inheritance_in_object-oriented_programming)*). In typical implementations, each class contains what is called a *[virtual table](https://en.wikipedia.org/wiki/Virtual_table)*—a table of functions that implement the polymorphic part of the class interface—and each object contains a pointer to the "vtable" of its class, which is then consulted whenever a polymorphic method is called. This mechanism is an example of:

- *[late binding](https://en.wikipedia.org/wiki/Late_binding)*, because virtual function calls are not bound until the time of invocation;

- *[single dispatch](https://en.wikipedia.org/wiki/Single_dispatch)* (i.e. single-argument polymorphism), because virtual function calls are bound simply by looking through the vtable provided by the first argument (the `this` object), so the runtime types of the other arguments are completely irrelevant.

  > NOTE: 这一段从实现层面详细描述了C++仅仅支持single dispatch的原因。

