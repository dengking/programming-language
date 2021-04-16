# stroustrup [Supporting SELL for High-Performance Computing](https://www.stroustrup.com/SELL-HPC.pdf)

## 1 Languages and libraries

For ease of programming, portability, and acceptable performance, we design and implement **special-purpose programming languages** for high-performance computing [20]. These languages usually die young(这些语言通常英年早逝). Here, we briefly present a rationale for an alternative approach: **Semantically Enhanced Library Languages, SELLs**. A SELL is a language created by extending a programming language (usually a popular general-purpose programming language) with a library providing the desired added functionality and then using a tool to provide the desired semantic guarantees needed to reach a goal (often a higher level semantics, absence of certain kinds of errors, or library-specific optimizations). This paper focuses on a tool, The Pivot, being developed to support SELLs in ISO C++ [15, 7] and its application to High-Performance Computing.

> NOTE: 
>
> 1、这一段的关于作者的**Semantically Enhanced Library Languages, SELLs**的论述是非常好的
>
> 

## 2 A brief overview of the Pivot

The Pivot is a general framework for the analysis and transformation of C++ programs. The Pivot is designed to handle the complete ISO C++, especially more advanced uses of templates and including some proposed C++0x features. It is compiler independent. The central part of the Pivot is a **fully typed abstract syntax tree** called **IPR (Internal Program Representation)**. There are lots of (more than 20) tools for static analysis and transformation of C++ programs, e.g. [11, 3, 12, 10]. However, few — if any — handle all of ISO Standard C++ [7, 15], most are specialized to particular forms of analysis or transformation, and few will work well in combination with other tools. We are particularly interested in advanced uses of templates as used in generic programming, template meta-programming, and experimental uses of libraries as the basis of language extension. For that, we need a representation that deals with types as first-class citizens and allows analysis and transformation based on their properties. In the C++ community, this is discussed under the heading of concepts and is likely to receive significant language support in the next ISO C++ standard (C++0x) [17, 18, 16, 13, 19]. You can think of a concept as the type of a type. From the point of view of support for HPC — and for the provision(规定) of special-purpose facilities in general — a concept can be seen as a way of specifying new types with associated semantics without the modification of compilers or new syntax. That done, the SELL approach then uses the concepts as a hook for semantic properties beyond what C++ offers. This paper is an overview and will not go into details of concepts, the Pivot, the IPR, or our initial uses.

