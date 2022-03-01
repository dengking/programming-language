# Exit time destructor

process可能normally exit也可能abnormally exit，当它exit的时候，destructor是否会被执行呢？

## 通过block scope来触发destructor被执行

有的时候，我们需要在`main`函数退出时，destructor未被执行，可以采取下面的措施：

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



## Exit-time destructor warning

### stackoverflow [How to deal with "exit-time destructor" warning in clang?](https://stackoverflow.com/questions/14335494/how-to-deal-with-exit-time-destructor-warning-in-clang)

In my C++11 code I get the clang warning "Declaration requires an exit-time destructor" in the following case:

```cpp
static const std::map<int, const someStruct> mymap = {
    {1, {
        "A",
        "B",
        "C"
    }},
    {2, {
        "D",
        "E",
        "F"
    }}
};
```



#### [A](https://stackoverflow.com/a/14335676)

Global and function static objects will get their destructors called when your application is exiting. these destructors are "exit time destructors". and are called in the reverse order that they were constructed in.

> NOTE: 
>
> 典型的stack order

As you said, if some of these destructors touch already destroyed objects, your program could crash. Also, destructors running at exit time will make the program exit slower, and most of the time they are not necessary for the correctness of the program (since when the program exits, it'll release all its memory anyway).

> NOTE: 
>
> 可能发生dangling

The warning is simply pointing out that you have destructors that'll be run at exit time.

The fix you proposed will heap allocate the object, which will not cause it to be automatically destroyed at program exit. For your case, this is probably good enough.

#### [A](https://stackoverflow.com/a/66137913)

See attributes no_destroy/always_destroy https://clang.llvm.org/docs/AttributeReference.html