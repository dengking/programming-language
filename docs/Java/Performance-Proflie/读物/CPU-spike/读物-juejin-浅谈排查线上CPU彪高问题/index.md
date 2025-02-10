# [juejin 浅谈排查线上CPU彪高问题](https://juejin.cn/post/7147561671459471368)

后端程序员出去面试经常会有面试官喜欢问你有没有排查过线上问题，遇到后怎么排查的，什么cpu飙高了，任务队列消息挤压了等等。

但是工作了两年多发现哪有这么多线上问题让你查，再加上又不是所有程序员所在公司都有saas服务，一般来说toc的企业的服务很少出现这种情况。由于我们的主线业务就是toc，所以每次在集成环境里出现这种问题得立马逮到把握住，毕竟面试常考题。

大家不要担心，看完这篇文章就算没排查过线上问题你也能说出个一二三四来。我总结排查CPU飙高主要就是三板斧。

事情是这样，我把上篇提到的redis stream当作服务于服务之间的消息通知组件搭了个demo，然后趁着有新功能版本发布到集成测试环境试了一波。果不其然上午发布下午就@我了。

## 第一板斧，查TOP

cpu高可以理解成计算机没有资源去处理用户新的事件了，通常表现为页面卡顿，请求一直处理中不返回结构。这时候登陆到对应服务器上直接用`TOP`命令看各个进程情况和负载情况。

top里的各个参数有兴趣可以深入了解下，包括`load average平均负载`，`zombie僵尸进程`，`wa IO等待占用CPU的百分比0.0%`。不过从这次这个例子中一眼就能看到其中一个Java服务的cpu使用率达到了快300%，显然是不正常的。

![图片](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/b6fb849fefb24072942846873f0aa54f~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

按c可以展示进程启动命令发现就是引入了消息流转的服务，厚礼蟹，cpu直接到快300%，不管这个现象跟卡顿有没有关系都要查一下为啥会出现这种情况。

在`top`里定位到异常进程复制PID，输入`top -Hp pid`查看该进程下所有的线程信息。

![图片](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/aed201b21dbb4be19a396d58211162db~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

一看排在第一位的哪个线程消耗的cpu也是最高，复制pid，用`printf "%x"线程pid` 拿到16进制的线程号。

![图片](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/1a85605141234459b47a835018f05f99~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

## 第二板斧，查堆栈

再用`jstack`命令搜一下线程信息，`jstack` 异常进程的`pid | grep "16进制线程号"`

![图片](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/5aa093a482d74c7496373afbbaf02e3c~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

其实这里就已经能看出来了，lettuce线程，springboot2.3之后默认的redis客户端从redission改为了lettuce，肯定是跟redis相关的。

但其实到这里我还是没反应过来，用`jstack -l pid > dump.txt`导出堆栈信息，用`https://fastthread.io/`(非常好用的jvm堆栈分析网站) 又是一顿分析。一直怀疑是因为上次迭代升级springboot版本导致的。

后来找东找西都没有头绪，又一顿分析日志发现都卡在epoll这里，了解些io的都是到这是多路复用接口搁那轮询处理io事件的，又联想起为了适配redis stream的java api升springboot版本，终于定位到了。

## 第三板斧，查代码

```java
@Bean("listenerContainer")
@DependsOn(value = "redisConnectionFactory")
public StreamMessageListenerContainer<String, ObjectRecord<String, Object>> init() {
    StreamMessageListenerContainer.StreamMessageListenerContainerOptions<String, ObjectRecord<String, Object>>
        options = StreamMessageListenerContainer.StreamMessageListenerContainerOptions.builder()
        .batchSize(10)
        .serializer(new StringRedisSerializer())
        .executor(new ForkJoinPool())
        .pollTimeout(Duration.ofSeconds(2)) 
        .targetType(Object.class)
        .build();
    return StreamMessageListenerContainer.create(redisConnectionFactory, options);
 }
```

设置StreamMessageListenerContainer的时候，有一项pollTimeout(Duration.ofSeconds(2))，`向redis不断poll数据的时间延迟，单位为s`，我当时想着要什么延迟，0就完事了，一直搁那监控数据就完了，有数据就拿回来。

小改成2s，一套打包连招放服务器上一测。

![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/fdd47b47bb714cc383377950c7ff03af~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?) 280%降到2.6%

# 总结

这样一来以后面试官问有没有解决过cpu飙升的问题就不虚了，先来一套丝滑连招

1. 去服务器top命令查看进程资源情况，`top -Hp pid`查看异常进程里的线程详情
2. 复制pid，用`printf "%x"线程pid` 拿到16进制的线程号，jstack pid | grep 16进制线程
3. 分析代码