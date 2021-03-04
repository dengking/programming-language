# Address Of



## More C++ Idioms/[Address Of](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Address_Of)

> NOTE: `addressof`可以看做是一个专门的用于获取object address的operator。获取object address不再使用`&`，而是使用`addressof`。

### Intent

Find address of an object of a class that has an overloaded unary ampersand (`&`) operator.

### Motivation

C++ allows overloading of unary ampersand (`&`) operator for class types. The return type of such an operator need not be the actual address of the object. Intentions(意图) of such a class are highly debatable but the language allows it nevertheless. Address-of idiom is a way to find the real address of an object irrespective(不考虑) of the overloaded unary ampersand（符号） operator and its access protection.

> NOTE: 如果class已经overload了`&`运算符，并且这个overload `&`不是获取地址的，那么此时就需要使用`addressof` idiom。

In the example below, the `main` function fails to compile because `operator &` of `nonaddressable` class is private. Even if it were accessible, a conversion from its return type `double` to a pointer would not have been possible or meaningful.

```c++
class nonaddressable 
{
public:
    typedef double useless_type;
private:
    useless_type operator&() const;
};

int main()
{
  nonaddressable na;
  nonaddressable * naptr = &na; // Compiler error here.
}
```

### Solution and Sample Code

The Address-of idiom retrieves the address of an object using a series of casts.

```c++
#include<iostream>
class nonaddressable 
{
public:
    typedef double useless_type;
private:
    useless_type operator&() const;
};

template <class T>
T * addressof(T & v)
{
  return reinterpret_cast<T *>(& const_cast<char&>(reinterpret_cast<const volatile char &>(v)));
}
int main()
{
  nonaddressable na;
  nonaddressable * naptr = addressof(na); // No more compiler error.
}
```

> NOTE: 上述实现，采用的是interpretation model、aliasing、serialization的思路，它是实现是考虑的了较多问题:
>
> 1、`T`可能是CV的，它的最内层`reinterpret_cast<const volatile char &>(v)`
>
> 2、第二层使用`const_cast<char&>`去除CV，这就得到了binary representation
>
> 3、`& const_cast<char&>` 用于取binary representation的地址
>
> 4、`reinterpret_cast<T *>`实现aliasing

#### C++11

In C++11 the template [std::addressof](http://en.cppreference.com/w/cpp/memory/addressof), in the `<memory>` header, was added to solve this problem. In C++17, the template is also `constexpr`.