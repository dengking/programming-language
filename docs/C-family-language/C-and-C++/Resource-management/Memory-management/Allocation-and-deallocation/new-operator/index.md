# `new` operator



## cppreference [new expression](https://en.cppreference.com/w/cpp/language/new)

Creates and initializes objects with dynamic [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration), that is, objects whose lifetime is not necessarily limited by the **scope** in which they were created.

### Syntax

```C++
::(optional) new (placement_params)(optional) ( type ) initializer(optional)	(1)	
::(optional) new (placement_params)(optional) type initializer(optional)	(2)	
```

> NOTE: 关于 `::` operator，参见下面的"Allocation"章节

1) Attempts to create an object of type, denoted by the [type-id](https://en.cppreference.com/w/cpp/language/type#Type_naming) `type`, which may be 

1 array type, and may include 

2 a [placeholder type specifier](https://en.cppreference.com/w/cpp/language/auto) (since C++11), or include 

3 a class template name whose argument is to be deduced by [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction) (since C++17).

2) Same, but `type` cannot include parentheses:

### Explanation

#### What `new` do and return?

The `new` expression attempts to **allocate storage** and then attempts to construct and initialize either a single unnamed object, or an unnamed array of objects in the allocated storage. 

> NOTE: 做了两件事:
>
> 1) allocate storage 
>
> 2) construct and initialize

The new-expression returns a **prvalue pointer** to the constructed object or, if an array of objects was constructed, a pointer to the initial element of the array.

> NOTE: 它的返回值就是是一个value，更加具体说是address value

#### Array type

If `type` is an array type, all dimensions other than the first must be specified as positive [integral constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) (until C++14)[converted constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) of type [std::size_t](https://en.cppreference.com/w/cpp/types/size_t) (since C++14), but (only when using un-parenthesized syntax (2)) the first dimension may be an expression of integral type, enumeration type, or class type with a single non-explicit conversion function to integral or enumeration type (until C++14)any expression convertible to [std::size_t](https://en.cppreference.com/w/cpp/types/size_t) (since C++14). This is the only way to directly create an array with size defined at runtime, such arrays are often referred to as *dynamic arrays*:

```C++
int n = 42;
double a[n][5]; // error
auto p1 = new  double[n][5];  // OK
auto p2 = new  double[5][n];  // error: only the first dimension may be non-constant
auto p3 = new (double[n][5]); // error: syntax (1) cannot be used for dynamic arrays
```

#### Allocation

The new-expression allocates storage by calling the appropriate [allocation function](https://en.cppreference.com/w/cpp/memory/new/operator_new). If `type` is a non-array type, the name of the function is `operator new`. If `type` is an array type, the name of the function is `operator new[]`.

> NOTE: overload new operator，在  [allocation function](https://en.cppreference.com/w/cpp/memory/new/operator_new) 中给出了所有的原型









## TODO

https://stackoverflow.com/questions/9603696/use-new-operator-to-initialise-an-array





## cnblogs [C++ 内存分配操作符new和delete详解](https://www.cnblogs.com/Philip-Tell-Truth/p/6567808.html)