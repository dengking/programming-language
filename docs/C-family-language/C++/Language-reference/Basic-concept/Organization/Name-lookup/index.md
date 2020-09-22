# 关于本章

当我们开始思考name lookup，其实是在思考compiler的行为。

compiler使用**tree结构**来表示program，这是一种hierarchy结构，所以compiler在进行name lookup的时候，也是沿着tree进行的，可以参考龙书的chapter 3。

name lookup是由compiler在compile-time完成的，由于C++语言的复杂性，name lookup过程也是非常复杂的，它涉及到了非常多的内容，对C++ name lookup过程有所了解，能够帮助我们理解C++语言中的很多问题；在name lookup中，一个非常重要的主题就是：对function的lookup。

Name lookup还涉及到的内容有：

- ADL
- Elaborated type specifier
- `using`

下面是涉及name lookup的好文章: 

- 文章[Namespaces and the Interface Principle](http://www.gotw.ca/publications/mill08.htm)中对此进行了描述







