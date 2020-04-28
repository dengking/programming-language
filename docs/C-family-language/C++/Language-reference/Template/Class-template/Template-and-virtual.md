# Template and virtual

一种是compile-time的多态，一种是runtime的多态。使用template的方式和使用inheritance-virtual的方式的本质差异在于：前者不同的specialization之间不是同一种class，而后者，不同的child class之间是同一种class。

如何两者兼得呢？

## Can template class have virtual method?



[Are pure virtual methods allowed within a template class?](https://stackoverflow.com/questions/8919566/are-pure-virtual-methods-allowed-within-a-template-class)

[Is it safe if a template contains virtual function?](https://stackoverflow.com/questions/7962570/is-it-safe-if-a-template-contains-virtual-function)

[Template and Virtual functions in C++ ? allowed ?](https://stackoverflow.com/questions/7212818/template-and-virtual-functions-in-c-allowed)

[C++ Templates with Virtual Functions?](https://rivermatrix.com/soft-eng/cpp/2014/10/27/c-templates-with-virtual-functions/)



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





## Virtual template method

有的时候，希望参数化基类的成员方法的类型，然后子类来进行override，但是非常明显的是，c++是不允许Virtual template method，那如何实现呢？

其实有两种方案：

- 使用template class，将这些模板参数放到class级别
- 使用`void*`

使用第一种方式的一个需要注意的点是：不是同一种类型，无法进行多态。

https://stackoverflow.com/questions/7968023/c-virtual-template-method

https://stackoverflow.com/questions/2354210/can-a-class-member-function-template-be-virtual?rq=1

https://www.experts-exchange.com/questions/20558666/virtual-template-function-workaround.html



### [On the Tension Between Object-Oriented and Generic Programming in C++](https://www.artima.com/cppsource/type_erasure.html)

> `void*`就是一种典型的type erasure
>
> 

上述使用`void*`让我想起来：C++ Programming: `Void*` Vs. Templates：

http://ixodoi.expertscolumn.com/article/c-programming-void-vs-templates

`void*`是c的generic programming。

duck type是python的generic programming。

template是`c++`的generic programming。



这让我想起来visitor pattern。