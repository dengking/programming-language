# Strong exception safety

无论是`std::uniqure_ptr`还是`std::shared_ptr`都涉及dynamic memory allocation，因此需要考虑resource leakage、exception safety的问题，programmer在使用的时候，需要保证strong exception safety。那如何来保证呢？这是本章探讨的话题。

## CppCoreGuidelines [R: Resource management](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-resource)

其中给出了很多的guidelines，参见 `CppCoreGuidelines-Resource-Management` 章节。

## 具体的guideline

本章的后续章节会给出实现strong exception safety的一些guideline:

1、Prefer-use-object-generator

2、Prefer-not-use-reset


