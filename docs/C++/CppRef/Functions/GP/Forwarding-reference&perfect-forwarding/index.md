# C++11 forwarding reference and perfect forwarding

一、C++11 forwarding reference是function generic programming的基础:

1、`auto&&`解决的是函数的返回值

2、`template &&`解决的函数的参数

后面会对它们进行专门介绍。

二、forwarding是和value category相关的。

## Perfect forwarding

Perfect forwarding = reference collapsing rule( `std::forward` 的实现有赖于此) + special type deduction rules for rvalue references

