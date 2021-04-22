# Initialization and constructor and conversion

搞清楚initialization 和 constructor 之间的对应关系非常重要，下面是比较好的素材。

## 以initialization为切入点

"Conversion is defined in terms of initialization" 并且 "各种initialization可能考虑多种constructor，即一对多"，因此以initialization为切入点是比较好的。

### Conversion and initialization

Conversion is defined in terms of initialization

#### implicit conversion

cppreference [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization)

> [Implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) is defined in terms of copy-initialization

implicit conversion是基于copy-initialization而定义的，copy-initialization是不考虑 [explicit constructors](https://en.cppreference.com/w/cpp/language/explicit) 的，因此 [Implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) 是不考虑  [explicit constructors](https://en.cppreference.com/w/cpp/language/explicit) 。因此，如果一个class只有 [explicit constructors](https://en.cppreference.com/w/cpp/language/explicit) ，因此，它就无法进行implicit conversion。 



cppreference [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)

It is said that a converting constructor specifies an implicit conversion from the types of its arguments (if any) to the type of its class. Note that non-explicit [user-defined conversion function](https://en.cppreference.com/w/cpp/language/cast_operator) also specifies an implicit conversion.





#### explicit conversion

cppreference [Explicit type conversion](https://en.cppreference.com/w/cpp/language/explicit_cast)

>  If there are more than one expression or [braced-init-list](https://en.cppreference.com/w/cpp/language/list_initialization) (since C++11) in parentheses, *new_type* must be a class with a suitably declared [constructor](https://en.cppreference.com/w/cpp/language/constructor). This expression is a prvalue of type *new_type* designating a temporary (until C++17)whose result object is (since C++17) [direct-initialized](https://en.cppreference.com/w/cpp/language/direct_initialization) with *expression-list*.



> A single-word type name followed by a *braced-init-list* is a prvalue of the specified type designating a temporary (until C++17)whose result object is (since C++17) [direct-list-initialized](https://en.cppreference.com/w/cpp/language/list_initialization) with the specified *braced-init-list*. If *new_type* is (possibly [cv-qualified](https://en.cppreference.com/w/cpp/language/cv)) `void`, the expression is a void prvalue without a result object (since C++17).

explicit conversion是否会考虑 [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization) ？我觉得它应该不会的。

### Initialization and constructor

1、C++定义了多种initialization，各种initialization可能考虑多种constructor，即一对多










