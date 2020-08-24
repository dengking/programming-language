# `dynamic_cast`



## panicsoftware [DYNAMIC_CAST AND TYPEID AS (NON) RTTI TOOLS.](https://blog.panicsoftware.com/dynamic_cast-and-typeid-as-non-rtti-tools/)

### The `dynamic_cast`

First thing you need to know about RTTI is, that it doesn’t work for the **non-polymorphic types**.

> NOTE: 这是作者列举的大多数人的错误认知，后面它会对此进行修正

And it turns out, that **dynamic_cast** can work for non-polymorphic types. How is that even possible? The answer is simple – not every case, that **dynamic_cast** handles, needs RTTI. We will prove that later on in the article.

> NOTE: 上面这段话描述的是正确的结论，它的大致意思是：`dynamic_cast`可以用于多种情形，当需要RTTI的时候，它才需要入参的type是polymorphic type，因为只有polymorphic type才有RTTI。

Example: downcast

```c++
struct B
{
};
struct D: B
{
};
int main()
{
	B* ptr = new D;
	dynamic_cast<D*>(ptr);
}
// g++ test.cpp

```

> NOTE: 上述程序编译报错如下:
>
> ```c++
> test.cpp: 在函数‘int main()’中:
> test.cpp:5:25: 错误：无法将‘ptr’从类型‘B*’动态转换到类型‘struct D*’(源类型不是多态的)
>      dynamic_cast<D*>(ptr);
> ```



The above snippet requires **RTTI** mechanism to ensure safety in your program. After all, you can try to cast to a different derived type, which is not a dynamic type of a pointer you are trying to cast. This case is handled by the **dynamic_cast** by returning **nullptr** or throwing the **std::bad_cast** exception (depending on whether you cast a pointer or a reference). Since type **B** has got *no virtual function*, then *this is non-polymorphic type* and RTTI cannot be used. This causes compilation error – **dynamic_cast** wants to use RTTI, but it can’t do so.



So now let’s try also to cast types the other way around, so from the derived to the base class. The snippet below represents that situation.

Example: upcast

```c++
struct B
{
};
struct D: B
{
};
int main()
{
	D* ptr = new D;
	dynamic_cast<B*>(ptr);
	delete ptr;
}
// g++ test.cpp

```

> NOTE: 上述程序可以正常编译通过

This works correctly. After all, even implicit cast would be fine here, but **dynamic_cast** also won’t complain. Why is that? The fact, that the cast is safe is known by the compiler at compile time. This means, that **dynamic_cast** does not need to use RTTI.

We can say in general, that **dynamic_cast** is a tool for moving around the inheritance tree – up and down. Whether the **dynamic_cast** uses RTTI depends only on whether the particular case needs it.

Using **dynamic_cast** can also make our intentions clearer. Whenever we say **dynamic_cast**, the reader knows we intend to cast to the **base** or **derived** class. Whenever we say **static_cast**, on the other hand, we know we mainly mean to perform some arithmetic casts, converting constructor calls or the user’s conversion operators.

### The dynamic_cast, you (probably) didn’t know

#### `dynamic_cast<void*>`

We said, that dynamic_cast is dedicated for moving around the inheritance tree. One of such moves is a move to the most derived object. With C++ we can perform such move even without knowing the most derived object’s type. And this is where **dynamic_cast<void\*>** can be used. Let’s have a look at an example:

```c++
#include <cassert>
#include <iostream>

struct B
{
	int a;
	int b;
	virtual ~B() = default;
};
struct C
{
	int a;
	int b;
	virtual ~C() = default;
};
struct D: C, B
{
	int c;
	int d;
};
int main()
{
	// ptrd、ptrb、ptrc 的值并不相同
	D* ptrd = new D;
	B* ptrb = ptrd;
	C* ptrc = ptrd;
	std::cout << ptrb << std::endl;
	std::cout << ptrc << std::endl;
	std::cout << ptrd << std::endl;
	assert(dynamic_cast<void*>(ptrb) == ptrd);
	assert(dynamic_cast<void*>(ptrc) == ptrd);
	delete ptrd;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```c++
> 0x134d020
> 0x134d010
> 0x134d010
> ```
>
> 

In this case, the **ptrb** will have different value, than **ptrd**, since **B** is the subobject of **D** (and therefore its address will have some offset from the beginning of the object **D**).

This, of course, needs the information about the type in the runtime, since we might not know what’s the exact type of the most derived object. For this reason structures B and C have virtual destructors (RTTI on those types must be possible).

#### Ambiguous casts

Did you ever wonder, what if some object of type (let’s say) **D** would inherit twice from the type **B** and we would like to cast it to this type?

Let’s have a look at the example of such cast:



## cppreference [`dynamic_cast`](https://en.cppreference.com/w/cpp/language/dynamic_cast)

Safely converts **pointers** and **references** to classes up, down, and sideways（平级） along the inheritance hierarchy.

> NOTE: 操作对象：pointer and reference



### Syntax

If the cast is successful, `dynamic_cast` returns a value of type *new-type*. If the cast fails and *new-type* is a pointer type, it returns a null pointer of that type. If the cast fails and *new-type* is a reference type, it throws an exception that matches a handler of type [std::bad_cast](https://en.cppreference.com/w/cpp/types/bad_cast).

> NOTE: 从上述对“cast fail”情况的讨论：
>
> - 如果new-type是pointer type，则return null pointer
> - 如果new-type是reference type，则throw exception
>
> 可以看出pointer 和 reference的差异所在：存在null pointer，但是不存在reference to nothing，所以在reference type的情况下，只能够throw exception。

### 1) Add constness



### 2) Null pointer value



### 3) Upcast

Note: an implicit conversion and `static_cast` can perform this conversion as well.

### 4) To the most derived object

> NOTE:关于这一点，在cppreference [Pointer declaration#Pointer to void](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_void)中也描述了，相比而言，那里的描述更加容易理解，结合来看，我们知道这种情况下的返回值为:
>
> > a `void*` that is pointing at the complete object of the most derived type
>
> 暂时想不到这个有什么用途。
>
> 在文章panicsoftware [DYNAMIC_CAST AND TYPEID AS (NON) RTTI TOOLS.](https://blog.panicsoftware.com/dynamic_cast-and-typeid-as-non-rtti-tools/)中给出了例子:
>
> 

### 5) dynamic downcast



### Notes



## `c++` `isinstanceof` `issubclass`

http://www.cplusplus.com/forum/general/50090/

https://www.gamedev.net/forums/topic/322781-c-instanceof/

https://www.tutorialspoint.com/cplusplus-equivalent-of-instanceof

https://stackoverflow.com/questions/27595076/instanceof-equivalent-in-c

https://stackoverflow.com/questions/500493/c-equivalent-of-javas-instanceof





