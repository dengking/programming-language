# Declarations

在basic concept中，我们已经介绍了declaration的概念，关于各种entity的declaration，将分散到在介绍各个entity的时候进行说明，本章主要介绍的是c++ declaration的一般形式，掌握此，进而掌握c++ declaration非常重要。熟练掌握declaration非常重要。

## cppreference [Declarations](https://en.cppreference.com/w/cpp/language/declarations)

> NOTE: 原文的行文思路是这样的：首先介绍了c++ language中的各种declaration，粗略地数了一下，一共有10种大类，然后原文开始着重介绍Simple declaration，原文给出了Simple declaration的grammar，简单而言，它是由`decl-specifier-seq`和 `init-declarator-list`组成。本文，对原文的simple declaration进行详细阅读。其实Simple declaration，就是最最简单的形如`int a = 0`的这种declaration，而对于其他entity的declaration，在原文中仅仅给出了link。

### Simple declaration

A simple declaration is a statement that introduces, creates, and optionally initializes one or several identifiers, typically variables.

> NOTE: 简单地说，simple declaration就是形如`int a = 0`的这种declaration。

```
decl-specifier-seq init-declarator-list(optional) ;	
attr decl-specifier-seq init-declarator-list;
```

> NOTE: 这就是c++中，simple declaration的grammar，后面会对它进行详细分析。



### Specifiers

参见[Specifiers](./Specifiers.md)。



### Declarators

`init-declarator-list` is a comma-separated sequence of one or more `init-declarators`, which have the following syntax:

> NOTE: `init-declarator-list`中的list表示它可能包含多个`init-declarator`。

```
declarator initializer(optional)	
declarator requires-clause
```

|                          |      |                                                              |
| ------------------------ | ---- | ------------------------------------------------------------ |
| `declarator`             | -    | the declarator                                               |
| `initializer`            | -    | optional initializer (except where required, such as when initializing references or const objects). See [Initialization](https://en.cppreference.com/w/cpp/language/initialization) for details. |
| `requires-clause(C`++20) | -    | [a requires-clause](https://en.cppreference.com/w/cpp/language/constraints#Requires_clauses), which adds a [constraint](https://en.cppreference.com/w/cpp/language/constraints) to a [function declaration](https://en.cppreference.com/w/cpp/language/function) |

Each declarator introduces exactly one **object**, **reference**, **function**, or (for `typedef` declarations) **type alias**, whose `type` is provided by `decl-specifier-seq` and optionally modified by operators such as `&` (reference to) or `[]` (array of) or `()` (function returning) in the declarator. These operators can be applied recursively, as shown below.

> NOTE: 上面这段话非常重要，它指出了，在c++中，type一般是由`decl-specifier-seq`来指定的。
>
> 上面这段话中的function returning的含义是函数返回类型。

> NOTE: 原文的后面总结了各种simple declaration的declarator，本文略去。



### Notes

If a declaration introduces a variable with automatic storage duration, it is initialized when its declaration statement is executed. All automatic variables declared in a block are destroyed on exit from the block (regardless how the block is exited: via [exception](https://en.cppreference.com/w/cpp/language/exceptions), [goto](https://en.cppreference.com/w/cpp/language/goto), or by reaching its end), in order opposite to their order of initialization.

> NOTE: 这一段非常重要，它是我们所谓的exception safety，RAII的理论基础。

