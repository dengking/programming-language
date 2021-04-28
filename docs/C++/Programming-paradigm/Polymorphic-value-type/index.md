# Polymorphic value type and concept based polymorphism

一、它是由Sean Parent提出，并大力推广的，参见 `Sean-Parent\Value-semantic-and-Concept-based-Polymorphism` 章节，后续为了描述方便，统一称为 "Polymorphic value type"

二、Polymorphic value type融合了OOP 和 GP的优势，这在 `Polymorphic-value-type-OOP-GP` 章节进行了介绍

## 标准化

open-std [A polymorphic value-type for `C++`](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0201r5.html)

## Implementation

Polymorphic value type的implementation主要使用type earsure technique，所以，参见 `Type-Erasure`。





### 一些library

[jbcoe](https://github.com/jbcoe)/**[polymorphic_value](https://github.com/jbcoe/polymorphic_value)**



### TODO `std::function` 其实是 Polymorphic value type 

TODO: 需要添加说明。