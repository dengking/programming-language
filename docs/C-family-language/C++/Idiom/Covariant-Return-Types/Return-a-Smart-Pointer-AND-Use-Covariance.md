# Return a Smart Pointer AND Use Covariance



## fluentcpp [How to Return a Smart Pointer AND Use Covariance](https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/)

### Use cases



### Preamble: Separation of concerns + private virtual function

```c++
class some_class
{
public:
   std::unique_ptr<some_class> clone() const
   {
      return std::unique_ptr<some_class>(this->clone_impl());
   }
 
private:
   virtual some_class * clone_impl() const
   {
      return new some_class(*this) ;
   }
};
```

### Simple Hierarchy: Covariance + Name hiding



```c++
#include <memory>
#include <utility>


// note: this implementation does not disable this overload for array types
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class cloneable
{
public:
	virtual ~cloneable()
	{
	}

	std::unique_ptr<cloneable> clone() const
	{
		return std::unique_ptr<cloneable>(this->clone_impl());
	}

private:
	virtual cloneable * clone_impl() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

class concrete: public cloneable
{
public:
	std::unique_ptr<concrete> clone() const
	{
		return std::unique_ptr<concrete>(this->clone_impl());
	}

private:
	virtual concrete * clone_impl() const override
	{
		return new concrete(*this);
	}
};
int main()
{
	std::unique_ptr<concrete> c = make_unique<concrete>();
	std::unique_ptr<concrete> cc = c->clone();

	cloneable * p = c.get();
	std::unique_ptr<cloneable> pp = p->clone();
}


```

> NOTE: 原文给出的代码中使用到了`std::make_unique  `, 这是c++14的，所以我将原文代码进行了修改，使它在c++11的compiler中能够编译通过。
>
> `g++ --std=c++11 exp.cpp`



And using a little (usually) annoying feature in C++, name hiding (i.e. when declaring a name in a derived class, this name hides all the symbols with the same name in the base class), we hide (not override) the `clone()` member function to return a smart pointer of the exact type we wanted.

> NOTE: name hiding这个概念非常重要，参见`C++\Language-reference\Classes\Overload-VS-override-VS-name-hiding.md`

When cloning from a concrete, we obtain a `unique_ptr<concrete>`, and when cloning from a `cloneable`, we obtain a `unique_ptr<cloneable>`.

One could get uneasy at the idea of having a clone_impl member function using a RAII-unsafe transfer of ownership, but the problem is mitigated as the member function is private, and is called only by clone. This limits the risk as the user of the class can’t call it by mistake.

> NOTE: 这些考虑是非常值得学习的

### Simple Hierarchy, v2: Enter the CRTP

The CRTP is a C++ idiom that enables the injection of the derived class name into its templated base. You can learn all about it in the [series on CRTP](https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/) on Fluent C++.

```c++
#include <memory>

// note: this implementation does not disable this overload for array types
template<typename T, typename ... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
class cloneable
{
public:
	virtual ~cloneable()
	{
	}

	std::unique_ptr<cloneable> clone() const
	{
		return std::unique_ptr<cloneable>(this->clone_impl());
	}

private:
	virtual cloneable * clone_impl() const = 0;
};

template<typename Derived, typename Base>
class clone_inherit: public Base
{
public:
	std::unique_ptr<Derived> clone() const
	{
		return std::unique_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
	}

private:
	virtual clone_inherit * clone_impl() const override
	{
		return new Derived(static_cast<const Derived&>(*this));
	}
};

class concrete
: public clone_inherit<concrete, cloneable>
{

};

int main()
{
	std::unique_ptr<concrete> c = make_unique<concrete>();
	std::unique_ptr<concrete> cc = c->clone();

	cloneable * p = c.get();
	std::unique_ptr<cloneable> pp = p->clone();
}

```



> NOTE: 原文给出的代码是无法编译通过的，上面是修正过后的版本

## riptutorial [3. Covariant smart pointer result (automated cleanup).](https://riptutorial.com/cplusplus/example/19282/3--covariant-smart-pointer-result--automated-cleanup--)

```c++
#include <iostream>
#include <memory>

using namespace std;
using std::unique_ptr;

template<class Type>
auto up(Type* p)
{
	return unique_ptr<Type>(p);
}

class Top
{
private:
	virtual Top* virtual_clone() const = 0;

public:
	unique_ptr<Top> clone() const
	{
		return up(virtual_clone());
	}

	virtual ~Top() = default;       // Necessary for `delete` via Top*.
};

class D: public Top
{
private:
	/* ← Covariant return */
	D* virtual_clone() const override
	{
		return new D(*this);
	}

public:
	/* ← Apparent covariant return */
	unique_ptr<D> clone() const
	{
		return up(virtual_clone());
	}
};

class DD: public D
{
private:
	int answer_ = 42;
	/* ← Covariant return */
	DD* virtual_clone() const override
	{
		return new DD(*this);
	}

public:
	int answer() const
	{
		return answer_;
	}

	/* ← Apparent covariant return */
	unique_ptr<DD> clone() const
	{
		return up(virtual_clone());
	}
};

int main()
{
	auto p1 = unique_ptr<DD>(new DD());
	auto p2 = p1->clone();
	// Correct dynamic type DD for *p2 is guaranteed by the static type checking.

	cout << p2->answer() << endl;          // "42"
	// Cleanup is automated via unique_ptr.
}

```

