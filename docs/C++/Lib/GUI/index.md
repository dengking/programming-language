# C++GUI

## zhihu [C++ 是否适合做 GUI？](https://www.zhihu.com/question/30608733)

### [蔡磊的回答](https://www.zhihu.com/question/24375005/answer/27656927) 

因为C++本身不具备反射和自省，类体系结构非常的静态，且抽象能力有限。但GUI是个相当动态的东西，所以最“基础”的C++并不适合写GUI，硬写的话最后的程序会非常的verbose。

聪明的GUI库通常会用一些技巧绕过这个问题：

\1. 大量宏技巧来保存meta信息。如MFC、wxWigdet。

\2. 用额外的source transformer来提供反射和动态messaging能力，如Qt。

\3. 在C++基础上实现一个Domain Specific Language。包括Qt，以及各种DirectUI方案。

严格意义上讲，后两种手段是统一的，都属于DSL方法，所以已经不能算是纯粹的C++。

> NOTE: 
>
> 更加准确的来说，是都需要auxiliary compiler

