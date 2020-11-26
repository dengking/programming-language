# decltype and `std::declval`



## arne-mertz [Modern C++ Features – decltype and std::declval](https://arne-mertz.de/2017/01/decltype-declval/)



### decltype



So, `decltype` “returns” a type. It can basically be used wherever a type is needed:

```C++
#include <vector>
#include <iostream>

struct X
{
	int i;
	double bar(short);
};


int main()
{
	X x;
	decltype(x) y; //y has type X;
	std::vector<decltype(x.i)> vi; //vector<int>
	using memberFunctionPointer = decltype(&X::bar); //double X::(*)(short)

	auto lam = [&]() -> decltype(y){	return y;}; //decltype(y) is const X&

}
// g++ --std=c++11 test.cpp

```



### decltype returns