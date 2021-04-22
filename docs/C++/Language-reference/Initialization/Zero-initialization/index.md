# Zero initialization



## cppreference [Zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization)

Sets the initial value of an object to zero.

> NOTE: 
>
> 1、描述的是一种执行效果

### Syntax

Note that this is not the syntax for **zero initialization**, which does not have a dedicated syntax in the language. These are examples of other types of initializations, which might perform zero initialization.



| syntax                                |                                                              |      |
| :------------------------------------ | ------------------------------------------------------------ | ---- |
| `static T object ;`                   |                                                              |      |
| `T () ;` <br>`T t = {} ;`<br>`T {} ;` | As part of [value-initialization](https://en.cppreference.com/w/cpp/language/value_initialization) sequence for non-class types and for members of value-initialized class types that have no constructors, including value initialization of elements of [aggregates](https://en.cppreference.com/w/cpp/language/aggregate_initialization) for which no initializers are provided. |      |
| `CharT array [ n ] = "";`             |                                                              |      |


