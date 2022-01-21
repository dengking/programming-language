# Plain old data



## wikipedia [Passive data structure](https://en.wikipedia.org/wiki/Passive_data_structure) 

In [computer science](https://en.wikipedia.org/wiki/Computer_science) and [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), a **passive data structure** (**PDS**, also termed a **plain old data structure**, or **plain old data**, **POD**), is a term for a [record](https://en.wikipedia.org/wiki/Record_(computer_science)), to contrast with objects. It is a [data structure](https://en.wikipedia.org/wiki/Data_structure) that is represented only as passive collections of [field](https://en.wikipedia.org/wiki/Field_(computer_science)) values ([instance variables](https://en.wikipedia.org/wiki/Instance_variable)), without using object-oriented features.

## Example

CSDN [C++ trivial和non-trivial构造函数及POD类型](https://blog.csdn.net/a627088424/article/details/48595525)

> 那个POD意思是Plain Old Data，也就是C++的内建类型或传统的C结构体类型。POD类型必然有trivial ctor/dtor/copy/assignment四种函数。

## 素材

在看linusakesson [The TTY demystified](http://www.linusakesson.net/programming/tty/index.php) 的时候，其中的一段话提及了passive object，非常有启示作用：

>An operating system process is "alive" (has an execution context), which means that it can perform actions. The TTY driver is not alive; in object oriented terminology, the TTY driver is a passive object. It has some data fields and some methods, but the only way it can actually do something is when one of its methods gets called from the context of a process or a kernel interrupt handler. The line discipline is likewise a passive entity.

