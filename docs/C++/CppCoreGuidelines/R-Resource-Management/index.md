# CppCoreGuidelines [R: Resource management](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-resource)

The fundamental aim is to ensure that we don't leak any resources and that we don't hold a resource longer than we need to. An entity that is responsible for releasing a resource is called an **owner**.

> NOTE: 
>
> 1、"don't leak any resources"需要 strong exception safety
>
> 2、后续很多内容都是围绕 "don't leak any resources"而展开 
>
> 3、resource ownership: 这里提出了**owner**的概念



## 1、Resource management rule summary:

[R.1: Manage resources automatically using resource handles and RAII (Resource Acquisition Is Initialization)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-raii)

[R.2: In interfaces, use raw pointers to denote individual objects (only)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-use-ptr)

[R.3: A raw pointer (a `T*`) is non-owning](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-ptr)

[R.4: A raw reference (a `T&`) is non-owning](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-ref)

[R.5: Prefer scoped objects, don't heap-allocate unnecessarily](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-scoped)

[R.6: Avoid non-`const` global variables](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-global)



### [R.1: Manage resources automatically using resource handles and RAII (Resource Acquisition Is Initialization)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-raii)

#### Reason

To avoid leaks and the complexity of manual resource management. C++'s language-enforced constructor/destructor symmetry mirrors the symmetry inherent in resource acquire/release function pairs such as `fopen`/`fclose`, `lock`/`unlock`, and `new`/`delete`. Whenever you deal with a resource that needs paired acquire/release function calls, encapsulate that resource in an object that enforces pairing for you -- acquire the resource in its constructor, and release it in its destructor.

> NOTE: 
>
> 总结的非常好。
>
> 1、RAII idiom、resource handle
>
> 2、object-based resource management
>
> 最终目的:
>
> 1、保证strong exception safety
>
> 2、no resource leakage
>
> 3、simplify
>
> 4、明确ownership，让代码容易理解、维护，关于这一点，参见:
>
> [R.3: A raw pointer (a `T*`) is non-owning](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r3-a-raw-pointer-a-t-is-non-owning)



#### Example, bad

> NOTE: 原因给出的例子一般，比较容易理解

#### Example

Consider:

```C++
void send(unique_ptr<X> x, string_view destination)  // x owns the X
{
    Port port{destination};            // port owns the PortHandle
    lock_guard<mutex> guard{my_mutex}; // guard owns the lock
    // ...
    send(port, x);
    // ...
} // automatically unlocks my_mutex and deletes the pointer in x
```

Now all resource cleanup is automatic, performed once on all paths whether or not there is an exception. As a bonus, the function now advertises that it takes over ownership of the pointer.

#### Note

Where a resource is "ill-behaved" in that it isn't represented as a class with a destructor, wrap it in a class or use [`finally`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Re-finally)

> NOTE: 
>
> 1、上面这段话中的"ill-behaved"如何理解？
>
> 2、 [`finally`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Re-finally) 其实就是guard

**See also**: [RAII](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Re-raii)

### [R.2: In interfaces, use raw pointers to denote individual objects (only)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-use-ptr)

> NOTE: 
>
> 1、原文的这段话，其实说的不够直接，它想表达是: 在interface中，不要使用raw pointer来数组表示array: 不要使用pointer + length的模式，而是使用`vector` 或者 `span`；仅仅使用raw pointer来表示单个object(individual object )

#### Reason

Arrays are best represented by a container type (e.g., `vector` (owning)) or a `span` (non-owning). Such containers and views hold sufficient information to do range checking.

#### Example, bad

```C++
void f(int* p, int n)   // n is the number of elements in p[]
{
    // ...
    p[2] = 7;   // bad: subscript raw pointer
    // ...
}
```

> NOTE: 
>
> 1、这是我们常常使用的方式

The compiler does not read comments, and without reading other code you do not know whether `p` really points to `n` elements. Use a `span` instead.

#### Example

```C++
void g(int* p, int fmt)   // print *p using format #fmt
{
    // ... uses *p and p[0] only ...
}
```

#### Exception

C-style strings are passed as single pointers to a zero-terminated sequence of characters. Use `zstring` rather than `char*` to indicate that you rely on that convention.



### [R.3: A raw pointer (a `T*`) is non-owning](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r3-a-raw-pointer-a-t-is-non-owning)

#### Reason

There is nothing (in the C++ standard or in most code) to say otherwise and most raw pointers are non-owning. We want owning pointers identified so that we can reliably and efficiently delete the objects pointed to by owning pointers.

> NOTE: 翻译如下:
>
> "没有什么(在c++标准或大多数代码中)不是这样说的，大多数原始指针是非拥有的。
> 我们希望标识拥有指针的对象，以便能够可靠和有效地删除由拥有指针所指向的对象。"
>
> 意思是:
>
> 1、默认情况下，在C++中，raw pointer表达的是"not owning semantic"，这是在 C++中约定俗成的
>
> 2、为了"明确Resource ownership"，可以使用smart pointer，并且它还是提供了RAII

#### Example

```C++
void f()
{
    int* p1 = new int{7};           // bad: raw owning pointer
    auto p2 = make_unique<int>(7);  // OK: the int is owned by a unique pointer
    // ...
}
```

The `unique_ptr` protects against leaks by guaranteeing the deletion of its object (even in the presence of exceptions). The `T*` does not.

#### Example

```c++
template<typename T>
class X {
public:
    T* p;   // bad: it is unclear whether p is owning or not
    T* q;   // bad: it is unclear whether q is owning or not
    // ...
};
```

We can fix that problem by making ownership explicit:

```c++
template<typename T>
class X2 {
public:
    owner<T*> p;  // OK: p is owning
    T* q;         // OK: q is not owning
    // ...
};
```

> NOTE: 
>
> 1、使semantic清晰

#### Note

`owner<T*>` has no default semantics beyond `T*`. It can be used without changing any code using it and without affecting ABIs. It is simply an indicator to programmers and analysis tools. For example, if an `owner<T*>` is a member of a class, that class better have a destructor that `delete`s it.

#### Example, bad

> NOTE: 
>
> 1、使用 resource return idiom

In addition to suffering from the problem from [leak](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#???), this adds a spurious allocation and deallocation operation, and is needlessly verbose. If `Gadget` is cheap to move out of a function (i.e., is small or has an efficient move operation), just return it "by value" (see ["out" return values](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rf-out)):

> NOTE: 
>
> 1、" If `Gadget` is cheap to move out of a function (i.e., is small or has an efficient move operation), just return it "by value"" 让我想起了: `Optimization-in-function-return` 章节、 `Move-and-return`章节、two-stage overload resolution

```C++
Gadget make_gadget(int n)
{
    Gadget g{n};
    // ...
    return g;
}
```



### [R.4: A raw reference (a `T&`) is non-owning](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r4-a-raw-reference-a-t-is-non-owning)

> NOTE: 和前面类似

### [R.5: Prefer scoped objects, don't heap-allocate unnecessarily](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r5-prefer-scoped-objects-dont-heap-allocate-unnecessarily)

> NOTE: 
>
> 1、比较好理解

## 2、Allocation and deallocation rule summary:

[R.10: Avoid `malloc()` and `free()`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-mallocfree)

[R.11: Avoid calling `new` and `delete` explicitly](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-newdelete)

[R.12: Immediately give the result of an explicit resource allocation to a manager object](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-immediate-alloc)

[R.13: Perform at most one explicit resource allocation in a single expression statement](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-single-alloc)

[R.14: Avoid `[]` parameters, prefer `span`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-ap)

[R.15: Always overload matched allocation/deallocation pairs](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-pair)

3、Smart pointer rule summary

> NOTE: 这部分放到了smart pointer章节





### [R.10: Avoid `malloc()` and `free()`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#ralloc-allocation-and-deallocation)

#### Reason

`malloc()` and `free()` do not support construction and destruction, and do not mix well with `new` and `delete`.

> NOTE: 
>
> 1、要理解上面这段话，需要对于C++ object lifetime有一个完整的认知，在下面章节对这个topic进行了讨论:
>
> a、`Lifetime-and-storage-duration`
>
> b、`C++\Language-reference\Classes\Lifetime`
>
> 其实，我们可以这样简单理解: `malloc` 和 `free` 是为C而设计的，`new` 和 `delete` 是为C++而设计的

#### Example

```c++
#include <string>
#include "stdlib.h"
using namespace std;

class Record
{
	int id;
	string name;
	// ...
};

void use()
{
	// p1 might be nullptr
	// *p1 is not initialized; in particular,
	// that string isn't a string, but a string-sized bag of bits
	Record *p1 = static_cast<Record*>(malloc(sizeof(Record)));

	auto p2 = new Record;

	// unless an exception is thrown, *p2 is default initialized
	auto p3 = new (nothrow) Record;
	// p3 might be nullptr; if not, *p3 is default initialized

	// ...

	delete p1;    // error: cannot delete object allocated by malloc()
	free(p2);    // error: cannot free() object allocated by new
}
int main()
{
	use();
}
// g++ -Wall -pedantic test.cpp && ./a.out
```

> NOTE: 
>
> 运行后，core dump了

#### Exception

There are applications and sections of code where exceptions are not acceptable. Some of the best such examples are in life-critical hard-real-time code. Beware that many bans on exception use are based on superstition (bad) or by concerns for older code bases with unsystematic resource management (unfortunately, but sometimes necessary). In such cases, consider the `nothrow` versions of `new`.

> NOTE: 翻译如下:
>
> "有些应用程序和代码段是不能接受异常的。
> 其中一些最好的例子是关键的硬实时代码。
> 要注意，许多禁止异常使用的做法都是基于迷信(不好)或对具有非系统资源管理的旧代码库的关注(不幸的是，但有时是必要的)。
> 在这种情况下，考虑一下“new”的“nothrow”版本。"
>
> 

#### Enforcement

Flag explicit use of `malloc` and `free`.

### [R.11: Avoid calling `new` and `delete` explicitly](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r11-avoid-calling-new-and-delete-explicitly)

#### Reason

The pointer returned by `new` should belong to a **resource handle** (that can call `delete`). If the pointer returned by `new` is assigned to a plain/naked pointer, the object can be leaked.

> NOTE: 
>
> 一、重点是理解why "If the pointer returned by `new` is assigned to a plain/naked pointer, the object can be leaked"？在下面的文章中，给出了回答:
>
> a、sean-parent [Value Semantics and Concept-based Polymorphism](https://sean-parent.stlab.cc/papers-and-presentations/#value-semantics-and-concept-based-polymorphism)
>
> `document.emplace_back(new my_class_t());`
>
> defects: 
>
> 1、An instance of `my_class_t` will be allocated first
>
> 2、Then the document will grow to make room
>
> 3、If growing the document throws an exception, the memory from `my_class_t` is leaked
>
> 二、使用RAII、smart pointer object generator
>
> 下面的"Enforcement"也是符合这个观点的
>
> 



#### Note

In a large program, a naked `delete` (that is a `delete` in application code, rather than part of code devoted to resource management) is a likely bug: if you have N `delete`s, how can you be certain that you don't need N+1 or N-1? The bug might be latent(隐藏的): it might emerge only during maintenance. If you have a naked `new`, you probably need a naked `delete` somewhere, so you probably have a bug.

> NOTE: 
>
> 1、这一节其实是在说明calling new and delete explicitly的缺点

#### Enforcement

(Simple) Warn on any explicit use of `new` and `delete`. Suggest using `make_unique` instead.

### [R.12: Immediately give the result of an explicit resource allocation to a manager object](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r12-immediately-give-the-result-of-an-explicit-resource-allocation-to-a-manager-object)



#### Reason

If you don't, an exception or a return might lead to a leak.

#### Example, bad

```C++
void f(const string& name)
{
    FILE* f = fopen(name, "r");            // open the file
    vector<char> buf(1024);
    auto _ = finally([f] { fclose(f); });  // remember to close the file
    // ...
}
```

The allocation of `buf` might fail and leak the file handle.

> NOTE: 
>
> 1、上述code是典型的scope guard

#### Example

```c++
void f(const string& name)
{
    ifstream f{name};   // open the file
    vector<char> buf(1024);
    // ...
}
```

The use of the file handle (in `ifstream`) is simple, efficient, and safe.

### [R.13: Perform at most one explicit resource allocation in a single expression statement](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r13-perform-at-most-one-explicit-resource-allocation-in-a-single-expression-statement)





### [R.14: Avoid `[]` parameters, prefer `span`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r14-avoid--parameters-prefer-span)

#### Reason

An array decays to a pointer, thereby losing its size, opening the opportunity for range errors. Use `span` to preserve size information.

> NOTE: 
>
> 1、关于span，参见 `C++\Library\Standard-library\STL\Containers-library\View\span` 章节

#### Example

```c++
void f(int[]);          // not recommended

void f(int*);           // not recommended for multiple objects
                        // (a pointer should point to a single object, do not subscript)

void f(gsl::span<int>); // good, recommended
```

#### Enforcement

Flag `[]` parameters. Use `span` instead.



## TODO

1、modernescpp [C++ Core Guidelines: Rules to Resource Management](https://www.modernescpp.com/index.php/c-core-guidelines-rules-to-resource-management)