# Object-based resource management

本节标题的含义是: 基于object的resource management。

1、object是C++的data model、C++允许programmer对object lifetime进行control、zhihu、

2、RAII将object 和 resource进行bind，然后就可以基于object来对resource进行management，`std::share_ptr`、`std::smart_ptr`都是基于这个思路的: 

`std::share_ptr`: reference count 等于 live object 个数

`std::smart_ptr`: resource 和 object 直接绑定

3、C++中，一般不会直接使用一个resource，而是会将它bind到一个object

4、Object-based resource management的优势有:

a、strong exception safety: 不会resource leak

5、C++中很多resource相关的idiom都是基于"Object-based resource management"思想的