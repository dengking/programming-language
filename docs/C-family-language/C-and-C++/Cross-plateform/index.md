Thread-local storage (TLS)  

这篇文章介绍了系统实现和各种编程语言的实现

跨平台的写法：

```
// macro hanlding for threadlocal variables
#ifdef __GNUC__
  #define MX_TREAD_LOCAL __thread
#elif __STDC_VERSION__ >= 201112L
  #define  MX_TREAD_LOCAL _Thread_local
#elif defined(_MSC_VER)
  #define MX_TREAD_LOCAL __declspec(thread)
#endif
```

一个[demo](https://stackoverflow.com/questions/11983875/what-does-the-thread-local-mean-in-c11)

