# OOP VS Procedural programming

维基百科[Procedural programming](https://en.wikipedia.org/wiki/Procedural_programming)

Procedural programming的一种典型写法就是：

```
pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);
pthread_mutex_lock(&lock);
pthread_mutex_unlock(&lock);
pthread_mutex_destroy(&lock);
```

它的模式是：定义一个`struct`，然后定义各种对这个`struct`进行操作的函数。可以参见redis的实现。

OOP中，则完全不同，OOP中，整个程序是object之间的交互，object之间是通过发送message来进行交互的。