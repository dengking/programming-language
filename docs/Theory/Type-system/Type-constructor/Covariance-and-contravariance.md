# Covariance and contravariance

"variance"是type constructor的一个非常重要的属性，本文对此进行描述。

"covariance"的含义是"协变"，"contravariance"的含义是"逆变"。"variance"的含义是"可变"，"invariance"的含义是"不可变"。

## csdn [对协变和逆变的理解(Contravariance and Covariance)](https://blog.csdn.net/zj547877350/article/details/53050101)

```C++
object[] objects = new string[2]; //Java
  
object[] objects = new string[2];  //C#
```

这就是**协变**，清注意对象的类型发生了变换，`C#`和Java都是支持 `数组协变` 的语言。

我们都知道`C#`和Java中`String`类型都是继承自`Object`的，姑且记做`String ≦ Object`，表示`String`是`Object`的**子类型**，`String`的对象可以赋给`Object`的对象。

而`Object`的数组类型`Object[]`，我们可以理解成是由`Object`构造出来的一种新的类型, 可以认为是一种`构造类型`，记`f(Object)`（可以类比下初中数学中函数的定义），那么我们可以这么来描述**协变**和**逆变**：

- 当`A ≦ B`时,如果有`f(A) ≦ f(B)`,那么f叫做`协变` **covariance** ；
- 当`A ≦ B`时,如果有`f(B) ≦ f(A)`,那么f叫做`逆变` **contravariance**；
- 如果上面两种关系都不成立则叫做`不可变` **invariant** ;

其实顾名思义，**协变**和**逆变**表示的一种类型转变的关系：“构造类型”之间相对“子类型”之间的一种关系。

“协”表示一种自然而然的转换关系，比如上面的`String[] ≦ Object[]`，这就是大家学习面向对象编程语言中经常说的：

> 子类变量能赋给父类变量，父类变量不能赋值给子类变量。

而“逆”则不那么直观，平时用的也很少，后面讲`Java泛型中的协变和逆变`会看到例子。

`不可变`的例子就很多了，比如Java中`List< Object >`和`List< String >`之间就是不可变的。

```java
List<String> list1 = new ArrayList<String>();
List<Object> list2 = list1;
```

这两行代码在Java中肯定是编译不过的，反过来更不可能，C#中也是一样。

那么`协变`和`逆变`作用到底是什么呢？我个人肤浅的理解：主要是语言设计的一种考量，目的是为了增加语言的灵活性和能力。

### 返回值协变和参数逆变

上面提到的Java和`C#`语言都没有把函数作为一等公民(first-class function)，那么那些支持一等函数的语言，即把函数也看做一种类型是如何支持**协变**和**逆变**的以及**里氏原则**的呢？

也就是什么时候用一个函数`g`能够替代其他使用函数`f`的地方。答案是：

> 函数`f`可以安全替换函数`g`，如果与函数`g`相比，函数`f`接受更一般的参数类型，返回更特化的结果类型。《维基百科》

