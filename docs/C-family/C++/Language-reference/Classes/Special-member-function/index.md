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
| destroy         |                               |



### Create

对应的magic function是constructor，在`Constructor`章节进行描述。



### Copy

copy: 可以copy from lvalue、copy from rvalue

对应的magic function是: copy constructor、assignment operator

对应的magic function是: lvalue copy constructor、rvalue copy constructor; lvalue assignment operator、rvalue assignment operator;

因此包含四个 magic function。

### Swap

交换两个object的value

### Move

C++ 并不直接move，更多的还是swap

### Destroy

对应的magic function是destructor，在`Destructor`章节进行描述。

## Common issue

C++ magic function的一些Common issue:

1、template

2、virtual

3、delete

4、default

## Rule of zero、three、five

参见 `The-rule-of-zero-three-five` 章节。