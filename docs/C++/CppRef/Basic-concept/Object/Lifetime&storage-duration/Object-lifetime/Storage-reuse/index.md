# Storage reuse

## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) # [Storage reuse](https://en.cppreference.com/w/cpp/language/lifetime#Storage_reuse)

A program is not required to call the destructor of an object to end its lifetime if the object is [trivially-destructible](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor) or if the program does not rely on the side effects of the destructor. However, if a program ends the lifetime of an **non-trivially destructible object** that is a variable explicitly, it must ensure that a new object of the same type is constructed in-place (e.g. via **placement new**) before the **destructor** may be called implicitly, i.e. due to scope exit or exception for automatic objects, due to thread exit for thread-local objects, or due to program exit for static objects; otherwise the behavior is undefined.

> NOTE: 
>
> 一、" [trivially-destructible](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor) " 其实就是 [Trivial destructor](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor) :
>
> > A trivial destructor is a destructor that performs no action. Objects with trivial destructors don't require a delete-expression and may be disposed of by simply deallocating their storage.
>
> 二、可以不调用destructor的情况:
>
> 1、 [trivially-destructible](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor) 
>
> 2、"the program does not rely on the side effects of the destructor"
>
> 三、"However, if a program ends the lifetime of an **non-trivially destructible object** that is a variable explicitly, it must ensure that a new object of the same type is constructed in-place (e.g. via placement new) before the destructor may be called implicitly, i.e. due to scope exit or exception for automatic objects, due to thread exit for thread-local objects, or due to program exit for static objects; otherwise the behavior is undefined"
>
> 这段话的意思是: 
>
> "如果想要reuse "an **non-trivially destructible object** that is a variable explicitly" 的storage，必须要在 "before the destructor may be called implicitly" 之前执行 placement new，否则就是undefined behavior"
>
> "the destructor be called implicitly"意味着底层的storage 被回收了，但是 "手工调用destructor-invoke-destructor-by-name-explicit" 并不意味着底层 storage被回收，因此它的storage是能够被reuse的，在下面的 *transparently replaceable* 章节中就给出了这样的例子
>
> 原文下面给出了example: 

```C++
#include <new>

class T {}; // trivial

struct B
{
	~B() {} // non-trivial
};

void x()
{
	long long n; // automatic, trivial
	new ( &n ) double (3.14); // reuse with a different type okay
} // okay

void h()
{
	B b; // automatic non-trivially destructible
	b.~B(); // end lifetime (not required, since no side-effects)
	new ( &b ) T; // wrong type: okay until the destructor is called
} // destructor is called: undefined behavior

int main()
{
	x();
	h();
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra -Werror

```

> NOTE: 
>
> 一、需要注意的是: 使用placement new 的时候，必须要 `#include <new>`，否则是无法编译通过的

It is undefined behavior to reuse storage that is or was occupied by a `const` complete object of static, thread-local, or automatic storage duration because such objects may be stored in read-only memory.

```C++
#include <new>
struct B
{
	B() {} // non-trivial
	~B() {} // non-trivial
};
const B b; // const static

void h()
{
	b.~B(); // end the lifetime of b
	new ( const_cast<B*>( &b ) ) const B; // undefined behavior: attempted reuse of a const
}
int main()
{
	h();
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra -Werror

```

### transparently replaceable

If a new object is created at the address that was occupied by another object, then all pointers, references, and the name of the original object will automatically refer to the new object and, once the lifetime of the new object begins, can be used to manipulate the new object, but only if the original object is ***transparently replaceable*** by the new object.

Object `x` is *transparently replaceable* by object `y` if:

1、the storage for `y` exactly overlays the storage location which `x` occupied

2、`y` is of the same type as `x` (ignoring the top-level cv-qualifiers)

3、`x` is not a complete `const` object

4、neither `x` nor `y` is a **base class subobject**, or a **member subobject** declared with `[[no_unique_address]]` (since C++20)

> NOTE: 
>
> 原文给出的上述条件比较多，目前未把握其精髓



