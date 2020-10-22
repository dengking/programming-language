# 关于本章

本章讨论C++17引入的**Common vocabulary types**。

## What is vocabulary type?

"vocabulary" 的意思是 "词汇"，那"vocabulary type"的含义是什么呢？

### packtpub [Chapter 5. Vocabulary Types](https://subscription.packtpub.com/book/application_development/9781787126824/5)

A "vocabulary" type is a type that purports to provide a single *lingua franca*, a common language, for dealing with its domain.

> “词汇表”类型声称提供一种单一的*通用语*，一种通用语言，用于处理其域。



### stackoverflow [What are “vocabulary types”, and how many exist?](https://stackoverflow.com/questions/38471380/what-are-vocabulary-types-and-how-many-exist)

Across programming languages, I've encountered similar [composite types](https://en.wikipedia.org/wiki/Composite_data_type) with different names:

1. [Optional / Maybe](https://en.wikipedia.org/wiki/Option_type)
2. [Any](http://en.cppreference.com/w/cpp/experimental/any)
3. [Variant / Sum](https://en.wikipedia.org/wiki/Tagged_union)
4. [Record / Product](https://en.wikipedia.org/wiki/Product_type)

People often use the term **vocabulary type**, yet I've never seen a definition of what makes a type "vocabulary".

### foonathan [What should be part of the C++ standard library?](https://foonathan.net/2017/11/standard-library/) # 2. Vocabulary types and concepts

## How To Use Vocabulary Types from C++17?

bfilipek [How To Use Vocabulary Types from C++17, Presentation](https://www.bfilipek.com/2019/09/vocabulary-types-talk.html)



## C++ VS C

`std::any` type erasure，是对`void *`的增强。

`std::variant` 对`union`的增强。

