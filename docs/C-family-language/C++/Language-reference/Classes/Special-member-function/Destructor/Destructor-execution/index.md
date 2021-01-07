# Process exit and destructor

process可能normally exit也可能abnormally exit，当它exit的时候，destructor是否会被执行呢？

## 通过scope、RAII来触发destructor被执行

有的时候，我们需要在`main`函数退出时，destructor被执行，可以采取下面的措施：

- 对于automatic variable，可以将它声明在`{}`来确保它的destructor被执行
- 对于heap variable，可以通过RAII+smart pointer+`{}`来确保它的destructor被执行

下面是example：

```c++
#include <iostream>
#include <memory>

class Base
{
	// some virtual methods
public:
	virtual ~Base()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class Derived: public Base
{
public:
	~Derived()
	{
		// Do some important cleanup
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

int main()
{
	// automatic variable
	{
		Derived d;
	}
	// heap variable
	{
		std::unique_ptr<Base> d = std::unique_ptr<Base> { new Derived };
	}
}
// g++ --std=c++11 test.cpp

```

上述程序的输出如下：

```c++
virtual Derived::~Derived()
virtual Base::~Base()
virtual Derived::~Derived()
virtual Base::~Base()
```

