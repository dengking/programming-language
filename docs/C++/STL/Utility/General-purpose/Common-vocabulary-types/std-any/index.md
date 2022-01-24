# `std::any`

一、`std::any` 是典型的type erasure

> NOTE: 关于type erasure，参见`C++\Idiom\Type-Erasure`章节。

二、通过`std::any_cast`来将它转换为真实的type，它能够实现dynamic type safety，是对`void *`的增强

## microsoft [std::any: How, when, and why](https://devblogs.microsoft.com/cppblog/stdany-how-when-and-why/)

> NOTE:
>
> 这篇文章从type safety的角度阐述了使用`std::any`来替代`void *`的原因

### Storing arbitrary user data

```C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
struct day {
	// ...things... 
	void* user_data;
};

struct month {
	std::vector<day> days;
	void* user_data;
};
class Foo
{
public:
	void frobnicate()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
int main() {
	day some_day;
	some_day.user_data = new std::string{ "Hello, World!" };
	// …much later 
	Foo* some_foo = static_cast<Foo*>( some_day.user_data );
	some_foo->frobnicate(); // BOOM!
}
// g++ -std=c++14 -O2 -Wall -pedantic  test.cpp && ./a.out

```

> NOTE: 
>
> 上面是一个典型的说void pointer而无法实现type safe的例子

#### shortcomings

This solution has a few immediately apparent shortcomings:

1、You can always cast a `void*` to a `Foo*` whether or not the object it points at is actually a `Foo`. The lack of **type information** for the associated data means that the library can’t provide even a basic level of type safety by guaranteeing that later accesses to stored data use the same type as was stored originally: 

> NOTE: 
>
> type safety 问题

```C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
struct day {
	// ...things... 
	void* user_data;
};

struct month {
	std::vector<day> days;
	void* user_data;
};
class Foo
{
public:
	void frobnicate()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
int main() {
	day some_day;
	some_day.user_data = new std::string{ "Hello, World!" };
	// …much later 
	Foo* some_foo = static_cast<Foo*>( some_day.user_data );
	some_foo->frobnicate(); // BOOM!
}
// g++ -std=c++14 -O2 -Wall -pedantic  test.cpp && ./a.out

```

> NOTE: 
>
> 上述代码就存在leakage

2、`void*` doesn’t manage lifetime like a smart pointer would, so clients must manage the lifetime of the associated data manually. Mistakes result in memory leaks:

```C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
struct day {
	// ...things... 
	void* user_data;
};

struct month {
	std::vector<day> days;
	void* user_data;
};
class Foo
{
public:
	void frobnicate()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
int main() {
	day some_day;
	some_day.user_data = new std::string{ "Hello, World!" };
	// …much later 
	Foo* some_foo = static_cast<Foo*>( some_day.user_data );
	some_foo->frobnicate(); // BOOM!
	delete some_day.user_data;
	some_day.user_data = nullptr;
	month some_month;
	some_month.days.clear(); // Oops: hopefully none of these days had 
							 // non-null user_data
}
// g++ -std=c++14 -O2 -Wall -pedantic  test.cpp && ./a.out

```

> NOTE: 
>
> 编译警告如下:
>
> ```C++
> test.cpp: In function ‘int main()’:
> test.cpp:28:25: warning: deleting ‘void*’ is undefined [-Wdelete-incomplete]
>    28 |         delete some_day.user_data;
>       |                ~~~~~~~~~^~~~~~~~~
> void Foo::frobnicate()
> ```

3、The library cannot copy the object that a `void*` points at since it doesn’t know that object’s type. For example, if your library provides facilities to copy annotations from one week to another, clients must copy the associated data manually. As was the case with **manual lifetime management**, mistakes are likely to result in dangling pointers, double frees, or leaks:

```C++
some_month.days[0] = some_month.days[1]; 
if (some_month.days[1].user_data) { 
  // I'm storing strings in user_data, and don't want them shared 
  // between days. Copy manually: 
  std::string const& src = *some_month.days[1].user_data; 
  some_month.days[0].user_data = new std::string(src); 
}
```

> NOTE: 
>
> 这种写法是典型的"**manual lifetime management**"，它导致难以维护，并且容易出现错误

### use `std::shared_ptr<void>`

> NOTE: 
>
> 下面的这个例子是非常典型的使用 `std::shared_ptr<void>` 的，非常值得借鉴学习

The C++ Standard Library provides us with at least one tool that can help: `shared_ptr<void>`. Replacing the `void*` with `shared_ptr<void>` solves the problem of **lifetime management**:

```C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
struct day {
	// ...things... 
	std::shared_ptr<void> user_data;
};

struct month {
	std::vector<day> days;
	std::shared_ptr<void> user_data;
};
class Foo
{
public:
	Foo()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~Foo()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	void frobnicate()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
int main() {
	day some_day;
	some_day.user_data = std::make_shared<Foo>();
	// …much later 
	day some_other_day;
	std::cout << "assignment" << std::endl;
	some_day = some_other_day; // the object at which some_day.user_data _was_
						   // pointing is freed automatically
	std::cout << "end" << std::endl;
}
// g++ -std=c++14 -O2 -Wall -pedantic  test.cpp && ./a.out

```

