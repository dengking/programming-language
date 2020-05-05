# Virtual destructor



## When should my destructor be `virtual`?

### stackoverflow [When to use virtual destructors?](https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors)

Virtual destructors are useful when you can delete an instance of a derived class through a pointer to **base class**:

```cpp
class Base 
{
    // some virtual methods
};

class Derived : public Base
{
    ~Derived()
    {
        // Do some important cleanup
    }
};
```

Here, you'll notice that I didn't declare Base's destructor to be `virtual`. Now, let's have a look at the following snippet:

```cpp
Base *b = new Derived();
// use b
delete b; // Here's the problem!
```

Since Base's destructor is not `virtual` and `b` is a `Base*` pointing to a `Derived` object, `delete b`has [undefined behaviour](https://stackoverflow.com/q/2397984/20984):

> [In `delete b`], if the **static type** of the object to be deleted is different from its **dynamic type**, the **static type** shall be a base class of the dynamic type of the object to be deleted and **the static type shall have a virtual destructor or the behavior is undefined**.

总结：

以下是我测试的代码，发现在这种情况下，确实如上所说，是不会触发dynamic dispatch，即不会执行`Derived`的析构函数，而只会执行基类的构造函数。

```cpp
#include <iostream>
class Base
{
public://必须要加上public，否则默认情况下，是private
    // some virtual methods
    ~Base()
    {
        // Do some important cleanup
        std::cout<<"Base destructor"<<std::endl;
    }
};

class Derived : public Base
{
    ~Derived()
    {
        // Do some important cleanup
        std::cout<<"derived destructor"<<std::endl;
    }
};

int main()
{
    Base *b = new Derived();
    // use b
    delete b; // Here's the problem!
}
```

In most implementations, the call to the destructor will be resolved like any **non-virtual** code, meaning that the destructor of the base class will be called but not the one of the derived class, resulting in a resources leak.

To sum up, always make base classes' destructors `virtual` when they're meant to be manipulated polymorphically.

If you want to prevent the deletion of an instance through a **base class pointer**, you can make the base class destructor **protected and nonvirtual**; by doing so, the compiler won't let you call `delete`on a base class pointer.

总结：这种通过使用编译器来保证正确性的做法是非常值的借鉴的，但是这里为什么`protected`而不是`private`呢？因为我在写上述代码的时候，第一次的确是将其声明为`private`了。其实这里就涉及到了c++中的destructor，代码如下：

```cpp
#include <iostream>
class Base
{
    // some virtual methods
//public:
    virtual ~Base()
    {
        std::cout<<"base destructor"<<std::endl;
    }
};

class Derived : public Base
{
     ~Derived()
    {
        // Do some important cleanup
        std::cout<<"derived destructor"<<std::endl;
    }
};

int main()
{
    //Base *b = new Derived();
    // use b
    //delete b; // Here's the problem!
}
```

上面这段代码一旦编译就会报错。

You can learn more about virtuality and virtual base class destructor in [this article from Herb Sutter](http://www.gotw.ca/publications/mill18.htm).

### isocpp When should my destructor be `virtual`? [¶](https://isocpp.org/wiki/faq/virtual-functions#virtual-dtors) [Δ](https://isocpp.org/wiki/faq/virtual-functions#)



## Why are destructors not `virtual` by default?

### isocpp Why are destructors not `virtual` by default? [¶](https://isocpp.org/wiki/faq/virtual-functions#virtual-dtor-rationale) [Δ](https://isocpp.org/wiki/faq/virtual-functions#)