这就是是所谓的`对输入类型是逆变的而对输出类型是协变的`*[Luca Cardelli提出的规则](http://lucacardelli.name/Papers/Inheritance (Semantics of Data Types).pdf)*。

虽然Java是面向对象的语言，但某种程度上它仍然遵守这个规则，见上一节的例子，这叫做`返回值协变`，Java子类覆盖父类方法的时候能够返回一个“更窄”的子类型，所以说Java是一门可以支持**返回值协变**的语言。

类似`参数逆变`是指子类覆盖父类方法时接受一个“更宽”的父类型。在Java和`C#`中这都被当作了`方法重载`。

可能到这又绕糊涂了，`返回值协变`和`参数逆变`又是什么东东？回头看看协变和逆变的理解。把方法当成一等公民： 
构造类型：`Apple ≦ Fruit` 
返回值：`String ≦ Object` 
参数：`Object ≧ String`

以上都是我个人对协变和逆变这两个概念的理解（欢迎拍砖）。



## 维基百科[Covariance and contravariance (computer science)](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science))

Many [programming language](https://en.wikipedia.org/wiki/Programming_language) [type systems](https://en.wikipedia.org/wiki/Type_system) support [subtyping](https://en.wikipedia.org/wiki/Subtyping). For instance, if the type `Cat` is a subtype of `Animal`, then an expression of type `Cat` [should be substitutable](https://en.wikipedia.org/wiki/Liskov_substitution_principle) wherever an expression of type `Animal` is used.

> NOTE: 直观感觉是: 将 [Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle) 进行推广。



**Variance** refers to how subtyping between more complex types relates to subtyping between their components. For example, how should a list of `Cat`s relate to a list of `Animal`s? Or how should a function that returns `Cat` relate to a function that returns `Animal`?

> NOTE: **variance** 所描述的是: 更复杂类型之间的subtyping关系(subtyping between more complex types) 和 它们的component的subtyping关系的关联。上面列举了两个例子来说明这个问题:
>
> 1) `Cat`和`Animal`之间有subtyping关系，那list of `Cat`s 和 list of `Animal`s之间是否沿用 `Cat`和`Animal`之间的subtyping关系呢？
>
> 2) `Cat`和`Animal`之间有subtyping关系，那function that returns `Cat` 和 function that returns `Animal`之间的关系呢？

