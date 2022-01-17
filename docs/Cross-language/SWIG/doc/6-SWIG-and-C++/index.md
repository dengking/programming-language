# 6 SWIG and C++

## 6.1 Comments on C++ Wrapping

> NOTE: 
>
> 这段论述还是不错的

## 6.2 Approach

To wrap C++, SWIG uses a layered approach to code generation. At the lowest level, SWIG generates a collection of procedural ISO C style wrappers. These wrappers take care of basic type conversion, type checking, error handling, and other low-level details of the C++ binding. These wrappers are also sufficient to bind C++ into any target language that supports built-in procedures. In some sense, you might view this layer of wrapping as providing a C library interface to C++. On top of the low-level procedural (flattened) interface, SWIG generates proxy classes that provide a natural object-oriented (OO) interface to the underlying code. The proxy classes are typically written in the target language itself. For instance, in Python, a real Python class is used to provide a wrapper around the underlying C++ object.

> NOTE: 
>
> 这一节主要描述SWIG的原理，在 Klaus Kämpf 的 [SWIG Generating language bindings for C/C++ libraries](https://en.opensuse.org/images/e/eb/Kkaempf_KnowledgeSharing_Swig.pdf) 中有着更好的图示。
>
> 一、layer
>
> 1、ISO C style wrappers
>
> 2、**proxy classes** that provide a natural object-oriented (OO) interface to the underlying code

## 6.3 Supported C++ features

As a rule of thumb, SWIG should not be used on raw C++ source files, use header files only.

## 6.5 Proxy classes

### 6.5.1 Construction of proxy classes

Proxy classes are always constructed as an extra layer of wrapping that uses low-level accessor functions. To illustrate, suppose you had a C++ class like this:

```C++
class Foo {
public:
    Foo();
    ~Foo();
    int bar(int x);

    int x;
};
```

Using C++ as pseudocode, a proxy class looks something like this:

```C++
class FooProxy {
private:
	Foo *self;
public:
    FooProxy() {
	    self = new_Foo();
    }
    ~FooProxy() {
    	delete_Foo(self);
    }
    int bar(int x) {
    	return Foo_bar(self, x);
    }
    int x_get() {
    	return Foo_x_get(self);
    }
    void x_set(int x) {
    	Foo_x_set(self, x);
    }
};
```

Of course, always keep in mind that the real proxy class is written in the target language. For example, in Python, the proxy might look roughly like this:

```Python
class Foo:
    def __init__(self):
        self.this = new_Foo()

    def __del__(self):
        delete_Foo(self.this)

    def bar(self, x):
        return Foo_bar(self.this, x)

    def __getattr__(self, name):
        if name == 'x':
            return Foo_x_get(self.this)

    # ...
    def __setattr__(self, name, value):
        if name == 'x':
            Foo_x_set(self.this, value)

```

### 6.5.2 Resource management in proxies

A major issue with proxies concerns the memory management of wrapped objects. Consider the following C++ code:

```C++
class Foo {
public:
	Foo();
    ~Foo();
    int bar(int x);
    int x;
};
class Spam {
public:
    Foo *value;
    ...
};
```

Consider some script code that uses these classes:

```Python
f = Foo() # Creates a new Foo
s = Spam() # Creates a new Spam
s.value = f # Stores a reference to f inside s
g = s.value # Returns stored reference
g = 4 # Reassign g to some other value
del f # Destroy f
```

## 6.6 Simple C++ wrapping



### 6.6.3 When constructor wrappers aren't created

## 6.19 Namespaces

> NOTE: 
>
> example: stackoverflow [Wrapping C-enum in a Python module with Swig](https://stackoverflow.com/questions/16471213/wrapping-c-enum-in-a-python-module-with-swig)
>
> 

## 6.25 C++ reference counted objects - ref/unref feature

## 6.29 Callbacks to the target language

> NOTE: 
>
> "Callbacks to the target language" 指的是在source language的程序中，执行target language程序中的函数

C/C++ function pointers are often used for callbacks and this is discussed in the Pointers to functions and callbacks section. The callback techniques described therein provide a way to control callbacks to a C/C++ function but not callbacks into the target language. The techniques described below show how the director feature can be used to support callbacks from C/C++ to the target language.

> NOTE: 
>
> 一、翻译如下:
>
> "C/C++函数指针通常用于回调，这将在函数指针和回调一节中讨论。这里描述的回调技术提供了一种方法来控制对C/C++函数的回调，而不是对目标语言的回调。下面描述的技术展示了如何使用director特性来支持从C/C++到目标语言的回调。"

### 6.29.1 Introduction to director classes

The **director feature** enables the ability for a target language class to derive from a **wrapped C++ class**. The target language can override virtual methods of a wrapped C++ class, thereby supporting **cross-language polymorphism**. Code can 'call up' from C++ into the target language by simply calling a virtual method overridden in a derived class in the target language. The **wrapped C++ classes** that have this ability are termed 'director' classes. The director feature is documented individually in each target language and the reader should locate and read this to obtain a full understanding of directors.

> NOTE: 
>
> 目标语言中的类来实现源语言C++中的virtual class，实现**cross-language polymorphism**。

### 6.29.2 Using directors and target language callbacks

SWIG's primary goal is to make it possible to call C/C++ code from a target language, however, the director feature enables the reverse. While there isn't simple direct support for calling target language code from C, the director feature makes this possible. It does require some work and additional wrapper code to be provided by the user. The additional code required must be C++ and not C code and hence may introduce a small dependency on C++ if using a pure C project. In a nutshell, the user must create a C++ base class and turn it into a **director class**. A **virtual method** in the director base class is required. SWIG generates the code to call up into the target language when wrapping the **director virtual method**.

> NOTE: 
>
> 所谓 "director class"，其实就是接口类