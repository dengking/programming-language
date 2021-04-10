# [A rationale for semantically enhanced library languages](https://www.stroustrup.com/SELLrationale.pdf)

> NOTE: 
>
> 1、是在阅读 github" [CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) # In.0: Don't panic!"时，其中提及了这篇文章:
>
> > These guidelines are designed according to the "subset of superset" principle ([Stroustrup05](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Stroustrup05)). 
>
> 因此，需要结合[CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) 和 GSL 来理解本文的内容
>
> 2、本文标题的含义是: 语义增强库语言的原理
>
> 

## Abstract

This paper presents the rationale(原理) for a novel approach to providing expressive, teachable, maintainable, and cost-effective **special-purpose languages**: A Semantically Enhanced Library Language (a SEL language or a SELL) is a dialect created by **supersetting** a language using a library and then **subsetting** the result using a tool that “understands” the syntax and semantics of both the **underlying language** and the **library**. The resulting language can be about as expressive as a **special purpose language** and provide as good **semantic** guarantees as a **special-purpose language**. However, a SELL can rely on the **tool chain** and **user community** of a major general purpose programming language. The examples of SELLs presented here (**Safe C++**, **Parallel C++**, and **Real-time C++**) are based on C++ and the Pivot program analysis and transformation infrastructure. As part of the rationale, the paper discusses practical problems with various popular approaches to providing special-purpose features, such as compiler options and preprocessors.

> NOTE: 
>
> 1、"Semantically Enhanced Library Language"是一种 **special-purpose languages**、dialect 。
>
> 它使用一个library来"**supersetting** a language"，然后使用一个同时 "“understands” the syntax and semantics of both the **underlying language** " 和 "the **library**" 的 "tool" 来  "**subsetting** the result "
>
> 这其实就是  github" [CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) # In.0: Don't panic!" 中所述的  "subset of superset" principle 。
>
> 2、语言的发展需要不断地添加新的特性，不断地进行强化，而语言的标准委员会可能不会立即采用这种新的特性，那么此时我们就可以使用 "semantically enhanced library languages" 来使用这些新特性。

## 1 Introduction

> NOTE: 
>
> 1、这一章节主要在比较 **special-purpose languages** 和 Semantically Enhanced Library Languages

We often need specialized languages. Researchers need to experiment with new language features, such as concurrency features [24], facilities for integration with databases [5], and graphics [4] . Developers can sometimes gain a couple of orders of magnitude reductions in source code size with corresponding
reductions in development time and defect rates, by using such special-purpose languages in their intended domains. Unfortunately, such special-purpose languages are typically hard to design, tedious to implement, expensive to maintain, and — despite their obvious utility — tend to die young.

> NOTE: 
>
> 1、上面这一段是在阐明specialized languages的优与劣

Using a (special-purpose) library is an obvious alternative to a special-purpose language. However, a library cannot express or exploit semantic guarantees beyond what its host language provides. The basic idea of **Semantically Enhanced Library Languages** (SEL Languages or simply SELLs) is that when augmented by a library, a general-purpose language can be about as expressive as a **special-purpose language** and by **subsetting** that extended language, a tool can provide about as good semantic guarantees. Such guarantees can be used to provide better code, better representations, and more sophisticated transformations than would be possible for the full base language. For example, we can provide support for parallel operations on containers as a library. We can then analyze the program to ensure that no undesirable access to elements of those containers occurs — a task that could be simplified by enforcing a ban of languages features that happened to be undesirable in this context. Finally we can perform high-level transformations (such as parallelizing) by taking advantage of the known semantics of the libraries.

Like a library, a SELL can benefit from the extensive educational, tools, and library infrastructure of the base language. Therefore, the cost of designing, implementing, and using a SELL is minuscule compared with a special-purpose language with a small user base. Examples will be based on ISO standard C++ supported by the Pivot infrastructure for program analysis and transformation (§5.2). The focus will be on
templates because they provide the key mechanism for statically type-safe expression of advanced ideas in C++.