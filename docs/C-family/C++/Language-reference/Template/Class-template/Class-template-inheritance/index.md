# Class template inheritance

本章讨论class template inheritance，主要讨论两种情况:

a、Inherit from class template

b、Class template inherit from non-template class

c、Variadic multiple inheritance

## Inherit from class template

本节标题的是含义是: 一个class继承一个class template，显然:

1、要么这个class也是一个class template

2、要么这个class需要特化它的parent class template

https://stackoverflow.com/questions/8810224/inheriting-from-a-template-class-in-c

https://www.cprogramming.com/tutorial/templates.html

https://www.tutorialspoint.com/cplusplus/cpp_templates.htm



### stackoverflow [Derived template-class access to base-class member-data](https://stackoverflow.com/questions/1120833/derived-template-class-access-to-base-class-member-data)

### 我的实践

```c++
#include "stddef.h"
#include <cstdint>
using FuncNoType = unsigned int;

struct MSG_HEAD
{
	uint32_t uMsgLen;
	uint32_t uFuncNo;
	int64_t nToken;
	int32_t nSenderID;
	uint32_t uReqNum;
};

/**
 * normal请求msg
 */
template<typename FieldType>
struct CNormalMsgReq
{
	MSG_HEAD MsgHead;
	FieldType Field;
};

template<typename ReqFieldType>
struct CMsgPacker
{
public:
	CMsgPacker(ReqFieldType *ReqField)
			: m_ReqField(ReqField)
	{

	}
	void InitMsgHead()
	{
		m_MsgHead = (MSG_HEAD*) m_ReqMsgPtr;
	}
	CMsgPacker& SetFuncNo(uint32_t FuncNo)
	{
		if (m_MsgHead)
		{
			m_MsgHead->uFuncNo = FuncNo;
		}
		return *this;
	}
	CMsgPacker& SetToken(int64_t Token)
	{
		if (m_MsgHead)
		{
			m_MsgHead->nToken = Token;
		}
		return *this;
	}

	CMsgPacker& SetSenderID(int32_t SenderID)
	{
		if (m_MsgHead)
		{
			m_MsgHead->nSenderID = SenderID;
		}
		return *this;
	}

	CMsgPacker& SetReqNum(uint32_t ReqNum)
	{
		if (m_MsgHead)
		{
			m_MsgHead->uReqNum = ReqNum;
		}
		return *this;
	}
	const void* GetMsgPtr()
	{
		return m_ReqMsgPtr;
	}
	size_t GetMsgLen()
	{
		return m_MsgLen;
	}
protected:

	void CopyReqFieldToMsgBody(size_t Offset)
	{
		void *lpBody = m_ReqMsgPtr + Offset;
		memcpy(lpBody, m_ReqField, sizeof(ReqFieldType));
	}
public:
	/**
	 * 指向消息头的指针
	 */
	MSG_HEAD* m_MsgHead { nullptr };
	/**
	 * 指向请求消息的指针
	 */
	char* m_ReqMsgPtr { nullptr };
	/**
	 * 消息长度
	 */
	size_t m_MsgLen { 0 };
	ReqFieldType *m_ReqField { nullptr };
	FuncNoType m_FuncNo { 0 };
};

template<typename ReqFieldType>
struct CNoIndexMsgPacker: public CMsgPacker<ReqFieldType>
{
public:
	using ReqMsgType = CNormalMsgReq<ReqFieldType>;
	CNoIndexMsgPacker(ReqFieldType *ReqField)
			: CMsgPacker<ReqFieldType>(ReqField)
	{
		this->m_MsgLen = sizeof(ReqMsgType);
		memset(&m_ReqMsg, 0x00, this->m_MsgLen);
		m_ReqMsgPtr = &m_ReqMsg;
		InitMsgHead();

		m_ReqMsg.MsgHead.uMsgLen = m_MsgLen;
		m_ReqMsg.MsgHead.uMsgType = MSG_NO_INDEX;

		CopyReqFieldToMsgBody(sizeof(MSG_HEAD));

	}
private:
	ReqMsgType m_ReqMsg;
};

struct Test
{
	int i;
};
int main()
{
	Test t;
	CNoIndexMsgPacker<Test>(&t);
}
```



### Why am I getting errors when my template-derived-class uses a member it inherits from its template-base-class? [¶](https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members) [Δ](https://isocpp.org/wiki/faq/templates#)



## Class template inherit from non-template class

本节标题的含义是: 类模板继承一个非模板类

类模板继承一个非模板类，如下类模板`Child`继承非模板类`Base`，在类模板`C`中，有一个`C2`类型的成员变量`m_c2`，这个类型需要一个类型为`Base`的入参，那这就出现了一个问题：如何将模板类`C`的特化后的类对象作为入参传入呢？下面是一个简单的测试出现，结果是可以的，其实显而易见，特化后的模板类是一个继承自`Base`的普通类。

### 我的实践

```c++
class Base
{
};

class C2
{
public:
C2(Base* b):m_b(b)
{
}
Base* m_b;

};

template<class T>
class Child:public Base
{
using TT = T;
public:
Child()
{
m_c2 = new C2(this);
}
C2 *m_c2;
};

int main()
{
C<int> c;
}
```



```c++
#include<iostream>
#include<vector>
struct Base{};

template<typename T>
class Child:public Base
{

};



int main(){
std::vector<Base*> v ;
v.push_back( new Base());

v.push_back( new Child<int>());

}
```

### OOP interface + template implementation

这是一种非常重要的用法，在如下领域应用了这种方法: 

1、type  erasure，参见 `Type-Erasure` 章节

2、external polymorphism，参见:

a、`External-polymorphism-pattern` 章节

## Specialize-template-method-of-base-class

https://stackoverflow.com/questions/39808971/specialize-the-bases-templated-member-function-in-derived-class