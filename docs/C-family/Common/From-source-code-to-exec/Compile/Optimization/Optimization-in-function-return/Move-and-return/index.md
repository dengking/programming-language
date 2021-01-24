# Move and return

在 `From-source-code-to-exec\Compile\Optimization\Copy-elision-and-RVO\RVO-and-NRVO` 中，我们已经知道，compiler的optimization，因此，一般return value optimization是不需要programmer承担的，但是不熟悉的programmer还是会画蛇添足。

## developers.redhat [Understanding when not to std::move in C++](https://developers.redhat.com/blog/2019/04/12/understanding-when-not-to-stdmove-in-c/)

One of the most important concepts introduced in C++11 was *[move semantics](https://en.cppreference.com/w/cpp/language/move_constructor).* Move semantics is a way to avoid expensive deep copy operations and replace them with cheaper move operations. Essentially, you can think of it as turning a deep copy into a shallow copy.

This version of swap consists of one move construction and two move assignments and does not involve any deep copies. All is well. However, `std::move` must be used judiciously(明智的); using it blithely may lead to performance degradation, or simply be redundant, affecting readability of the code. Fortunately, the compiler can sometimes help with finding such wrong uses of `std::move`. In this article, I will introduce two new warnings I’ve implemented for GCC 9 that deal with incorrect usage of `std::move`.

### -Wpessimizing-move

When returning a local variable of the same class type as the function return type, the compiler is free to omit any copying or moving (i.e., perform [*copy/move elision*](https://en.cppreference.com/w/cpp/language/copy_elision)), if the variable we are returning is a non-volatile automatic object and is not a function parameter. In such a case, the compiler can construct the object directly in its final destination (i.e., in the caller’s stack frame). The compiler is free to perform this optimization even when the move/copy construction has side effects. Additionally, C++17 says that copy elision is mandatory in certain situations. This is what we call *Named Return Value Optimization* (NRVO). (Note that this optimization does not depend on any of the `-O` levels.) For instance:

```
struct T {
  // ...
};

T fn()
{
  T t;
  return t;
}

T t = fn ();
```

The object a function returns doesn’t need to have a name. For example, the return statement in the function `fn` above might be `return T();` and copy elision would still apply. In this case, this optimization is simply *Return Value Optimization* (RVO).

Some programmers might be tempted to “optimize” the code by putting `std::move` into the return statement like this:

```
T fn()
{
  T t;
  return std::move (t);
}
```

However, here the call to `std::move` precludes the NRVO, because it breaks the conditions specified in the C++ standard, namely [*[class.copy.elision\]*](http://eel.is/c++draft/class.copy.elision): the returned expression must be a name. The reason for this is that `std::move` returns a reference, and in general, the compiler can’t know to what object the function returns a reference to. So GCC 9 will issue a warning (when `-Wall` is in effect):

```
t.C:8:20: warning: moving a local object in a return statement prevents copy elision [-Wpessimizing-move]
8 | return std::move (t);
  |        ~~~~~~~~~~^~~
t.C:8:20: note: remove ‘std::move’ call
```

### -Wredundant-move

When the class object that a function returns is a function parameter, copy elision is not possible. However, when all the other conditions for the RVO are satisfied, C++ (as per the resolution of [Core Issue 1148](https://wg21.link/cwg1148)) says that a move operation should be used: overload resolution is performed as if the object were an rvalue (this is known as *two-stage overload resolution*). The parameter is an lvalue (because it has a name), but it’s about to be destroyed. Thus, the compiler ought to treat is as an rvalue.

For instance:

```C++
#include <iostream>
#include <utility>
struct T
{
	T()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~T()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	T(const T&) = delete;
	T(T&&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

T fn(T t)
{
	return t; // move used implicitly
}

int main()
{

	T t;
	T t2 = fn(std::move(t));
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

> NOTE: 上述程序的输出如下:
>
> ```C++
> T::T()
> T::T(T&&)
> T::T(T&&)
> T::~T()
> T::~T()
> T::~T()
> 
> ```
>
> 

Explicitly using `return std::move (t);` here would not be pessimizing—a move would be used in any case—it is merely redundant. The compiler can now point that out using the new warning `-Wredundant-move`, enabled by `-Wextra`:

```
r.C:8:21: warning: redundant move in return statement [-Wredundant-move]
8 | return std::move(t); // move used implicitly
  |        ~~~~~~~~~^~~
r.C:8:21: note: remove ‘std::move’ call
```

Because the GNU C++ compiler implements [Core Issue 1579](http://wg21.link/cwg1579), the following call to `std::move` is also redundant:

```
struct U { };
struct T { operator U(); };

U f()
{
  T t;
  return std::move (t);
}
```

Copy elision isn’t possible here because the types `T` and `U` don’t match. But, the rules for the implicit rvalue treatment are less strict than the rules for the RVO, and the call to `std::move` is not necessary.

There are situations where returning `std::move (expr)` makes sense, however. The rules for the **implicit move** require that the selected constructor take an **rvalue reference** to the returned object’s type. Sometimes that isn’t the case. For example, when a function returns an object whose type is a class derived from the class type the function returns. In that case, overload resolution is performed a second time, this time treating the object as an lvalue:

```C++
#include <iostream>
#include <utility>
struct U
{
	U()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	U(U &&u)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	U(const U &u)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
struct T
{
	operator U()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return U();
	}
};

U f()
{
	T t;
	return std::move(t);
}
int main()
{

	U u = f();
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out


```

While in general `std::move` is a great addition to the language, it’s not always appropriate to use it, and, sadly, the rules are fairly complicated. Fortunately, the compiler is able to recognize the contexts where a call to `std::move` would either prevent elision of a move or a copy—or would actually not make a difference—and warns appropriately. Therefore, we recommend enabling these warnings and perhaps adjusting the code base. The reward may be a minor performance gain and cleaner code. GCC 9 will be part of [Fedora 30](https://fedoraproject.org/wiki/Releases/30/Schedule), but you can try it right now on [Godbolt](https://gcc.godbolt.org/).

## stackoverflow [C++11 rvalues and move semantics confusion (return statement)](https://stackoverflow.com/questions/4986673/c11-rvalues-and-move-semantics-confusion-return-statement)

I'm trying to understand **rvalue references** and **move semantics** of C++11.

What is the difference between these examples, and which of them is going to do no vector copy?

### First example

```cpp
std::vector<int> return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return tmp;
}

std::vector<int> &&rval_ref = return_vector();
```

### Second example

```cpp
std::vector<int>&& return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return std::move(tmp);
}

std::vector<int> &&rval_ref = return_vector();
```

### Third example

```cpp
std::vector<int> return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return std::move(tmp);
}

std::vector<int> &&rval_ref = return_vector();
```

***comments*** : 

1、Please do not return local variables by reference, ever. An **rvalue reference** is still a reference.



### [A](https://stackoverflow.com/a/4986802)

#### First example

```cpp
#include <iostream>
#include <utility>
#include <vector>

std::vector<int> return_vector(void)
{
	std::vector<int> tmp { 1, 2, 3, 4, 5 };
	return tmp;
}

int main()
{
	std::vector<int> &&rval_ref = return_vector();
	for (auto i : rval_ref)
	{
		std::cout << i << "\n";
	}
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

The first example returns a temporary which is caught by `rval_ref`. That **temporary** will have its life extended beyond the `rval_ref` definition and you can use it as if you had caught it **by value**. This is very similar to the following:

```cpp
const std::vector<int>& rval_ref = return_vector();
```

except that in my rewrite you obviously can't use `rval_ref` in a non-const manner.

> NOTE: 
>
> 这个例子中的写法是可行的；
>
> 这段总结非常好，在# [Understanding the warning: binding r-value to l-value reference](https://stackoverflow.com/questions/34240794/understanding-the-warning-binding-r-value-to-l-value-reference) 也对这个问题进行了讨论，并且这篇文章讨论地非常透彻，有收录这篇文章。

#### Second example

```cpp
#include <iostream>
#include <utility>
#include <vector>

std::vector<int>&& return_vector(void)
{
	std::vector<int> tmp { 1, 2, 3, 4, 5 };
	return  std::move(tmp);
}

int main()
{
	std::vector<int> &&rval_ref = return_vector();
	for (auto i : rval_ref)
	{
		std::cout << i << "\n";
	}
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

> NOTE: 编译报错如下:
>
> ```C++
> main.cpp: In function 'std::vector<int>&& return_vector()':
> main.cpp:8:9: error: cannot bind rvalue reference of type 'std::vector<int>&&' to lvalue of type 'std::vector<int>'
>     8 |  return tmp;
>       |         ^~~
> ```
>
> 

In the second example you have created a run time error. `rval_ref` now holds a reference to the destructed `tmp` inside the function. With any luck, this code would immediately crash.

> NOTE: 显然，上述例子是不可行的，因为它:
>
> "Please do not return local variables by reference, ever. An **rvalue reference** is still a reference."
>
> 

#### Third example

```cpp
#include <iostream>
#include <utility>
#include <vector>

std::vector<int> return_vector(void)
{
	std::vector<int> tmp { 1, 2, 3, 4, 5 };
	return std::move(tmp);
}

int main()
{
	std::vector<int> &&rval_ref = return_vector();
	for (auto i : rval_ref)
	{
		std::cout << i << "\n";
	}
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out


```

Your third example is roughly equivalent to your first. The `std::move` on `tmp` is unnecessary and can actually be a performance pessimization as it will inhibit return value optimization.

The best way to code what you're doing is:

#### Best practice

```cpp
#include <iostream>
#include <utility>
#include <vector>

std::vector<int> return_vector(void)
{
	std::vector<int> tmp { 1, 2, 3, 4, 5 };
	return tmp;
}

int main()
{
	std::vector<int> rval_ref = return_vector();
	for (auto i : rval_ref)
	{
		std::cout << i << "\n";
	}
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out


```

I.e. just as you would in C++03. `tmp` is implicitly treated as an rvalue in the return statement. It will either be returned via **return-value-optimization** (no copy, no move), or if the compiler decides it can not perform RVO, then it [will use vector's move constructor to do the return](https://stackoverflow.com/questions/11088023/is-an-object-guaranteed-to-be-moved-when-it-is-returned). Only if RVO is not performed, and if the returned type did not have a **move constructor** would the **copy constructor** be used for the return.





## stackoverflow [Is an object guaranteed to be moved when it is returned?](https://stackoverflow.com/questions/11088023/is-an-object-guaranteed-to-be-moved-when-it-is-returned)