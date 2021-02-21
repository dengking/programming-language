# rivermatrix [C++ Templates with Virtual Functions?](https://rivermatrix.com/soft-eng/cpp/2014/10/27/c-templates-with-virtual-functions/)

> NOTE: 
>
> 1、这篇文章，存在一定的错误，质量一般 ，但是对使用policy-based design来实现virtual method template的介绍还是不错的
>
> 

C++ offers two forms of polymorphism: 

1、virtual functions and overrides / implementations

2、templates. 

There can be cases when it makes the most sense to essentially have both. You can’t do this though.  C++ expressly forbids **virtual template functions** because the **virtual tables** that would have to be built are way too complex. Luckily, C++ offers a way around this. It’s a programming paradigm called **Policy Based Design**. In this article, I’ll cover why you might need this, and how it works.

I recently was presented with a case where I needed a class that had common operations to be performed on varying different type implementations, so I implemented it as a template.

```C++
#include<string>
template<typename type = "">
class Loader
{
public:
	Loader(Type *const value, const std::string refName);
	std::string getRefName() const;
	Type* const getType() const;
private:
	Type *value;
	std::string name;
};
```



> NOTE: 原文给出的是存在明显的错误，下面是我修正的版本:
>
> ```C++
> #include <string>
> template<typename Type>
> class Loader
> {
> public:
> 	Loader(Type *const value, const std::string refName);
> 	std::string getRefName() const;
> 	Type* const getType() const;
> private:
> 	Type *value;
> 	std::string name;
> };
> 
> ```
>
> 



This is a normal thing in C++, no big deal. I then realized though that there were a number of operations that needed to be specific for different implementations. Now, normally I might handle this by specializing the template like below:

> NOTE: 
>
> "I then realized though that there were a number of operations that needed to be specific for different implementations"，这段话的简单的含义是: 想要实现polymorphism。
>
> "Now, normally I might handle this by specializing the template like below"，这段话的意思是: 通过specialization来实现

```C++
template<typename type="">
std::string Loader::getRefName() const {
    return "Irrational Number " + name;
}
```

But I needed more than just specialization. What I really needed was an interface that I could extend, and then implement the extension. The only way to do this is to have some sort of virtual function in the template that could be inherited or overrode.  But C++ forbids this, and for good reason.

> NOTE: 
>
> "The only way to do this is to have some sort of virtual function in the template that could be inherited or overrode" ，这段话所指的应该是: virtual method template，而不是**Class template中定义non-template virtual method**

## Policy-based design

There is hope though. In 2001, a gentleman by the name of Andrei Alexandrescu wrote about a new paradigm called [Policy Based Design](http://en.wikipedia.org/wiki/Policy-based_design). The essentials are that you write your **base / interface template**, and have an extra template parameter for the **implementer of the interface**. Then any of the methods you want to act as a method to be implemented, you write a call on the implementer for the method of that signature.

```c++
#include <string>
template<typename type = "">
class Loader
{
public:
	Loader(Type *const value, const std::string refName);
	std::string getRefName() const;
	Type* const getType() const;
	Type* const load(int itemNum)
	{
		return impl.load(itemNum);
	}
private:
	Implementer impl;
	Type *value;
	std::string name;
};

```

> NOTE: 原文给出的是存在明显的错误，下面是我修正的版本:
>
> ```C++
> #include <string>
> template<typename Type, typename Implementer>
> class Loader
> {
> public:
> 	Loader(Type *const value, const std::string refName);
> 	std::string getRefName() const;
> 	Type* const getType() const;
> 	Type* const load(int itemNum)
> 	{
> 		return impl.load(itemNum);
> 	}
> private:
> 	Implementer impl;
> 	Type *value;
> 	std::string name;
> };
> 
> ```
>
> 

How in the world does this work? During precompilation, the compiler adds a note to look for a method with a signature similar enough to how it was used in the method definition that at least with a cast, it can complete the call. Then, if it finds it during compilation, it substitutes that method call in. If not, it throws an error. Using the example above to explain this, the compiler sees line 8, and will make a note to look in any Implementer for a method that returns the type specified as the first template parameter called load that takes an integer as its argument.

### Flexibility in class design 

This paradigm gives a lot of extra flexibility in class design in C++. For example, you can have multiple extensions of the interface provided by the base class. Another example is in defining the specific implementation to use when you compile. You don’t need this paradigm to do this, but it does make it easier.

```C++
/* LoaderConfig.h */
 
typedef ImaginaryNumberFileSystemImpl ImaginaryNumberLoaderImpl;
typedef IrrationalNumberFileSystemImpl IrrationalNumberLoaderImpl;
```

```C++
/* ImaginaryNumberLoader */
 
typedef Loader<imaginarynumber, imaginarynumberloaderimpl=""> ImaginaryNumberLoader;
```

With this, I can have configurations for different builds for, say different systems, all by substituting out different versions of `LoaderConfig.h` header file. I could easily have another that uses a sockets implementation for my loaders.

As I’ve said before, there’s extra flexibility presented with this paradigm, and a lot of extra power. You might not realize all the things this allows you to do at first, but over time, with this in your toolbox, you can start to do some pretty amazing things in C++ that developers in other languages can only dream about.