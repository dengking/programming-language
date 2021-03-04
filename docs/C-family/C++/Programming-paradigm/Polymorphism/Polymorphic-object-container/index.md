# Polymorphic object container



## Exception safety 问题 以及 解决方案

需要遵循 : CppCoreGuidelines [R.11: Avoid calling `new` and `delete` explicitly](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r11-avoid-calling-new-and-delete-explicitly) ，在下面文章中对此进行了详细说明: 

1、sean-parent [Value Semantics and Concept-based Polymorphism](https://sean-parent.stlab.cc/papers-and-presentations/#value-semantics-and-concept-based-polymorphism)

在其中进行了非常深入的探讨



2、[Can I have polymorphic containers with value semantics in C++?](https://stackoverflow.com/questions/41045/can-i-have-polymorphic-containers-with-value-semantics-in-c) # [A](https://stackoverflow.com/a/41059)

One reasonable solution is to store container safe smart pointers. I normally use `boost::shared_ptr` which is safe to store in a container. Note that `std::auto_ptr` is not.

```C++
vector<shared_ptr<Parent>> vec;
vec.push_back(shared_ptr<Parent>(new Child()));
```

`shared_ptr` uses reference counting so it will not delete the underlying instance until all references are removed.

3、[What's the preferred C++ idiom to own a collection of polymorphic objects?](https://stackoverflow.com/questions/17532076/whats-the-preferred-c-idiom-to-own-a-collection-of-polymorphic-objects) # [A](https://stackoverflow.com/a/17532810)

### 能否使用value semantic来进行解决？

这在 `Polymorphic-containers-with-value-semantic` 中进行了讨论。

### Smart pointer + container的方式

1、能够解决exception safety问题

2、需要注意的是，Smart pointer + container在实现上是依赖于move semantic的: 

a、performance principle: prefer move over copy

b、[如何评价 C++11 的右值引用（Rvalue reference）特性？ - Tinro的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/30801982) 

在这篇文章中，有着非常好的解答

## stackoverflow [What's the preferred C++ idiom to own a collection of polymorphic objects?](https://stackoverflow.com/questions/17532076/whats-the-preferred-c-idiom-to-own-a-collection-of-polymorphic-objects)



[A](https://stackoverflow.com/a/17532810)



Polymorphic objects have to be handled by pointer or reference. Since their lifetime is probably not bound to a particular scope they will also probably have dynamic storage duration, which means you should use a smart pointer.

Smart pointers such as `std::shared_ptr` and `std::unique_ptr` work just fine in the standard collection types.

```cpp
std::vector<std::unique_ptr<Base>>
```

Using this in `Owner` looks like:

```cpp
class Owner {
public:
    void do_all_stuff() {
        //iterate through all items and call do_stuff() on them
    }

    void add_item(std::unique_ptr<Base> item) {
        items.push_back(std::move(item));
    }

    vector<std::unique_ptr<Base>> items;
}
```

The argument type to `add_item` identifies the ownership policy required for adding an item, and requires the user to go out of their way to screw it up. For example they can't accidentally pass a raw pointer with some implicit, incompatible ownership semantics because `unique_ptr` has an explicit constructor.

`unique_ptr` will also take care of deleting the objects owned by `Owner`. Although you do need to ensure that `Base` has a virtual destructor. With your current definition you will get undefined behavior. Polymorphic objects should pretty much always have a virtual destructor.

[A](https://stackoverflow.com/a/17581078)

Other alternatives worth considering are to use `boost::ptr_container`, or even better, use a library like `adobe::poly` or `boost::type_erasure` for your polymorphic types, to exploit value-based run-time polymorphism—avoids the need for pointers, inheritance, etc.

