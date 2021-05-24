# Relocate

在 github [remove move constructor for not_null. #842](https://github.com/microsoft/GSL/pull/842)  #**[hsutter](https://github.com/hsutter)** commented [on 5 Feb 2020](https://github.com/microsoft/GSL/pull/842#issuecomment-582074941) 

> I agree that `[strict_]not_null` move construction and move assignment should be `=delete`. Thanks for pointing this out!
>
> Yes, an intended effect of this change is that a `not_null<unique_ptr<T>>` can only sit there, it can't be moved anywhere. But this is already inherently true, moving one of those is impossible today without breaking the `not_null` invariant. The correct long-term answer for this would be if C++ gets something along the lines of the **relocation / destructive move semantics proposals**, where roughly "relocation/destructive-move leaves an object that is guaranteed to be no longer used" or similar (in those proposals, including even its dtor won't be called), then that would naturally enable cases like returning a local `not_null<unique_ptr<T>>` by value.

其中提及了  "**relocation / destructive move semantics proposals**"，Google了一下，发现了 open-std [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html) ，这篇paper中，作者提出了relocate、trivially relocatable。这篇论文中，总结的一些内容是不错的，值得深入学习。



## stackoverflow [c++ type trait to say “trivially movable” - examples of](https://stackoverflow.com/questions/45747932/c-type-trait-to-say-trivially-movable-examples-of)

> NOTE: 
>
> 1、提问者、回答者的内容，在 open-std [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html) 中，都有涵盖。
>
> 2、它揭示了move的陷阱

I would define "trivially movable" by

> Calling the move constructor (or the move assignment operator) is equivalent to `memcpy` the bytes to the new destination and not calling the destructor on the moved-from object.

For instance, if you know that this property holds, you can use `realloc` to resize a `std::vector` or a memory pool.

Types failing this would typically have pointers to their contents that needs to be updated by the move constructor/assignment operator.

> NOTE:
>
> 1、这就是 open-std [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html)  中，提出的 "Class contains pointer to self"

There is no such type traits in the standard that I can find. I am wondering whether this already has a (better) name, whether it's been discussed and whether there are some libraries making use of such a trait.

Edit 1:

From the first few comments, `std::is_trivially_move_constructible` and `std::is_trivially_move_assignable` are not equivalent to what I am looking for. I believe they would give `true` for types containing pointers to themselves, since reading your own member seems to fall under "trivial" operation.

> NOTE: 
>
> 1、通过下面的测试程序可知，`std::is_trivially_move_constructible` and `std::is_trivially_move_assignable` 对于   中，提出的 "Class contains pointer to self" 的确返回为true，显然，这是一个严重的陷阱，会导致dangling



### A

I think what you need is `std::is_trivially_relocatable` from proposal [P1144](http://open-std.org/JTC1/SC22/WG21/docs/papers/2020/p1144r5.html). Unfortunately the proposal didn't make it into [C++20](https://en.cppreference.com/w/cpp/20), so we shouldn't expect it before 2023. Which is sad, because this type trait would enable great optimizations for `std::vector` and similar types.



### A

Well, this got me thinking... It is very important to overload type traits of structs that hold a pointer to themselves.

The following code demonstrates how fast a bug can creep in code, when type_traits are not defined properly.

```C++
#include <memory>
#include <type_traits>

struct A
{
	int a;
	int b;
	int *p { &a };
};

int main()
{
	auto p = std::make_unique<A>();
	A a = std::move(*p.get());  // gets moved here, a.p is dangling.

	return std::is_move_assignable < A > ::value;  // <-- yet, this returns true.
}
// g++ test.cpp --std=c++14 -pedantic -Wall

```

输出为: 

```
1
```



```C++
#include <memory>
#include <iostream>
#include <type_traits>

struct A
{
	int a;
	int b;
	int *p { &a };
};

int main()
{
	auto p = std::make_unique<A>();
	A a = std::move(*p.get());  // gets moved here, a.p is dangling.

	std::cout << std::is_move_assignable<A>::value << std::endl;  // <-- yet, this returns true.
	std::cout << std::is_trivially_move_assignable<A>::value << std::endl;  // <-- yet, this returns true.
	std::cout << std::is_trivially_move_constructible<A>::value << std::endl;  // <-- yet, this returns true.
}
// g++ test.cpp --std=c++14 -pedantic -Wall

```

输出为: 

```
1
1
1
```

