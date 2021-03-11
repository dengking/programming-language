# Comparison of programming paradigms

本章对各种programming paradigm进行比较。



## wikipedia [Comparison of programming paradigms](https://en.wikipedia.org/wiki/Comparison_of_programming_paradigms)

> NOTE: 对于有一定编程经验的人员，推荐首先阅读这篇文章，它对programming paradigm的总结是非常好的，在其[Pseudocode examples comparing various paradigms](https://en.wikipedia.org/wiki/Comparison_of_programming_paradigms#Pseudocode_examples_comparing_various_paradigms)段所列举的例子非常具有启发性，胜过了一堆理论的描述。



## [Declarative programming](https://en.wikipedia.org/wiki/Declarative_programming) VS [Imperative programming](https://en.wikipedia.org/wiki/Imperative_programming)

这两种是典型的对立的两种paradigm。



## OOP VS Procedural programming

素材: 

1、wikipedia [Procedural programming](https://en.wikipedia.org/wiki/Procedural_programming)

2、Procedural programming的一种典型写法就是：

```c++
pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);
pthread_mutex_lock(&lock);
pthread_mutex_unlock(&lock);
pthread_mutex_destroy(&lock);
```

它的模式是：定义一个`struct`，然后定义各种对这个`struct`进行操作的函数。可以参见redis的实现。

OOP中，则完全不同，OOP中，整个程序是object之间的交互，object之间是通过发送message来进行交互的。



## OOP VS FP

OOP是有state的，FP（functional programming）是没有state的。

下面是描述关于OOP VS FP非常好的文章:

1、eli.thegreenplace [The Expression Problem and its solutions](https://eli.thegreenplace.net/2016/the-expression-problem-and-its-solutions/)



## OOP VS AOP

|         | 抽象内容                                                  | 方向         |
| ------- | --------------------------------------------------------- | ------------ |
| **AOP** | 抽象出 *cross-cutting concerns* 即 横切关注点 或者 aspect | 是**横向**的 |
| **OOP** | 抽象出 base class、object、class                          | 是**纵向**的 |

两者都能够让我们**抽象**出公共部分，实现code reuse。

### AOP VS Mixin

AOP是一种编程思想，它告诉我们使用aspect来进行抽象；

Mixin是OOP中的一种技巧，目的是促进code reuse，参见`Theory\Programming-paradigm\Object-oriented-programming\Assemble\Mixin`;

两者都能够让我们实现code reusage。

Mixin and AOP：https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Parameterized_Base_Class，在`C++\Idiom\TMP\Parameterized-Base-Class\Parameterized-Base-Class`中收录了这篇文章。



### Good article

1) 在 [C++11实现一个轻量级的AOP框架](https://www.cnblogs.com/qicosmos/p/4772389.html) 中进行了介绍:

> AOP（Aspect-Oriented Programming，面向方面编程），可以解决面向对象编程中的一些问题，是OOP的一种有益补充。面向对象编程中的继承是一种从上而下的关系，不适合定义从左到右的横向关系，如果继承体系中的很多无关联的对象都有一些公共行为，这些公共行为可能分散在不同的组件、不同的对象之中，通过继承方式提取这些公共行为就不太合适了。使用AOP还有一种情况是为了提高程序的可维护性，AOP将程序的非核心逻辑都“横切”出来，将非核心逻辑和核心逻辑分离，使我们能集中精力在核心逻辑上，例如图1所示的这种情况。

2) [Aspect Oriented Programming vs. Object-Oriented Programming](https://stackoverflow.com/questions/232884/aspect-oriented-programming-vs-object-oriented-programming) # [A](https://stackoverflow.com/a/232918):



## AOP VS Meta-programming

1) [Aspect Oriented Programming vs. Object-Oriented Programming](https://stackoverflow.com/questions/232884/aspect-oriented-programming-vs-object-oriented-programming) # [A](https://stackoverflow.com/a/232918):

> To me AOP is some kind of "meta-programming".

2) Python使用metaclass来实现AOP



## TODO: OOP VS GP

> TODO 重点比较OOP和duck type







