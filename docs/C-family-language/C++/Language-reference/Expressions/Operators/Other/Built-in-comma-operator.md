# Built-in comma operator

是在学习cppreference [Expression SFINAE](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE)的时候，发现的Built-in comma operator。

需要注意的是：[Built-in comma operator](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator)的left-hand argument是[Discarded-value expressions](https://en.cppreference.com/w/cpp/language/expressions#Discarded-value_expressions)

## cppreference [Built-in comma operator](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator)



## Examples

### Use expression SFINAE on the return type

### cppreference [Expression SFINAE](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE)

其中给出的例子非常具有代表性。

#### [What is decltype with two arguments?](https://stackoverflow.com/questions/16044514/what-is-decltype-with-two-arguments)



[A](https://stackoverflow.com/a/16044577)

```c++
#include <type_traits>

int main()
{
	static_assert(std::is_same<decltype(42, 3.14), double>::value, "Will not fire");
}

```

