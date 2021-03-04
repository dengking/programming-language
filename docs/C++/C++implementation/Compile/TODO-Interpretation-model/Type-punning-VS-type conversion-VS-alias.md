

[alias](https://en.wikipedia.org/wiki/Aliasing_(computing))描述的是在program中非常普遍的一种现象:

> In [computing](https://en.wikipedia.org/wiki/Computing), **aliasing** describes a situation in which a data location in [memory](https://en.wikipedia.org/wiki/Memory_(computers)) can be accessed through different symbolic names in the program. 

`c++`中的reference就是一种典型的alias，并且c中使用了指针，这就让programmer有了极大的灵活性来access，它可以使用不同的type来进行access，其实这就是[pointer punning](https://en.wikipedia.org/wiki/Aliasing_(computing))，关于[pointer punning](https://en.wikipedia.org/wiki/Aliasing_(computing))的pros and cons，参见它的介绍；

[type punning](https://en.wikipedia.org/wiki/Type_punning)则是希望使用一种新的type来解释重解释一个已经声明为另外一种类型的object，它不是[type conversion](https://en.cppreference.com/w/c/language/cast)（type conversion会读取原object，然后构造一个新的类型的object），type punning是就地进行重解释，或者说它是告诉compiler它：按照这种类型来进行解释，在c中，实现type punning可以使用不同的指针类型来进行alias（显然，这是危险的）即[pointer punning](https://en.wikipedia.org/wiki/Aliasing_(computing))，c标准中认可的方法是使用`union`，在[Objects and alignment](https://en.cppreference.com/w/c/language/object)中专门说明了这种用法：

> Note that type-punning may also be performed through the inactive member of a [union](https://en.cppreference.com/w/c/language/union).

在[Type punning](https://en.wikipedia.org/wiki/Type_punning)中有这样的一段描述：

> This aliasing problem can be fixed by the use of a `union` (though, this example still makes the assumption about IEEE-754 bit-representation of floating-point types).



本质上来说，[type punning](https://en.wikipedia.org/wiki/Type_punning)是对[alias](https://en.wikipedia.org/wiki/Aliasing_(computing))的应用；

一提及[alias](https://en.wikipedia.org/wiki/Aliasing_(computing))，就会涉及c中的[Strict aliasing](https://en.cppreference.com/w/c/language/object#Strict%20aliasing) ，后来我看了一下标志文档，发现在标志文档中并没有strict aliasing的内容，在[cppreference.com](https://en.cppreference.com/)的[Objects and alignment](https://en.cppreference.com/w/c/language/object)页面中的Strict aliasing章节的内容是取自标志文档的6.5 Expressions的`7`。

