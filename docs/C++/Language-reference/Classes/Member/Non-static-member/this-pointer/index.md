# `this` pointer



## cppreference [`this` pointer](https://en.cppreference.com/w/cpp/language/this)

### Value category of `this` pointer

The keyword `this` is a [prvalue](https://en.cppreference.com/w/cpp/language/value_category) [expression](https://en.cppreference.com/w/cpp/language/expressions) whose value is the address of the [implicit object parameter](https://en.cppreference.com/w/cpp/language/overload_resolution) (object on which the non-static member function is being called).

> NOTE: 在`C++\Language-reference\Expressions\Value-categories\Value-categories.md`中对此进行了解释。
>
> 在cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions) 中:
>
> unlike cv-qualification, ref-qualification does not change the properties of the [this](https://en.cppreference.com/w/cpp/language/this) pointer: within a rvalue ref-qualified function, `*this` remains an **lvalue expression**.
>
> `*this`是 **lvalue expression**是因为在cppreference [Value categories](https://en.cppreference.com/w/cpp/language/value_category) 中有这样的规则:
>
> lvalue:
>
> `*p`, the built-in [indirection](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_indirection_operator) expression;



### CV of `this` pointer

The type of `this` in a member function of class `X` is `X*` (pointer to X). If the member function is [cv-qualified](https://en.cppreference.com/w/cpp/language/member_functions), the type of `this` is `cv X*` (pointer to identically cv-qualified X). Since constructors and destructors cannot be cv-qualified, the type of `this` in them is always `X*`, even when constructing or destroying a const object.

> NOTE: 关于CV of `this` pointer，在`./Function-member`中进行了更加详细的说明



### `this` is dependent name

In class templates, `this` is a [dependent expression](https://en.cppreference.com/w/cpp/language/dependent_name), and explicit `this->` may be used to force another expression to become **dependent**.

> NOTE: 关于dependent expression，参见`C++\Language-reference\Basic-concept\Organization\Name-lookup\Dependent-name-lookup`。

```C++
template<typename T>
struct B
{
	int var;
};

template<typename T>
struct D: B<T>
{
	D()
	{
		// var = 1;    // error: 'var' was not declared in this scope
		this->var = 1; // ok
	}
};

int main()
{
	D<int> d;
}
// g++ test.cpp

```



> NOTE: 关于`this` is dependent name，参见`C++\Language-reference\Basic-concept\Organization\Name-lookup\Dependent-name-lookup\index.md`，其中有着较好的描述。