# [Chromium Style Plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors) 、 [Chromium Style Checker Errors](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/)

> NOTE: 
>
> 一、chromium不仅仅提出chromium C++ style guide，并且还基于clang开发了[Chromium Style Plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors) ，用于检查代码是否遵循了style guide；

Due to constant over inlining, we now have plugins to the clang compiler which check for certain patterns that can increase code bloat. This document lists these errors, explains the motivation behind adding it as an error and shows how to fix them. The [chromium/clang wiki page](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang_format_binaries.md) explains how you can run the plugin yourself, or how you can run your CLs through the clang trybots which run this plugin as well.

> NOTE: 
>
> 一、[Chromium Style Plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors) 的目的主要是"check for certain patterns that can increase code bloat"，通过下面的内容可知，涉及code bloat的主要如下两个方向：
>
> 1、[Constructor/Destructor Errors](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#constructordestructor-errors)
>
> 这种方式是主要的
>
> 2、[Virtual Method Out-of-lining](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#virtual-method-out-of-lining)
>
> 这种方式gain有限

## [Constructor/Destructor Errors](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#constructordestructor-errors)

> NOTE: 
>
> 非常值得一读

Constructors and destructors are often much larger and more complex than programmers think. They are also implicitly defined in each translation unit if you don't declare them. This can have a cascading effect if a class has member variables that have inlined/implicitly declared constructors/destructors, or are templates.

> NOTE: 
>
> 一、关于这种情况的验证，参见 `测试验证chromium-style-plugin` 章节

(Internally, the plugin determines whether a class is complex using a **point system**, adding 9 points for each templated base class, 10 for each templated member variable, 3 for each non-POD type member variable, and (only for the constructor score) 1 for each integral data type. If a class's score is greater than or equal to 10, it will trip these errors.)

> NOTE:
>
> 一、可以看到，"templated member variable"是比"templated base class"的分数更高的

If you get any of these compiler errors:

1、Complex class/struct needs an explicit out-of-line constructor

2、Complex class/struct needs an explicit out-of-line destructor

It's because you've written something like the following in a header:

```c++
class ComplexStuff
{

public:
  void NoDeclaredConstructor();

private:
  // Enough data members to trip the detector

  BigObject big_object_;

  std::map<std::string, OtherObj> complex_stl_stuff_;
};

```

This is fixed by adding declared constructors:

```c++
class ComplexStuff
{

public:
  // Defined in the cc file:
  ComplexStuff();

  ~ComplexStuff();

private:
  BigObject big_object_;

  std::map<std::string, OtherObj> complex_stl_stuff_;
};
```

Likewise, if you get these compiler errors:

1、Complex constructor has an inlined body.

2、Complex destructor has an inline body.

It's because you've written something like the following in a header:

```c++
class MoreComplexStuff
{
public:
  MoreComplexStuff() {}

  ~MoreComplexStuff() {}

private:
  BigObject big_object_;

  std::map<std::string, OtherObj> complex_stl_stuff_;
};
```

The solution is the same; put the definitions for the constructor/destructor in the implementation file.

Sometimes, you might need to inline a constructor/destructor for performance reasons. The style checker makes an exception for explicitly inlined constructors/destructors:

```c++
// In the .h file:

class ComplexFastStuff
{
public:
  ComplexFastStuff();

  ~ComplexFastStuff();

private:
  /* complicated members */
};
```



```c++
// In the .cc file:
inline ComplexFastStuff::ComplexFastStuff() {}

inline ComplexFastStuff::~ComplexFastStuff()
{
}
```

If this class is being copied somewhere in the code, then the compiler will generate an **implicit copy constructor** for the class. As a result, you might also see the following error:

- Complex class/struct needs an explicit out-of-line copy constructor.

This can be fixed by adding an explicit copy constructor to your class:

```c++
// In the .h file:

class ComplexCopyableStuff
{
public:
  ComplexCopyableStuff();

  ComplexCopyableStuff(const ComplexCopyableStuff &other);

  ~ComplexCopyableStuff();

private:
  /* complicated members */
};

// In the .cc file:
ComplexCopyableStuff::ComplexCopyableStuff(const ComplexCopyableStuff &other) = default;
```

Note that by defaulting the implementation, you ensure that you don't forget to copy a member and this code will not need to be updated as the members of the class change.

Also note that copying a complex class can be an expensive operation. The preferred solution is almost always is to avoid the copy if possible. Please consider changing the code and eliminating unnecessary copies instead of simply adding an **out of line copy constructor** to silence the error. Additionally, in a large number of cases, you can utilize move semantics to improve the efficiency of the code.

> NOTE: 
>
> 一、performance-principle-prefer-move-to-copy

Usually a compiler would generate a move constructor for your class, making moving objects efficient. This is particularly true for STL containers such as [std::map](https://en.cppreference.com/w/cpp/container/map). However, specifying either a **copy constructor** or a **destructor** prevents the compiler from generating a **move constructor**. All in all, you should prefer specifying a move constructor instead of a copy constructor. In cases where your object is used as an rvalue, this will also prevent the error since the compiler will not generate a copy constructor:

```c++
// In the .h file:
class ComplexMovableStuff
{
public:
  ComplexMovableStuff();

  ComplexMovableStuff(ComplexMovableStuff &&other);

  ~ComplexMovableStuff();

private:
  /* complicated members */
};

// In the .cc file:
ComplexMovableStuff::ComplexMovableStuff(ComplexMovableStuff &&other) = default;

```

For more information on move semantics, please refer to https://www.chromium.org/rvalue-references

## [Virtual Method Out-of-lining](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#virtual-method-out-of-lining)

> NOTE: 
>
> 一、"out of line"的含义呢？
>
> "out of line"显然是和 "inline" 相对应的，在 stackoverflow [Out-of-Line Virtual Method](https://stackoverflow.com/questions/16801222/out-of-line-virtual-method) # [A](https://stackoverflow.com/a/16801276/10173843) 中有着非常好的解释:
>
> > The compiler must emit a vtable for classes with virtual methods. This contains the pointers to these methods. If all the virtual methods are inline (defined in the header), then the compiler doesn't know which translation unit (.cpp file) to emit the vtable within, so it emits a copy in all of them and the linker gets to deal with it. This makes extra work and bloat in the object files. If, on the other hand, a virtual function is defined out-of-line (in a .cpp), the vtable can be emitted there and thus only one copy will be emitted. The same applies to the RTTI.

Virtual methods are almost never inlined in practice. Because of this, methods on a base class will be emitted(发出、产生) in each translation unit that allocates the object or any subclasses that don't override that method. This is usually not a major gain, unless you have a wide class hierarchy in which case it can be a big win (http://codereview.chromium.org/5741001/). Since virtual methods will almost never be inlined anyway (because they'll need to go through vtable dispatch), there are almost no downsides.

If you get the error:

- virtual methods with non-empty bodies shouldn't be declared inline

It's because you wrote something like this:

```c++
class BaseClass
{
public:
  virtual void ThisOneIsOK() {}

  virtual bool ButWeDrawTheLineAtMethodsWithRealImplementation() { return false; }
};
```

And can be fixed by out of lining the method that does work:

```c++
class BaseClass
{
public:
  virtual void ThisIsHandledByTheCompiler() {}

  virtual bool AndThisOneIsDefinedInTheImplementation();
};
```



## [Virtual methods and override/final](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#virtual-methods-and-overridefinal)

If you get the error:

- Overriding method must be marked with 'override' or 'final'.



## [Redundant Virtual specifiers](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#redundant-virtual-specifiers)



## [Virtual final base class methods](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#virtual-final-base-class-methods)



## ["Using namespace" in a header](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#using-namespace-in-a-header)



## [RefCounted types with public destructors](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#refcounted-types-with-public-destructors)



## [Enumerator max values](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#enumerator-max-values)