# Resource management 

process在其生命周期内会占用一定的system resource，所以它就涉及resource management即如何来管理它所占有的system resource。对于programmer而言，最最常见的resource management包括：

- memory
- file

关于process可能占用的resource，参见工程[Linux-OS](https://dengking.github.io/Linux-OS/)。

## 维基百科[Resource management (computing)](https://en.wikipedia.org/wiki/Resource_management_(computing))



## Implementation of resource management

不同的programming language的resource management策略是不同的。

不同的resource的management的策略也是不同的，正如在维基百科[Garbage collection (computer science)](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))中 所总结的：

> Resources other than memory, such as [network sockets](https://en.wikipedia.org/wiki/Network_socket), database [handles](https://en.wikipedia.org/wiki/Handle_(computing)), [user interaction windows](https://en.wikipedia.org/wiki/Window_(computing)), [file](https://en.wikipedia.org/wiki/File_(computing)) and device descriptors, are not typically handled by garbage collection.

不同语言的resource management策略也是不同的。