# Template template parameter

 Template template parameter的含义是使用一个template来作为template parameter。

## stackoverflow [STL container as a template parameter](https://stackoverflow.com/questions/18613770/stl-container-as-a-template-parameter)





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

## stackoverflow [What are some uses of template template parameters?](https://stackoverflow.com/questions/213761/what-are-some-uses-of-template-template-parameters)



## stackoverflow [Templated class specialization where template argument is a template](https://stackoverflow.com/questions/4189945/templated-class-specialization-where-template-argument-is-a-template)

Google template template specialization	







### Template template parameter

#### modernescpp [Types-, Non-Types, and Templates as Template Parameters](https://www.modernescpp.com/index.php/types-non-types-and-templates-as-template-parameters)

> 这篇文章不错，其实的例子非常好。

```c++
// templateTemplateParameters.cpp

#include <iostream>
#include <list>
#include <vector>
#include <string>

template <typename T, template <typename, typename> class Cont >   // (1)
class Matrix{
public:
  explicit Matrix(std::initializer_list<T> inList): data(inList){  // (2)
    for (auto d: data) std::cout << d << " ";
  }
  int getSize() const{
    return data.size();
  }

private:
  Cont<T, std::allocator<T>> data;                                 // (3)                               

};

int main(){

  std::cout << std::endl;

                                                                    // (4)
  Matrix<int, std::vector> myIntVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
  std::cout << std::endl;
  std::cout << "myIntVec.getSize(): " << myIntVec.getSize() << std::endl;

  std::cout << std::endl;

  Matrix<double, std::vector> myDoubleVec{1.1, 2.2, 3.3, 4.4, 5.5}; // (5)
  std::cout << std::endl;
  std::cout << "myDoubleVec.getSize(): "  << myDoubleVec.getSize() << std::endl;

  std::cout << std::endl;
                                                                    // (6)
  Matrix<std::string, std::list> myStringList{"one", "two", "three", "four"};  
  std::cout << std::endl;
  std::cout << "myStringList.getSize(): " << myStringList.getSize() << std::endl;

  std::cout << std::endl;

}
```

#### With default template argument

```c++
template<typename PushFieldType, template<typename = PushFieldType> typename  PushListType>
void UpdatePushList(PushFieldType* pField,  PushListType<PushFieldType> & PushList)
{

}
```





### `std::stack`

[`std::stack`](https://code.woboq.org/gcc/libstdc++-v3/include/bits/stl_stack.h.html) 的实现是非常值得借鉴的

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



### Type template parameter `_Sequence` VS template template parameter `Cont`

Template template parameter中的`class Matrix`的第二个template parameter `Cont`是一个template template parameter，所以它的argument可以是`std::vector`（需要注意的是`std::vector`是一个template class，而不是一个class）；所以它的instantiation如下：

```c++
Matrix<int, std::vector> myIntVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
```

`std::stack`的`Cont`是type template parameter，所以它的argument不能够是template，而只能够是type；所以它的instantiation如下：

```c++
std::stack<double, std::vector<double>> stack2;
```

### Container as template parameter



stl的container都是模板类，如何将它们作为template parameter呢？这是本节探讨的一个话题。

[STL container as a template parameter](https://stackoverflow.com/questions/18613770/stl-container-as-a-template-parameter)

http://www.cs.technion.ac.il/users/yechiel/c++-faq/template-of-template.html

