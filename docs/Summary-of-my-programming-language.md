# Summary of my programming language

截止目前为止，我已经接触了多种programming language，现在非常有必要对这些programming language的同与异进行总结了；

主要以`python`，`c`，`c++`来作为研究对象。

对它们的对比，需要从多个角度来进行展开，主要以《Compilers Principles Techniques and Tools 2nd.pdf》为蓝本，然后加上一些我之前在computer science中所总结的内容；



## compiler and interpreter  

参考内容：

- 《Compilers Principles Techniques and Tools 2nd.pdf》chapter 1.1 Language Processors

首先需要准确地理解compiler和interpreter；

Simply stated, a compiler is a program that can read a program in one language , *the source language* and **translate** it into an equivalent program in another language , *the target language*;

```

source program
      |
     \|/
  {Compiler}
      |
     \|/
target program
```

`c`和`c++`中，compiler的target language是executable machine-language program；

python中，compiler的target language是[bytecode](https://en.wikipedia.org/wiki/Bytecode).（参见[Is Python interpreted or compiled? Yes.](https://nedbatchelder.com/blog/201803/is_python_interpreted_or_compiled_yes.html)）

显然，`c`和`c++`结果编译达到的program是可以直接执行的，而python结果编译生成的program则需要python interpreter的解释执行；





## Programming Language Basics

### declaration，definition，statement，expression

declaration和definition是两个非常接近且容易混淆的概念，在此之前先必须把这两个搞清楚；

在`c`和`c++`中，有头文件（header file `.h`）和源代码文件（source code file `.c`和`.cpp`）文件，在这两门语言中同时包含declaration和definition，并且区分它们；而python则只有`.py`文件，并且[python lacks declaration](https://docs.python.org/3/reference/executionmodel.html#execution-model)，即python中只有definition；

在`compiler/declaration，definition，statement，expression`中对这个问题进行了专门的分析；





## resource management

此处的resource为：network connection，file

`c++`中使用RAII，python中使用`with`，而`c`中，则没有特别的机制，所有的一切都需要programmer显示地调用与执行，c中经常使用`goto`。

对资源的管理设计到一对相反的操作：

- new and delete
- lock and unlock

不同的语言有不同的支持方式：

- C++ RAII
- python with

在工程[Linux-OS](https://dengking.github.io/Linux-OS/)的[Linux-OS's-multitasking](https://dengking.github.io/Linux-OS/Kernel/Guide/Linux-OS's-multitasking/00-Multitask/)中的process-resource章节对此进行了描述。



思考：可以看到，无论python还是c++，resource management方案都是涉及一对相反的操作，c++的resource management方案使用的基于scope的，而python的scope非常简单，所以它使用的是magic function的方式。

## magic function的使用

c中压根就没有magic function，c compiler在编译source code的时候，无需为其生成或者选择指定的函数；

`c++`中使用了magic function，比如它支持各种constructor，其实constructor就是最最典型的magic function；`c++` compiler会在翻译代码的过程中，根据需求为在编译生成的代码加入constructor，比如`MyClass a`，编译器在编译过程中，就会加入constructor；显然，`c++`中，这些magic function的选择是static的，是在compile time的；并且c++区分copy语义和move语义，编译器能够根据source code选择正确的constructor；



|||
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [DefaultConstructible](https://en.cppreference.com/w/cpp/named_req/DefaultConstructible) | specifies that an object of the type can be default constructed  (named requirement) |
| [MoveConstructible](https://en.cppreference.com/w/cpp/named_req/MoveConstructible)(C++11) | specifies that an object of the type can be constructed from rvalue  (named requirement) |
| [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) | specifies that an object of the type can be constructed from lvalue  (named requirement) |
| [MoveAssignable](https://en.cppreference.com/w/cpp/named_req/MoveAssignable)(C++11) | specifies that an object of the type can be assigned from rvalue  (named requirement) |
| [CopyAssignable](https://en.cppreference.com/w/cpp/named_req/CopyAssignable) | specifies that an object of the type can be assigned from lvalue  (named requirement) |
| [Destructible](https://en.cppreference.com/w/cpp/named_req/Destructible) | specifies that an object of the type can be destroyed  (named requirement) |

python中则存在着大量的magic function，比如为了支持iteration，支持index等；python使用的是name lookup，对magic function的选择也是使用的name lookup，并且python的name lookup是发生在runtime，所以python的magic function是发生在runtime的；



`c++`比较复杂，并且它的类型限制非常严格，并且它的类型限制都是通过compiler来进行的；它的standard library对于操作的**类型**会提出[named requirement](https://en.cppreference.com/w/cpp/named_req)，如果对应的类型不满足standard library所要求的[named requirement](https://en.cppreference.com/w/cpp/named_req)的话，则compiler会报错；如下是一些典型的[named requirement](https://en.cppreference.com/w/cpp/named_req)，其实如果programmer在进行program的时候，如果要想让自己定义的type满足对应的requirement的话，就需要提供对应的function；

***SUMMARY*** : 关于`c++`的[named requirement](https://en.cppreference.com/w/cpp/named_req)，通过这篇文章就可以进行理解：[C++ vector emplace_back calls copy constructor](https://stackoverflow.com/questions/40457302/c-vector-emplace-back-calls-copy-constructor)

python和`c++`不同，它也是强类型的，但是python是duck type，python的类型限制不是通过它的compiler来进行的，它的**类型限制**是通过runtime的name lookup，主要对象定义了它所期望的magic function，则它就认为这个对象满足了自己的**类型限制**；

和python与`c++`相比，`c`则没有如此都的限制，`c`语言中，压根就没有magic method；



## type system

### constant

`c++`和`c`中使用[const (computer programming)](https://en.wikipedia.org/wiki/Const_(computer_programming)) 来作为[Type qualifier](https://en.wikipedia.org/wiki/Type_qualifier) 

python中采取的是完全不同的方式，python中没有[Type qualifier](https://en.wikipedia.org/wiki/Type_qualifier)，python中，type就决定了数据的const，并且在python中，压根就不叫const，python中叫做*mutable*，在python的Data model[¶](https://docs.python.org/3/reference/datamodel.html#data-model)中对这个问题进行了描述；An object’s mutability is determined by its type;numbers, strings and tuples are immutable, while dictionaries and lists are mutable.



***THINKING*** : 

- 在c中，何时必须要使用const？如通过`const char *`来引用一个`c string`





### [Explicit or implicit declaration and inference](https://en.wikipedia.org/wiki/Type_system#Explicit_or_implicit_declaration_and_inference)

关于这一点，`c`，`c++`和python采用了完全相反的方式；

C and `C++`, require *type declarations*: the programmer must explicitly associate each variable with a specific type. 

而python则无需type declaration；



### [Type safety](https://en.wikipedia.org/wiki/Type_system#Type_safety_and_memory_safety)

c语言即非type safe也非memory safe；



### [Type checking](https://en.wikipedia.org/wiki/Type_system#Type_checking)

`c`和`c++`所采用的是static check，而python所采用的是dynamic type；





## scope

这三种programming language采用的都是static scope，都是基于block scope





## python display vs `c`, `c++` Initialization

[`c` Initialization](https://en.cppreference.com/w/c/language/initialization) , [`c++` Initialization](https://en.cppreference.com/w/cpp/language/initialization) 实现对variable的initialize；

python lack declaration，但是python中也提供了类似的语法，python这种叫做[display](https://docs.python.org/3/reference/expressions.html#displays-for-lists-sets-and-dictionaries)



## exception

`c++`和python都提供了exception机制；由于python是由python interpreter解释执行，而`c++`程序是由OS直接运行，因此两者的exception和OS之间的关联是不同的；python中的exception会导致python interpreter的执行路径改变，基本上不会涉及到OS；而`c++`中的exception，则会转换为OS的signal，最终可能导致程序终止运行；



c语言没有；

## python and c++

```
pMeanRate = tanhx(pMeanRate, 2);
```
这段代码在`c++`中是非常好理解的：`pMeanRate`是一个左值，所以它既能够被read，也能够被write；但是在python中，`=`所表示的是name bind；

