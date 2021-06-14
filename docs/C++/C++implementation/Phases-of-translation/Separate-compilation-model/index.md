# Separate compilation model

我是在查找compiler如何compile template的时候，在isocpp的"Why can’t I separate the definition of my templates class from its declaration and put it inside a .cpp file? [¶](https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl) [Δ](https://isocpp.org/wiki/faq/templates#)"中提及了“Separate compilation model”，遂特地Google了一下，结合之前的思考，发现，c和c++中的很多问题都与此有关：

1、在`Theory/Organization`章节中，其实已经探讨了与此相关的问题了，显然，compiler的编译是基于programming language的organization的。

C和`C++`都是采用的Separate compilation model，其实所谓的“Separate compilation model”，简而言之是：

> 每个source file就是一个translation unit，compiler逐个编译这些translation unit得到object file，然后link 这些object file，最终得到目标。



下面是关于此的一些比较好的文章。

## informit [Separate Compilation and Namespaces in C++](https://www.informit.com/articles/article.aspx?p=26039)



## cs.sjsu.edu [Separate Compilation](http://www.cs.sjsu.edu/~pearce/modules/lectures/cpp/advanced/SeparateCompilation.htm)

![img](http://www.cs.sjsu.edu/~pearce/modules/lectures/cpp/advanced/SeparateCompilation_files/image004.gif)



## Translation unit

translation unit说明编译的单位，一般是source file。

### wikipedia [Translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming))



### TODO wikipedia [Single Compilation Unit](https://en.wikipedia.org/wiki/Single_Compilation_Unit)





## C family language中基于translation unit的一些内容

最终的target file是由各个translation unit的object file组成的，C family language中有很多内容是基于translation unit而建立的，其实理解这些内容的一个突破口是：根据range来进行划分，在同一个translation unit之内还是不同translation unit之间，如下是对这些内容的总结: 

### Initialization order of no-local object with static storage duration

参见`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Static-storage-duration`

### Linkage

Linkage显然是建立在translation unit之间的一个概念，参见`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Static-storage-duration`

Linkage: 允许不同的translation unit

顺序的无法确定: 对于no-local object with static storage duration的initialization 的无法确定

允许link: linkage



## TODO 

https://stackoverflow.com/questions/3749099/why-should-the-implementation-and-the-declaration-of-a-template-class-be-in-the

https://www.cs.bu.edu/teaching/c/separate-compilation/



