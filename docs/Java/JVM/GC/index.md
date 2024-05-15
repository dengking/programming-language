# Java GC



## Generational GC



深入理解Java虚拟机：JVM高级特性与最佳实践（第3版）周志明: 

> 注意 刚才我们已经提到了“Minor GC”，后续文中还会出现其他针对不同分代的类似名词， 为避免读者产生混淆，在这里统一定义： 
>
> - 部分收集（Partial GC）：指目标不是完整收集整个Java堆的垃圾收集，其中又分为： 
>
>   - 新生代收集（Minor GC/Young GC）：指目标只是新生代的垃圾收集。 
>
>   - 老年代收集（Major GC/Old GC）：指目标只是老年代的垃圾收集。目前只有CMS收集器会有单 独收集老年代的行为。另外请注意“Major GC”这个说法现在有点混淆，在不同资料上常有不同所指， 读者需按上下文区分到底是指老年代的收集还是整堆收集。 
>
>   - 混合收集（Mixed GC）：指目标是收集整个新生代以及部分老年代的垃圾收集。目前只有G1收 集器会有这种行为。 
>
> - 整堆收集（Full GC）：收集整个Java堆和方法区的垃圾收集。
>
> 

### gpt-4-vision

**Generational Garbage Collection** is a fundamental concept in memory management that is employed by many modern garbage collectors, including those in Java. The core idea behind **generational collection** is that most objects are short-lived, and as such, it's more efficient to organize memory into different "generations" that reflect the lifespan of stored objects.

> NOTE:
>
> 一、根据object或者更加具体的说，是根据memory的lifespan来进行组织分类，显然memory的lifespan是GC非常关注的一个内容

Here's a breakdown of how generational garbage collection typically works:

#### Generations:

1. **Young Generation(新生代)**: This is where new objects are allocated. The young generation is further divided into three parts:
   - **Eden Space**: Where most new objects are allocated.
   - **Survivor Spaces (S0 and S1)**: When a minor garbage collection occurs, objects that have survived are moved from Eden to one of the survivor spaces. In subsequent collections, objects are moved back and forth between the two survivor spaces until they are old enough to be moved to the old generation.

2. **Old Generation (Tenured Generation)(老年代)**: Objects that have lived long enough in the young generation (survived several garbage collection cycles) are promoted to the old generation. This space is for objects that have a longer lifetime.
3. **Permanent Generation (PermGen) or Metaspace (in Java 8 and above)**: This is not part of the heap but a separate memory space that stores metadata such as class definitions and method data. In Java 8, PermGen was replaced by Metaspace, which is located in the native memory.

#### Garbage Collection Phases:

1. **Minor GC**: This happens when the young generation fills up. It's typically fast and involves moving live objects from Eden to one of the survivor spaces, and from one survivor space to the other. Objects that are old enough or if the survivor space is not large enough are promoted to the old generation.
2. **Major GC (Full GC)**: This involves cleaning the old generation and is usually much slower than a minor GC because it involves a larger area of memory and often requires scanning the entire heap. A major GC also includes a minor GC.

#### Advantages of Generational Garbage Collection:

- **Efficiency**: Since most objects die young, collecting garbage in the young generation is fast and can often reclaim a significant amount of space due to the high mortality(死亡率) rate of young objects.

- **Reduced Fragmentation**: Moving objects from Eden to the survivor spaces and then to the old generation helps to compact memory, reducing fragmentation.

  > NOTE:
  >
  > 一、这如何理解？

- **Performance**: By focusing on the young generation, where most garbage is likely to be found, the garbage collector can run more frequently but with less impact on performance.

#### Java Garbage Collectors:

Different garbage collectors in Java use the generational concept with various algorithms and strategies:

