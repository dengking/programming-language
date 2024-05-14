# Java GC



## Generational GC

Young Generation-Minor GC

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