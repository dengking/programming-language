# Template metaprogramming

c++ template是属于template metaprogramming流派的，在`Theory\Programming-paradigm\Metaprogramming`中对Metaprogramming进行了介绍。

## 维基百科[Template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming)

**Template metaprogramming** (**TMP**) is a [metaprogramming](https://en.wikipedia.org/wiki/Metaprogramming) technique in which [templates](https://en.wikipedia.org/wiki/Generic_programming) are used by a [compiler](https://en.wikipedia.org/wiki/Compiler) to generate temporary [source code](https://en.wikipedia.org/wiki/Source_code), which is merged by the compiler with the rest of the source code and then compiled. The **output** of these templates include [compile-time](https://en.wikipedia.org/wiki/Compile_time) [constants](https://en.wikipedia.org/wiki/Constant_(programming)), [data structures](https://en.wikipedia.org/wiki/Data_structure), and complete [functions](https://en.wikipedia.org/wiki/Function_(computer_science)). The use of templates can be thought of as [compile-time execution](https://en.wikipedia.org/wiki/Compile_time_function_execution). The technique is used by a number of languages, the best-known being [C++](https://en.wikipedia.org/wiki/C%2B%2B), but also [Curl](https://en.wikipedia.org/wiki/Curl_programming_language), [D](https://en.wikipedia.org/wiki/D_programming_language), and [XL](https://en.wikipedia.org/wiki/XL_Programming_Language).

> NOTE: template是c++语言的核心所在

> NOTE: Java更加偏向于 interface-oriented programming，它充分运用了polymorphic；而`c++`，则并没有将这样的要求倾注给用户；`c++`的metaprogramming完全是由compiler实现的，Java也有编译过程，所以Java应该也支持template metaprogramming。



### Components of template metaprogramming

Template metaprogramming is [Turing-complete](https://en.wikipedia.org/wiki/Turing-complete) （图灵完全的）, meaning that any computation expressible by a computer program can be computed, in some form, by a template metaprogram.[[3\]](https://en.wikipedia.org/wiki/Template_metaprogramming#cite_note-Veldhuizen2003-3)

> NOTE: template metaprogramming的[Turing-complete](https://en.wikipedia.org/wiki/Turing-complete) 性质是非常重要的，它说明了Template metaprogramming的强大所在。

Templates are different from *macros*. A macro, which is also a compile-time language feature, generates code in-line using text manipulation and substitution. **Macro systems** often have limited compile-time **process flow**（典型的process flow为[recursion](https://en.wikipedia.org/wiki/Recursion_(computer_science))） abilities and usually lack awareness of the semantics and type system of their companion language (a notable exception being [Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language)) macros, which, as a result of Lisp's [homoiconicity](https://en.wikipedia.org/wiki/Homoiconicity) (同像性), are written in Lisp itself and work by directly elaborating the [abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) of the to-be generated code, rather than by producing and substituting text to be parsed and compiled in [later steps](https://en.wikipedia.org/wiki/Compile_time)).

> NOTE: 上面这段话总结了template和Marco之间的异同。

**Template metaprograms** have no [mutable variables](https://en.wikipedia.org/wiki/Immutable_object)— that is, no variable can change value once it has been initialized, therefore **template metaprogramming** can be seen as a form of [functional programming](https://en.wikipedia.org/wiki/Functional_programming). In fact many template implementations implement **flow control** only through [recursion](https://en.wikipedia.org/wiki/Recursion_(computer_science)), as seen in the example below.

> NOTE: 在`./TMP-and-FP`中，会对template metaprogram和functional programming之间的关联进行分析。
>



#### Using template metaprogramming

Though the syntax of **template metaprogramming** is usually very different from the programming language it is used with, it has practical uses. Some common reasons to use templates are to implement **generic programming** (avoiding sections of code which are similar except for some minor variations) or to perform **automatic compile-time optimization** such as doing something once at **compile time** rather than every time the program is run — for instance, by having the compiler **unroll loops** to eliminate jumps（联想for-loop的流程图，可以想到此处的jump所指为每轮for-loop结束后，jump到循环开始的地方，然后继续执行） and loop count decrements whenever the program is executed.

> NOTE: 这一段说明的是使用metaprogramming的原因所在。

### Compile-time class generation

What exactly "programming at compile-time" means can be illustrated with an example of a factorial function, which in non-template C++ can be written using recursion as follows:

```c++
unsigned int factorial(unsigned int n) {
	return n == 0 ? 1 : n * factorial(n - 1); 
}

// Usage examples:
// factorial(0) would yield 1;
// factorial(4) would yield 24.
```

The code above will execute at run time to determine the factorial value of the literals 4 and 0. By using **template metaprogramming** and **template specialization** to provide the **ending condition** for the recursion, the factorials(阶数) used in the program—ignoring any factorial not used—can be calculated at compile time by this code:

```c++
template <unsigned int n>
struct factorial {
	enum { value = n * factorial<n - 1>::value };
};

template <>
struct factorial<0> {
	enum { value = 1 };
};

// Usage examples:
// factorial<0>::value would yield 1;
// factorial<4>::value would yield 24.
```

The code above calculates the factorial value of the literals 4 and 0 at **compile time** and uses the results as if they were **precalculated constants**. To be able to use templates in this manner, the compiler must know the value of its parameters at **compile time**, which has the natural precondition that `factorial<X>::value `can only be used if `X` is known at **compile time**. In other words, `X `must be a **constant literal** or a **constant expression**.

> NOTE: 在维基百科[Compile time function execution](https://en.wikipedia.org/wiki/Compile_time_function_execution)中，给出了c++11和c++14中，上述例子的书写方法。

### Compile-time code optimization

See also: [Compile time function execution](https://en.wikipedia.org/wiki/Compile_time_function_execution)

The factorial example above is one example of **compile-time code optimization** in that all factorials used by the program are **pre-compiled** and injected as **numeric constants** at compilation, saving both run-time overhead and memory footprint. It is, however, a relatively minor optimization.

As another, more significant, example of compile-time [loop unrolling](https://en.wikipedia.org/wiki/Loop_unrolling), template metaprogramming can be used to create length-*n* vector classes (where *n* is known at compile time). The benefit over a more traditional length-*n* vector is that the loops can be unrolled, resulting in very optimized code. As an example, consider the addition operator. A length-*n* vector addition might be written as

```c++
template <int length>
Vector<length>& Vector<length>::operator+=(const Vector<length>& rhs) 
{
    for (int i = 0; i < length; ++i)
        value[i] += rhs.value[i];
    return *this;
}
```

When the compiler instantiates the function template defined above, the following code may be produced:[*citation needed*]

```c++
template <>
Vector<2>& Vector<2>::operator+=(const Vector<2>& rhs) 
{
    value[0] += rhs.value[0];
    value[1] += rhs.value[1];
    return *this;
}
```

The compiler's optimizer should be able to unroll the `for` loop because the template parameter `length` is a constant at compile time.

However, take care and exercise caution as this may cause code bloat as separate unrolled code will be generated for each 'N'(vector size) you instantiate with.



### Static polymorphism

[Polymorphism](https://en.wikipedia.org/wiki/Type_polymorphism) is a common standard programming facility where **derived objects** can be used as instances of their base object but where the derived objects' methods will be invoked, as in this code

```c++
class Base
{
public:
    virtual void method() { std::cout << "Base"; }
    virtual ~Base() {}
};

class Derived : public Base
{
public:
    virtual void method() { std::cout << "Derived"; }
};

int main()
{
    Base *pBase = new Derived;
    pBase->method(); //outputs "Derived"
    delete pBase;
    return 0;
}
```

where all invocations of `virtual` methods will be those of the most-derived class. This *dynamically polymorphic* behaviour is (typically) obtained by the creation of [virtual look-up tables](https://en.wikipedia.org/wiki/Vtable) for classes with virtual methods, tables that are traversed at run time to identify the method to be invoked. Thus, *run-time polymorphism* necessarily entails execution overhead (though on modern architectures the overhead is small).

However, in many cases the polymorphic behaviour needed is invariant and can be determined at compile time. Then the [Curiously Recurring Template Pattern](https://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern) (CRTP) can be used to achieve **static polymorphism**, which is an imitation of polymorphism in programming code but which is resolved at compile time and thus does away with run-time virtual-table lookups. For example:

```c++
template <class Derived>
struct base
{
    void interface()
    {
         // ...
         static_cast<Derived*>(this)->implementation();
         // ...
    }
};

struct derived : base<derived>
{
     void implementation()
     {
         // ...
     }
};
```

Here the base class template will take advantage of the fact that **member function bodies** are not instantiated until after their declarations, and it will use members of the derived class within its own member functions, via the use of a `static_cast`, thus at compilation generating an object composition with polymorphic characteristics. As an example of real-world usage, the CRTP is used in the [Boost](https://en.wikipedia.org/wiki/Boost_library) [iterator](https://en.wikipedia.org/wiki/Iterator) library.[[5\]](https://en.wikipedia.org/wiki/Template_metaprogramming#cite_note-5)

Another similar use is the "[Barton–Nackman trick](https://en.wikipedia.org/wiki/Barton%E2%80%93Nackman_trick)", sometimes referred to as "restricted template expansion", where common functionality can be placed in a base class that is used not as a contract but as a necessary component to enforce conformant behaviour while minimising code redundancy.

### Benefits and drawbacks of template metaprogramming



## Template is a kind of abstraction

类是一种抽象，模板也是一种抽象，模板类是抽象的抽象。





## SUMMARY of C++ template metaprogramming

从meta programming的角度来看，c++ program可以分为

- c++ program
- metaprogram，metaprogram是compiler-time computation

在`Theory\Programming-paradigm\Metaprogramming\Metaprogramming.md`中对这种观点进行了总结。

在下面文章中使用了这个观点

microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019)中有这样的描述

> To enable compilers as well as **C++ programs** and **metaprograms** to reason about the suitability of any given type for operations that depend on a particular memory layout, C++14 introduced three categories of simple classes and structs

文章galowicz [What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)中对这个观点进行了非常深刻的介绍。



### Short Intro to C++ metaprogramming

在阅读文章galowicz [What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)时，其中的Short Intro段对c++的metaprogramming进行了较好的总结：

> Looking at complex C++ meta programs, which appear seemlessly（无缝的） embedded into normal program code, is confusing at first. It is like looking at brain fuck code. That is, because the **meta programming syntax** is ugly and bloated. *Why is it so ugly?* The answer is simply, that C++ was not designed from the very beginning to contain a **meta programming language**. The language evolved, controlled by a consortium(联盟) which always tried to keep newer language standards backwards compatible to older ones. One day, people realized, that this growing little template engine is actually *Turing complete*. Soon, people started to write really crazy **meta programs** with it, which were able to elevate(提升) implementations of C++ libraries to a level of unprecedented **usability**, **versatility**(多功能性) and **elegance** (from the perspective of a user which has not seen the implementation). Data structures and functions can be implemented in a way, where they do magic of all kinds on any user provided type - with no overhead at **runtime**, because all the magic happens at compile time, resulting in completely fixed and optimized run time code.



`constexpr` function、trait等都是meta program的典型例子，在c++中，meta program是compile-time computation。我们将它们嵌入到normal program code中，来对program的生成进行控制，比如可以实现polymorphism，它们给予了C++非常强大的灵活性。



### Application of template metaprogramming

维基百科[Template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming)中已经总结了部分Application of template metaprogramming，下面是一些补充内容：

#### SFINAE

- 控制overload resolution，即function template SFINAE，在`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if\SFINAE\Function-template-SFINAE.md`中对此进行了分析
- 控制template specification，即class template SFINAE，在`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if\SFINAE\Clasas-template-SFINAE.md`中对此进行了分析





### Idiom of template metaprogramming

在`C-family-language\C++\Idiom\Template-metaprogramming`中介绍了Idiom of template metaprogramming。





