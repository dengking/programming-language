# Strong type

在阅读 fluentcpp [Strongly typed constructors](https://www.fluentcpp.com/2016/12/05/named-constructors/) 时，发现的这个主题。

## Fluentcpp strong type

https://www.fluentcpp.com/2016/12/08/strong-types-for-strong-interfaces/

https://www.fluentcpp.com/2017/08/08/metaclasses-ultimate-answer-strong-typing-c/



## arne-mertz.de [Use Stronger Types!](https://arne-mertz.de/2016/11/stronger-types/)



## Implementation

### Mixin from above

1、Variadic inheritance

2、CRTP mixin from above

Example: 

1、[joboccara](https://github.com/joboccara)/**[NamedType](https://github.com/joboccara/NamedType)**

参见 `C++\Expert\Fluentcpp\joboccara-NamedType` 章节。

### Mixin from below

1、parameter base class

2、CRTP

Example:

2、[hugoArregui](https://github.com/hugoArregui)/**[CPP_AOP-CRTP](https://github.com/hugoArregui/CPP_AOP-CRTP)**