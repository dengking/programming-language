# joining_thread

在阅读 [microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)** 时，发现的: 

| Feature        | Supported? | Description                                      |
| -------------- | ---------- | ------------------------------------------------ |
| joining_thread | ☐          | a RAII style version of `std::thread` that joins |

这让我想起了，在destructor中，如果不显示的join的话，则会抛出exception:

```C++
CHQImpl::~CHQImpl()
{

	if (m_StartHqClientThread.joinable())
	{
		m_StartHqClientThread.join();
 	}

}
```





## github [Implement joining_thread #808](https://github.com/microsoft/GSL/pull/808)





## csdn [C++核心准则：CP.25: gsl::joining_thread好于std::thread](https://blog.csdn.net/craftsman1970/article/details/108295078)