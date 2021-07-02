# Performance  optimization

一、无论是compiler还是programmer，都需要遵循optimization principle。因此，我们后续谈论optimization的时候，会从两个方面来进行描述:

1、compiler(实现层)执行的optimization

2、programmer执行的optimization

二、C++的一个发展方向: performance optimization。

本文对C++ Performance  optimization进行总结。

## Optimization的方向/how to optimize

C++中的很多Optimization technique和`Software-analysis\Performance-optimization`总结的各种Optimization technique是一脉相承的。

## Avoid copy

Performance optimization中，"avoid copy"是非常重要的一种优化思路，C++无论是在实现(compiler)还是在language层，都贯穿了这个思路，本文将对C++ avoid copy进行总结，下面将分两个部分来进行总结。



### Avoid copy optimization in compiler 

本节总结compiler对Avoid copy optimization 技术的运用。

#### Copy elision

参见 `C-family\Common\From-source-code-to-exec\Compile\Optimization\Copy-elision` 章节。

#### RVO

参见 "RVO" 章节。



### 语言层的optimization

需要由programmer来完成。

#### Emplace/construct in place

C++ emplace可以作为一个例子，另外它的实现还依赖于placement new。参见`C++\Library\Standard-library\STL\Containers-library\Common-issue\Emplace`章节。

#### View/non-owning type

1、`string_view`

参见 `String\C++string\basic_string_view` 章节

2、`span`

参见 `C++\Library\Standard-library\STL\Containers-library\View\span` 章节

#### Move semantic

参见 `C++\Language-reference\Reference\Move-semantic` 章节。

#### `std::string` VS  `const char *` VS `std::string_view`

`std::string` 是典型的deep copy。

`const char *` 和 `std::string_view` 是典型的non-copy



## Memory optimization

### Placement new

参见 `Resource-management\Memory-management\Allocation-and-deallocation\C++\new-operator\Placement-new` 章节。



### SSO



## Compiler type deduction

### reference

C++compiler的template argument，deduction使用reference是符合optimization原则的





## Programmers give hint to compiler

本节标题的含义是: 给编译器提示；虽然当今compiler比较强大，但是毕竟它不是万能的，作为programmer，我们可以给予它一些hint，让能够满足我们的目的: 

1) 让compiler能够更好地优化code

关于此的例子有:

`inline`

`likely` 和 `unlikely`

2) 让compiler实现static polymorphism

含义是: 让compiler选择(statically)正确的实现

例子:

1 multiple dispatch，参见

- `Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Behavioral-pattern\Visitor-pattern`
- `C++\Pattern\Visitor-pattern`

2 primary template 和 specialization

当两者处于不同header file的时候，在使用的时候，需要将它们都包含到同一个translation unit中才能够生效。

3 CRTR都是give hint to compiler

它都是generic programming中的技巧，参见 `Generic-programming\Curiously-recurring-template-pattern` 章节。

4 tag dispatch



## Concurrent/parallel computing

参见

1、`C++\Language-reference\Basic-concept\Abstract-machine\Memory-model`

2、`C++\Library\Standard-library\Thread`

3、C++17 execution policy

## Compile-time computation

参见`C-and-C++\Compile-time-and-run-time\Compile-time-computation`章节。

