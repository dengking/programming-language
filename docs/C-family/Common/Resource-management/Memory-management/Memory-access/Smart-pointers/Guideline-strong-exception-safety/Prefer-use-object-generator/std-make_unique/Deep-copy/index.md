# `std::make_unique` and deep copy

## 起因

在阅读 wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer) 的 "C++ Example: **d-pointer pattern**" 时，其中给出的例子是典型的 "Copy constructor for a class with `unique_ptr`"，按照 stackoverflow [Copy constructor for a class with unique_ptr](https://stackoverflow.com/questions/16030081/copy-constructor-for-a-class-with-unique-ptr) 中的说法，显然这种情况下是需要deep copy的，但是其中的实现是使用的`std::make_unique`，这个函数是否是deep copy呢？

## stackoverflow [Why std::make_unique calls copy constructor](https://stackoverflow.com/questions/57019871/why-stdmake-unique-calls-copy-constructor)

I'm wondering why make_unique calls copy constructor but does not call default constructor.

```C++
#include <memory>
#include <iostream>

template<typename, typename, typename, typename >
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

	std::unique_ptr<Node<int, int, int, int>> test5 = std::make_unique < Node<int, int, int, int>(test4); //Calling copy Cons
}
// g++ --std=c++14 test.cpp

```

> NOTE: 上述是完整的测试程序