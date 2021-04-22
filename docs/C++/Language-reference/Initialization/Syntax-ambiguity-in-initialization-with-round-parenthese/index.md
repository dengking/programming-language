# Syntax ambiguity in initialization with round parenthese

## Direct initialization、Value initialization 与 function declaration的歧义

我们知道Direct initialization、Value initialization的syntax是非常类似的，区别在于 Direct initialization 带参而后者不带参，可以看到，它们的syntax 和 function declaration是非常类似的，因此，就导致了歧义。

1、cppreference [Direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) 中，给出的如下syntax

```C++
T object ( arg );
T object ( arg1, arg2, ... );
```

可以看到，它和function declaration是非常类似的。



### cppreference [Direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization)

In case of ambiguity between a variable declaration using the direct-initialization syntax (1) (with round parentheses) and a [function declaration](https://en.cppreference.com/w/cpp/language/function), the compiler always chooses function declaration. This disambiguation rule is sometimes counter-intuitive and has been called the [most vexing parse](https://en.wikipedia.org/wiki/Most_vexing_parse).

```C++
#include <iterator>
#include <string>
#include <fstream>
int main()
{
	std::ifstream file("data.txt");
	// the following is a function declaration:
	std::string str(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	// it declares a function called str, whose return type is std::string,
	// first parameter has type std::istreambuf_iterator<char> and the name "file"
	// second parameter has no name and has type std::istreambuf_iterator<char>(),
	// which is rewritten to function pointer type std::istreambuf_iterator<char>(*)()

	// pre-c++11 fix: extra parentheses around one of the arguments
	std::string str1((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	// post-C++11 fix: list-initialization for any of the arguments
	std::string str2(std::istreambuf_iterator<char> { file }, { });
}
// g++ test.cpp -pedantic -Wall -Wextra

```

> NOTE: 
>
> 在 "g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-28)" 中，上述code是能够正常编译通过的值，只有如下警告:
>
> ```C++
> test.cpp: 在函数‘int main()’中:
> test.cpp:17:24: 警告：extended initializer lists only available with -std=c++11 or -std=gnu++11 [默认启用]
>   std::string str2(std::istreambuf_iterator<char> { file }, { });
>                         ^
> test.cpp:17:58: 警告：extended initializer lists only available with -std=c++11 or -std=gnu++11 [默认启用]
>   std::string str2(std::istreambuf_iterator<char> { file }, { });
> 
> ```
>
> 

Similarly, in the case of an ambiguity between a expression statement with a function-style cast expression (3) as its leftmost subexpression and a declaration statement, the ambiguity is resolved by treating it as a declaration. This disambiguation is purely syntactic: it doesn't consider the meaning of names occurring in the statement other than whether they are type names.

> NOTE: 
>
> 1、上面这段话没有说清楚"declaration statement"到底是什么？下面的例子，我也没有明白`L(n);`的含义。

```C++
struct M
{
};
struct L
{
	L(M&){}
};

M n;
void f()
{
	M (m); // declaration, equivalent to M m;
	L(n); // ill-formed declaration
	L (l)(m); // still a declaration
}
int main()
{

}
// g++ test.cpp -pedantic -Wall -Wextra

```

> NOTE: 
>
> 编译报错如下 :
>
> ```C++
> test.cpp: 在函数‘void f()’中:
> test.cpp:13:5: 错误：对‘L::L()’的调用没有匹配的函数
>   L(n); // ill-formed declaration
> 
> ```
>
> 下面是正确的写法:
>
> ```C++
> struct M
> {
> };
> struct L
> {
> 	L(M&)
> 	{
> 	}
> };
> 
> M n;
> void f()
> {
> 	M (m); // declaration, equivalent to M m;
> 	L { n }; 
> 	L (l)(m); // still a declaration
> }
> int main()
> {
> 
> }
> // g++ test.cpp -pedantic -Wall -Wextra --std=c++11
> ```
>
> 





### cppreference [Value initialization (since C++03)](https://en.cppreference.com/w/cpp/language/value_initialization)

The syntax `T object();` does not initialize an object; it declares a function that takes no arguments and returns `T`. The way to value-initialize a named variable before C++11 was `T object = T();`, which value-initializes a temporary and then copy-initializes the object: most compilers [optimize out the copy](https://en.cppreference.com/w/cpp/language/copy_elision) in this case.