# Function objects



## What is Function objects?

cppreference中的如下章节介绍了function object: 

1) cppreference [Function #Function objects](https://en.cppreference.com/w/cpp/language/functions#Function_objects)

2) cppreference [C++ named requirements: *FunctionObject*](https://en.cppreference.com/w/cpp/named_req/FunctionObject)

function object的概念可以顾名思义地来进行理解，我们把它拆解开来，逐个理解:

function: 说明它需要支持the function call expression 

object: 说明它需要是object，那么这就排除了function lvalue、reference to function

### Function object也是first class citizen

C++中object是first class citizen，因此function object也是first class citizen；

### cppreference [Function #Function objects](https://en.cppreference.com/w/cpp/language/functions#Function_objects)

Besides **function lvalues**, the **function call expression** supports 

1) pointers to functions

2) any value of **class type** that overloads the **function-call operator** or is convertible to **function pointer** (including [lambda-expressions](https://en.cppreference.com/w/cpp/language/lambda)) (since C++11).

> NOTE: lambda expression本质上是一个unnamed function，因此它convertible to function pointer。
>
> 上述 "**class type** that overloads the **function-call operator**"是functor，在`./Functor`中进行了描述。

Together, these types are known as [*FunctionObject*s](https://en.cppreference.com/w/cpp/named_req/FunctionObject), and they are used ubiquitously(无所不在) through the C++ standard library, see for example, usages of [*BinaryPredicate*](https://en.cppreference.com/w/cpp/named_req/BinaryPredicate) and [*Compare*](https://en.cppreference.com/w/cpp/named_req/Compare).

The standard library also provides a number of pre-defined [function object templates](https://en.cppreference.com/w/cpp/utility/functional) as well as the methods to compose new ones (including [std::less](https://en.cppreference.com/w/cpp/utility/functional/less), [std::mem_fn](https://en.cppreference.com/w/cpp/utility/functional/mem_fn), [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind), and [std::function](https://en.cppreference.com/w/cpp/utility/functional/function) (since C++11)).

> NOTE: 关于standard library提供的内容，参见`C++\Language-reference\Functions\Function-as-first-class-citizen\Function-objects\std-function-object`。

### cppreference [C++ named requirements: *FunctionObject*](https://en.cppreference.com/w/cpp/named_req/FunctionObject)

#### Requirements

| Expression |       Requirements       |
| :--------: | :----------------------: |
|  f(args)   | performs a function call |

#### Notes

Functions and references to functions are not function object types, but can be used where function object types are expected due to function-to-pointer [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast).

> NOTE: 章节传函数名即可。

## Application

Function object是我们首先function as first class citizen的重要方式。


