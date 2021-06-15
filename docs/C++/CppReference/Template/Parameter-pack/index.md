# Parameter pack

本文中的FP的含义是functional programming；

## 概述

一、Parameter pack是linear structure



二、对Parameter pack使用FP的方式来进行操作，最最常见的是recursion(参见`Recursive-variadic-function`)。



三、parameter pack大大提高了C++ generic programming，使得如下成为可能: 

1、`std::tuple`

最能体现parameter pack意义的是: `std::tuple`

2、variadic class template

参见 `Variadic-inheritance`章节

3、variadic function template

4、idiom

"parameter pack + forwarding reference" 使得programmer能够写出非常generic的程序。



四、parameter pack大大提高了C++ compile time computation

典型的就是`Recursive-variadic-function`。



五、下面是使用了Parameter pack的一些案例:

1、`Variadic-CRTP`

2、`Variadic-inheritance`

3、`Variadic-mixin`

4、`Variadic-template-VS-ellipsis`

## 发展概述

### C++11

引入Variadic templates，参见: 

1) Wikipedia [C++11#Variadic templates](https://infogalactic.com/info/C%2B%2B11#Variadic_templates) 

2) cppreference [C++11](https://en.cppreference.com/w/cpp/11)# [variadic templates](https://en.cppreference.com/w/cpp/language/parameter_pack)

不足: 

不能够直接支持FP apply/reduce，需要通过recursion来实现；

### C++14

[C++14](https://en.cppreference.com/w/cpp/14) 新增 [generic lambdas](https://en.cppreference.com/w/cpp/language/lambda) 特性，可以将variadic template用于lambda expression，参见`C++\Language-reference\Functions\Lambda-expression`章节。

### C++17

新增特性: [fold expression(since C++17)](https://en.cppreference.com/w/cpp/language/fold)，直接支持FP apply/reduce，目的是simplify C++。 

参见:

1) Wikipedia [C++17#Folding expressions](https://infogalactic.com/info/C%2B%2B17)

