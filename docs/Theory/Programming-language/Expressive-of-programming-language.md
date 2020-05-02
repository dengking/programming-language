# Expressive of programming language

本文中的expressive并不是在文章[Language](https://dengking.github.io/Post/Language/Language/)中的expressive power概念，它所指的是programming language的表现力、描述能力，是否更加能够描述现实生活中或其他学科的事物、概念。

不同的programming language的expressive是不同的，直观感觉就是OOP比面向过程语言更加expressive，关于此，参见[Summary of OO](./Programming-paradigm/Object-oriented-programming/Summary-of-OO.md)（在后面的Programming-paradigm/Object-oriented-programming章节会对此进行描述）。

下面是我在阅读过程碰到的一些关于programming language的expressive的描述：

- 维基百科[Parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)中有这样的描述

> In [programming languages](https://en.wikipedia.org/wiki/Programming_language) and [type theory](https://en.wikipedia.org/wiki/Type_theory), **parametric polymorphism** is a way to make a language more expressive, while still maintaining full static [type-safety](https://en.wikipedia.org/wiki/Type-safety).

## To make the programming language more expressive

本质上来说，我们使用语言来进行描述，我们不断地往programming language中添加各种feature，或者说不断地创造新的programming language，最终目的都是让programming language更加地expressive，表达（描述）起来更加地convenient。

expressive意味着更加易用、易于理解。

### To make framework more expressive

比如同样是event-driven programming：

flask的描述方式

JavaScript的描述方式

tornado的描述方式

c++的描述方式

c的描述方式



## General-purpose programming language vs domain-specific programming language

domain-specific programming language显然在描述该domain的相关问题上更加地容易。

它们往往是建立与general-purpose programming language之上。



### Domain-specific languages

在不同的领域，有着不同的language，比如math expression、EBNF等，它们都是domain-specific language；作为programmer，我们往往需要使用general-purpose programming language或者domain specific language来描述domain-specific language，显然，我们使我们的program更加易于使用，我们需要使用domain的概念。

> NOTE: 这一节是我在阅读[More C++ Idioms/Expression-template](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Expression-template)时想到的，原文非常棒，非常能够说明本节的思想。

### User-friend api

所谓user-friend api，可以认为是它们更加domain-specific，更加地human-readable。

for human，其实就是user-friend的含义:

[keras](https://keras.io/) : 

> **User friendliness.** Keras is an API designed for human beings, not machines. It puts user experience front and center. Keras follows best practices for reducing cognitive load: it offers consistent & simple APIs, it minimizes the number of user actions required for common use cases, and it provides clear and actionable feedback upon user error.





## From expressive programming language to readable program

expressive programming language让我们能够编写更加readable、更加domain-specific的programming language。



## Thoughts

越expressive的语言不通你越觉得就像是在进行配置一样