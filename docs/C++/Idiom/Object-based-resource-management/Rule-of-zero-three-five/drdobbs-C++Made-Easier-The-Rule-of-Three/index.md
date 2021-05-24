# drdobbs [C++ Made Easier: The Rule of Three](https://www.drdobbs.com/c-made-easier-the-rule-of-three/184401400)



## An Example and a Misconception

[example code](./Code/drdobbs/IntVec-Misconception.cpp)

> NOTE: 
>
> 1、原文给出的这个例子是非常具有代表性的。
>
> 这个例子，导致错误是double free，它的implicit defined copy constructor是shallow copy的

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



## Fixing Our Class



### Non-copyable



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

	// these two member functions added
	IntVec(const IntVec&);
	IntVec& operator=(const IntVec&);
};

int main()
{
	IntVec x(100);
	IntVec y = x;   // Trouble! compile error
	return 0;
}
// g++ test.cpp -pedantic -Wall -Wextra

```



### Rule of three



```C++
#include<iostream>
// This class corrects the error by
// defining copying and assignment
class IntVec
{
public:
	IntVec(int n) :
					data(new int[n]), size(n)
	{
	}
	~IntVec()
	{
		delete[] data;
	}
	;
	int& operator[](int n)
	{
		return data[n];
	}
	const int& operator[](int n) const
	{
		return data[n];
	}

	IntVec(const IntVec &v) :
					data(new int[v.size]), size(v.size)
	{
		std::copy(data, data + size, v.data);
	}
	IntVec&
	operator=(const IntVec &v)
	{
		int *newdata = new int[v.size];
		std::copy(v.data, v.data + v.size, newdata);
		delete[] data;
		data = newdata;
		size = v.size;
		return *this;
	}

private:
	int *data;
	int size;
};
int main()
{
	IntVec x(100);
	IntVec y = x;   // Trouble!
	return 0;
}
// g++ test.cpp -pedantic -Wall -Wextra

```

