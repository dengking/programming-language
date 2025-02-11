# OS swap

## [juejin 一次JVM GC长暂停的排查过程](https://juejin.cn/post/7403337327831810088?searchId=202502101655176B1203FA5915C723154A)

> NOTE: 
> 
> [csdn 详细解析一次JVM GC长暂停的排查过程](https://blog.csdn.net/wdj_yyds/article/details/130367267?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7EPaidSort-1-130367267-blog-132728779.235%5Ev43%5Epc_blog_bottom_relevance_base6&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7EPaidSort-1-130367267-blog-132728779.235%5Ev43%5Epc_blog_bottom_relevance_base6&utm_relevant_index=1)  
> 
> [dev Troubleshooting a JVM GC Long Pause](https://dev.to/ppsrap/troubleshooting-a-jvm-gc-long-pause-10c8)

### 背景

在高并发下，Java程序的GC问题属于很典型的一类问题，带来的影响往往会被进一步放大。不管是「GC频率过快」还是「GC耗时太长」，由于GC期间都存在Stop The World问题，因此很容易导致服务超时，引发性能问题。

事情最初是线上某应用垃圾收集出现**Full GC**异常的现象，应用中个别实例**Full GC**时间特别长，持续时间约为15~30秒，平均每2周左右触发一次；

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/a9d72fc84ced42a1a89c8bb3545a625d~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=GgVblrJ1f3G8JyHp665viYLzsjo%3D)

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/69ba967edf6246079edf2d208e37d35a~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=27l1q7zUyLnZhvCgqUliGhSMzwI%3D)

JVM参数配置“-Xms2048M –Xmx2048M –Xmn1024M –XX:MaxPermSize=512M”

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/75eafaca97d74177b2e777f9b4a01118~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=6%2FliXrB%2BoQTigEw6jfvBKBKNmV0%3D)

### 排查过程

Ø **分析GC 日志**

GC 日志它记录了每一次的 GC 的执行时间和执行结果，通过分析 GC 日志可以调优堆设置和 GC 设置，或者改进应用程序的**对象分配模式**。

这里**Full GC**的reason是**Ergonomics**，是因为开启了**UseAdaptiveSizePolicy**，jvm自己进行自适应调整引发的**Full GC**。

这份日志主要体现GC前后的变化，目前为止看不出个所以然来。

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/970bfea1db4a40ae8622d9e0ac389aa4~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=%2Bx4Bb%2BTAtjvJoHRs6c%2BDDF10%2BCM%3D)

开启GC日志，需要添加如下 JVM 启动参数：

```shell
-XX:+PrintGCDetails -XX:+PrintGCDateStamps -Xloggc:/export/log/risk_pillar/gc.log
```

常见的 **Young GC**、**Full GC** 日志含义如下：

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/af73452ebb6b4eaba3155f9752031356~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=lLMbEypz19PeuXX85UyBvv41xf4%3D)

Ø **进一步查看服务器性能指标**

> NOTE: 这一段体现了发现和排查问题的思路

获取到了GC耗时的时间后，通过监控平台获取到各个**监控项**，开始排查这个时点有异常的指标，最终分析发现，在5.06分左右（GC的时点），CPU占用显著提升，而SWAP出现了释放资源、memory资源增长出现**拐点**的情况（详见下图红色框，橙色框中的变化是因修改配置导致，后面会介绍，暂且可忽略）

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/9589d7cc2f0f404b826e8653c12ef36b~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=zWYu4%2Bblw3cLRqOiecSWvAA3YhY%3D)

JVM用到了**swap**？是因为GC导致的CPU突然飙升，并且释放了swap交换区这部分内存到memory？

为了验证JVM是否用到swap，我们通过检查proc下的进程内存资源占用情况

```shell
for i in (cd/proc;ls∣grep"[0−9]"∣awk′0 >100') ;do awk '/Swap:/{a=a+2}END{print '"i"',a/1024"M"}' /proc/$i/smaps 2>/dev/null ; done | sort -k2nr | head -10
```

`head -10` 表示 取出 前10个内存占用高的进程；取出的第一列为进程的id，第二列进程占用swap大小。 

