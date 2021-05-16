# Decorator pattern

文章:

1、`geeksforgeeks-The-Decorator-Pattern`

结合具体的案例，非常好

2、`wikipedia-Decorator-pattern`

比较冗杂，论述很多

## Composition over inheritance: decorator is an alternative to subclassing

一、最能够体现"decorator is an alternative to subclassing"的例子是: geeksforgeeks [Decorator Pattern | Set 1 (Background)](https://www.geeksforgeeks.org/decorator-pattern/) 、 geeksforgeeks [The Decorator Pattern | Set 2 (Introduction and Design)](https://www.geeksforgeeks.org/the-decorator-pattern-set-2-introduction-and-design/) 中给出的pizza + topping的例子: 

geeksforgeeks [Decorator Pattern | Set 1 (Background)](https://www.geeksforgeeks.org/decorator-pattern/) 中，使用subclassing ，并分析了它的disadvantage， geeksforgeeks [The Decorator Pattern | Set 2 (Introduction and Design)](https://www.geeksforgeeks.org/the-decorator-pattern-set-2-introduction-and-design/) 使用decorator来进行改进，可以看到，使用decorator非常灵活、扩展性非常好

二、在 wikipedia [Decorator pattern](https://en.wikipedia.org/wiki/Decorator_pattern) 中，对这个topic进行了总结。

## Use case

### 解决Combination explode、an alternative to subclassing

这是Decorator pattern能够很好的解决的问题，example:

1、`geeksforgeeks-The-Decorator-Pattern` 中的pizza+topping例子



### Pipeline multiple layer、流程、chain composition

其实这也是combination，不同的是它还添加了流程、multiple layer，example:

#### Protocol interceptor



1、apache [HttpClient Tutorial](http://hc.apache.org/httpcomponents-client-ga/tutorial/html/index.html) # 1.4. HTTP protocol interceptors

The HTTP protocol interceptor(拦截器) is a routine that implements a specific aspect of the **HTTP protocol**. Usually **protocol interceptors** are expected to act upon one specific header or a group of related headers of the incoming message, or populate the outgoing message with one specific header or a group of related headers. Protocol interceptors can also manipulate content entities enclosed with messages - transparent content compression / decompression being a good example. Usually this is accomplished by using the **'Decorator' pattern** where a **wrapper entity class** is used to decorate the original entity. Several protocol interceptors can be combined to form one logical unit.



2、wikipedia [Decorator pattern](https://en.wikipedia.org/wiki/Decorator_pattern) 中的 `class WebPage` 例子



## TODO

1、sourcemaking [Decorator Design Pattern](https://sourcemaking.com/design_patterns/decorator)

2、refactoring [Decorator](https://refactoring.guru/design-patterns/decorator)

