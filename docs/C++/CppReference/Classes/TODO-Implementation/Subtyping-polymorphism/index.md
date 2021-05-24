# 关于本章

## Implementation需要考虑的问题



### 可能的class hierarchy

C++ Subtype polymorphism的实现需要考虑如下可能的情况: 

1、single inheritance

2、multiple inheritance

3、virtual inheritance

### Object memory layout

参见 `Class-memory-layout` 章节



## 可行的方案

下面是一种可行的实现方案:

**dispatch table + method resolution order + pointer fixup(thunk)**

compiler会polymorphic type采用上述实现方案。

### Method selection



#### 对于single inheritance、multiple inheritance

使用 dispatch table即可，

#### 对于virtual inheritance

对于virtual inheritance，则需要使用dispatch table + method resolution order；

关于此，参见:

1、stackoverflow [Method resolution order in C++](https://stackoverflow.com/questions/3310910/method-resolution-order-in-c) # [A](https://stackoverflow.com/a/3310948)

There is no MRO in C++ like Python. If a method is ambiguous, it is a compile-time error. Whether a method is virtual or not doesn't affect it, but virtual *inheritance* will.

The algorithm is described in the C++ standard §[class.member.lookup] (10.2). Basically it will find the closest unambiguous implementation in the **superclass graph**. 



### Subobject(static type) 和 entire object(dynamic type) 之间的转换

这主要使用thunk来实现的

1、entire object -> subobject 

2、subobject -> entire object

1、在 wikipedia [Thunk](https://en.wikipedia.org/wiki/Thunk) # Applications # Object-oriented programming 中对此进行了详细说明。

2、在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)`#`[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)中提及了这个topic：

> Within each polymorphic object, the implementation stores additional information (in every existing implementation, it is one pointer unless optimized out), which is used by [virtual function](https://en.cppreference.com/w/cpp/language/virtual) calls and by the RTTI features ([dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast) and [typeid](https://en.cppreference.com/w/cpp/language/typeid)) to determine, at run time, the type with which the object was created, regardless of the expression it is used in.

对于polymorphic object的，在使用它的时候，一个非常重要的topic是: 得到dynamic type entire objec，因为它们一般都是通过static type subobject来进行使用的。

## stackoverflow [Why do we need virtual table?](https://stackoverflow.com/questions/3004501/why-do-we-need-virtual-table)

### [A](https://stackoverflow.com/a/3004555)

Without virtual tables you wouldn't be able to make runtime polymorphism work since all references to functions would be bound at compile time. A simple example

```cpp
struct Base {
  virtual void f() { }
};

struct Derived : public Base {
  virtual void f() { }
};

void callF( Base *o ) {
  o->f();
}

int main() {
  Derived d;
  callF( &d );
}
```

Inside the function `callF`, you only know that `o` points to a `Base` object. However, at runtime, the code should call `Derived::f` (since `Base::f` is virtual). At compile time, the compiler can't know which code is going to be executed by the `o->f()` call since it doesn't know what `o` points to.

Hence, you need something called a "virtual table" which is basically a **table of function pointers**. Each object that has virtual functions has a "**v-table pointer**" that points to the virtual table for objects of its type.

The code in the `callF` function above then only needs to look up the entry for `Base::f` in the **virtual table** (which it finds based on the **v-table pointer** in the object), and then it calls the function that table entry points to. That *might* be `Base::f` but it is also possible that it points to something else - `Derived::f`, for instance.

This means that due to the virtual table, you're able to have polymorphism at runtime because the actual function being called is determined at runtime by looking up a function pointer in the virtual table and then calling the function via that pointer - instead of calling the function directly (as is the case for non-virtual functions).

## vtable的本质: table of function pointer

virtual table其实就是一个dispatch table

### wikipedia [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science))

> 如下总结，我觉得描述地非常好: 

[Object-oriented programming languages](https://en.wikipedia.org/wiki/Object-oriented_programming_language) offer subtype polymorphism using *[subclassing](https://en.wikipedia.org/wiki/Subclass_(computer_science))* (also known as *[inheritance](https://en.wikipedia.org/wiki/Inheritance_in_object-oriented_programming)*). In typical implementations, each class contains what is called a *[virtual table](https://en.wikipedia.org/wiki/Virtual_table)*—a table of functions that implement the polymorphic part of the class interface—and each object contains a pointer to the "vtable" of its class, which is then consulted whenever a polymorphic method is called. This mechanism is an example of:

1、*[late binding](https://en.wikipedia.org/wiki/Late_binding)*, because virtual function calls are not bound until the time of invocation;

2、*[single dispatch](https://en.wikipedia.org/wiki/Single_dispatch)* (i.e. single-argument polymorphism), because virtual function calls are bound simply by looking through the vtable provided by the first argument (the `this` object), so the runtime types of the other arguments are completely irrelevant.

> NOTE: 这一段从实现层面详细描述了C++仅仅支持single dispatch的原因。

### stackoverflow [What is vtable in C++ [duplicate]](https://stackoverflow.com/questions/3554909/what-is-vtable-in-c)

[A](https://stackoverflow.com/a/3555290)

V-tables (or virtual tables) are how most C++ implementations do polymorphism. For each concrete implementation of a class, there is a **table of function pointers to all the virtual methods**. A pointer to this table (called the virtual table) exists as a data member in all the objects. When one calls a virtual method, we lookup the object's v-table and call the appropriate derived class method.

> NOTE: 
>
> 1、每个polymorphic object，都有一个执行virtual table的pointer
>
> 2、最后一段话描述的是late binding、dynamic binding的过程

[A](https://stackoverflow.com/a/3554995)

For all it's worth, it is not a standard C++ terminology. It is just an implementation detail used by the implementation to implement virtual functions/dynamic binding

> NOTE: 
>
> 1、standard and implementation



### stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) # [A](https://stackoverflow.com/a/6044720) 

The "different" methods simply differ in the way they add semantic sugar. Virtual functions, e.g., are just semantic sugar for

```cpp
struct Class {
    struct vtable {
        void (*dtor)(Class*);
        void (*func)(Class*,double);
    } * vtbl
};
```

iow: function pointers.

## 当前，virtual table是compile time生成的



### CppCoreGuidelines [T.83: Do not declare a member function template virtual](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t83-do-not-declare-a-member-function-template-virtual)

#### Reason

C++ does not support that. If it did, vtbls could not be generated until link time. And in general, implementations must deal with dynamic linking.

> NOTE: 
>
> 1、当前，virtual table是compile time生成的；如果支持template virtual function，那么virtual table需要直到link time才能够生成，并且还需要支持dynamic linking。
>
> 2、参见 `Template-and-virtual` 章节



## 为什么reference semantic才能够实现dynamic polymorphism

1、value semantic时，它的concrete type是已知的，compiler能够直接选择implementation，显然这样是符合optimization principle的

2、一般，implementation是需要进行pointer pointer fixup







## draft

需要考虑，使用哪个实现方式，因为可能的实现方式是多个的。

### 实现方式

wikipedia [Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)



most derived class

在https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019中提及了

> For example, if a class has virtual functions, all the instances of that class might share a single virtual function table



需要对比c++ runtime polymorphysim的实现和Python的runtime polymorphysim的实现，两者都需要存储 映射关系。c++的vtbl和Python的`__dict__`类似的。Python仅仅有reference semantic的。



这本质是是对function call的实现，即根据function name，调用对应的function。compiler编译过程中，的scope resolution过程，与上述过程也是非常类似的，都是逐级搜索；我应该根据compiler-time、runtime来进行区分，对两者的实现逻辑进行对比；它们本质上都是建立起映射关系：function name到function的映射；它们的搜索过程，都是逐级进行的。



在文章artima ["Pure Virtual Function Called": An Explanation](https://www.artima.com/cppsource/pure_virtual.html)中对此进行了解释。



## TODO

pabloariasal [Understandig Virtual Tables in C++](https://pabloariasal.github.io/2017/06/10/understanding-virtual-tables/)

