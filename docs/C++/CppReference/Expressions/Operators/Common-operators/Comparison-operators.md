# Comparison operators



## cppreference [Comparison operators](https://en.cppreference.com/w/cpp/language/operator_comparison)

> NOTE: 
>
> - comparison operators的return type是`bool`
> - comparison operators 不支持chain

## Examples

### Example 1: 错误使用chain `==`

在 [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c)的[answer](https://stackoverflow.com/a/4810668)的“Indexing operator"章节中给出了一个例子：

```cpp
#include <iostream>

int main()
{
	int x[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int i = 3;
	bool equal = &x[i] == &*(x + i) == (x + i);
	std::cout << std::boolalpha;
	std::cout << equal << std::endl;
}
// g++ --std=c++11 test.cpp

```

NOTE: 上诉程序编译是会报错的：

```c++
test.cpp: 在函数‘int main()’中:
test.cpp:7:43: 错误：ISO C++ 不允许比较指针和整数的值 [-fpermissive]
bool equal = &x[i] == &*(x + i) == (x + i);
```

问题原因在于对`==`的错误使用，`&x[i] == &*(x + i) == (x + i)`等价于:

```c++
bool equal = &x[i] == &*(x + i);
equal = equal == (x + i);
```

显然，`equal == (x + i);`就触发了“比较指针和整数的值”，则导致上述编译报错。修改后的版本如下：

```c++
#include <iostream

int main()
{
	int x[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int i = 3;
	bool equal = &x[i] == &*(x + i) && &*(x + i) == (x + i);
	std::cout << std::boolalpha;
	std::cout << equal << std::endl;
}
// g++ --std=c++11 test.cpp

```

输出:

```c++
true
```

