# Common Lisp Object System

是在阅读wikipedia [Covariance and contravariance (computer science)](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science))时，其中关于CLOS的multiple dispatch的描述吸引了我的注意: 

> Another language feature that can help is *multiple dispatch*. One reason that binary methods are awkward to write is that in a call like `a.compareTo(b)`, selecting the correct implementation of `compareTo` really depends on the runtime type of both `a` and `b`, but in a conventional OO language only the runtime type of `a` is taken into account. In a language with [Common Lisp Object System](https://en.wikipedia.org/wiki/Common_Lisp_Object_System) (CLOS)-style [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch), the comparison method could be written as a generic function where both arguments are used for method selection.

multiple dispatch是一个非常有用的特性，值得注意。

## wikipedia [Common Lisp Object System](https://en.wikipedia.org/wiki/Common_Lisp_Object_System)

The **Common Lisp Object System (CLOS)** is the facility for [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) which is part of [ANSI](https://en.wikipedia.org/wiki/American_National_Standards_Institute) [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp). CLOS is a powerful [dynamic](https://en.wikipedia.org/wiki/Dynamic_programming_language) object system which differs radically from the OOP facilities found in more [static languages](https://en.wikipedia.org/wiki/Static_language) such as [C++](https://en.wikipedia.org/wiki/C%2B%2B) or [Java](https://en.wikipedia.org/wiki/Java_(programming_language)).

CLOS is a [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch) system. This means that [methods](https://en.wikipedia.org/wiki/Method_(computer_science)) can be specialized upon any or all of their required arguments. Most OO languages are single-dispatch, meaning that methods are only specialized on the first argument. 

### Features



CLOS is a [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch) system. This means that [methods](https://en.wikipedia.org/wiki/Method_(computer_science)) can be specialized upon any or all of their required arguments. Most OO languages are single-dispatch, meaning that methods are only specialized on the first argument. 