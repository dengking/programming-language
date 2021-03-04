# Object operation and special member function、magic function



## Object operation and magic function

在 `Object-operation` 章节做了如下总结:

| object lifetime | operation                     |
| --------------- | ----------------------------- |
| create          |                               |
|                 | refer to                      |
|                 | access                        |
|                 | stored in arrays              |
|                 | copy                          |
|                 | assign                        |
|                 | move                          |
|                 | swap                          |
|                 | serialization-deserialization |
|                 | ......                        |
| destroy         |                               |

1、copy、swap是基础操作，能够用于实现assign

2、move的实现很多都依赖于swap

### Create

1、对应的magic function是constructor

参见如下章节:

1、`Constructor`



### Copy

1、copy: 可以copy from lvalue、copy from rvalue

2、对应的magic function是: copy constructor、assignment operator

3、对应的magic function是: lvalue copy constructor、rvalue copy constructor; lvalue assignment operator、rvalue assignment operator;

因此包含四个 magic function。

参见如下章节:

1、`Copy` 章节



### Swap

交换两个object的value。



### Move

1、C++ 并不直接move

2、C++ move很多是基于swap而实现的

参见如下章节:

1、`Move`章节



### Destroy

1、对应的magic function是destructor

参见如下章节:

1、在`Destructor`章节



## Rule of zero、three、five

参见 `The-rule-of-zero-three-five` 章节。