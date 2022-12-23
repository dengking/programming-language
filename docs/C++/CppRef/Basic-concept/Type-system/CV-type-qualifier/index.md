# CV type qualifiers

本章对C++的type specifier进行说明，目前C++中有两个type specifier；

本章采用的在`C++\Language-reference\Basic-concept\index.md#Extend to OOP`中总结的思路:

> 首先描述Non-OOP，然后描述OOP



## cppreference [cv (const and volatile) type qualifiers](https://en.cppreference.com/w/cpp/language/cv)

> 总结：其实看了这个之后，才发觉自己对c++的type system的认知是浅薄的，原来c++中，通过这些specifier是可以进一步对type进行修饰的，从而可以提供更加丰富的语义。

 Appear in any type specifier, including `decl-specifier-seq` of [declaration grammar](https://en.cppreference.com/w/cpp/language/declarations), to specify **constness** or **volatility** of the object being declared or of the type being named.

- `const` - defines that the type is *constant*.
- `volatile` - defines that the type is *volatile*.
- `mutable` - applies to non-static [class members](https://en.cppreference.com/w/cpp/language/data_members) of **non-reference** **non-const** type and specifies that the member does not affect the **externally visible state** of the class (as often used for mutexes, memo caches, lazy evaluation, and access instrumentation). `mutable` members of *const* class instances are modifiable. (Note: the C++ language grammar treats `mutable` as a [storage-class-specifier](https://en.cppreference.com/w/cpp/language/storage_duration), but it does not affect storage class.)

> 总结：显然mutable和const的从语义来看是相反的，mutable和volatile的从字面含义上来看是相近的。但是实际上我看到mutable和const是是可以一起使用的，并且当涉及到对象和对象之间的关系（比如一个对象作为另外一个对象的成员变量），那么这三个specifier和这些关系的组合情况就是非常复杂的了，并且下面的介绍也是从这个角度来进行的。所以准确把握他们的语义是非常重要的。从后续的介绍来看，它主要是介绍`const`和`volatile`，而`mutable`仅仅是作为相关内容。显然，`mutable`并不属于**cv type qualifiers**之列，但`mutable`和**cv type qualifiers**又有非常重要的联系，从上面这段话可以看出：mutable和const的语义是相反的，并且，`mutable` members of *const* class instances are modifiable. 

### Explanation

For any type `T` (including incomplete types), other than [function type](https://en.cppreference.com/w/cpp/language/functions) or [reference type](https://en.cppreference.com/w/cpp/language/reference), there are three more distinct types in the **C++ type system**:

- const-qualified `T`,
- volatile-qualified `T`
- const-volatile-qualified `T`.

Note: [array types](https://en.cppreference.com/w/cpp/language/array) are considered to have the same cv-qualification as their element types.

When an object is first created, the **cv-qualifiers** used (which could be part of `decl-specifier-seq` or part of a `declarator` in a [declaration](https://en.cppreference.com/w/cpp/language/declarations), or part of `type-id` in a [new-expression](https://en.cppreference.com/w/cpp/language/new)) determine the **constness** or **volatility** of the object, as follows:

- **const object** - an object whose type is **const-qualified**, or a **non-mutable** subobject of a **const object**. Such object cannot be modified: attempt to do so directly is a compile-time error, and attempt to do so indirectly (e.g., by modifying the const object through a reference or pointer to non-const type) results in **undefined behavior**(可以通过使用`const_cast`将一个const object转换为non-const object，但是如果修改转换后的object，则是会导致undefined behavior的，在[这篇文章](http://www.cplusplus.com/doc/tutorial/typecasting/)中对这个内容有介绍).
- **volatile object** - an object whose type is **volatile-qualified**, or a subobject of a **volatile object**, or a **mutable** subobject of a **const-volatile object**. Every access (read or write operation, member function call, etc.) made through a **glvalue expression** of **volatile-qualified type** is treated as a **visible side-effect** for the [purposes of optimization](https://en.cppreference.com/w/cpp/language/as_if) (that is, within a single thread of execution, **volatile accesses** cannot be optimized out or reordered with another **visible side effect** that is [sequenced-before](https://en.cppreference.com/w/cpp/language/eval_order) or sequenced-after the volatile access. This makes volatile objects suitable for communication with a [signal handler](https://en.cppreference.com/w/cpp/utility/program/signal), but not with another thread of execution, see [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)). Any attempt to refer to a **volatile object** through a non-volatile [glvalue](https://en.cppreference.com/w/cpp/language/value_category#glvalue) (e.g. through a reference or pointer to non-volatile type) results in undefined behavior.
- **const volatile object** - an object whose type is const-volatile-qualified, a non-mutable subobject of a const volatile object, a const subobject of a volatile object, or a non-mutable volatile subobject of a const object. Behaves as both a const object and as a volatile object.

>This section is incomplete Reason: should discuss more about the differences between cv-qualified objects and cv-qualified expressions |

There is partial ordering of cv-qualifiers by the order of increasing restrictions. The type can be said *more* or *less* cv-qualified then:

- *unqualified* < `const`
- *unqualified* < `volatile`
- *unqualified* < `const volatile`
- `const` < `const volatile`
- `volatile` < `const volatile`

References and pointers to cv-qualified types may be [implicitly converted](https://en.cppreference.com/w/cpp/language/implicit_cast#Qualification_conversions) to references and pointers to *more cv-qualified* types. In particular, the following conversions are allowed:

- reference/pointer to *unqualified* type can be converted to reference/pointer to `const`
- reference/pointer to *unqualified* type can be converted to reference/pointer to `volatile`
- reference/pointer to *unqualified* type can be converted to reference/pointer to `const volatile`
- reference/pointer to `const` type can be converted to reference/pointer to `const volatile`
- reference/pointer to `volatile` type can be converted to reference/pointer to `const volatile`

Note: [additional restrictions](https://en.cppreference.com/w/cpp/language/implicit_cast#Qualification_conversions) are imposed on multi-level pointers.

To convert a reference or a pointer to a cv-qualified type to a reference or pointer to a *less cv-qualified* type, [const_cast](https://en.cppreference.com/w/cpp/language/const_cast) must be used.

### Keywords

[`const`](https://en.cppreference.com/w/cpp/keyword/const), [`volatile`](https://en.cppreference.com/w/cpp/keyword/volatile), [`mutable`](https://en.cppreference.com/w/cpp/keyword/mutable)

### Notes

The `const` qualifier used on a declaration of a non-local non-volatile variable that is not declared `extern` gives it [internal linkage](https://en.cppreference.com/w/cpp/language/storage_duration#Linkage). This is different from C where const file scope variables have external linkage.


### supply

Note, however, that cv-qualifiers applied to an array type actually apply to its elements.

The cv-qualified and cv-unqualified types are distinct. That is `int` is a distinct type from `const int`





## const and non-const

const -> non const是危险的，丢失了CV，可能导致undefined behavior；
non-const -> const是正常的，不存在问题；

下面是一个典型non-const->const的例子：

```C++
#include <iostream>

void func(const void* input)
{
	const int* i = reinterpret_cast<const int*>(input); // 保持CV
	std::cout << *i << std::endl;
}
int main()
{
	const int i = 0;
	func(&i);
}
// g++ test.cpp
```



```c++
#include <iostream>

int main()
{
	int i = 3;                 // i is not declared const
	const int& rci = i;
	const_cast<int&>(rci) = 4; // OK: modifies i
	std::cout << "i = " << i << '\n';
}
// g++ test.cpp
```



## C and C++

C++的CV比C要严格。