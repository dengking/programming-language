# Pointer conversion

## Implicit conversions/decay

在C-family language中，存在着与pointer有关的两个非常重要的implicit conversions：

- [array to pointer conversion](https://en.cppreference.com/w/cpp/language/implicit_conversion#Array_to_pointer_conversion)
- [function to pointer conversion](https://en.cppreference.com/w/cpp/language/implicit_conversion#Function_to_pointer)

## Explicit cast

在C++中，使用`reinterpret_cast` operator来实现pointer cast。

两中cast都涉及如下问题：

1、alignment

2、strict aliasing

