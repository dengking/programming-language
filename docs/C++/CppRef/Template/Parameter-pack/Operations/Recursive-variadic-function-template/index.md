# Recursive variadic function template

## FP recursion

### Example

```C++
#include <iostream>
 
void tprintf(const char* format) // base function
{
    std::cout << format;
}
 
template<typename T, typename... Targs>
void tprintf(const char* format, T value, Targs... Fargs) // recursive variadic function
{
    for ( ; *format != '\0'; format++ ) {
        if ( *format == '%' ) {
           std::cout << value;
           tprintf(format+1, Fargs...); // recursive call
           return;
        }
        std::cout << *format;
    }
}
 
int main()
{
    tprintf("% world% %\n","Hello",'!',123);
    return 0;
}
```



上述例子是: 递归关系 + 终止条件(base case)



## TODO

1、需要补充案例

2、eli.thegreenplace [Variadic templates in C++](https://eli.thegreenplace.net/2014/variadic-templates-in-c/)