# Lifetime

## Guide

本节是我对cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 总结、梳理，能够帮助读者快速地掌握cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 中的内容。

### Lifetime of an object

下面是以表格的形式展示的Lifetime of an object，相比于cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) ，它更加的清晰、直观。

| activity         | 说明                                                        | 章节                                    |
| ---------------- | ----------------------------------------------------------- | --------------------------------------- |
| allocation       | 为object分配内存区域                                        |                                         |
| initialization   | 初始化object<br>- 如果是OOP object，会调用合适的constructor | `C++\Language-reference\Initialization` |
| deinitialization | 反初始化object<br>- 如果是OOP object，会调用destructor      |                                         |
| deallocation     | 回收object的内存                                            |                                         |

需要注意的是：上面是按照发生顺序进行排列的，即：allocation->initialization->deinitialization->deallocation。

对于不同的object，它的上述四个activity发生的时间是不同的，后面会进行详细的说明。

### 内容简介

cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 的内容是比较杂乱的，因此有必要梳理清楚: 

| 主题                              | 注解                                                         |
| --------------------------------- | ------------------------------------------------------------ |
| Lifetime of object                | - [explicitly created object](https://en.cppreference.com/w/cpp/language/object#Object_creation) <br>- [implicitly create objects](https://en.cppreference.com/w/cpp/language/object#Object_creation) of implicit-lifetime types <br>- temporary object |
| Lifetime of reference             |                                                              |
| Lifetime of objects of class type | C++是支持OOP的，所以它需要对objects of class type的initialization、deinitialization进行准确的说明 |

通过上述表格可以看出，C++对各种可能的object的lifetime都进行了详细的定义。

| 主题                       | 注解                              |
| -------------------------- | --------------------------------- |
| Storage reuse              | 同一个storage，可以用于多个object |
| Access outside of lifetime |                                   |



## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)



### Lifetime of an explicitly created object

begin

### Lifetime of an implicitly created object

Some operations [implicitly create objects](https://en.cppreference.com/w/cpp/language/object#Object_creation) of implicit-lifetime types (see below) in given region of storage and start their lifetime.



### Lifetime of reference

The lifetime of a [reference](https://en.cppreference.com/w/cpp/language/reference) begins when its initialization is complete and ends as if it were a scalar object.

Note: the lifetime of the referred object may end before the end of the lifetime of the reference, which makes [dangling references](https://en.cppreference.com/w/cpp/language/reference#Dangling_references) possible.



### Lifetime of objects of class type

Lifetimes of non-static data members and base subobjects **begin** and **end** following [class initialization order](https://en.cppreference.com/w/cpp/language/initializer_list#Initialization_order).