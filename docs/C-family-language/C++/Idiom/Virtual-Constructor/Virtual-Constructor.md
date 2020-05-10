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

### A modern solution

```c++
class Interface
{
public:
    virtual std::unique_ptr<Interface> clone() const = 0;
 
    virtual void doSomething() const = 0;
    virtual ~Interface() = default;
};
 
class Implementation : public Interface
{
public:
    virtual std::unique_ptr<Interface> clone() const override
    {
        return std::make_unique<Implementation>(*this);
    }
 
    virtual void doSomething() const override
    {
        /* ... */
    }
};
int main()
{
Interface& x = // ...
std::unique_ptr<Interface> y = x.clone();
}
```



First, your compiler may not have `std::make_unique` since it arrived in C++14 while `std::unique_ptr` only came in C++11 (I believe this was just an oversight). If so, you can use this implementation proposed by [cppreference.com](http://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique):

```c++
// note: this implementation does not disable this overload for array types
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```



```c++

1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
class Interface1
{
public:
    virtual Interface1* clone() const = 0;
    virtual void doSomething() const = 0;
    virtual ~Interface1() = default;
};
 
class Interface2
{
public:
    virtual Interface2* clone() const = 0;
    virtual void doSomethingElse() const = 0;
    virtual ~Interface2() = default;
};
 
class Implementation : public Interface1, public Interface2
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
    virtual void doSomethingElse() const override
    {
        /* ... */
    }
};
```

### Clearing the ambiguity

```c++
class Interface1
{
public:
    virtual std::unique_ptr<Interface1> cloneInterface1() const = 0;
    virtual void doSomething() const = 0;
    virtual ~Interface1() = default;
};
 
class Interface2
{
public:
    virtual std::unique_ptr<Interface2> cloneInterface2() const = 0;
    virtual void doSomethingElse() const = 0;
    virtual ~Interface2() = default;
};
 
class Implementation : public Interface1, public Interface2
{
public:
    virtual std::unique_ptr<Interface1> cloneInterface1() const override
    {
        return make_unique<Implementation>(*this);
    }
    virtual std::unique_ptr<Interface2> cloneInterface2() const override
    {
        return make_unique<Implementation>(*this);
    }
    virtual void doSomething() const override
    {
        
    }
    virtual void doSomethingElse() const override
    {
        
    }
};
```



## TO READ

http://www.cplusplus.com/forum/general/7288/

https://stackoverflow.com/questions/1021626/c-could-polymorphic-copy-constructors-work