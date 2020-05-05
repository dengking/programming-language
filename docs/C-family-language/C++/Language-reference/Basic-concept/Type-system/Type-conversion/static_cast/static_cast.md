# static_cast



## 维基百科[static_cast](https://en.wikipedia.org/wiki/Static_cast)



## cppreference [static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)



## Example

维基百科[Curiously recurring template pattern#Polymorphic copy construction](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern#Polymorphic_copy_construction)

```c++
    std::unique_ptr<AbstractShape> clone() const override {
        return std::make_unique<Derived>(static_cast<Derived const&>(*this));
    }
```

将`this`转换为指向derived class的对象，如果缺少`const`，则会编译会报：

```
casts away qualifiers
```



[Question of using static_cast on “this” pointer in a derived object to base class](https://stackoverflow.com/questions/4543670/question-of-using-static-cast-on-this-pointer-in-a-derived-object-to-base-clas)