# Lambda expressions



## microsoft [Lambda Expressions in C++](https://docs.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=vs-2019)

> NOTE: 这篇讲解地非常好

### Parts of a Lambda Expression

```c++
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

void abssort(float* x, unsigned n)
{
	std::sort(x, x + n,
			// Lambda expression begins
			[](float a, float b)
			{
				return (std::abs(a) < std::abs(b));
			} // end of lambda expression
			);
}

int main()
{
	float Array[] = { 6, -5, 4, -3, 2, 1 };

	std::cout << "Before sort:" << std::endl;
	for (auto Element : Array)
	{
		std::cout << Element << std::endl;
	}
	std::size_t Len = sizeof(Array) / sizeof(float);
	std::cout << "After sort:" << std::endl;
	abssort(Array, Len);
	for (auto Element : Array)
	{
		std::cout << Element << std::endl;
	}
}

```



This illustration shows the parts of a lambda:

![](./lambdaexpsyntax.png)

1. *capture clause* (Also known as the *lambda-introducer* in the C++ specification.)
2. *parameter list* Optional. (Also known as the *lambda declarator*)
3. *mutable specification* Optional.
4. *exception-specification* Optional.
5. *trailing-return-type* Optional.
6. *lambda body*.

#### Capture Clause



## cppreference [Lambda expressions](https://en.cppreference.com/w/cpp/language/lambda)



## drdobbs [Lambdas in C++11](https://www.drdobbs.com/cpp/lambdas-in-c11/240168241)

### Basic Syntax and Elements for Lambda Expressions



### Passing Lambda Expressions with `std::function`

Each time you are creating a **lambda expression**, its implementation creates a new class under the hood. Thus, each lambda creates a separate class and has a different type. As you might guess, even when many lambda expressions receive the same arguments and return the same type, each of them will be a different class; that is, a different type. Luckily, C++11 incorporates a **wrapper** for any of the following functions with `std::function` and makes it easy to pass around lambda expressions:

> NOTE: `std::function`是一个abstraction

- Lambda expression
- Function pointer
- Functor



