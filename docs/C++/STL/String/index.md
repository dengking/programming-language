# 关于本章

本章对C family language中的string进行讨论。

## C++ string is a wrapper of C string

C++ string 包括:

1) `std::string`

2) `std::string_view`

其实都是对C string的wrapper，关于wrapper，参见`Wrapper`。



## char、string、`string_view`、`char_traits`

1、C++ string是非常灵活的，可以看到，它基本上是根据它的char type进行分类

2、下面表格是根据cppreference [Strings library](https://en.cppreference.com/w/cpp/string) 绘制的

3、需要梳理清楚char、string之间的对应关系

| char type  | `char_traits`                             | string                                                       |                                                              | `string_view`                                                |
| ---------- | ----------------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `char`     | `template<> class char_traits<char>;`     | [std::string](https://en.cppreference.com/w/cpp/string/basic_string) | [std::basic_string](http://en.cppreference.com/w/cpp/string/basic_string)`<char>` | [std::string_view](https://en.cppreference.com/w/cpp/string/basic_string_view) (since C++17) |
| `wchar_t`  | `template<> class char_traits<wchar_t>;`  | [std::wstring](https://en.cppreference.com/w/cpp/string/basic_string) | [std::basic_string](http://en.cppreference.com/w/cpp/string/basic_string)<wchar_t> | [std::wstring_view](https://en.cppreference.com/w/cpp/string/basic_string_view) (since C++17) |
| `char8_t`  | `template<> class char_traits<char8_t>;`  | [std::u8string](https://en.cppreference.com/w/cpp/string/basic_string) (since C++20) | [std::basic_string](http://en.cppreference.com/w/cpp/string/basic_string)<char8_t> | [std::u8string_view](https://en.cppreference.com/w/cpp/string/basic_string_view) (since C++20) |
| `char16_t` | `template<> class char_traits<char16_t>;` | [std::u16string](https://en.cppreference.com/w/cpp/string/basic_string) (since C++11) | [std::basic_string](http://en.cppreference.com/w/cpp/string/basic_string)<char16_t> | [std::u16string_view](https://en.cppreference.com/w/cpp/string/basic_string_view) (since C++17) |
| `char32_t` | `template<> class char_traits<char32_t>;` | [std::u32string](https://en.cppreference.com/w/cpp/string/basic_string) (since C++11) | [std::basic_string](http://en.cppreference.com/w/cpp/string/basic_string)<char32_t> | [std::u32string_view](https://en.cppreference.com/w/cpp/string/basic_string_view) (since C++17) |



