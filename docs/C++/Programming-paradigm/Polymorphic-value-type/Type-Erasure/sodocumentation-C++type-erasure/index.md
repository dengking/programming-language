# sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

> NOTE: 
>
> 1、非常好的一篇文章，对C++ type erasure进行了非常好的总结

## Introduction

Type erasure is a set of techniques for creating a type that can provide a **uniform interface** to various underlying types, while hiding the underlying type information from the client. `std::function<R(A...)>`, which has the ability to hold callable objects of various types, is perhaps the best known example of type erasure in C++.

> NOTE: 
>
> 1、上述 "**uniform interface**"，其实就是consistent interface

## Basic mechanism

**Type erasure** is a way to hide the type of an object from code using it, even though it is not derived from a **common base class**. In doing so, it provides a bridge between the worlds of **static polymorphism** (templates; at the place of use, the exact type must be known at compile time, but it need not be declared to conform to an **interface** at definition) and **dynamic polymorphism** (inheritance and virtual functions; at the place of use, the exact type need not be known at compile time, but must be declared to conform to an interface at definition).

> NOTE:
>
> 1、polymorphic value type-融合OOP-和-GP的优势

### Basic mechanism of type erasure

The following code shows the basic mechanism of type erasure.

```C++
#include <ostream>

class Printable
{
public:
  template <typename T>
  Printable(T value) : pValue(new Value<T>(value)) {}
  ~Printable() { delete pValue; }
  void print(std::ostream &os) const { pValue->print(os); }

private:
  Printable(Printable const &)        /* in C++1x: =delete */; // not implemented
  void operator = (Printable const &) /* in C++1x: =delete */; // not implemented
  struct ValueBase
  {
      virtual ~ValueBase() = default;
      virtual void print(std::ostream &) const = 0;
  };
  template <typename T>
  struct Value : ValueBase
  {
      Value(T const &t) : v(t) {}
      virtual void print(std::ostream &os) const { os << v; }
      T v;
  };
  ValueBase *pValue;
};
```

At the use site, only the above definition need to be visible, just as with base classes with virtual functions. For example:

```C++
#include <iostream>

void print_value(Printable const &p)
{
    p.print(std::cout);
}
```

Note that this is *not* a template, but a normal function that only needs to be declared in a header file, and can be defined in an implementation file (unlike templates, whose definition must be visible at the place of use).

At the definition of the concrete type, nothing needs to be known about `Printable`, it just needs to conform to an interface, as with templates:

```C++
struct MyType { int i; };
ostream& operator << (ostream &os, MyType const &mc)
{
  return os << "MyType {" << mc.i << "}";
}
```

We can now pass an object of this class to the function defined above:

```C++
MyType foo = { 42 };
print_value(foo);
```

### 完整测试程序

```C++
#include <iostream>

class Printable
{
public:
	template<typename T>
	Printable(T value) :
					pValue(new Value<T>(value))
	{
	}
	~Printable()
	{
		delete pValue;
	}
	void print(std::ostream &os) const
	{
		pValue->print(os);
	}

private:
	Printable(Printable const&) /* in C++1x: =delete */; // not implemented
	void operator =(Printable const&) /* in C++1x: =delete */; // not implemented
	struct ValueBase
	{
		virtual ~ValueBase() = default;
		virtual void print(std::ostream&) const = 0;
	};
	template<typename T>
	struct Value: ValueBase
	{
		Value(T const &t) :
						v(t)
		{
		}
		virtual void print(std::ostream &os) const
		{
			os << v;
		}
		T v;
	};
	ValueBase *pValue;
};

struct MyType
{
	int i;
};
std::ostream& operator <<(std::ostream &os, MyType const &mc)
{
	return os << "MyType {" << mc.i << "}";
}

void print_value(Printable const &p)
{
	p.print(std::cout);
}

int main()
{
	Printable p { 1 };
	print_value(p);
	print_value(1);
	MyType foo = { 42 };
	print_value(foo);
}
// g++ test.cpp
```



## Erasing down to a Regular type with manual vtable

