# `const_cast`

drop CV是危险的，但是`C++`还是给programmer提供了这种权利，这是遵循C++ philosophy的（`C++\Philosophy.md`）。

## cppreference [`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) 

only `const_cast` may be used to cast away (remove) constness or volatility.

> NOTE: C++中drop CV的唯一的方法

### 1)

### 2) 

> NOTE: 需要注意的是，必须是相同类型。

#### Example: `this`

```c++
#include <iostream>

struct type
{
	int i;

	type() : i(3)
	{
	}

	void f(int v) const
	{
		// this->i = v;                 // compile error: this is a pointer to const
		const_cast<type*>(this)->i = v; // OK as long as the type object isn't const
	}
};

int main()
{
	type t; // if this was const type t, then t.f(4) would be undefined behavior
	t.f(4);
	std::cout << "type::i = " << t.i << '\n';
}
// g++ test.cpp

```



#### Example: reference

```c++
#include <iostream>

int main()
{
	int i = 3;                 // i is not declared const
	const int& rci = i;
	const_cast<int&>(rci) = 4; // OK: modifies i
	std::cout << "i = " << i << '\n';
}
// g++ test.cpp

```



#### Example: different type

```c++
#include "stddef.h"

int main()
{
	const void* ptr = NULL;
	const_cast<char*>(ptr); // compiler error
}
// g++ test.cpp

```

上述程序编译报错如下：

```c++
test.cpp: 在函数‘int main()’中:
test.cpp:6:23: 错误：从类型‘const void*’到类型‘char*’中的 const_cast 无效
  const_cast<char*>(ptr); // compiler errorc
```



#### Example: cppreference [`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) 

```c++
#include <iostream>

struct type
{
	int i;

	type()
			: i(3)
	{
	}

	void f(int v) const
	{
		// this->i = v;                 // compile error: this is a pointer to const
		const_cast<type*>(this)->i = v; // OK as long as the type object isn't const
	}
};

int main()
{
	int i = 3;                 // i is not declared const
	const int& rci = i;
	const_cast<int&>(rci) = 4; // OK: modifies i
	std::cout << "i = " << i << '\n';

	type t; // if this was const type t, then t.f(4) would be undefined behavior
	t.f(4);
	std::cout << "type::i = " << t.i << '\n';

	const int j = 3; // j is declared const
	int* pj = const_cast<int*>(&j);
	// *pj = 4;      // undefined behavior

	void (type::*pmf)(int) const = &type::f; // pointer to member function
	// const_cast<void(type::*)(int)>(pmf);   // compile error: const_cast does
	// not work on function pointers
}
// g++ test.cpp

```



### 3) 

### NOTE

`const_cast` makes it possible to form a reference or pointer to non-const type that is actually referring to a [const object](https://en.cppreference.com/w/cpp/language/cv) or a reference or pointer to non-volatile type that is actually referring to a [volatile object](https://en.cppreference.com/w/cpp/language/cv). Modifying a const object through a non-const access path and referring to a volatile object through a non-volatile [glvalue](https://en.cppreference.com/w/cpp/language/value_category#glvalue) results in undefined behavior.

> NOTE: 虽然危险，但是给予了programmer这样的权利

https://stackoverflow.com/questions/19554841/how-to-use-const-cast



## modify a const variable

https://www.geeksforgeeks.org/how-to-modify-a-const-variable-in-c/

https://stackoverflow.com/questions/2006161/changing-the-value-of-const-variable-in-c

## Cast away CV

阅读[static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)、[dynamic_cast conversion](https://en.cppreference.com/w/cpp/language/dynamic_cast)，可以看到两者都有这样的描述：

> except when such conversions would cast away *constness* or *volatility*.



如果cast away CV，compiler是会报错的：

```c++
static_cast from 'const clone_inherit<concrete, cloneable>' to 'concrete &' casts away qualifiers
```