Depending on the variance of the [type constructor](https://en.wikipedia.org/wiki/Type_constructor), the subtyping relation of the simple types may be either preserved(保留), reversed(反转), or ignored(忽视) for the respective complex types.

> NOTE: 这段话的意思是: variance of the [type constructor](https://en.wikipedia.org/wiki/Type_constructor) 决定了 complex types 是 preserve、reverse还是ignore "subtyping relation of the simple types"。
>
> **covariance**对应的是: preserved
>
> **contravariance**对应的是: reverse
>
> type constructor其实是一个比较容易理解的概念，下面是下面章节的一些例子:
>
> - **list type constructor**
> - **function type constructor** 

In the [OCaml](https://en.wikipedia.org/wiki/OCaml) programming language, for example, "`list` of `Cat`" is a subtype of "`list` of `Animal`" because the **list type constructor** is **covariant**. This means that the subtyping relation of the simple types are preserved for the complex types.

> NOTE: 这段话以OCaml语言中的 **list type constructor** 为例，介绍了covariant的含义。

On the other hand, "function from `Animal` to `String`" is a **subtype** of "function from `Cat` to `String`" because the **function type constructor** is **contravariant** in the parameter type. Here the subtyping relation of the simple types is reversed for the complex types.

> NOTE: 这段话以 **function type constructor**为例，介绍了contravariant。



A programming language designer will consider variance when devising(设计) typing rules for language features such as arrays, inheritance, and [generic datatypes](https://en.wikipedia.org/wiki/Generic_datatype). By making **type constructors** **covariant** or **contravariant** instead of **invariant**, more programs will be accepted as well-typed. On the other hand, programmers often find contravariance unintuitive(不直观), and accurately tracking variance to avoid runtime type errors can lead to complex typing rules.

### Formal definition

Within the [type system](https://en.wikipedia.org/wiki/Type_system) of a [programming language](https://en.wikipedia.org/wiki/Programming_language), a typing rule or a type constructor is:

- *covariant* if it preserves the [ordering of types (≤)](https://en.wikipedia.org/wiki/Subtype), which orders types from more specific to more generic;
- *contravariant* if it reverses this ordering;
- *bivariant* if both of these apply (i.e., both `I<A>` ≤ `I<B>` and `I<B>` ≤ `I<A>` at the same time);[[1\]](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)#cite_note-1)
- *variant* if covariant, contravariant or bivariant;
- *invariant* or *nonvariant* if not variant.

The article considers how this applies to some common type constructors.

#### C`#` examples

For example, in [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)), if `Cat` is a subtype of `Animal`, then:

- `IEnumerable<Cat>` is a subtype of `IEnumerable<Animal>`. The subtyping is preserved because `IEnumerable<T>` is **covariant** on `T`.
- `Action<Animal>` is a subtype of `Action<Cat>`. The subtyping is reversed because `Action<T>` is **contravariant** on `T`.
- Neither `IList<Cat>` nor `IList<Animal>` is a subtype of the other, because `IList<T>` is **invariant** on `T`.

### Arrays

#### Covariant arrays in Java and `C#`

### Function types

Languages with [first-class functions](https://en.wikipedia.org/wiki/First-class_function) have [function types](https://en.wikipedia.org/wiki/Function_type) like "a function expecting a `Cat` and returning an `Animal`" (written `Cat -> Animal` in [OCaml](https://en.wikipedia.org/wiki/OCaml) syntax or `Func<Cat,Animal>` in [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) syntax).

Those languages also need to specify when one function type is a **subtype** of another—that is, when it is safe to use a function of one type in a context that expects a function of a different type. It is safe to substitute a function *f* for a function *g* if *f* accepts a more **general** type of argument and returns a more **specific** type than *g*. For example, functions of type `Animal -> Cat`, `Cat -> Cat`, and `Animal -> Animal` can be used wherever a `Cat -> Animal` was expected. (One can compare this to the [robustness principle](https://en.wikipedia.org/wiki/Robustness_principle) of communication: "be liberal in what you accept and conservative in what you produce.") The general rule is:
$$
S_{1}\rightarrow S_{2}\leq T_{1}\rightarrow T_{2} \quad if \quad T_{1}\leq S_{1} \quad and \quad S_{2}\leq T_{2}
$$


Using [inference rule notation](https://en.wikipedia.org/wiki/Rule_of_inference) the same rule can be written as:
$$
T_{1}\leq S_{1}\quad S_{2}\leq T_{2} \over S_{1}\rightarrow S_{2}\leq T_{1}\rightarrow T_{2}
$$


In other words, the **→ type constructor** is *contravariant in the input type* and *covariant in the output type*. This rule was first stated formally by [John C. Reynolds](https://en.wikipedia.org/wiki/John_C._Reynolds),[[3\]](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)#cite_note-3) and further popularized in a paper by [Luca Cardelli](https://en.wikipedia.org/wiki/Luca_Cardelli).[[4\]](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)#cite_note-4)

> NOTE: 上述结论是非常重要的

When dealing with [functions that take functions as arguments](https://en.wikipedia.org/wiki/Higher-order_function) (Higher-order function), this rule can be applied several times. 

> NOTE: 原文的这段话的后半段是没有读懂的

### Inheritance in object-oriented languages

When a subclass [overrides](https://en.wikipedia.org/wiki/Method_overriding) a method in a superclass, the compiler must check that the overriding method has the right type. 

While some languages require that the type exactly matches the type in the superclass (invariance), it is also type safe to allow the overriding method to have a "better" type. 

By the usual subtyping rule for function types, this means that the overriding method should return a more specific type (**return type covariance**), and accept a more general argument (**parameter type contravariance**). 

In [UML](https://en.wikipedia.org/wiki/Unified_Modeling_Language) notation, the possibilities are as follows:

**Variance and method overriding: overview**

|                                                              |                                                              |                                                              |                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d6/Vererbung_T.svg/120px-Vererbung_T.svg.png)](https://en.wikipedia.org/wiki/File:Vererbung_T.svg) | [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/0/00/Inheritance_invariant.svg/120px-Inheritance_invariant.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_invariant.svg) | [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/2/27/Inheritance_covariant_return.svg/120px-Inheritance_covariant_return.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_covariant_return.svg) | [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/2/2c/Inheritance_contravariant_argument.svg/120px-Inheritance_contravariant_argument.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_contravariant_argument.svg) | [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/2/2c/Inheritance_covariant_argument.svg/120px-Inheritance_covariant_argument.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_covariant_argument.svg) |
| Subtyping of the parameter/return type of the method.        | *Invariance*. The signature of the overriding method is unchanged. | *Covariant return type*. The subtyping relation is in the same direction as the relation between ClassA and ClassB. | *Contravariant parameter type*. The subtyping relation is in the opposite direction to the relation between ClassA and ClassB. | *Covariant parameter type*. Not type safe.                   |

For a concrete example, suppose we are writing a class to model an [animal shelter](https://en.wikipedia.org/wiki/Animal_shelter). We assume that `Cat` is a subclass of `Animal`, and that we have a base class (using Java syntax)

[![UML diagram](https://upload.wikimedia.org/wikipedia/commons/thumb/5/55/AnimalShelter.svg/170px-AnimalShelter.svg.png)](https://en.wikipedia.org/wiki/File:AnimalShelter.svg)

```java
class AnimalShelter {

    Animal getAnimalForAdoption() {
        // ...
    }
    
    void putAnimal(Animal animal) {
        //...
    }
}
```

Now the question is: if we subclass `AnimalShelter`, what types are we allowed to give to `getAnimalForAdoption` and `putAnimal`?

#### Covariant method return type

In a language which allows [covariant return types](https://en.wikipedia.org/wiki/Covariant_return_type), a derived class can override the `getAnimalForAdoption` method to return a more specific type:

[![UML diagram](https://upload.wikimedia.org/wikipedia/commons/thumb/b/b1/Inheritance_covariant_return_animalshelter.svg/170px-Inheritance_covariant_return_animalshelter.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_covariant_return_animalshelter.svg)

```java
class CatShelter extends AnimalShelter {

    Cat getAnimalForAdoption() {
        return new Cat();
    }
}
```

Among mainstream OO languages, [Java](https://en.wikipedia.org/wiki/Java_(Programming_language)) and [C++](https://en.wikipedia.org/wiki/C%2B%2B) support **covariant return types**, while [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) does not. Adding the covariant return type was one of the first modifications of the C++ language approved by the standards committee in 1998.[[5\]](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)#cite_note-5) [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)) and [D](https://en.wikipedia.org/wiki/D_(programming_language)) also support covariant return types.

#### Contravariant method parameter type

Similarly, it is type safe to allow an overriding method to accept a more general argument than the method in the base class:

[![UML diagram](https://upload.wikimedia.org/wikipedia/commons/thumb/3/30/Inheritance_contravariant_argument_animalshelter.svg/170px-Inheritance_contravariant_argument_animalshelter.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_contravariant_argument_animalshelter.svg)

```java
class CatShelter extends AnimalShelter {
    void putAnimal(Object animal) {
        // ...
    }
}
```

Not many object-oriented languages actually allow this. `C++` and Java would interpret this as an unrelated method with an [overloaded](https://en.wikipedia.org/wiki/Function_overloading) name.



#### Covariant method parameter type

A couple of mainstream languages, [Eiffel](https://en.wikipedia.org/wiki/Eiffel_(programming_language)) and [Dart](https://en.wikipedia.org/wiki/Dart_(programming_language))[[6\]](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)#cite_note-Fixing_Common_Type_Problems_|_Dart-6) allow the parameters of an overriding method to have a *more* specific type than the method in the superclass (parameter type covariance). Thus, the following Dart code would type check, with `putAnimal` overriding the method in the base class:

[![UML diagram](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a5/Inheritance_covariant_argument_animalshelter.svg/170px-Inheritance_covariant_argument_animalshelter.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_covariant_argument_animalshelter.svg)

```java
class CatShelter extends AnimalShelter {

    void putAnimal(covariant Cat animal) {
        // ...
    }
}
```

This is not type safe. By up-casting a `CatShelter` to an `AnimalShelter`, one can try to place a dog in a cat shelter. That does not meet `CatShelter` parameter restrictions, and will result in a runtime error.

> NOTE: 上述up-casting所对应的是 [Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle) ；在基类`AnimalShelter`中`putAnimal(Animal animal)`的入参类型为`Animal` ，因此能够接收dog ，但是这显然是不符合`CatShelter`的`putAnimal(covariant Cat animal)`。显然这就是导致了 runtime error 。显然这是Eiffel language层面的问题。

Despite the **type safety** problem, the Eiffel designers consider **covariant parameter types** crucial(至关重要的) for modeling real world requirements.[[8\]](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)#cite_note-competentCompilers-8) The cat shelter illustrates a common phenomenon: it is *a kind of* animal shelter but has *additional restrictions*, and it seems reasonable to use inheritance and restricted parameter types to model this. In proposing this use of inheritance, the Eiffel designers reject the [Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle), which states that objects of subclasses should always be less restricted than objects of their superclass.

> NOTE: 从上面的讨论可以看出，Covariant method parameter type 和  [Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle) 是无法兼得的。如何兼得呢？在后面的"Avoiding the need for covariant parameter types"章节对此进行了说明。



Another example where **covariant parameters** seem helpful is so-called **binary methods**, i.e. methods where the parameter is expected to be of the same type as the object the method is called on(希望parameter和调用method的object的type是同类的). An example is the `compareTo` method: `a.compareTo(b)` checks whether `a` comes before or after `b` in some ordering, but the way to compare, say, two rational numbers will be different from the way to compare two strings. Other common examples of **binary methods** include equality tests, arithmetic operations, and set operations like subset and union.

In older versions of Java, the comparison method was specified as an interface `Comparable`:

```java
interface Comparable {

    int compareTo(Object o);
}
```

The drawback of this is that the method is specified to take an argument of type `Object`. A typical implementation would first down-cast this argument (throwing an error if it is not of the expected type):

```java
class RationalNumber implements Comparable {
    int numerator;
    int denominator;
    // ...
 
    public int compareTo(Object other) {
        RationalNumber otherNum = (RationalNumber)other;
        return Integer.compare(numerator * otherNum.denominator,
                               otherNum.numerator * denominator);
    }
}
```

In a language with **covariant parameters**, the argument to `compareTo` could be directly given the desired type `RationalNumber`, hiding the **typecast**. (Of course, this would still give a runtime error if `compareTo` was then called on e.g. a `String`.)

#### Avoiding the need for covariant parameter types

> NOTE: 当代programming language提出的解决"Covariant method parameter type"问题的方案，它使得programmer能够兼得**apparent benefits of covariant parameters** 和 **Liskov substitutability**。

Other language features can provide the **apparent benefits of covariant parameters** while preserving **Liskov substitutability**.

In a language with *generics* (a.k.a. [parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)) and [bounded quantification](https://en.wikipedia.org/wiki/Bounded_quantification), the previous examples can be written in a type-safe way.[[9\]](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)#cite_note-9) Instead of defining `AnimalShelter`, we define a parameterized class `Shelter<T>`. (One drawback of this is that the implementer of the base class needs to foresee(预见) which types will need to be specialized in the subclasses.)

```C++
class Shelter<T extends Animal> {

    T getAnimalForAdoption() {
        // ...
    }

    void putAnimal(T animal) {
        // ...
    }
}

    
class CatShelter extends Shelter<Cat> {

    Cat getAnimalForAdoption() {
        // ...
    }

    void putAnimal(Cat animal) {
        // ...
    }
}
```

Similarly, in recent versions of Java the `Comparable` interface has been parameterized, which allows the downcast to be omitted in a type-safe way:

```C++
class RationalNumber implements Comparable<RationalNumber> {

    int numerator;
    int denominator;
    // ...
         
    public int compareTo(RationalNumber otherNum) {
        return Integer.compare(numerator * otherNum.denominator, 
                               otherNum.numerator * denominator);
    
```



Another language feature that can help is *multiple dispatch*. One reason that binary methods are awkward to write is that in a call like `a.compareTo(b)`, selecting the correct implementation of `compareTo` really depends on the **runtime type** of both `a` and `b`, but in a conventional OO language only the runtime type of `a` is taken into account. In a language with [Common Lisp Object System](https://en.wikipedia.org/wiki/Common_Lisp_Object_System) (CLOS)-style [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch), the comparison method could be written as a **generic function** where both arguments are used for **method selection**.

Giuseppe Castagna[[10\]](https://en.wikipedia.org/wiki/Covariance_and_contravariance_(computer_science)#cite_note-10) observed that in a typed language with **multiple dispatch**, a **generic function** can have some parameters which control **dispatch** and some "left-over"(剩下的) parameters which do not. Because the method **selection rule** chooses the **most specific applicable method**, if a method **overrides** another method, then the overriding method will have more **specific types** for the controlling parameters. On the other hand, to ensure type safety the language still must require the left-over parameters to be at least as general. Using the previous terminology, types used for **runtime method selection** are **covariant** while types not used for runtime method selection of the method are **contravariant**. Conventional **single-dispatch** languages like Java also obey this rule: only one argument is used for **method selection** (the receiver object, passed along to a method as the hidden argument `this`), and indeed the type of `this` is more specialized inside overriding methods than in the superclass.

Castagna suggests that examples where covariant parameter types are superior (particularly, binary methods) should be handled using **multiple dispatch**; which is naturally **covariant**. However, most programming languages do not support **multiple dispatch**.

> NOTE: Giuseppe Castagna的分析所强调的是: 对于支持covariant parameter types的programming language，应该使用multiple dispatch来保证安全。

#### Summary of variance and inheritance

|                                                              | Parameter type |                Return type                |
| :----------------------------------------------------------: | :------------: | :---------------------------------------: |
| [C++](https://en.wikipedia.org/wiki/C%2B%2B) (since 1998), [Java](https://en.wikipedia.org/wiki/Java_(programming_language)) (since [J2SE 5.0](https://en.wikipedia.org/wiki/Java_Platform,_Standard_Edition)), [D](https://en.wikipedia.org/wiki/D_(programming_language)) |   Invariant    |                 Covariant                 |
| [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) |   Invariant    | Covariant (since C# 9 - before Invariant) |
| [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)), [Sather](https://en.wikipedia.org/wiki/Sather) | Contravariant  |                 Covariant                 |
| [Eiffel](https://en.wikipedia.org/wiki/Eiffel_(programming_language)) |   Covariant    |                 Covariant                 |

### Generic types

In programming languages that support **generics** (a.k.a. [parametric polymorphism](https://en.wikipedia.org/wiki/Parametric_polymorphism)), the programmer can extend the type system with new **constructors**. For example, a `C#` interface like `IList<T>` makes it possible to construct new types like `IList<Animal>` or `IList<Cat>`. The question then arises what the **variance** of these type constructors should be.

There are two main approaches. 

1) In languages with *declaration-site variance annotations* (e.g., [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language))), the programmer annotates the definition of a **generic type** with the intended variance of its type parameters. 

2) With *use-site variance annotations* (e.g., [Java](https://en.wikipedia.org/wiki/Java_(programming_language))), the programmer instead annotates the places where a **generic type** is instantiated.

#### Declaration-site variance annotations

The most popular languages with declaration-site variance annotations are [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) and [Kotlin](https://en.wikipedia.org/wiki/Kotlin_(programming_language)) (using the keywords `out` and `in`), and [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)) and [OCaml](https://en.wikipedia.org/wiki/OCaml) (using the keywords `+` and `-`). `C#` only allows **variance annotations** for **interface types**, while Kotlin, Scala and OCaml allow them for both interface types and concrete data types.

#### Use-site variance annotations (wildcards)

> NOTE: 暂时不去了解



## wikipedia [Covariant return type](https://en.wikipedia.org/wiki/Covariant_return_type)

