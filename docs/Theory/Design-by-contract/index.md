# Design by contract



## wikipedia [Design by contract](https://en.wikipedia.org/wiki/Design_by_contract)

**Design by contract** (**DbC**), also known as **contract programming**, **programming by contract** and **design-by-contract programming**, is an approach for [designing software](https://en.wikipedia.org/wiki/Software_design).

It prescribes that software designers should define [formal](https://en.wikipedia.org/wiki/Formal_methods), precise and verifiable interface specifications for [software components](https://en.wikipedia.org/wiki/Component-based_software_engineering#Software_component), which extend the ordinary definition of [abstract data types](https://en.wikipedia.org/wiki/Abstract_data_type) with [preconditions](https://en.wikipedia.org/wiki/Precondition), [postconditions](https://en.wikipedia.org/wiki/Postcondition) and [invariants](https://en.wikipedia.org/wiki/Invariant_(computer_science)). These specifications are referred to as "contracts", in accordance with a [conceptual metaphor](https://en.wikipedia.org/wiki/Conceptual_metaphor) with the conditions and obligations of business contracts.



The DbC approach [assumes](https://en.wikipedia.org/wiki/Offensive_programming) all *client components* that invoke an operation on a *server component* will meet the preconditions specified as required for that operation.

### History

Design by contract has its roots in work on [formal verification](https://en.wikipedia.org/wiki/Formal_verification), [formal specification](https://en.wikipedia.org/wiki/Formal_specification) and [Hoare logic](https://en.wikipedia.org/wiki/Hoare_logic). 

### Description

The notion of a contract extends down to the method/procedure level; the contract for each method will normally contain the following pieces of information:[*[citation needed](https://en.wikipedia.org/wiki/Wikipedia:Citation_needed)*]

- Acceptable and unacceptable input values or types, and their meanings
- Return values or types, and their meanings
- Error and [exception](https://en.wikipedia.org/wiki/Exception_handling) condition values or types that can occur, and their meanings
- [Side effects](https://en.wikipedia.org/wiki/Side_effect_(computer_science))
- [Preconditions](https://en.wikipedia.org/wiki/Precondition)
- [Postconditions](https://en.wikipedia.org/wiki/Postcondition)
- [Invariants](https://en.wikipedia.org/wiki/Invariant_(computer_science))
- (more rarely) Performance guarantees, e.g. for time or space used

### Performance implications

In many programming languages, contracts are implemented with [assert](https://en.wikipedia.org/wiki/Assertion_(software_development)). Asserts are by default compiled away in release mode in C/C++, and similarly deactivated in C#[[8\]](https://en.wikipedia.org/wiki/Design_by_contract#cite_note-8) and Java.

### Language support

#### Languages with native support

- [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language))[[13\]](https://en.wikipedia.org/wiki/Design_by_contract#cite_note-scala-assertions-dbc-13)[[14\]](https://en.wikipedia.org/wiki/Design_by_contract#cite_note-14)

- [Kotlin](https://en.wikipedia.org/wiki/Kotlin_(programming_language))

- [Eiffel](https://en.wikipedia.org/wiki/Eiffel_(programming_language))





## Contract and aspect

Contract 更多是理念层、而aspect则是如何实现，在 "wikipedia [Design by contract](https://en.wikipedia.org/wiki/Design_by_contract) # Languages with third-party support" 中给出了一些通过AOP来实现contract的例子:

1、[JavaScript](https://en.wikipedia.org/wiki/JavaScript), via AspectJS

2、[Java](https://en.wikipedia.org/wiki/Java_(programming_language))

- [OVal](https://sebthom.github.io/oval/) with [AspectJ](https://en.wikipedia.org/wiki/AspectJ)

- JavaDbC using AspectJ



## 编写可以发现问题的代码

Design by contract能够帮助我们"编写可以发现问题的代码"，对于一些错误情况，程序员需要能够提前断言，然后将这些断言编码到程序中，当此断言成真，即程序进入到了这个分支，需要使用适当的方式通知，不同的编程语言提供了不同的通知方式：

1、python可以raise Exception

关于断言，参见：`Assertion` 章节。



