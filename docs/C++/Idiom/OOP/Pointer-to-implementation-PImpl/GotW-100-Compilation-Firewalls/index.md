# [GotW #100: Compilation Firewalls](https://herbsutter.com/gotw/_100/)

*[This is a C++11-updated version of the original* [*GotW #24*](http://www.gotw.ca/gotw/024.htm)*.]*

## JG Questions

\1. What is the Pimpl Idiom, and why is it useful?

## Guru Questions

\2. What is the best way to express the basic Pimpl Idiom in C++11?

\3. What parts of the class should go into the *impl* object? Some potential options include:

- put all private data (but not functions) into *impl*;
- put all private members into *impl*;
- put all private and protected members into *impl*;
- put all private nonvirtual members into *impl*;
- put everything into *impl*, and write the public class itself as only the public interface, each implemented as a simple forwarding function (a handle/body variant).

What are the advantages/drawbacks of each? How would you choose among them?

\4. Does the *impl* require a back pointer to the public object? If yes, what is the best way to provide it? If not, why not?

> NOTE: 上面这些question和[*GotW #24*](http://www.gotw.ca/gotw/024.htm) 中是一致的；

## Solution

### 1. What is the Pimpl Idiom, and why is it useful?

> NOTE: 这一节介绍的是C++ compiler的编译机制，了解这些编译机制是才能够完全掌握使用Pimpl的意图

In C++, when anything in a header file class definition changes, all users of that class must be recompiled – even if the only change was to the **private class members** that the users of the class cannot even access. This is because C++’s build model is based on **textual inclusion**, and because C++ assumes that callers know two main things about a class that can be affected by **private members**:

1、**Size and Layout:** The calling code must know the size and layout of the class, including private data members. This constraint of always being able to see implementations incurs the cost of more tightly coupling callers and callees, but is central to C++’s object model and philosophy because guaranteeing that the compiler has direct access to objects by default is an (perhaps “the”) essential ingredient in enabling C++ to achieve its famed heavily-optimizable efficiency.

> NOTE: 在C++中，calling code必须知道 "size and layout of the class, including private data members"，那C++为什么这样设计呢？
>
> 1、弊端: "incurs the cost of more tightly coupling callers and callees"
>
> 2、优势: 保证compiler能够直接access object，使得C++实现了非常好的optimization

2、**Functions:** The calling code must be able to resolve calls to member functions of the class, including inaccessible **private functions** that overload with **nonprivate functions** — if the private function is a better match, the calling code will fail to compile. (C++ took the deliberate design decision to perform **overload resolution** before **accessibility checking** for safety reasons. For example, it was felt that changing the accessibility of a function from p*rivate* to *public* shouldn’t change the meaning of legal calling code.)

> NOTE: 
>
> 1、compiler先执行overload resolution，因此如果private member function "is a better match"，compiler将会选择它；然后compiler才执行**accessibility checking**，如果选择的是private member function，则compiler会报编译错误
>
> 2、上述设计的目的是: "for safety reasons"，作者列举了一个例子: 将一个function从private转换为public不应该改变"the meaning of legal calling code"，因为只要现有的code是编译通过的，那么显然之前它编译的时候选择的肯定是一个best match，那么选择再将一个function从private转换为public，这并不会导致重新进行overload resolution时，选择一个更好的match，显然这种设计保证了一致；

To reduce these compilation dependencies, a common technique is to use an opaque pointer to hide some of the implementation details. Here’s the basic idea:

```C++
// Pimpl idiom - basic idea
class widget
{
	// :::
private:
	struct impl;        // things to be hidden go here
	impl *pimpl_;       // opaque pointer to forward-declared class
};

```

#### Advantage: breaks compile-time dependencies

One big advantage of this idiom is that it breaks compile-time dependencies. 

First, system builds run faster because using a Pimpl can eliminate extra *`#include`*s. I have worked on projects where converting just a few widely-visible classes to use Pimpls has halved the system’s build time. 

Second, it localizes the build impact of code changes because the parts of a class that reside in the Pimpl can be freely changed – that is, members can be freely added or removed – without recompiling client code. Because it’s so good at eliminating compilation cascades due to changes in only the now-hidden members, it’s often dubbed(被称为) a “compilation firewall.”

But this leaves some questions option: Should *pimpl* be a raw pointer? What should go into the Pimpl class, anyway? So let’s look at these and other important details.

### 2. What is the best way to express the basic Pimpl Idiom in C++11?

Avoid using raw pointers and explicit *delete*. To express Pimpl using only standard C++ facilities, the most appropriate choice is to hold the Pimpl object by *unique_ptr* since the Pimpl object is uniquely owned by the visible class. Using *unique_ptr* also leads to the simplest code. [3]

```C++
// in header file
class widget
{
public:
	widget();
	~widget();
private:
	class impl;
	unique_ptr<impl> pimpl;
};

// in implementation file
class widget::impl
{
	// :::
};

widget::widget() :
				pimpl { new impl { /*...*/} }
{
}
widget::~widget()
{
}

```

Note some key parts of the pattern:

> NOTE: 
>
> 1、由于 `class widget` 中，使用的是`unique_ptr<impl>`，因此下面的描述主要集中在`unique_ptr`上
>
> 2、下面的描述，基本上告诉了我们如何进行完整地实现

1、Prefer to hold the Pimpl using a *unique_ptr*. It’s more efficient than using a *shared_ptr*, and correctly expresses the intent that the Pimpl object should not be shared.

2、Define and use the Pimpl object in your own implementation file. This is what keeps its details hidden.

3、In the visible class’ **out-of-line constructor**, allocate the Pimpl object.

4、You still need to write the visible class’ destructor yourself and define it out of line in the implementation file, even if normally it’s the same as what the compiler would generate. This is because although both *unique_ptr* and *shared_ptr* can be instantiated with an incomplete type, *unique_ptr*’s destructor requires a complete type in order to invoke *delete* (unlike *shared_ptr* which captures more information when it’s constructed). By writing it yourself in the implementation file, you force it to be defined in a place where *impl* is already defined, and this successfully prevents the compiler from trying to automatically generate the destructor on demand in the caller’s code where *impl* is not defined.

> NOTE: 
>
> 1、上面这段话的分析非常好

5、The above pattern does not make the visible class either copyable or movable by default, because C++11 is less eager to have the compiler generate default copying and moving operations for you. Because we’ve had to write a user-defined destructor, that turns off the compiler-generated move constructor and move assignment operator. If you do decide to supply copy and/or move, note that the copy assignment and move assignment operator need to be defined out of line in the implementation class for the same reason as the destructor.

A new advantage to Pimpl in C++11 is that Pimpl’d types are very move-friendly types because they only have to copy a single pointer value. C’est très cool.