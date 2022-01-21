# Address Of

## cppreference [C++ named requirements: *CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) # Notes

Until C++11, classes that overloaded `operator&` were not *CopyConstructible* and thus weren't usable in the standard library containers. As of C++11, the standard library uses [std::addressof](https://en.cppreference.com/w/cpp/memory/addressof) whenever the address of an object is needed.



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



## cppreference [std::addressof](https://en.cppreference.com/w/cpp/memory/addressof)



```C++

template<class T>
typename std::enable_if<std::is_object<T>::value, T*>::type  addressof(T& arg) noexcept
{
    return reinterpret_cast<T*>(
               &const_cast<char&>(
                   reinterpret_cast<const volatile char&>(arg)));
}
 
template<class T>
typename std::enable_if<!std::is_object<T>::value, T*>::type addressof(T& arg) noexcept
{
    return &arg;
}
```



### Example

```C++
#include <iostream>
#include <memory>

template<class T>
struct Ptr {
	T* pad; // add pad to show difference between 'this' and 'data'
	T* data;
	Ptr(T* arg) : pad(nullptr), data(arg)
	{
		std::cout << "Ctor this = " << this << std::endl;
	}

	~Ptr() { delete data; }
	T** operator&() { return &data; }
};

template<class T>
void f(Ptr<T>* p)
{
	std::cout << "Ptr   overload called with p = " << p << '\n';
}

void f(int** p)
{
	std::cout << "int** overload called with p = " << p << '\n';
}

int main()
{
	Ptr<int> p(new int(42));
	f(&p);                 // calls int** overload
	f(std::addressof(p));  // calls Ptr<int>* overload, (= this)
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -Werror

```

> NOTE: 
>
> 输出如下:
>
> ```C++
> Ctor this = 0x7ffe5e31c290
> 
> int** overload called with p = 0x7ffe5e31c298
> 
> Ptr   overload called with p = 0x7ffe5e31c290
> ```
>
> 