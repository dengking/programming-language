# 关于本章

1、当我们开始思考name lookup，其实是在思考compiler的行为。

2、compiler使用**tree结构**来表示program，这是一种hierarchy结构，所以compiler在进行name lookup的时候，也是沿着tree进行的，可以参考龙书的`chapter 3`。

3、name lookup是由compiler在compile-time完成的，由于C++语言的复杂性，name lookup过程也是非常复杂的，它涉及到了非常多的内容，对C++ name lookup过程有所了解，能够帮助我们理解C++语言中的很多问题；

## Compile error

很多compile error都可以追溯到name lookup，因此掌握name lookup是解决很多compile error的前提。

## Implementation of static polymorphism is based on the result of name lookup

本节标题的含义是：static polymorphism是基于name lookup的结果，关于static polymorphism，参见`C++\Guide\Implementation-of-polymorphism`。

下面对这个观点进行详细解释：

| static polymorphism | 实现章节                                               |      |
| ------------------- | ------------------------------------------------------ | ---- |
| template            | `C++\Language-reference\Template\Implementation`       |      |
| overload            | `C++\Language-reference\Functions\Overload-resolution` |      |

compiler首先执行name lookup，从而得到candidates，然后从candidates选择出 best one。

## Function name lookup

在name lookup中，一个非常重要的主题就是：对function的lookup。

Function name lookup还涉及到的内容有：

1、ADL

2、Elaborated type specifier

3、`using`

下面是涉及function name lookup的好文章: 

1、文章gotw [Namespaces and the Interface Principle](http://www.gotw.ca/publications/mill08.htm)中对此进行了描述







