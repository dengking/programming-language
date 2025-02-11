# 自定义类加载器如何拉长YGC

## [juejin 记一次艰难的GC问题排查](https://juejin.cn/post/6961597031060078605?from=search-suggest)

### 排查过程

#### 确定GC出问题

在某一天的上午突然出现了报警，发现是ZK断开了链接， ![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/fb7320a40c294cdd87e81cdbaf4fd9c3~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

从图上看我们这个错误是间断性的出现，最开始以为是zk出现了问题，后来经过排查其他服务的zk并没有出现任何问题。所以就怀疑是内部的代码出现问题导致的，研究之后发现是zk出现了**心跳超时**情况才导致的断开链接，所以就怀疑了两种情况：

- 网络有抖动
- 机器间歇性卡死

如果网络有抖动的话的确是会出现偶发性超时，但是很明显，其他所有的服务都没问题，应该不是抖动导致。所以机器应该是间歇性的一个卡死，一般出现这个情况首当其冲的就是我们CPU被打满了，导致机器卡死，发现CPU并无问题，然后就是我们的gc带来的STW,会导致我们的jvm进程卡顿。

观察之后的确是young gc很慢，导致我们的JVM发生了GC卡顿，所以出现了这个现象。

#### 排查原因

**GC**出现问题一般来说两大法宝可以解决大部分问题：

- GC日志
- dump文件

出现问题之后我立马打开了**GC日志**，截图如下：

![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/6d9bd789bc15466ab11716bc19f39948~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp) 可以发现我们的**young gc**已经达到2.7s了，大家知道我们的**young gc**是全程STW的，那就意味着每次gc就会卡顿2.7s，那么zk超时断开链接也就符合正常了。再看了下这个gc收集情况，每次也能完全收集。在日志中很明显在**root scanning**的时间比较长，当时对这个阶段不太熟悉(后面会继续讲)，所以一直也不明白为什么这样，在网上各种搜索，也没有结论。

这个时候我在why哥公众号读到了一篇文章：[mp.weixin.qq.com/s/KDUccdLAL…](https://link.juejin.cn?target=https%3A%2F%2Fmp.weixin.qq.com%2Fs%2FKDUccdLALWdjNBrFjVR74Q "https://mp.weixin.qq.com/s/KDUccdLALWdjNBrFjVR74Q"), 建议大家可以阅读一下这篇文章，这个文章中主要谈到了我们jvm的一个优化，大家都知道我们进入STW的时候是需要一个安全点才可以的，而询问是否进入到安全点是需要耗费资源的，所以jvm在做jit优化的时候会讲counted loop 也就是计数循环优化成整个循环结束之后再进入安全点，在小米的技术文章中也提到了相关的问题:[《HBase实战：记一次Safepoint导致长时间STW的踩坑之旅》](https://juejin.cn/post/6844903878765314061 "https://juejin.cn/post/6844903878765314061") 。

看完这两个文章之后，我突然想到了我们的代码也是counted loop的形式，所以就怀疑有可能也是这个问题导致的，马上进行代码优化，将for(int i = 0; i< n; i++) 中的int 换成了long，就可以避免这种jit的优化，马上兴冲冲的将其上线，结果过了一天之后依然存在这个问题，此时人都快崩溃，搞了半天原来不是这个问题导致的。

#### 定位问题

对于G1之前只是看了些原理相关的，但是此时原理相关的东西好像在这里基本没啥用，所以我决定系统性的学习一下，这里我选择的是《jvm G1源码分析和调优》这本书，在读到5.4节的时候：

![image.png](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/68b48b8766044b3aaa9feb4d155612a1~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

发现有两个之前没有见过的参数:

- `G1LogLevel`

- `UnlockExperimentalVMOptions`，

从解释说明上来看配置了之后能获取到更加详细的YGC日志，于是加上了这个参数然后继续观察，日志格式太长，只截取了部分日志信息，有兴趣的可以下来自己打印一下：

![image.png](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/72a43f73e93449b8bb1a30d1fbc8ab26~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp) 可以发现在SystemDictionary Roots阶段是比较慢的，但是这个又是啥玩意呢？在书里面是没有任何介绍的，于是又进行大量谷歌，终于是找到了一篇你假笨写的一篇文章：[JVM源码分析之自定义类加载器如何拉长YGC](https://link.juejin.cn?target=http%3A%2F%2Flovestblog.cn%2Fblog%2F2016%2F03%2F15%2Fygc-classloader%2F "http://lovestblog.cn/blog/2016/03/15/ygc-classloader/")，强烈推荐大家读完这篇文章。

好了最后我来盘一盘到底为什么会出现gc慢的问题呢？
我们这个定时任务是一个定时查询微信退款信息的，微信的退款信息需要解析XML,就有如下代码:

![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/b94f6dc81be9448795a74fd434b6b95c~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

而我们的罪魁祸首其实就在这个`new XStream`这个方法中，我们的默认构造方法会调用下面的这个构造方法：

![image.png](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/68c40f1601d44b63ab33ef38e542ccda~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

需要注意的是我们每次创建一个`XStream`都会新创建一个ClassLoader，先解释一下`ClassLoader`，这里直接引用你假笨的一段话：

> 这里着重要说的两个概念是初始类加载器和定义类加载器。举个栗子说吧，`AClassLoader`->`BClassLoader`->`CClassLoader`，表示`AClassLoader`在加载类的时候会委托`BClassLoader`类加载器来加载，`BClassLoader`加载类的时候会委托`CClassLoader`来加载，假如我们使用`AClassLoader`来加载`X`这个类，而`X`这个类最终是被`CClassLoader`来加载的，那么我们称`CClassLoader`为X类的**定义类加载器**，而AClassLoader为X类的**初始类加载器**，JVM在加载某个类的时候对`AClassLoader`和`CClassLoader`进行记录，记录的数据结构是一个叫做`SystemDictionary`的hashtable，其key是根据`ClassLoader`对象和类名算出来的hash值（其实是一个entry，可以根据这个hash值找到具体的index位置，然后构建一个包含kalssName和classloader对象的entry放到map里），而value是真正的由定义类加载器加载的Klass对象，因为初始类加载器和定义类加载器是不同的classloader，因此算出来的hash值也是不同的，因此在SystemDictionary里会有多项值的value都是指向同一个Klass对象。

我们把这个放到我们的场景来看就是下面这个情况：

![image.png](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/0b909888b57e48068b5bd016ec963caf~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

由于我们每次请求都会新创建一个`Xstream`对象，从而也会新创建一个ClassLoader，由于我们的ClassLoader的key是根据每个对象来算出来的hash值，如果每次都新创建，自然hash值不一样，从而导致我们有很多`ClassLoader`指向`XStream`这个class。为什么SystemDictionary的大小会影响我们GC时间呢？

> 想象一下这么个情况，我们加载了一个类，然后构建了一个对象(这个对象在eden里构建)当一个属性设置到这个类（static变量）里，如果gc发生的时候，这个对象是不是要被找出来标活才行，那么自然而然我们加载的类肯定是我们一项重要的gc root，这样SystemDictionary就成为了gc过程中的被扫描对象了。

我们的class信息是被分配在哪里的呢？在java7的话是在永久代，在java8就来到了**元数据空间**也就是我们的堆外，所以我们的young gc的时候是不会回收我们的class信息的，那么我们怎么解决这个问题呢？

- java7: 在G1垃圾收集器中，只有在进行full GC的时候，永久代才会被回收，这一过程是stop-the-world的。当不做Full GC的时候，G1运行是最优化的。只有当永久代满了或者应用分配内存的速度超过了G1回收垃圾的速度的时候，G1才会触发Full GC。在CMS垃圾收集器中，我们可以使用-XX:+CMSClassUnloadingEnabled在CMS concurrent cycle中回收集永久代。在G1里面没有对应的设置。G1只有在stop-the-world的Full GC的时候，才会回收永久代。我们可以根据应用的需要，设置PermSize和MaxPermSize参数来调优永久代的大小。
- java8：提供了四个参数-XX:MetaspaceSize，-XX:MaxMetaspaceSize，-XX:MinMetaspaceFreeRatio，-XX:MaxMetaspaceFreeRatio用来控制元空间的大小，当超过比例或者大小的时候就会进行收集。

但是我们这个问题不应该通过垃圾收集去解决，而是应该从根源上去解决，那就是不能使用默认的XStream构造函数,而是需要使用固定ClassLoader的构造函数。

![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/b4891a77ad7c4490ae90a01670e03758~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

经过修改之后上线，经过观察，没有出现慢GC的现象。

### 最后

经过这次排查的经验来看，遇到GC问题尤其是那种比较不常见的，真的是非常难搞，你可能需要对这个问题进行系统的学习，以及大量的查找资料才能找到原因，我在排查这个问题的时候掉了不少头发。在这里记录一下这个经验，希望对大家以后的一些排查能有帮助。

## [JVM源码分析之自定义类加载器如何拉长YGC - 你假笨](http://lovestblog.cn/blog/2016/03/15/ygc-classloader/)

## [juejin 大量类加载器创建导致诡异FullGC现象](https://juejin.cn/post/6882005163838537735)
