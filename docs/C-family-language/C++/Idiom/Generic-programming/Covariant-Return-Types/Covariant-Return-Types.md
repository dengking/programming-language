# Covariant return type

在阅读fluentcpp [Polymorphic clones in modern C++](https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/)这篇文章的时候，其中提及了

> Notice that the return type of the `clone` method differ between the interface in the implementation. It is because C++ allows overriding a virtual method with one that has a different return type, provided this return type is a pointer (resp. reference) to a class convertible to the one pointed to (resp. referenced by) the return type of the base class. This is called **covariance**.

## More C++ Idioms [Covariant Return Types](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Covariant_Return_Types)



## 维基百科[Covariant return type](https://en.wikipedia.org/wiki/Covariant_return_type)



## [HOWTO: Covariant Return Types in C++](https://www.lwithers.me.uk/articles/covariant.html)

```c++
class Base {
public:
    virtual Base* clone() const
    {
        return new Base(this);
    }
};

class Derived : public Base {
public:
    virtual Base* clone() const
    {
        return new Derived(this);
    }
};
```

However, now consider a bit of code that knows it specifically has a `Derived` instance, and wishes to clone it. This code must retrieve a base class pointer and then dynamically **upcast** it（转换为`Derived` instance）. We could assume that the **upcast** always succeeds and not perform check, but this is bad practice (what if somebody changes the underlying function, or somebody copies and pastes the dynamic cast line?).

This leads to the following code:

```c++
    Derived* d = new Derived;
    Base* b = d->clone();
    Derived* d2 = dynamic_cast<Derived*>(b);
    if(!d2) {
        delete b;
        throw SomeCrazyException();
    }
```

As you can see, this is pretty long-winded given that we _know_ `d2` should be a `Derived` instance.



Fortunately, a language feature does exist to do this: covariant return types. If a derived class method returns a *more-derived* type than its overridden base class method, the derived class return type is said to be *covariant*.

```c++
class Base {
public:
    virtual Base* clone() const
    {
        return new Base(this);
    }
};

class Derived : public Base {
public:
    virtual Derived* clone() const
    {
        return new Derived(this);
    }
}
```

Of course, the return type does not have to be connected with the class in which the method resides. A perhaps illuminating example:

```c++
/* Inheritance hierarchies

         NetServer
             |
             ^
            / \
NetServerTCP   NetServerSCTP


         NetClient
             |
             ^
            / \
NetClientTCP   NetClientSCTP

*/

class NetServer {
public:
    virtual NetClient* acceptConnection() = 0;
};

class NetServerTCP : public NetServer {
public:
    virtual NetClientTCP* acceptConnection();
};

class NetServerSCTP : public NetServer {
public:
    virtual NetClientSCTP* acceptConnection();
};
```



## [When is C++ covariance the best solution?](https://stackoverflow.com/questions/1260757/when-is-c-covariance-the-best-solution)

> NOTE: 讨论的是，何时使用covariance

1. [Virtual constructor](https://stackoverflow.com/a/1260785)

2. https://stackoverflow.com/a/1261355
3. [Factory pattern](https://stackoverflow.com/a/1260934)



## [Covariance and contravariance in C++](https://quuxplusone.github.io/blog/2019/01/20/covariance-and-contravariance/)

这篇文章非常好