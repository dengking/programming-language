# Python garbage collection



素材:

1、zhihu [C++内存管理：shared_ptr/weak_ptr源码（长文预警）](https://zhuanlan.zhihu.com/p/532215950)

> 首先c++不会像python一样提供“部分标记-清除算法”来检测并破除循环引用，这对性能的影响太大。

2、wikipedia [Reference counting # Python](https://en.wikipedia.org/wiki/Reference_counting#Python) 

> [Python](https://en.wikipedia.org/wiki/Python_(programming_language)) also uses reference counting and offers cycle detection as well (and can reclaim them).[[21\]](https://en.wikipedia.org/wiki/Reference_counting#cite_note-21)

3、docs.python [**Extending and Embedding the Python Interpreter**](https://docs.python.org/2.4/ext/refcounts.html)



## thoughs

Python 和 Java 都是带GC的语言，能够返回在函数中声明的临时变量，准确来说，它不是临时变量，它应该是？？我觉得非常类似于pointer，即它指向的是一个位于heap中的内存区域。而对于`c++`和`c`而言，这是不可能的。

python name and pointer，我觉得python中identifier非常类似于指针。python中的`.`，有些类似于`c`或者`c++`中的`->`。

在运行一个python函数的时候，如果在其中创建了一个对象，则这个对象应该是`new`出来的， 即它是位于heap上。

java应该也是如此。

python的GC应该作用于heap。