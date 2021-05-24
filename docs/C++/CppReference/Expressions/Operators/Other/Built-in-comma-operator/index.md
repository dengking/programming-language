# Built-in comma operator

1、是在学习cppreference [Expression SFINAE](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE)的时候，发现的Built-in comma operator。

2、需要注意的是：[Built-in comma operator](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator)的left-hand argument是[Discarded-value expressions](https://en.cppreference.com/w/cpp/language/expressions#Discarded-value_expressions) 。

## cppreference [Built-in comma operator](https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator)



The comma in various comma-separated lists, such as function argument lists (`f(a, b, c)`) and initializer lists int `a[] = {1,2,3}`, is not the comma operator. If the comma operator needs to be used in such contexts, it has to be parenthesized: `f(a, (n++, n+b), c)`.

> NOTE: 从上面的描述来看，其实C++语言并没有对comma operator作出形式化的定义。
>
> 上段的最后一句话需要结合下面的例子来进行说明：
>
> ```c++
> #include <iostream>
> 
> void f(int a, int b, int c)
> {
> 	std::cout << "a:" << a << std::endl;
> 	std::cout << "b:" << b << std::endl;
> 	std::cout << "c:" << c << std::endl;
> }
> int main()
> {
> 	int n = 0;
> 	int a = 1;
> 	int b = 2;
> 	int c = 3;
> 	f(a, (n++, n + b), c);
> }
> // g++ test.cpp
> ```
>
> `(n++, n + b)`就是中的`,`就是comma operator ，上述程序所展示的是：comma operator的first argument会被evaluated。
>
> 上述程序的输出如下：
>
> ```c++
> a:1
> b:3
> c:3
> ```





