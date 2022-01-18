# 关于本章

## 描述原则

在工程discrete的`Make-it-computational`章节，我们知道，实现"computational"的前提必要条件有:

1、Structuralization-and-formalization

2、Ordering

按照这个思路，我们在描述OOP lifetime的相关问题的时候，也是遵循这个思路的: 

1、Structuralization: 基于class hierarchy

2、Ordering: 需要梳理 order:

a、基于class hierarchy的order

b、the order of declaration of the members and the order of initialization、deinitialization

## 内容概述

在`C++\Language-reference\Basic-concept\Data-model\Object\Lifetime-and-storage-duration`章节中，我们介绍了object的lifetime大体流程，本章对OOP object的lifetime中的activity进行详细描述。

本章描述的重点是: 

| activity                     | 说明                                                         | magic function |
| ---------------------------- | ------------------------------------------------------------ | -------------- |
| allocation                   | 不描述                                                       |                |
| initialization/construction  | 详细描述思路: <br>- 基于class hierarchy来描述: initialization order<br>- member data initialization: <br>- initialization order | constructor    |
| deinitialization/destruction | 详细描述:<br>- TODO                                          | destructor     |
| deallocation                 | 不描述                                                       |                |

