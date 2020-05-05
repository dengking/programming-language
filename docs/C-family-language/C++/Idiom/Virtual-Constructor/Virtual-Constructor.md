# Virtual Constructor

How to copy an object that is accessible only by an interface that it implements?



## fluentcpp [Polymorphic clones in modern C++](https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/)

### The classical problem

And there is a problem here because,  in C++, to construct an object we must spell out in the code the actual type of the object to be constructed (except in the case of implicit conversions). And here we don’t know what this type is. It could be `Implementation`, or any other class inheriting from `Interface`.

> NOTE: 原文的这段话非常重要



### The classical solution

The classical solution is to “virtualize” the constructor, as Scott Meyers puts it. That is to say add a `clone` method in the interface, that delegates the object construction to the implementation itself. The interface then looks like:

```c++
class Interface
{
public:
    virtual Interface* clone() const = 0;
 
    virtual void doSomething() const = 0;
    virtual ~Interface() = default;
};


class Implementation : public Interface
{
public:
    virtual Implementation* clone() const override
    {
        return new Implementation(*this);
    }
 
    virtual void doSomething() const override
    {
        /* ... */
    }
};

int main()
{
	Interface* x = new Implementation();// 
	Interface* y = x->clone();
    delete x;
    delete y;
}
```



Notice that the return type of the `clone` method differ between the interface in the implementation. It is because C++ allows overriding a virtual method with one that has a different return type, provided this return type is a pointer (resp. reference) to a class convertible to the one pointed to (resp. referenced by) the return type of the base class. This is called **covariance**.

> NOTE: "covariance"概念非常重要。