> NOTE: 
>
> 一、输出如下:
>
> ```C++
> Foo::Foo()
> 
> assignment
> 
> Foo::~Foo()
> 
> end
> ```
>
> 可以看到在执行 `some_day = some_other_day;` 后，就`user_data`就被release了

since `shared_ptr` squirrels away enough type info to know how to properly destroy the object it points at. A client could create a `shared_ptr<Foo>`, and the deleter would continue to work just fine after converting to `shared_ptr<void>` for storage in the calendar.

This solution may help solve the copyability problem as well, if the client is happy to have multiple days/weeks/etc. hold copies of the same `shared_ptr<void>` – denoting a single object – rather than independent values. 

> NOTE: 
>
> 使用`shared_ptr<void>`能够解决 "copyability problem"，这是通过shared ownership来实现的

`shared_ptr` doesn’t help with the primary problem of **type-safety**, however. Just as with `void*`, `shared_ptr<void>` provides no help tracking the proper type for associated data. Using a `shared_ptr` instead of a `void*` also makes it impossible for clients to “hack the system” to avoid memory allocation by reinterpreting integral values as `void*` and storing them directly; using `shared_ptr` forces us to allocate memory even for tiny objects like `int`.

> NOTE: 
>
> 上面描述了使用 `shared_ptr` 的弊端

### Not just `any` solution will do

`std::any` is the smarter `void*`/`shared_ptr<void>`. You can initialize an `any` with a value of any copyable type. 

#### example1

```C++
#include <iostream>
#include <any>
#include <vector>

struct day {
	// ...things... 
	std::any user_data;
};

struct month {
	std::vector<day> days;
	std::any user_data;
};

class Foo
{
public:
	Foo()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	Foo(const Foo&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	Foo(Foo&&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;

	}
	Foo operator=(const Foo&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return Foo();
	}
	Foo operator=(Foo&&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return Foo();
	}
	~Foo()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	void frobnicate()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
int main() {
	day some_day;
	some_day.user_data = Foo();
	// …much later 
	day some_other_day;
	std::cout << "assignment" << std::endl;
	some_day = some_other_day; // the object at which some_day.user_data _was_
						   // pointing is freed automatically
	std::cout << "end" << std::endl;
}
// g++ -std=c++17 -O2 -Wall -pedantic  test.cpp && ./a.out

```

> NOTE: 
>
> 输出如下:
>
> ```C++
> Foo::Foo()
> 
> Foo::Foo(Foo&&)
> 
> Foo::~Foo()
> 
> assignment
> 
> Foo::~Foo()
> 
> end
> ```

1、Like `shared_ptr`, `any` remembers how to destroy the contained value for you when the `any` object is destroyed

2、Unlike `shared_ptr`, `any` also remembers how to *copy* the contained value and does so when the `any` object is copied

3、Unlike `shared_ptr`, `any` knows what type it contains

and uses that knowledge to ensure that when you access the **contained value** – for example, by obtaining a reference with `any_cast` – you access it with the **correct type**:

```C++
assert(std::any_cast<int&>(a1) == 42);             // succeeds
std::string str = std::any_cast<std::string&>(a1); // throws bad_any_cast since
                                                   // a1 holds int, not string
assert(std::any_cast<month&>(a2).days.size() == 0);
std::any_cast<month&>(a2).days.push_back(some_day);
```

If you want to avoid exceptions in a particular code sequence and you are uncertain what type an `any` contains, you can perform a **combined type query** and access with the **pointer overload** of `any_cast`:

```C++
if (auto ptr = std::any_cast<int>(&a1)) {
  assert(*ptr == 42); // runs since a1 contains an int, and succeeds
}
if (auto ptr = std::any_cast<std::string>(&a1)) {
  assert(false);      // never runs: any_cast returns nullptr since
                      // a1 doesn't contain a string
}
```

> NOTE: 
>
> 一、上面是使用了 [const T* any_cast(const any* operand) noexcept;](https://en.cppreference.com/w/cpp/utility/any/any_cast) `noexcept` 的特性做到的

### Small object optimization

The C++ Standard encourages implementations to store small objects with **non-throwing move constructors** directly in the storage of the `any` object, avoiding the costs of **dynamic allocation**. This feature is best-effort and there’s no guaranteed threshold below which `any` is portably guaranteed not to allocate. In practice, the Visual C++ implementation uses a larger `any` that avoids allocation for object types with non-throwing moves up to a handful of pointers in size, whereas libc++ and libstdc++ allocate for objects that are two or more pointers in size (See https://godbolt.org/z/RQd_w5).

## cppreference [std::any](https://en.cppreference.com/w/cpp/utility/any)





## 素材

bfilipek [Everything You Need to Know About std::any from C++17](https://www.bfilipek.com/2018/06/any.html)

stackoverflow [When should I use std::any](https://stackoverflow.com/questions/52715219/when-should-i-use-stdany)

open-std [*Any* Library Proposal (Revision 3)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3804.html)

