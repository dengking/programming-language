# 为什么move可以是non-throw

1、简单来说，它无需system call、dynamic allocation，对于resource wrapper，应该实行non-throw swap



## stackoverflow [Why implementing swap() as non-throwing](https://stackoverflow.com/questions/44042043/why-implementing-swap-as-non-throwing)



## blog.ycshao.com [Effective C++ item 25: Consider Support For a Non-throwing swap](https://blog.ycshao.com/2017/10/22/effective-c-item-25-consider-support-for-a-non-throwing-swap/)

