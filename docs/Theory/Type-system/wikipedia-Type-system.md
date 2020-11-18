# wikipedia [Type system](https://en.wikipedia.org/wiki/Type_system)

This article is about type systems from the point-of-view of computer programming. For a theoretical formulation(阐述), see [type theory](https://en.wikipedia.org/wiki/Type_theory).

In [programming languages](https://en.wikipedia.org/wiki/Programming_language), a **type system** is a set of rules that assigns a property called [type](https://en.wikipedia.org/wiki/Type_(computer_science)) to the various constructs(组成部分) of a [computer program](https://en.wikipedia.org/wiki/Computer_program), such as [variables](https://en.wikipedia.org/wiki/Variable_(computer_science)), [expressions](https://en.wikipedia.org/wiki/Expression_(computer_science)), [functions](https://en.wikipedia.org/wiki/Function_(computer_science)) or [modules](https://en.wikipedia.org/wiki/Modular_programming)(将类型作为一个属性).[[1\]](https://en.wikipedia.org/wiki/Type_system#cite_note-FOOTNOTEPierce20021-1) These types formalize and enforce the otherwise implicit categories the programmer uses for data structures and components (e.g. "string", "array of float", "function returning boolean")(这段话中“the programmer uses for data structures and components ”是定语，修饰categories，字面意思是：类型形式化并强制执行程序员用于数据结构和组件的隐式类别。各种编程语言中都会提供基本类型，如`int`，`float`等，也会提供一些复合类型，如容器string等。显然这些复合类型的定义是离不开基本类型的). 

## Purpose of type system

The main purpose of a **type system** is to reduce possibilities for [bugs](https://en.wikipedia.org/wiki/Bug_(computer_programming)) in computer programs[[2\]](https://en.wikipedia.org/wiki/Type_system#cite_note-FOOTNOTECardelli20041-2) by defining [interfaces](https://en.wikipedia.org/wiki/Interface_(computer_science)) between different parts of a computer program, and then checking that the parts have been connected in a consistent way. **This checking** can happen statically (at [compile time](https://en.wikipedia.org/wiki/Compile_time)), dynamically (at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase))), or as a combination of static and dynamic checking. Type systems have other purposes as well, such as :

- expressing business rules(表达业务规则)
- enabling certain compiler optimizations
- allowing for [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch)
- providing a form of documentation
- etc



A type system **associates** a type with each computed value and, by examining the flow of these values, **attempts** to ensure or prove that no [type errors](https://en.wikipedia.org/wiki/Type_error) can occur. The given type system in question determines exactly what constitutes(构成) a type error, but in general the aim is to prevent operations expecting a certain kind of value from being used with values for which that operation does not make sense ([logic errors](https://en.wikipedia.org/wiki/Logic_error)). Type systems are often specified as part of [programming languages](https://en.wikipedia.org/wiki/Programming_language), and built into the interpreters and compilers for them; although the type system of a language can be extended by [optional tools](https://en.wikipedia.org/wiki/Extended_static_checking) that perform added kinds of checks using the language's original type syntax and grammar.

> NOTE: 前面这段文字主要阐述了这些内容：
>
> - 什么是类型
> - 编程语言中为什么要使用类型即使用类型能够为编程语言带来什么好处
> - 类型系统的组成
>
> Q&A:
>
> 运行时类型检查是什么？是和多态有关的吗？

## Usage overview

A [compiler](https://en.wikipedia.org/wiki/Compiler) may also use the static type of a value to optimize the storage it needs and the choice of algorithms for operations on the value. In many [C](https://en.wikipedia.org/wiki/C_(programming_language)) compilers the *float* [data type](https://en.wikipedia.org/wiki/Data_type), for example, is represented in 32 [bits](https://en.wikipedia.org/wiki/Bit), in accord with(与什么一致) the [IEEE specification for single-precision floating point numbers](https://en.wikipedia.org/wiki/IEEE_754-2008). They will thus use floating-point-specific [microprocessor operations](https://en.wikipedia.org/wiki/Instruction_set) on those values (floating-point addition, multiplication, etc.)(它们将对这些值使用浮点特定的微处理器操作（浮点加法，乘法等），这是一种优化).

> NOTE：这段胡以float类型的数据为例来说明：编译器在汇编阶段可以根据value的static type来优化对该value的存储以及选择处理该value的instruction。这就是interpretation model，参见`C-family-language\C-and-C++\Interpretation-model`。

The depth of type constraints(类型限制) and the manner of their evaluation(类型推断) affect the *typing* of the language. A [programming language](https://en.wikipedia.org/wiki/Programming_language) may further associate an operation with various resolutions for each type, in the case of [type polymorphism](https://en.wikipedia.org/wiki/Type_polymorphism)(在类型多态性的情况下，编程语言可以进一步将操作与每种类型的各种分辨率相关联。此处将resolutions翻译为分辨率，感觉是不对的). 

[Type theory](https://en.wikipedia.org/wiki/Type_theory) is the study of type systems. The concrete types of some programming languages, such as integers and strings, depend on practical issues of computer architecture, compiler implementation, and language design.

## Fundamentals

Formally, [type theory](https://en.wikipedia.org/wiki/Type_theory) studies type systems. A programming language must have occurrence to type check using the *type system* whether at compile time or runtime, manually annotated or automatically inferred. 

> NOTE: 无论是在编译时还是在运行时，手动注释或自动推断，编程语言都必须使用类型系统进行类型检查。

As [Mark Manasse](https://en.wikipedia.org/w/index.php?title=Mark_Manasse&action=edit&redlink=1) concisely put it:[[3\]](https://en.wikipedia.org/wiki/Type_system#cite_note-FOOTNOTEPierce2002208-3)

> The fundamental problem addressed by a type theory is to ensure that programs have meaning. The fundamental problem caused by a type theory is that meaningful programs may not have meanings ascribed to（归属于） them. The quest(追求) for richer type systems results from this tension.

> NOTE: Mark Manasse的观点是**类型**使程序具有**意义**，下面的话对这个观点进行了阐述：

Assigning a data type, termed *typing*, gives **meaning** to a sequence of [bits](https://en.wikipedia.org/wiki/Bit) such as a value in [memory](https://en.wikipedia.org/wiki/Computer_memory) or some [object](https://en.wikipedia.org/wiki/Object_(computer_science)) such as a [variable](https://en.wikipedia.org/wiki/Variable_(computer_science)). The **hardware** of a [general purpose computer](https://en.wikipedia.org/wiki/General_purpose_computer) is unable to discriminate(区分) between for example a [memory address](https://en.wikipedia.org/wiki/Memory_address) and an [instruction code](https://en.wikipedia.org/wiki/Instruction_code_(programming))(指令代码), or between a [character](https://en.wikipedia.org/wiki/Character_(computing)), an [integer](https://en.wikipedia.org/wiki/Integer), or a [floating-point number](https://en.wikipedia.org/wiki/Floating-point_number), because it makes no intrinsic distinction(区分) between any of the possible values that a sequence of bits might *mean*.[[note 1\]](https://en.wikipedia.org/wiki/Type_system#cite_note-Burroughs-4) Associating a sequence of bits with a type conveys that [meaning](https://en.wiktionary.org/wiki/meaning) to the programmable hardware to form a *symbolic system* composed of that hardware and some program.

> NOTE: 这段话所描述的其实就是interpretation model，参见参见`C-family-language\C-and-C++\Interpretation-model`。



When a programming language evolves a more elaborate(复杂) type system, it gains a more finely grained(细化) rule set than basic type checking, but this comes at a price when the type inferences (and other properties) become [undecidable](https://en.wikipedia.org/wiki/Undecidable_problem)(不可判定), and when more attention must be paid by the programmer to annotate(注释) code or to consider computer-related operations and functioning. It is challenging to find a sufficiently expressive type system that satisfies all programming practices in a [type safe](https://en.wikipedia.org/wiki/Type_safe) manner.

总结：随着编程语言的进化，类型系统也会变得更加复杂，除了最基本的type checking，类型系统将会包含更加细化的规则集，但是更加细化的规则集也意味着类型推导可能变得不确定，也意味着程序员需要花费更多的精力在对代码进行注释，思考与计算机相关的操作，功能等。所以寻找到一个既类型安全，有满足所有编程最佳实践的，并且具有足够表达力的类型系统是具有挑战性的。

The more type restrictions that are imposed by the compiler, the more *strongly typed* a programming language is. **Strongly typed languages**(强类型语言) often require the programmer to make explicit conversions(显示类型转换) in contexts where an implicit conversion would cause no harm(强类型语言通常需要程序员在隐式转换不会造成伤害的上下文中进行显式转换). Pascal's type system has been described as "too strong" because, for example, the size of an array or string is part of its type, making some programming tasks difficult.[[4\]](https://en.wikipedia.org/wiki/Type_system#cite_note-5)[[5\]](https://en.wikipedia.org/wiki/Type_system#cite_note-6) [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)) is also strongly typed but its types are automatically inferred(类型是自动推导的) so that explicit conversions are often (but not always) unnecessary.

总结：如果从类型限制的强弱来判定编程语言的话，那些类型限制较强的语言就可以称之为“强类型语言”。从作者所列举的**Pascal**和**Haskell**的例子来看，编程语言如何为程序员减低编程的工作量是非常重要的，两者同为强类型语言，但是Haskell的自动类型推导机制显然减低了其编程的工作量。

A programming language compiler can also implement a *dependent type* or an *effect system*, which enables even more program specifications to be verified by a type checker. Beyond simple value-type pairs, a virtual "region" of code is associated with an "effect" component describing *what* is being done *with what*, and enabling for example to "throw" an error report. Thus the symbolic system may be a *type and effect system*, which endows it with more safety checking than type checking alone.

总结：编程语言编译器也可以实现dependent type或effect system，这使type checker可以验证更多的程序规范。 除简单的value-type对之外，代码的虚拟“region”可以和一个"effect"**组件**相关联，这个**组件**能够描述正在做什么、“抛出”错误报告。 因此，symbolic system 可能是一个*type and effect system*，即除了type，它还涉及effect，它比单纯的类型检查赋予它更多的安全检查。

Whether automated by the compiler or specified by a **programmer**, a type system makes program behavior illegal if outside the type-system rules(类型系统无论是由编译器自动执行还是由程序员指定，如果程序不符合类型系统的规则，则程序行为就有可能变得非法). Advantages provided by programmer-specified type systems include:

- *Abstraction* (or *modularity*) – Types enable programmers to think at a higher level than the bit or byte, not bothering with low-level implementation. For example, programmers can begin to think of a string as a set of character values instead of as a mere array of bytes. Higher still, types enable programmers to think about and express [interfaces](https://en.wikipedia.org/wiki/Interface_(computer_science)) between two of *any*-sized subsystems. This enables more levels of localization so that the definitions required for interoperability of the subsystems remain consistent when those two subsystems communicate.
- *Documentation* – In more expressive type systems, types can serve as a form of [documentation](https://en.wikipedia.org/wiki/Documentation) clarifying the intent of the programmer. For example, if a programmer declares a function as returning a timestamp type, this documents the function when the timestamp type can be explicitly declared deeper in the code to be an integer type.

Advantages provided by compiler-specified type systems include:

- *Optimization* – Static type-checking may provide useful compile-time information. For example, if a type requires that a value must align in memory at a multiple of four bytes, the compiler may be able to use more efficient machine instructions.
- *Safety* – A type system enables the [compiler](https://en.wikipedia.org/wiki/Compiler) to detect meaningless or probably invalid code. For example, we can identify an expression `3 / "Hello, World"` as invalid, when the rules do not specify how to divide an [integer](https://en.wikipedia.org/wiki/Integer) by a [string](https://en.wikipedia.org/wiki/String_(computer_science)). Strong typing offers more safety, but cannot guarantee complete *type safety*.

总结：类型系统可以由编译器来实现（这是主流0），当然也可以由程序员来进行指定，无论是有谁，如果查询不符合类型规则，则程序的行为都可能是非法的。并且上面的内容还讨论了programmer-specified type systems和compiler-specified type systems各自的优势。

### Type errors

A type error is an unintended condition which might manifest(出现) in multiple stages of a program's development. Thus a facility for detection of the error is needed in the **type system**. In some languages, such as Haskell, for which [type inference](https://en.wikipedia.org/wiki/Type_inference) is automated, [lint](https://en.wikipedia.org/wiki/Lint_(software)) might be available to its compiler to aid in the detection of error.

**Type safety** contributes to [program correctness](https://en.wikipedia.org/wiki/Program_correctness), but can only guarantee correctness at the cost of making the type checking itself an [undecidable problem](https://en.wikipedia.org/wiki/Undecidable_problem).In a *type system* with automated type checking a program may prove to run incorrectly yet be safely typed, and produce no compiler errors. [Division by zero](https://en.wikipedia.org/wiki/Division_by_zero) is an unsafe and incorrect operation, but a type checker running at [compile time](https://en.wikipedia.org/wiki/Compile_time) only doesn't scan for division by zero in most languages, and then it is left as a [runtime error](https://en.wikipedia.org/wiki/Runtime_error). To prove the absence of these more-general-than-types defects, other kinds of [formal methods](https://en.wikipedia.org/wiki/Formal_method), collectively known as [program analyses](https://en.wikipedia.org/wiki/Program_analysis_(computer_science)), are in common use. Alternatively, a sufficiently expressive type system, such as in dependently typed languages, can prevent these kinds of errors (for example, expressing *the type of non-zero numbers*). In addition [software testing](https://en.wikipedia.org/wiki/Software_testing) is an [empirical](https://en.wikipedia.org/wiki/Empirical) method for finding errors that the type checker cannot detect.

## Type checking

The process of verifying and enforcing the constraints of types—*type checking*—may occur either at [compile-time](https://en.wikipedia.org/wiki/Compile-time) (a static check) or at [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)). If a language specification(语言规范) requires its typing rules strongly (i.e., more or less allowing only those automatic type conversions that do not lose information(即或多或少只允许那些不会丢失信息的自动类型转换)), one can refer to the process as *strongly typed*, if not, as *weakly typed*. The terms are not usually used in a strict sense.

**Q&A**:

哪些类型之间是能够进行自动类型转换的呢？

#### Static type checking

**Static type checking** is the process of verifying the [type safety](https://en.wikipedia.org/wiki/Type_safety) of a program based on analysis of a program's text (source code). If a program passes a static type checker, then the program is guaranteed to satisfy some set of type safety properties for all possible inputs.

**Static type checking** can be considered a limited form of [program verification](https://en.wikipedia.org/wiki/Program_verification) (see [type safety](https://en.wikipedia.org/wiki/Type_safety)), and in a type-safe language, can be considered also an optimization. If a compiler can prove that a program is well-typed, then it does not need to emit **dynamic safety checks**, allowing the resulting compiled binary to run faster and to be smaller.

**Static type checking** for **Turing-complete languages** is inherently(本质上) conservative. That is, if a **type system** is both *sound* (meaning that it rejects all incorrect programs) and *decidable* (meaning that it is possible to write an algorithm that determines whether a program is well-typed), then it must be *incomplete* (meaning there are correct programs, which are also rejected, even though they do not encounter runtime errors).[[6\]](https://en.wikipedia.org/wiki/Type_system#cite_note-7) 

总结：上面三段话对静态类型检查进行了介绍，其中第一段话和第二段话是比较好理解的，比较难理解的是第三段话。其实如果结合下面 这个例子的话，第三段话的含义也是可以倒推出来的。第三段话的意思是：对于**Turing-complete languages** 而言，静态类型检查是保守的。如果一个类型系统是sound和decidable的，那么它一定是incomplete的（意味着一个正确的程序，即使它不会产生运行时错误，但是静态类型检查还是会认为它是错误的而拒绝它）。比如下面的这个例子，如果`complex text`永远是`true`，则`else`分支将永远都不会被执行到，那么`else`分支中的代码就不会产生运行时错误，但是大多数type checker是会拒绝这个代码的。

For example, consider a program containing the code:

`if <complex test> then <do something> else <generate type error>`

Even if the expression `<complex test>` always evaluates to `true` at run-time, most **type checkers** will reject the program as ill-typed, because it is difficult (if not impossible) for a **static analyzer** to determine that the `else` branch will not be taken.[[7\]](https://en.wikipedia.org/wiki/Type_system#cite_note-FOOTNOTEPierce2002-8) Conversely, a **static type checker** will quickly detect type errors in rarely used code paths. Without **static type checking**, even [code coverage](https://en.wikipedia.org/wiki/Code_coverage) tests with 100% coverage may be unable to find such type errors. The tests(指前面提到的code coverage tests ) may fail to detect such type errors, because the combination of all places where values are created and all places where a certain value is used must be taken into account.

总结：这段话是结合上面的那个小程序对type checker进行了一个辩证地分析。

A number of useful and common programming language features cannot be checked statically, such as [downcasting](https://en.wikipedia.org/wiki/Downcasting). Thus, many languages will have both static and dynamic type checking; the static type checker verifies what it can, and dynamic checks verify the rest.

Many languages with **static type checking** provide a way to bypass(绕过) the **type checker**. Some languages allow programmers to choose between static and dynamic type safety. For example, [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) distinguishes between *statically-typed* and *dynamically-typed* variables. Uses of the former are checked statically, whereas uses of the latter are checked dynamically. Other languages allow writing code that is not type-safe. For example, in [C](https://en.wikipedia.org/wiki/C_(programming_language)), programmers can freely cast a value between any two types that have the same size.

总结：一些非常有用的语言特性是无法进行静态检查的，所以很多编程语言既有静态类型检查又有动态类型检查。并且结合前面的那个对静态类型检查进行分析的例子，可以看出有时候是不需要进行静态类型检查的，所以有的编程语言就允许由程序员来指定是否进行静态类型检查和动态类型检查，比如`C#`，来决定程序是否满足**静态类型安全性**和**动态类型安全性**；甚至有的语言并不要求类型安全，比如`C`语言，它允许对相同大小的类型进行转换。请参照下面这个链接来看哪些语言是会进行静态类型检查的。

For a list of languages with static type checking, see [the category for statically typed languages](https://en.wikipedia.org/wiki/Category:Statically_typed_programming_languages).

#### Dynamic type checking and runtime type information

**Dynamic type checking** is the process of verifying the **type safety** of a program at runtime. Implementations of **dynamically type-checked languages** generally associate each runtime object with a  **type tag**  (i.e., a reference to a type) containing its **type information**. This runtime type information (**RTTI**) can also be used to implement [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch), [late binding](https://en.wikipedia.org/wiki/Late_binding), [downcasting](https://en.wikipedia.org/wiki/Downcasting), [reflection](https://en.wikipedia.org/wiki/Reflection_(computer_programming)), and similar features.

总结：这段话给出了**Dynamic type checking** 即动态类型检查的含义的解释，以及其实现方式：RTTI，并且这里还提到了RTTI处理可以用于**Dynamic type checking** 外，还可以用于实现 [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch), [late binding](https://en.wikipedia.org/wiki/Late_binding), [downcasting](https://en.wikipedia.org/wiki/Downcasting), [reflection](https://en.wikipedia.org/wiki/Reflection_(computer_programming)), and similar features.

Most **type-safe languages** include some form of **dynamic type checking**, even if they also have a static type checker. The reason for this is that many useful features or properties are difficult or impossible to verify statically. For example, suppose that a program defines two types, A and B, where B is a **subtype** of A. If the program tries to convert a value of type A to type B, which is known as [downcasting](https://en.wikipedia.org/wiki/Downcasting), then the operation is legal only if the value being converted is actually a value of type B(如果用python来解释的话就是type()==B). Thus, a dynamic check is needed to verify that the operation is safe. This requirement is one of the criticisms(批评) of **downcasting**.

总结：这段话以**downcasting**为例说明了为什么编程语言需要**dynamic type checking**。

By definition, **dynamic type checking** may cause a program to fail at runtime. In some programming languages, it is possible to anticipate and recover from these failures. In others, type-checking errors are considered fatal.

Programming languages that include **dynamic type checking** but not **static type checking** are often called "**dynamically typed programming languages**". For a list of such languages, see [the category for dynamically typed programming languages](https://en.wikipedia.org/wiki/Category:Dynamically_typed_programming_languages).

总结：这里给出了**dynamically typed programming languages**的正确的定义。python就是一门**dynamically typed programming language**

#### Combining static and dynamic type checking

Some languages allow both static and dynamic typing (type checking), sometimes called *soft* typing. For example, Java and some other ostensibly statically typed languages support [downcasting](https://en.wikipedia.org/wiki/Downcasting) types to their [subtypes](https://en.wikipedia.org/wiki/Subtypes), querying an object to discover its **dynamic type,** and other type operations that depend on **runtime type information**. More generally, most programming languages include mechanisms for dispatching over different 'kinds' of data, such as [disjoint unions](https://en.wikipedia.org/wiki/Disjoint_union), [subtype polymorphism](https://en.wikipedia.org/wiki/Polymorphism_in_object-oriented_programming), and [variant types](https://en.wikipedia.org/wiki/Variant_type). Even when not interacting with type annotations or type checking, such mechanisms are materially similar to dynamic typing implementations. See [programming language](https://en.wikipedia.org/wiki/Programming_language) for more discussion of the interactions between static and dynamic typing.

Objects in **object-oriented languages** are usually accessed by a reference whose **static target type** (or **manifest type**(表面类型)) is equal to either the object's **run-time type** (its **latent type**(潜藏类型)) or a supertype thereof. This is conformant with the [Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle), which states that all operations performed on an instance of a given type can also be performed on an instance of a **subtype**. This concept is also known as **subsumption**. In some languages subtypes may also possess [covariant or contravariant](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)) return types and argument types respectively.

Certain languages, for example [Clojure](https://en.wikipedia.org/wiki/Clojure), [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp), or [Cython](https://en.wikipedia.org/wiki/Cython) are dynamically type-checked by default, but allow programs to opt into static type checking by providing optional **annotations**. One reason to use such hints would be to optimize the performance of critical sections of a program. This is formalized by [gradual typing](https://en.wikipedia.org/wiki/Type_system#Gradual_typing). The programming environment *DrRacket*, a pedagogic environment based on Lisp, and a precursor of the language [Racket](https://en.wikipedia.org/wiki/Racket_(programming_language)#Development) was also soft-typed.

Conversely, as of version 4.0, the C# language provides a way to indicate that a variable should not be statically type-checked. A variable whose type is `dynamic` will not be subject to static type checking. Instead, the program relies on runtime type information to determine how the variable may be used.[[8\]](https://en.wikipedia.org/wiki/Type_system#cite_note-9)

#### Static and dynamic type checking in practice

The choice between static and dynamic typing requires certain [trade-offs](https://en.wikipedia.org/wiki/Trade-off).

Static typing can find type errors reliably at compile time, which should increase the reliability of the delivered program. However, programmers disagree over how commonly type errors occur, resulting in further disagreements over the proportion of those bugs that are coded that would be caught by appropriately representing the designed types in code.[[9\]](https://en.wikipedia.org/wiki/Type_system#cite_note-10)[[10\]](https://en.wikipedia.org/wiki/Type_system#cite_note-11) Static typing advocates[*who?*] believe programs are more reliable when they have been well type-checked, whereas dynamic-typing advocates[*who?*] point to distributed code that has proven reliable and to small bug databases.[*citation needed*] The value of static typing, then, presumably[*vague*]increases as the strength of the type system is increased. Advocates of [dependent typing](https://en.wikipedia.org/wiki/Dependent_type),[*who?*] implemented in languages such as [Dependent ML](https://en.wikipedia.org/wiki/Dependent_ML) and [Epigram](https://en.wikipedia.org/wiki/Epigram_(programming_language)), have suggested that almost all bugs can be considered type errors, if the types used in a program are properly declared by the programmer or correctly inferred by the compiler.[[11\]](https://en.wikipedia.org/wiki/Type_system#cite_note-12)

Static typing usually results in compiled code that executes faster. When the compiler knows the exact data types that are in use (which is necessary for static verification, either through declaration or inference) it can produce optimized machine code. Some dynamically typed languages such as [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp) allow optional type declarations for optimization for this reason.

By contrast, dynamic typing may allow compilers to run faster and [interpreters](https://en.wikipedia.org/wiki/Interpreter_(computing)) to dynamically load new code, because changes to source code in dynamically typed languages may result in less checking to perform and less code to revisit.[*clarification needed*] This too may reduce the edit-compile-test-debug cycle.

Statically typed languages that lack [type inference](https://en.wikipedia.org/wiki/Type_inference) (such as [C](https://en.wikipedia.org/wiki/C_(programming_language)) and [Java](https://en.wikipedia.org/wiki/Java_(programming_language))) require that programmers declare the types that a method or function must use. This can serve as added program documentation, that is active and dynamic, instead of static. This allows a compiler to prevent it from drifting out of synchrony, and from being ignored by programmers. However, a language can be statically typed without requiring type declarations (examples include [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)), [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)), [OCaml](https://en.wikipedia.org/wiki/OCaml), [F#](https://en.wikipedia.org/wiki/F_Sharp_(programming_language)), and to a lesser extent [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) and [C++](https://en.wikipedia.org/wiki/C%2B%2B)), so explicit type declaration is not a necessary requirement for static typing in all languages.

Dynamic typing allows constructs that some static type checking would reject as illegal. For example, *eval* functions, which execute arbitrary data as code, become possible. An *eval* function is possible with static typing, but requires advanced uses of [algebraic data types](https://en.wikipedia.org/wiki/GADT). Further, dynamic typing better accommodates transitional code and prototyping, such as allowing a placeholder data structure ([mock object](https://en.wikipedia.org/wiki/Mock_object)) to be transparently used in place of a full data structure (usually for the purposes of experimentation and testing).

Dynamic typing typically allows [duck typing](https://en.wikipedia.org/wiki/Duck_typing) (which enables [easier code reuse](https://en.wikipedia.org/wiki/Duck_typing#Comparison_with_other_type_systems)). Many[*specify*] languages with static typing also feature [duck typing](https://en.wikipedia.org/wiki/Duck_typing#In_statically_typed_languages) or other mechanisms like [generic programming](https://en.wikipedia.org/wiki/Generic_programming) that also enable easier code reuse.

Dynamic typing typically makes [metaprogramming](https://en.wikipedia.org/wiki/Metaprogramming) easier to use. For example, [C++](https://en.wikipedia.org/wiki/C%2B%2B) [templates](https://en.wikipedia.org/wiki/Template_(programming)) are typically more cumbersome to write than the equivalent [Ruby](https://en.wikipedia.org/wiki/Ruby_(programming_language)) or [Python](https://en.wikipedia.org/wiki/Python_(programming_language)) code since [C++](https://en.wikipedia.org/wiki/C%2B%2B) has stronger rules regarding type definitions (for both functions and variables). This forces a developer to write more boilerplate code for a template than a Python developer would need to. More advanced run-time constructs such as [metaclasses](https://en.wikipedia.org/wiki/Metaclass) and [introspection](https://en.wikipedia.org/wiki/Introspection_(computer_science)) are often harder to use in statically typed languages. In some languages, such features may also be used e.g. to generate new types and behaviors on the fly, based on run-time data. Such advanced constructs are often provided by [dynamic programming languages](https://en.wikipedia.org/wiki/Dynamic_programming_language); many of these are dynamically typed, although *dynamic typing* need not be related to *dynamic programming languages*.

#### Strong and weak type systems 

Main article: [Strong and weak typing](https://en.wikipedia.org/wiki/Strong_and_weak_typing)

Languages are often colloquially referred to as *strongly typed* or *weakly typed*. In fact, there is no universally accepted definition of what these terms mean. In general, there are more precise terms to represent the differences between type systems that lead people to call them "strong" or "weak".

#### Type safety and memory safety 

Main article: [Type safety](https://en.wikipedia.org/wiki/Type_safety)

A third way of categorizing the type system of a programming language uses the safety of typed **operations** and **conversions**. Computer scientists consider a language "type-safe" if it does not allow operations or conversions that violate the rules of the type system.

Some observers use the term *memory-safe language* (or just *safe language*) to describe languages that do not allow programs to access memory that has not been assigned for their use. For example, a memory-safe language will [check array bounds](https://en.wikipedia.org/wiki/Bounds_checking), or else statically guarantee (i.e., at compile time before execution) that array accesses out of the array boundaries will cause compile-time and perhaps runtime errors.

Consider the following program of a language that is both **type-safe** and **memory-safe**:[[12\]](https://en.wikipedia.org/wiki/Type_system#cite_note-13)

```
1 var x := 5;     
2 var y := "37";  
3 var z := x + y;
```

In this example, the variable `z` will have the value 42. Although this may not be what the programmer anticipated, it is a well-defined result. If `y` were a different string, one that could not be converted to a number (e.g. "Hello World"), the result would be well-defined as well. Note that a program can be type-safe or memory-safe and still crash on an invalid operation; in fact, if a program encounters an operation that is not type-safe, terminating the program is often the only option.

Now consider a similar example in C:

```c
1 int x = 5;
2 char y[] = "37";
3 char* z = x + y;
```

In this example `z` will point to a memory address five characters beyond `y`, equivalent to three characters after the terminating zero character of the string pointed to by `y`. This is memory that the program is not expected to access. It may contain garbage data, and it certainly doesn't contain anything useful. As this example shows, C is neither a **memory-safe** nor a **type-safe** language.

In general, **type-safety** and **memory-safety** go hand in hand. For example, a language that supports **pointer arithmetic** and **number-to-pointer conversions** (like C) is neither **memory-safe** nor **type-safe**, because it allows arbitrary memory to be accessed as if it were valid memory of any type.

For more information, see [memory safety](https://en.wikipedia.org/wiki/Memory_safety).

#### Variable levels of type checking 

Some languages allow different levels of checking to apply to different regions of code. Examples include:

- The `use strict` directive in [JavaScript](https://en.wikipedia.org/wiki/JavaScript)[[13\]](https://en.wikipedia.org/wiki/Type_system#cite_note-14)[[14\]](https://en.wikipedia.org/wiki/Type_system#cite_note-15)[[15\]](https://en.wikipedia.org/wiki/Type_system#cite_note-16) and [Perl](https://en.wikipedia.org/wiki/Perl) applies stronger checking.
- The `declare(strict_types=1)` in [PHP](https://en.wikipedia.org/wiki/PHP)[[16\]](https://en.wikipedia.org/wiki/Type_system#cite_note-17) on a per-file basis allows only a variable of exact type of the type declaration will be accepted, or a `TypeError` will be thrown.
- The `Option Strict On` in [VB.NET](https://en.wikipedia.org/wiki/VB.NET) allows the compiler to require a conversion between objects.

Additional tools such as [lint](https://en.wikipedia.org/wiki/Lint_(software)) and [IBM Rational Purify](https://en.wikipedia.org/wiki/IBM_Rational_Purify) can also be used to achieve a higher level of strictness.

#### Optional type systems 

It has been proposed, chiefly by [Gilad Bracha](https://en.wikipedia.org/wiki/Gilad_Bracha), that the choice of type system be made independent of choice of language; that a type system should be a module that can be *plugged* into a language as needed. He believes this is advantageous, because what he calls mandatory type systems make languages less expressive and code more fragile.[[17\]](https://en.wikipedia.org/wiki/Type_system#cite_note-18) The requirement that types do not affect the semantics of the language is difficult to fulfill.

Optional typing is related to [gradual typing](https://en.wikipedia.org/wiki/Gradual_typing), but still distinct from it.[[18\]](https://en.wikipedia.org/wiki/Type_system#cite_note-19)[*better source needed*]

## Polymorphism and types 

Main article: [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))

The term *polymorphism* refers to the ability of code (especially, functions or classes) to act on values of multiple types, or to the ability of different instances of the same data structure to contain elements of different types. Type systems that allow polymorphism generally do so in order to improve the potential for code re-use: in a language with polymorphism, programmers need only implement a data structure such as a list or an [associative array](https://en.wikipedia.org/wiki/Associative_array) once, rather than once for each type of element with which they plan to use it. For this reason computer scientists sometimes call the use of certain forms of polymorphism *generic programming*. The type-theoretic foundations of polymorphism are closely related to those of [abstraction](https://en.wikipedia.org/wiki/Abstraction_(computer_science)), [modularity](https://en.wikipedia.org/wiki/Module_(programming)) and (in some cases) [subtyping](https://en.wikipedia.org/wiki/Subtype).

### Duck typing 

Main article: [Duck typing](https://en.wikipedia.org/wiki/Duck_typing)

In *duck typing*,[[19\]](https://en.wikipedia.org/wiki/Type_system#cite_note-20) a statement calling a [method](https://en.wikipedia.org/wiki/Method_(computer_science)) `m` on an object does not rely on the declared type of the object; only that the object, of whatever type, must supply an implementation of the method called, when called, at run-time.

Duck typing differs from [structural typing](https://en.wikipedia.org/wiki/Structural_type_system) in that, if the *part* (of the whole module structure) needed for a given local computation is present *at runtime*, the duck type system is satisfied in its type identity analysis. On the other hand, a structural type system would require the analysis of the whole module structure at compile time to determine type identity or type dependence.

Duck typing differs from a [nominative type system](https://en.wikipedia.org/wiki/Nominative_type_system) in a number of aspects. The most prominent ones are that for duck typing, type information is determined at runtime (as contrasted to compile time), and the name of the type is irrelevant to determine type identity or type dependence; only partial structure information is required for that for a given point in the program execution.

Duck typing uses the premise that (referring to a value) "if it walks like a duck, and quacks like a duck, then it is a duck" (this is a reference to the [duck test](https://en.wikipedia.org/wiki/Duck_test) that is attributed to [James Whitcomb Riley](https://en.wikipedia.org/wiki/James_Whitcomb_Riley)). The term may have been coined[*citation needed*] by [Alex Martelli](https://en.wikipedia.org/wiki/Alex_Martelli) in a 2000 message[[20\]](https://en.wikipedia.org/wiki/Type_system#cite_note-21) to the comp.lang.python [newsgroup](https://en.wikipedia.org/wiki/Newsgroup) (see [Python](https://en.wikipedia.org/wiki/Python_(programming_language))).

While one controlled experiment showed an increase in developer productivity for duck typing in single developer projects,[[21\]](https://en.wikipedia.org/wiki/Type_system#cite_note-22) other controlled experiments on API usability show the opposite.[[22\]](https://en.wikipedia.org/wiki/Type_system#cite_note-23)[[23\]](https://en.wikipedia.org/wiki/Type_system#cite_note-24)



## Explicit or implicit declaration and inference

Further information: [Type inference](https://en.wikipedia.org/wiki/Type_inference)

Many static type systems, such as those of C and Java, require *type declarations*: The programmer must explicitly associate each variable with a specific type. Others, such as Haskell's, perform *type inference*: The compiler draws conclusions about the types of variables based on how programmers use those variables. For example, given a function `f(x, y)` that adds `x` and `y` together, the compiler can infer that `x` and `y` must be numbers – since addition is only defined for numbers. Thus, any call to `f` elsewhere in the program that specifies a non-numeric type (such as a string or list) as an argument would signal an error.

Numerical and string constants and expressions in code can and often do imply type in a particular context. For example, an expression `3.14` might imply a type of [floating-point](https://en.wikipedia.org/wiki/Floating-point), while `[1, 2, 3]` might imply a list of integers – typically an [array](https://en.wikipedia.org/wiki/Array_data_structure).

Type inference is in general possible, if it is [decidable](https://en.wikipedia.org/wiki/Decidability_(logic)) in the type theory in question. Moreover, even if inference is undecidable in general for a given type theory, inference is often possible for a large subset of real-world programs. Haskell's type system, a version of [Hindley–Milner](https://en.wikipedia.org/wiki/Type_inference#Hindley%E2%80%93Milner_type_inference_algorithm), is a restriction of [System Fω](https://en.wikipedia.org/wiki/System_F-omega) to so-called rank-1 polymorphic types, in which type inference is decidable. Most Haskell compilers allow arbitrary-rank polymorphism as an extension, but this makes type inference undecidable. (Type checking is decidable, however, and rank-1 programs still have type inference; higher rank polymorphic programs are rejected unless given explicit type annotations.)

## Unified type system 

> NOTE: "unified" 的含义是 "统一的"。

Some languages like [Perl 6](https://en.wikipedia.org/wiki/Perl_6) or [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) have a unified type system.[[27\]](https://en.wikipedia.org/wiki/Type_system#cite_note-28) This means that all [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) types including primitive types inherit from a single root object. Every type in [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) inherits from the Object class. [Java](https://en.wikipedia.org/wiki/Java_(programming_language)) has several primitive types that are not objects. Java provides wrapper object types that exist together with the primitive types so developers can use either the wrapper object types or the simpler non-object primitive types.

## Compatibility: equivalence and subtyping 

A type-checker for a statically typed language must verify that the type of any [expression](https://en.wikipedia.org/wiki/Expression_(programming)) is consistent with the type expected by the context in which that expression appears. For example, in an [assignment statement](https://en.wikipedia.org/wiki/Assignment_statement) of the form `x := *e*`, the inferred type of the expression *e* must be consistent with the declared or inferred type of the variable `x`. This notion of consistency, called *compatibility*, is specific to each programming language.

If the type of *e* and the type of `x` are the same, and assignment is allowed for that type, then this is a valid expression. Thus, in the simplest type systems, the question of whether two types are compatible reduces to that of whether they are *equal* (or *equivalent*). Different languages, however, have different criteria for when two type expressions are understood to denote the same type. These different *equational theories* of types vary widely, two extreme cases being *structural type systems*, in which any two types that describe values with the same structure are equivalent, and *nominative type systems*, in which no two syntactically distinct type expressions denote the same type (*i.e.*, types must have the same "name" in order to be equal).

In languages with [subtyping](https://en.wikipedia.org/wiki/Subtype), the compatibility relation is more complex. In particular, if `A` is a subtype of `B`, then a value of type `A` can be used in a context where one of type `B` is expected, even if the reverse is not true. Like equivalence, the subtype relation is defined differently for each programming language, with many variations possible. The presence of parametric or ad hoc [polymorphism](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) in a language may also have implications for type compatibility.

