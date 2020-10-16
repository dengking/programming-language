# User-defined static polymorphism 

C++除了语言内置的static polymorphism，还支持让用户自己建立起static polymorphism，这极大的增加了C++语言的灵活性。



## Dispatch based on constant

关于constant，参见`C++\Language-reference\Expressions\Constant-expressions`。

实现方式有:

1) enum dispatch : 参见`C++\Language-reference\Enum`的"Static dispatch"段



## Tag dispatch

参见`C++\Idiom\TMP\SFINAE-trait-enable-if\Tag-Dispatching`



## enum dispatch VS tag dispatch

enum是named constant，而tag是type。它们都可以用来实现static dispatch，但是实现的成本、方式确实不同的:

1) tag可以用作type parameter，而enum无法，它如果要用于template，则需要使用SFINAE

### TODO

Google c++ static polymorphism on enum values

fluentcpp [When to Use Enums and When to Use Tag Dispatching in C++](https://www.fluentcpp.com/2018/05/01/when-to-use-enums-and-when-to-use-tag-dispatching-in-cpp/)

stackoverflow [Polymorphic Enum in C++](https://stackoverflow.com/questions/3117462/polymorphic-enum-in-c)

