# thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) 

## A simple problem and a solution

```c++
#include <iostream>

template<typename T>
struct Base {
    void f() {
        std::cerr << "Base<T>::f\n";
    }
};

template<typename T>
struct Derived : Base<T> {
    void g() {
        std::cerr << "Derived<T>::g\n  ";
        f();
    }
};

int main() {
    Derived<int> D;
}
// g++ test.cpp

```

The intention of `Derived<T>::g` is to call `Base<T>::f`, but what the compiler does instead is produce this error:

```c++
: In member function ‘void Derived<T>::g()’:
:18:10: error: there are no arguments to ‘f’ that depend on a template parameter, so a declaration of ‘f’ must be available
:18:10: note: (if you use ‘-fpermissive’, G++ will accept your code, but allowing the use of an undeclared name is deprecated)
```

> NOTE: 
>
> 1、中文如下:
>
> ```c++
> test.cpp: 在成员函数‘void Derived<T>::g()’中:
> test.cpp:16:5: 错误：‘f’的实参不依赖模板参数，所以‘f’的声明必须可用 [-fpermissive]
> f();
> ^
> test.cpp:16:5: 附注：(如果您使用‘-fpermissive’，G++ 会接受您的代码，但是允许使用未定义的名称是不建议使用的风格)
> 
> ```
>
> 2、由于`f`没有在 `struct Derived` 中定义，因此，compiler会在 `Base<T>` 中进行查找，而由于 `Base<T>` 是一个class template，因此，`f`就是一个dependent name，而上述程序的写法是不符合dependent name的用法的。

First, let's see how to fix this. It's easy. All you have to do is to make the compiler understand that the call `f` depends on the template parameter `T`. A couple of ways to do this are replacing `f()` with `Base<T>::f()`, or with `this->f()` (since `this` is implicitly dependent on `T`). For example:

```c++
#include <iostream>

template<typename T> struct Base
{
	void f()
	{
		std::cerr << "Base<T>::f\n";
	}
};

template<typename T> struct Derived: Base<T>
{
	void g()
	{
		std::cerr << "Derived<T>::g\n  ";
		this->f();
	}
};

int main()
{
	Derived<float> df;
	df.g();

	Derived<int> di;
	di.g();
	return 0;
}
// g++ test.cpp

```

`main` instantiates two `Derived` objects, parametrized for different types, for reasons that will soon become apparent. This code compiles without errors and prints:

```C++
Derived<T>::g
  Base<T>::f
Derived<T>::g
  Base<T>::f
```

## 为什么需要dependent name lookup？

Problem fixed. Now, let's understand what's going on. Why does the compiler need an explicit specification for which `f` to call? Can't it figure out on its own that we want it to call `Base<T>::f`? It turns out it can't, because this isn't correct in the general case. Suppose that a specialization of the `Base` class is later created for `int`, and it also defines `f`:

```c++
#include <iostream>

template<typename T> struct Base
{
	void f()
	{
		std::cerr << "Base<T>::f\n";
	}
};

template<typename T> struct Derived: Base<T>
{
	void g()
	{
		std::cerr << "Derived<T>::g\n  ";
		this->f();
		// Base<T>::f(); // 这种写法也是允许的
	}
};

template<>
struct Base<int>
{
	void f()
	{
		std::cerr << "Base<int>::f\n";
	}
};

int main()
{
	Derived<float> df;
	df.g();

	Derived<int> di;
	di.g();
	return 0;
}
// g++ test.cpp

```

With this **specialization** in place, the `main` from the sample above would actually print:

```c++
Derived<T>::g
  Base<T>::f
Derived<T>::g
  Base<int>::f
```

> NOTE: 
>
> 1、通过上述输出可以看出，`struct Base<int>::f()` 被调用了。
>
> 2、上述例子，体现了specialization and inheritance
>
> 

This is the correct behavior. The `Base` template has been specialized for `int`, so it should be used for **inheritance** when `Derived<int>` is required. But how does the compiler manage to figure it out? After all, `Base<int>` was defined *after* `Derived`!



## Two-phase name lookup(如何实现)

To make this work, the C++ standard defines a "two-phase name lookup" rule for names in templates. Names inside templates are divided to two types:

| type            | explanation                                                  | example |
| --------------- | ------------------------------------------------------------ | ------- |
| *Dependent*     | names that depend on the template parameters but aren't declared within the template. |         |
| *Non-dependent* | names that don't depend on the template parameters, plus the name of the template itself and names declared within it. |         |

When the compiler tries to resolve some name in the code, it first decides whether the name is **dependent** or not, and the resolution process stems from this distinction(解析过程就是根据这种区别进行的). While **non-dependent names** are resolved "normally" - when the template is defined, the resolution for **dependent names** happens at the point of the **template's instantiation**. This is what ensures that a **specialization** can be noticed correctly in the example above(dependent name lookup保证能够找到**specialization**).

> NOTE: 
>
> 一、关于template instantiation，参见`C++\Language-reference\Template\Implementation\index.md`

### 非常好的解释

Now, back to our original problem. Why doesn't the compiler look `f` up in the base class? First, notice that in the call to `f()` in the first code snippet, `f` is a **non-dependent name**. So it must be resolved at the point of the **template's definition**. At that point, the compiler still doesn't know what `Base<T>::f` is, because it can be **specialized** later. So it doesn't look names up in the base class, but only in the enclosing scope. Since there's no `f` in the enclosing scope, the compiler complains.

