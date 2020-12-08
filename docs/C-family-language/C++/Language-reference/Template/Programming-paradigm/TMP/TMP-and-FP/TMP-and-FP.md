non-type template parameter的computation，主要以template parameter pack的computation、recursion为基础来进行总结。





#### Template metaprogramming and functional programming and non-type template parameter

上面这段话对于理解template metaprogramming是非常重要的：

template metaprogramming can be seen as a form of functional programming

在后面，我们会学习到template parameter和template argument，如果将template metaprogramming看做是一种functional programming的话，这个概念是非常容易理解的。

上面这段话中中特别提及了recursion，这就是典型的functional programming。

需要注意的是，一般使用non-type template parameter来实现compile-time execution，它可以看做是一种典型的functional programming，而其他的template parameter，如type template parameter、template template parameter，其实就不属于functional programming的概念了。

下面的`factorial`就是一个典型的例子；`factorial`的实现是典型的functional programming，使用`c++`的template metaprogramming可以实现compiler-time的functional programming。





## template recursion function





## TMP VS functional programming

C++: template and functional programming 

parse tree、recursion 

https://en.wikipedia.org/wiki/Expression_templates 

https://en.wikipedia.org/wiki/Spirit_Parser_Framework 

https://www.drdobbs.com/cpp/the-spirit-parser-library-inline-parsing/184401692 

这让我想到了：template is Turing-complete的



## recursion

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



上述例子是: 递归关系 + 终止条件