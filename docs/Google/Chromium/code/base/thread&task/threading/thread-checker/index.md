# thread checker

更加准确地说是: thread-affinity-checker。



## code 

1、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/[threading](https://github.com/chromium/chromium/tree/main/base/threading)/**[thread_checker.h](https://github.com/chromium/chromium/blob/main/base/threading/thread_checker.h)**

```
// ThreadChecker is a helper class used to help verify that some methods of a
// class are called from the same thread (for thread-affinity).  It supports
// thread safety annotations (see base/thread_annotations.h).
```

