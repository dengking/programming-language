# [STL container as a template parameter](https://stackoverflow.com/questions/18613770/stl-container-as-a-template-parameter)





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

