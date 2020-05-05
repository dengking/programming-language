# Downcast and upcast



## 维基百科[Downcasting](https://en.wikipedia.org/wiki/Downcasting)

In [class-based programming](https://en.wikipedia.org/wiki/Class-based_programming), **downcasting** or type refinement is the act of [casting](https://en.wikipedia.org/wiki/Type_conversion) a reference of a base class to one of its derived classes.

> NOTE: 为什么叫downcast？这是因为在class hierarchy diagram中，base class往往处于root位置，所以处于最顶端，显然down表示的是从上到下，即从更顶端的base class到更底端的derived class。



## upcast

在OOP语言中，upcast其实是默认支持的行为，它就是[Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle)，所以我们在进行program的时候，需要显示进行cast的是downcast。