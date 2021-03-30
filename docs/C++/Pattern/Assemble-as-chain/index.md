# C++ assemble as chain

## Function的入参类型和出参类型相同

1、linked list中，每个node都有一个next pointer，这个next pointer的类型、指向相同的node type

2、如果一个函数，它的入参和出参类型相同，那么就能够非常方便地将这些函数串联起来

3、典型的例子就是C++ stream，下面就以C++ stream来进行说明

### 例子: stream的pipeline、function composition模式

1、将上一个的输出作为下一个的输入

2、下面这个例子中，`PrintBizHead`、`PrintMsgBody`的入参、出参类型都是`std::ostream &`，这就让programmer能够非常便利地将它们给串联起来:

```C++
std::ostream& Print(std::ostream &Stream) override
{
    return PrintMsgBody(PrintBizHead(Stream));
}
```

2、可以看到，这个例子中，它是通过如上述的嵌套函数调用来实现的，即将上一个函数的返回值，作为下一个函数的入参。

#### 完整程序

```C++
struct CLdpQueryRspFieldUnpackerBase
{
	/**
	 * @brief 打印业务头
	 *
	 * @param Stream
	 * @return
	 */
	std::ostream& PrintBizHead(std::ostream &Stream)
	{
		Stream << "IsFinished=" << m_IsFinished << "PositionSn=" << m_PositionSn << ";";
		return Stream;
	}
	/**
	 * @brief 打印业务体
	 *
	 * @param Stream
	 * @return
	 */
	std::ostream& PrintMsgBody(std::ostream &Stream)
	{
		if (this->m_LdpMsg.IsSuccess())
		{
			int i = 0;
			for (auto &Rsp : m_RspFileds)
			{
				Stream << "第" << i << "条记录:" << Rsp; // 访问第i条记录
				++i;
			}
		}
		return Stream;
	}
	std::ostream& Print(std::ostream &Stream) override
	{
		return PrintMsgBody(PrintBizHead(Stream));
	}
};

```



### 例子: steam的append模式

1、`std::ostream` + overload operator `<<`

2、operator `<<` 就像linked list的next pointer，它的入参类型和出参类型相同(`std::ostream &Stream`)，那么在这种情况下，就能够使用它来形成linked list。

3、这是非常典型的stream model，通过`std::ostream`将一串函数给顺次连接起来了，每个函数中，都将内容输出到`std::ostream`中。

#### My library # Protocol-parsing-library

```C++
	std::ostream& Print(std::ostream &Stream) override
	{
		Stream << this->m_Msg << m_RspInfoFieldUnpacker << m_QueryRspFieldUnpacker;
		return Stream;
	}
```



#### 推广

典型的例子包括: 

1、stackoverflow [A better LOG() macro using template metaprogramming](https://stackoverflow.com/questions/19415845/a-better-log-macro-using-template-metaprogramming)

其中的"SOLUTION #3: EXPRESSION TEMPLATES"就是这种典型。