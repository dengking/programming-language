# Functional programming

“functional programming"即”函数式编程“。

## 维基百科[Functional programming](https://en.wikipedia.org/wiki/Functional_programming)

> NOTE: 维基百科的这篇文章有些难以理解，我觉得理解Functional programming的关键有：
>
> Functional programming是一种[Declarative programming](https://en.wikipedia.org/wiki/Declarative_programming)
>
> 其次是：
>
> > treats [computation](https://en.wikipedia.org/wiki/Computation) as the evaluation of [mathematical functions](https://en.wikipedia.org/wiki/Function_(mathematics)) and avoids changing-[state](https://en.wikipedia.org/wiki/Program_state) and [mutable](https://en.wikipedia.org/wiki/Immutable_object) data
>
> 这说明在functional programming中，是没有我们熟知的那些[statement](https://en.wikipedia.org/wiki/Statement_(computer_science))，比如 [assignment](https://en.wikipedia.org/wiki/Assignment_(computer_science))，因为 [assignment](https://en.wikipedia.org/wiki/Assignment_(computer_science))可能会change state，所以在functional programming中，仅仅只有expression。
>
> 需要结合具体例子来进行理解也是一种捷径，在原文的[Coding styles](https://en.wikipedia.org/wiki/Functional_programming#Coding_styles)中给出的例子就非常易懂。
>
> 按照，functional programming的定义，使用[higher-order function](https://en.wikipedia.org/wiki/Higher-order_function)不一定是functional programming。
>
> 在维基百科[Template metaprogramming#Components of template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming#Components_of_template_metaprogramming)中提及：
>
> Template metaprograms have no [mutable variables](https://en.wikipedia.org/wiki/Immutable_object)— that is, no variable can change value once it has been initialized, therefore template metaprogramming can be seen as a form of [functional programming](https://en.wikipedia.org/wiki/Functional_programming). In fact many template implementations implement flow control only through [recursion](https://en.wikipedia.org/wiki/Recursion_(computer_science)), as seen in the example below.