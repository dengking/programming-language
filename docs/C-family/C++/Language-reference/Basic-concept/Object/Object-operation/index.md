一、object operation:
common: 每一个operation都有对应的magic function

new、construct: 对应的是constructor

serialization-deserialization，或者说 获得value representation


copy: 对应的是copy constructor、assignment operator

可以copy from lvalue、copy from rvalue

因此包含四个 magic function

swap: 交换两个object的value

move 