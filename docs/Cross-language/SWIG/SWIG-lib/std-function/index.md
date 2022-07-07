# `std::function`



## issues



github [Feature Request: Add support for std::function #341](https://github.com/swig/swig/issues/341)



## python



stackoverflow [c++ back end call the python level defined callbacks with swig wrapper](https://stackoverflow.com/questions/11516809/c-back-end-call-the-python-level-defined-callbacks-with-swig-wrapper)

github [Python interface #210](https://github.com/gazebosim/gz-math/issues/210)



## java



### stackoverflow [How to use SWIG to wrap std::function objects?](https://stackoverflow.com/questions/32644268/how-to-use-swig-to-wrap-stdfunction-objects)



#### [A](https://stackoverflow.com/a/32668302)

> NOTE: 
>
> 关于 `%inline`，参见 **5.6.3 Inlined code blocks**
>
> 



1、We want to be able to call `std::function` objects from within Java code.

> NOTE: 能够从java代码中待用" `std::function` "对象

2、The wrapped `std::function` objects need to get passed around like any other object, including crossing the language boundaries in either direction.

> NOTE: 

3、It should be possible to write `std::function` objects inside of Java, which can be passed back to C++ without having to modify existing C++ code that works on `std::function` objects (i.e. maintaining type erasure of `std::function` cross language)

> NOTE: "type erasure of `std::function` cross language"

4、We should be able to construct `std::function` objects in Java using C++ pointer to function types.

> NOTE: 这是什么含义？

#### first version

Where possible I'll keep the solution language agnostic too.

> NOTE: "language agnostic"意味着

I've put together a test interface to illustrate how it will be used:

```c++
%module test
%include "std_function.i"

%std_function(Functor, void, int, double);

%{
#include <iostream>
%}

%inline %{
  std::function<void(int,double)> make_functor() {
    return [](int x, double y){
      std::cout << x << ", " << y << "\n";
    };
  }
%}
```

Basically to use this all you need do is include the file "std_function.i" and then use the macro `%std_function` which takes arguments as:

```c++
%std_function(Name, Ret, ...)
```

You call this once per instantiation of the `std::function` template you want to wrap, where `Name` is what you want to call the type in Java, `Ret` is the **return type** and then the remaining (variadic) arguments are the inputs to your function. So in my test interface above I'm basically looking to wrap `std::function<void(int,double)>`.

> NOTE: 上面这段话结合下面的内容是更加容易理解的

Writing a first version of "std_function.i" isn't actually too tricky(棘手的). All you need to get basic working requirements #1 and #2 is:

```cpp
%{
  #include <functional>
%}

%define %std_function(Name, Ret, ...)
%rename(Name) std::function<Ret(__VA_ARGS__)>;
%rename(call) std::function<Ret(__VA_ARGS__)>::operator();
namespace std {
  struct function<Ret(__VA_ARGS__)> {
    // Copy constructor
    function<Ret(__VA_ARGS__)>(const std::function<Ret(__VA_ARGS__)>&);

    // Call operator
    Ret operator()(__VA_ARGS__) const;
  };
}

%enddef
```

This includes the C++ header file in the **generated wrapper code** once and then sets up the macro for usage in interfaces. SWIG's support for [C++11 variadic templates](http://www.swig.org/Doc3.0/CPlusPlus11.html#CPlusPlus11_variadic_templates) isn't actually very helpful for us in this usage scenario, so the macro I wrote basically re-implements [the default template expansion functionality](http://www.swig.org/Doc1.3/SWIGPlus.html#SWIGPlus_nn30) using C99 variadic macro arguments (which are much better supported). Coincidentally(巧合的是) this means the SWIG code we're writing will work with 2.x or even some 1.3.x releases. (I tested with 2.x). Even if/when your version of SWIG does have `%template` support that works with `std::function` retaining this macro is still useful for the rest of the glue that makes it actually callable.

> NOTE: 
>
> 一、"so the macro I wrote basically re-implements [the default template expansion functionality](http://www.swig.org/Doc1.3/SWIGPlus.html#SWIGPlus_nn30) using C99 variadic macro arguments (which are much better supported)."
>
> 上面这段话的意思是: 上面的实现方式其实包含了template expansion，具体来说，就是下面的:
>
> ```c++
>   struct function<Ret(__VA_ARGS__)> {
>     // Copy constructor
>     function<Ret(__VA_ARGS__)>(const std::function<Ret(__VA_ARGS__)>&);
> 
>     // Call operator
>     Ret operator()(__VA_ARGS__) const;
>   };
> ```
>
> 后面作者将这种方式称之为"manual expansion"
>
> 原文后面的内容其实是在论述这种方式的优势所在。

The **manual expansion** of the `std:function` template is limited to just the bits we care about for our usage: the actual `operator()` and a copy constructor that might come in handy.

The only other thing to be done is renaming `operator()` to something that matches the target language, e.g. for Java renaming it to be just a regular function called "call", or if you were targeting Python to `__call__` or using tp_slots if required.

This is now sufficient to let our interface work, to demonstrate it I wrote a little bit of Java:

```typescript
public class run {
    public static void main(String[] argv) {
        System.loadLibrary("test");
        test.make_functor().call(1,2.5);
    }
}
```

Which I compiled with:

```shell
swig2.0 -Wall -c++ -java  test.i
g++ -Wall -Wextra -std=c++11 test_wrap.cxx -o libtest.so -I/usr/lib/jvm/default-java/include/ -I/usr/lib/jvm/default-java/include/linux -shared -fPIC
javac run.java
LD_LIBRARY_PATH=. java run
```

and it worked.

---

#### second version

Requirement #4 is pretty easy to cross off the list now at this point. All we need to do is tell SWIG there's another constructor in `std::function` which accepts compatible function pointers:

> NOTE: 从列表中划掉

```c++
// Conversion constructor from function pointer
function<Ret(__VA_ARGS__)>(Ret(*const)(__VA_ARGS__));
```

And then we can use this with the [`%callback` mechanism](http://www.swig.org/Doc3.0/SWIG.html#SWIG_nn30) in SWIG, our test interface file becomes:

```c++
%module test
%include "std_function.i"

%std_function(Functor, void, int, double);

%{
#include <iostream>
void add_and_print(int a, double b) {
  std::cout << a+b << "\n";
}
%}

%callback("%s_cb");
void add_and_print(int a, double b);
%nocallback;

%inline %{
  std::function<void(int,double)> make_functor() {
    return [](int x, double y){
      std::cout << x << ", " << y << "\n";
    };
  }
%}
```

and the Java we use to call this is then:

```typescript
public class run {
    public static void main(String[] argv) {
    System.loadLibrary("test");
    test.make_functor().call(1,2.5);
    new Functor(test.add_and_print_cb).call(3,4.5);
    }
}
```

Which we compile and run identically successfully at this point.

(Note that it's normal and desirable to see some Java classes created at this point that start with the name "SWIGTYPE_p_f_..." - these wrap the "pointer to function" types that are used by the pointer to function constructor and callback constants)

---

#### third version

Requirement #3 is where things start to get trickier(棘手). Essentially we've hit the same problem as I answered [on making SWIG generate an interface in Java](https://stackoverflow.com/a/8246375/168175) previously, except now we're looking to do it within a macro more generically.

> NOTE: 使用macro的方式来让它"generically"

It turns out that in this instance because the interface we want to generate is fairly simple we can use some tricks inside our macro to make SWIG generate it for us.



The main thing that we need to do in order to make this work is to setup **SWIG directors** to provide **cross-language polymorphism** and allow something written in Java to implement a C++ interface. This is the class generated with the suffix "Impl" in my code.

> NOTE: " **cross-language polymorphism**"

To make things "feel right" to Java developers we want to still use the same type for both C++ and Java implemented `std::function` objects. Even if `std::function::operator()` were virtual we still don't want to have **SWIG directors** use that type directly as it's pretty common to pass `std::function` by value which would lead to type [slicing problems](https://stackoverflow.com/questions/274626/what-is-object-slicing). So when a Java developer extends our `std::function` objects and overrides `call` we need to do some extra work to make it so that C++ which uses that object actually calls the Java implementation given that we can't just use directors to handle this automatically.

> NOTE: 
>
> 一、"Even if `std::function::operator()` were virtual we still don't want to have **SWIG directors** use that type directly as it's pretty common to pass `std::function` by value which would lead to type [slicing problems](https://stackoverflow.com/questions/274626/what-is-object-slicing). "
>
> 这段话的意思是: 即使 `std::function::operator()` 是virtual的，我们仍然不希望" **SWIG directors**" 主管直接使用该类型，因为按值传递 `std::function` 会导致类型切片问题。
>
> 二、"So when a Java developer extends our `std::function` objects and overrides `call` we need to do some extra work to make it so that C++ which uses that object actually calls the Java implementation given that we can't just use directors to handle this automatically."
>
> 这段话描述的其实就是cross- language-polymorphism；如果不能够使用"director"，"we need to do some extra work to"来实现cross-language-polymorphism。
>
> 



So what we do looks a little bit weird. If you construct a Java object that is meant to implement `std::function` then there's a special, protected constructor for that. This constructor leaves the `swigCPtr` member variable, which normally points to a real C++ object as 0 and instead creates an anonymous wrapper object that implements the "Impl" interface and simply proxies everything back to the `call` member of the Java object.





## C#

github [AOT support for C# delegates #1579](https://github.com/swig/swig/issues/1579)

github [AOT support for C# directors #1165](https://github.com/swig/swig/issues/1165)

github [Wrapping std::function from C++ to C# #1719](https://github.com/swig/swig/issues/1719)

