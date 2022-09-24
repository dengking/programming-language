# github [CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md)



> NOTE: 
>
> 一、由于内容非常多，因此将它的很多内容，分散到了不同的章节中
>
> 二、链接: 
>
> isocpp.github [C++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
>
> modernescpp [C++ Core Guidelines](https://www.modernescpp.com/index.php/category/modern-c?start=0)
>
> 
>



## [In: Introduction](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#in-introduction)



### In.aims: Aims

We do not suffer the delusion(妄想) that every one of these rules can be effectively applied to every code base. Upgrading(升级) old systems is hard. However, we do believe that a program that uses a rule is less error-prone and more maintainable than one that does not. Often, rules also lead to faster/easier **initial development**. As far as we can tell, these rules lead to code that performs as well or better than older, more conventional(传统的；常见的；惯例的) techniques; they are meant to follow the **zero-overhead principle** ("what you don't use, you don't pay for" or "when you use an abstraction mechanism appropriately, you get at least as good performance as if you had handcoded using lower-level language constructs"). Consider these rules ideals for new code, opportunities to exploit when working on older code, and try to approximate these ideals as closely as feasible. Remember:

> NOTE: 
>
> 一、翻译如下:
> 
> "我们不会妄想这些规则中的每一条都可以有效地应用到每一个代码库中。升级旧系统非常困难。
> 然而，我们相信使用规则的程序比不使用规则的程序更不易出错，更容易维护。通常情况下，规则也会导致更快/更容易的初始开发。就我们所知，这些规则会导致代码的性能和旧的、更传统的技术一样好，甚至更好;
> 它们意味着遵循零开销原则(“不使用的，不付费”或“当您适当地使用抽象机制时，您至少可以获得与使用低级语言结构手工编码一样好的性能”)。考虑这些规则对于新代码的理想，在处理旧代码时利用的机会，并尽可能接近这些理想"
> 
> 二、关于" **zero-overhead principle** "，参见对应章节



#### In.0: Don't panic!

These guidelines are designed according to the "subset of superset" principle ([Stroustrup05](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Stroustrup05)). They do not simply define a subset of C++ to be used (for reliability, safety, performance, or whatever). Instead, they strongly recommend the use of a few simple "extensions" ([library components](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-gsl)) that make the use of the most error-prone features of C++ redundant, so that they can be banned(禁止) (in our set of rules).

> NOTE:
>
> 一、"[Stroustrup05](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Stroustrup05)"所链接的是: [A rationale for semantically enhanced library languages](http://www.stroustrup.com/SELLrationale.pdf)，一定要阅读这篇文章，感觉" [CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) + gsl + CppCorechecker"就是按照这篇文章的思想设计的
>
> 二、"[library components](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-gsl)"所指的是gsl。
>
> 三、"Instead, they strongly recommend the use of a few simple "extensions" ([library components](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-gsl)) that make the use of the most error-prone features of C++ redundant, so that they can be banned(禁止) (in our set of rules)" 要如何理解？
>
> 这段话的意思是: 使用gsl来替代"the use of the most error-prone features of C++"，从而使得它们是redundant，这样"so that they can be banned(禁止) (in our set of rules)"

The rules emphasize **static type safety** and **resource safety**. For that reason, they emphasize possibilities for **range checking**, for avoiding dereferencing `nullptr`, for avoiding **dangling pointers**, and the systematic(系统的) use of exceptions (via RAII). Partly to achieve that and partly to minimize obscure code as a source of errors, the rules also emphasize simplicity and the hiding of necessary complexity behind well-specified interfaces.

### In.not: Non-aims



### In.force: Enforcement

> NOTE: 
>
> 1、这段话讨论的是如何(强制)实施这些rule

Rules with no enforcement are unmanageable for large code bases. Enforcement of all rules is possible only for a small weak set of rules or for a specific user community.

> NOTE: 
>
> 1、这段话的意思是: 对于large code bases 而言，没有实施的规则是难以管理的；但是，"Enforcement of all rules " 有的时候是不可能的

1、But we want lots of rules, and we want rules that everybody can use.

2、But different people have different needs.

3、But people don't like to read lots of rules.

4、But people can't remember many rules.

So, we need **subsetting** to meet a variety of needs.

- But arbitrary subsetting leads to chaos.

We want guidelines that help a lot of people, make code more uniform, and strongly encourage people to modernize their code. We want to encourage best practices, rather than leave all to individual choices and management pressures. The ideal is to use all rules; that gives the greatest benefits.

This adds up to quite a few dilemmas. We try to resolve those using **tools**. Each rule has an **Enforcement** section listing ideas for enforcement. Enforcement might be done by code review, by static analysis, by compiler, or by run-time checks. Wherever possible, we prefer "mechanical" checking (humans are slow, inaccurate, and bore easily) and static checking. Run-time checks are suggested only rarely where no alternative exists; we do not want to introduce "distributed bloat". Where appropriate, we label a rule (in the **Enforcement** sections) with the name of groups of related rules (called "profiles"). A rule can be part of several profiles, or none. For a start, we have a few profiles corresponding to common needs (desires, ideals):

> NOTE: 
>
> 一、"Where appropriate, we label a rule (in the **Enforcement** sections) with the name of groups of related rules (called "profiles"). A rule can be part of several profiles, or none"
>
> 这段话阐述了profile 和 rule之间的关系
>
> 一、"Wherever possible, we prefer "mechanical" checking (humans are slow, inaccurate, and bore easily) and static checking" 的实现: 
>
> 1、cppcorecheck，参见:
>
> [cppcorecheck Archives | C++ Team Blog](https://devblogs.microsoft.com/cppblog/tag/cppcorecheck/)
>
> [Use the C++ Core Guidelines checkers](https://docs.microsoft.com/en-us/cpp/code-quality/using-the-cpp-core-guidelines-checkers?view=msvc-160)
>
> 

1、**type**: No type violations (reinterpreting a `T` as a `U` through casts, unions, or varargs)

2、**bounds**: No bounds violations (accessing beyond the range of an array)

3、**lifetime**: No leaks (failing to `delete` or multiple `delete`) and no access to invalid objects (dereferencing `nullptr`, using a dangling reference).

> NOTE: 
>
> 1、lifetime对应的是object based resource management；
>
> 2、multiple `delete` 中最最典型的就是double free

The profiles are intended to be used by tools, but also serve as an aid to the human reader. We do not limit our comment in the **Enforcement** sections to things we know how to enforce; some comments are mere wishes that might inspire some tool builder.

Tools that implement these rules shall respect the following syntax to explicitly suppress a rule:

```C++
[[gsl::suppress(tag)]]
```

and optionally with a message (following usual C++11 standard attribute syntax):

```C++
[[gsl::suppress(tag, justification: "message")]]
```

where

- `tag` is the **anchor name** of the item where the Enforcement rule appears (e.g., for [C.134](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rh-public) it is "Rh-public"), the name of a profile group-of-rules ("type", "bounds", or "lifetime"), or a specific rule in a profile ([type.4](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Pro-type-cstylecast), or [bounds.2](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Pro-bounds-arrayindex))
- `"message"` is a string literal



> NOTE: 
>
> 1、关于上述"suppress a rule"的例子:
>
> [microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)**/[include](https://github.com/microsoft/GSL/tree/main/include)/[gsl](https://github.com/microsoft/GSL/tree/main/include/gsl)/[narrow](https://github.com/microsoft/GSL/blob/main/include/gsl/narrow)
>
> ```C++
> // narrow() : a checked version of narrow_cast() that throws if the cast changed the value
> template <class T, class U>
> // clang-format off
> GSL_SUPPRESS(type.1) // NO-FORMAT: attribute
> GSL_SUPPRESS(f.6) // NO-FORMAT: attribute // TODO: MSVC /analyze does not recognise noexcept(false)
> // clang-format on
>     constexpr T narrow(U u) noexcept(false)
> {
>     constexpr const bool is_different_signedness =
>         (std::is_signed<T>::value != std::is_signed<U>::value);
> 
>     const T t = narrow_cast<T>(u);
> 
>     if (static_cast<U>(t) != u || (is_different_signedness && ((t < T{}) != (u < U{}))))
>     {
>         throw narrowing_error{};
>     }
> 
>     return t;
> }
> ```
>
> 



## Appendix D: Supporting tools

This section contains a list of tools that directly support adoption of the C++ Core Guidelines. This list is not intended to be an exhaustive list of tools that are helpful in writing good C++ code. If a tool is designed specifically to support and links to the C++ Core Guidelines it is a candidate for inclusion.

### Tools: [Clang-tidy](http://clang.llvm.org/extra/clang-tidy/checks/list.html)

Clang-tidy has a set of rules that specifically enforce the C++ Core Guidelines. These rules are named in the pattern `cppcoreguidelines-*`.

### Tools: [CppCoreCheck](https://docs.microsoft.com/en-us/visualstudio/code-quality/using-the-cpp-core-guidelines-checkers)

The Microsoft compiler's C++ code analysis contains a set of rules specifically aimed at enforcement of the C++ Core Guidelines.