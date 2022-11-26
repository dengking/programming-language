# [Tour of Scala](https://docs.scala-lang.org/tour/tour-of-scala.html)



## Scala is object-oriented

Scala is a pure object-oriented language in the sense that [every value is an object](https://docs.scala-lang.org/tour/unified-types.html). Types and behaviors of objects are described by [classes](https://docs.scala-lang.org/tour/classes.html) and [traits](https://docs.scala-lang.org/tour/traits.html). Classes can be extended by subclassing, and by using a flexible [mixin-based composition](https://docs.scala-lang.org/tour/mixin-class-composition.html) mechanism as a clean replacement for multiple inheritance.

> NOTE:
>
> 一、非常好的说明:
>
> 1、interface is description of behavior
>
> 2、composition over inheritance



## Scala is functional

Scala is also a functional language in the sense that [every function is a value](https://docs.scala-lang.org/tour/unified-types.html). 

> NOTE:
>
> 一、pure OOP+first class function

Scala provides a [lightweight syntax](https://docs.scala-lang.org/tour/basics.html#functions) for defining anonymous functions, it supports [higher-order functions](https://docs.scala-lang.org/tour/higher-order-functions.html), it allows functions to be [nested](https://docs.scala-lang.org/tour/nested-functions.html), and it supports [currying](https://docs.scala-lang.org/tour/multiple-parameter-lists.html). 

> NOTE:
>
> 一、上面这一段主要是在讲述functional programming

Scala’s [case classes](https://docs.scala-lang.org/tour/case-classes.html) and its built-in support for [pattern matching](https://docs.scala-lang.org/tour/pattern-matching.html) provide the functionality of algebraic types, which are used in many functional languages. 

> NOTE:
>
> 一、algebraic data type pattern match 是 FP的标配

[Singleton objects](https://docs.scala-lang.org/tour/singleton-objects.html) provide a convenient way to group functions that aren’t members of a class.

> NOTE:
>
> 一、pure OOP

Furthermore, Scala’s notion of pattern matching naturally extends to the [processing of XML data](https://github.com/scala/scala-xml/wiki/XML-Processing) with the help of [right-ignoring sequence patterns](https://docs.scala-lang.org/tour/regular-expression-patterns.html), by way of general extension via [extractor objects](https://docs.scala-lang.org/tour/extractor-objects.html). In this context, [for comprehensions](https://docs.scala-lang.org/tour/for-comprehensions.html) are useful for formulating queries. These features make Scala ideal for developing applications like web services.



## [Basics](https://docs.scala-lang.org/tour/basics.html)