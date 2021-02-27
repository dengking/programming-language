# Variadic CRTP

在重要使用的是 [Parameter pack(since C++11)](https://en.cppreference.com/w/cpp/language/parameter_pack) 中的 "Base specifiers and member initializer lists" 语法点。

## fluentcpp [Variadic CRTP: An Opt-in for Class Features, at Compile Time](https://www.fluentcpp.com/2018/06/22/variadic-crtp-opt-in-for-class-features-at-compile-time/)

> NOTE: 
>
> 1、"opt-in"的含义是"选择性加入"，其实就是mixin

The CRTP is a technique that allows to add extra features to a class. Variadic templates, brought by C++11, make this technique more powerful by adding a new aspect to it: composition.

> NOTE: 上述"add extra features to a class"，其实就是mixin

Combining the CRTP with variadic templates generates customizable classes, by opting in for a various set of features, and with a expressive syntax.

It is used extensively in the customizable skills of the [`NamedType`](http://github.com/joboccara/NamedType) library.

Before introducing variadic templates in the CRTP, here is a brief recap about the CRTP itself.

### The CRTP

On its most basic description, the technical definition of the CRTP is a class that inherits from a template base class, passing itself as template parameter:

```C++
template<typename Derived>
class Base
{
    
};

class X : public Base<X>
{
    
};
```

If you’re not familiar with the CRTP, take a moment to wrap your head around the above code.

Now beyond the technical definition, what is [the point of the CRTP](https://www.fluentcpp.com/2017/05/16/what-the-crtp-brings-to-code/)? In a word, the CRTP allows to plug in extra features to your class, that use its public interface:

```c++
template<typename Derived>
class ExtraFeature
{
public:
    void extraMethod()
    {
        auto derived = static_cast<Derived&>(*this);
        derived.basicMethod();
    }
};

class X : public ExtraFeature<X>
{
public:
    void basicMethod() {}
};
```

By inheriting from `ExtraFeature`, the class `X` has indeed gained a new feature: `extraMethod`. Indeed, it is now part of the public interface of `X`, and we can write this call:

```c++
X x;
x.extraMethod();
```

The code of this `extraMethod` uses the public interface of `X`. Once again, if this is the first CRTP you see, take some time to go through the definition of `X` line by line. If you’d like to see more details, check out this [detailed post about the CRTP](https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/).

The point of having `ExtraFeature` decoupled from `X` is that it can be reused with any other class `Y`, as long as it also exposes the public interface that `ExtraFeature` uses (here, `basicMethod`).

> NOTE: code reuse

### A variadic CRTP

This was about adding **one** extra feature to our class `X`. Now how can we add **several** extra features?

One way would be to add other methods to the base class of the CRTP, `ExtraFeature`. It makes sense if those new methods relate to `ExtraFeature` and to whatever `extraMethod` does.

But if you’d like to add an unrelated feature, it would make more sense to package it into another CRTP base class:

```c++
template<typename Derived>
class ExtraFeature2
{
public:
    void extraMethod2()
    {
        auto derived = static_cast<Derived&>(*this);
        // does something else with derived.basicMethod() ...
    }
};

class X : public ExtraFeature<X>, public ExtraFeature2<X>
{
public:
    void basicMethod() {}
};
```

Now `X` has been augmented with both `extraMethod` and `extraMethod2`.

In some cases, such a design is enough. But some cases have way more than two extra features, and you want to choose which ones to define on `X` depending on the context (for example, this is the case in the `NamedType` library, where you can choose amongst a various sets of operators (`operator+`, `operator*`, `operator<<`, `operator int`, …) which one to tack(附加) on a given strong type).

One way to go about this is to make `X` a template, with a variadic pack of template arguments. And the elements of the pack are the extra features to add to `X`.

But how should we write this pack? Let’s to write it with a set of `typename` parameters:

```C++
template<typename... Skills>
class X : public Skills...
{
public:
    void basicMethod() {}
};
```

The `Skills` are supposed to be the set of extra features to tack(附加) on to `X`.

#### Aside: which name for the template parameters?

> NOTE: aside的意思是"旁白"

Before attempting to compile this code, a little note on the name, “Skills”.

In general customizing the aspects of a class with template parameters is called using “policies”, not “skills”. A policy is one particular aspect of a class, that can have several behaviours. That class is a template, and you choose a **behaviour** when you instantiate it (one of them can be a default).

To quote an example from Modern C++ Design, there are several ways to manage the life cycle of a `Singleton` object. Indeed, if a singleton depends on another singleton, their order of destruction matters. So the `Singleton` class defines a lifetime “policy” as a template parameter, that allows to choose between four options:

1、the singleton is destroyed when the program exists,

- the singleton can be re-initialized after its destruction if is needed then (“phoenix(死而复生的) singleton”),

2、the singleton has a longevity(长寿) that allows to customize the order of destructions of singletons,

3、the singleton is never destroyed.

To implement this choice, the `Singleton` class has a `LifetimePolicy` template parameter:

```C++
template<LifetimePolicy, /* other points of customisation */>
class Singleton
{
    // ...
};
```

And this policy can be set to either one of `DefaultLifetime`, `PhoenixSingleton`, `SingletonWithLongevity`, `NoDestroy`. The beauty of this design is that there are other points of customization in the `Singleton` class, such as how it is created and how it handles multithreading. Those are two other policies, independent from the previous one but also implemented as template parameters.

For more about **policy-based design**, check out the book [Modern C++ Design](https://www.amazon.com/gp/product/0201704315/ref=as_li_tl?ie=UTF8&camp=1789&creative=9325&creativeASIN=0201704315&linkCode=as2&tag=fluentcpp-20&linkId=5aeb3f292048e43723ef58985015c6e6).

In our case with the variadic CRTP, I don’t think that the extra feature we’re adding to `X` are policies, because they are not points of customization by themselves. Their *presence*(存在) is the customization. And since `X` can do new thing with them, I call them “skills”.

Happy to discuss this naming point further in the comments section below.

#### Template template parameters

> NOTE: 其实是在告诉我们: "variadic template + template template parameter"的写法。

Let’s move on with the name `Skills` for the moment. So here was our attempt of fitting them into `X`:

```C++
template<typename... Skills>
class X : public Skills...
{
public:
    void basicMethod() {}
};
```

With, for example, this instantiation:

```C++
using X2 = X<ExtraFeature2>;
X2 x;
x.extraMethod2();
```

But this leads us right into a compilation error:

```C++
error: type/value mismatch at argument 1 in template parameter list for 'template<class ... Skills> class X'
     using X2 = X<ExtraFeature2>;
                               ^
expected a type, got 'ExtraFeature2'
```

Look at the last line of this error message. It says that `X` expected a type. Indeed, its template parameter is `typename`. But `ExtraFeature` is a template, not a type. A type would be `ExtraFeature<X2>`. But if we try this:

```C++
using X2 = X<ExtraFeature2<X2>>;
```

We now get:

```C++
error: 'X2' was not declared in this scope
```

The solution here is not to pass a type, but rather a template. So `X` should not expect `typename`s, but templates. Here is the correct code for `X`, and we review it in details just afterwards:

```C++
template<template<typename> typename... Skills>
class X : public Skills<X<Skills...>>...
{
public:
    void basicMethod();
};
```

So, step by step:

```C++
template<template<typename> typename... Skills>
```

The `template<typename> typename` indicates that we are expecting a template that takes one `typename`. Note that this is equivalent to `template<typename T> typename`: we expect a template that takes a `typename T`. But since we don’t use `T` here, we can omit it.

Note that before C++17, for some reason we couldn’t use this exact expression. We had to use `class` instead of `typename` for the second `typename`: `template<typename> class`.

Finally, the variadic pack `...` allows to pass several such templates.

The second tricky bit is what `X` inherits from: `Skills<X<Skills...>>...`. Let’s examine it from the inside out.

`Skills...` is the list of template parameters. `X<Skills...>` is the current class `X` that is being instantiated, because we merely repeat the template parameters of its instantiation. Then `Skills<X<Skills...>>...` is the pack of CRTP base class. It is equivalent to `ExtraFeature2<X<ExtraFeature2>>` in our example.

With more extra features, the relations between classes look like this:

![variadic CRTP](https://www.fluentcpp.com/wp-content/uploads/2018/06/image1.png)

Here is an example of calling code with one extra feature:

```
using X2 = X<ExtraFeature2>;
X2 x;
x.extraMethod2();
```

We can add to it other extra CRTP features, just by mentioning them in the template parameters list:

```
using X12 = X<ExtraFeature1, ExtraFeature2>;
X12 x;
x.extraMethod1();
x.extraMethod2();
```

Note the concise syntax.

So this is a variadic CRTP. It allows to add as many extra features as you want to a class, that enrich its interface by using its public method (including those of the other CRTP classes!).

One aspect to pay attention to is when there are many extra features, or if the extra features have complex types. Indeed, this can cause the type name of the instantiation of `X` to grow, sometimes too much. In a later post, we will see how to keep control of this and avoid the name to bloat.

### 完整程序

```C++
#include <iostream>
template<typename Derived>
class ExtraFeature
{
public:
	void extraMethod()
	{
		auto derived = static_cast<Derived&>(*this);
		derived.basicMethod();
	}
};

template<typename Derived>
class ExtraFeature2
{
public:
	void extraMethod2()
	{
		auto derived = static_cast<Derived&>(*this);
		// does something else with derived.basicMethod() ...
	}
};

//class X: public ExtraFeature<X>, public ExtraFeature2<X>
//{
//public:
//	void basicMethod()
//	{
//	}
//};

template<template<typename > class... Skills>
class X: public Skills<X<Skills...>> ...
{
public:
	void basicMethod()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
int main()
{
	using X2 = X<ExtraFeature2>;
	X2 x;
	x.extraMethod2();
	using X12 = X<ExtraFeature, ExtraFeature2>;
	X12 x12;
	x12.extraMethod();
	x12.extraMethod2();
}
// g++ --std=c++11 test.cpp

```

