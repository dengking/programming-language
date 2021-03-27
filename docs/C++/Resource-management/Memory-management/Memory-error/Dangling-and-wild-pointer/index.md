# Dangling pointer

1、Dangling pointer是"Access outside of object lifetime"的一种，我将很多涉及到dangling pointer的内容都放到了`Access-outside-of-object-lifetime` 章节，便于统一讨论。

2、Dangling pointer是导致wild pointer的一个导火索。

3、dangling pointer and segment fault

access 一个 dangling pointer，就会导致segmentation fault



## wikipedia [Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer)	





## How to solve dangling pointer?

如何解决dangling pointer导致的问题？本节对这个topic进行总结:

1、Tombstone，参见 `Tombstone` 章节

2、Inversion of control，参见 `Access-outside-of-object-lifetime` 章节