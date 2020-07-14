# Zero initialization



## cppreference [Zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization)

### Syntax

Note that this is not the syntax for **zero initialization**, which does not have a dedicated syntax in the language. These are examples of other types of initializations, which might perform zero initialization.

> NOTE: 上面这段话是让人疑惑的，对于programmer而言，似是而非、缺乏统一标准是应当避免的。



| syntax                                |                                                              |      |
| :------------------------------------ | ------------------------------------------------------------ | ---- |
| `static T object ;`                   |                                                              |      |
| `T () ;` <br>`T t = {} ;`<br>`T {} ;` | As part of [value-initialization](https://en.cppreference.com/w/cpp/language/value_initialization) sequence for non-class types and for members of value-initialized class types that have no constructors, including value initialization of elements of [aggregates](https://en.cppreference.com/w/cpp/language/aggregate_initialization) for which no initializers are provided. |      |
| `CharT array [ n ] = "";`             |                                                              |      |



## Example of `T {}`

```c++
#include <iostream>

struct Test
{
	int a;
	int b;
};

class C
{

public:
	Test t;
};

int main()
{
	C c;
	std::cout << c.t.a << std::endl;
	std::cout << c.t.b << std::endl;
}
```

上述例子是不使用zero initialization，可以看到`c.t.a`、`c.t.b`都是随机值。



```c++
#include <iostream>

struct Test
{
	int a;
	int b;
};

class C
{

public:
	Test t { };
};

int main()
{
	C c;
	std::cout << c.t.a << std::endl;
	std::cout << c.t.b << std::endl;
}
```

上述例子是使用zero initialization，可以看到`c.t.a`、`c.t.b`都是默认值0。

作为对比，可以看看下面的例子：

```c++
#include <iostream>

struct Test
{
int a;
int b;
};


class C
{

public:
    Test t {1};

};

int main()
{
    C c;
    std::cout<<c.t.a<<std::endl;
    std::cout<<c.t.b<<std::endl;
}

```

输出如下：

```
1
0
```



作为对比，看看涉及[Value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)的例子：

```c++
// C++ code to demonstrate zero initialisation 

#include <iostream> 
#include <string> 

struct foo { 
	int x, y, z; 
}; 

double f[3]; // zero-initialized to three 0.0's 

int* p; // zero-initialized to null pointer value 

// zero-initialized to indeterminate value 
// then default-initialized to "" 
std::string s; 

int main(int argc, char* argv[]) 
{ 
	foo x = foo(); 
	
	std::cout << x.x << x.y << x.z << '\n'; 
	
	return 0; 
} 

```

输出为:

```
000
```

