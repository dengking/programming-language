# The rule of three/five/zero



一、首先要知道rule of three、five、zero分别是什么含义，指代的什么，然后考虑如何来高效的实现它们。

二、另外需要注意的是，首先需要指定Copyable and movable，然后进行实现，不能够死板的套用"Rule of three/five/zero"

## What is rule of three/five/zero？

一、cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three) # Rule of three

If a class requires a user-defined [destructor](https://en.cppreference.com/w/cpp/language/destructor), a user-defined [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor), or a user-defined [copy assignment operator](https://en.cppreference.com/w/cpp/language/as_operator), it almost certainly requires all three.

> NOTE: 
>
> 支持copyable

二、cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three) # Rule of five

Because the presence of a user-defined destructor, copy-constructor, or copy-assignment operator prevents implicit definition of the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) and the [move assignment operator](https://en.cppreference.com/w/cpp/language/move_operator), any class for which move semantics are desirable, has to declare all five special member functions

> NOTE: 
>
> 支持copyable and movable



## How to implementation



### stackoverflow [Rule of Four (and a half)?](https://stackoverflow.com/questions/45754226/what-is-the-rule-of-four-and-a-half)



### Copy and swap idiom

