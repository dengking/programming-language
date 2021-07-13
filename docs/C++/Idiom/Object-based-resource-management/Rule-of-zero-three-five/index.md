# The rule of three/five/zero

一、首先要知道rule of three、five、zero分别是什么含义，指代的什么，然后考虑如何来高效的实现它们。

二、另外需要注意的是，首先需要指定Copyable and movable，然后进行实现，不能够死板的套用"Rule of three/five/zero"

## What is rule of three/five/zero？

一、Rule of three

cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three) # Rule of three

> If a class requires a user-defined [destructor](https://en.cppreference.com/w/cpp/language/destructor), a user-defined [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor), or a user-defined [copy assignment operator](https://en.cppreference.com/w/cpp/language/as_operator), it almost certainly requires all three.

支持copyable

二、Rule of five

cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three) # Rule of five

> Because the presence of a user-defined destructor, copy-constructor, or copy-assignment operator prevents implicit definition of the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) and the [move assignment operator](https://en.cppreference.com/w/cpp/language/move_operator), any class for which move semantics are desirable, has to declare all five special member functions

支持copyable and movable

三、rule of zero

feabhas [The Rule of Zero](https://blog.feabhas.com/2015/01/the-rule-of-zero/) 

> “The Rule of Zero” basically states:
>
> > *You should NEVER implement a destructor, copy constructor, move constructor or assignment operators in your code.*
>
> With the (very important) corollary to this:
>
> > *You should NEVER use a raw pointer to manage a resource.*

需要注意的是，是"NEVER implement a destructor, copy constructor, move constructor or assignment operators"，constructor还是要实现的。

### 从 "automatic/manual  resource management" 的角度来分析rule of three/five/zero

参见 florianwolters [The Rule of Zero](http://blog.florianwolters.de/educational/2015/01/31/The_Rule_of_Zero/) ，其中对此进行了非常深入的分析。

## How to implementation

1、C++11 move constructor=default constructor+swap

2、assignment operator=[copy move] constructor+swap

3、Rule of Four (and a half)

stackoverflow [Rule of Four (and a half)?](https://stackoverflow.com/questions/45754226/what-is-the-rule-of-four-and-a-half)

> I've also seen references to the [Rule of Four (and a half)](https://blog.feabhas.com/2015/01/the-rule-of-the-big-four-and-a-half-move-semantics-and-resource-management/), which is a combination of the Rule of Five and the copy-and-swap idiom.

#### Copy and swap idiom

参见 `Copy-and-swap-idiom` 章节。



## TODO

stackoverflow [Rule-of-Three becomes Rule-of-Five with C++11?](https://stackoverflow.com/questions/4782757/rule-of-three-becomes-rule-of-five-with-c11)
