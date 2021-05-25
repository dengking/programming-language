# Array initialization



## 将数组初始化为0

### Static array/ stack array

```C++
#include <iostream>
#include <algorithm>
int main()
{
	constexpr int ArraySize = 100;
	int Array[ArraySize] { };

	std::cout << std::all_of(Array, Array + ArraySize, [](int a)
	{	return a==0;}
	) << std::endl;

}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```



### Dynamic array

```C++
#include <iostream>
#include <algorithm>
#include <memory>
int main()
{
	constexpr int ArraySize = 100;
	int *Array { new int[ArraySize] { } };

	std::cout << std::all_of(Array, Array + ArraySize, [](int a)
	{	return a==0;}
	) << std::endl;

}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```



### stackoverflow [Initializing an array of zeroes](https://stackoverflow.com/questions/38892455/initializing-an-array-of-zeroes)

It is well known that missing initializers for an array of scalars are defaulted to zero.

```cpp
int A[5]; // Entries remain uninitialized
int B[5]= { 0 }; // All entries set to zero
```

But is this (below) guaranteed ?

```cpp
int C[5]= { }; // All entries set to zero
```

[A](https://stackoverflow.com/a/38892531)

The empty braced initialisation performs aggregation-initialization of the array: this leads to zero-initialization of the `int` elements.

Yes, this is guaranteed.



## stackoverflow [C++ array initialization](https://stackoverflow.com/questions/1920430/c-array-initialization)



[A](https://stackoverflow.com/a/1920481)



If you believe it improves readability, you can certainly use

```cpp
bool myBoolArray[ARRAY_SIZE] = { false };
char* myPtrArray[ARRAY_SIZE] = { nullptr };
```

> NOTE: 
>
> 测试程序:
>
> ```C++
> #include <iostream>
> #include <algorithm>
> #include <memory>
> int main()
> {
> 	constexpr int ArraySize = 100;
> 	bool myBoolArray[ArraySize] = { false };
> 	char *myPtrArray[ArraySize] = { nullptr };
> 
> 	std::cout << std::all_of(myBoolArray, myBoolArray + ArraySize, [](bool a) 
> 	{	return a==false;}
> 	) << std::endl;
> 	std::cout << std::all_of(myPtrArray, myPtrArray + ArraySize, [](char *a) 
> 	{	return a==nullptr;}
> 	) << std::endl;
> }
> // g++ test.cpp --std=c++11 -pedantic -Wall -Wextra
> 
> ```
>
> 输出如下:
>
> ```C++
> 1
> 1
> ```
>
> 说明数组中的所有元素都被设置为相同值了。
>
> 





However, in C++ `= { 0 }` might not work for all types, like enum types, for example, which cannot be initialized with integral `0`. 



But C++ supports the shorter form

```c++
T myArray[ARRAY_SIZE] = {};
```



## cppreference [Array initialization](https://en.cppreference.com/w/c/language/array_initialization)





## TODO

https://www.dummies.com/programming/cpp/initializing-an-array-in-c/

https://www.learncpp.com/cpp-tutorial/62-arrays-part-ii/

https://stackoverflow.com/questions/4057948/initializing-a-member-array-in-constructor-initializer



https://www.dummies.com/programming/cpp/initializing-an-array-in-c/

https://stackoverflow.com/questions/4057948/initializing-a-member-array-in-constructor-initializer

https://stackoverflow.com/questions/2983819/how-initialize-array-of-classes



