# explicit and implict 隐式和显式 H1

## [explicit specifier](https://en.cppreference.com/w/cpp/language/explicit) H2

其实，这里提到了explicit，那么就必须要搞清楚c++中的implicit



## [Implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion) H2

[What are move semantics?](http://note.youdao.com/noteshare?id=ec11cf641add562011d335b955381d09&sub=B1A8901E852F4AF88D7C81ED5C457A03)一文中的**Implicit conversions**章节的内容提醒了我一个非常重要的知识：在进行类型转换的时候，执行过程不是在原来的变量上进行修改，而是重新创建一个临时变量。