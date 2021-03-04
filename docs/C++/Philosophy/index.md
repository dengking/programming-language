# Philosophy



## CppCoreGuidelines [Philosophy](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#p-philosophy)

### P.1: Express ideas directly in code

**Reason** Compilers don’t read comments (or design documents) and neither do many programmers (consistently). What is expressed in code has defined semantics and can (in principle) be checked by compilers and other tools.

> NOTE: programming language也是一种[语言](https://dengking.github.io/Post/Language/Language/)，我们使用它来描述万事万物；我们需要使用programming language提供的feature来完整地的“描述”我们的意图，而不是将意图隐式地在comments 、document、甚至口头 说明。
>
> 这一点与[zen of python](https://www.python.org/dev/peps/pep-0020/)的
>
> ```
> Explicit is better than implicit.
> ```
>
> 一致。
>
> 这一点和P.3: Express intent相关；



#### Example

```c++
change_speed(double s);  // bad: what does s signify?
// ...
change_speed(2.3);
```

```c++
change_speed(Speed s);  // better: the meaning of s is specified
// ...
change_speed(2.3);    // error: no unit
change_speed(23m / 10s); // meters per second
```



#### 补充

`concept` 在c++20中，programmer可以使用concept feature来formally描述对type的期望，这种方式能够让compiler来检查type，是典型的express ideas directly in code，关于此参见：

- cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)，本地路径: `C++\Named-requirements`

`std::chrono::seconds`

`std::unique_lock`、`std::unique_ptr`、`std::shared_lock`、`std::shared_ptr`



### P.3: Express intent

**Reason** Unless the intent of some code is stated (e.g., in names or comments), it is impossible to tell whether the code does what it is supposed to do.

> NOTE: 上述“stated”的含义是“说明、描述、表述”
>
> 在阅读维基百科[smart pointer](https://en.wikipedia.org/wiki/Smart_pointer)时，其中提及了smart pointer属于[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)，显然smart pointer是典型的express intent。

#### 补充

cast operator: C++提供了丰富的cast operator来让programmer准确的表达其cast intent，关于此，参见:

- `C++\Language-reference\Basic-concept\Type-system\Type-conversion\Cast-operator`



### P.4: Ideally, a program should be statically type safe

**Reason** Ideally, a program would be completely statically (compile-time) type safe. Unfortunately, that is
not possible. Problem areas:

- unions
- casts
- array decay
- range errors
- narrowing conversions

> NOTE: 上述这些问题，C++基于C努力地进行着改进，这部分能否在`C-family-language\C-and-C++\VS-C-VS-c++.md`中有描述。
>
> C++的meta-programming 技术，赋予了programmer非常强大的、灵活的对类型进行描述、检查的能力，一旦类型不符合预期，则立即给出编译报错，终止编译，典型特性包括：
>
> - concept
> - detection idiom
> - trait（一种reflection）
>
> 另外一個例子是：`static_assert`




