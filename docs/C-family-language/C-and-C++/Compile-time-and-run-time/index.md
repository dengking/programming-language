# Compile-time and run-time

## c++

### [`static_cast`](https://en.cppreference.com/w/cpp/language/static_cast) VS [`dynamic_cast`](https://en.cppreference.com/w/cpp/language/dynamic_cast) 

`static_cast` 是 compile-time

`dynamic_cast` 是 run-time

### [`static_assert`](https://en.cppreference.com/w/cpp/language/static_assert) VS [`assert`](https://en.cppreference.com/w/cpp/error/assert) 

`static_assert` 是compile-time

`assert` 是 run-time

### static array VS dynamic array

在 https://stackoverflow.com/a/4810676 中提及了；

### compile-time concept

下面总结了c++中的compile-time concept。

#### constant expression

关于constant expression，参见`C-family-language\C++\Language-reference\Expressions\Operators\Other\Constant-expressions`章节。

#### template

关于template，参见`C-family-language\C++\Language-reference\Template`章节。

#### enum

参见`C++\Language-reference\Enum`章节。

### Compile-time context

下面是总结的（部分）compile-time context：

- template-meta programming
- array sizes，参见[Array declaration](https://en.cppreference.com/w/cpp/language/array)
- [switch statement](https://en.cppreference.com/w/cpp/language/switch)



### static 多态 VS runtime多态

参见`C++\Guide\Implementation-of-polymorphism\index.md`章节。



### static type and dynamic type

参见`C-family-language\C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system.md`。



### run-time concept

`virtual`