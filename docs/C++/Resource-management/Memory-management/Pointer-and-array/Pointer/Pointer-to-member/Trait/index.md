# Trait

## cppreference [std::is_member_function_pointer](https://en.cppreference.com/w/cpp/types/is_member_function_pointer)



```C++
#include<type_traits>
template<class T>
struct is_member_function_pointer_helper: std::false_type
{
};

template<class T, class U>
struct is_member_function_pointer_helper<T U::*> : std::is_function<T>
{
};

template<class T>
struct is_member_function_pointer: is_member_function_pointer_helper<typename std::remove_cv<T>::type>
{
};

```

> NOTE: 
>
> 1、上述是典型的specialization+inheritance+trait
>
> 2、`struct is_member_function_pointer_helper<T U::*> : std::is_function<T>`  中的 `T` 如何理解？
>
> 根据 `std::is_function<T>`  推测: 它是一个pointer？



```C++
#include <type_traits>
 
class A {
public:
    void member() { }
};
 
int main()
{
    // fails at compile time if A::member is a data member and not a function
    static_assert(std::is_member_function_pointer<decltype(&A::member)>::value,
                  "A::member is not a member function."); 
}
```



## cppreference [std::is_member_pointer](https://en.cppreference.com/w/cpp/types/is_member_pointer)



```C++
template< class T >
struct is_member_pointer_helper         : std::false_type {};
 
template< class T, class U >
struct is_member_pointer_helper<T U::*> : std::true_type {};
 
template< class T >
struct is_member_pointer : 
    is_member_pointer_helper<typename std::remove_cv<T>::type> {};
```

