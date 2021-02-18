

## 循序渐进地实现

Programming language的设计者在设计实现某种paradigm的时候，需要考虑的问题是非常之多的，工作量也是非常之大的，所以它可能会将对某种paradigm的实现分为多个版本来进行实现，在某个版本中提供初级的实现，然后在后续版本中提供更多的、高级的实现。一个典型的例子就是C++对generic programming的实现，generic programming的核心思想中明确地提出了concept，但是c++知道c++20版本才提供显式定义concept的实现，在此之前的版本，用户只能够通过`SFINAE`、`enable_if`来间接地描述concept。关于这一点是我在阅读[Generic Programming in ConceptC++](http://www.generic-programming.org/languages/conceptcpp.html)时，受到的启发

> ConceptC++ is an extension to the C++ Programming Language that introduces first-class support for Generic Programming. It is the first attempt to introduce **complete** support for Generic Programming into a mainstream programming language. 



## 博采众长

另外非常重要的一点是：每种paradigm，有着各自的优势，能够解决一些问题，所以现代programming language往往会采用多种paradigm，即“博采众长”，显然，这样的做法，让programming language更加地强大，让programmer通过灵活运用、结合多种paradigm从而充分发挥programming language的powered。关于这个思想，在下面文章中有提及:

1) stackoverflow [Aspect Oriented Programming vs. Object-Oriented Programming](https://stackoverflow.com/questions/232884/aspect-oriented-programming-vs-object-oriented-programming) # [A](https://stackoverflow.com/a/232918)

> Why "vs"? It is not "vs". You can use Aspect Oriented programming in combination with functional programming, but also in combination with Object Oriented one.

当然，也不可避免的会带来programming language的复杂度的提高，一个典型的例子就是C++，它的两个典型的paradigm是OOP和generic programming。









## Draft: Domain-specific programming paradigm

本节标题的含义是: 特定领域的programming paradigm。

本章所讲述的programming paradigm是software engineering中通用的programming paradigm，与此相对立的是domain-specific programming paradigm，它是指在特定领域，为了更加高效地解决特定问题，而设计的programming paradigm，这其实非常类似于**domain-specific language**，下面是一些例子: 

|                                                              | first class citizen |
| ------------------------------------------------------------ | ------------------- |
| OOP                                                          | object              |
| [process-oriented programming](https://en.wikipedia.org/wiki/Process-oriented_programming), | process             |
| TensorFlow tensor-oriented programming                       | Tensor              |
| numpy ndarray-oriented programming                           | ndarray             |

在下面文章中，描述了**domain-specific language**:

1) `C++\Idiom\TMP\Expression-Template`