看到确实有用到305MB的swap

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/32ac6e3f217d4a749cb1e55056c1c892~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=7C3LnzcsVemLNSXIZefxWikNORk%3D)

这里简单介绍下什么是**swap**?

swap指的是一个交换分区或文件，主要是在内存使用存在压力时，触发内存回收，这时可能会将部分内存的数据交换到swap空间，以便让系统不会因为内存不够用而导致oom或者更致命的情况出现。

当某进程向OS请求内存发现不足时，OS会把内存中暂时不用的数据交换出去，放在swap分区中，这个过程称为swap out。

当某进程又需要这些数据且OS发现还有空闲物理内存时，又会把swap分区中的数据交换回物理内存中，这个过程称为swap in。

为了验证GC耗时与swap操作有必然关系，我抽查了十几台机器，重点关注耗时长的GC日志，通过时间点确认到GC耗时的时间点与swap操作的时间点确实是一致的。

进一步查看虚拟机各实例 swappiness 参数，一个普遍现象是，凡是发生较长Full GC的实例都配置了参数 vm.swappiness = 30（值越大表示越倾向于使用swap）；而GC时间相对正常的实例配置参数 vm.swappiness = 0（最大限度地降低使用swap）。

swappiness 可以设置为 0 到 100 之间的值，它是Linux的一个内核参数，控制系统在进 行swap时，内存使用的相对权重。

Ø swappiness=0: 表示最大限度使用物理内存，然后才是 swap空间

Ø swappiness=100: 表示积极的使用swap分区，并且把内存上的数据及时的交换到swap空间里面

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/153f468267d2498e8aef7d5fd2a352fa~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=elMadYEuoA6b2ImAM8LBSaxwXgQ%3D)

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/ec93a9a345d844ec86f6f22b7dcc0bad~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=Rm5nKZEFrcG2SerEqoX2ACov088%3D)

对应的物理内存使用率和swap使用情况如下

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/285faba1dffb4a0f9daff0774a6dfa29~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=86YR18LTXidNxPRhlSs3Lw9OITc%3D)

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/8915a5c1a7c64e948140b2a4755a3e9f~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=jew9E4xjLk3v1Qaf2JVDOmuLr4U%3D)

至此，矛头似乎都指向了swap。

Ø **问题分析**

当内存使用率达到水位线(vm.swappiness)时，linux会把一部分暂时不使用的内存数据放到磁盘swap去，以便腾出更多可用内存空间；

当需要使用位于swap区的数据时，再将其换回内存中，当JVM进行GC时，需要对相应堆分区的已用内存进行遍历；

假如GC的时候，有堆的一部分内容被交换到swap空间中，遍历到这部分的时候就需要将其交换回内存，由于需要访问磁盘，所以相比物理内存，它的速度肯定慢的令人发指，GC停顿的时间一定会非常非常恐怖；

进而导致Linux对swap分区的回收滞后（内存到磁盘换入换出操作十分占用CPU与系统IO），在高并发/QPS服务中，这种滞后带来的结果是致命的(STW)。

Ø **问题解决**

至此，答案似乎很清晰，我们只需尝试把swap关闭或释放掉，看看能否解决问题？

**如何释放swap?**

1. 设置vm.swappiness=0（重启应用释放swap后生效），表示尽可能不使用交换内存

a、 临时设置方案，重启后不生效

### 设置vm.swappiness为0

sysctl vm.swappiness=0

### 查看swappiness值

cat /proc/sys/vm/swappiness

b、 永久设置方案，重启后仍然生效

vi /etc/sysctl.conf

### 添加

vm.swappiness=0

2. 关闭交换分区swapoff –a

前提：首先要保证内存剩余要大于等于swap使用量，否则会报Cannot allocate memory！swap分区一旦释放，所有存放在swap分区的文件都会转存到物理内存上，可能会引发系统IO或者其他问题。

a、 查看当前swap分区挂载在哪？

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/fa595af6208f476d8bd248c580ef7898~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=SGaelruyRu386gvJrRIcutBHV70%3D)

b、 关停分区

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/21f42b84ac0640aeb628511350e68833~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=TOi%2BCeG3yf0GTggERWIBHTXDUbc%3D)

