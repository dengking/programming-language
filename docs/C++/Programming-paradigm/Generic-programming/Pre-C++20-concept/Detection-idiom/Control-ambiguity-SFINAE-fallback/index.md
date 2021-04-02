# Control ambiguity SFINAE fallback

这是我在阅读 More C++ Idioms/[Member Detector](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector) 时，受其中的内容启发而总结的。

## Ambiguity in C++

1、overload

2、多个parent中，有同名的member

在 More C++ Idioms/[Member Detector](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector) 中，就是使用的这种方式。



## Control ambiguity SFINAE fallback

使用这种方式，可以实现detection idiom。