# 关于本章

linkage是link-time概念，它是name的属性（name可能是object，所以object也具有linkage的概念）。

storage class是run-time概念，它是object的属性。

c++和c的对linkage和storage class密切相关，两种语言都将两者放到了一起来进行描述：



linkage是link-time概念，在cppreference中，将它和storage class放到了一起描述：

- c [Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)
- c++ [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)