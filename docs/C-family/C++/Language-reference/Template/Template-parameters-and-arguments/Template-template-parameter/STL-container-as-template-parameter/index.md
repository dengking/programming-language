# STL container as a template parameter

stl的container都是模板类，如何将它们作为template parameter呢？这是本节探讨的一个话题。

## stackoverflow [STL container as a template parameter](https://stackoverflow.com/questions/18613770/stl-container-as-a-template-parameter)

I'm trying to pass an STL container as a template parameter. In this case, the vector.

Here is my not-functional code:

```c++
template<template<class> class TContainer, class TObject>
class Foobar
{
public:

    explicit Foobar( TContainer<TObject*> & container )
    :
    container_( container ){}


private:

    TContainer<TObject*> & container_;
};


int _tmain(int argc, _TCHAR* argv[])
{
    std::vector<IUnknown*> v;

    Foobar<std::vector, IUnknown*> bla( v );

    return 0;
}
```

Is this, what I'm trying to do possible at all, because the compiler cannot swallow this?

### [A](https://stackoverflow.com/a/18614068)

There are several things wrong with your code, here is a working example:

```c++
template<template<class, class> class TContainer, class TObject>
class Foobar
{
public:
    explicit Foobar( TContainer<TObject*, std::allocator<TObject*>> & container )
    :
    container_( container ){}

private:
    TContainer<TObject*, std::allocator<TObject*>> & container_;
};

int main()
{
    std::vector<IUnknown*> v;
    Foobar<std::vector, IUnknown> bla( v );
}
```

The main fault of your codes it that `std::vector` takes two template arguments. It looks like this `template<class T, class Allocator = std::allocator<T>> class vector;`. Also, [Joachim Pileborg](https://stackoverflow.com/a/18613865/906773) is right about the double pointer issue, `IUnknown**`. However, you could simplify your code with the following:

```C++
template<class TContainer>
class Foobar
{
public:
    explicit Foobar( TContainer & container )
    :
    container_( container ){}

private:
    TContainer & container_; // Be careful with reference members
};

int main()
{
    std::vector<IUnknown*> v;
    Foobar<std::vector<IUnknown*>> bla( v ); // C++11 decltype(v) could be used
}
```

## libstdc++ [`std::stack`](https://code.woboq.org/gcc/libstdc++-v3/include/bits/stl_stack.h.html) 

libstdc++ [`std::stack`](https://code.woboq.org/gcc/libstdc++-v3/include/bits/stl_stack.h.html) 的实现是非常值得借鉴的:

```c++
template<typename _Tp, typename _Sequence = deque<_Tp> >
class stack
{


};
```

需要注意的是， 它的第二个模板参数`_Sequence`不是template template parameter，而是一个普通的type template parameter。

下面是`std::stack`的一些例子：

```c++
std::stack<int> stack1;
stack1.push(5);
    
std::stack<double, std::vector<double>> stack2;
stack2.push(10.5);
```



## Type template parameter `_Sequence` VS template template parameter `Cont`

modernescpp [Types-, Non-Types, and Templates as Template Parameters](https://www.modernescpp.com/index.php/types-non-types-and-templates-as-template-parameters) 中 `class Matrix`的第二个template parameter `Cont`是一个template template parameter，所以它的argument可以是`std::vector`（需要注意的是`std::vector`是一个template class，而不是一个class）；所以它的instantiation如下：

```c++
Matrix<int, std::vector> myIntVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
```

libstdc++ [`std::stack`](https://code.woboq.org/gcc/libstdc++-v3/include/bits/stl_stack.h.html) 的`_Sequence`是type template parameter，所以它的argument不能够是template，而只能够是type；所以它的instantiation如下：

```c++
std::stack<double, std::vector<double>> stack2;
```



## Application

### Generic STL container printer

1、stackoverflow [What are some uses of template template parameters?](https://stackoverflow.com/questions/213761/what-are-some-uses-of-template-template-parameters) # [A](https://stackoverflow.com/a/14311714)

## TODO



http://www.cs.technion.ac.il/users/yechiel/c++-faq/template-of-template.html







## 我的实践

将一个模板类作为一个目标类的成员变量：

```c++
template<class T>
class T1
{};

template<class T>
class T2
{
T1<T> t;
};

int main()
{
T1<int> t1;
T2<int> t2;

}
```

将一个模板类作为参数:

```c++
template<class T>
class T1
{};

template<template<class> class argument>
class T2
{};

int main()
{
T1<int> t1;
T2<T1> t2;

}
```



```c++
template<typename FieldT>
struct CUstOptOrderInfoBase
{

	HSIssuetype Issuetype;

	HSNum SequenceNumber;

	FieldT RspQryOrder;
};

template<template<typename > typename InfoT, typename FieldT>
struct CUstOptOrderRspBase
{
	MSG_HEAD MsgHead;
	SBusinessNotifyHead NotifyHead;
	STradeNotifyKey TradeNotifyKey;
	InfoT<FieldT> RtnMsgRsp;
};

// 行权
using CUstOptExerciseInfo = CUstOptExerciseInfoBase<CHSInsRspQryExerciseField>;
using CUstOptExerciseRsp = CUstOptExerciseRspBase<CUstOptExerciseInfoBase, CHSInsRspQryExerciseField>;
```



