# Template argument deduction



## cppreference [Template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction)



Template argument deduction takes place after the function template [name lookup](https://en.cppreference.com/w/cpp/language/lookup) (which may involve [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)) and before [template argument substitution](https://en.cppreference.com/w/cpp/language/function_template#Template_argument_substitution) (which may involve [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)) and [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution).

> NOTE: 需要将这些流程给串联起来

### Deduction from a function call



If there are multiple parameters, each `P/A` pair is deduced **separately** and the deduced template arguments are then **combined**. If deduction fails or is ambiguous for any `P/A` pair or if different pairs yield different deduced template arguments, or if any template argument remains neither deduced nor explicitly specified, compilation fails.

> NOTE: 上面这段话非常重要，它描述了deduction的机制



#### Non-deduced contexts

> NOTE: 关于non-deduced contexts，是我在阅读如下文章时碰到的：
>
> bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)，其中对`std::forward`的实现的分析中有如下描述：
>
> > The type `T` is not deduced, therefore we had to specify it when **using** `std::forward`.
>
> thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)，其中对`std::forward`的实现的分析中有如下描述：
>
> > Another thing I want to mention is the use of `std::remove_reference<T>`. In fact, if you think about it, `forward` could do without it. **Reference collapsing** does the job already, so `std::remove_reference<T>` is superfluous（多余的）. It's there to turn the `T& t` into a **non-deducing context** (according to the C++ standard, section 14.8.2.5), thus forcing us to explicitly specify the **template parameter** when calling `std::forward`.
>
> 这让我思考：如何来强制不进行deduction。
>
> 在某些情况下，programmer是不想启用deduction的，所以C++提供了给了programmer这种控制能力，这是C++灵活性的体现。
>
> ## Disable argument-based template parameter deduction for functions
>
> stackoverflow [Better way to disable argument-based template parameter deduction for functions?](https://stackoverflow.com/questions/37737487/better-way-to-disable-argument-based-template-parameter-deduction-for-functions)
>
> You can do it by putting T in non deducible context (to the left of `::`), and use [std::common_type](http://en.cppreference.com/w/cpp/types/common_type) from `<type_traits>`.
>
> example:
>
> ```cpp
> template <typename T> void f(typename std::common_type<T>::type obj) {std::cout << obj;}
> ```

In the following cases, the types, templates, and non-type values that are used to compose `P` do not participate in **template argument deduction**, but instead use the **template arguments** that were either deduced elsewhere or explicitly specified. If a template parameter is used only in **non-deduced contexts** and is not explicitly specified, template argument deduction fails.

1) The *nested-name-specifier* (everything to the left of the scope resolution operator `::`) of a type that was specified using a [qualified-id](name.html#Qualified_identifiers):

##### Example 1

```c++
#include <vector>
#include <complex>
// the identity template, often used to exclude specific arguments from deduction
// (available as std::type_identity as of C++20)
template<typename T> struct identity
{
	typedef T type;
};
template<typename T> void bad(std::vector<T> x, T value = 1)
{
}
template<typename T> void good(std::vector<T> x, typename identity<T>::type value = 1)
{
}

int main()
{
	std::vector<std::complex<double>> x;
	// bad(x, 1.2);  // P1 = std::vector<T>, A1 = std::vector<std::complex<double>>
				  // P1/A1: deduced T = std::complex<double>
				  // P2 = T, A2 = double
				  // P2/A2: deduced T = double
				  // error: deduction fails, T is ambiguous
	good(x, 1.2); // P1 = std::vector<T>, A1 = std::vector<std::complex<double>>
				  // P1/A1: deduced T = std::complex<double>
				  // P2 = identity<T>::type, A2 = double
				  // P2/A2: uses T deduced by P1/A1 because T is to the left of :: in P2
				  // OK: T = std::complex<double>
}

```

> NOTE: 上述`identity<T>::type`就是 **non-deduced contexts** 。



##### Example 2

关于此的另外一个例子就是`std::forward`，这在bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)、thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)中都有描述；



2) The expression of a decltype-specifier(since C++11):

> NOTE: 关于这一点，是在阅读stackoverflow [How does `void_t` work](https://stackoverflow.com/questions/27687389/how-does-void-t-work) 时，其中提及的。这一点，收录在`C++\Language-reference\Basic-concept\Type-system\decltype\decltype.md` 中。

#### Deduction from a type



### Other contexts

