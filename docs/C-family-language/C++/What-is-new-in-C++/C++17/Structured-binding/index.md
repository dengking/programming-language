# Structured binding

## 类比Python assignment

在`Comparison-of-programming-language\Cpp-VS-Python`中对两者进行了类比，只能够帮助我们理解这个特性。



## codeproject [C++ 17 New Features and Trick](https://www.codeproject.com/Articles/5262072/Cplusplus-17-New-Features-and-Trick) # [Structure Binding](https://www.codeproject.com/Articles/5262072/Cplusplus-17-New-Features-and-Trick#Structure%20binding)

Multiple return values from functions are not a new concept in programming and similar functionality is present in many other programming languages. C++17 comes with a new feature (structured bindings) that provides functionality similar to the multiple return values provided in other languages.

In the following example, I want to provide an overview of some of the options that we have in the old C++ standard, in the modern standard (C++11/14) and today in C++17 to return multiple values from functions:

```C++
#include <iostream>
#include <tuple> // std::tie
#include <math.h>       /* sin */

const double PI = 3.14159265;

void calculateSinCos(const double param, double &resSin, double &resCos)
{
	resSin = sin(param * PI / 180.0); // converting degrees to radians
	resCos = cos(param * PI / 180.0); // converting degrees to radians
}

std::pair<double, double> calculateSinCos(const double param)
{
	return
	{	sin(param * PI / 180.0), cos(param * PI / 180.0)};
}

std::tuple<double, double> calculateSinCos_Tuple(const double param)
{
	return std::make_tuple(sin(param * PI / 180.0), cos(param * PI / 180.0));  // packing values into tuple
}

int main()
{
	double param { 90.0 };
	double resultSin { 0.0 };
	double resultCos { 0.0 };

	//C++98
	calculateSinCos(param, resultSin, resultCos);
	std::cout << "C++98 : sin(" << param << ") = " << resultSin << ", cos(" << param << ") = " << resultCos << "\n";

	//C++11
	const auto resSinCos(calculateSinCos(param));
	std::cout << "C++11 : sin(" << param << ") = " << resSinCos.first << ", cos(" << param << ") = " << resSinCos.second << "\n";

	//C++11
	std::tie(resultSin, resultCos) = calculateSinCos(param);
	std::cout << "C++11 : sin(" << param << ") = " << resultSin << ", cos(" << param << ") = " << resultCos << "\n";

	// C++17
	auto [a, b] = calculateSinCos(param);
	std::cout << "C++17 :sin(" << param << ") = " << a << ", cos(" << param << ") = " << b << "\n";

	// C++17
	auto [x, y] = calculateSinCos_Tuple(param);
	// std::make_tuple(sin(val), cos(val));
	std::cout << "C++17 : sin(" << param << ") = " << x << ", cos(" << param << ") = " << y << "\n";
}

```

Alternatively, before C++17, it would be possible to use `std::tie` to unpack a tuple/pair to achieve a similar effect:

```C++
std::tie(resultSin, resultCos) = calculateSinCos(param);
```

This approach demonstrates how to **unpack** the resulting pair into two variables. Notice, this example shows all the power of `std::tie`, but nonetheless, the `std::tie` is less powerful than **structured bindings**, because we must first define all the variables we want to bind.

Structured binding is a new functionality of C++17, making the code even more readable, expressive and concise.

```C++
auto[a, b] = calculateSinCos(param);
```

Notice, the variables `a` and `b` are not references; they are aliases (or bindings) to the generated object member variables. The compiler assigns a unique name to the temporary object.

In C++11, the `std::tuple` container has been added to build a tuple that contains multiple return values. But neither C++11 nor C++14 does support an easy way to get elements in a `std::tuple` directly from the tuple (Of course, we can unpack a tuple using  `std::tie`, but we still need to understand the type of each object and how many objects are this tuple. Phew, how painful it is...)

C++17 fixes this flaw, and the **structured bindings** allow us to write code as follows:

```c++
auto[x, y] = calculateSinCos_Tuple(param);
```



## geeksforgeeks [Structured binding in C++](https://www.geeksforgeeks.org/structured-binding-c/)





```C++
#include <bits/stdc++.h> 
#include <map> 
using namespace std; 
  
int main() 
{ 
    // Creating a map with key and value  
    // fields as String 
    map<string, string> sites; 
      
    sites.insert({ "GeeksforGeeks", "Coding Resources" }); 
    sites.insert({ "StackOverflow", "Q-A type" }); 
    sites.insert({ "Wikipedia", "Resources + References" }); 
  
    for (auto & [ key, value ] : sites)  
    { 
       cout << key.c_str() << " " << value.c_str() << endl; 
    } 
      
    return 0; 
} 
```





## cppreference [Structured binding declaration (since C++17)](https://en.cppreference.com/w/cpp/language/structured_binding)

## 思考

1) Python  `a, b = b, a` swaps two variables，那C++中呢？

2) 下面程序在C++中的运行结果如何呢？

```python

x = [0, 1]
i = 0
i, x[i] = 1, 2         # i is updated, then x[i] is updated
print(x)
```

Python中的运行结果如下:

```
[0, 2]
```

