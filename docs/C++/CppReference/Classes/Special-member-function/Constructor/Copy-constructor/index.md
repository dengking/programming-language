# Copy constructor



## cppreference [Copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor)



## [LeetBook C++ 面试突击](https://leetcode-cn.com/leetbook/detail/cpp-interview-highlights/) # [为什么拷贝构造函数必须为引用？](https://leetcode-cn.com/leetbook/read/cpp-interview-highlights/efiakm/)

原因：避免拷贝构造函数无限制的递归，最终导致栈溢出。

> NOTE: 
>
> pass-by-value-copy

