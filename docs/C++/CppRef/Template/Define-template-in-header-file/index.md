# Define template in header file

在使用template的时候，一个非常重要的事实是: 需要将template定义于header file中，一般不能定义于source file中。

## stackoverflow [Why can templates only be implemented in the header file?](https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file)

### [A](https://stackoverflow.com/a/16493574)

> NOTE: 
>
> 1、这个回答我觉得是最好的

It's because of the requirement for **separate compilation** and because templates are **instantiation-style polymorphism**.

> NOTE: 关于**instantiation-style polymorphism**，后面的"原因一"中有解释。

Lets get a little closer to concrete for an explanation. Say I've got the following files:

1、`foo.h`

declares the interface of `class MyClass<T>`

2、`foo.cpp`

defines the implementation of `class MyClass<T>`

3、`bar.cpp`

uses `MyClass<int>`

#### Separate compilation

Separate compilation means I should be able to compile **foo.cpp** independently from **bar.cpp**. The compiler does all the hard work of analysis, optimization, and code generation on each compilation unit completely independently; we don't need to do whole-program analysis. It's only the linker that needs to handle the entire program at once, and the linker's job is substantially easier.

**bar.cpp** doesn't even need to exist when I compile **foo.cpp**, but I should still be able to link the **foo.o** I already had together with the **bar.o** I've only just produced, without needing to recompile **foo.cpp**. **foo.cpp** could even be compiled into a dynamic library, distributed somewhere else without **foo.cpp**, and linked with code they write years after I wrote **foo.cpp**.

#### "Instantiation-style polymorphism"

"Instantiation-style polymorphism" means that the template `MyClass<T>` isn't really a generic class that can be compiled to code that can work for any value of `T`. That would add overhead such as boxing, needing to pass function pointers to allocators and constructors, etc. The intention of C++ templates is to avoid having to write nearly identical `class MyClass_int`, `class MyClass_float`, etc, but to still be able to end up with compiled code that is mostly as if we *had* written each version separately. So a template is *literally* a template; a class template is *not* a class, it's a recipe for creating a new class for each `T` we encounter. A template cannot be compiled into code, only the result of instantiating the template can be compiled.

So when **foo.cpp** is compiled, the compiler can't see **bar.cpp** to know that `MyClass<int>` is needed. It can see the template `MyClass<T>`, but it can't emit code for that (it's a template, not a class). And when **bar.cpp** is compiled, the compiler can see that it needs to create a `MyClass<int>`, but it can't see the template `MyClass<T>` (only its interface in **foo.h**) so it can't create it.

If **foo.cpp** itself uses `MyClass<int>`, then code for that will be generated while compiling **foo.cpp**, so when **bar.o** is linked to **foo.o** they can be hooked up and will work. We can use that fact to allow a finite set of template instantiations to be implemented in a .cpp file by writing a single template. But there's no way for **bar.cpp** to use the template *as a template* and instantiate it on whatever types it likes; it can only use pre-existing versions of the templated class that the author of **foo.cpp** thought to provide.

You might think that when compiling a template the compiler should "generate all versions", with the ones that are never used being filtered out during linking. Aside from the huge overhead and the extreme difficulties such an approach would face because "type modifier" features like pointers and arrays allow even just the built-in types to give rise to an infinite number of types, what happens when I now extend my program by adding:

1、`baz.cpp`

declares and implements `class BazPrivate`, and uses `MyClass<BazPrivate>`

There is no possible way that this could work unless we either

1、Have to recompile **foo.cpp** every time we change *any other file in the program*, in case it added a new novel instantiation of `MyClass<T>`

2、Require that **baz.cpp** contains (possibly via header includes) the full template of `MyClass<T>`, so that the compiler can generate `MyClass<BazPrivate>` during compilation of **baz.cpp**.

Nobody likes (1), because whole-program-analysis compilation systems take *forever* to compile , and because it makes it impossible to distribute compiled libraries without the source code. So we have (2) instead.



### [A](https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file/495056#495056)

#### Alternative solution

> NOTE: 
>
> 1、这种做法是值得借鉴的
>
> 2、它能够实现declaration 和 definition的分离？

Another solution is to keep the implementation separated, and explicitly instantiate all the template instances you'll need:

Foo.h

```C++
// no implementation
template <typename T> struct Foo { ... };
```

Foo.cpp

```c++
// implementation of Foo's methods

// explicit instantiations
template class Foo<int>;
template class Foo<float>;
// You will only be able to use Foo with int or float
```

If my explanation isn't clear enough, you can have a look at the [C++ Super-FAQ on this subject](https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl).

## 为什么这样做



### 原因一

“The definition of a template must be visible at the point of implicit instantiation, which is why template libraries typically provide all template definitions in the headers”（截取自cppreference [Templates](https://en.cppreference.com/w/cpp/language/templates)）;

这样设计的原因是:

1、这是template的本质，所谓"template"，其实就是一个pattern，compiler对它的处理主要是substitution，因此compiler在编译template的时候，是需要得到"all template definitions"的，这样才能够完成template instantiation。这就是 stackoverflow [Why can templates only be implemented in the header file?](https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file) # [A](https://stackoverflow.com/a/16493574) 中提及的 "instantiation-style polymorphism"。



2、这是因为template implementation主要发生于compile time，在link time做的事情是非常少的，因此compiler在进行编译一个source file的时候，如果其中使用了template，那么compiler是需要能够获得完整的template definition。

### 原因二

C++ 采用 "Separate compilation model";

每个source file就是一个translation unit，C++ compiler一次只编译一个source file，因此如果将template definition放到单独的source file中，则可能出现在编译source file A的时候，它需要的template definition放在了source  file B中，那么此时它就无法获得完整的template definition，那么它就无法完成编译。

---



下面是探讨了这个问题的一些文章: 

## isocpp Why can’t I separate the definition of my templates class from its declaration and put it inside a .cpp file? [¶](https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl) [Δ](https://isocpp.org/wiki/faq/templates#)



