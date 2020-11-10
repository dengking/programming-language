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



## wikimili [Functional programming](https://wikimili.com/en/Functional_programming)



## Higher-order Function

## utexas [CS 378, Symbolic Programming#Functional Programming](https://www.cs.utexas.edu/~novak/cs378.pdf)

A functional program is one with no side effects:

- changing a global variable
- updating a database
- printing

If we call `sin(x)`, it will just return a value, but will have no side effects.

Functional programming does everything by ***composition*** of functions:

```C++
guacamole:
season(mash(slice(peel(wash(avocado)))))
```

Functions are composed so that the output of one function is the input of the next function(pipelined). 

## Functional programming in distributed computing

utexas [CS 378, Symbolic Programming#Functional Programming](https://www.cs.utexas.edu/~novak/cs378.pdf): 

Functional programming works well with distributed cloud computing: the function can be replicated on many servers and executed in parallel on massive amounts of data.



### Promise and future

Promise and future起源自functional programming，关于Promise and future，参见工程Parallel-computing的`Programming-model\Promise-future`章节。

### Map (parallel pattern)

https://infogalactic.com/info/Map_(parallel_pattern)

## Pattern

functional programming中的一种非常常见的pattern是在工程discrete的`Relation-structure-computation\Computation\Repetition`章节中提出的对structure顺序执行某个computation。

关于functional programming，参见:

1) cornell [Higher-order Programming](https://www.cs.cornell.edu/courses/cs3110/2017fa/l/06-hop/notes.html)

2) https://softwarefoundations.cis.upenn.edu/lf-current/Poly.html

3) Python Functional Programming HOWTO[¶](https://docs.python.org/3/howto/functional.html#functional-programming-howto)

4) wikimili [Fold (higher-order function)](https://wikimili.com/en/Fold_(higher-order_function))



下面是一些非常常见的模式，它们都是基于one-by-one computation model。

### apply/map

对一个data structure中的每个元素都执行同一个函数

https://infogalactic.com/info/Map_(higher-order_function)

### fold/reduce



参见 C++ fold

https://en.wikipedia.org/wiki/Fold_(higher-order_function)

https://wikimili.com/en/Fold_(higher-order_function)

### filter

对一个sequence进行过滤

https://infogalactic.com/info/Filter_(higher-order_function)

### Convolution

https://infogalactic.com/info/Convolution_(computer_science)

