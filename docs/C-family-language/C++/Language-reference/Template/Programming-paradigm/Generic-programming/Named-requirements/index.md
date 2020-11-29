# Named requirements

前面已经对named requirements进行了详细介绍。

## Named requirement is behavior-based

其实本节标题的"Named requirement is behavior based"是和"Template-is-behavior-based"章节的观点是重复的，我在此处添加的目的仅仅是为了提示。

可以看到cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)中描述的各种type requirement的Requirements要求object of the type能够满足各种**Expression**，这其实就是behavior-based。

## cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)

Some of these requirements are being formalized in C++20 using the [concepts](https://en.cppreference.com/w/cpp/language/constraints) language feature. Until then, the burden is on the programmer to ensure that library templates are instantiated with template arguments that satisfy these requirements. Failure to do so may result in very complex compiler diagnostics.

> NOTE: "Failure to do so may result in very complex compiler diagnostics"这是C++20 concept着力解决的问题之一。

### Library-wide

| concept                                                      |      | chapter                                                      |
| ------------------------------------------------------------ | ---- | ------------------------------------------------------------ |
| [Swappable](https://en.cppreference.com/w/cpp/named_req/Swappable) (C++11) |      | `C-family-language\C++\Idiom\OOP\Resource-management\Non-throwing-swap` |
|                                                              |      |                                                              |