> NOTE: 
>
> 1、custom virtual table
>
> 2、在 stlab [Small Object Optimization for Polymorphic Types](https://stlab.cc/tip/2017/12/23/small-object-optimizations.html) 中，我们知道[`adobe::any_regular_t`](https://github.com/stlab/adobe_source_libraries/blob/master/adobe/any_regular.hpp) 采用的类似的实现方式

C++ thrives on what is known as a Regular type (or at least Pseudo-Regular).

> NOTE: 
>
> 一、上面这段话的意思是: C++是在regular type的基础上成长起来的

### Regular type

A Regular type is a type that can be constructed and assigned-to and assigned-from via copy or move, can be destroyed, and can be compared equal-to. It can also be constructed from no arguments. Finally, it also has support for a few other operations that are highly useful in various `std` algorithms and containers.

> NOTE: 
>
> 其实regular type也是一个concept

### Implementation

[This is the root paper](http://www.stepanovpapers.com/DeSt98.pdf), but in C++11 would want to add `std::hash` support.

I will use the manual vtable approach to type erasure here.



```C++
#include <utility>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std::literals;

using dtor_unique_ptr = std::unique_ptr<void, void( * )( void* )>;

template<class T, class...Args>
dtor_unique_ptr make_dtor_unique_ptr(Args&&... args) {
	return { new T(std::forward<Args>(args)...), [](void* self) { delete static_cast<T*>( self ); } };
}
struct regular_vtable {
	void( *copy_assign )( void* dest, void const* src ); // T&=(T const&)
	void( *move_assign )( void* dest, void* src ); // T&=(T&&)
	bool( *equals )( void const* lhs, void const* rhs ); // T const&==T const&
	bool( *order )( void const* lhs, void const* rhs ); // std::less<T>{}(T const&, T const&)
	std::size_t(*hash)( void const* self ); // std::hash<T>{}(T const&)
	std::type_info const& ( *type )( ); // typeid(T)
	dtor_unique_ptr(*clone)( void const* self ); // T(T const&)
};

template<class T>
regular_vtable make_regular_vtable() noexcept {
	return {
	  [](void* dest, void const* src) { *static_cast<T*>( dest ) = *static_cast<T const*>( src ); },
	  [](void* dest, void* src) { *static_cast<T*>( dest ) = std::move(*static_cast<T*>( src )); },
	  [](void const* lhs, void const* rhs) { return *static_cast<T const*>( lhs ) == *static_cast<T const*>( rhs ); },
	  [](void const* lhs, void const* rhs) { return std::less<T>{}( *static_cast<T const*>( lhs ),*static_cast<T const*>( rhs ) ); },
	  [](void const* self) { return std::hash<T>{}( *static_cast<T const*>( self ) ); },
	  []()->decltype( auto ) { return typeid( T ); },
	  [](void const* self) { return make_dtor_unique_ptr<T>(*static_cast<T const*>( self )); }
	};
}
template<class T>
regular_vtable const* get_regular_vtable() noexcept {
	static const regular_vtable vtable = make_regular_vtable<T>();
	return &vtable;
}

struct regular_type {
	using self = regular_type;
	regular_vtable const* vtable = 0;
	dtor_unique_ptr ptr{ nullptr, [](void*) {} };

	bool empty() const { return !vtable; }

	template<class T, class...Args>
	void emplace(Args&&... args) {
		ptr = make_dtor_unique_ptr<T>(std::forward<Args>(args)...);
		if (ptr)
			vtable = get_regular_vtable<T>();
		else
			vtable = nullptr;
	}
	friend bool operator==(regular_type const& lhs, regular_type const& rhs) {
		if (lhs.vtable != rhs.vtable) return false;
		return lhs.vtable->equals(lhs.ptr.get(), rhs.ptr.get());
	}
	bool before(regular_type const& rhs) const {
		auto const& lhs = *this;
		if (!lhs.vtable || !rhs.vtable)
			return std::less<regular_vtable const*>{}( lhs.vtable, rhs.vtable );
		if (lhs.vtable != rhs.vtable)
			return lhs.vtable->type().before(rhs.vtable->type());
		return lhs.vtable->order(lhs.ptr.get(), rhs.ptr.get());
	}
	// technically friend bool operator< that calls before is also required

	std::type_info const* type() const {
		if (!vtable) return nullptr;
		return &vtable->type();
	}
	regular_type(regular_type&& o) :
		vtable(o.vtable),
		ptr(std::move(o.ptr))
	{
		o.vtable = nullptr;
	}
	friend void swap(regular_type& lhs, regular_type& rhs) {
		std::swap(lhs.ptr, rhs.ptr);
		std::swap(lhs.vtable, rhs.vtable);
	}
	regular_type& operator=(regular_type&& o) {
		if (o.vtable == vtable) {
			vtable->move_assign(ptr.get(), o.ptr.get());
			return *this;
		}
		auto tmp = std::move(o);
		swap(*this, tmp);
		return *this;
	}
	regular_type(regular_type const& o) :
		vtable(o.vtable),
		ptr(o.vtable ? o.vtable->clone(o.ptr.get()) : dtor_unique_ptr{ nullptr, [](void*) {} })
	{
		if (!ptr && vtable) vtable = nullptr;
	}
	regular_type& operator=(regular_type const& o) {
		if (o.vtable == vtable) {
			vtable->copy_assign(ptr.get(), o.ptr.get());
			return *this;
		}
		auto tmp = o;
		swap(*this, tmp);
		return *this;
	}
	std::size_t hash() const {
		if (!vtable) return 0;
		return vtable->hash(ptr.get());
	}
	template<class T,
		std::enable_if_t < !std::is_same<std::decay_t<T>, regular_type>{}, int > * = nullptr
	>
		regular_type(T && t) {
		emplace<std::decay_t<T>>(std::forward<T>(t));
	}
};
namespace std {
	template<>
	struct hash<regular_type> {
		std::size_t operator()(regular_type const& r)const {
			return r.hash();
		}
	};
	template<>
	struct less<regular_type> {
		bool operator()(regular_type const& lhs, regular_type const& rhs) const {
			return lhs.before(rhs);
		}
	};
}

int main() {
	std::unordered_map< regular_type, int > uom{
	  std::make_pair(1,1),
	  std::make_pair("hello"s, 2),
	  std::make_pair(2,3),
	};
	std::map< regular_type, int > om{
	  {1,1},
	  {"hello"s, 2},
	  {2,3},
	};
	uom[1] = -10;
	for (auto&& e : uom)
		std::cout << e.second << '\n';
	om["hello"s] = -10;
	for (auto&& e : om)
		std::cout << e.second << '\n';
}
// clang++ -std=c++14 -O2 -Wall -pedantic -pthread main.cpp && ./a.out
```

> NOTE: 
>
> 一、实现总结
>
> 1、abstract  type + template object generator 
>
> `dtor_unique_ptr` + `make_dtor_unique_ptr`
>
> `regular_vtable` + `make_regular_vtable`

[live example](http://coliru.stacked-crooked.com/a/28ef6be761012a81).

Such a regular type can be used as a key for a `std::map` or a `std::unordered_map` that accepts *anything regular* for a key, like:

```C++
std::map<regular_type, std::any>
```

would be basically a map from anothing regular, to anything copyable.

Unlike `any`, my `regular_type` does no small object optimization nor does it support getting the original data back. Getting the original type back isn't hard.

Small object optimization requires that we store an aligned storage buffer within the `regular_type`, and carefully tweak the deleter of the `ptr` to only destroy the object and not delete it.

I would start at `make_dtor_unique_ptr` and teach it how to sometimes store the data in a buffer, and then in the heap if no room in the buffer. That may be sufficient.





## A move-only `std::function`

`std::function` type erases down to a few operations. One of the things it requires is that the stored value be copyable.

This causes problems in a few contexts, like lambdas storing unique ptrs. If you are using the `std::function` in a context where copying doesn't matter, like a thread pool where you dispatch tasks to threads, this requirement can add overhead.

In particular, `std::packaged_task<Sig>` is a callable object that is move-only. You can store a `std::packaged_task<R(Args...)>` in a `std::packaged_task<void(Args...)>`, but that is a pretty heavy-weight and obscure way to create a move-only callable type-erasure class.

> NOTE: 
>
> 1、上面没有读懂

Thus the `task`. This demonstrates how you could write a simple `std::function` type. I omitted the copy constructor (which would involve adding a `clone` method to `details::task_pimpl<...>` as well).

```C++
#include <iostream>
#include <memory>
#include <utility>

template<class Sig>
struct task;

// putting it in a namespace allows us to specialize it nicely for void return value:
namespace details {
  template<class R, class...Args>
  struct task_pimpl {
    virtual R invoke(Args&&...args) const = 0;
    virtual ~task_pimpl() {};
    virtual const std::type_info& target_type() const = 0;
  };

  // store an F.  invoke(Args&&...) calls the f
  template<class F, class R, class...Args>
  struct task_pimpl_impl:task_pimpl<R,Args...> {
    F f;
    template<class Fin>
    task_pimpl_impl( Fin&& fin ):f(std::forward<Fin>(fin)) {}
    virtual R invoke(Args&&...args) const final override {
      return f(std::forward<Args>(args)...);
    }
    virtual const std::type_info& target_type() const final override {
      return typeid(F);
    }
  };

  // the void version discards the return value of f:
  template<class F, class...Args>
  struct task_pimpl_impl<F,void,Args...>:task_pimpl<void,Args...> {
    F f;
    template<class Fin>
    task_pimpl_impl( Fin&& fin ):f(std::forward<Fin>(fin)) {}
    virtual void invoke(Args&&...args) const final override {
      f(std::forward<Args>(args)...);
    }
    virtual const std::type_info& target_type() const final override {
      return typeid(F);
    }
  };
};

template<class R, class...Args>
struct task<R(Args...)> {
  // semi-regular:
  task()=default;
  task(task&&)=default;
  // no copy

private:
  // aliases to make some SFINAE code below less ugly:
  template<class F>
  using call_r = std::result_of_t<F const&(Args...)>;
  template<class F>
  using is_task = std::is_same<std::decay_t<F>, task>;
public:
  // can be constructed from a callable F
  template<class F,
    // that can be invoked with Args... and converted-to-R:
    class= decltype( (R)(std::declval<call_r<F>>()) ),
    // and is not this same type:
    std::enable_if_t<!is_task<F>{}, int>* = nullptr
  >
  task(F&& f):
    m_pImpl( make_pimpl(std::forward<F>(f)) )
  {}

  // the meat: the call operator
  R operator()(Args... args)const {
        return m_pImpl->invoke( std::forward<Args>(args)... );
  }
  explicit operator bool() const {
    return (bool)m_pImpl;
  }
  void swap( task& o ) {
    std::swap( m_pImpl, o.m_pImpl );
  }
  template<class F>
  void assign( F&& f ) {
    m_pImpl = make_pimpl(std::forward<F>(f));
  }
  // Part of the std::function interface:
  const std::type_info& target_type() const {
    if (!*this) return typeid(void);
    return m_pImpl->target_type();
  }
  template< class T >
  T* target() {
    return target_impl<T>();
  }
  template< class T >
  const T* target() const {
    return target_impl<T>();
  }
  // compare with nullptr    :
  friend bool operator==( std::nullptr_t, task const& self ) { return !self; }
  friend bool operator==( task const& self, std::nullptr_t ) { return !self; }
  friend bool operator!=( std::nullptr_t, task const& self ) { return !!self; }
  friend bool operator!=( task const& self, std::nullptr_t ) { return !!self; }
private:
  template<class T>
  using pimpl_t = details::task_pimpl_impl<T, R, Args...>;

  template<class F>
  static auto make_pimpl( F&& f ) {
    using dF=std::decay_t<F>;
    using pImpl_t = pimpl_t<dF>;
    return std::make_unique<pImpl_t>(std::forward<F>(f));
  }
  std::unique_ptr<details::task_pimpl<R,Args...>> m_pImpl;

  template< class T >
  T* target_impl() const {
    return dynamic_cast<pimpl_t<T>*>(m_pImpl.get());
  }
};

```

To make this library-worthy, you'd want to add in a small buffer optimization, so it does not store every callable on the heap.

Adding SBO would require a non-default `task(task&&)`, some `std::aligned_storage_t` within the class, a `m_pImpl` `unique_ptr` with a deleter that can be set to destroy-only (and not return the memory to the heap), and a `emplace_move_to( void* ) = 0` in the `task_pimpl`.

[live example](http://coliru.stacked-crooked.com/a/6e6811e8626a37d1) of the above code (with no SBO).

## Erasing down to a contiguous buffer of T

> NOTE: 
>
> 1、这种type erasure是比较有局限性的
>
> 2、`array_view`其实就是`span`

Not all type erasure involves virtual inheritance, allocations, placement new, or even function pointers.

What makes type erasure type erasure is that it describes a (set of) behavior(s), and takes any type that supports that behavior and wraps it up. All information that isn't in that set of behaviors is "forgotten" or "erased".

An `array_view` takes its incoming range or container type and erases everything except the fact it is a contiguous buffer of `T`.

```C++
#include <type_traits>
#include <utility>

/**
 * helper traits for SFINAE:
 */
template<class T>
using data_t = decltype( std::declval<T>().data() );

template<class Src, class T>
using compatible_data = std::integral_constant<bool, std::is_same< data_t<Src>, T* >{} || std::is_same< data_t<Src>, std::remove_const_t<T>* >{}>;

template<class T>
struct array_view {
  // the core of the class:
  T* b=nullptr;
  T* e=nullptr;
  T* begin() const { return b; }
  T* end() const { return e; }

  // provide the expected methods of a good contiguous range:
  T* data() const { return begin(); }
  bool empty() const { return begin()==end(); }
  std::size_t size() const { return end()-begin(); }

  T& operator[](std::size_t i)const{ return begin()[i]; }
  T& front()const{ return *begin(); }
  T& back()const{ return *(end()-1); }

  // useful helpers that let you generate other ranges from this one
  // quickly and safely:
  array_view without_front( std::size_t i=1 ) const {
    i = (std::min)(i, size());
    return {begin()+i, end()};
  }
  array_view without_back( std::size_t i=1 ) const {
    i = (std::min)(i, size());
    return {begin(), end()-i};
  }

  // array_view is plain old data, so default copy:
  array_view(array_view const&)=default;
  // generates a null, empty range:
  array_view()=default;

  // final constructor:
  array_view(T* s, T* f):b(s),e(f) {}
  // start and length is useful in my experience:
  array_view(T* s, std::size_t length):array_view(s, s+length) {}

  // SFINAE constructor that takes any .data() supporting container
  // or other range in one fell swoop:
  template<class Src,
    std::enable_if_t< compatible_data<std::remove_reference_t<Src>&, T >{}, int>* =nullptr,
    std::enable_if_t< !std::is_same<std::decay_t<Src>, array_view >{}, int>* =nullptr
  >
  array_view( Src&& src ):
    array_view( src.data(), src.size() )
  {}

  // array constructor:
  template<std::size_t N>
  array_view( T(&arr)[N] ):array_view(arr, N) {}

  // initializer list, allowing {} based:
  template<class U,
    std::enable_if_t< std::is_same<const U, T>{}, int>* =nullptr
  >
  array_view( std::initializer_list<U> il ):array_view(il.begin(), il.end()) {}
};

```

an `array_view` takes any container that supports `.data()` returning a pointer to `T` and a `.size()` method, or an array, and erases it down to being a random-access range over contiguous `T`s.

It can take a `std::vector<T>`, a `std::string<T>` a `std::array<T, N>` a `T[37]`, an initializer list (including `{}` based ones), or something else you make up that supports it (via `T* x.data()` and `size_t x.size()`).

In this case, the data we can extract from the thing we are erasing, together with our "view" non-owning state, means we don't have to allocate memory or write custom type-dependent functions.

[Live example](http://coliru.stacked-crooked.com/a/c9f8e013a309ca66).

An improvement would be to use a non-member `data` and a non-member `size` in an ADL-enabled context.

## Type erasing type erasure with `std::any`

> NOTE: 
>
> 1、未阅读完成

This example uses C++14 and `boost::any`. In C++17 you can swap in `std::any` instead.

The syntax we end up with is:

```C++
const auto print =
  make_any_method<void(std::ostream&)>([](auto&& p, std::ostream& t){ t << p << "\n"; });

super_any<decltype(print)> a = 7;

(a->*print)(std::cout);
```

which is almost optimal.



First we use a tag to pass around types:

```C++
template<class T>struct tag_t{constexpr tag_t(){};};
template<class T>constexpr tag_t<T> tag{};
```

> NOTE: 
>
> 1、"constexpr constructor literal type tag for dispatch idiom"