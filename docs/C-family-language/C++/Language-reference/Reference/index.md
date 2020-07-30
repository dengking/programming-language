# 关于本章

本章描述C++ reference。其中重点描述C++11的新特性：

- rvalue reference
- forward reference

本章，采用的是在`Theory\Programming-language\How-to-master-programming-language.md`的”What's new in the programming language“章节中描述的方法来学习这两个新特性：

rvalue reference用于move semantic；

forward reference用于perfect forwarding；

这两个特性，为C++语言带来了较大的改变。



本章先描述perfect forwarding，然后描述move semantic，因为在move semantic会涉及perfect forwarding中的内容，然后描述cppreference中，关于reference的描述；