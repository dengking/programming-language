# Profile Java



## Memory profiling in Java

https://medium.com/@fullstacktips/memory-profiling-in-java-1b12ab099864

stackoverflow [How to memory profile in Java?](https://stackoverflow.com/questions/10108942/how-to-memory-profile-in-java) # [A](https://stackoverflow.com/a/10163105/23877800) : [JProfiler](http://www.ej-technologies.com/products/jprofiler/overview.html) 

oracle [Java VisualVM - Profiling Applications](https://docs.oracle.com/javase/8/docs/technotes/guides/visualvm/profiler.html)

how to get the peak memory usage of java application?

vogella [Java Performance - Memory and Runtime Analysis - Tutorial](https://www.vogella.com/tutorials/JavaPerformance/article.html)







## async-profiler

https://github.com/async-profiler



## Java Performance - The Definitive Guide

https://www.amazon.com/Java-Performance-Definitive-Guide-Getting/dp/1449358454

https://github.com/leemuncon/book-1/blob/master/%5BJAVA%5D%5BJava%20Performance%20-%20The%20Definitive%20Guide%5D.pdf

https://book.douban.com/subject/26740520/





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



### How to get jvm info in java

### java.lang.management package

https://javadoc.scijava.org/Java6/java/lang/management/package-use.html

https://docs.oracle.com/javase/8/docs/api/java/lang/management/package-summary.html



### skywalking [JVM Metrics APIs](https://skywalking.apache.org/docs/main/v9.7.0/en/api/jvm-protocol/)

https://www.cnblogs.com/jiefang1874/p/16613800.html

https://skywalking.apache.org/docs/main/v9.7.0/en/api/jvm-protocol/ 



https://github.com/apache/skywalking/issues/6639





### zabbix jvm monitoring

https://www.zabbix.com/documentation/current/en/manual/config/items/itemtypes/jmx_monitoring

https://www.zabbix.com/integrations/java_monitoring

https://www.zabbix.com/integrations/jmx



### How to monitor full GC event in jvm



Monitoring Full GC events in the JVM is crucial for understanding and optimizing the performance of Java applications. Full GC events are when the garbage collector reclaims memory from all parts of the heap, which can cause significant pauses in application execution. Here's how you can monitor Full GC events:

1. **Using JVM Flags for GC Logging**:
   Enable GC logging by adding JVM flags when starting your Java application. For Java 8 and earlier, you can use:

   ```
   -XX:+PrintGCDetails -XX:+PrintGCDateStamps -Xloggc:<path-to-gc-log-file>
   ```

   For Java 9 and later, the logging system has been changed to Unified JVM Logging, and you can use:

   ```
   -Xlog:gc*:file=<path-to-gc-log-file>:time,level,tags
   ```

   These flags will create a log file with detailed information about each GC event, including Full GCs.

2. **Analyzing GC Logs**:
   Use tools like GCViewer, GCEasy, or HPJmeter to analyze the GC log files generated by the JVM. These tools can help you visualize GC events and identify Full GC occurrences.

3. **Using JMX and MBeans**:
   Connect to the JVM using JConsole or VisualVM and monitor the `java.lang:type=GarbageCollector` MBeans. These MBeans provide attributes like `CollectionCount` and `CollectionTime` for each collector, including those that perform Full GCs (e.g., `MarkSweepCompact` for CMS or `G1 Old Generation` for G1).

4. **Programmatic Monitoring**:
   Write a Java program that uses the `GarbageCollectorMXBean` from the `java.lang.management` package to monitor GC events programmatically. You can set up a notification listener to react to GC notifications.

5. **Using Prometheus and Grafana**:
   If you have Prometheus set up, you can use the JMX Exporter to expose GC metrics to Prometheus. Then, visualize and set up alerts using Grafana dashboards.

6. **Using Java Flight Recorder (JFR)**:
   Start a JFR recording session to capture detailed runtime information, including GC events. Analyze the recording using JDK Mission Control (JMC) to inspect Full GC events.

7. **Third-Party APM Tools**:
   Use Application Performance Management (APM) tools like New Relic, AppDynamics, or Dynatrace, which provide built-in JVM monitoring capabilities, including GC event tracking.

8. **Using jstat Tool**:
   The `jstat` command-line utility can be used to monitor the GC statistics in real-time. The `-gc` option provides information about the behavior of the garbage collected heap.

9. **Alerting**:
   Set up alerting mechanisms using tools like ElastAlert with Elasticsearch or Alertmanager with Prometheus to notify you when Full GC events occur or exceed certain thresholds.

Monitoring Full GC events is essential for maintaining the health and performance of Java applications. By using the appropriate tools and techniques, you can gain insights into the GC behavior and take action to optimize memory management and reduce application pauses.

