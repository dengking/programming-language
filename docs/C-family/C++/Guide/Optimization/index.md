# Optimization

一、无论是compiler还是programmer，都需要遵循optimization principle。因此，我们后续谈论optimization的时候，会从两个方面来进行描述:
1、compiler(实现层)执行的optimization

2、programmer执行的optimization

二、C++的一个发展方向: optimization。

本文对C++ optimization进行总结。

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



## Compiler type deduction

### reference

C++compiler的template argument，deduction使用reference是符合optimization原则的