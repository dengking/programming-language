# Trailing return type

这是c++11引入的新特性，首先搞清楚它解决的是什么问题，然后再来学习它的具体的用法。



arne-mertz [Trailing return types everywhere](https://arne-mertz.de/2016/11/trailing-return-types-everywhere/)

ibm [Introduction to the C++11 feature: trailing return types](https://www.ibm.com/developerworks/community/blogs/5894415f-be62-4bc0-81c5-3956e82276f3/entry/introduction_to_the_c_11_feature_trailing_return_types?lang=en)

[Should the trailing return type syntax style become the default for new C++11 programs? ](https://stackoverflow.com/questions/11215227/should-the-trailing-return-type-syntax-style-become-the-default-for-new-c11-pr)



https://www.cprogramming.com/c++11/c++11-auto-decltype-return-value-after-function.html



## Trailing return type

### Examples

在[How can I output the value of an enum class in C++11](https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11)的[回答](https://stackoverflow.com/a/11421471)中给出的一个例子：

```c++
template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}


std::cout << as_integer(a) << std::endl;
```

