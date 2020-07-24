# 关于本章

当我们开始思考name lookup，其实是在思考compiler的行为。



compiler使用**tree结构**来表示program，这是一种hierarchy结构，所以compiler在进行name lookup的时候，也是验证tree进行的，可以参考龙书的chapter 3。



c++ name lookup，它是复杂的，是compile-time的，下面描述了function lookup，它还涉及到的内容有：ADL、overload resolution、member access、Elaborated type specifier、using等，文章[Namespaces and the Interface Principle](http://www.gotw.ca/publications/mill08.htm)中对此进行了描述；需要将所有涉及的内容，放到一起；