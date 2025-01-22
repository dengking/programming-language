# `std::reference_wrapper` 



## why introduce `std::reference_wrapper` in C++

In C++, the element type of a [`std::vector`](https://en.cppreference.com/w/cpp/container/vector) cannot be a reference type directly. This is because `std::vector` is designed to store its elements by value, and references are not object(see [cppreference Reference declaration](https://en.cppreference.com/w/cpp/language/reference)) and they do not have a size or a copyable nature that allows them to be stored in a container like `std::vector`.  [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) is a utility in C++ that provides a way to store references in a way that can be copied and assigned. It is part of the C++11 standard library and is included in the [`<functional>` header](https://en.cppreference.com/w/cpp/header/functional). Here are some key reasons and scenarios where `std::reference_wrapper` is useful:

### 1. Storing References in Containers

In C++, standard containers (like `std::vector`, `std::list`, etc.) store their elements by value. This means that if you try to store a reference directly, it will be sliced, and you will lose the reference. `std::reference_wrapper` allows you to store references in these containers without losing the reference semantics.

```C++
#include <functional>
#include <iostream>
#include <vector>

int main() {
    int a = 10;
    int b = 20;

    // Create a vector of reference wrappers
    std::vector<std::reference_wrapper<int>> vec;
    vec.push_back(std::ref(a));
    vec.push_back(std::ref(b));

    // Modify the original variables through the reference wrappers
    for (auto &ref: vec) {
        ref.get() += 5; // Increment the original values
    }

    std::cout << "a: " << a << ", b: " << b << std::endl; // Output: a: 15, b: 25
    return 0;
}

```

### 2. Passing References to Functions

When you need to pass references to functions that expect their parameters to be copyable, `std::reference_wrapper` can be used. This is particularly useful in callback scenarios or when using algorithms that require copyable types.

### 3. **Avoiding Dangling References**

Using `std::reference_wrapper` can help avoid dangling references in certain situations. When you store a reference directly, if the referenced object goes out of scope, the reference becomes invalid. With `std::reference_wrapper`, you can manage the lifetime of the referenced object more safely, especially when combined with smart pointers.



## [cppreference std::reference_wrapper C++11](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)  





## [cppreference std::reference_wrapper C++11](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)  # Possible implementation

```c++
#include <memory>
#include <type_traits>

namespace detail {
    template<class T>
    constexpr T &FUN(T &t) noexcept {
        return t;
    }
    template<class T>
    void FUN(T &&) = delete;
} // namespace detail

template<class T>
class reference_wrapper {
public:
    // types
    using type = T;

    // construct/copy/destroy
    template<class U, class = decltype(detail::FUN<T>(std::declval<U>()),
                                       std::enable_if_t<!std::is_same_v<reference_wrapper, std::remove_cvref_t<U>>>())>
    constexpr reference_wrapper(U &&u) noexcept(noexcept(detail::FUN<T>(std::forward<U>(u)))) :
        _ptr(std::addressof(detail::FUN<T>(std::forward<U>(u)))) {}

    reference_wrapper(const reference_wrapper &) noexcept = default;

    // assignment
    reference_wrapper &operator=(const reference_wrapper &x) noexcept = default;

    // access
    constexpr operator T &() const noexcept { return *_ptr; }
    constexpr T &get() const noexcept { return *_ptr; }

    template<class... ArgTypes>
    constexpr std::invoke_result_t<T &, ArgTypes...> operator()(ArgTypes &&...args) const
            noexcept(std::is_nothrow_invocable_v<T &, ArgTypes...>) {
        return std::invoke(get(), std::forward<ArgTypes>(args)...);
    }

private:
    T *_ptr;
};

// deduction guides
template<class T>
reference_wrapper(T &) -> reference_wrapper<T>;

```

[cppreference std::is_same](https://en.cppreference.com/w/cpp/types/is_same)







The code you've provided is a more sophisticated implementation of `std::reference_wrapper`, which is part of the C++ Standard Library. Let's break down the components of this implementation to understand how it works and what each part does.

### Breakdown of the Implementation

1. **Namespace and Detail Functions**:
   ```cpp
   namespace detail {
       template<class T> constexpr T& FUN(T& t) noexcept { return t; }
       template<class T> void FUN(T&&) = delete;
   }
   ```
   - The `detail` namespace contains a function template `FUN` that takes a reference to `T` and returns it. This function is used to ensure that only lvalue references can be passed to the `reference_wrapper` constructor.
   - The second overload of `FUN` is deleted, which prevents rvalue references from being passed. This is a way to enforce that `reference_wrapper` can only wrap lvalue references.

2. **Class Definition**:
   ```cpp
   template<class T>
   class reference_wrapper {
   public:
       using type = T;
   ```
   - The `reference_wrapper` class template is defined to hold a reference to an object of type `T`. The `using type = T;` line allows users to access the type of the wrapped reference.

3. **Constructor**:
   ```cpp
   template<class U, class = decltype(
       detail::FUN<T>(std::declval<U>()),
       std::enable_if_t<!std::is_same_v<reference_wrapper, std::remove_cvref_t<U>>>()
   )>
   constexpr reference_wrapper(U&& u)
       noexcept(noexcept(detail::FUN<T>(std::forward<U>(u))))
       : _ptr(std::addressof(detail::FUN<T>(std::forward<U>(u)))) {}
   ```
   - This constructor template allows the creation of a `reference_wrapper` from an object of type `U`, provided that `U` can be converted to `T` (using the `detail::FUN` function) and that `U` is not already a `reference_wrapper`.
   - The `std::addressof` function is used to get the address of the reference returned by `FUN`, which is stored in the private member `_ptr`.

4. **Copy Constructor and Assignment Operator**:
   ```cpp
   reference_wrapper(const reference_wrapper&) noexcept = default;
   reference_wrapper& operator=(const reference_wrapper& x) noexcept = default;
   ```
   - The copy constructor and assignment operator are defaulted, allowing for the copying of `reference_wrapper` objects.

5. **Access Operators**:
   ```cpp
   constexpr operator T& () const noexcept { return *_ptr; }
   constexpr T& get() const noexcept { return *_ptr; }
   ```
   - The conversion operator allows a `reference_wrapper` to be used as if it were a reference to `T`.
   - The `get()` method returns a reference to the underlying object.

6. **Invoke Operator**:
   ```cpp
   template<class... ArgTypes>
   constexpr std::invoke_result_t<T&, ArgTypes...>
       operator() (ArgTypes&&... args ) const
           noexcept(std::is_nothrow_invocable_v<T&, ArgTypes...>)
   {
       return std::invoke(get(), std::forward<ArgTypes>(args)...);
   }
   ```
   - This operator allows the `reference_wrapper` to be called like a function. It forwards the arguments to the underlying object, which must be callable (e.g., a function or a functor).

7. **Private Member**:
   ```cpp
   private:
       T* _ptr;
   ```
   - The private member `_ptr` holds the address of the referenced object.

8. **Deduction Guide**:
   ```cpp
   template<class T>
   reference_wrapper(T&) -> reference_wrapper<T>;
   ```
   - This deduction guide allows the compiler to automatically deduce the template parameter `T` when a `reference_wrapper` is constructed from an lvalue reference.

### Summary

This implementation of `std::reference_wrapper` provides a robust way to wrap references in a copyable and assignable manner. It ensures that only lvalue references can be wrapped, provides access to the underlying reference, and allows the wrapped object to be invoked if it is callable.

### Key Features

- **Type Safety**: The use of `decltype` and `std::enable_if_t` ensures that only valid types can be wrapped.
- **No Rvalue References**: The design prevents rvalue references from being wrapped, which is crucial for maintaining reference semantics.
- **Callable Objects**: The `operator()` allows the wrapped object to be called like a function, enhancing usability.
- **Memory Management**: The `reference_wrapper` does not manage the lifetime of the referenced object, which