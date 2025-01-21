# Mixin

本文讨论C++中的mixin。

## 为什么需要mixin?

在 [More C++ Idioms Parameterized Base Class](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Parameterized_Base_Class) 中将mixin和AOP进行联系，将AOP中的**cross point concern**使用mixin class来进行实现，mixin最大的好处就是code reuse: 

a. mixin常见功能、behavior

对于一些常见功能，比如factory、singleton、clone等**cross point concern**，它们的实现模式是非常类似的，为了促进code reuse，使用mixin来将这些功能/概念给予给子类;

需要注意的是，虽然mixin的实现是依赖于inheritance，但是它们并不符合**is-a**关系，而是AOP中的**cross point concern**;

b. Requiring-or-Prohibiting

参见"Requiring-or-Prohibiting"相关章节；



## 实现方式

Mixin from above: CRTR + inheritance

Mixin from below: Parameterized Base Class

两种实现方式对比: 

1. 上述两种实现方式，都需要使用inheritance。

2. 在文章 fluentcpp [Mixin Classes: The Yang of the CRTP](https://www.fluentcpp.com/2017/12/12/mixin-classes-yang-crtp/) 中，对两者有一定的对比，下面是一个非常形象的图: 

![Mixin classes CRTP](https://www.fluentcpp.com/wp-content/uploads/2017/12/mixin_classes.jpg)

标题中的"Yang"的意思是: 阳，即太极的"阳"，阅读完原文的内容后可知: 在本文中，它的含义是: mixin classes，是CRTP的反面。其实就是mixin from below。并且原文中的配图，也是非常形象的:

> 
>
> ### So, CRTP or mixin class?
>
> CRTP and mixin classes provide two approaches to the same problem: adding a generic functionality to an existing class, but with **different trade-offs**.
>
> Here are the points where they differ:
>
> The **CRTP**:
>
> - impacts the definition of the existing class, because it has to inherit from the CRTP,
> - client code uses the original class directly and benefits from its augmented functionalities.
>
> The **mixin class**:
>
> - leaves the original class unchanged,
> - client code doesn’t use the original class directly, it needs to wrap it into the mixin to use the augmented functionality,
> - inherits from a the original class even if it doesn’t have a virtual destructor. This is ok unless the mixin class is deleted polymorphically through a pointer to the original class.
>
> Understanding these trade-off lets you choose the solution that fits best to a given situation.
>
> There is more than that to the CRTP. If you want to know more about it, I’ve dedicated a whole [series of posts to the CRTP](https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/), which has become quite popular.
>
> Special thanks to Arthur for his [talk](https://www.youtube.com/watch?v=vwrXHznaYLA), and also for taking the time to help me understand mixin classes.



需要注意的是: 两种实现方式互为补充，没有优劣之分，需要根据需求，选择合适的实现方式:

> Now that we’re clear on [how the CRTP works](https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/), let me share with you another technique involving templates that is complementary(补充) to the CRTP: **Mixin classes**.

## Mixin from below: Parameterized Base Class

### [More C++ Idioms Parameterized Base Class](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Parameterized_Base_Class) 

#### Also Known As

1、Mixin-from-below

2、Parameterized Inheritance

#### Motivation

A certain aspect can be abstracted out from requirements and be developed as templates (e.g., object serialization). Serialization is a **cross-cutting concern** that many classes/POD types in an application may have. Such a concern can be abstracted out in a manageable reusable module. By addition of an **aspect**, substitutability(可替换性) with the original type is not broken so another motivation is to have a IS-A (public inheritance) or WAS-A (private inheritance) relationship with the type parameter.

> NOTE: **cross-cutting concern**即横切关注点，它是[aspect oriented programming](https://en.wikipedia.org/wiki/Aspect-oriented_programming)中的概念



#### Solution and Sample Code

```c++
template<class T>
class Serializable : public T, /// Parameterized Base Class Idiom
                     public ISerializable {
public:
    Serializable(const T &t = T()) : T(t) {}
    virtual int serialize(char *&buffer, size_t &buf_size) const {
        const size_t size = sizeof(T);
        if (size > buf_size)
            throw std::runtime_error("Insufficient memory!");

        memcpy(buffer, static_cast<const T *>(this), size);
        buffer += size;
        buf_size -= size;
        return size;
    }
};

```

`Serializable <T>` can be used polymorphically as a `T` as well as a `ISerializable`. Above example works correctly only if `T` is a user-defined POD type without pointers.



## Application&example





### Repeated print

[fluentcpp Mixin Classes: The Yang of the CRTP](https://www.fluentcpp.com/2017/12/12/mixin-classes-yang-crtp/)  # Plugging a generic functionality over your type

> NOTE: 将通用的功能插入到你的类型中，这个例子所展示的是将重复打印的功能混入到已有的class中

The main usage of the CRTP is to [add a generic functionality](https://www.fluentcpp.com/2017/05/16/what-the-crtp-brings-to-code/) to a particular class. Mixin classes do that too.

#### Mixin classes

Mixin classes are **template classes** that define a generic behaviour, and are designed to **inherit** from the type you wish to plug(插入) their functionality onto.

> NOTE: mixin class需要继承你希望被插入功能的类

Here is an example. Let’s take a class representing the name of a person. It has a **first name** and a **last name**, and it can print out that name with a specific format:

```C++
class Name {
public:
    Name(std::string firstName, std::string lastName) :
        firstName_(std::move(firstName)), lastName_(std::move(lastName)) {}

    void print() const { std::cout << lastName_ << ", " << firstName_ << '\n'; }

private:
    std::string firstName_;
    std::string lastName_;
};

```

We could add a `repeat` method to the `Name` class. But the concept of repeatedly call the `print` method is something that could apply to other classes, like a `PhoneNumber` class that could also have a `print()` method.

The idea of the mixin class is to isolate the generic functionality into its own class, template this class on the type we want to plug in onto, and derive from that type:

```C++
template<typename Printable>
struct RepeatPrint : Printable {
    explicit RepeatPrint(Printable const &printable) : Printable(printable) {}
    void repeat(unsigned int n) const {
        while (n-- > 0) {
            this->print();
        }
    }
};

```

> NOTE: 
>
> 显然，mixin class `RepeatPrint` 是一个独立的类型，它的基类是模板参数 `Printable`，它需要继承你希望被插入功能的类`Printable` ，并且它还以基类对象 `printable` 作为入参数传入mixin class `RepeatPrint` 中
>
> `this->print()` 中加上 `this` 的目的是实现dependent name lookup; 

To avoid specifying template arguments explicitly we use a function that deduces them:

```c++
template<typename Printable>
RepeatPrint<Printable> repeatPrint(Printable const &printable) {
    return RepeatPrint<Printable>(printable);
}
```

> NOTE: 这是object generator idiom

##### 完整代码

```c++
#include <iostream>
#include <string>

class Name {
public:
    Name(std::string firstName, std::string lastName) :
        firstName_(std::move(firstName)), lastName_(std::move(lastName)) {}

    void print() const { std::cout << lastName_ << ", " << firstName_ << '\n'; }

private:
    std::string firstName_;
    std::string lastName_;
};


template<typename Printable>
struct RepeatPrint : Printable {
    explicit RepeatPrint(Printable const &printable) : Printable(printable) {}
    void repeat(unsigned int n) const {
        while (n-- > 0) {
            this->print();
        }
    }
};


template<typename Printable>
RepeatPrint<Printable> repeatPrint(Printable const &printable) {
    return RepeatPrint<Printable>(printable);
}

int main() {
    Name ned("Eddard", "Stark");
    repeatPrint(ned).repeat(10); // 通过object generator的方式调用非常自然
}

```

#### CRTP



##### 完整代码

```c++
#include <iostream>
#include <string>

template<typename Printable>
struct RepeatPrint {
    void repeat(unsigned int n) const {
        while (n-- > 0) {
            static_cast<Printable const &>(*this).print();
        }
    }
};

class Name : public RepeatPrint<Name> {
public:
    Name(std::string firstName, std::string lastName) :
        firstName_(std::move(firstName)), lastName_(std::move(lastName)) {}

    void print() const { std::cout << lastName_ << ", " << firstName_ << '\n'; }

private:
    std::string firstName_;
    std::string lastName_;
};

int main() {
    Name ned("Eddard", "Stark");
    ned.repeat(10);
}

```

#### 实现对比

两种实现方式都使用了 `Printable` concept，两种实现方式都将concept作为模板参数。



### Singleton mixin



`C++\Pattern\Singleton\Singleton-mixin`



### Factory mixin

`C++\Pattern\Factory`





## Variadic multiple inheritance 

"Parameterized Base Class"其实采用的"template base class"用法，在C++ idiom的世界中，它特指的是一种实现 "Mixin-from-below" 的方法；

与它类似的是: "Variadic multiple inheritance"其实采用的也是"template base class"用法。

