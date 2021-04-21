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



## Initialization



### cppreference [Direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization)

> NOTE: 
>
> 1、这种是最容易理解的

|                                                     |      |                     |
| --------------------------------------------------- | ---- | ------------------- |
| T object ( arg );<br/>T object ( arg1, arg2, ... ); |      |                     |
| T object { arg };                                   |      |                     |
| T ( other )<br/>T ( arg1, arg2, ... )               |      |                     |
| static_cast< T >( other )                           |      | explicit conversion |
| new T(args, ...)                                    |      |                     |
|                                                     |      |                     |



#### Constructor

Direct-initialization is more permissive than copy-initialization: copy-initialization only considers non-[explicit](https://en.cppreference.com/w/cpp/language/explicit) constructors and non-explicit user-defined [conversion functions](https://en.cppreference.com/w/cpp/language/cast_operator), while direct-initialization considers all constructors and all user-defined conversion functions.



### cppreference [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization)

|                      |                                                              |      |
| -------------------- | ------------------------------------------------------------ | ---- |
| T object = other;    |                                                              |      |
| T object = {other} ; |                                                              |      |
| f(other)             | when [passing an argument](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_function_call_operator) to a function by value |      |

#### Constructor

Copy-initialization is less permissive than direct-initialization: [explicit constructors](https://en.cppreference.com/w/cpp/language/explicit) are not [converting constructors](https://en.cppreference.com/w/cpp/language/converting_constructor) and are not considered for copy-initialization.



#### Implicit conversion and copy-initialization

[Implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) is defined in terms of copy-initialization: if an object of type `T` can be copy-initialized with expression `E`, then `E` is implicitly convertible to `T`.



#### Equals sign, `=`, in copy-initialization VS assignment operator overload

The equals sign, `=`, in copy-initialization of a named variable is not related to the assignment operator. Assignment operator overloads have no effect on copy-initialization.



### Summary

一、Direct initialization VS Copy initialization

cppreference [Direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) 

> Direct-initialization is more permissive(自由、宽松) than copy-initialization: 
>
> copy-initialization only considers non-[explicit](https://en.cppreference.com/w/cpp/language/explicit) constructors and non-explicit user-defined [conversion functions](https://en.cppreference.com/w/cpp/language/cast_operator), while 
>
> direct-initialization considers all constructors and all user-defined conversion functions.

如果只有explicit，那么是无法使用copy initialization的。





## Constructors



### 分类

#### explicit VS non-explicit

1、[explicit constructors](https://en.cppreference.com/w/cpp/language/explicit) 不是 [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)

2、一个class，应该不能够同时既定义 [explicit constructors](https://en.cppreference.com/w/cpp/language/explicit)  又定义 [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor) ，从下面的描述来看，两者之间存在着一定的对立。





### cppreference [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)

#### What is converting constructor？

A constructor that is not declared with the specifier [explicit](https://en.cppreference.com/w/cpp/language/explicit) and which can be called with a single parameter (until C++11) is called a *converting constructor*.

Implicitly-declared and user-defined non-explicit [copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor) and [move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) are converting constructors.



Unlike explicit constructors, which are only considered during [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) (which includes [explicit conversions](https://en.cppreference.com/w/cpp/language/explicit_cast) such as [static_cast](https://en.cppreference.com/w/cpp/language/static_cast)), converting constructors are also considered during [copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization), as part of [user-defined conversion sequence](https://en.cppreference.com/w/cpp/language/implicit_cast).

> NOTE: 
>
> 1、这是取自下面的例子:
>
> ```C++
> A a1 = 1;      // OK: copy-initialization selects A::A(int)
> ```
>
> 2、上述  [user-defined conversion sequence](https://en.cppreference.com/w/cpp/language/implicit_cast) ，其实就是 [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast)
>
> 3、copy constructor会参与两种initialization: 
>
> copy initialization 和 direct initialization



It is said that a converting constructor specifies an implicit conversion from the types of its arguments (if any) to the type of its class. Note that non-explicit [user-defined conversion function](https://en.cppreference.com/w/cpp/language/cast_operator) also specifies an implicit conversion.