关闭swap交换区后的内存变化见下图橙色框，此时swap分区的文件都转存到了物理内存上

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/4fc08847188c4a09a3bb63bb070f5f4b~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=1XLf%2F7lwV4Ip5X975UqdratxcvI%3D)

关闭Swap交换区后，于2.23再次发生Full GC，耗时190ms，问题得到解决。

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/4e8b9ed3756b4068b5d8e541463d70d8~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=4GMZtaMEB2rT5JrZT2LFq56TgyI%3D)

Ø **疑惑**

**1、 是不是只要开启了swap交换区的JVM，在GC的时候都会耗时较长呢？**

**2、 既然JVM对swap如此不待见，为何JVM不明令禁止使用呢？**

**3、 swap工作机制是怎样的？这台物理内存为8g的server，使用了交换区内存（swap），说明物理内存不够使用了，但是通过free命令查看内存使用情况，实际物理内存似乎并没有占用那么多，反而Swap已占近1G？**

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/1dd2a9649cd8437187d5f62520288205~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=oaWC2jyC1u3lB3vF6QlGJ4QxZXY%3D)

free：除了buff/cache剩余了多少内存

shared：共享内存

buff/cache：缓冲、缓存区内存数（使用过高通常是程序频繁存取文件）

available：真实剩余的可用内存数

---

大家可以想想，关闭交换磁盘缓存意味着什么？

其实大可不必如此激进，要知道这个世界永远不是非0即1的，大家都会或多或少选择走在中间，不过有些偏向0，有些偏向1而已。

很显然，在swap这个问题上，JVM可以选择偏向尽量少用，从而降低swap影响，要降低swap影响有必要弄清楚Linux内存回收是怎么工作的，这样才能不遗漏任何可能的疑点。

**先来看看swap是如何触发的？**

Linux会在两种场景下触发内存回收，一种是在内存分配时发现没有足够空闲内存时会立刻触发内存回收；另一种是开启了一个守护进程（kswapd进程）周期性对系统内存进行检查，在可用内存降低到特定阈值之后主动触发内存回收。

通过如下图示可以很容易理解，详细信息参见：[hbasefly.com/2017/05/24/…](https://link.juejin.cn?target=http%3A%2F%2Fhbasefly.com%2F2017%2F05%2F24%2Fhbase-linux%2F "http://hbasefly.com/2017/05/24/hbase-linux/")

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/172894f770664e9b9296104563af7faa~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=thILFaKmmqTDDJ3gGqoubTjRpA8%3D)

**解答是不是只要开启了swap交换区的JVM，在GC的时候都会耗时较长**

笔者去查了一下另外的一个应用，相关指标信息请见下图。

实名服务的QPS是非常高的，同样能看到应用了swap，GC平均耗时 576ms，这是为什么呢？

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/7c1feebe82834a72be9996ea3f85c1fe~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=xnNgKE%2B47FFNPF5yNoSHESWwX8E%3D)

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/120cd6f65f454ab999704008dc1471d0~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=TElMojSolTvM4RKyZHPV45rnaqo%3D)

通过把时间范围聚焦到发生GC的某一时间段，从监控指标图可以看到swapUsed没有任何变化，也就是说没有swap活动，进而没有影响到垃级回收的总耗时。

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/579dc16517c14be38a8a81ee3dadc960~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=zPe71nyENPZKoJMRX9qcTNYnhtY%3D)

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/a30e727816e4402e88dd0089f65d8933~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=dfVYDv5615Yo%2Fq%2BjLvLGOhCvF2A%3D)

通过如下命令列举出各进程swap空间占用情况，很清楚的看到实名这个服务swap空间占用的较少（仅54.2MB）

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/5a9dff57d61845838640b373c09d5475~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=eq9IrLtp%2FG%2BlvPibIfLkRLjZjpM%3D)

另一个显著的现象是实名服务Full GC间隔较短（几个小时一次），而我的服务平均间隔2周一次Full GC

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/f2528a0329aa4a9d9b193467ef279f55~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=p9KtpXm1XzbXhZnGIJu%2B4mgW3C4%3D)

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/9e6accd760db4491ba1fac80291f53bf~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=0dBiwrTlQvR9Luth0rvhEVP6vHo%3D)

基于以上推测

