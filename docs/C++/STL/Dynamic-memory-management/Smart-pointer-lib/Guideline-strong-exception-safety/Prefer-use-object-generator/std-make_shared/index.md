# `std::make_shared`

## Object-Generator idiom

`make_shard`使用的是Object-Generator idiom，参见`C++\Idiom\Template-metaprogramming\Object-Generator`。

## Prefer `std::make_shared` over new expression

在下面文章中，对此进行了说明:

1、stackoverflow [is it better to use shared_ptr.reset or operator =?](https://stackoverflow.com/questions/14836691/is-it-better-to-use-shared-ptr-reset-or-operator)

2、

## cppreference [std::make_shared](https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared)





## Example

### Example1 `make_shard` with no argument

这个例子是源自cppreference [std::make_shared, std::make_shared_for_overwrite](https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared)

```c++
#include <iostream>
#include <memory>
#include <type_traits>
 
struct C
{
  
  int i;
};
 
int main()
{
    auto sp = std::make_shared<C>();
 
    //static_assert(std::is_same_v<decltype(sp), std::shared_ptr<C>>);
 
    std::cout << sp->i << '\n';
}
```

### Example2

这个例子源自[learncpp 15.6 — std::shared_ptr](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)：

```c++
#include <iostream>
#include <memory> // for std::shared_ptr
 
class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};
 
int main()
{
	// allocate a Resource object and have it owned by std::shared_ptr
	auto ptr1 = std::make_shared<Resource>();
	{
		auto ptr2 = ptr1; // create ptr2 using copy initialization of ptr1
 
		std::cout << "Killing one shared pointer\n";
	} // ptr2 goes out of scope here, but nothing happens
 
	std::cout << "Killing another shared pointer\n";
 
	return 0;
} // ptr1 goes out of scope here, and the allocated Resource is destroyed
```

