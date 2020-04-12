# [spdlog](https://github.com/gabime/spdlog)

## 实现点评

以完全面向对象的方式来进行实现。



## data structure

在一个application中，可以创建多个logger，每个logger都需要有一个unique name，这些logger都要向global registry 进行注册，可以通过[`spdlog::get("...")`](https://github.com/gabime/spdlog/wiki/2.-Creating-loggers#accessing-loggers-using-spdlogget) 来访问指定的logger。主要的data structure如下：

```
`global registry` aggregate `logger` aggregate `sink` has a `formatter`
```

### registry

参见[5. Logger registry](https://github.com/gabime/spdlog/wiki/5.-Logger-registry)。

实现文件`registry.h`

singleton

### logger

实现文件`logger.h`



### sink

参见[4. Sinks](https://github.com/gabime/spdlog/wiki/4.-Sinks)。

实现文件`sink.h`

### config



## `_mt` VS `_st`

参见[1.1. Thread Safety](https://github.com/gabime/spdlog/wiki/1.1.-Thread-Safety)、[2. Creating loggers](https://github.com/gabime/spdlog/wiki/2.-Creating-loggers)



## 线程模型

### thread pool

实现类`class SPDLOG_API thread_pool`，实现文件`thread_pool.h`。

`class SPDLOG_API registry`有成员变量`std::shared_ptr<thread_pool> tp_;`