# gotw#24 [Compilation Firewalls](http://www.gotw.ca/gotw/024.htm)

*Using the Pimpl Idiom can dramatically reduce code interdependencies and build times. But what should go into a `pimpl_` object, and what is the safest way to use it?*

## Problem

In C++, when anything in a class definition changes (even private members) all users of that class must be recompiled. To reduce these dependencies, a common technique is to use an **opaque pointer** to hide some of the implementation details:

```C++
class X
{
public:
	/* ... public members ... */
protected:
	/* ... protected members? ... */
private:
	/* ... private members? ... */
	class XImpl *pimpl_;  // opaque pointer to
						  // forward-declared class
};

```

## Questions

**1.** What should go into `XImpl`? There are four common disciplines, including:

> \- put all private data (but not functions) into `XImpl`;
>
> \- put all private members into `XImpl`;
>
> \- put all private and protected members into `XImpl`;
>
> \- make `XImpl` entirely the class that X would have been, and write X as only the public interface made up entirely of simple forwarding functions (a **handle/body** variant).

What are the advantages/drawbacks? How would you choose among them?

**2.** Does `XImpl` require a "back pointer" to the X object?

## Solution

First, two definitions:

> **visible class** : the class the client code sees and manipulates (here `X`)
>
> **pimpl** : the implementation class (here `XImpl`) hidden behind an opaque pointer (the eponymous(以……的名字命名的) `pimpl_`) in the visible class

In C++, when anything in a class definition changes (even private members) all users of that class must be recompiled. To reduce these dependencies, a common technique is to use an **opaque pointer** to hide some of the implementation details:

### Variant of the handle/body idiom

This is a variant of the **handle/body idiom**. As documented by Coplien,**[[1\]](http://www.gotw.ca/gotw/024.htm#1)** it was described as being primarily useful for reference counting of a shared implementation.

As it turns out, handle/body (in the form of what I call the "pimpl idiom" because of the intentionally pronounceable "`pimpl_`" pointer)**[[2\]](http://www.gotw.ca/gotw/024.htm#2)** is also useful for breaking compile-time dependencies, as pointed out by Lakos.**[[3\]](http://www.gotw.ca/gotw/024.htm#3)** The rest of this solution concentrates on that usage, and some of the following is not true for handle/body in general.

> NOTE: 这段话的意思是: pimpl主要是发挥了handle/body idiom的breaking compile-time dependency。

### Cost 

> NOTE: 使用pimpl的代价

The major costs of this idiom are in performance:

\1. Each construction must allocate memory. This can be mitigated using a custom allocator, but that's more work.

\2. Each access of a hidden member requires at least one extra indirection. (If the hidden member being accessed itself uses a **back pointer** to call a function in the visible class, there will be multiple indirections.)

### 1、What should go into `XImpl`? There are four common disciplines, including:

> NOTE: 这段话，作者对"Questions"中提出的question 1进行解释，由于question 1包含四个小问题，因此作者分别对这四个小问题进行了解释。

> \- put all private data (but not functions) into `XImpl`;

This is a good start, because now we can forward-declare any class which only appears as a data member (rather than `#include` the class' actual declaration, which would make client code depend on that too). Still, we can usually do better.

> \- put all private members into `XImpl`;

> NOTE: 是否应该"put all private members into `XImpl`"，作者对这个问题进行了细致的回答，大多数情况下(**almost**)，是可以的，但是有例外的情况，作者对例外的情况进行了详细说明。

This is (**almost**) my usual practice these days. After all, in C++, the phrase "client code shouldn't and doesn't care about these parts" is spelled "private," and privates are best hidden (except in some Scandinavian countries with more liberal laws).

There are two caveats, the first of which is the reason for my "almost" above:

\1. You can't hide virtual member functions in the pimpl class, even if the virtual functions are private. If the virtual function overrides one inherited from a base class, then it must appear in the actual derived class. If the virtual function is not inherited, then it must still appear in the visible class in order to be available for overriding by further derived classes.

> NOTE: 翻译如下:
>
> "不能在pimpl类中隐藏虚成员函数，即使虚函数是私有的。如果虚函数覆盖了从基类继承的函数，那么它必须出现在实际的派生类中。如果虚函数没有被继承，那么它必须仍然出现在可见类中，以便被进一步的派生类覆盖。"
>
> 上面这段话的意思是: visible class的virtual member function是不能够被移到pimpl class中的，即使它是private的，原文介绍了两个原因，简单说来就是: 能够让subclass去override virtual member function。

\2. Functions in the pimpl may require a "back pointer" to the visible object if they need to use other functions, which adds another level of indirection. By convention this back pointer is usually named `self_` where I've worked.



> \- put all private and protected members into `XImpl`;

> NOTE: 不能够将protected member放到impl class中的原因其实和前面分析的virtual member function的原因是一致的

Taking this extra step is actually wrong. Protected members should never go into a pimpl, since putting them there just emasculates(阉割) them. After all, protected members exist specifically to be seen and used by derived classes, and so aren't nearly as useful if derived classes can't see or use them.



> \- make `XImpl` entirely the class that `X` would have been, and write `X` as only the public interface made up entirely of simple forwarding functions (a handle/body variant).

This is useful in a few restricted cases, and has the benefit of avoiding a back pointer since all services are available in the pimpl class. The chief drawback is that it normally makes the visible class useless for any inheritance, as either a base or a derived class.

### 2、Does `XImpl` require a "back pointer" to the X object?

> NOTE: 通过下面的回答可以看出，在某些情况下，是需要back pointer的。

Often, unhappily, yes. After all, what we're doing is splitting each object into two halves for the purposes of hiding one part.

Whenever a function in the visible class is called, usually some function or data in the hidden half is needed to complete the request. That's fine and reasonable. What's perhaps not as obvious at first is that often a function in the pimpl must call a function in the visible class, usually because the called function is public or virtual.