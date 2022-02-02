# Virtual Constructor/clone

1、本文讨论"How to copy an object that is accessible only by an interface that it implements?"

2、首先回答的一个问题是: 为什么C++不提供virtual constructor

2、然后回答，如何来实现walk-around

## 为什么C++不提供virtual constructor？

根本原因在于此时压根就没有object，因此无法实现dynamic  polymorphism。

### stackoverflow [Why do we not have a virtual constructor in C++?](https://stackoverflow.com/questions/733360/why-do-we-not-have-a-virtual-constructor-in-c)

Why does C++ not have a virtual constructor?

[A](https://stackoverflow.com/a/733382)

> NOTE: 这个回答是容易理解的

Virtual functions basically provide **polymorphic behavior**. That is, when you work with an object whose dynamic type is different than the static (compile time) type with which it is referred to, it provides behavior that is appropriate for the *actual* type of object instead of the static type of the object.

> NOTE: 这段话其实是在介绍**polymorphic behavior**的含义

Now try to apply that sort of behavior to a constructor. When you construct an object the **static type** is always the same as the actual object type since:

> To construct an object, a constructor needs the exact type of the object it is to create [...] Furthermore [...]you cannot have a **pointer to a constructor**

(Bjarne Stroustup (P424 The C++ Programming Language SE))

[A](https://stackoverflow.com/a/733393)

Hear it from the horse's mouth. :)

From Bjarne Stroustrup's C++ Style and Technique FAQ [Why don't we have virtual constructors?](http://www.stroustrup.com/bs_faq2.html#virtual-ctor)

> A virtual call is a mechanism to get work done given partial information. In particular, "virtual" allows us to call a function knowing only any interfaces and not the exact type of the object. To create an object you need complete information. In particular, you need to know the exact type of what you want to create. Consequently, a "call to a constructor" cannot be virtual.

The FAQ entry goes on to give the code for a way to achieve this end without a virtual constructor.

> NOTE: 原文中，虽然这个回答获赞较高，但是，我觉得上面的那个答案是更好的

[A](https://stackoverflow.com/a/733436)

> NOTE: 
>
> 1、这个回答也不错，它剖析的角度是从对比的角度来展开的: Python。Python中，"everything is an object"，因此class也是一个object，而Python的member function默认是virtual的，因此Python的constructor是virtual的，因此Python中就不需要abstract factory pattern

Unlike object oriented languages such as Smalltalk or Python, where the constructor is a virtual method of the object representing the class (which means you don't need the GoF [abstract factory pattern](http://c2.com/cgi/wiki?AbstractFactoryPattern), as you can pass the object representing the class around instead of making your own), C++ is a class based language, and does not have objects representing any of the language's constructs. The class does not exist as an object at runtime, so you can't call a virtual method on it.

This fits with the 'you don't pay for what you don't use' philosophy, though every large C++ project I've seen has ended up implementing some form of abstract factory or reflection.

### [LeetBook C++ 面试突击](https://leetcode-cn.com/leetbook/detail/cpp-interview-highlights/) # [构造函数、析构函数是否需要定义成虚函数？为什么？](https://leetcode-cn.com/leetbook/read/cpp-interview-highlights/ef0i4d/)

> NOTE: 
>
> 这个解释得也是比较好的
>
> 

**构造函数**一般不定义为**虚函数**，原因：

> NOTE: 

从存储空间的角度考虑：构造函数是在实例化对象的时候进行调用，如果此时将构造函数定义成虚函数，需要通过访问该对象所在的内存空间才能进行虚函数的调用（因为需要通过指向虚函数表的指针调用虚函数表，虽然虚函数表在编译时就有了，但是没有虚函数的指针，虚函数的指针只有在创建了对象才有），但是此时该对象还未创建，便无法进行虚函数的调用。所以构造函数不能定义成虚函数。

从使用的角度考虑：虚函数是基类的指针指向派生类的对象时，通过该指针实现对派生类的虚函数的调用，构造函数是在创建对象时自动调用的。

从实现上考虑：虚函数表是在创建对象之后才有的，因此不能定义成虚函数。

从类型上考虑：在创建对象时需要明确其类型。

**析构函数**一般定义成虚函数，原因：

析构函数定义成虚函数是为了防止内存泄漏，因为当基类的指针或者引用指向或绑定到派生类的对象时，如果未将基类的析构函数定义成虚函数，会调用基类的析构函数，那么只能将基类的成员所占的空间释放掉，派生类中特有的就会无法释放内存空间导致内存泄漏。





## fluentcpp [Polymorphic clones in modern C++](https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/)

How to copy an object that is accessible only by an interface that it implements?

### The classical problem

Let’s take the example of the following interface:

```C++
class Interface
{
public:
    virtual void doSomething() const = 0;
    virtual ~Interface() = default;
};
```

With one the classes implementing this interface:

```C++
class Implementation : public Interface
{
public:
    virtual void doSomething() const override
    {
        /* ... */
    }
};
```

How to make a copy of the `Implementation` object?

If you have access to the object itself, there is nothing easier:

```C++
Implementation x = // ...
Implementation y = x;
```

But the existence of the `Interface` suggests that there *are* polymorphic contexts where the object is accessible only via the interface:

```C++
Interface& x = // ...
Interface& y = ??
```

And there is a problem here because,  in C++, to construct an object we must spell out in the code the actual type of the object to be constructed (except in the case of implicit conversions). And here we don’t know what this type is. It could be `Implementation`, or any other class inheriting from `Interface`.

> NOTE: 
>
> 1、关于这段话的解释，参见 "为什么C++不提供virtual constructor？"段

And even if, for some reason, we knew for sure that it *was* an `Implementation`, the calling code may not have access to this class, which is one of the purposes of having an interface in the first place.

> NOTE: 
>
> 1、information hiding

What to do then?



### The classical solution

The classical solution is to “virtualize” the constructor, as Scott Meyers puts it. That is to say add a `clone` method in the interface, that delegates the object construction to the implementation itself. The interface then looks like:

> NOTE: 
> 1、delegate

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

This technique allows the desired copy, but exhibits another classical problem: the call site receives the responsibility to delete the cloned object, but nothing ensures that it will do it. Particularly if there is an early return or an exception thrown further down the code, the object has a risk to leak.



### A modern solution

> NOTE: 
>
> 1、其实就是使用resource return

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

Second, and much more annoyingly, the covariance doesn’t hold any more, because the `clone` method is no longer returning pointers. It now has to return an `std::unique_ptr<Interface>` in the interface AND in the implementation.



In the above case it doesn’t cause any practical problem, given that `Implementation` already depends on `Interface` anyway. But let’s consider the case where an implementation inherits from **several interfaces**. The solution without smart pointers scales effortlessly(轻松地；毫不费劲地) because the `clone` method is independent from the interface:

> NOTE: 翻译如下:
>
> "在上述情况下，它不会导致任何实际问题，因为实现已经依赖于接口。但是让我们考虑这样一种情况:一个实现继承了几个接口。没有智能指针的解决方案可以毫不费力地扩展，因为克隆方法独立于接口:"
>
> 



```c++
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

But with smart pointers, the situation is different: the `clone` method, bound to `Interface1`, cannot be used for `Interface2`! And since the `clone` method doesn’t take any argument, there is no way to add a new overload returning a unique_ptr to `Interface2`.

One solution that comes to mind is to use template methods. But there is no such such thing as a **template *virtual* method** so this solution is off the table.

> NOTE: 
>
> 1、关于 template virtual method，参见 "Virtual-method-template" 章节

Another idea would be to isolate the `clone` method in a `clonable` interface. But this would force the call site to `dynamic_cast` back and forth from the real interface to the clonable interface. Not good either.

### Clearing the ambiguity

The alternative I would suggest is to use **different names** for the clone methods in the interfaces.

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

But to be viable, this solution has to rely on a guideline for **interface designers**: if you choose to implement a clone method that returns a smart pointer, then **don’t call it just `clone`**.

Rather, use a specific name, like `cloneInterfaceX`, that won’t conflict with the copy functions coming from the other interfaces.

Now this is a solution for this particular problem, but there is a bigger C++ question behind this: **how to make smart pointers work with covariance**? You will have the answer on the next post, written by Raoul Borges who’s much more experienced than me on that question.



## When to use virtual clone?

如果一个class有一个成员，它的类型是pointer to polymorphic type object，那么这个polymorphic type就可能需要virtual constructor。

下面是一些例子:

1、stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques)

2、stackexchange [DeepPtr: a deep-copying unique_ptr wrapper in C++](https://codereview.stackexchange.com/questions/103744/deepptr-a-deep-copying-unique-ptr-wrapper-in-c)



## Virtual clone mixin

参见 `Generic-programming\CRTP` 章节。



## TO READ

http://www.cplusplus.com/forum/general/7288/

https://stackoverflow.com/questions/1021626/c-could-polymorphic-copy-constructors-work

stackoverflow [Copying a Polymorphic object in C++](https://stackoverflow.com/questions/5148706/copying-a-polymorphic-object-in-c)

cpppatterns [Virtual constructor](https://cpppatterns.com/patterns/virtual-constructor.html)

