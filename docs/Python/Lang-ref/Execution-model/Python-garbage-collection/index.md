# Python garbage collection







## thoughs

Python 和 Java 都是带GC的语言，能够返回在函数中声明的临时变量，准确来说，它不是临时变量，它应该是？？我觉得非常类似于pointer，即它指向的是一个位于heap中的内存区域。而对于`c++`和`c`而言，这是不可能的。

python name and pointer，我觉得python中identifier非常类似于指针。python中的`.`，有些类似于`c`或者`c++`中的`->`。

在运行一个python函数的时候，如果在其中创建了一个对象，则这个对象应该是`new`出来的， 即它是位于heap上。

java应该也是如此。

python的GC应该作用于heap。