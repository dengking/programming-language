# Value initialization



## cppreference [Value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)







## Example

```c++
#include <iostream>
struct A
{
	int i;
	A()
	{
	} // user-provided default ctor, does not initialize i
};

struct B
{
	A a;
};
// implicitly-defined default ctor
int main()
{
	std::cout << B().a.i << '\n'; // value-initializes a B temporary
}
```

输出如下：

```
0
```