> NOTE:
>
> 1、"At that point, the compiler still doesn't know what `Base<T>::f` is, because it can be **specialized** later. So it doesn't look names up in the base class, but only in the enclosing scope. " 
>
> 理解这段话的前提是对C++ compiler编译 template的流程有一个完整的认知，参见 `Compile-template` 章节

On the other hand, when we explicitly make the lookup of `f` dependent by calling it through `this->`, the lookup rule changes. Now `f` is resolved at the point of the **template's instantiation**, where the compiler has full understanding of the base class and can resolve the name correctly.

## Disambiguating dependent type names

> NOTE: 
>
> 一、“disambiguate” 即 “消除歧义”

I've mentioned above that to fix the problem and make the lookup of `f` dependent, we can either say `this->f()` or `Base<T>::f()`. While this works for **identifiers** like **member names**, it doesn't work with types. Consider this code snippet:

> NOTE: 
>
> 一、dependent name可以有如下三种:
>
> | dependent name | example                                                      | how to disambiguate                                          |
> | -------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | identifier     | 上面例子中的`f`是identifier                                  | compiler的default behavior将dependent name当做identifier     |
> | type           | 下面例子中的`MyType`是type                                   | 通过keyword `typename`来告诉compiler，这个name表示的是type   |
> | template       | “Disambiguating dependent template names”中的例子中的`foo_method`是 function template | 通过keyword `template`来告诉compiler，这个name表示的是template |
>
> 

```c++
#include <iostream>

template<typename T> struct Base
{
	typedef int MyType;
};

template<typename T> struct Derived: Base<T>
{
	void g()
	{
		// A. error: ‘MyType’ was not declared in this scope
		// MyType k = 2;

		// B. error: need ‘typename’ before ‘Base<T>::MyType’ because
		// ‘Base<T>’ is a dependent scope
		// Base<T>::MyType k = 2;

		// C. works!
		typename Base<T>::MyType k = 2;

		std::cerr << "Derived<T>::g --> " << k << "\n";
	}
};

int main()
{
	Derived<float> df;
	df.g();
	return 0;
}
// g++ test.cpp

```

Three attempts are shown to declare a **local variable** `k` of type `MyType`. The first two are commented out because they result in compile errors. (A) should be obvious by now - since `MyType` is non-dependent, it can't be found in the **base class** - same problem as before.

### 为什么需要 `typename`

But why doesn't (B) work? Well, because `Base<T>` can be specialized, so the compiler can't be sure whether `MyType` is a **type** or not. A **specialization** can easily declare a method called `MyType` instead of it being a **type**. And neither can the compiler delay this decision until the **instantiation point**, because whether `MyType` is a type or not affects how the rest of the definition is *parsed*. So we must tell the compiler explicitly, at the point of definition, whether `MyType` is a type or not. It turns out that the default is "not a type", and we must precede the name with `typename` to tell the compiler it *is* a type. This is stated in the `C++` standard, section 14.6:

> A name used in a template declaration or definition and that is dependent on a template-parameter is assumed not to name a type unless the applicable name lookup finds a type name or the name is qualified by the keyword `typename`.



> NOTE: 因为 `MyType` 是否是一个类型会影响compiler对后面代码的解析，因此，compiler也不能将此决定延迟到**instantiation point**，所以我们必须在定义的时候明确地告诉编译器`MyType`是否是一个类型。结果是，默认值是“不是类型”，我们必须在名称前面加上`typename`，以告诉编译器它*是*类型。这是在“c++”标准14.6节中规定的:

## Disambiguating dependent template names

While we're at it, the following is yet another example of explicit disambiguation that is sometimes required to guide the compiler when templates and specializations are involved:

```c++
struct Foo {
    template<typename U>
    static void foo_method()
    {
    }
};

template<typename T> void func(T* p) {
    // A. error: expected primary-expression before ‘>’ token
    // T::foo_method<T>();

    // B. works!
    T::template foo_method<T>();
}
```



The first attempt to call `T::foo_method` fails - the compiler can't parse the code. As explained before, when a **dependent name** is encountered, it is assumed to be some sort of **identifier** (such as a function or variable name). Previously we've seen how to use the `typename` keyword to explicitly tell the compiler that it deals with a **type**.

So in declaration (A) above can't be parsed, because the compiler assumes `foo_method` is just a member function and interprets the `<` and `>` symbols as comparison operators. But `foo_method` is a template, so we have to notify the compiler about it. As declaration (B) demonstrates, this can be done by using the keyword `template`.

> NOTE:
>
> 1、关于primary-expression，参见`C++\Language-reference\Expressions\Expressions.md`。
>
> 2、写法A中，compiler将`T::foo_method<T>()`中的`<`解析为less than operator，将`>`解析为more than operator，而C++规定: 
>
> > The operands of any operator may be other expressions or primary expressions
>
> 按照C++的规定，`T::foo_method<T`不是一个primary expression，所以compiler explain。
>
>  实际上`foo_method`是一个function template。
>
> 3、`foo_method` 是一个dependent name

## Resources

The following resources have been helpful in the preparation of this article and contain additional information if you're interested to dig deeper:

- [Name lookup](http://gcc.gnu.org/onlinedocs/gcc/Name-lookup.html) in the g++ docs
- [C++ templates FAQ](http://womble.decadent.org.uk/c++/template-faq.html)
- [C++ FAQ Lite](http://www.parashift.com/c++-faq-lite/templates.html), section 18
- C++11 standard, working draft N3242, section 14.6

