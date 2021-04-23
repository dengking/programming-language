# drdobbs [C++ Made Easier: The Rule of Three](https://www.drdobbs.com/c-made-easier-the-rule-of-three/184401400)



## An Example and a Misconception

[example code](./Code/drdobbs/IntVec-Misconception.cpp)

> NOTE: 原文给出的这个例子是非常具有代表性的。
>
> 这个例子，导致错误是double free，它的根源在于shallow copy

```C++
#include<iostream>
// This class contains a subtle error
class IntVec
{
public:
	IntVec(int n) :
					data(new int[n])
	{
	}
	~IntVec()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		delete[] data;
	}
	int& operator[](int n)
	{
		return data[n];
	}
	const int& operator[](int n) const
	{
		return data[n];
	}

private:
	int *data;
};

int main()
{
	IntVec x(100);
	IntVec y = x;   // Trouble!
	return 0;
}
// g++ test.cpp -pedantic -Wall -Wextra

```

