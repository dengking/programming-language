# non-assignable

## Example

这个例子是取自 cppreference [C++ named requirements: *Swappable*](https://en.cppreference.com/w/cpp/named_req/Swappable)，它的non assignable的写法是:

```C++
	IntVector& operator=(IntVector) = delete; // not assignable
```

显然它的这种写法是包含了:

1、copy assign

```c++
class_name & class_name :: operator= ( const class_name & )=delete;    
```



2、move assign

```C++
class_name & class_name :: operator= ( class_name && )=delete;
```



### 使用copy assign

```C++
#include <iostream>
#include <vector>

class IntVector
{
	std::vector<int> v;

public:
	void swap(IntVector &other)
	{
		v.swap(other.v);
	}
	IntVector& operator=(IntVector) = delete; // not assignable
};

int main()
{
	IntVector v1, v2;
	v2 = v1;
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```

编译报错如下:

```C++
test.cpp: 在函数‘int main()’中:
test.cpp:20:5: 错误：使用了被删除的函数‘IntVector& IntVector::operator=(IntVector)’
  v2 = std::move(v1);
     ^
test.cpp:14:13: 错误：在此声明
  IntVector& operator=(IntVector) = delete; // not assignable
```

### 使用move assign

```c++
#include <iostream>
#include <vector>
#include <utility>

class IntVector
{
        std::vector<int> v;

public:
        void swap(IntVector &other)
        {
                v.swap(other.v);
        }
        IntVector& operator=(IntVector) = delete; // not assignable
};

int main()
{
        IntVector v1, v2;
        v2 = std::move(v1);
}
// g++ --std=c++11 test.cpp -Wall -pedantic
```

编译报错如下:

```C++
test.cpp: 在函数‘int main()’中:
test.cpp:20:5: 错误：使用了被删除的函数‘IntVector& IntVector::operator=(IntVector)’
  v2 = std::move(v1);
     ^
test.cpp:14:13: 错误：在此声明
  IntVector& operator=(IntVector) = delete; // not assignable
```

## Non-assignable mixin



### Before C++11

```C++
class nonassignable
{
protected:
	nonassignable()
	{
	}
	~nonassignable()
	{
	}
private:  // emphasize the following members are private
	nonassignable& operator=(nonassignable);
};

```



### Since C++11

```C++
class nonassignable
{
protected:
	~nonassignable() = default;
public:
	nonassignable& operator=(nonassignable) = delete;
};

```

