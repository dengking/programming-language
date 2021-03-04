# 关于本章

本章总结C++中关于temporary的内容。

## Temporary object lifetime

在cppreference的如下章节中，描述了lifetime of temporary: 

1) cppreference [Lifetime#Temporary object lifetime](https://en.cppreference.com/w/cpp/language/lifetime#Temporary_object_lifetime)

2) cppreference  [Reference initialization#Lifetime of a temporary](https://en.cppreference.com/w/cpp/language/reference_initialization#Lifetime_of_a_temporary)



## Temporary materialization

在`C++\What-is-new-in-C++\C++17\Temporary-materialization`章节对它进行了详细说明。

## Idiom using C++ temporary

在C++中，有很多idiom都使用temporary，这在`C++\Idiom\OOP\index.md#Idiom concerning temporary/`中进行了总结。



## Not use unmanned temporary

这是我在阅读 [Differences between std::make_unique and std::unique_ptr with new](https://stackoverflow.com/questions/22571202/differences-between-stdmake-unique-and-stdunique-ptr-with-new) # [A](https://stackoverflow.com/a/22571331) 时，其中提及的:

> `make_unique` is safe for creating temporaries, whereas with explicit use of `new` you have to remember the rule about not using unnamed temporaries.