# `std::make_unique` and deep copy

## 起因

在阅读 wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer) 的 "C++ Example: **d-pointer pattern**" 时，其中给出的例子是典型的 "Copy constructor for a class with `unique_ptr`"，按照 stackoverflow [Copy constructor for a class with unique_ptr](https://stackoverflow.com/questions/16030081/copy-constructor-for-a-class-with-unique-ptr) 中的说法，显然这种情况下是需要deep copy的，但是其中的实现是使用的`std::make_unique`，这个函数是否是deep copy呢？

## stackoverflow [Why std::make_unique calls copy constructor](https://stackoverflow.com/questions/57019871/why-stdmake-unique-calls-copy-constructor)

I'm wondering why make_unique calls copy constructor but does not call default constructor.

```C++
#include <memory>
#include <iostream>

template<typename T1, typename T2, typename T3, typename  T4>
class Node
{
public:
	Node()
	{
		std::cout << "default\n";
	}

	~Node()
	{
		std::cout << " Delete\n";
	}

	Node(const Node &other)
	{
		std::cout << "copy\n";
	}
};

int main()
{
	Node<int, int, int, int> test1; //Calling default Cons
	std::unique_ptr<Node<int, int, int, int>> test2 = std::make_unique<Node<int, int, int, int>>(test1); //Nothing called

	Node<int, int, int, int> *test3 = test2.get();
	Node<int, int, int, int> test4 = Node<int, int, int, int>(*test3); // Calling copy Cons

	std::unique_ptr<Node<int, int, int, int>> test5 = std::make_unique<Node<int, int, int, int>>(test4); //Calling copy Cons
}
// g++ --std=c++14 test.cpp

```

> NOTE: 
>
> 上述是完整的测试程序
>
> 测试环境: https://coliru.stacked-crooked.com/  ；
>
> 编译指令为:
>
> ```sh
> g++ -std=c++17  -Wall -pedantic -pthread main.cpp && ./a.out
> ```
>
> 输出为:
>
> ```
> default
> copy
> copy
> copy
>  Delete
>  Delete
>  Delete
>  Delete
> ```
>
> 可以看到，上述输出与提问者描述的不同，差异在:`std::unique_ptr<Node<int, int, int, int>> test2 = std::make_unique<Node<int, int, int, int>>(test1);`
>
> 在上述测试环境中，它会调用copy constructor。

For example in code shown above: Frist, we create Node object -> calling default constructor. Then we wrap this object into smart-pointer object -> no calls.

But if we make deep copy of Node object -> calling copy constructor And then wrap copy into smart-pointer object-> calling copy constructor.

It's somehow connected to the creation of new control block?

[A](https://stackoverflow.com/a/57019883)

```cpp
std::unique_ptr<Node<int,int,int,int>> test5 = 
   std::make_unique<Node<int,int,int,int>(test4);// You copy from test4
```

`std::make_unique` creates a *new* pointer. It doesn't reuse an existing pointer. So since test4 is already there, it has to copy it. After test5 is constructed, test4 is still there (so can't do a move) and test5 is holding a new object, copied from test4.



## wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer)

> NOTE: 在这篇文章中也给出了例子，它被收录在 `Opaque-data-type\Opaque-pointer` 章节