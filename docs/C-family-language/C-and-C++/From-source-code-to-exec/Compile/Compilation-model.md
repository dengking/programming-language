# Separate compilation model

我是在查找compiler如何compile template的时候，在isocpp的Why can’t I separate the definition of my templates class from its declaration and put it inside a .cpp file? [¶](https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl) [Δ](https://isocpp.org/wiki/faq/templates#)中提及了“Separate compilation model”，遂特地Google了一下，结合之前的思考，发现，c和c++中的很多问题都与此有关：

- 在`Theory/Organization`章节中，其实已经探讨了与此相关的问题了，显然，compiler的编译是基于programming language的organization的。

C和`C++`都是采用的Separate compilation model，其实所谓的“Separate compilation model”，简而言之是：每个source file就是一个translation unit，compiler逐个编译这些translation unit得到object file，然后link 这些object file，最终得到目标。

下面是关于此的一些比较好的文章。

## informit [Separate Compilation and Namespaces in C++](https://www.informit.com/articles/article.aspx?p=26039)





## 维基百科[Single Compilation Unit](https://en.wikipedia.org/wiki/Single_Compilation_Unit)