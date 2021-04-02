# Access outside of lifetime

1、要在object lifetime内使用它，如果不遵循这个原则，在object lifetime外使用它，就会导致memory error，因为C++ object对应的是memory，关于 memory error，参见`Resource-management\Memory-management\Memory-error`章节。

2、我将这类错误统一称为"Access outside of lifetime"，这是一种常见的memory error，它和 object lifetime相关的。它的另外一个说法是: "dangling"，比如dangling pointer、dangling reference。

3、以"Access outside of object lifetime"这个角度出发，可以组织很多内容。



## 原因和修改方法

### Indirection

C++支持iterator、pointer、reference等indirection，pointee object(被执行的object)的释放，pointer的如果仍然去访问，则就导致了access out of object lifetime的发生，它对应的memory error中的"Dangling-and-wild-pointer"，下面对上述几种example中的原因进行总结。



### 由于out of order导致 access outside of object lifetime 

关于C++ object lifetime，参见如下章节:

1、`Object\Lifetime-and-storage-duration`

2、`Classes\Object-lifetime`

3、`Object\Object-operation`

通过其中的内容可知，C++ object lifetime是按照ordering发生的。很多导致"Access outside of lifetime"的情况，其实都是由于out of order而导致的

典型的例子包括: 

1、object还没有构造好

2、object已经被释放了，仍然去访问



#### How to solve?

对于这种由于out of order而导致的access outside of object lifetime，我的解决思路自然而然就是: take control，make it computational and ordering。

### TODO: local object

local object也是比较如意导致access outside of object lifetime的。



## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) # [Access outside of lifetime](https://en.cppreference.com/w/cpp/language/lifetime#Access_outside_of_lifetime)

> NOTE: 这是一种常见的error，这种error是和lifetime相关的

Before the lifetime of an object has started but after the storage which the object will occupy has been allocated

> NOTE: "after the storage which the object will occupy has been allocated"的意思是: storage已经被分配了; "before the lifetime of an object has started"说明: initialization还没有完成

or,

after the lifetime of an object has ended and before the storage which the object occupied is reused or released, 

> NOTE: "before the storage which the object occupied is reused or released"说明: storage还没有给回收; "after the lifetime of an object has ended"说明: deinitialization已经完成了

the following uses of the glvalue expression that identifies that object are undefined:

> NOTE: 需要补充

During construction and destruction, other restrictions apply, see [virtual function calls during construction and destruction](https://en.cppreference.com/w/cpp/language/virtual#During_construction_and_destruction).

> NOTE: 
>
> 1、与此相关的一个idiom是: `OOP\Calling-Virtual-During-Initialization`
>
> 2、[dangling references](https://en.cppreference.com/w/cpp/language/reference#Dangling_references) 也是一种access outside of lifetime error，参见`C++\Language-reference\Reference\Reference`章节。

## Example

参见`Case-example`章节。

## TODO



geeksforgeeks [Dangling, Void , Null and Wild Pointers](https://www.geeksforgeeks.org/dangling-void-null-wild-pointers/)