- **Serial GC**: Uses a single thread for garbage collection events and is suitable for small applications with a small amount of data.
- **Parallel GC (Throughput Collector)**: Uses multiple threads for young generation garbage collection and a single thread for old generation garbage collection. It's designed for applications with medium to large data sets that are run on multiprocessor or multi-threaded hardware.
- **Concurrent Mark Sweep (CMS) GC**: Attempts to minimize pauses by doing most of the garbage collection work concurrently with the application threads.
- **G1 (Garbage-First) GC**: Divides the heap into regions and uses multiple threads to scan them incrementally, aiming to predictably maintain a low pause time.
- **ZGC (Z Garbage Collector) and Shenandoah**: Aim for low pause times by performing more work concurrently with the running Java threads.

Understanding generational garbage collection is crucial for Java developers, especially when tuning the garbage collector for performance or debugging memory issues.



### Full GC

#### gpt-4-vision

A Full GC (Garbage Collection) is an event in the Java Virtual Machine (JVM) that stops all application threads to reclaim memory in the entire Java heap. During a Full GC, the garbage collector identifies and removes objects that are no longer in use and compacts the heap to free up space. Full GC events are typically more time-consuming than other types of GC events, such as Minor GCs or Young Generation collections, because they involve the entire heap, including both the Young and Old generations.

Here are some key points about Full GC:

1. **Stop-the-World Event**: Full GC is a "stop-the-world" event, meaning that all application threads are paused during the garbage collection process to ensure that the memory can be safely reclaimed without interference from running threads.

2. **Generational Garbage Collection**: In generational garbage collectors like the Parallel GC, CMS, and G1 GC, a Full GC is usually triggered when the Old Generation becomes full or when the garbage collector is unable to reclaim enough space in the Young Generation during a Minor GC.

3. **Heap Compaction**: Full GC often includes heap compaction, which reduces memory fragmentation by moving objects so that they are contiguous in memory. This can improve performance by optimizing memory access patterns.

4. **Causes of Full GC**: Full GC can be triggered by various causes, such as:
   - Explicit calls to `System.gc()`.
   - Allocation failures when there's not enough space in the Old Generation to accommodate new objects promoted from the Young Generation.
   - JVM internal operations, such as class metadata allocation or interned string allocation, that require space in the Old Generation.
   - Invocation of the garbage collector by the JVM when it detects that it is running low on free memory.

5. **Impact on Performance**: Full GC events can have a significant impact on application performance due to their longer pause times. Applications may experience latency spikes and reduced throughput during Full GC.

6. **Monitoring and Tuning**: It's important to monitor Full GC events using tools like Java Mission Control, VisualVM, or garbage collection logs. If Full GC events are too frequent or too long, it may be necessary to tune the JVM's garbage collection settings or adjust the application's memory usage patterns.

7. **Avoiding Full GC**: To minimize the impact of Full GC, developers can:
   - Increase the size of the Old Generation to delay the need for Full GC.
   - Optimize the application's memory allocation patterns to reduce the rate of object promotion to the Old Generation.
   - Use garbage collectors designed to minimize pause times, such as G1 GC, ZGC, or Shenandoah, which aim to perform most of their work concurrently with the application threads.

In summary, Full GC is a critical but potentially disruptive(破坏性的) process in the JVM's memory management. Understanding and managing Full GC events is an essential part of optimizing Java application performance.



## Eden Space(伊甸园)

### gpt-4-vision

In Java's memory model, particularly in the context of **generational garbage collection**, the **Eden space** is the area of the heap where new objects are allocated. When an object is first created, it is placed in the **Eden space**, which is a part of the **Young Generation**.

Here's how the **Eden space** fits into the overall garbage collection process:

1. **Allocation**: New objects are typically allocated in the **Eden space**. This space is designed to be large enough to hold a significant number of objects and is very fast to allocate from because it typically involves just a pointer bump(碰撞).

2. **Survival**: When a garbage collection event occurs in the **Young Generation** (also known as a **Minor GC**), live objects from the Eden space are moved to one of the Survivor spaces (S0 or S1). Objects that are no longer referenced are simply discarded, as the entire **Eden space** is cleared during a **Minor GC**(彻底清扫干净Eden space).

