# 关于本章

## draft

OOP、behavior、interface、magic function

## draft: Behavior，magic function，给予或禁止某种behavior，valid expression

每种behavior都有对应的operator、magic function，language support function（`std::swap`），都可以使用**valid expression**来进行描述，这是典型的**behavior-based**，比如thread是movable，但是non-copyable

### Valid expression

可以使用Valid expression来描述某种behavior、运行，进而去:

1、runtime computation: 运行、计算，其实就是我们的program

2、compile type computation: 校验、reflection，这其实就是下面的"Concept/named requirement"



#### Concept/named requirement

1、concept也是使用valid expression来表达的，比如expression SFINAE， 另外还有constexpr，

2、`std::declval`就是用于构造expression、declval 方便地表达expression

3、Concept/named requirement其实就是compile-time expression，其实是compile-time computation

## Summary

> NOTE: 总结这样的table非常重要， 能够对C++ OOP有完整的认知

| concept/named requirement | expression | magic function |
| ------------------------- | ---------- | -------------- |
|                           |            |                |
|                           |            |                |
|                           |            |                |



## draft: Mixin 给予或删除

1、CRTP behavior mixin 给予或删除

2、non-copyable、strong type都是这个的例子

3、behavior、interface/magic function、able(在fluentcpp strong type的实现中，就非常多地使用"able"这个词语)

## draft

对于某种类型，可以对其给予、禁止某种behavior，这主要通过magic function来实现的。



## Example

1、fluentcpp strong type

基于behavior

2、non-copyable

3、Requiring-or-Prohibiting-Heap-based-Objects



