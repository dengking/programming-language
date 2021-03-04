# Concepts

是在阅读cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)的时候，发现了[concepts](https://en.cppreference.com/w/cpp/language/constraints)特性的。

## cppreference [Constraints and concepts](https://en.cppreference.com/w/cpp/language/constraints) 

> NOTE: 查看右侧的目录可以知道，原文的行文思路是：
>
> 首先介绍constraint和concept的概念，然后对concept进行介绍，然后对constraint进行介绍。

Named sets of such requirements are called *concepts*. Each concept is a predicate, evaluated at compile time, and becomes a part of the interface of a template where it is used as a **constraint**:

> NOTE: 这段话表述了concept和constrain的关系：我们可以定义concept，然后将这个concept用作constraint。



The intent of concepts is to model semantic categories (Number, Range, RegularFunction) rather than syntactic restrictions (HasPlus, Array). According to [ISO C++ core guideline T.20](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t20-avoid-concepts-without-meaningful-semantics), "The ability to specify a meaningful semantics is a defining characteristic of a true concept, as opposed to a syntactic constraint."

> NOTE: concept提供给programmer来表达semantic的



### [Concepts](https://en.cppreference.com/w/cpp/language/constraints#Concepts)



### [Constraints](https://en.cppreference.com/w/cpp/language/constraints#Constraints)

#### Conjunctions



#### Disjunctions



#### Atomic constraints



#### Constraint normalization



### [Requires clauses](https://en.cppreference.com/w/cpp/language/constraints#Requires_clauses)

### [Requires expressions](https://en.cppreference.com/w/cpp/language/constraints#Requires_expressions)



#### Simple requirements



#### Type requirements



#### Compound Requirements



#### Nested requirements



### [Partial ordering of constraints](https://en.cppreference.com/w/cpp/language/constraints#Partial_ordering_of_constraints)


