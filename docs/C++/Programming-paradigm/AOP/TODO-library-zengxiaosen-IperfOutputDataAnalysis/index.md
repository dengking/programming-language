# [zengxiaosen](https://github.com/zengxiaosen)/**[IperfOutputDataAnalysis](https://github.com/zengxiaosen/IperfOutputDataAnalysis)**

4,9之后，使用ioc和基于动态织入的轻量级aop，使用AOP还有一种情况是为了提高程序的可维护性，将程序的非核心逻辑都“横切”出来，将非核心逻辑和核心逻辑分离，比如日志，认证等 (100%); 4,10之后，使用log线程读取线程池中任务线程产生的日志数据，目前采取的策略是存取logData类里面static静态成员变量。（100%） 结构如下: 分发任务线程->同步队列->线程池中异步任务线程 <--通信--> 收集log日志线程 ->分析