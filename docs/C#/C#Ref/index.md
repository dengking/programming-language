# [C# documentation](https://docs.microsoft.com/en-us/dotnet/csharp/)





## [A tour of the C# language](https://docs.microsoft.com/en-us/dotnet/csharp/tour-of-csharp/)

###  .NET architecture

C# programs run on .NET, a virtual execution system called the **common language runtime (CLR)** and a set of class libraries. The **CLR** is the implementation by Microsoft of the **common language infrastructure (CLI)**, an international standard. The **CLI** is the basis for creating execution and development environments in which languages and libraries work together seamlessly.

> NOTE:
>
> CLI->CLR

Source code written in C# is compiled into an [intermediate language (IL)](https://docs.microsoft.com/en-us/dotnet/standard/managed-code) that conforms to the **CLI specification**. The **IL code** and resources, such as bitmaps and strings, are stored in an **assembly**, typically with an extension of *.dll*. An assembly contains a **manifest** that provides information about the assembly's types, version, and culture.

> NOTE: 
>
> 一、关于**manifest**，参见wikipedia [Manifest file](https://en.wikipedia.org/wiki/Manifest_file)
>
> 二、上面提及了 **assembly**，其实可以简单的理解它对于的是  "*.dll*"

When the C# program is executed, the assembly is loaded into the CLR. The CLR performs Just-In-Time (JIT) compilation to convert the IL code to native machine instructions. The CLR provides other services related to automatic garbage collection, exception handling, and resource management. Code that's executed by the CLR is sometimes referred to as "**managed code**." "**Unmanaged code**," is compiled into native machine language that targets a specific platform.

### Types and variables

There are two kinds of types in C#: *value types* and *reference types*. 

With value types, the variables each have their own copy of the data, and it isn't possible for operations on one to affect the other (except for `ref` and `out` parameter variables).

> NOTE:
>
> 关于`ref`，参见 docs.microsoft [ref (C# Reference)](https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/ref)
>
> 关于`out`，参见 docs.microsoft [out parameter modifier (C# Reference)](https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/out-parameter-modifier) 
>
> 


