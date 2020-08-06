# Pointer and integer



理论上来说，在c family language中，使用pointer来保存address；理论上来说，address其实就是integer，所以我们自然而然地想到pointer和integer之间的关联，下面是一些样例。



## iso c++ forbids comparison between pointer and integer fpermissive

这个编译错误的原因是非常清晰的，即进行了pointer和integer的比较，下面的是样例程序。

### Example 1: 错误的使用`==`



```c++
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

这个例子源自 https://stackoverflow.com/a/4810668 ，上述程序编译报错如下：

```c++
test.cpp: 在函数‘int main()’中:
test.cpp:7:43: 错误：ISO C++ 不允许比较指针和整数的值 [-fpermissive]
  bool equal = &x[i] == &*(x + i) == (x + i);
```

问题原因在于对`==`的错误使用：

`&x[i] == &*(x + i) == (x + i)`等价于:

```c++
bool equal = &x[i] == &*(x + i);
equal = equal == (x + i);
```

显然，`equal == (x + i)`就触发了“比较指针和整数的值”，则导致上述编译报错。修改后的版本如下：

```c++
#include <iostream>

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



### Example 2: compare `char` with `const char *`



```c++
#include <iostream>
#include <string>
using namespace std;
bool accept()
{
	cout << "Do you want to proceed (y or n)?\n";
	char answer;
	cin >> answer;
	if (answer == "y")
	{
		return true;
	}
	return false;
}

int main()
{
	if (accept())
	{

	}
}

```

上述例子源自：[c++ compile error: ISO C++ forbids comparison between pointer and integer](https://stackoverflow.com/questions/2263681/c-compile-error-iso-c-forbids-comparison-between-pointer-and-integer)，上述程序编译报错如下：

```c++
test.cpp: 在函数‘bool accept()’中:
test.cpp:9:16: 错误：ISO C++ 不允许比较指针和整数的值 [-fpermissive]
  if (answer == "y")
```



