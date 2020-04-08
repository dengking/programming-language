# Function



## cppreference [Functions](https://en.cppreference.com/w/cpp/language/functions)



## cppreference [Function declaration](https://en.cppreference.com/w/cpp/language/function)





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

