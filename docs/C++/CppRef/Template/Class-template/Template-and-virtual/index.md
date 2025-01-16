# Template and virtual

1、template是compile-time、static polymorphism，virtual method是runtime、dynamic polymorphism。

2、C++中template-specialization和inheritance-virtual的本质差异在于：前者不同的specialization之间不是同一种class，而后者，不同的child class之间是同一种class。

如何两者兼得呢？

## Class template and virtual method template

1、C++允许在class template中定义non-template virtual method

2、C++不允许virtual method template

why? 在下面文章中解释了原因:

a、stackoverflow [Are pure virtual methods allowed within a template class?](https://stackoverflow.com/questions/8919566/are-pure-virtual-methods-allowed-within-a-template-class) # [A](https://stackoverflow.com/a/8919588)

本文收录了这篇文章，参见下面。

b、rivermatrix [C++ TEMPLATES WITH VIRTUAL FUNCTIONS?](https://rivermatrix.com/c-templates-with-virtual-functions/)

c、CppCoreGuidelines [T.83: Do not declare a member function template virtual](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t83-do-not-declare-a-member-function-template-virtual)

C++ does not support that. If it did, vtbls could not be generated until link time. And in general, implementations must deal with dynamic linking.

**Example, don't**

```c++
class Shape {
    // ...
    template<class T>
    virtual bool intersect(T* p);   // error: template cannot be virtual
};
```



> NOTE: 
>
> 1、当前，virtual table是compile time生成的；如果支持template virtual function，那么virtual table需要直到link time才能够生成，并且还需要支持dynamic linking。
>
> 

### 验证小程序

下面是验证小程序:

**Class template中定义non-template virtual method**

```c++
#include <iostream>
template<typename T>
class C
{
  public:
    C(){};
    virtual void Set(T t){}
    T& Get()
    {
        return m_t;
    }
  protected:
    T m_t;
};
class MyClass:public C<int>
{
    public:
    virtual void Set(int t)
    {
        m_t = t;
    }
};
int main()
{
    MyClass c;
    c.Set(10);
    std::cout<<c.Get()<<std::endl;
}
```

**Virtual method template**

```c++
#include <iostream>

class C
{
  public:
    C(){};
    template<typename T>
    virtual void Print(T t){}
    
};
class MyClass:public C
{
    public:
    virtual void Print(int t)
    {
        std::cout<<t<<std::endl;
    }
};
int main()
{
    MyClass c;
    c.Print(10);
    
}
```

上述代码是编译报错的。

```
test.cpp:8:5: 错误：模板不能是‘virtual’的
     virtual void Print(T t){}

```



### stackoverflow [Are pure virtual methods allowed within a template class?](https://stackoverflow.com/questions/8919566/are-pure-virtual-methods-allowed-within-a-template-class)

[A](https://stackoverflow.com/a/8919588)

A class template can indeed contain virtual or pure virtual functions. This was employed by Andrei Alexandresu in "Modern C++ Design" to implement the visitor pattern using templates and type lists. You can see the code [here in his Loki library](http://loki-lib.cvs.sourceforge.net/loki-lib/loki/include/loki/Visitor.h?view=markup) if you're interested.

With most standard C++ implementations, this is fine, because when the template is instantiated the virtual function ends up being one single function. Consequently, the number of slots needed in the vtable can be known within the translation unit, so a vtable can be generated.

As you mentioned, you cannot have a virtual template member function because the number of vtable slots wouldn't be known within the translation unit.

Hope this helps!

> NOTE: 这段话解释了为什么不允许virtual method template

### stackoverflow [Is it safe if a template contains virtual function?](https://stackoverflow.com/questions/7962570/is-it-safe-if-a-template-contains-virtual-function)

[A](https://stackoverflow.com/a/7962611)

It is completely safe. Once you instantiate the class template, it becomes normal class just like other classes.

```cpp
template<typename T> 
class base {
public:
    T data;
    virtual void fn(T t){}
};


class derived : base<int> {
public:
    virtual void fn(int t){} //override
};

base<int> *pBase = new derived();
pBase->fn(10); //calls derived::fn()
```

I would also like to point out that while it is allowed virtual function in a class template, it is not allowed virtual function template inside a class (as shown below):

```cpp
class A
{
   template<typename T>
   virtual void f(); //error: virtual function template is not allowed
};
```

## 如何模拟？

在 `Virtual-method-template` 章节进行了讨论。