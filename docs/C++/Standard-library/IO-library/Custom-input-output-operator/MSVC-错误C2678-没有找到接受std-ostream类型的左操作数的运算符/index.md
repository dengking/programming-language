# MSVC 错误	C2678	二进制“<<”: 没有找到接受“std::ostream”类型的左操作数的运算符(或没有可接受的转换)

下面code，在MSVC中进行编译的时候，报了本节标题中罗列的错误。

```C++
	std::ostream& Print(std::ostream &Stream)
	{
		Stream << PrintMsgHead(Stream) << "\n" << PrintMsgBody(Stream);
		return Stream;
	}

	std::ostream& PrintMsgHead(std::ostream &Stream)
	{
		Stream << "消息头:" << "FuncNo=" << FunctionID << ";" << "SenderID=" << SenderID << ";";
		return Stream;
	}

	std::ostream& PrintMsgBody(std::ostream &Stream)
	{

		 if (lpReqField)
		 {
		     Stream << "请求消息体:" << lpReqField;
		 }
		return Stream;
	}
```

Google了一下，没有找到有价值的文章；既然报错说"没有找到接受“`std::ostream`”类型的左操作数的运算符"，那么我的第一想法就是不chain，而是使用如下写法:

```C++
	std::ostream& Print(std::ostream &Stream)
	{
		/**
		 * Stream << PrintMsgHead(Stream) << "\n" << PrintMsgBody(Stream);
		 * 上面这种写法在MSVC中，会报编译如下编译错误:
		 * 错误 C2678 二进制“<<”: 没有找到接受“std::ostream”类型的左操作数的运算符
		 * 下面的这种写法，能够保证编译通过，并且功能相同。
		 */
		PrintMsgHead(Stream);
		Stream << "\n";
		PrintMsgBody(Stream);
		return Stream;
	}
```



