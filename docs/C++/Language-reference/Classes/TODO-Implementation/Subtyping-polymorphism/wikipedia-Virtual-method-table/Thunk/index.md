# Thunk



## wikipedia [Thunk](https://en.wikipedia.org/wiki/Thunk)

### Applications

#### Object-oriented programming

Thunks are useful in [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) platforms that allow a [class](https://en.wikipedia.org/wiki/Class_(computer_programming)) to [inherit multiple interfaces](https://en.wikipedia.org/wiki/Multiple_inheritance), leading to situations where the same [method](https://en.wikipedia.org/wiki/Method_(computer_programming)) might be called via any of several interfaces. The following code illustrates such a situation in [C++](https://en.wikipedia.org/wiki/C%2B%2B).

```C++
#include <iostream>
class A
{
public:
	virtual int Access() const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return value_;
	}

private:
	int value_;
};

class B
{
public:
	virtual int Access() const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return value_;
	}

private:
	int value_;
};

class C: public A, public B
{
public:
	int Access() const override
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return better_value_;
	}

private:
	int better_value_;
};

int use(B *b)
{
	return b->Access();
}

int main()
{
	// ...
	B some_b;
	use(&some_b);
	C some_c;
	use(&some_c);
}
// g++ test.cpp --std=c++11

```

> NOTE: 
>
> 1、上述程序的输出如下:
>
> ```C++
> virtual int B::Access() const
> virtual int C::Access() const
> ```
>
> 

In this example, the code generated for each of the classes `A`, `B` and `C` will include a [dispatch table](https://en.wikipedia.org/wiki/Dispatch_table) that can be used to call `Access` on an object of that type, via a reference that has the same type. 

Class `C` will have an additional dispatch table, used to call `Access` on an object of type `C` via a reference of type `B`. The expression `b->Access()` will use `B`'s own dispatch table or the additional `C` table, depending on the type of object `b` refers to. If it refers to an object of type `C`, the compiler must ensure that `C`'s `Access` implementation receives an [instance address](https://en.wikipedia.org/wiki/This_(computer_programming)) for the entire C object, rather than the inherited B part of that object.[[7\]](https://en.wikipedia.org/wiki/Thunk#cite_note-BS99-8)

> NOTE: 
>
> 1、上面这段话中的"call `Access` on an object of type `C` via a reference of type `B`"所描述的是如下情况:
>
> ```C++
>     C some_c;
> 	use(&some_c);
> ```
>
> 这是典型的dynamic type 和 static type
>
> 2、上面这段话是从implementation、底层细节的角度来描述的dynamic polymorphism
>
> 3、"If it refers to an object of type `C`, the compiler must ensure that `C`'s `Access` implementation receives an [instance address](https://en.wikipedia.org/wiki/This_(computer_programming)) for the entire C object, rather than the inherited B part of that object."
>
> 这就是下面所述的"pointer adjustment"
>
> 调用`Access`方法的时候，传入的是必须是entire object，而不能是subobject



> NOTE: 
>
> 一、上面这一段所描述的是要求，下面这一段所描述的是实现方式
>
> 二、实现方式是: 
>
> a、每个dispatch table entry都有一个integer offset字段
>
> b、每个dispatch table entry中，都包含了method implementation address
>
> c、"The code generated for each call through these dispatch tables must then retrieve the offset and use it to adjust the instance address before calling the method"
>
> 存在如下的可能性:
>
> 1、derived class
>
> 2、parent class

As a direct approach to this pointer adjustment problem, the compiler can include an **integer offset** in each dispatch table entry. This offset is the difference between the reference's address and the address required by the method implementation. The code generated for each call through these dispatch tables must then retrieve the offset and use it to adjust the instance address before calling the method.

The solution just described has problems similar to the naïve implementation of call-by-name described earlier: the compiler generates several copies of code to calculate an argument (the instance address), while also increasing the dispatch table sizes to hold the offsets. As an alternative, the compiler can generate an *adjustor thunk* along with C's implementation of `Access` that adjusts the instance address by the required amount and then calls the method. The thunk can appear in C's dispatch table for B, thereby eliminating the need for callers to adjust the address themselves.[[8\]](https://en.wikipedia.org/wiki/Thunk#cite_note-DH01-9)