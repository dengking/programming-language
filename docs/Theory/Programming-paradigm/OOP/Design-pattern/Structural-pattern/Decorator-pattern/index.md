# Decorator pattern

文章:

1、`geeksforgeeks-The-Decorator-Pattern`

结合具体的案例，非常好

2、`wikipedia-Decorator-pattern`

比较冗杂，论述很多

## Use case

### Combination explode

这是Decorator pattern能够很好的解决的问题，example:

1、`geeksforgeeks-The-Decorator-Pattern` 中的pizza+topping例子



### Pipeline multiple layer、流程

其实这也是combination，不同的是它还添加了流程、multiple layer，example:

1、apache [HttpClient Tutorial](http://hc.apache.org/httpcomponents-client-ga/tutorial/html/index.html) # 1.4. HTTP protocol interceptors

> The HTTP protocol interceptor(拦截器) is a routine that implements a specific aspect of the HTTP protocol. Usually **protocol interceptors** are expected to act upon one specific header or a group of related headers of the incoming message, or populate the outgoing message with one specific header or a group of related headers. Protocol interceptors can also manipulate content entities enclosed with messages - transparent content compression / decompression being a good example. Usually this is accomplished by using the **'Decorator' pattern** where a **wrapper entity class** is used to decorate the original entity. Several protocol interceptors can be combined to form one logical unit.

2、wikipedia [Decorator pattern](https://en.wikipedia.org/wiki/Decorator_pattern) 中的 `class WebPage` 例子



## TODO

1、sourcemaking [Decorator Design Pattern](https://sourcemaking.com/design_patterns/decorator)

2、refactoring [Decorator](https://refactoring.guru/design-patterns/decorator)

