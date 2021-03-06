# Object operation and special member function/magic function and concept

1、object operation、magic function、concept之间有着对应关系，本文对此进行总结。

2、其实在cppreference [Named Requirements](https://en.cppreference.com/w/cpp/named_req)中，已经对此进行了总结，参见 `Named-requirements` 章节。

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

## cppreference [Special member functions](https://en.cppreference.com/w/cpp/language/classes)

> NOTE: 
>
> 下面是 cppreference 中总结的special member function

| Special member functions                                     |
| ------------------------------------------------------------ |
| [Default constructor](https://en.cppreference.com/w/cpp/language/default_constructor) |
| [Copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor) |
| [Move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) (C++11) |
| [Copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment) |
| [Move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment) (C++11) |
| [Destructor](https://en.cppreference.com/w/cpp/language/destructor) |



## Concepts

可以看到，上述每种operation

### cppreference [Named Requirements](https://en.cppreference.com/w/cpp/named_req)

| Basic                                                        |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [DefaultConstructible](https://en.cppreference.com/w/cpp/named_req/DefaultConstructible) | specifies that an object of the type can be default constructed (named requirement) |
| [MoveConstructible](https://en.cppreference.com/w/cpp/named_req/MoveConstructible)(C++11) | specifies that an object of the type can be constructed from rvalue (named requirement) |
| [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) | specifies that an object of the type can be constructed from lvalue (named requirement) |
| [MoveAssignable](https://en.cppreference.com/w/cpp/named_req/MoveAssignable)(C++11) | specifies that an object of the type can be assigned from rvalue (named requirement) |
| [CopyAssignable](https://en.cppreference.com/w/cpp/named_req/CopyAssignable) | specifies that an object of the type can be assigned from lvalue (named requirement) |
| [Destructible](https://en.cppreference.com/w/cpp/named_req/Destructible) | specifies that an object of the type can be destroyed        |

关于 named requirement，参见 `Named-requirements` 章节。

