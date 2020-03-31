# Basic concepts

本文是对cppreference [Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)的阅读理解，说实话，它比较不好理解，记得初次阅读它的时候，我完全被其中的一些列的概念给混淆了，经历了一段时间的探索、学习、使用后，现在再来阅读它，有了完全不同的感受：

- 我觉得它对C++语言总结地非常好，如果能够充分地理解它，就能够建立起对c++语言的高屋建瓴的视角
- 我觉得如果有compiler principle（参见工程[**compiler principle**](https://dengking.github.io/compiler-principle/)）、programming language的基础知识，那么理解起来会更加容易

## cppreference [Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)



A C++ program is a sequence of text files (typically header and source files) that contain [declarations](https://en.cppreference.com/w/cpp/language/declarations). They undergo [translation](https://en.cppreference.com/w/cpp/language/translation_phases) to become an executable program, which is executed when the C++ implementation calls its [main function](https://en.cppreference.com/w/cpp/language/main_function).

Certain words in a C++ program have special meaning, and these are known as [keywords](https://en.cppreference.com/w/cpp/keyword). Others can be used as [identifiers](https://en.cppreference.com/w/cpp/language/identifiers). [Comments](https://en.cppreference.com/w/cpp/comment) are ignored during translation. Certain characters in the program have to be represented with [escape sequences](https://en.cppreference.com/w/cpp/language/escape).

The *entities* of a C++ program are values, [objects](https://en.cppreference.com/w/cpp/language/objects), [references](https://en.cppreference.com/w/cpp/language/reference), [structured bindings](https://en.cppreference.com/w/cpp/language/structured_binding) (since C++17), [functions](https://en.cppreference.com/w/cpp/language/functions), [enumerators](https://en.cppreference.com/w/cpp/language/enum), [types](https://en.cppreference.com/w/cpp/language/type), class members, [templates](https://en.cppreference.com/w/cpp/language/templates), [template specializations](https://en.cppreference.com/w/cpp/language/template_specialization), [namespaces](https://en.cppreference.com/w/cpp/language/namespace), and [parameter packs](https://en.cppreference.com/w/cpp/language/parameter_pack). Preprocessor [macros](https://en.cppreference.com/w/cpp/preprocessor/replace) are not C++ entities.

> NOTE: 
>
> 原文的上述三段在向我们解释“C++ program”，包括what is C++ program、C++ program的组成。
>
> ### What is C++ program？
>
> 对于问题“what is C++ program”，这个其实是非常好理解的，在上述第一段中给出了解释。
>
> ### C++ program的组成
>
> 上述第三段回答了问题“C++ program的组成”，这一段中，使用“entity”的概念来描述C++ program的组成，作者（其实就是c++语言的设计者）认为，一个C++ program由上述所列举的11种entity组成。如果你已经有了编写c++ program的经验，那么请你回想一下，你的c++ program是否是由上面枚举的这些entity所组成的。
>
> 比较不幸的是，原文并没有给出“entity”概念的定义，而仅仅告诉了读者在“C++ program”中，哪些是entity，那要如何来理解“entity”呢？既然它是一个由“entity”是c++语言的设计者所**抽象**的，在文章[Abstraction](https://dengking.github.io/Post/Abstraction/Abstraction/)中我们已经对**抽象**进行了总结：
>
> > 抽象是概括的过程，抽象是提取公共特征的过程，它所概括的、所提取的公共特征，可以使用 [concepts](https://en.wikipedia.org/wiki/Concept) 来进行表示
>
> 显然，它是C++ program的组成单位，它是c++语言的设计者提供给用户来使用的，从这个角度来看，c++语言的使用者，在进行编程的时候，就是在描述一个一个的entity并使用已经描述好的entity。
>
> 显然entity是一个比statement更加大的概念，因为我们知道function是由多个statement所组成的（参见文章[Unit](https://dengking.github.io/Post/Unit/)的[Programming language的unit of user-defined action](https://dengking.github.io/Post/Unit/#programming-languageunit-of-user-defined-action)段），关于此，在后面也会进行描述。
>
> 需要注意的是，entity是c++ program的组成单位，它是一个编译时的概念，它需要由compiler来进行“理解”（参见工程[compiler-principle](https://dengking.github.io/compiler-principle/)的[Chapter 7 Run-Time Environments](https://dengking.github.io/compiler-principle/Chapter-7-Run-Time-Environments/)）。
>
> 经过上面的分析，我们已经清楚了entity的概念，现在我们思考这样的一个问题：这些entity之间的关系是怎样的？关于这个问题，参见文章[**C++ Entities and Relationships**](https://scottmcpeak.com/elkhound/sources/elsa/doc/cpp_er.html)。



*[Declartions](https://en.cppreference.com/w/cpp/language/declarations)* may introduce entities, associate them with [names](https://en.cppreference.com/w/cpp/language/name) and define their properties. The declarations that define all properties required to use an entity are [definitions](https://en.cppreference.com/w/cpp/language/definition). A program must contain only one definition of any non-inline function or variable that is [odr-used](https://en.cppreference.com/w/cpp/language/definition#ODR-use).

> NOTE:
>
> 前面描述了“entity”的概念，上面这段则告诉我们，在c++语言中通过*[declartion](https://en.cppreference.com/w/cpp/language/declarations)* 来引入（或者说“创建”）“entity”，在declare一个entity的时候，用户需要指定它的“name”，这样我们就可以通过这个name来使用这个entity了。



Definitions of functions include sequences of [statements](https://en.cppreference.com/w/cpp/language/statements), some of which include [expressions](https://en.cppreference.com/w/cpp/language/expressions), which specify the computations to be performed by the program.

> NOTE: 
>
> 上面这段话描述了function和statement之间的关系、statement和expression之间的关系



Names encountered in a program are associated with the declarations that introduced them using [name lookup](https://en.cppreference.com/w/cpp/language/lookup). Each name is only valid within a part of the program called its [scope](https://en.cppreference.com/w/cpp/language/scope). Some names have [linkage](https://en.cppreference.com/w/cpp/language/storage_duration) which makes them refer to the same entities when they appear in different scopes or translation units.

> NOTE: 
>
> 通过前面的描述，我们已经知道了：在declare一个entity的时候，我们会给它指定一个name，在进行编程的时候，我们通过这个name来引用这个entity。上面这段话则对与name相关的一些内容进行了介绍。需要注意的是， [name lookup](https://en.cppreference.com/w/cpp/language/lookup)是compiler的一个动作。



Each object, reference, function, expression in C++ is associated with a [type](https://en.cppreference.com/w/cpp/language/type), which may be [fundamental](https://en.cppreference.com/w/cpp/language/types), compound, or [user-defined](https://en.cppreference.com/w/cpp/language/classes), complete or [incomplete](https://en.cppreference.com/w/cpp/language/incomplete_type), etc.

> NOTE:
>
> 关于object，在文章Object中进行了详细分析。



Declared objects and declared references that are not [non-static data members](https://en.cppreference.com/w/cpp/language/data_members) are *variables*.

