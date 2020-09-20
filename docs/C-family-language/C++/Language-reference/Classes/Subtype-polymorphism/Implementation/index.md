# 关于本章

本章描述C++ Subtype polymorphism的实现。



## draft

需要考虑，使用哪个实现方式，因为可能的实现方式是多个的。

#### 实现方式

[Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)



most derived class

在https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019中提及了

> For example, if a class has virtual functions, all the instances of that class might share a single virtual function table



需要对比c++ runtime polymorphysim的实现和Python的runtime polymorphysim的实现，两者都需要存储 映射关系。c++的vtbl和Python的`__dict__`类似的。Python仅仅有reference semantic的。



这本质是是对function call的实现，即根据function name，调用对应的function。compiler编译过程中，的scope resolution过程，与上述过程也是非常类似的，都是逐级搜索；我应该根据compiler-time、runtime来进行区分，对两者的实现逻辑进行对比；它们本质上都是建立起映射关系：function name到function的映射；它们的搜索过程，都是逐级进行的。



在文章artima ["Pure Virtual Function Called": An Explanation](https://www.artima.com/cppsource/pure_virtual.html)中对此进行了解释。

