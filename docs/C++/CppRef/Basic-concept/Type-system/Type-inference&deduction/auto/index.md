# auto



## cppreference [C++ keywords: auto](https://en.cppreference.com/w/cpp/keyword/auto)

| 版本        | Usage                                                        |      |
| ----------- | ------------------------------------------------------------ | ---- |
| until C++11 | [automatic storage duration specifier](https://en.cppreference.com/w/cpp/language/storage_duration) |      |
| since C++11 | [`auto` placeholder type specifier](https://en.cppreference.com/w/cpp/language/auto) |      |
|             | [function declaration](https://en.cppreference.com/w/cpp/language/function) with trailing return type |      |
| since C++17 | [structured binding declaration](https://en.cppreference.com/w/cpp/language/structured_binding) |      |



## `auto` and polymorphism

由于C++ auto 是由compiler执行的，因此，它使用的是static type，因此它是无法支持多态行为的，关于这一点，是我在阅读 [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) # [C.67: A polymorphic class should suppress public copy/move](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) 时，其中的一个这样的例子:


```C++
class B
{ // BAD: polymorphic base class doesn't suppress copying
public:
    virtual char m() { return 'B'; }
    // ... nothing about copy operations, so uses default ...
};

class D : public B
{
public:
    char m() override { return 'D'; }
    // ...
};

void f(B &b)
{
    auto b2 = b; // oops, slices the object; b2.m() will return 'B'
}

int main()
{
    D d;
    f(d);
}

```



# see also

1、microsoft [`auto` (C++)](https://docs.microsoft.com/en-us/cpp/cpp/auto-cpp?view=msvc-160)

