# Linkage and ORD and header only 

这是接着昨天的关于`const and linkage`而来的，昨天讨论了`const` variable的linkage，显然在一个头文件中的定义的`const` variable，是可以被多态其他的translation unit所引用的，这让我想到了类似的问题：header only class，显然在一个header file中定义了一个class，它可以被多个其他的translation unit所引用，那这是否表明class是internal linkage。

Google：c++ header only class and ODR，下面是比较好的解释：

- [Class definitions in headers and ODR?](https://stackoverflow.com/questions/45900040/class-definitions-in-headers-and-odr)

维基百科[Header-only](https://en.wikipedia.org/wiki/Header-only)
维基百科[One Definition Rule](https://en.wikipedia.org/wiki/One_Definition_Rule)
ORD和linkage之间的关联？linkage是否