# cppreference [Parameter pack(since C++11)](https://en.cppreference.com/w/cpp/language/parameter_pack)

A **template parameter pack** is a **template parameter** that accepts zero or more **template arguments** (non-types, types, or templates).

> NOTE: 需要注意的是:
>
> 1) 它的名称是**parameter** pack，它是**parameter**，这种语法是告诉compiler，这个parameter可以接受多个argument的，它是一个**pack**，这是template parameter pack的内涵。
>
> 2) 既然是parameter，它可以用于如下template:
>
> - [alias template](https://en.cppreference.com/w/cpp/language/type_alias)
> - [class template](https://en.cppreference.com/w/cpp/language/class_template)
> - [function template](https://en.cppreference.com/w/cpp/language/function_template) parameter lists
> - [variable template](https://en.cppreference.com/w/cpp/language/variable_template) 
>
> 显然，一旦使用了parameter pack，那么它就是variadic template。
>
> 3) 既然有pack，那么肯定有对应的unpack过程，在原文中，将unpack过程称为“Parameter pack expansion”。
>
> 

A template with at least one parameter pack is called a *variadic template*.



## Syntax

Template parameter pack (appears in [alias template](https://en.cppreference.com/w/cpp/language/type_alias), [class template](https://en.cppreference.com/w/cpp/language/class_template), [variable template](https://en.cppreference.com/w/cpp/language/variable_template) and [function template](https://en.cppreference.com/w/cpp/language/function_template) parameter lists)

> NOTE: 在下面可以看到，上述三种类型的argument(non-types, types, or templates)的对应的template parameter pack的声明方式是不同的。

|      | Syntax                                                       | Explanation                                                  |                                                         |
| ---- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------- |
| 1    | `type ... Args(optional)`                                    | A non-type **template parameter pack** with an optional name | `template<std::size_t... Is>`                           |
| 2    | `typename|class ... Args(optional)`                          | A type **template parameter pack** with an optional name     |                                                         |
| 3    | `template < parameter-list > typename | class ... Args(optional)` | A **template template parameter pack** with an optional name |                                                         |
| 4    | `Args ... args(optional)`                                    | A **function parameter pack** with an optional name          |                                                         |
| 5    | `pattern ...`                                                | **Parameter pack expansion**:<br>expands to comma-separated list of zero or more `pattern`s. <br>Pattern must include at least one parameter pack. | 对pack中的每个元素都执行pattern操作，后面会进行详细介绍 |

> NOTE: 
>
> 关于 expand的grammar，在下面的pack expansion章节会进行介绍；
>
> 关于(3)的例子，参见 [joboccara](https://github.com/joboccara)/**[NamedType](https://github.com/joboccara/NamedType)** ，其中有着运用。

## Explanation(说明)

### Variadic class template

```C++
template<class ... Types>
struct Tuple
{
};
int main()
{
	Tuple<> t0;           // Types contains no arguments
	Tuple<int> t1;        // Types contains one argument: int
	Tuple<int, float> t2; // Types contains two arguments: int and float
	// Tuple<0> error;       // error: 0 is not a type

}
```

### Variadic function template 



```C++

template<class ... Types>
void f(Types ... args)
{

}
int main()
{
	f();       // OK: args contains no arguments
	f(1);       // OK: args contains one argument: int
	f(2, 1.0);       // OK: args contains two arguments: int and double
}

```



## Pack expansion

> NOTE: 
>
> ## "parameter pack是linear structure"
>
> 之所以强调此的原因是: 我们应该将parameter pack看做是一个linear structure，后续对它的computation都是基于traverse on linear structure。
>
> ## 对Pack expansion的解释
>
> 将pattern看做是一个function/operation，则pack expansion的过程其实就是对其中的所有的parameter pack(是一个linear structure)**顺序**执行这个function/operation，其实这个过程非常类似于functional programming中的apply: apply a function(pattern) to linear structure(s)；显然这个function/pattern的入参个数需要和出现在pack expansion中的parameter pack的个数相同。
>
> ### grammar
>
> 按照上述描述，pack expansion的grammar: `pattern ...`中，将pattern放在前面，因此它可以简记为: apply pattern to linear structure。
>
> ### 结果
>
> expansion的结果: comma-separated list of zero or more `pattern`s。
>
> ## Parameter and argument
>
> 正如函数有parameter和argument，pack同样有: 
>
> 1、type/parameter pack
>
> 2、argument pack
>
> 下面介绍的operation一般作用于argument pack
>
> ## Operation
>
> 可以对parameter pack这些的一些常见pattern/operation: 
>
> - `len` 查询它的长度，参见[sizeof... operator](https://en.cppreference.com/w/cpp/language/sizeof...)
>
> - index 索引其中的某个元素
> - fold，参见[fold expression(since C++17)](https://en.cppreference.com/w/cpp/language/fold)
> - apply
>
> ### Operaton on multiple pack
>
> 对multiple pack可能的expand情况: 
>
> 1、expand multiple pack simultaneously
>
> 将一个pattern共识expand到multiple pack
>
> 2、nesting expand: 先expand inner，然后expand outer
>
> 嵌套: 内部一个expand，然后外部一个expand
>
> 对于上述的两种情况，下面都有对应的例子。

A **pattern** followed by an **ellipsis**, in which the name of at least one **parameter pack** appears at least once, is *expanded* into zero or more comma-separated instantiations of the pattern, where the name of the **parameter pack** is replaced by each of the elements from the **pack**, in order.

```C++
template<class ...Us> void f(Us ... pargs)
{
}
template<class ...Ts> void g(Ts ... args)
{
	f(&args...); // “&args...” is a pack expansion
				 // “&args” is its pattern
}
int main()
{
	g(1, 0.2, "a"); // Ts... args expand to int E1, double E2, const char* E3
					// &args... expands to &E1, &E2, &E3
					// Us... pargs expand to int* E1, double* E2, const char** E3
}

```

### Expand multiple pack simultaneously

If the names of two parameter packs appear in the same pattern, they are expanded simultaneously, and they must have the same length:

```C++
template<typename ...> struct Tuple
{
};
template<typename T1, typename T2> struct Pair
{
};

template<class ...Args1> struct zip
{
	template<class ...Args2> struct with
	{
		typedef Tuple<Pair<Args1, Args2> ...> type;
//        Pair<Args1, Args2>... is the pack expansion
//        Pair<Args1, Args2> is the pattern
	};
};

typedef zip<short, int>::with<unsigned short, unsigned>::type T1;
// Pair<Args1, Args2>... expands to
// Pair<short, unsigned short>, Pair<int, unsigned int>
// T1 is Tuple<Pair<short, unsigned short>, Pair<int, unsigned>>

// typedef zip<short>::with<unsigned short, unsigned>::type T2;
// error: pack expansion contains parameter packs of different lengths
int main()
{

}
// g++ --std=c++11 test.cpp


```

> NOTE: 上面就是 parameter pack `Args1` 和 `Args2` 一起出现在`Pair<Args1, Args2>` pattern中。

### Nesting expand

If a **pack expansion** is nested within another **pack expansion**, the **parameter packs** that appear inside the innermost **pack expansion** are expanded by it, and there must be another pack mentioned in the enclosing pack expansion, but not in the innermost one:

> NOTE: 规则是怎样的？是从内向外进行expand？从下面的的`f(h(args...) + args...)`例子来看，它是先expand `h(args...)`，然后将它的结构和`+ args...`进行expand，看似是从内向外。

```C++
template<class ...Args>
void g(Args ... args)
{
	f(const_cast<const Args*>(&args)...);
	// const_cast<const Args*>(&args) is the pattern, it expands two packs
	// (Args and args) simultaneously

	f(h(args...) + args...); // Nested pack expansion:
	// inner pack expansion is "args...", it is expanded first
	// outer pack expansion is h(E1, E2, E3) + args..., it is expanded
	// second (as h(E1,E2,E3) + E1, h(E1,E2,E3) + E2, h(E1,E2,E3) + E3)
}
int main()
{

}
// g++ --std=c++11 test.cpp


```

> NOTE: `f(h(args...) + args...);`的expansion结果在原文的"`Expansion loci#Function argument lists`"章节中介绍了，如下: 
>
> ```C++
> f(h(args...) + args...); // expands to 
> // f(h(E1,E2,E3) + E1, h(E1,E2,E3) + E2, h(E1,E2,E3) + E3)
> ```
>
> 

## Expansion loci

> NOTE: "loci"的意思是"点"

Depending on where the expansion takes place, the resulting comma-separated list is a different kind of list: 

1) function parameter list

2) member initializer list

3) attribute list, etc. 

The following is the list of all allowed contexts

### Function argument lists

A **pack expansion** may appear inside the parentheses of a function call operator, in which case the **largest expression** or **braced-init-list** to the left of the ellipsis is the pattern that is expanded.

```C++
f(&args...); // expands to f(&E1, &E2, &E3)
f(n, ++args...);// expands to f(n, ++E1, ++E2, ++E3);
f(++args..., n);// expands to f(++E1, ++E2, ++E3, n);
f(const_cast<const Args*>(&args)...);
// f(const_cast<const E1*>(&X1), const_cast<const E2*>(&X2), const_cast<const E3*>(&X3))
f(h(args...) + args...);// expands to
// f(h(E1,E2,E3) + E1, h(E1,E2,E3) + E2, h(E1,E2,E3) + E3)

```

### Parenthesized initializers

A pack expansion may appear inside the parentheses of a [direct initializer](https://en.cppreference.com/w/cpp/language/direct_initialization), a [function-style cast](https://en.cppreference.com/w/cpp/language/explicit_cast), and other contexts ([member initializer](https://en.cppreference.com/w/cpp/language/initializer_list), [new-expression](https://en.cppreference.com/w/cpp/language/new), etc.) in which case the rules are identical to the rules for a **function call expression** above

```C++
Class c1(&args...);             // calls Class::Class(&E1, &E2, &E3)
Class c2 = Class(n, ++args...); // calls Class::Class(n, ++E1, ++E2, ++E3);
::new((void *)p) U(std::forward<Args>(args)...) // std::allocator::allocate

```



### Brace-enclosed initializers

In a *braced-init-list* (brace-enclosed list of initializers and other *braced-init-list*s, used in [list-initialization](https://en.cppreference.com/w/cpp/language/list_initialization) and some other contexts), a **pack expansion** may appear as well:

```C++
#include <iostream>
template<typename ... Ts>
void func(Ts ... args)
{
	const int size = sizeof...(args) + 2;
	int res[size] = { 1, args..., 2 };
	// since initializer lists guarantee sequencing, this can be used to
	// call a function on each element of a pack, in order:
	int dummy[sizeof...(Ts)] = { (std::cout << args <<" ", 0)... };
	std::cout << std::endl;
	for (auto &&i : dummy)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

int main()
{
	func(1, 2, 3, 4, 5, 6);
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```C++
> 1 2 3 4 5 6 
> 0 0 0 0 0 0
> ```
>
> 重点是理解: `int dummy[sizeof...(Ts)] = { (std::cout << args <<" ", 0)... };`
>
> 1) `std::cout << args <<" ", 0`是 [comma expression](https://en.cppreference.com/w/cpp/language/operator_other) ，它的返回值是 `0`



### Template argument lists

Pack expansions can be used anywhere in a template argument list, provided the template has the parameters to match the expansion.

```C++
template<class A, class B, class...C> void func(A arg1, B arg2, C...arg3)
{
    container<A,B,C...> t1;  // expands to container<A,B,E1,E2,E3> 
    container<C...,A,B> t2;  // expands to container<E1,E2,E3,A,B> 
    container<A,C...,B> t3;  // expands to container<A,E1,E2,E3,B> 
}
```

### Function parameter list

In a function parameter list, if an ellipsis appears in a parameter declaration (whether it names a function parameter pack (as in, *Args* `**...**` *args*) or not) the parameter declaration is the pattern:

``` C++
#include <iostream>
template<typename ...Ts>
void f(Ts...)
{
}

template<typename ...Ts, int ... N>
void g(Ts (&...arr)[N])
{
}

int main()
{
	f('a', 1);  // Ts... expands to void f(char, int)
	f(0.1);     // Ts... expands to void f(double)
	int n[1];
	g<const char, int>("a", n); // Ts (&...arr)[N] expands to
								// const char (&)[2], int(&)[1]
}
// g++ --std=c++11 test.cpp

```



Note: In the pattern `Ts (&...arr)[N]`, the ellipsis is the innermost element, not the last element as in all other pack expansions.

Note: `Ts (&...)[N]` is not allowed because the C++11 grammar requires the parenthesized ellipsis to have a name: [CWG #1488](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1488).



### Template parameter list

Pack expansion may appear in a **template parameter list**:

```C++
template<typename... T> struct value_holder
{
    template<T... Values> // expands to a non-type template parameter 
    struct apply { };     // list, such as <int, char, int(&)[5]>
};
```

### Base specifiers and member initializer lists

> NOTE: 
>
> 一、这种technique被称为:
>
> 1、Variadic multiple inheritance
>
> 二、使用它的一个例子是: fluentcpp strong type，参见:
>
> 1、`Curiously-recurring-template-pattern\Variadic-CRTP` 章节
>
> 2、`C++\Expert\Fluentcpp\joboccara-NamedType`章节
>
> 三、它可以用于实现mixin multiple

A **pack expansion** may designate the list of base classes in a [class declaration](https://en.cppreference.com/w/cpp/language/class). Typically, this also means that the constructor needs to use a **pack expansion** in the [member initializer list](https://en.cppreference.com/w/cpp/language/initializer_list) to call the constructors of these bases:

```C++
#include <iostream>
template<class ... Mixins>
class X: public Mixins...
{
public:
	X(const Mixins &... mixins) :
					Mixins(mixins)...
	{
		std::cout << __FILE__ << " " << __LINE__ << " " << __PRETTY_FUNCTION__  << std::endl;
	}
};
class T1
{
public:
	T1()
	{
		std::cout << __FILE__ << " " << __LINE__ << " " << __PRETTY_FUNCTION__  << std::endl;
	}
};

class T2
{
public:
	T2()
	{
		std::cout << __FILE__ << " " << __LINE__ << " " << __PRETTY_FUNCTION__  << std::endl;
	}
};
int main()
{
	T1 t1;
	T2 t2;
	X<T1, T2> x(t1, t2);
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下: 
>
> ```c++
> test.cpp 17 T1::T1()
> test.cpp 26 T2::T2()
> test.cpp 9 X<Mixins>::X(const Mixins& ...) [with Mixins = {T1, T2}]
> 
> ```
>
> 

### Lambda captures (since C++14)

A parameter pack may appear in the capture clause of a [lambda](https://en.cppreference.com/w/cpp/language/lambda) expression

```C++
#include <iostream>

void print_all()
{
	// base case
	std::cout << std::endl;
}

template<class T, class ... Ts>
void print_all(T const &first, Ts const &... rest)
{
	std::cout << first << " ";

	print_all(rest...);
}

template<class ...Args>
void g(Args ...args)
{
	print_all(args...);
}
template<class ...Args>
void f(Args ... args)
{
//	auto lm = [&, args ...] {	return g(args...);};
//	lm();
	print_all(args...);
}
int main()
{
	f("hello", "world");
}
// g++ --std=c++11 test.cpp

```



### The sizeof... operator

The [sizeof...](https://en.cppreference.com/w/cpp/language/sizeof...) operator is classified as a pack expansion as well

```C++
#include <iostream>

template<class ... Types>
struct count
{
	static const std::size_t value = sizeof...(Types);
};

int main()
{
	std::cout << count<int, double>::value << std::endl;
}
// g++ --std=c++11 test.cpp

```



### Alignment specifier

Pack expansions are allowed in both the lists of types and the lists of expressions used by the keyword [alignas](https://en.cppreference.com/w/cpp/language/alignas)。

### Attribute list

Pack expansions are allowed in the lists of [attributes](https://en.cppreference.com/w/cpp/language/attributes), as in `[[attributes...]]`. For example: `void [[attributes...]] function()`。

### Fold-expressions (since C++17)

In [fold-expressions](https://en.cppreference.com/w/cpp/language/fold), the pattern is the entire subexpression that does not contain an unexpanded parameter pack.

### Using-declarations (since C++17)

In [using declaration](https://en.cppreference.com/w/cpp/language/using_declaration), ellipsis may appear in the list of declarators, this is useful when deriving from a parameter pack:

```C++
template<typename ... bases>
struct X: bases...
{
	using bases::g...;
};
X<B, D> x; // OK: B::g and D::g introduced

```

