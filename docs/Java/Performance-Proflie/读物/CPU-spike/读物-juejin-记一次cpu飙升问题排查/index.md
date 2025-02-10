# [juejin 记一次cpu飙升问题排查](https://juejin.cn/post/7139202066362138654)

首先问题是这样的，周五正在写文档，突然收到了线上报警，发现cpu占用达到了90多(CPU升高可能是由于发生了异常)，上平台监控系统查看容器，在jvm监控中发现有一个pod在两个小时内产生了61次youngGc一次fullGc，这个问题特别严重且少见，由于我之前也没有排查过此类问题，所以也是百度，但整个过程也有一些自己的思考，所以跟大家分享一下~

# 当时场景

我先给大家看一下一副正常的gc曲线监控（为保密性，我自己按照平台监控画了出来）：

1.正常的jvm监控曲线图

![](https://pic.imgdb.cn/item/6313798d16f2c2beb146dfd2.jpg)

2.产生问题的jvm监控曲线图

![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/92c1cef5cd2c454b842240bf844aa31f~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?)

可以看的出来，正常情况下该系统很少gc(具体看业务系统使用情况、jvm内存分配)，但是在图二中出现了大量异常的gc情况甚至触发了fullGc，所以我当时立马进行了分析。

> NOTE: 为什么陡然增多的是异常gc?

# 具体分析

首先异常gc的情况只出现在一个pod上（系统有多个pod）,在监控系统找到对应的pod，进入pod内部查看问题原因

1. 进入pod之后，输入top查看各linux进程对系统资源的使用情况(因我这是事后补稿，资源使用不高，大家看步骤即可)![](https://pic.imgdb.cn/item/63137c7316f2c2beb14aeddf.jpg)

2. 分析资源使用情况在当时的情况下，![image.png](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/0932e8165b9a48939bfdf93deb5220e4~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?)当时我的pid为1的进程cpu上到了130（多核）那我认定就是java应用出问题了，control+c退出继续往下走
   
   > NOTE: 为什么"cpu上到了130（多核）"就可以认定Java出现问题?

3. 输入`top -H -p pid` 通过此命令可以查看实际占用CPU最高的的线程的id，pid为刚才资源使用高的pid号![](https://pic.imgdb.cn/item/63137ee316f2c2beb14d67c8.jpg)

4. 出现具体线程的资源使用情况，表格里的pid代表线程的id，我们称他为tid![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/a3272692ef7f4486bdf22df5f605b5c0~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?)

5. 我记得当时的tip为746（上述图片只是我给大家重复步骤），使用命令`printf "%x\n" `746，将线程tid转换为16进制![image.png](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/e0dde491c55a466aa6464d80a1c38305~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?)，因为我们线程id号在**堆栈**里是**16进制**的所以需要做一个进制转换

6. 输入`jstack pid | grep 2ea >gc.stack`![image.png](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/2bde782d65fe484c949baec7a533bdb9~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?)解释一下，jstack是jdk给提供的监控调优小工具之一，jstack会生成JVM当前时刻的**线程快照**，然后我们可以通过它查看某个Java进程内的线程堆栈信息，之后我们把堆栈信息通过管道收集2ea线程的信息，然后将信息生成为`gc.stack`文件，我随便起的，随意

7. 当时我先`cat gc.stack` 发现数据有点多在容器里看不方便，于是我下载到本地浏览，因为公司对各个机器的访问做了限制，我只能用跳板机先找到一台没用的机器a，把文件下载到a然后我再把a里的文件下载到本地（本地访问跳板机OK），先输入`python -m SimpleHTTPServer 8080`，linux自带python，这个是开启一个简单http服务供外界访问，![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/e41bb4c3f3ac40c29b01e761f1b4ca25~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?)，然后登录跳板机，使用curl下载curl -o [http://ip地址/gcInfo.stack](https://link.juejin.cn?target=http%3A%2F%2Fip%25E5%259C%25B0%25E5%259D%2580%2FgcInfo.stack "http://ip%E5%9C%B0%E5%9D%80/gcInfo.stack") 为方便演示，我在图中把ip换了一个假的![image.png](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/edee67cbbae34304b0e9d54bdd07d0ce~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?)之后用同样的方法从本地下载跳板机就可以了，记得关闭python开启的建议服务嗷

8. 把文件下载到了本地，打开查看编辑器搜索2ea，找到nid为2ea的堆栈信息， ![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/78c84dc794f94ca0882979a2b74253da~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?)，之后找到对应的impl根据行数分析程序

9. 发现是在文件异步导出excel的时候，导出接口使用了公共列表查询接口，列表接口查询数据最多为分页200一批，而导出数据量每个人的权限几万到十几万不等![image.png](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/24e876fb3d9d47e890f29fa30a1ea086~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp?)并且该判断方法使用了嵌套循环里判断，且结合业务很容易get不到value，guawa下的newArrayList就是返回一个newArrayList（好像不用说这么细 (；一_一 ），在整个方法结束之前，产生的lists生命周期还在所以发生多次gc触发重启之后还影响到了别的pod。然后对代码进行了fix，紧急上线，问题解决~

# 评论

## 1

补一张gc图

![](https://p26-juejin-sign.byteimg.com/tos-cn-i-k3u1fbpfcp/e2103c5b4fb34943b003375d3fba9a2a~tplv-k3u1fbpfcp-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5qmY5a2Qenp6eg==:q75.awebp?rk3s=436ef1ef&x-expires=1739267076&x-signature=0T5A3zLYQm8arvzEjps6d7OrCfU%3D)

## 2

源代码

![](https://p3-juejin-sign.byteimg.com/tos-cn-i-k3u1fbpfcp/f87509f8a1a644fca1b6f293a5c95334~tplv-k3u1fbpfcp-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5qmY5a2Qenp6eg==:q75.awebp?rk3s=436ef1ef&x-expires=1739268104&x-signature=e68QVjSsGwY5Y%2BKAbApeZHQnDi8%3D)

内存使用图

![](https://p3-juejin-sign.byteimg.com/tos-cn-i-k3u1fbpfcp/7599374f0d774aafb7317b1f638c1c98~tplv-k3u1fbpfcp-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5qmY5a2Qenp6eg==:q75.awebp?rk3s=436ef1ef&x-expires=1739268104&x-signature=Uo9sifN1BeIkE%2F%2BkwOdPGOfEy1Y%3D)