# Method resolution order

1、需要按照在 `Class-relation-hierarchy-diagram` 章节中总结的"Ordering Structuralization"思路来进行理解。class hierarchy是典型的non-linear structure，method resolution order是对它的traverse，显然method resolution order是一个linear structure

2、在使用method resolution order来实现dynamic polymorphism的时候，往往是自底向上的: 即从most derived class开始，逐级向上进行search，显然这是符合polymorphism的要求的。

3、一般，在支持multiple inheritance的时候，是需要使用 method resolution order

## Python method resolution order

realpython [C3 Superclass Linearization (Optional)](https://realpython.com/lessons/c3-superclass-linearization-optional/)

> [In the last video, you learned that Python uses the method resolution order, or *MRO*,](https://realpython.com/lessons/c3-superclass-linearization-optional/#t=0.54) [to determine what order to search parent classes for the desired method. In](https://realpython.com/lessons/c3-superclass-linearization-optional/#t=7.5600000000000005) [Python 3,](https://realpython.com/lessons/c3-superclass-linearization-optional/#t=13.08) [the algorithm that generates the MRO is called the *C3 superclass linearization algorithm*,](https://realpython.com/lessons/c3-superclass-linearization-optional/#t=14.13) [or *C3* for short.](https://realpython.com/lessons/c3-superclass-linearization-optional/#t=21.0)





## draft

overload resolution其实也是一种method resolution order

C++ virtual function的调用其实也是一种method resolution order

