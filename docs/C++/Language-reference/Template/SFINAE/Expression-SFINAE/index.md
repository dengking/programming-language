# Expression SFINAE

## cppreference [Expression SFINAE](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE)

A common idiom is to use expression SFINAE on the return type, where the expression uses the **comma operator**, whose left subexpression is the one that is being examined (cast to `void` to ensure the **user-defined operator comma** on the returned type is not selected), and the right subexpression has the type that the function is supposed to return.

> NOTE: “cast to `void` to ensure the **user-defined operator comma** on the returned type is not selected”是利用[Discarded-value expressions](https://en.cppreference.com/w/cpp/language/expressions#Discarded-value_expressions)的性质。

```c++
#include <iostream>
 
// this overload is always in the set of overloads
// ellipsis parameter has the lowest ranking for overload resolution
void test(...)
{
    std::cout << "Catch-all overload called\n";
}
 
// this overload is added to the set of overloads if
// C is a reference-to-class type and F is a pointer to member function of C
template <class C, class F>
auto test(C c, F f) -> decltype((void)(c.*f)(), void())
{
    std::cout << "Reference overload called\n";
}
 
// this overload is added to the set of overloads if
// C is a pointer-to-class type and F is a pointer to member function of C
template <class C, class F>
auto test(C c, F f) -> decltype((void)((c->*f)()), void())
{
    std::cout << "Pointer overload called\n";
}
 
struct X { void f() {} };
 
int main(){
  X x;
  test( x, &X::f);
  test(&x, &X::f);
  test(42, 1337);
}
// g++ --std=c++11 test.cpp
```



## open-std [Solving the SFINAE problem for expressions](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2634.html)

```c++
#include <iostream>

template<int I> struct A
{
};

char xxx(int)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}
char xxx(float)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template<class T>
A<sizeof(xxx((T) 0))> f(T)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main()
{
	f(1);
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序的输出：
>
> ```c++
> A<sizeof (xxx((T)(0)))> f(T) [with T = int]
> ```
>
> 

### The proposed solution

```c++
#include <iostream>

struct X
{
};
struct Y
{
	Y(X)
	{
	}
};

template<class T> auto f(T t1, T t2) -> decltype(t1 + t2) // #1
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}
X f(Y, Y)  // #2
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}
int main()
{
	X x1, x2;
	X x3 = f(x1, x2);  // deduction fails on #1 (cannot add X+X), calls #2
}
// g++ --std=c++11 test.cpp
```

> NOTE： 上述程序的输出:
>
> ```
> X f(Y, Y)
> ```
>
> 

## stackoverflow [What is “Expression SFINAE”?](https://stackoverflow.com/questions/12654067/what-is-expression-sfinae)

[A](https://stackoverflow.com/a/12654277)

Expression SFINAE is explained quite well in the paper you linked, I think. It's [SFINAE](http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) on expressions. If the expression inside `decltype` isn't valid, well, kick the function from the VIP lounge（休息室） of overloads. You can find the normative wording at the end of this answer.

A note on VC++: They didn't implement it *completely*. On simple expressions, it might work, but on others, it won't. See a discussion in the comments [on this answer](https://stackoverflow.com/a/6194623/500104) for examples that fail. To make it simple, this won't work:

```cpp
#include <iostream>

// catch-all case
void test(...)
{
	std::cout << "Couldn't call\n";
}

// catch when C is a reference-to-class type and F is a member function pointer
template<class C, class F>
auto test(C c, F f) -> decltype((c.*f)(), void()) // 'C' is reference type
{
	std::cout << "Could call on reference\n";
}

// catch when C is a pointer-to-class type and F is a member function pointer
template<class C, class F>
auto test(C c, F f) -> decltype((c->*f)(), void()) // 'C' is pointer type
{
	std::cout << "Could call on pointer\n";
}

struct X
{
	void f()
	{
	}
};

int main()
{
	X x;
	test(x, &X::f);
	test(&x, &X::f);
	test(42, 1337);
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序的输出如下:
>
> ```
> Could call on reference
> Could call on pointer
> Couldn't call
> ```
>
> 

With `Clang`, this outputs the expected:

> Could call with reference
> Could call with pointer
> Couldn't call

With MSVC, I get... well, a compiler error:

```c++
1>src\main.cpp(20): error C2995: ''unknown-type' test(C,F)' : function template has already been defined
1>          src\main.cpp(11) : see declaration of 'test'
```

It also seems that GCC 4.7.1 isn't quite up to the task:

```c++
source.cpp: In substitution of 'template decltype ((c.*f(), void())) test(C, F) [with C = X*; F = void (X::*)()]':
source.cpp:29:17:   required from here
source.cpp:11:6: error: cannot apply member pointer 'f' to 'c', which is of non-class type 'X*'
source.cpp: In substitution of 'template decltype ((c.*f(), void())) test(C, F) [with C = int; F = int]':
source.cpp:30:16:   required from here
source.cpp:11:6: error: 'f' cannot be used as a member pointer, since it is of type 'int'
```

A common use of Expression SFINAE is when defining traits, like a trait to check if a class sports a certain member function:

```cpp
#include <iostream>
#include <type_traits>
#include <vector>

struct has_member_begin_test
{
	template<class U>
	static auto test(U* p) -> decltype(p->begin(), std::true_type());
	template<class >
	static auto test(...) -> std::false_type;
};

template<class T>
struct has_member_begin
: decltype(has_member_begin_test::test<T>(0))
{
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << has_member_begin<int>::value << std::endl;
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序输出如下：
>
> ```
> false
> ```



[Live example.](http://liveworkspace.org/code/45043173e3b62ee33deaeec3e609516a) (Which, surprisingly, works again on GCC 4.7.1.)

See also [this answer of mine](https://stackoverflow.com/a/9154394/500104), which uses the same technique in another environment (aka without traits).

------

Normative wording:

```
§14.8.2 [temp.deduct]
```

> p6 **At certain points in the template argument deduction process it is necessary to take a function type that makes use of template parameters and replace those template parameters with the corresponding template arguments.** This is done at the beginning of template argument deduction when any explicitly specified template arguments are substituted into the function type, and **again at the end of template argument deduction when any template arguments that were deduced or obtained from default arguments are substituted**.
>
> p7 **The substitution occurs in all types and expressions** that are used in the function type and in template parameter declarations. **The expressions include not only constant expressions** such as those that appear in array bounds or as nontype template arguments **but also general expressions** (i.e., non-constant expressions) **inside** `sizeof`, **`decltype`**, and other contexts that allow non-constant expressions.
>
> p8 If a substitution results in an invalid type or expression, type deduction fails. An invalid type or expression is one that would be ill-formed if written using the substituted arguments. [...]







## Examples

### Use expression SFINAE on the return type

#### cppreference [Expression SFINAE](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE)

其中给出的例子非常具有代表性。

### stackoverflow [What is decltype with two arguments?](https://stackoverflow.com/questions/16044514/what-is-decltype-with-two-arguments)



[A](https://stackoverflow.com/a/16044577)

```c++
#include <type_traits>

int main()
{
	static_assert(std::is_same<decltype(42, 3.14), double>::value, "Will not fire");
}

```

> NOTE: `42, 3.14`中的`,`为comma operator。

