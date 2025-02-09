# JVM Log & Analyzer

Q: 如何开启GC日志?

[nipafx Unified Logging Of JVM Messages With The `-Xlog` Option](https://nipafx.dev/java-unified-logging-xlog/) 

[Java](https://docs.oracle.com/en/java/index.html) / [Java SE](https://docs.oracle.com/en/java/javase/index.html) / [Tools Reference](https://docs.oracle.com/en/java/javase/11/tools/) / [java](https://docs.oracle.com/en/java/javase/11/tools/java.html) / [Enable Logging with the JVM Unified Logging Framework](https://docs.oracle.com/en/java/javase/11/tools/java.html#GUID-BE93ABDC-999C-4CB5-A88B-1994AAAC74D5) 



## [JEP 158: Unified JVM Logging](https://openjdk.org/jeps/158)

### Summary

Introduce a common logging system for all components of the JVM.

### Goals

- Common command-line options for all logging
- Log messages are categorized using **tags** (e.g. compiler, gc, classload, metaspace, svc, jfr, ...). One message can have multiple tags (*tag-set*)
- Logging is performed at different levels: `error, warning, info, debug, trace, develop`.
- Possible to select what messages that are logged based on level.
- Possible to redirect logging to console or file.
- The default configuration is that all messages using warning and error level are output to stderr.
- File rotation of log files by size and number of files to keep (similar to what is available for GC logs today)
- Print line-at-a-time (no interleaving within same line)
- Logging messages are in human-readable plain text
- Messages can be "decorated". The default decorations are: **`uptime, level, tags`**.
- Ability to configure **which** decorations that will be printed.
- Existing '`tty->print...`' logging should use unified logging as output
- Logging can be configured **dynamically** at runtime via **jcmd** or **MBeans**
- Tested and supported -- should not crash if/when enabled by user/customer

Stretch goals:

- Multi-line logging: several lines can be logged in a way that keeps them together (non interleaved) when output
- Enable/disable individual log messages (for example by using `__FILE__` / `__LINE__`)
- Implement syslog and Windows Event Viewer output
- Ability to configure in **which order** decorations should be printed
