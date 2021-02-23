# Object operation

1、object是C++ first class citizen

2、(几乎) 每一个operation都有对应的magic function

## Object operation and magic function



1、object operation包含了对object lifetime的控制

2、下面总结了对object的manipulation、operation:

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
| destroy         |                               |

### Serialization deserialization

获得: 

1、value representation

2、object representation

### 其他

object operation 和 magic function之间的对应关系主要涉及OOP，因此将这部分内容放到了`Classes`章节。

