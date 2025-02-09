# [krzysztofslusarski](https://github.com/krzysztofslusarski)/**[jvm-gc-logs-analyzer](https://github.com/krzysztofslusarski/jvm-gc-logs-analyzer)**

> NOTE: 通过日志来进行分析，提供了非常好的视图，思考: 它对于分析性能有什么意义?

This project is a **Java Virtual Machine** and **Garbage Collector** log analyzer.

### Features

#### Garbage collector logs analyzer

```shell
gc*,
gc+stringdedup=debug,
gc+ergo=trace,
gc+age=trace,
gc+phases=trace,
gc+humongous=trace
```



#### Safepoint logs anaylzer

```shell
-Xlog:safepoint
```



#### Classloader logs analyzer

```shell
-Xlog:class+unload,class+load
```



#### Thread logs analyzer

```shell
-Xlog:os+thread
```



#### Compilation logs analyzer

```shell
-Xlog:jit+compilation=debug
```



#### Code cache and sweeper logs analyzer

```shell
-Xlog:codecache+sweep*=trace
```