3. **Promotion**: Objects that survive garbage collection in the Young Generation and have been moved between Survivor spaces a certain number of times (determined by a threshold called the "tenuring threshold"(任期阈值)) are then "promoted" to the Old Generation. The Old Generation is intended for objects with a longer lifespan and is collected less frequently.

The reason for this separation into generations, and specifically the use of the **Eden space**, is based on the observation that most objects are **short-lived**(基于经验主义). By frequently cleaning up the **Eden space**, where most objects "die," the garbage collector can reclaim memory with minimal effort and time, leading to better application performance.

The size of the **Eden space**, as well as the sizes of the **Survivor spaces** and the **Old Generation**, can be tuned using JVM options. Proper tuning can lead to significant performance improvements, especially for applications with specific memory usage patterns.

For example, you can set the initial and maximum size of the heap, as well as the sizes of the **Young Generation** and Eden space, with the following JVM options:

- `-Xms<size>`: Sets the initial size of the heap.
- `-Xmx<size>`: Sets the maximum size of the heap.
- `-Xmn<size>`: Sets the size of the Young Generation.
- `-XX:NewRatio=<ratio>`: Sets the ratio between the Young and Old Generation.
- `-XX:SurvivorRatio=<ratio>`: Sets the ratio between the Eden space and one Survivor space.

Adjusting these parameters can help optimize garbage collection performance, but it often requires careful analysis and testing to find the best settings for a particular application.



## How to select gc algorithm in java

In Java, you can select the Garbage Collection (GC) algorithm by passing specific command-line options to the Java Virtual Machine (JVM) when starting your application. The choice of GC algorithm can have a significant impact on the performance characteristics of your application, such as throughput(吞吐量), latency(延时), and memory footprint(内存占用).

