# The Rule of Zero

相比于rule of three/five，rule of zero是比较难理解的，下面是一些要点: 

一、运用C++11-default-member-initializer，来简化constructor

cpppatterns [The rule of zero](https://cpppatterns.com/patterns/rule-of-zero.html) 中的两个例子是非常典型的。

二、cpppatterns [The rule of zero](https://cpppatterns.com/patterns/rule-of-zero.html)

> we can avoid writing any custom copy/move constructors, assignment operators, or destructors by using existing types that support the appropriate copy/move semantics

1、class的moveable、copyable是受其member的movable、copyable影响的，比如有一个non-movable data member，则这个class就是non-movable的

三、运用implicitly-defined constructor、destructor、assignment，后者default

四、rule of zero使用的是默认行为，如果这些默认行为不符合预期，则需要custom implementation

五、Single Responsibility Principle and RAII and composition

1、这是我在阅读  stackoverflow [Rule of Zero confusion?](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion)  的 comment

> Ownership == responsibility for the lifecycle of a managed resource. For example, the various smart pointer classes model different kinds of ownership (shared vs. unique, etc.)

时，联想到的。

single responsibility principle: 每个member都管理一个resource

RAII: 每个member，都遵循rule of five/three/zero，RAII

composition: 将它们作为member data

C++11-default-member-initializer: 每个member，都需要合适的initialization

六、florianwolters [The Rule of Zero](http://blog.florianwolters.de/educational/2015/01/31/The_Rule_of_Zero/) # The Rule of Zero

> This basically means that one should never use a **raw pointer** to manage a resource. Therefore no destructor, copy constructor, copy assignment operator, move constructor and move assignment operator has to be implemented.
>
> *The Rule of Zero* allows to implement **{C, M}**, **{UC, M}**, **{C, UM}** and **{UC, UM}**, **without declaring them explicitly**. The emphasized part of the last sentence is the important difference to *The Rule of Five*.
>
> The aim of *The Rule of Zero* is to eliminate resource management for the user and let the Standard Library do all the work related to resource management.

使用STL来完成resource management。

那对于自己的一些个性化的resource management呢？其实也应该遵循rule of zero进行设计。

七、florianwolters [The Rule of Zero](http://blog.florianwolters.de/educational/2015/01/31/The_Rule_of_Zero/) # Summary

> By using automatic resource management, all required constructors and assignment operators can be implicitly declared and defined by the compiler.

Rule of zero 有一些 automatic resource management 的意思。

八、feabhas [The Rule of Zero](https://blog.feabhas.com/2015/01/the-rule-of-zero/) 

> “The Rule of Zero” basically states:
>
> > *You should NEVER implement a destructor, copy constructor, move constructor or assignment operators in your code.*
>
> With the (very important) corollary to this:
>
> > *You should NEVER use a raw pointer to manage a resource.*

需要注意的是，是"NEVER implement a destructor, copy constructor, move constructor or assignment operators"，constructor还是要实现的。



## stackoverflow [Rule of Zero confusion?](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion) 

### comments

Ownership == responsibility for the lifecycle of a managed resource. For example, the various smart pointer classes model different kinds of ownership (shared vs. unique, etc.) – [Oliver Charlesworth](https://stackoverflow.com/users/129570/oliver-charlesworth) [Jul 9 '17 at 15:22](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion#comment76969848_44997955)

### [A](https://stackoverflow.com/a/45016295)

**The Rule of Zero**

The rule of zero is another rule of thumb about how to write classes that need to use some resources like memory or other objects. In the example the dynamically allocated memory containing the characters of the string is a resource that has to be managed.

The recommendation is to let specialized classes manage resources, and do only that. In the example, `std::string` takes care of all the details of managing the allocated memory.

> NOTE: 
>
> single responsibility principle，这是实现rule of zero的关键

The rule emerged after the introduction of C++11, because the language and the standard library had improved, providing much better facilities to manage dynamically allocated object lifetimes (`unique_ptr` and `shared_ptr`). Also the containers now allow **in-place construction**, removing another reason for dynamic allocation. It should probably be seen as an update to the much older rule of three.

> NOTE: 
>
> "in-place construction"指的是 emplace。

So if you previously would have used *new* in your constructor to create some member and *delete* in the destructor, you should now use a unique_ptr to manage the lifetime of the member, getting move construction and move assignment "for free".

Shared pointers can remember the correct destructor to call, so the common need for a virtual destructor goes away for objects that are exclusively managed via shared pointer, even if they are used polymorphically.

So basically a class that can rely on its members to do all the required actions for initialization, moving, copying and destruction should not declare any of the special member functions.

**The Rule of Five**

As always with C++ things are not always that simple.

As Scott Meyers pointed out, if you do have to add a destructor for whatever reason, the implicit generation of move constructors and move assignment operator are disabled, even if the compiler could generate them.

Then the compiler will happily copy your class all over the place instead of moving it which may not be what you expect. This may for example slow down your program, as more copying has to be performed. The compiler will not warn about this by default.

Therefore he recommends to explicitly specify which of the five special methods you want, in order to avoid surprises due to unrelated changes. He still recommends writing non-resource-management classes such that the defaults generated by the compiler can be used.

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

