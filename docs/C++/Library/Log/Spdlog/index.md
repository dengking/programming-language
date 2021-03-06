# github [**spdlog**](https://github.com/gabime/spdlog)

## Design pattern

### OOP

以完全面向对象的方式来进行实现。

[QuickStart](https://github.com/gabime/spdlog/wiki/1.-QuickStart):

> spdlog is a header only library. 
>
> spdlog takes the "include what you need" approach - your code should include the features that actually needed.

### Smart pointer

完全使用smart pointer来进行自动内存管理。

### Policy-based design

`spdlog::basic_logger_mt<spdlog::async_factory>`是policy-based design。

## Data structure

在一个application中，可以创建多个logger，每个logger都需要有一个unique name，这些logger都要向global registry 进行注册，可以通过[`spdlog::get("...")`](https://github.com/gabime/spdlog/wiki/2.-Creating-loggers#accessing-loggers-using-spdlogget) 来访问指定的logger。主要的data structure如下：

```
`global registry` aggregate `logger` aggregate `sink` has a `formatter`、`log level`
```

下面对各个structure进行说明: 

## 1 Registry

参见[5. Logger registry](https://github.com/gabime/spdlog/wiki/5.-Logger-registry)。

实现文件`registry.h`

singleton



## 2 Logger

|                    | synchronous logger                                  | asynchronous logger                  |
| ------------------ | --------------------------------------------------- | ------------------------------------ |
| **logger class**   | `logger.h:class logger`                             | `async_logger.h: class async_logger` |
| **logger factory** | `details:synchronous_factory.h:synchronous_factory` | `async.h:async_factory_impl`         |
|                    |                                                     |                                      |
|                    |                                                     |                                      |





## 3 Sink

参见[4. Sinks](https://github.com/gabime/spdlog/wiki/4.-Sinks)。

实现文件`sink.h`



## Config



## `_mt` VS `_st`

参见[1.1. Thread Safety](https://github.com/gabime/spdlog/wiki/1.1.-Thread-Safety)、[2. Creating loggers](https://github.com/gabime/spdlog/wiki/2.-Creating-loggers)



## Install

通过阅读[INSTALL](https://github.com/gabime/spdlog/blob/v1.x/INSTALL)可知，它有两种使用模式：

1、Header only version

2、Compiled library version

编译的时候，需要`-DSPDLOG_COMPILED_LIB`

## [Asynchronous logging](https://github.com/gabime/spdlog/wiki/6.-Asynchronous-logging)

参见 `./Asynchronous-logging` 章节。