1、 实名服务由于 GC 间隔较短，内存中的东西根本没有机会置换到swap中就被回收了，GC的时候不需要将swap分区中的数据交换回物理内存中，完全基于内存计算，所以要快很多

2、 将哪些内存数据置换进swap交换区的筛选策略应该是类似于LRU算法（最近最少使用原则）

为了证实上述猜测，我们只需跟踪swap变更日志，监控数据变化即可得到答案，这里采用一段shell 脚本实现

| #!/bin/bash echo -e `date +%y%m%d%H%M%S` echo -e "PID\t\tSwap\t\tProc_Name" # 拿出/proc目录下所有以数字为名的目录（进程名是数字才是进程，其他如sys,net等存放的是其他信息） for pid in `ls -l /proc | grep ^d | awk '{ print $9 }'| grep -v [^0-9]` do if [ pid−eq1];thencontinue;figrep−q"Swap"/proc/pid/smaps 2>/dev/null if [ ?−eq0];thenswap=(gawk '/Swap/{ sum+=2;} END{ print sum }' /proc/pid/smaps) #统计占用的swap分区的大小 单位是KB proc_name=(psaux∣grep−w"pid" | awk '!/grep/{ for(i=11;i<=NF;i++){ printf("%s ",i); }}') #取出进程的名字 if [ swap -gt 0 ];then #判断是否占用swap 只有占用才会输出 echo -e "pid\t{swap}\t{proc_name:0:100}" fi fi done | sort -k2nr | head -10 | gawk -F'\t' '{ #排序取前 10 pid[NR]=1; size[NR]=2;name[NR]=3; } END{ for(id=1;id<=length(pid);id++) { if(size[id]<1024) printf("%-10s\t%15sKB\t%s\n",pid[id],size[id],name[id]); else if(size[id]<1048576) printf("%-10s\t%15.2fMB\t%s\n",pid[id],size[id]/1024,name[id]); else printf("%-10s\t%15.2fGB\t%s\n",pid[id],size[id]/1048576,name[id]); } }' |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |

由于上面图中 2022.3.2 19:57:00 至 2022.3.2 19:58:00 发生了一次Full GC，我们重点关注下这一分钟内swap交换区的变化即可，我这里每10s做一次信息采集，可以看到在GC时点前后，swap确实没有变化

![](https://p6-xtjj-sign.byteimg.com/tos-cn-i-73owjymdk6/599b6ed742604adba1f8c32f3e690c25~tplv-73owjymdk6-jj-mark-v1:0:0:0:0:5o6Y6YeR5oqA5pyv56S-5Yy6IEAg5Lqs5Lic5LqR5byA5Y-R6ICF:q75.awebp?rk3s=f64ab15b&x-expires=1739290899&x-signature=BUZ87Il2qEj8z%2F%2FRK7JvCmsMVtk%3D)

通过上述分析，回归本文核心问题上，现在看来我的处理方式过于激进了，其实也可以不用关闭swap，通过适当降低堆大小，也是能够解决问题的。

这也侧面的说明，部署Java服务的Linux系统，在内存分配上并不是无脑大而全，需要综合考虑不同场景下JVM对Java永久代 、Java堆(新生代和老年代)、线程栈、Java NIO所使用内存的需求。

### 总结

综上，我们得出结论，swap和GC同一时候发生会导致GC时间非常长，JVM严重卡顿，极端的情况下会导致服务崩溃。

主要原因是：JVM进行GC时，需要对对应堆分区的已用内存进行遍历，假如GC的时候，有堆的一部分内容被交换到swap中，遍历到这部分的时候就须要将其交换回内存；更极端情况同一时刻因为内存空间不足，就需要把内存中堆的另外一部分换到SWAP中去，于是在遍历堆分区的过程中，会把整个堆分区轮流往SWAP写一遍，导致GC时间超长。线上应该限制swap区的大小，如果swap占用比例较高应该进行排查和解决，适当的时候可以通过降低堆大小，或者添加物理内存。

因此，部署Java服务的Linux系统，在内存分配上要慎重。

以上内容希望可以起到抛转引玉的作用，如有理解不到位的地方烦请指出。

作者：京东云开发者  
链接：https://juejin.cn/post/7403337327831810088  
来源：稀土掘金  
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
