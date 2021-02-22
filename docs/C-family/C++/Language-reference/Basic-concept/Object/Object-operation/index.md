# Object operation

1、object是C++ first class citizen

2、(几乎) 每一个operation都有对应的magic function

## Object operation and magic function

### draft

object operation包含了object lifetime

下面总结了对object的manipulation、operation:

- create
- destroy
- refer to
- access
- stored in arrays
- copied
- assigned
- ......

### draft

1、new、construct: 对应的是constructor

2、serialization-deserialization，或者说 获得value representation

3、copy: 可以copy from lvalue、copy from rvalue

对应的是: copy constructor、assignment operator

对应的是: lvalue copy constructor、rvalue copy constructor; lvalue assignment operator、rvalue assignment operator;

因此包含四个 magic function

4、swap: 交换两个object的value

5、move: C++ 并不直接move，更多的还是swap