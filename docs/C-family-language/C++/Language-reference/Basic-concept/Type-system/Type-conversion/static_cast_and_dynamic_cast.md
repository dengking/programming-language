# 前言

说起C++中的继承、多态、虚函数等概念，可能很多同学都有所了解，但是要说真正熟知的同学可能就不是很多了。最近在编程过程中了解到C++类型的层次转换（这就涉及到了多态和继承的相关概念），通常C语言中可以对内置类型进行**强制转换**，但是这样做不是很安全，在C++标准中，提供了关于类型层次转换中的两个关键字`static_cast`和`dynamic_cast`。

# 一、`static_cast`关键字（编译时类型检查）

用法：

```c++
static_cast < type-id > ( expression )
```

该运算符把`expression`转换为`type-id`类型，但没有运行时类型检查来保证转换的安全性，它主要有如下几种用法：

（1）用于基本数据类型之间的转换，如把`int`转换为`char`，把`int`转换成`enum`，但这种转换的**安全性**需要开发者自己保证（这可以理解为保证数据的精度，即程序员能不能保证自己想要的程序安全），如在把int转换为char时，如果char没有足够的比特位来存放int的值（int>127或int<-127时），那么static_cast所做的只是简单的截断，及简单地把int的低8位复制到char的8位中，并直接抛弃高位。

（2）把空指针转换成目标类型的空指针

（3）把任何类型的表达式类型转换成void类型

（4）用于类层次结构中父类和子类之间指针和引用的转换。

对于以上第（4）点，存在两种形式的转换，即上行转换（子类到父类）和下行转换（父类到子类）。对于static_cast，上行转换时安全的，而下行转换时不安全的，为什么呢？因为static_cast的转换时粗暴的，它仅根据类型转换语句中提供的信息（尖括号中的类型）来进行转换，这种转换方式对于上行转换，由于子类总是包含父类的所有数据成员和函数成员，因此从子类转换到父类的指针对象可以没有任何顾虑的访问其（指父类）的成员。而对于下行转换为什么不安全，是因为static_cast只是在编译时进行类型坚持，没有运行时的类型检查，具体原理在dynamic_cast中说明。

**二、dynamic_cast关键字（运行时类型检查）**

用法：同static_cast

dynamic_cast主要用于类层次结构中父类和子类之间指针和引用的转换，由于具有运行时类型检查，因此可以保证下行转换的安全性，何为安全性？即转换成功就返回转换后的正确类型指针，如果转换失败，则返回NULL，之所以说static_cast在下行转换时不安全，是因为即使转换失败，它也不返回NULL。

对于上行转换，dynamic_cast和static_cast是一样的。

对于下行转换，说到下行转换，有一点需要了解的是在C++中，一般是可以用父类指针指向一个子类对象，如parent* P1 = new Children(); 但这个指针只能访问父类定义的数据成员和函数，这是C++中的静态联翩，但一般不定义指向父类对象的子类类型指针，如Children* P1 = new parent；这种定义方法不符合生活习惯，在程序设计上也很麻烦。**这就解释了也说明了，在上行转换中，static_cast和dynamic_cast效果是一样的，而且都比较安全，因为向上转换的对象一般是指向子类对象的子类类型指针；而在下行转换中，由于可以定义就不同了指向子类对象的父类类型指针，同时static_cast只在编译时进行类型检查，而dynamic_cast是运行时类型检查，则需要视情况而定。**下面通过代码进行说明

**[cpp]** [view plain](https://blog.csdn.net/qq_26849233/article/details/62218385#) [copy](https://blog.csdn.net/qq_26849233/article/details/62218385#)

1. class Base  
2. {  
3. ​    virtual void fun(){}  
4. };  
5.   
6. class Derived:public Base  
7. {  
8. };  

由于需要进行向下转换，因此需要定义一个

父类类型的指针Base *P

，但是由于子类继承与父类，父类指针可以指向父类对象，也可以指向子类对象，这就是重点所在。如果

 P

指向的确实是子类对象，则dynamic_cast和static_cast都可以转换成功，如下所示：

**[cpp]** [view plain](https://blog.csdn.net/qq_26849233/article/details/62218385#) [copy](https://blog.csdn.net/qq_26849233/article/details/62218385#)

1. Base *P = new Derived();  
2. Derived *pd1 = static_cast<Derived *>(P);  
3. Derived *pd2 = dynamic_cast<Derived *>(P);  

以上转换都能成功。

但是，如果 **P** 指向的不是子类对象，而是父类对象，如下所示：

**[cpp]** [view plain](https://blog.csdn.net/qq_26849233/article/details/62218385#) [copy](https://blog.csdn.net/qq_26849233/article/details/62218385#)

1. Base *P = new Base;  
2. Derived *pd3 = static_cast<Derived *>(P);  
3. Derived *pd4 = dynamic_cast<Derived *>(P);  

在以上转换中，static_cast转换在编译时不会报错，也可以返回一个子类对象指针（假想），但是这样是不安全的，在运行时可能会有问题，因为子类中包含父类中没有的数据和函数成员，这里需要理解转换的字面意思，转换是什么？转换就是把对象从一种类型转换到另一种类型，如果这时用 pd3 去访问子类中有但父类中没有的成员，就会出现访问越界的错误，导致程序崩溃。而dynamic_cast由于具有运行时类型检查功能，它能检查P的类型，由于上述转换是不合理的，所以它返回NULL。

**三、总结**

C++中层次类型转换中无非两种：上行转换和下行转换

对于上行转换，static_cast和dynamic_cast效果一样，都安全；

对于下行转换：你必须确定要转换的数据确实是目标类型的数据，即需要注意要转换的父类类型指针是否真的指向子类对象，如果是，static_cast和dynamic_cast都能成功；如果不是static_cast能返回，但是不安全，可能会出现访问越界错误，而dynamic_cast在运行时类型检查过程中，判定该过程不能转换，返回NULL。