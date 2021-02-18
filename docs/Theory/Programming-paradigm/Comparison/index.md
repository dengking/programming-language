# Comparison of programming paradigms

本章对各种programming paradigm进行比较。



## wikipedia [Comparison of programming paradigms](https://en.wikipedia.org/wiki/Comparison_of_programming_paradigms)

> NOTE: 对于有一定编程经验的人员，推荐首先阅读这篇文章，它对programming paradigm的总结是非常好的，在其[Pseudocode examples comparing various paradigms](https://en.wikipedia.org/wiki/Comparison_of_programming_paradigms#Pseudocode_examples_comparing_various_paradigms)段所列举的例子非常具有启发性，胜过了一堆理论的描述。



## OOP VS Procedural programming

维基百科[Procedural programming](https://en.wikipedia.org/wiki/Procedural_programming)

Procedural programming的一种典型写法就是：

```c++
pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);
pthread_mutex_lock(&lock);
pthread_mutex_unlock(&lock);
pthread_mutex_destroy(&lock);
```

它的模式是：定义一个`struct`，然后定义各种对这个`struct`进行操作的函数。可以参见redis的实现。

OOP中，则完全不同，OOP中，整个程序是object之间的交互，object之间是通过发送message来进行交互的。



## [Declarative programming](https://en.wikipedia.org/wiki/Declarative_programming) VS [Imperative programming](https://en.wikipedia.org/wiki/Imperative_programming)

这两种是典型的对立的两种paradigm。



## OOP VS FP

OOP是有state的，FP（functional programming）是没有state的。



## 更多

在`Theory\Programming-paradigm\Abstraction\Abstraction-in-programming-language`中以abstraction的角度对programming paradigm进行了分析。





