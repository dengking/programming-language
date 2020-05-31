# Enum



## cppreference [Enumeration declaration](https://en.cppreference.com/w/cpp/language/enum)

An *enumeration* is a distinct type whose value is restricted to a range of values (see below for details), which may include several explicitly named constants ("*enumerators*"). The values of the constants are values of an integral type known as the *underlying type* of the enumeration.

> NOTE: *underlying type*的概念是非常重要的，如何获取它呢？参见：[std::underlying_type](https://en.cppreference.com/w/cpp/types/underlying_type)

## Enum Classes VS Enum

https://www.geeksforgeeks.org/enum-classes-in-c-and-their-advantage-over-enum-datatype/

## Enum and string

### 输出enum

在cppreference [Enumeration declaration](https://en.cppreference.com/w/cpp/language/enum)的[Example](https://en.cppreference.com/w/cpp/language/enum#Example)中给出的例子非常好。

### [How can I output the value of an enum class in C++11](https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11)

这个问题的回答中给出的code，值的借鉴：

```c++
template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
```



### enum to string

#### [enum to string in modern C++11 / C++14 / C++17 and future C++20](https://stackoverflow.com/questions/28828957/enum-to-string-in-modern-c11-c14-c17-and-future-c20)



#### [enum class of type string in C++](https://stackoverflow.com/questions/43510615/enum-class-of-type-string-in-c)



#### [magic_enum](https://github.com/Neargye/magic_enum)



## underlying type of enum

[What is the underlying type of a c++ enum?](https://stackoverflow.com/questions/1122096/what-is-the-underlying-type-of-a-c-enum)