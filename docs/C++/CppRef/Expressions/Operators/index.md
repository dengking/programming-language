# 关于本章

本章根据cppreference [Expressions](https://en.cppreference.com/w/cpp/language/expressions)的Operators章节对c++语言中的operator进行分类组织。



## Operator is a kind of function

本节标题的含义是：operator是一种function。在C++中，存在着非常丰富的operator，我觉得可以将它们看做是function，这是因为operator本质和function非常类似，并且C++的operator overload是以member method的形式实现的；

这样能够使我们以统一的方式来进行描述，典型的例子有:

| example                                                      | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `C++\Language-reference\Basic-concept\Type-system\Type-conversion\Implicit-conversion\index.md` | operator涉及的conversion和function涉及的conversion是类似的   |
| `C++\Language-reference\Expressions\Value-categories\Value-categories\Value-categories.md` | operator expression的value category规则和function call expression的value category规则是非常类似的 |
| `C++\Language-reference\Expressions\Operators\Operator-overloading` | C++的operator overload是以member method的形式实现的          |

