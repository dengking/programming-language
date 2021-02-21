# Template is behavior-based

本文所要表达的思想是: 在使用template的时候，应该谨记: template is behavior-based。

关于behavior-based，参见:

1) `Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation`

2) `Theory\Programming-paradigm\Generic-programming\Type-requirement`

## drdobbs-Templates-and-Duck-Typing

参见`drdobbs-Templates-and-Duck-Typing`章节。

## wikipedia [Generic programming](https://en.wikipedia.org/wiki/Generic_programming) # [Templates in C++](https://en.wikipedia.org/wiki/Generic_programming#Templates_in_C++)

This works whether the arguments `x` and `y` are integers, strings, or any other type for which the expression `x < y` is sensible, or more specifically, for any type for which operator`<` is defined. Common inheritance is not needed for the set of types that can be used, and so it is very similar to [duck typing](https://en.wikipedia.org/wiki/Duck_typing#Templates_or_generic_types). 



## wikipedia [Duck typing](https://en.wikipedia.org/wiki/Duck_typing) # [Templates or generic types](https://en.wikipedia.org/wiki/Duck_typing#Templates_or_generic_types) 

[Template](https://en.wikipedia.org/wiki/Template_metaprogramming), or [generic](https://en.wikipedia.org/wiki/Generic_programming) functions or methods apply the **duck test** in a [static typing](https://en.wikipedia.org/wiki/Type_system#Static_typing) context; this brings all the advantages and disadvantages of [static versus dynamic type checking](https://en.wikipedia.org/wiki/Type_system#Static_and_dynamic_type_checking_in_practice) in general. **Duck typing** can also be more flexible in that only the methods *actually called at runtime* need to be implemented, while templates require implementations of all methods that *can not be proven [unreachable](https://en.wikipedia.org/wiki/Unreachable_code) at compile time*.  

> NOTE: 上面这段话从static versus dynamic type的角度来进行比较

Languages like Python, Java and Objective-C are examples of duck typing because it is possible in them to construct new types in runtime via [reflection](https://en.wikipedia.org/wiki/Reflection_(computer_programming)) and inspect whether these objects implement certain methods. On the other hand, there are languages that rely on **compile-time metaprogramming** techniques (like C++ and its template system) and thus do not fit into the category of duck typing; instead, at some point in the compilation pipeline, all placeholder types become substituted with some concrete types specified in a particular instantiation. Even though certain type erasure is possible in them, runtime inspection is limited.



## cppreference Named requirement

参见 `./Named-requirements`，其中有关于这个话题的讨论。