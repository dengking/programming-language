# ADL

ADL是compiler的一种行为，可以将compiler的lookup分为两种：

- ADL
- ordinary lookup

## cppreference [ADL](https://en.cppreference.com/w/cpp/language/adl) 

Argument-dependent lookup, also known as ADL, or Koenig lookup, is the set of rules for looking up the **unqualified function names** in [function-call expressions](operator_other.html), including implicit function calls to [overloaded operators](operators.html). These **function names** are looked up in the namespaces of their arguments in addition to the scopes and namespaces considered by the usual [unqualified name lookup](lookup.html).

> NOTE: 简而言之：在lookup function name的时候，考虑如下namespace：
>
> - ADL: namespace of their argument
> - ordinary lookup: scope and namespace considered by the usual [unqualified name lookup](lookup.html)（可能包含当前scope）
>
> 思考：上述两者，谁先谁后？
>
> 通过`swap`的例子来看，应该是首先进行ADL，然后进行ordinary lookup。在cpppatterns [Range-based algorithms](https://cpppatterns.com/patterns/range-based-algorithms.html)中对此进行了说明：
>
> On [lines 12–13](https://cpppatterns.com/patterns/range-based-algorithms.html#line12), we call `begin` and `end` on the range to get the respective iterators to the beginning and end of the range. We use using-declarations on [lines 7–8](https://cpppatterns.com/patterns/range-based-algorithms.html#line7) to allow these calls to be found via [argument-dependent lookup](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup) before using the standard [`std::begin`](http://en.cppreference.com/w/cpp/iterator/begin) and [`std::end`](http://en.cppreference.com/w/cpp/iterator/end) functions. With these iterators, we can now implement the algorithm over the elements between them.

### Example

```c++
#include <iostream>
int main()
{
    std::cout << "Test\n"; // There is no operator<< in global namespace, but ADL
                           // examines std namespace because the left argument is in
                           // std and finds std::operator<<(std::ostream&, const char*)
    operator<<(std::cout, "Test\n"); // same, using function call notation
 
    // however,
    std::cout << endl; // Error: 'endl' is not declared in this namespace.
                       // This is not a function call to endl(), so ADL does not apply
 
    endl(std::cout); // OK: this is a function call: ADL examines std namespace
                     // because the argument of endl is in std, and finds std::endl
 
    (endl)(std::cout); // Error: 'endl' is not declared in this namespace.
                       // The sub-expression (endl) is not a function call expression

```

> NOTE: 上述例子能够极好的帮助我们理解ADL：
>
> `std::cout << endl`无法编译通过的原因是：ADL是用于lookup function的，unqualified的`endl`，没有在当前scope中声明，所以它无法被找到。

### Details

> NOTE: 原文的一段，冗长且没有例子，难以阅读；

Otherwise, for every argument in a function call expression its type is examined to determine the *associated set of namespaces and classes* that it will add to the lookup.

> NOTE: 即考虑argument的namespace

### Notes

Because of **argument-dependent lookup**, **non-member functions** and **non-member operators** defined in the same namespace as a **class** are considered part of the **public interface** of that class (if they are found through ADL) [[1\]](adl.html#cite_note-1).

> NOTE: 这非常重要，从interface的角度来理解。

ADL is the reason behind the established idiom for swapping two objects in generic code:

```c++
using std::swap;
swap(obj1, obj2);
```

because calling [std::swap](../algorithm/swap.html)`(obj1, obj2)` directly would not consider the user-defined `swap()` functions that could be defined in the same namespace as the types of `obj1` or `obj2`, and just calling the unqualified `swap(obj1, obj2)` would call nothing if no user-defined overload was provided. In particular, [std::iter_swap](../algorithm/iter_swap.html) and all other standard library algorithms use this approach when dealing with [*Swappable*](../named_req/Swappable.html) types.

**Name lookup rules** make it impractical to declare operators in global or user-defined namespace that operate on types from the `std` namespace, e.g. a custom `operator>>` or `operator+` for [std::vector](../container/vector.html) or for [std::pair](../utility/pair.html) (unless the element types of the vector/pair are user-defined types, which would add their namespace to ADL). Such operators would not be looked up from template instantiations, such as the standard library algorithms. See [dependent names](dependent_name.html) for further details.

> NOTE: 上面这一段话，没有搞懂

ADL can find a [friend function](friend.html) (typically, an overloaded operator) that is defined entirely within a class or class template, even if it was never declared at namespace level.

```c++
template<typename T>
struct number
{
    number(int) { }
    friend number gcd(number x, number y) { return 0; }; // definition within
                                                         // a class template
};
// unless a matching declaration is provided gcd is an invisible (except through ADL)
// member of this namespace
void g() {
    number<double> a(3), b(4);
    a = gcd(a,b); // finds gcd because number<double> is an associated class,
                  // making gcd visible in its namespace (global scope)
//  b = gcd(3,4); // Error; gcd is not visible
}

int main()
{
	g();
}
```

> NOTE: `g++ test.cpp`

(until C++20)

Although a function call can be resolved through **ADL** even if **ordinary lookup** finds nothing, a function call to a [function template](function_template.html) with explicitly-specified template arguments requires that there is a declaration of the **template** found by **ordinary lookup** (otherwise, it is a syntax error to encounter an unknown name followed by a less-than character)

> NOTE: lookup template和lookup function call

```c++
namespace N1 {
  struct S {};
  template<int X> void f(S) {}
}
namespace N2 {
  template<class T> void f(T t) {}
}
void g(N1::S s) {
  f<3>(s);      // Syntax error until C++20 (unqualified lookup finds no f)
  N1::f<3>(s);  // OK, qualified lookup finds the template 'f'
  N2::f<3>(s);  // Error: N2::f does not take a non-type parameter
                //        N1::f is not looked up because ADL only works
                //              with unqualified names
  using N2::f;
  f<3>(s); // OK: Unqualified lookup now finds N2::f
           //     then ADL kicks in because this name is unqualified
           //     and finds N1::f
}
int main()
{
g(N1::S());
}
```



In the following contexts **ADL-only lookup** (that is, lookup in associated namespaces only) takes place:

- the lookup of non-member functions `begin` and `end` performed by the [range-for](range-for.html) loop if member lookup fails
- the [dependent name lookup](dependent_name.html#Lookup_rules) from the point of template instantiation.
- the lookup of non-member function `get` performed by [structured binding declaration](structured_binding.html) for tuple-like types (since C++17)

## [What is “Argument-Dependent Lookup” (aka ADL, or “Koenig Lookup”)?](https://stackoverflow.com/questions/8111677/what-is-argument-dependent-lookup-aka-adl-or-koenig-lookup)

> NOTE: 这篇文章介绍的更多一些



http://www.gotw.ca/gotw/030.htm

https://quuxplusone.github.io/blog/2019/04/26/what-is-adl/

https://stackoverflow.com/questions/18086292/is-bjarne-wrong-about-this-example-of-adl-or-do-i-have-a-compiler-bug



## Practice

### Idiom based on ADL

- swap idiom
- [Range-based algorithms](https://cpppatterns.com/patterns/range-based-algorithms.html)

### `get_rtti`

我已经使用了c++ ADL了，`get_rtti`就是一个例子。下面是简化的代码：

```c++
#include <string>
#include <iostream>

/**
 *  * 结构体1
 *   */
struct MyStruct1
{
};

/**
 *  * 结构体
 *   */
struct MyStruct2
{
};

/**
 *  * 结构体类型信息
 *   */
typedef std::string CStructRtti;

/**
 *  * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct1* f)
{
	return "MyStruct1";
}

/**
 *  * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct2* f)
{
	return "MyStruct2";
}

/**
 *  * 模板函数，根据结构体类型来获取结构体类型信息，它的实现是派发到上述的根据结构体对象来获取结构体类型休息
 *   * @return
 *    */
template<typename StructT>
CStructRtti GetRttiByType()
{
	StructT Tmp = StructT();
	return GetRtti(&Tmp);
}

class Utility
{
public:
	template<typename StrucT>
	static CStructRtti GetRtti()
	{
		return GetRttiByType<StrucT>();
	}

};
/**
 *  * 该namespace内部和外部的结构基本一致
 *   * @param f
 *    * @return
 *     */
namespace NS
{
/**
 *  * 结构体1
 *   */
struct MyStruct1
{
};

/**
 *  * 结构体
 *   */
struct MyStruct2
{
};

/**
 *  * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct1* f)
{
	return "NS::MyStruct1";
}

/**
 *  * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct2* f)
{
	return "NS::MyStruct2";
}


class Utility
{
public:
	template<typename StrucT>
	static CStructRtti GetRtti()
	{
		return GetRttiByType<StrucT>();
	}

};
}

int main()
{
	std::cout<<Utility::GetRtti<MyStruct1>()<<std::endl;
	std::cout<<NS::Utility::GetRtti<NS::MyStruct1>()<<std::endl;
}


```

