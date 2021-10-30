# 我的实践

1、将通信日志和其他日志分别放到不同的日志文件中

通信日志: `CommunicateLogger`

其他日志: `SystemLogger`

2、使用daily log



```C++
m_Logger = spdlog::daily_logger_mt("daily_communicate_logger", FileName, m_Hour, m_Minute);
```



3、提供macro

```C++
#define LOG_ERROR(...)                                                          \
	if (SystemLogger::Instance().IsEnable())                                    \
	{                                                                           \
		SPDLOG_LOGGER_ERROR(SystemLogger::Instance().GetLogger(), __VA_ARGS__); \
	}
```

