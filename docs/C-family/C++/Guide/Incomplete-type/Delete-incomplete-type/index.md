# Delete object of incomplete type

比较意外的是 ，"Delete object of incomplete type" 是能够编译通过的，但是这样会招致undefined behavior，因此programmer需要添加check，这在下一章节描述如何实现check。

## stackoverflow [Delete objects of incomplete type](https://stackoverflow.com/questions/4325154/delete-objects-of-incomplete-type)

This one made me think:

```cpp
class X;

void foo(X* p)
{
    delete p;
}
```

> NOTE: 完整测试程序如下:
>
> ```C++
> class X;
> 
> void foo(X *p)
> {
> 	delete p;
> }
> int main()
> {
> 
> }
> g++ main.cpp --std=c++11
> ```
>
> 编译报错如下:
>
> ```C++
> main.cpp: 在函数‘void foo(X*)’中:
> main.cpp:5:12: 警告：检测到调用 delete 运算符时可能出现的问题： [默认启用]
>      delete p;
>             ^
> main.cpp:3:6: 警告：‘p’类型不完全 [默认启用]
>  void foo(X* p)
>       ^
> main.cpp:1:7: 警告：‘class X’的前向声明 [默认启用]
>  class X;
>        ^
> main.cpp:5:12: 附注：neither the destructor nor the class-specific operator delete will be called, even if they are declared when the class is defined
>      delete p;
> ```
>
> 

How can we possibly `delete p` if we do not even know whether `X` has visible destructor? g++ 4.5.1 gives three warnings:

> ```cpp
> warning: possible problem detected in invocation of delete operator:
> warning: 'p' has incomplete type
> warning: forward declaration of 'struct X'
> ```

And then it says:

> note: **neither the destructor nor the class-specific operator delete will be called, even if they are declared when the class is defined.**

Wow... are compilers required to diagnose this situation like g++ does? Or is it undefined behavior?



> NOTE: 显然，compiler不会选择destructor、class-specific operator delete，因此这就导致了undefined behavior

[A](https://stackoverflow.com/a/4325223)

From the standard [expr.delete]:

> If the object being deleted has incomplete class type at the point of deletion and the complete class has a **non-trivial destructor** or a **deallocation function**, the behavior is undefined.

So, it's UB if there's nontrivial stuff to do, and it's ok if there isn't. Warnings aren't neccessary for UB.



## stackoverflow [Why, really, deleting an incomplete type is undefined behaviour?](https://stackoverflow.com/questions/2517245/why-really-deleting-an-incomplete-type-is-undefined-behaviour)

Consider this classic example used to explain what *not* to do with forward declarations:

```cpp
//in Handle.h file
class Body;

class Handle
{
   public:
      Handle();
      ~Handle() {delete impl_;}
   //....
   private:
      Body *impl_;
};

//---------------------------------------
//in Handle.cpp file

#include "Handle.h"

class Body 
{
  //Non-trivial destructor here
    public:
       ~Body () {//Do a lot of things...}
};

Handle::Handle () : impl_(new Body) {}

//---------------------------------------
//in Handle_user.cpp client code:

#include "Handle.h"

//... in some function... 
{
    Handle handleObj;

    //Do smtg with handleObj...

    //handleObj now reaches end-of-life, and BUM: Undefined behaviour
} 
```

I understand from the standard that this case is headed towards UB since Body's destructor is non trivial. What I'm trying to understand is really the root cause of this.

I mean, the problem seems to be "triggered" by the fact that Handle's dtor is inline, and so the compiler does something like the following "inline expansion" (almost pseudo-code here).

```cpp
inline Handle::~Handle()
{
     impl_->~Body();
     operator delete (impl_);
}
```

In all translation units (only `Handle_user.cpp` in this case) where a Handle instance gets to be destroyed, right? I just can't understand this: ok, when generating the above inline expansion the compiler doesn't have a full definition of the Body class, but why cannot it simply have the linker resolve for the `impl_->~Body()` thing and so have it call the Body's destructor function that's actually defined in its implementation file?

In other words: I understand that at the point of Handle destruction the compiler doesn't even know if a (non-trivial) destructor exists or not for Body, but why can't it do as it always does, that is leave a "placeholder" for the linker to fill in, and eventually have a linker "unresolved external" if that function is really not available?

Am I missing something big here (and in that case sorry for the stupid question)? If that's not the case, I'm just curious to understand the rationale behind this.