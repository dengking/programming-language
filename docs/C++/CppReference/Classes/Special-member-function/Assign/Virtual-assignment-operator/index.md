# Virtual assignment operator

## 不应该使用virtual assignment operator

在下面文章中提出了这个观点:

1、sonarsource [Assignment operators should not be "virtual"](https://rules.sonarsource.com/cpp/RSPEC-3657)

2、[C++ Core Guidelines C.60](https://github.com/isocpp/CppCoreGuidelines/blob/036324/CppCoreGuidelines.md#c60-make-copy-assignment-non-virtual-take-the-parameter-by-const-and-return-by-non-const) - Make copy assignment non-virtual, take the parameter by `const&`, and return by non-const`&`

3、[C++ Core Guidelines C.63](https://github.com/isocpp/CppCoreGuidelines/blob/036324/CppCoreGuidelines.md#c63-make-move-assignment-non-virtual-take-the-parameter-by--and-return-by-non-const-) - Make move assignment non-virtual, take the parameter by `&&`, and return by non-const `&`



## sonarsource [Assignment operators should not be "virtual"](https://rules.sonarsource.com/cpp/RSPEC-3657)

C++ does not support polymorphic copy or move assignment operators. 

For example, the signature of a copy assignment operator on a "Base" class would be `Base& operator=(const Base& other)`. And on a "Derived" class that extends "Base", it would be `Derived& operator=(const Derived& other)`.

> NOTE: 
>
> 1、上述说法是片面的，还是有办法实现virtual assignment operator的，下面的

Because these are two entirely different **method signatures**, the second method does not override the first, and adding `virtual` to the "Base" signature does not change which method is called.

> NOTE: 
>
> 1、这段话让我想到了C++仅仅支持Covariant return type，并不支持 Covariant function parameter

It is possible to add an `operator=` override in a derived class, but doing so is an indication that you may need to reexamine your application architecture.

### Noncompliant Code Example

> NOTE: Noncompliant 

```C++
class Base {
public:
  virtual Base& operator=(const Base& other); // Noncompliant
};

class Derived : public Base {
public:
  Derived& operator=(const Derived& other);
};

```

> NOTE: 测试程序如下:
>
> ```C++
> class Base
> {
> public:
> 	virtual Base& operator=(const Base &other) // Noncompliant
> 	{
> 		return *this;
> 	}
> };
> 
> class Derived: public Base
> {
> public:
> 	Derived& operator=(const Derived &other)
> 	{
> 		return *this;
> 	}
> };
> int main()
> {
> 
> }
> // g++ -Wall -pedantic test.cpp && ./a.out
> 
> ```
>
> 

### Compliant Solution

> NOTE: 
>
> 1、"Compliant"的意思是:"顺从的；服从的；应允的"

```C++
class Base {
protected:
  Base& operator=(const Base& other); // not virtual
};

class Derived : public Base {
public:
  Derived& operator=(const Derived& other);
};
```

> NOTE: 测试程序如下:
>
> ```C++
> class Base
> {
> public:
> 	Base& operator=(const Base &other) // not virtual
> 	{
> 		return *this;
> 	}
> };
> 
> class Derived: public Base
> {
> public:
> 	Derived& operator=(const Derived &other)
> 	{
> 		return *this;
> 	}
> };
> int main()
> {
> 
> }
> // g++ -Wall -pedantic test.cpp && ./a.out
> 
> ```
>
> 

### See

1、[C++ Core Guidelines C.60](https://github.com/isocpp/CppCoreGuidelines/blob/036324/CppCoreGuidelines.md#c60-make-copy-assignment-non-virtual-take-the-parameter-by-const-and-return-by-non-const) - Make copy assignment non-virtual, take the parameter by `const&`, and return by non-const`&`

2、[C++ Core Guidelines C.63](https://github.com/isocpp/CppCoreGuidelines/blob/036324/CppCoreGuidelines.md#c63-make-move-assignment-non-virtual-take-the-parameter-by--and-return-by-non-const-) - Make move assignment non-virtual, take the parameter by `&&`, and return by non-const `&`



## 实现 virtual assignment operator

如果实在要使用virtual assignment operator，那如何实现呢？本节讨论这个。

在 stackoverflow [What is object slicing?](https://stackoverflow.com/questions/274626/what-is-object-slicing) # [A](https://stackoverflow.com/a/14461532) 中，给出了virtual assignment operator的实现，这篇文章收录在了`Object-slicing` 章节。



## TODO root.cer [When to use virtual assignment](https://root.cern.ch/TaligentDocs/TaligentOnline/DocumentRoot/1.0/Docs/books/WM/WM_136.html)