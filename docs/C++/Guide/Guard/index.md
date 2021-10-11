# Guard



## 将Scope Guard的思想推广

1、Scope Guard是C++中的established idiom，它描述的是当scope exit的时候，执行某个callback

2、那么我们能否将这个思想推广:

a、Exit guard: 当program exit的时候，执行某个callback

b、Crash guard: 当program crash的时候，执行某个call back

## Common issue

为例哪种guard，都需要考虑如下问题:

1、callback

2、如何判断是否需要执行callback



## Exit guard

在下面章节、文章、library中，涉及了这个topic:

1、`Static-local-object`

2、`Singleton-mixin`

3、chromium `at_exit`

## Crash guard

1、[macmade](https://github.com/macmade)/**[CrashGuard](https://github.com/macmade/CrashGuard)**



## System call

其实Linux system已经提供了实现类似概念的system call。