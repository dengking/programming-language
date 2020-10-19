# Name

## name in programming language

在阅读cppreference c++的[Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)时，其中描述了“name”的概念。

在阅读python 4. Execution model[¶](https://docs.python.org/3/reference/executionmodel.html#execution-model)的4.2. Naming and binding[¶](https://docs.python.org/3/reference/executionmodel.html#naming-and-binding)章节的时候，其中也描述了“name”的概念。

其实，“name”的字面意思非常好理解，它表示的是“名称”，显然name是用来标识一个事物，正如本章的导言中所述：

> 我们使用一种programming language编写program的过程可以看做是使用这门programming language所提供的各种construct（或者说是entity）进行定义、引用的过程，在进行定义的时候，我们会指定一个“name”，然后通过这个name来引用这个定义好的construct。

python是一门解释型语言，正如在，中所述：everything in python is an object，即在python中，我们定义的function、class都是由object来实现的，显然，在python中，我们通过name来refer to object。

c++是一门编译型语言，在cppreference c++的[Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)中，提出了entity的概念，其实，此处的entity就是c++programming language所提供的construct，我们在编写c++ program的过程，就是使用使用这些entity进行定义、引用的过程，通过name来refer to entity，这些entity是由compiler来进行翻译、转换的。c++中，通过declaration来引入entity。

## Thoughts

在各种programming language章，与name相关的问题是：name的scope，显然，scope可以看做是name的一个属性。



在python中，每个scope都可以认为有一个namespace。

scope是nesting的、namespace也是nesting的。

无论是c++还是python，都无法将function scope的variable导出到另外一个file unit中。这是一种什么规则呢？

python的`impot`只能够导入file scope的name，python的scope和namespace是一致的。

c++的namespace仅仅是为了解决name conflict。

c++中的namespace也是一个scope， scope resolution operator