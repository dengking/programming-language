# 2 Introduction

SWIG was originally designed to make it extremely easy for scientists and engineers to build extensible scientific software without having to get a degree in software engineering. Because of this, the use of SWIG tends to be somewhat informal and ad-hoc (e.g., SWIG does not require users to provide formal interface specifications as you would find in a dedicated IDL compiler). 

> NOTE: 关于IDL compiler，参见:
>
> 1、https://chromium.googlesource.com/chromium/src/+/master/third_party/blink/renderer/bindings/IDLCompiler.md
>
> 2、https://en.wikipedia.org/wiki/Web_IDL
>
> 3、https://docs.oracle.com/cd/E13203_01/tuxedo/tux80/cref/idlchap.htm

Although this style of development isn't appropriate for every project, it is particularly well suited to software development in the small; especially the research and development work that is commonly found in scientific and engineering projects. However, nowadays SWIG is known to be used in many large open source and commercial projects. 



## 2.3 Target languages

SWIG is very broadly composed of two components. A core component creates a parse tree from the input ISO C/C++ and SWIG directives (extensions to the C/C++ standards). The parse tree is then passed to a second component, one of the target language modules for generating code specific to a higher level language. SWIG supports many different target languages. 

> NOTE: SWIG implementation



## 2.9 SWIG and freedom

No, this isn't a special section on the sorry state of world politics. However, it may be useful to know that SWIG was written with a certain "philosophy" about programming---namely that programmers are smart and that tools should just stay out of their way. Because of that, you will find that SWIG is extremely permissive in what it lets you get away with. In fact, you can use SWIG to go well beyond "shooting yourself in the foot" if dangerous programming is your goal. On the other hand, this kind of freedom may be exactly what is needed to work with complicated and unusual C/C++ applications. 

> NOTE: 
>
> "namely that programmers are smart and that tools should just stay out of their way. Because of that, you will find that SWIG is extremely permissive in what it lets you get away with. In fact, you can use SWIG to go well beyond "shooting yourself in the foot" if dangerous programming is your goal. "的意思是: 
>
> "程序员很聪明，工具应该不碍事。正因为如此，你会发现 SWIG 在让你侥幸逃脱的事情上是极其宽容的。事实上，如果危险的编程是您的目标，那么您可以使用 SWIG 远远超越“搬起石头砸自己的脚”。另一方面，这种自由可能正是处理复杂和不寻常的 C/C++ 应用程序所需要的。"

Ironically, the freedom that SWIG provides is countered by an extremely conservative approach to code generation. At its core, SWIG tries to distill even the most advanced C++ code down to a small well-defined set of interface building techniques based on ISO C programming. Because of this, you will find that SWIG interfaces can be easily compiled by virtually every C/C++ compiler and that they can be used on any platform. Again, this is an important part of staying out of the programmer's way----the last thing any developer wants to do is to spend their time debugging the output of a tool that relies on non-portable or unreliable programming features. Dependencies are often a source of incompatibilities and problems and so additional third party libraries are not used in the generated code. SWIG will also generally avoid generating code that introduces a dependency on the C++ Standard Template Library (STL). SWIG will generate code that depends on the C libraries though. 

> NOTE: 
>
> 一、全文翻译如下:
>
> "具有讽刺意味的是，SWIG 提供的自由被一种极其保守的代码生成方法所抵消。 SWIG 的核心是尝试将最先进的 C++ 代码提炼成一组基于 ISO C 编程的、定义明确的接口构建技术。正因为如此，您会发现 SWIG 接口几乎可以被每个 C/C++ 编译器轻松编译，并且可以在任何平台上使用。同样，这是远离程序员的一个重要部分——任何开发人员最不想做的事情就是花时间调试依赖于不可移植或不可靠的编程特性的工具的输出。依赖关系通常是不兼容和问题的根源，因此在生成的代码中不使用额外的第三方库。 SWIG 通常还会避免生成引入对 C++ 标准模板库 (STL) 的依赖的代码。不过，SWIG 将生成依赖于 C 库的代码。"
>
> 二、"you will find that SWIG interfaces can be easily compiled by virtually every C/C++ compiler" 这段话中的 "SWIG interfaces" 指的是由SWIG生成的C files。