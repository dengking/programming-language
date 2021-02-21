

# Virtual method template 

前面已经知道virtual method template是不可行的，那是否有方法来实现类似的效果呢？

## 意图

从本质上来说，使用 virtual method template的意图是: polymorphism。这种polymorphism，可以是static的也可以是dynamic的。

> draft: 下面是我之前的说法:
>
> 有的时候，希望参数化基类的virtual method的类型，然后子类来进行override，但是非常明显的是，c++是不允许Virtual template method，那如何实现呢？

## 解决方案

下面是我目前想到的解决方案：

### 使用template class，将这些模板参数放到class级别

"template class with non-template virtual method"是可行的，但是这种方案的不能够解决所有的问题，比如类似如下的形式:

```C++
template<typename T>
void Test(T t)
{}
```

由于它的入参是参数的化的，显然使用本节描述的这种方案是不行的



### 使用`void*` type erasure

1、这种方案是使用C generic programming的思路，它其实对应的是type erasure，但是感觉它不是C++的做法。

2、按照artima [On the Tension Between Object-Oriented and Generic Programming in C++](https://www.artima.com/cppsource/type_erasure.html) 中的说法，它是:

> Type Erasure as the Glue between OO and Generic Programming



### 使用policy-based design

这种做法是非常符合C++ template meta programming的:

1、它能够实现polymorphism 

2、它是type safe的

3、高效: static polymorphism

在下面章节中，对它进行了介绍:

1、`rivermatrix-C++templates-with-virtual-functions`

2、`Policy-based-design`



使用第一种方式的一个需要注意的点是：不是同一种类型，无法进行多态。

https://stackoverflow.com/questions/7968023/c-virtual-template-method

https://stackoverflow.com/questions/2354210/can-a-class-member-function-template-be-virtual?rq=1

https://www.experts-exchange.com/questions/20558666/virtual-template-function-workaround.html


