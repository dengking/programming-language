# stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques)

(With type erasure, I mean hiding some or all of the type information regarding a class, somewhat like [Boost.Any](http://www.boost.org/doc/libs/1_46_0/doc/html/any.html).)

The first and most obvious, and commonly taken approach, that I know, are **virtual functions**. Just hide the implementation of your class inside an interface based class hierarchy. Many Boost libraries do this, for example [Boost.Any](http://www.boost.org/doc/libs/1_46_0/doc/html/any.html) does this to hide your type and [Boost.Shared_ptr](http://www.boost.org/doc/libs/1_46_1/libs/smart_ptr/shared_ptr.htm) does this to hide the (de)allocation mechanic.

> NOTE: 参见 "Virtual function、consistent type、consistent interface" 段

Then there is the option with function pointers to templated functions, while holding the actual object in a `void*` pointer, like [Boost.Function](http://www.boost.org/doc/libs/1_46_1/doc/html/function.html) does to hide the real type of the functor. Example implementations can be found at the end of the question.

> NOTE: function pointer

So, for my actual question:

What other type erasure techniques do you know of? Please provide them, if possible, with an example code, use cases, your experience with them and maybe links for further reading.

> NOTE: 显然，提问者的问题是: 征集type erasure techniques 。

**Edit**
(Since I wasn't sure wether to add this as an answer, or just edit the question, I'll just do the safer one.)

Another nice technique to hide the actual type of something **without** virtual functions or `void*` fiddling, is the one GMan employs [here](https://stackoverflow.com/questions/2552839/which-c-standard-library-wrapper-functions-do-you-use/2553109#2553109), with relevance to [my question](https://stackoverflow.com/questions/4985800/why-is-the-derived-classs-destructor-invoked-on-a-const-reference-to-the-base-cl) on how exactly this works.

```C++
#include <iostream>
#include <string>

/**
 * @brief The class name indicates the underlying type erasure technique
 *
 */

/**
 * @brief this behaves like the Boost.Any type w.r.t. implementation details
 *
 */
class Any_Virtual
{
	struct holder_base
	{
		virtual ~holder_base()
		{
		}
		virtual holder_base* clone() const = 0;
	};

	template<class T>
	struct holder: holder_base
	{
		holder() :
						held_()
		{
		}

		holder(T const &t) :
						held_(t)
		{
		}

		virtual ~holder()
		{
		}

		virtual holder_base* clone() const
		{
			return new holder<T>(*this);
		}

		T held_;
	};

public:
	/**
	 * @brief default constructor
	 *
	 */
	Any_Virtual() :
					storage_(0)
	{
	}
	/**
	 * @brief copy constructor
	 *
	 * @param other
	 */
	Any_Virtual(Any_Virtual const &other) :
					storage_(other.storage_->clone())
	{
	}
	/**
	 * @brief template constructor
	 *
	 * @tparam T
	 * @param t
	 */
	template<class T>
	Any_Virtual(T const &t) :
					storage_(new holder<T>(t))
	{
	}

	~Any_Virtual()
	{
		Clear();
	}

	Any_Virtual& operator=(Any_Virtual const &other)
	{
		Clear();
		storage_ = other.storage_->clone();
		return *this;
	}

	template<class T>
	Any_Virtual& operator=(T const &t)
	{
		Clear();
		storage_ = new holder<T>(t);
		return *this;
	}

	void Clear()
	{
		if (storage_)
			delete storage_;
	}

	template<class T>
	T& As()
	{
		return static_cast<holder<T>*>(storage_)->held_;
	}

private:
	holder_base *storage_;
};

/**
 * @brief the following demonstrates the use of void pointers
 * and function pointers to templated operate functions
 * to safely hide the type
 *
 */

enum Operation
{
	CopyTag, DeleteTag
};

template<class T>
void Operate(void *const&in, void *&out, Operation op)
{
	switch (op)
	{
		case CopyTag:
			out = new T(*static_cast<T*>(in));
			return;
		case DeleteTag:
			delete static_cast<T*>(out);
	}
}

/**
 * @brief 使用 void pointer 来实现 type erasure
 *
 */
class Any_VoidPtr
{
public:
	/**
	 * @brief default constructor
	 *
	 */
	Any_VoidPtr() :
					object_(0), operate_(0)
	{
	}

	Any_VoidPtr(Any_VoidPtr const &other) :
					object_(0), operate_(other.operate_)
	{
		if (other.object_)
			operate_(other.object_, object_, CopyTag);
	}
	/**
	 * @brief template constructor
	 *
	 * @tparam T
	 * @param t
	 */
	template<class T>
	Any_VoidPtr(T const &t) :
					object_(new T(t)), operate_(&Operate<T>)
	{
	}

	~Any_VoidPtr()
	{
		Clear();
	}

	Any_VoidPtr& operator=(Any_VoidPtr const &other)
	{
		Clear();
		operate_ = other.operate_;
		operate_(other.object_, object_, CopyTag);
		return *this;
	}

	template<class T>
	Any_VoidPtr& operator=(T const &t)
	{
		Clear();
		object_ = new T(t);
		operate_ = &Operate<T>;
		return *this;
	}

	void Clear()
	{
		if (object_)
			operate_(0, object_, DeleteTag);
		object_ = 0;
	}

	template<class T>
	T& As()
	{
		return *static_cast<T*>(object_);
	}

private:
	typedef void (*OperateFunc)(void* const&, void*&, Operation);

	void *object_;
	OperateFunc operate_;
};

int main()
{
	Any_Virtual a = 6;
	std::cout << a.As<int>() << std::endl;

	a = std::string("oh hi!");
	std::cout << a.As<std::string>() << std::endl;

	Any_Virtual av2 = a;

	Any_VoidPtr a2 = 42;
	std::cout << a2.As<int>() << std::endl;

	Any_VoidPtr a3 = a.As<std::string>();
	a2 = a3;
	a2.As<std::string>() += " - again!";
	std::cout << "a2: " << a2.As<std::string>() << std::endl;
	std::cout << "a3: " << a3.As<std::string>() << std::endl;

	a3 = a;
	a3.As<Any_Virtual>().As<std::string>() += " - and yet again!!";
	std::cout << "a: " << a.As<std::string>() << std::endl;
	std::cout << "a3->a: " << a3.As<Any_Virtual>().As<std::string>() << std::endl;

	std::cin.get();
}
// g++ -Wall -pedantic main.cpp && ./a.out
```

> NOTE: 输出如下:
>
> ``` C++
> 6
> oh hi!
> 42
> a2: oh hi! - again!
> a3: oh hi!
> a: oh hi!
> a3->a: oh hi! - and yet again!!
> 
> ```
>
> 



> NOTE: 
>
> 上述例子非常好，它展示三种实现方式:
>
> 1、OOP interface + template implementation
>
> 2、void pointer + function pointer



## Comments

@Xeo: I much prefer the first technic, because of the type safety it provides (using RTTI). – [Matthieu M.](https://stackoverflow.com/users/147192/matthieu-m) [Mar 27 '11 at 16:15](https://stackoverflow.com/questions/5450159/type-erasure-techniques#comment6175272_5450159)