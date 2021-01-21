# The C++ detection idiom

在阅读wikipedia [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) 时，其中有这样的描述:

With the standardisation of the detection idiom in the [Library fundamental v2 (n4562)](http://en.cppreference.com/w/cpp/experimental/lib_extensions_2) proposal, the above code could be re-written as follows:

```C++
#include <iostream>
#include <type_traits>

template<typename T>
using has_typedef_foobar_t = typename T::foobar;

struct foo
{
	using foobar = float;
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << std::is_detected<has_typedef_foobar_t, int>::value << std::endl;
	std::cout << std::is_detected<has_typedef_foobar_t, foo>::value << std::endl;
}

```

显然，C++ standard library试图提供detection idiom的标准版。

## cppreference [The C++ detection idiom](https://en.cppreference.com/w/cpp/experimental/lib_extensions_2#The_C++_detection_idiom)



## riptutorial [C++ is_detected](https://riptutorial.com/cplusplus/example/18585/is-detected)

> NOTE: 这篇文章给出了实现的源代码，可以借鉴。