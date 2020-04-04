# State pattern

## CSDN [Java设计模式——状态模式（STATE PATTERN）](https://blog.csdn.net/u012401711/article/details/52675873)

我是通过阅读CSDN的[Java设计模式——状态模式（STATE PATTERN）](https://blog.csdn.net/u012401711/article/details/52675873)而了解state pattern的，state pattern让我想到了[state machine](https://en.wikipedia.org/wiki/Finite-state_machine)（在工程[automata-and-formal-language](https://dengking.github.io/automata-and-formal-language)中有对它的描述），我的感觉它是使用面向对象语言来描述state machine（另外一种描述方式是graph），后来阅读维基百科[State pattern](https://en.wikipedia.org/wiki/State_pattern)，其中也有这样的描述，由此可见，state machine 模型是可以刻画很多事物的。

### 可以使用state pattern解决的问题

其实，如果所需要解决的问题能够使用state machine来进行描述，那么，这些问题都是可以使用state pattern来进行解决的，我们结合这篇文章中所列举的电梯的这个例子来看：有多种状态、状态之间的转换、不同的状态下，运行运行的操作是不同的。

### 实现方式

使用`class`来显示地描述state，使用context来记录系统当前的state。

那如何来实现state之间的转换呢？其实系统的状态是由用户的操作而触发来进行转换的，状态之间的转换就相当于连接它们之间的边，实现方式是每个状态都定义各种用户允许的操作，记录各种状态在各种操作的转换。

## 维基百科[State pattern](https://en.wikipedia.org/wiki/State_pattern)

