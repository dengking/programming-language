# cppreference  pointer



## cppreference [Pointer declaration](https://en.cppreference.com/w/cpp/language/pointer)

### Syntax

> NOTE: 不同于上面的基于“Function and data model”来对pointer进行分类，cppreference中是基于OOP来对pointer进行分类的:
>
> - pointer 
> - pointer-to-member

|             | pointer                                                      | pointer-to-member                                            |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **syntax**  | `decl-specifier-seq * attr(optional) cv(optional) declarator` | `decl-specifier-seq nested-name-specifier * attr(optional) cv(optional) declarator` |
| **example** | `S* D;` <br>declares `D` as a pointer to the type determined by *decl-specifier-seq* `S`. | `S C::* D;` <br/>declares `D` as a pointer to non-static member of `C` of type determined by *decl-specifier-seq* `S`. |

参见:

*nested-name-specifier*	-	a [sequence of names and scope resolution operators `::`](https://en.cppreference.com/w/cpp/language/identifiers#Qualified_identifiers)

*attr(C++11)*	-	 optional list of 

***declarator***	-	[declarator](https://en.cppreference.com/w/cpp/language/declarations)   



### Initialization

> NOTE: pointer是object，因此需要对它进行initialization

[address-of operator](https://en.cppreference.com/w/cpp/language/operator_member_access)



### Pointers to objects

```c++
#include <iostream>

int main()
{
	int n;
	int *np = &n; // pointer to int
	int * const *npp = &np; // non-const pointer to const pointer to non-const int

	int a[2];
	int (*ap)[2] = &a; // pointer to array of int

	struct S
	{
		int n;
	} s = { 1 };
	int* sp = &s.n; // pointer to the int that is a member of s
}
// g++ --std=c++11 test.cpp

```



### Pointers to void

> NOTE: 这部分内容放到了下面的Pointer to void章节



### [Pointers to functions](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_functions)

Because of the [function-to-pointer](https://en.cppreference.com/w/cpp/language/implicit_cast) implicit conversion, the address-of operator is optional:

```c++
void f(int)
{

}

int main()
{

	void (*p1)(int) = &f;
	void (*p2)(int) = f; // same as &f
}
// g++ test.cpp

```



### Pointers to members



#### Pointers to data members



#### Pointers to member functions

> 参见: `C++\Language-reference\Functions\First-class-function\Member-function-as-first-class-function` 章节

### Null pointers

> NOTE: 
>
> 1、参见 `Null-pointer` 章节

### Constness

| Syntax               | meaning                             | 说明                                                         |
| -------------------- | ----------------------------------- | ------------------------------------------------------------ |
| `const T* ptr`       | pointer to constant object          |                                                              |
| `T const* ptr`       | pointer to constant object          |                                                              |
| `T* const ptr`       | constant pointer to object          | 按照[这篇文章](https://stackoverflow.com/a/596750)中的看法: <br>A *reference* can be thought of as a *constant pointer* |
| `const T* const ptr` | constant pointer to constant object |                                                              |
| `T const* const ptr` | constant pointer to constant object |                                                              |

> NOTE: 原文，并没有添加variable name，不添加variable name是不易理解的，上面是我添加了variable name的描述。
>
> 在下面文章中，对这个问题进行了深入分析:
>
> 1) [What is the difference between char * const and const char *?](https://stackoverflow.com/questions/890535/what-is-the-difference-between-char-const-and-const-char)
>
> 2) [Constant pointer vs Pointer to constant [duplicate]](https://stackoverflow.com/questions/21476869/constant-pointer-vs-pointer-to-constant)
>
> 3) [What is the difference between const int*, const int * const, and int const *?](https://stackoverflow.com/questions/1143262/what-is-the-difference-between-const-int-const-int-const-and-int-const)
>
> 其实，使用 [Clockwise/Spiral Rule](http://c-faq.com/decl/spiral.anderson.html)， 我们可以非常快速地理解其语义，参见`C-and-C++\Declaration\The-Clockwise-Spiral-Rule.md`。
>
> 
>
> ### stackoverflow [What is the difference between const int*, const int * const, and int const *?](https://stackoverflow.com/questions/1143262/what-is-the-difference-between-const-int-const-int-const-and-int-const)
>
> Q:
>
> I always mess up how to use `const int*`, `const int * const`, and `int const *` correctly. Is there a set of rules defining what you can and cannot do?
>
> I want to know all the do's and all don'ts in terms of assignments, passing to the functions, etc.
>
> 
>
> A:
>
> You can use the ["Clockwise/Spiral Rule"](http://c-faq.com/decl/spiral.anderson.html) to decipher most C and C++ declarations. – [James McNellis](https://stackoverflow.com/users/151292/james-mcnellis) 
>
> A:
>
> [cdecl.org](http://www.cdecl.org/) is a great website which auto-translates C declarations for you. – [Dave Gallagher](https://stackoverflow.com/users/153040/dave-gallagher) 
>
> [A](https://stackoverflow.com/a/1143272):
>
> Read it backwards (as driven by [Clockwise/Spiral Rule](http://c-faq.com/decl/spiral.anderson.html)):
>
> - `int*` - pointer to int
> - `int const *` - pointer to const int
> - `int * const` - const pointer to int
> - `int const * const` - const pointer to const int
>
> Now the first `const` can be on either side of the type so:
>
> - `const int *` == `int const *`
> - `const int * const` == `int const * const`
>
> If you want to go really crazy you can do things like this:
>
> - `int **` - pointer to pointer to int
> - `int ** const` - a const pointer to a pointer to an int
> - `int * const *` - a pointer to a const pointer to an int
> - `int const **` - a pointer to a pointer to a const int
> - `int * const * const` - a const pointer to a const pointer to an int
> - ...
>
> And to make sure we are clear on the meaning of const
>
> ```
> const int* foo;
> int *const bar; //note, you actually need to set the pointer 
>                 //here because you can't change it later ;)
> ```
>
> `foo` is a variable pointer to a constant integer. This lets you change what you point to but not the value that you point to. Most often this is seen with C-style strings where you have a pointer to a `const char`. You may change which string you point to but you can't change the content of these strings. This is important when the **string** itself is in the **data segment** of a program and shouldn't be changed.
>
> `bar` is a constant or fixed pointer to a value that can be changed. This is like a **reference** without the extra syntactic sugar. Because of this fact, usually you would use a reference where you would use a `T* const` pointer unless you need to allow `NULL` pointers.



## cppreference [Pointer comparison operators](https://en.cppreference.com/w/cpp/language/operator_comparison#Pointer_comparison_operators)



