# 关于本章

在`C++\Language-reference\Basic-concept\Data-model\Object\Object-lifetime-and-storage-duration`章节中，我们介绍了object的lifetime大体流程，本章对OOP object的lifetime中的activity进行详细描述。

本章描述的重点是: 

| activity         | 说明                                                         |
| ---------------- | ------------------------------------------------------------ |
| allocation       | 不描述                                                       |
| initialization   | 详细描述: <br>- 基于class hierarchy来描述: initialization order<br>- member data initialization: <br>- - initialization order |
| deinitialization | 详细描述:                                                    |
| deallocation     | 不描述                                                       |

除了描述这些细节，我们还描述与此相关的问题。

## TODO

### the order of declaration of the members in c++ and the order of destruction

[Order of member constructor and destructor calls  ](https://stackoverflow.com/questions/2254263/order-of-member-constructor-and-destructor-calls  )