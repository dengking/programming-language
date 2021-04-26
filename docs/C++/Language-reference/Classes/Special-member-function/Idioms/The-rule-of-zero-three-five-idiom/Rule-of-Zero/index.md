# The Rule of Zero

一、运用C++11-default-member-initializer，来避免写constructor

cpppatterns [The rule of zero](https://cpppatterns.com/patterns/rule-of-zero.html) 中的两个例子是非常典型的。

二、"we can avoid writing any custom copy/move constructors, assignment operators, or destructors by using existing types that support the appropriate copy/move semantics"

> NOTE: 
>
> 上述摘抄自 cpppatterns [The rule of zero](https://cpppatterns.com/patterns/rule-of-zero.html)。

三、运用Implicitly-defined constructor、destructor、assignment

四、rule of zero使用的是默认行为，如果这些默认行为不符合预期，则需要custom implementation

五、Single Responsibility Principle and RAII and composition

> NOTE: 
>
> 1、这是我在阅读  stackoverflow [Rule of Zero confusion?](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion)  comment
>
> > Ownership == responsibility for the lifecycle of a managed resource. For example, the various smart pointer classes model different kinds of ownership (shared vs. unique, etc.)
>
> 时，联想到的。
>
> 

single responsibility principle: 每个member都管理一个resource

RAII: 每个member，都遵循rule of five/three/zero，RAII

composition: 将它们作为member data

C++11-default-member-initializer: 每个member，都需要合适的initialization

## stackoverflow [Rule of Zero confusion?](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion) 

### comments

Ownership == responsibility for the lifecycle of a managed resource. For example, the various smart pointer classes model different kinds of ownership (shared vs. unique, etc.) – [Oliver Charlesworth](https://stackoverflow.com/users/129570/oliver-charlesworth) [Jul 9 '17 at 15:22](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion#comment76969848_44997955)

### [A](https://stackoverflow.com/a/45016295)



## cpppatterns [The rule of zero](https://cpppatterns.com/patterns/rule-of-zero.html)

Utilise the value semantics of existing types to avoid having to implement custom copy and move operations.

> NOTE: 
>
> 1、simplify C++

### Example 1

> NOTE: 
>
> 1、copyable and movable

```C++
#include<iostream>
#include<memory>
#include<utility>
#include <vector>
class foo
{
private:
	int x = 10;
	std::vector<int> v = { 1, 2, 3, 4, 5 };
};

int main()
{
	foo f;
	foo f1(f);

	foo f2(std::move(f1));

	return 0;
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++14

```



### Example 2

> NOTE: 
>
> 1、movable but not copyable

The class `bar` on [lines 11–15](https://cpppatterns.com/patterns/rule-of-zero.html#line11) is not copyable by default because it has a [`std::unique_ptr`](http://en.cppreference.com/w/cpp/memory/unique_ptr) member which itself is not copyable. However, it correctly supports move operations, which will **transfer ownership** of the dynamically allocated resource.

```C++
#include<iostream>
#include<memory>
#include<utility>
class bar
{
public:
	std::unique_ptr<int> p = std::make_unique<int>(5);
};
int main()
{
	bar b;

	bar b2 { std::move(b) };

	std::cout << b.p.get() << std::endl;

    // bar bb { b };
	/**
	 * test.cpp: In function 'int main()':
test.cpp:12:16: error: use of deleted function 'bar::bar(const bar&)'
     bar bb { b };
                ^
test.cpp:4:7: note: 'bar::bar(const bar&)' is implicitly deleted because the default definition would be ill-formed:
 class bar
       ^~~
test.cpp:4:7: error: use of deleted function 'std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = int; _Dp = std::default_delete<int>]'
In file included from /usr/include/c++/8/memory:80,
                 from test.cpp:2:
/usr/include/c++/8/bits/unique_ptr.h:389:7: note: declared here
       unique_ptr(const unique_ptr&) = delete;
	 *
	 */

	return 0;
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++14

```

> NOTE: 
>
> 1、输出为:
>
> ```C++
> 0
> ```
>
> 2、通过上述输出可以知道:
>
> implicit defined move constructor是遵循: 
>
> a、CppCoreGuidelines [C.64: A move operation should move and leave its source in a valid state](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state)

## TODO

最近在阅读一些开源软件中，发现它们广泛地使用`=defaul`，这让我想起来rule of zero。之前了解了一些，但是由于对于C++的了解不深入，所以没有来得及深入学习。


https://stackoverflow.com/questions/44997955/rule-of-zero-confusion




### fluentcpp [The Rule of Zero in C++](https://www.fluentcpp.com/2019/04/23/the-rule-of-zero-zero-constructor-zero-calorie/)




### `std::unique_ptr` and rule of zero

https://stackoverflow.com/questions/59146954/how-to-implement-the-rule-of-zero-with-unique-ptr

http://www.nirfriedman.com/2015/06/27/cpp-rule-of-zero/

https://marcoarena.wordpress.com/2014/04/12/ponder-the-use-of-unique_ptr-to-enforce-the-rule-of-zero/

