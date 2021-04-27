# stackexchange [Polymorphic (owned) reference wrapper for class hierarchies](https://codereview.stackexchange.com/questions/54371/polymorphic-owned-reference-wrapper-for-class-hierarchies)

> NOTE: 
>
> 1、典型的 Polymorphic-containers-with-value-semantic

```C++
#pragma once

#include <memory>
#include <cassert>
#include <functional>
#include <stdexcept>
#include <vector>

namespace stdex
{
inline namespace details
{

/// @brief Deep copy construct from (Specialized&)*src
///
/// @retval nullptr if src is nullptr
/// @retval Specialized clone of *src
///
/// @note Undefined behavior if src does not point to a Specialized*
template<typename Base, typename Specialized>
Base* polymorphic_clone(const Base *src)
{
	static_assert(std::is_base_of<Base, Specialized>::value,
					"Specialized is not a specialization of Base");

	if (src == nullptr)
		return nullptr;
	return new Specialized { static_cast<const Specialized&>(*src) };
}
}

/// @brief polymorphic reference interface over a base class
///
/// Respects polymorphic behavior of class ref.
/// Instances have deep copy semantics (clone) and
/// "[const] Base&" interface
///
/// @note Not regular: no trivial way to implement non-intrusive equality
///
/// @note safe to use with standard containers
template<typename Base>
class polymorphic final
{
public:

	/// Functor capable to convert a Base* to it's specialized type
	/// and clone it (intrusive implementation can be used)
	///
	/// example intrusive implementation (if supported by Base):
	/// []( const Base* src ) { return src->clone(); }
	typedef std::function<Base* (const Base*)> clone_functor;

	/// @brief construct (takes ownership of ptr)
	template<typename Specialized, typename CloneSpecialized>
	polymorphic(Specialized *ptr, CloneSpecialized functor) noexcept :
					instance_ { ptr }, clone_ { std::move(functor) }
	{
		static_assert(std::is_base_of<Base, Specialized>::value,
						"Specialized is not a specialization of Base");
		static_assert(
						std::is_constructible<clone_functor, CloneSpecialized>::value,
						"CloneSpecialized is not valid for a clone functor");
	}

	// not implemented: UB cloning in case client provides specialized ptr
	// polymorphic(Base* ptr);

	// @note empty constructor for std:: containers support
	polymorphic() = default;

	polymorphic(polymorphic&&) = default;

	polymorphic(const polymorphic &other)
	// : polymorphic{std::move(other.clone())}
	:
					polymorphic { other.clone() } // comment by @dyp
	{
	}

	// polymorphic& operator=(polymorphic other)
	polymorphic& operator=(polymorphic other) noexcept // comment by @dyp
	{
		std::swap(instance_, other.instance_);
		std::swap(clone_, other.clone_);
		return *this;
	}

	~polymorphic() = default;

	/// @brief Cast to contained type
	/// @pre instance not moved
	/// @pre *this initialized with valid instance
	operator Base&() const
	{
		assert(instance_.get());
		return *instance_.get();
	}

	/// @brief Cast to contained type
	/// @pre instance not moved
	/// @pre *this initialized with valid instance
	operator const Base&() const
	{
		assert(instance_.get());
		return *instance_.get();
	}

private:
	polymorphic clone() const
	{
		return
		{	clone_(instance_.get()), clone_functor
			{	clone_}};
	}

	std::unique_ptr<Base> instance_;
	clone_functor clone_;
};

// edited after comment by @dyp
template<typename Base, typename Specialized, typename CF>
polymorphic<Base> to_polymorphic(Specialized &&temp, CF functor)
{
	return
	{
		new Specialized
		{	std::move(temp)},
		typename polymorphic<Base>::clone_functor
		{	std::move(functor)}
	};
}

template<typename Base, typename Specialized>
polymorphic<Base> to_polymorphic(Specialized &&temp)
{
	static_assert(std::is_base_of<Base, Specialized>::value,
					"Specialized is not a specialization of Base");

	return to_polymorphic<Base, Specialized>(std::move(temp), polymorphic_clone<Base, Specialized>);
}

template<typename Base, typename Specialized, typename ...Args>
// polymorphic<Base> to_polymorphic(Args ...args)
polymorphic<Base> to_polymorphic(Args &&...args) // comment by @dyp
{
	static_assert(std::is_constructible<Specialized, Args...>::value,
					"Cannot instantiate Specialized from arguments");

	return to_polymorphic<Base, Specialized>(std::move(Specialized { std::forward<Args...>(args...) }));
}

template<typename Base> using polymorphic_vector =
std::vector<polymorphic<Base>>;

template<typename Base, typename ...Args>
polymorphic_vector<Base> to_polymorphic_vector(Args &&...args)
{
	// comment by @dyp (add std::forward)
	return
	{
		to_polymorphic<Base>(std::forward<Args>(args))...
	};
}

} // stdex

```



Example use (using a class hierarchy based on `view`, a generic responder for HTTP requests - the implementation of view is not important here, I just had it in existing code):

```cpp
stdex::polymorphic_vector<view> views = // explicit type for clarity
    stdex::to_polymorphic_vector<view>(
        echo_view{"/echo"}, // class echo_view : public view
        directory_view{"/static_files", "~/http-server/static"}
            // class directory_view : public view
    );

for(auto& v: views)
    if(v.matches(reuqest.url())) // bool view::matches(...);
        auto response = v.handle(request); // virtual view::handle(...) = 0;
```