```C++
#include <new>
#include <iostream>
struct C
{
	int i;
	void f()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	const C& operator=(const C&);
	~C()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

const C& C::operator=(const C& other)
{
	if (this != &other)
	{
		this->~C();          // lifetime of *this ends
		new ( this ) C(other); // new object of type C created
		f();                 // well-defined
	}
	return *this;
}


int main()
{
	C c1{}; // 这和原文不同
	C c2{}; // 
	c1 = c2; // well-defined
	c1.f();  // well-defined; c1 refers to a new object of type C
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra -Werror

```

> NOTE: 
>
> 一、编译warning如下:
>
> ```C++
> test.cpp: In member function ‘const C& C::operator=(const C&)’:
> 
> test.cpp:22:37: warning: implicitly-declared ‘constexpr C::C(const C&)’ is deprecated [-Wdeprecated-copy]
> 
>    22 |                 new ( this ) C(other); // new object of type C created
> 
>       |                                     ^
> 
> test.cpp:17:10: note: because ‘C’ has user-provided ‘const C& C::operator=(const C&)’
> 
>    17 | const C& C::operator=(const C& other)
> ```
>
> 



### 使用 [std::launder](https://en.cppreference.com/w/cpp/utility/launder) 来突破***transparently replaceable***的requirement(since C++17)

If the conditions listed above are not met, a valid pointer to the new object may still be obtained by applying the pointer optimization barrier [std::launder](https://en.cppreference.com/w/cpp/utility/launder):

```C++
#include <new>
#include <iostream>
#include <cassert>
struct A
{
	virtual int transmogrify();
};

struct B : A
{
	int transmogrify() override {
        std::cout << __PRETTY_FUNCTION__ << std::endl;        
		::new( this ) A; // 将当前对象替换为A，这是在子类上构造基类，将子类替换为基类
		return 2;
	}
};

inline int A::transmogrify() { 
   
    std::cout << __PRETTY_FUNCTION__ << std::endl;
	::new( this ) B; // 将当前对象替换为B，这是在基类上构造子类，将基类替换为子类
	return 1; 
}

void test()
{
	A i;
	int n = i.transmogrify(); // 此时 i 是 B
    std::cout << n << std::endl;
	// int m = i.transmogrify(); // undefined behavior:
	// the new A object is a base subobject, while the old one is a complete object
	int m = std::launder(&i)->transmogrify(); // OK
    std::cout << m << std::endl;    
	assert(m + n == 3);
}

int main()
{
	test();
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra -Werror

```

> NOTE: 
>
> 一、上述程序的输出如下:
>
> ```C++
> virtual int A::transmogrify()
> 
> virtual int B::transmogrify()
> ```
>
> 二、`B`是`A`的子类，它们两者的storage是相同的，因此实际上可以相互转换的，但是并不满足前面的transparently replaceable的requirement，因此就需要使用 `std::launder`
>
> 

Similarly, if an object is created in the storage of a class member or array element, the created object is only a subobject (member or element) of the original object's containing object if:

1、the lifetime of the containing object has begun and not ended

2、the storage for the new object exactly overlays the storage of the original object

3、the new object is of the same type as the original object (ignoring cv-qualification).

Otherwise, the name of the original subobject cannot be used to access the new object without [std::launder](https://en.cppreference.com/w/cpp/utility/launder) (since C++17)

### byte array

As a special case, objects can be created in arrays of unsigned char or [`std::byte`](https://en.cppreference.com/w/cpp/types/byte) (since C++17) (in which case it is said that the array *provides storage* for the object) if

1、the lifetime of the array has begun and not ended

2、the storage for the new object fits entirely within the array

3、there is no array object that satisfies these constraints nested within the array.

If that portion of the array previously provided storage for another object, the lifetime of that object ends because its storage was reused, however the lifetime of the array itself does not end (its storage is not considered to have been reused)



```C++
#include <new>
#include <iostream>
template<typename... T>
struct AlignedUnion
{
	alignas( T... ) unsigned char data[std::max(sizeof(T)...)];
};

int f()
{
	AlignedUnion<int, char> au;
	int* p = new ( au.data ) int;     // OK, au.data provides storage
	char* c = new ( au.data ) char(); // OK, ends lifetime of *p
	char* d = new ( au.data + 1 ) char();
	return *c + *d; // OK
}

int main()
{
	std::cout << f() << std::endl;
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra -Werror

```

