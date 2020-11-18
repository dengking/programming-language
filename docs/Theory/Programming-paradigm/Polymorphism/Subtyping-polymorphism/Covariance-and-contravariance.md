# Covariance and contravariance

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



|                                                              |                                                              |                                                              |                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d6/Vererbung_T.svg/120px-Vererbung_T.svg.png)](https://en.wikipedia.org/wiki/File:Vererbung_T.svg) | [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/0/00/Inheritance_invariant.svg/120px-Inheritance_invariant.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_invariant.svg) | [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/2/27/Inheritance_covariant_return.svg/120px-Inheritance_covariant_return.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_covariant_return.svg) | [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/2/2c/Inheritance_contravariant_argument.svg/120px-Inheritance_contravariant_argument.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_contravariant_argument.svg) | [![img](https://upload.wikimedia.org/wikipedia/commons/thumb/2/2c/Inheritance_covariant_argument.svg/120px-Inheritance_covariant_argument.svg.png)](https://en.wikipedia.org/wiki/File:Inheritance_covariant_argument.svg) |
| Subtyping of the parameter/return type of the method.        | *Invariance*. The signature of the overriding method is unchanged. | *Covariant return type*. The subtyping relation is in the same direction as the relation between ClassA and ClassB. | *Contravariant parameter type*. The subtyping relation is in the opposite direction to the relation between ClassA and ClassB. | *Covariant parameter type*. Not type safe.                   |

  



## wikipedia [Covariant return type](https://en.wikipedia.org/wiki/Covariant_return_type)

