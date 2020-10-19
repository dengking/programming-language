# Organization

> 以自顶向下的方式来进行描述，构建完整的视图。
>
> 每个scope都有一个namespace，scope之间是nesting关系，这样namespace之间也是nesting关系。
>
> 形成一颗完整的tree。
>
> 以hierarchy的方式来进行组织: 
>
> 整体project，由多个file unit组成。
>
> 

对于大型的project，name非常之多，所以这就要求programming language的提供一定机制来对这些name进行organize，目前大多数programming language所采用的方式是：**namespace**。

除了对name进行组织，还需要对program本身进行分解组织，不可能将所有的program写在一个文件中，而是将其分解为多个file unit（文件单元），这些file unit共同构成了整个项目，我们将此称为**file organization**。在c和c++中，显然，file unit就是source file，在c和c++中，它们的另外一个名称是[translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming))。python的file unit是`.py`文件。

总的来说，目前大多数programming language的organization都是基于namespace和file organization的，下面对c++、python的进行了总结：

python的file organization：module-》package。

`c++`采用的file organization非常简单：[translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming))，

Java采用的file organization是`.class`文件-》package。

`c++`、python都有namespace的概念，（java不清楚）。

对比这三种语言可以发现：

python(Java不确定是否如此)的namespace和file organization是一致的，因为在python中，一个module就是一个namespace，也就是说python的organization是通过file organization来实现的，所以在python中，我们无需显式地定义namespace，因为module就是一个namespace；

而`c++`不同，它的namespace是用户显示地使用namespace来进行定义，这就导致的一个问题是：它的file organization和它的namespace是不一致的，所以在使用c++的时候，我们需要思考这样的问题：c++的file organization和它的namespace之间的关系，更加具体地说是：能否在一个source file中定义多个namespace，多个source file能否共用同一个namespace。

显然，c++的这种设计，导致了它需要使用`#include`、`using`，而python仅仅使用`import`。

c++的file organization和它的namespace是不一致的，而python的file organization和它的namespace是一致的。并且由于`c++`中有`.h`文件，`.cpp`文件，而python和Java进行了简化，仅仅只有一种类型的文件，所以从这个角度来看，python和Java的复杂度要比`c++`低一些。



## `class`也是一种namespace

目前大多数programming language，都将class也作为一个namespace，python和c++都采用了这种做法。



## Structure of namespace

从整体来看，整个project的namespace是tree structure，即树形结构，或者说是nesting 结构。



## See also

维基百科 [Translation unit (programming)](https://en.wikipedia.org/wiki/Translation_unit_(programming))

维基百科 [Namespace](https://en.wikipedia.org/wiki/Namespace)