> NOTE:
>
> 一、深入理解Java虚拟机：JVM高级特性与最佳实践（第3版）周志明： 
>
> > 衡量垃圾收集器的三项最重要的指标是：内存占用（Footprint）、吞吐量（Throughput）和延迟 （Latency），三者共同构成了一个“[不可能三角](https://en.wikipedia.org/wiki/Impossible_trinity)”。三者总体的表现会随技术进步而越来越好，但是 要在这三个方面同时具有卓越表现的“完美”收集器是极其困难甚至是不可能的，一款优秀的收集器通 常最多可以同时达成其中的两项。

As of Java 9 and later, the JVM has several garbage collectors from which you can choose:

1. **Serial GC**: Suitable for small applications with a single-threaded environment.
   - To enable: `-XX:+UseSerialGC`

2. **Parallel GC (Throughput Collector)**: Suitable for multi-threaded applications that require high throughput.
   - To enable: `-XX:+UseParallelGC`

3. **Concurrent Mark Sweep (CMS) GC**: Designed to minimize application pause times. (Deprecated in Java 9 and removed in Java 14)
   - To enable: `-XX:+UseConcMarkSweepGC`

4. **G1 GC (Garbage-First Collector)**: Suitable for applications with a large heap size that require predictable pause times.
   - To enable: `-XX:+UseG1GC`

5. **ZGC (Z Garbage Collector)**: Designed for low-latency and scalability up to multi-terabyte heaps. (Available from Java 11)
   - To enable: `-XX:+UseZGC`

6. **Shenandoah GC**: Similar to ZGC, aims for low pause times without compromising throughput. (Available from Java 12)
   - To enable: `-XX:+UseShenandoahGC`

7. **Epsilon GC**: A no-op garbage collector that handles memory allocation but does not actually perform any garbage collection. It's useful for performance testing and extremely short-lived jobs.
   - To enable: `-XX:+UseEpsilonGC`

8. **Omega GC**: An experimental garbage collector that is not yet part of the standard JVM distribution.

To select a GC algorithm, you would typically add the corresponding option to your Java command line. For example:

```sh
java -XX:+UseG1GC -jar my-application.jar
```

This command would start your Java application with the G1 garbage collector enabled.

It's important to note that some GC options are platform-specific or may not be available in all JVM implementations or versions. Additionally, the default GC algorithm may vary depending on the JVM version and the characteristics of the system (e.g., the number of CPUs, the size of the heap, etc.).

Before selecting a GC algorithm, it's recommended to understand the needs of your application and to perform thorough(彻底的、完整的) testing under realistic workloads to determine the most suitable GC strategy.



## How to list gc algorithm in java

### gpt-4-vision

As of Java 9 and later, there isn't a direct command-line option that lists all available Garbage Collection (GC) algorithms. However, you can get an idea of the available GC algorithms by looking at the JVM options related to garbage collection. 

To see the available GC-related options, you can use the following command:

```sh
java -XX:+PrintFlagsFinal -version | grep -iE 'Use.*GC'
```

This command will print all JVM flags that are final (i.e., their values won't change after JVM startup) and filter for flags that contain "Use" and "GC". The output will include flags that enable different GC algorithms.

Here's an example of what the output might look like:

```sh
     bool UseParallelGC                          = true                                {product}
     bool UseSerialGC                            = false                               {product}
     bool UseConcMarkSweepGC                     = false                               {product} {deprecated}
     bool UseG1GC                                = false                               {product}
     bool UseZGC                                 = false                               {product}
     bool UseShenandoahGC                        = false                               {product}
     bool UseEpsilonGC                           = false                               {experimental}
     ...
```

Each line represents a JVM flag that can be used to enable a specific GC algorithm. The `true` or `false` value indicates the default setting for that flag. Note that some flags may be marked as `{deprecated}` or `{experimental}`, indicating their status in the JVM.

Keep in mind that the availability of GC algorithms may vary depending on the JVM implementation and version you are using. For example, ZGC and Shenandoah GC are not available in all JVM distributions or versions.

To get the most accurate and up-to-date information about the GC algorithms available in your specific JVM, you should refer to the official documentation of the JVM distribution you are using. Additionally, you can check the release notes of your JVM version for any changes or updates to the available GC algorithms.



## Parallel GC algorithm

### PS Scavenge(清除污物)

#### gpt-4-vision

`PS Scavenge` is the name of the Garbage Collector (GC) that handles the young generation part of the heap when using the **Parallel GC algorithm** in the HotSpot Java Virtual Machine (JVM). The "PS" stands for "Parallel Scavenge."

The **Parallel GC** is designed for applications with medium to large data sets that are run on multiprocessor or multi-threaded hardware. It aims to maximize application throughput by performing most of the garbage collection work concurrently with the application threads.

In the context of Parallel GC, there are typically two main garbage collectors:

1. **PS Scavenge**: This collector is responsible for the **young generation** (also known as the "eden" space and "survivor" spaces). It uses a parallel version of the copying algorithm, which is efficient because it assumes that most objects in the young generation are short-lived and can be collected quickly.

2. **PS MarkSweep**: This collector handles the **old generation** (also known as the **"tenured" generation**). It uses a parallel version of the **mark-sweep-compact algorithm**, which is designed to deal with longer-lived objects.

When the `PS Scavenge` collector runs, it typically triggers a minor GC, which cleans up the young generation by moving surviving objects to the old generation or to the survivor space within the young generation.

To enable the Parallel GC, you can use the `-XX:+UseParallelGC` JVM flag when starting your Java application. If you want to see whether the Parallel GC is being used in a running JVM, you can use the `jinfo` command:

```sh
jinfo -flag UseParallelGC <pid>
```

Replace `<pid>` with the process ID of your Java application. If the output shows that the `UseParallelGC` flag is enabled, then the `PS Scavenge` collector is being used for the young generation.





## Z Garbage Collector

### gpt-4-vision

The Z Garbage Collector (ZGC) is a scalable, low-latency garbage collector introduced in Java 11 as an experimental feature and later made production-ready in Java 15. ZGC is designed to handle large heaps with ease while keeping pause times low, regardless of the size of the heap. It's particularly well-suited for applications that require both a large heap and minimal pause times, such as high-frequency trading platforms or big data processing systems.

Key features of ZGC include:

1. **Concurrent Phases**: ZGC performs most of its work concurrently with the application threads. This means that it doesn't need to stop the application for extended periods to perform garbage collection, which results in low pause times.

2. **Region-Based Memory**: ZGC organizes the heap into regions, which can be managed independently. This approach allows ZGC to scale with the size of the heap and the number of processors.

3. **Load Barriers**: ZGC uses load barriers to handle references between objects. These barriers are used to perform necessary actions when a reference to an object is loaded, such as remapping the reference if the object has been moved.

4. **Colored Pointers**: ZGC uses colored pointers to encode information about the objects, such as whether the object is live, if it has been moved, or if it's in the process of being marked. This technique helps ZGC to manage and track objects efficiently.

5. **Multi-Threaded Compaction**: ZGC can compact the heap while the application is running, using multiple threads to relocate objects. This helps to prevent memory fragmentation and maintain high application throughput.

6. **Pause Time Goals**: ZGC aims to keep pause times below a certain threshold, typically around 10 milliseconds. This is achieved by splitting the work into small increments that can be executed in parallel with the application threads.

7. **No Full GC**: Unlike some other garbage collectors, ZGC is designed to avoid full garbage collection events, which can cause long pauses. Instead, it incrementally collects and compacts the heap.

To use ZGC, you need to run your application on a JVM that supports it (Java 11 or later) and enable it with the following JVM option:

```
-XX:+UseZGC
```

You can also configure additional options to control the behavior of ZGC, such as the maximum heap size or the desired pause time goal.

It's important to note that while ZGC is designed for low-latency and scalability, it may not be the best choice for every application. For instance, applications with small heaps or those that can tolerate longer pause times might be better served by other garbage collectors like G1 or Parallel GC. As with any performance tuning, it's essential to test and measure the impact of using ZGC with your specific application workload to determine if it's the right choice.

## G1 GC

### gpt-4-vision

The Garbage-First Collector (G1 GC) is a modern, low-pause, server-style garbage collector that was introduced in Java 7 Update 4 and became the default garbage collector in Java 9. It is designed to better support heaps ranging from a few gigabytes to hundreds of gigabytes in size, while maintaining high throughput and predictable pause times.

G1 GC operates under a few key principles and features:

#### Region-Based Heap:

- The heap is divided into equal-sized regions, which can be individually managed and collected. This division allows G1 to more efficiently manage memory and collect garbage incrementally, avoiding long pauses.

#### Generational Collection:

- Although G1 is not strictly a generational collector, it conceptually separates the heap into young and old generations. Young generation collections (also known as minor GCs) are typically more frequent and aim to reclaim short-lived objects, while full GCs are less frequent and involve the entire heap.

#### Pause-Time Prediction:

- G1 aims to meet a set pause-time target (which can be specified by the user) by estimating the amount of memory that can be reclaimed within that time. It uses this prediction to decide which regions to collect during a GC cycle.

#### Evacuation Pauses:

- G1 performs "evacuation pauses" (both young and mixed) to move live objects out of regions that are being reclaimed. Young pauses clean only the young generation, while mixed pauses clean both young and old generations.

#### Incremental Collection:

- G1 can incrementally collect regions of the heap to spread the collection work over time and avoid long pauses.

#### Garbage-First:

- The name "Garbage-First" comes from the strategy of prioritizing the collection of regions with the most garbage, hence reclaiming space with minimal effort and improving efficiency.

#### Usage:

To enable G1 GC, you can use the following JVM option:

```
-XX:+UseG1GC
```

You can also specify additional options to tune the behavior of G1 GC, such as:

- `-XX:MaxGCPauseMillis=<value>`: Sets a target for the maximum GC pause time.
- `-XX:GCPauseIntervalMillis=<value>`: Sets a target for the interval between pauses.
- `-XX:InitiatingHeapOccupancyPercent=<value>`: Sets the heap occupancy threshold that triggers a concurrent GC cycle.

G1 GC is particularly well-suited for applications that require large heaps with short GC pause times. However, it's important to note that while G1 can significantly reduce pause times compared to older garbage collectors like Parallel GC or CMS, it may not always achieve the lowest possible pause times for all workloads. For applications with extremely low pause time requirements, other collectors like ZGC or Shenandoah might be more appropriate.

As with any garbage collector, the best way to determine if G1 GC is suitable for a particular application is through thorough testing and performance tuning.



## Monitoring

在 Full GC gpt-4-vision中有如下描述: 

> It's important to monitor Full GC events using tools like Java Mission Control, VisualVM, or garbage collection logs. If Full GC events are too frequent or too long, it may be necessary to tune the JVM's garbage collection settings or adjust the application's memory usage patterns.

上面这段话提示了monitor JVM的重要性。

### Monitoring a cluster of JVMs

Monitoring a cluster of JVMs involves collecting, aggregating, and analyzing metrics from multiple Java applications running on different nodes. Here are some steps and tools you can use to effectively monitor a cluster of JVMs:

1. **Enable JMX (Java Management Extensions)**:
   - JMX is a standard technology that provides a simple and standard way to manage and monitor Java applications. Enable JMX on each JVM instance to expose various metrics and management operations.

2. **Use Monitoring Tools**:
   - There are several tools available for monitoring JVM clusters. Some popular ones include:
     - **JConsole**: A JMX-compliant graphical tool for monitoring Java applications.
     - **VisualVM**: A visual tool that integrates several command-line JDK tools and lightweight profiling capabilities.
     - **Java Mission Control (JMC)**: A set of tools for monitoring, managing, and profiling Java applications without introducing the performance overhead normally associated with profiling tools.

3. **Leverage APM (Application Performance Management) Solutions**:
   - APM solutions like New Relic, AppDynamics, Dynatrace, and Datadog provide comprehensive monitoring capabilities for JVM clusters. They offer features like distributed tracing, real-time analytics, and alerting.

4. **Use Open-Source Monitoring Tools**:
   - Tools like [Prometheus](https://en.wikipedia.org/wiki/Prometheus_(software)), [Grafana](https://en.wikipedia.org/wiki/Grafana), and ELK Stack ([Elasticsearch](https://en.wikipedia.org/wiki/Elasticsearch), [Logstash](https://www.elastic.co/guide/en/logstash/current/introduction.html), [Kibana](https://www.elastic.co/kibana)) can be used to collect, store, and visualize JVM metrics. Prometheus can scrape(收割) JMX metrics exposed by each JVM and store them in a time-series database. Grafana can then be used to create dashboards that visualize these metrics.

5. **Implement Custom Metrics Collection**:
   - If you have specific monitoring needs, you can implement custom metrics collection by using libraries like [Micrometer](https://micrometer.io/) or [Dropwizard Metrics](https://metrics.dropwizard.io/4.2.0/). These libraries can be integrated into your Java applications to collect and expose custom metrics.

6. **Centralize Log Management**:
   - Centralize logs from all JVMs using tools like Fluentd, Logstash, or Splunk. This will help you correlate(关联到一起) events across the cluster and quickly identify issues.

7. **Set Up Alerts and Notifications**:
   - Configure alerts based on specific metrics thresholds or patterns in logs. This will help you proactively(主动地) address issues before they impact the cluster's performance.

8. **Automate Monitoring Tasks**:
   - Use orchestration tools like [Kubernetes](https://en.wikipedia.org/wiki/Kubernetes), [Ansible](https://en.wikipedia.org/wiki/Ansible_(software)), or [Terraform](https://en.wikipedia.org/wiki/Terraform_(software)) to automate the deployment of monitoring agents and the configuration of monitoring settings across the cluster.

9. **Regularly Review and Analyze Metrics**:
   - Regularly review the collected metrics to understand the performance characteristics and resource usage patterns of your JVM cluster. Look for trends, anomalies, and potential bottlenecks.

10. **Perform Stress Testing**:
    - Conduct stress tests on your cluster to understand how it behaves under load. Use this information to fine-tune your monitoring setup and ensure that it provides the necessary visibility during high-traffic periods.

When monitoring a cluster of JVMs, it's important to focus on key metrics such as **heap memory usage**, **garbage collection statistics**, **thread activity**, **CPU usage**, **response times**, and **throughput**. By using a combination of the above tools and practices, you can gain a comprehensive view of your JVM cluster's health and performance.



### Open source and light weight tools to monitor a cluster of jvms

When it comes to monitoring a cluster of JVMs, there are several open-source and lightweight tools that you can use to collect, aggregate, and visualize metrics. Here are some popular options:

1. **Prometheus**:
   - Prometheus is an open-source monitoring system with a dimensional data model, flexible query language, efficient time series database, and modern alerting approach. It can scrape(收割) metrics exposed by JVMs over HTTP and store them in a time-series database.
   - You can use the **JMX exporter** to expose metrics from JVMs to Prometheus.

2. **Grafana**:
   - Grafana is an open-source platform for monitoring and observability. It allows you to query, visualize, alert on, and understand your metrics no matter where they are stored.
   - It integrates with Prometheus (and many other data sources) to create comprehensive dashboards for your JVM metrics.

3. **JVisualVM**:
   - JVisualVM is an all-in-one Java troubleshooting tool that comes bundled with the JDK. It can monitor local and remote JVMs, analyze heap dumps, capture thread dumps, and profile applications.

4. **jStatd**:
   - jStatd is a simple RMI server application provided with the JDK that monitors the creation and termination of instrumented Java HotSpot VMs and provides an interface to allow remote monitoring tools to attach to JVMs running on the local host.

5. **Elastic Stack (ELK)**:
   - The ELK Stack—Elasticsearch, Logstash, and Kibana—is a set of powerful open-source tools for searching, analyzing, and visualizing data in real-time.
   - [Filebeat](https://www.elastic.co/beats/filebeat) and [Metricbeat](https://www.elastic.co/beats/metricbeat) can be used to collect logs and metrics from JVMs and send them to Elasticsearch for storage and indexing.
   - Kibana can then be used to create visualizations and dashboards based on the collected data.

6. [**Netdata**](https://en.wikipedia.org/wiki/Netdata):
   - Netdata is a real-time performance monitoring solution that can monitor a wide range of systems and applications, including JVMs.
   - It is designed to run on all systems without disrupting their core function, providing insights in real-time.

7. [**SkyWalking**](https://skywalking.apache.org/):
   - Apache SkyWalking is an application performance monitoring tool that provides observability for distributed systems, including tracing, metrics, and diagnostics.
   - It supports Java and can be used to monitor JVM performance and distributed tracing.

8. **Micrometer**:
   - Micrometer is a metrics instrumentation library(仪器库) for JVM-based applications. It provides a simple facade over the instrumentation clients for various monitoring systems.
   - You can use Micrometer to instrument your code and expose JVM metrics, which can then be scraped(收割) by Prometheus or other monitoring systems.

9. **jConsole**:
   - jConsole is a JMX-compliant graphical tool for monitoring Java applications. It can connect to local and remote JVMs to monitor memory usage, thread usage, class loading, and runtime metrics.

10. [**Java Melody**](https://github.com/javamelody/javamelody):
    - Java Melody is designed for monitoring Java or Java EE applications in QA and production environments.
    - It focuses on simplicity and efficiency for both development and operations.

When choosing a tool, consider factors such as ease of use, integration with your existing infrastructure, the granularity of metrics, and the overhead on the monitored JVMs. It's also common to use a combination of these tools to achieve comprehensive monitoring coverage for your cluster of JVMs.



### JMX exporter to expose metrics from JVMs

The **JMX Exporter** is a tool that can be used to expose metrics from JVMs to Prometheus, which is a popular open-source monitoring and alerting toolkit. The JMX Exporter works as an agent which can be attached to the JVM or as a standalone process that connects to the JVM via remote JMX.

Here's how to use the JMX Exporter to expose metrics from JVMs:

1. **Download the JMX Exporter**:
   - You can download the latest JMX Exporter JAR from the official Prometheus JMX Exporter GitHub repository's releases page: https://github.com/prometheus/jmx_exporter/releases

2. **Configuration File**:
   - Create a configuration file in YAML format that specifies which JMX metrics you want to expose and how they should be transformed for Prometheus. An example configuration might look like this:

   ```yaml
   ---
   startDelaySeconds: 0
   jmxUrl: service:jmx:rmi:///jndi/rmi://localhost:12345/jmxrmi
   lowercaseOutputName: false
   lowercaseOutputLabelNames: false
   ssl: false
   rules:
     - pattern: 'java.lang<type=Memory><>(HeapMemoryUsage|NonHeapMemoryUsage):'
       name: jvm_memory_usage_bytes
       labels:
         area: "$1"
       value: value
   ```

   This configuration will expose memory usage metrics from the JVM.

3. **Run the JMX Exporter**:
   - There are two ways to run the JMX Exporter: as a Java agent or as a standalone process.

   **As a Java Agent**:
   - Add the JMX Exporter JAR to your application's JVM arguments with the path to the configuration file:

   ```
   java -javaagent:/path/to/jmx_prometheus_javaagent-<version>.jar=<port>:<config.yaml> -jar your-application.jar
   ```

   Replace `<version>` with the version of the JMX Exporter you downloaded, `<port>` with the port you want Prometheus to scrape metrics from, and `<config.yaml>` with the path to your configuration file.

   **As a Standalone Process**:
   - If you cannot modify the JVM arguments (e.g., when monitoring a third-party application), you can run the JMX Exporter as a standalone process:

   ```
   java -jar jmx_prometheus_httpserver-<version>-jar-with-dependencies.jar <port> <config.yaml>
   ```

   Again, replace `<version>`, `<port>`, and `<config.yaml>` with the appropriate values.

4. **Configure Prometheus to Scrape Metrics**:
   - Add a new scrape configuration to your `prometheus.yml` file for the JMX Exporter:

   ```yaml
   scrape_configs:
     - job_name: 'jmx'
       static_configs:
         - targets: ['localhost:<port>']
   ```

   Replace `<port>` with the port number you specified when running the JMX Exporter.

5. **Start Prometheus**:
   - Run Prometheus with the updated configuration file, and it will start scraping metrics from the JMX Exporter.

6. **Visualize Metrics**:
   - Use Grafana or Prometheus's built-in expression browser to visualize the metrics collected from your JVMs.

Remember to secure your JMX Exporter endpoint, especially if it's exposed to the internet. You can use firewall rules, reverse proxies, or the JMX Exporter's built-in HTTP server's authentication and SSL capabilities to protect access to the metrics endpoint.



### Prometheus



> Prometheus can scrape JMX metrics exposed by each JVM and store them in a time-series database. Grafana can then be used to create dashboards that visualize these metrics.





### Micrometer

https://micrometer.io/

https://github.com/micrometer-metrics/micrometer

https://www.baeldung.com/micrometer

类似于 [SLF4J](https://en.wikipedia.org/wiki/SLF4J) 

### Dropwizard Metrics

https://metrics.dropwizard.io/4.2.0/

### Netdata

https://www.netdata.cloud/



### 参考

https://grafana.com/blog/2020/06/25/monitoring-java-applications-with-the-prometheus-jmx-exporter-and-grafana/