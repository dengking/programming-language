# More C++ Idioms/[Type Erasure](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure)

## Also Known As

`std::variant`, `boost::any`, `std::any`



## Motivation

It is often useful to have a variable which can contain more than one type. Type Erasure is a technique to represent a variety of concrete types through a single generic interface.

## Implementation and Example

Type Erasure is achieved in C++ by encapsulating a concrete implementation in a generic wrapper and providing virtual accessor methods to the concrete implementation via a generic interface.

> NOTE: 通过统一的interface，通过下面的例子可以看到，这个interface是由`inner_base` 定义的

The key components in this example interface are `var`, `inner_base` and `inner` classes:

```c++
#include <memory>

struct var
{
	struct inner_base
	{
		using ptr = std::unique_ptr<inner_base>;
	};
	template<typename _Ty> struct inner: inner_base
	{

	};
private:
	typename inner_base::ptr _inner;
};
```

The `var` class holds a pointer to the `inner_base` class. Concrete implementations on `inner` (such as `inner<int>` or `inner<std::string>`) inherit from `inner_base`. The `var` representation will access the concrete implementations through the generic `inner_base` interface. To hold arbitrary types of data a little more scaffolding（脚手架） is needed:

```c++
#include <memory>

struct var
{
	template<typename _Ty> var(_Ty src)
			: _inner(new inner<_Ty>(std::forward < _Ty > (src)))
	{
	} //construct an internal concrete type accessible through inner_base
	struct inner_base
	{
		using ptr = std::unique_ptr<inner_base>;
	};
	template<typename _Ty> struct inner: inner_base
	{
		inner(_Ty newval)
				: _value(newval)
		{
		}
	private:
		_Ty _value;
	};
private:
	typename inner_base::ptr _inner;
};
```



## Complete Implementation

```c++
#include "stddef.h"
#include <algorithm>
#include <memory>
#include <string>
#include <type_traits>

struct var
{
	var() : _inner(new inner<int>(0))
	{
	} //default construct to an integer

	var(const var& src)
			: _inner(src._inner->clone())
	{
	} //copy constructor calls clone method of concrete type

	template<typename _Ty> var(_Ty src)
			: _inner(new inner<_Ty>(std::forward < _Ty > (src)))
	{
	}

	template<typename _Ty> var& operator =(_Ty src)
	{ //assign to a concrete type
		_inner = std::make_unique < inner < _Ty >> (std::forward < _Ty > (src));
		return *this;
	}

	var& operator=(const var& src)
	{ //assign to another var type
		var oTmp(src);
		std::swap(oTmp._inner, this->_inner);
		return *this;
	}

	//interrogate the underlying type through the inner_base interface
	const std::type_info& Type() const
	{
		return _inner->Type();
	}
	bool IsPOD() const
	{
		return _inner->IsPOD();
	}
	size_t Size() const
	{
		return _inner->Size();
	}

	//cast the underlying type at run-time
	template<typename _Ty> _Ty& cast()
	{
		return *dynamic_cast<inner<_Ty>&>(*_inner);
	}

	template<typename _Ty> const _Ty& cast() const
	{
		return *dynamic_cast<inner<_Ty>&>(*_inner);
	}

	struct inner_base
	{
		using Pointer = std::unique_ptr < inner_base >;
		virtual ~inner_base()
		{
		}
		virtual inner_base * clone() const = 0;
		virtual const std::type_info& Type() const = 0;
		virtual bool IsPOD() const = 0;
		virtual size_t Size() const = 0;
	};

	template<typename _Ty> struct inner: inner_base
	{
		inner(_Ty newval)
				: _value(std::move(newval))
		{
		}
		virtual inner_base * clone() const override
		{
			return new inner(_value);
		}
		virtual const std::type_info& Type() const override
		{
			return typeid(_Ty);
		}
		_Ty & operator *()
		{
			return _value;
		}
		const _Ty & operator *() const
		{
			return _value;
		}
		virtual bool IsPOD() const
		{
			return std::is_pod < _Ty > ::value;
		}
		virtual size_t Size() const
		{
			return sizeof(_Ty);
		}
	private:
		_Ty _value;
	};

	inner_base::Pointer _inner;
};

//this is a specialization of an erased std::wstring
template<>
struct var::inner<std::wstring>: var::inner_base
{
	inner(std::wstring newval)
			: _value(std::move(newval))
	{
	}
	virtual inner_base * clone() const override
	{
		return new inner(_value);
	}
	virtual const std::type_info& Type() const override
	{
		return typeid(std::wstring);
	}
	std::wstring & operator *()
	{
		return _value;
	}
	const std::wstring & operator *() const
	{
		return _value;
	}
	virtual bool IsPOD() const
	{
		return false;
	}
	virtual size_t Size() const
	{
		return _value.size();
	}
private:
	std::wstring _value;
};

```



