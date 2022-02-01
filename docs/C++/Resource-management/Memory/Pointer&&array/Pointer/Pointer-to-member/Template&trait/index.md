# template and trait



## Template pointer to member function

1、github [macmade](https://github.com/macmade)/**[CPPTraits](https://github.com/macmade/CPPTraits)**

```C++
#define XS_TYPE_TRAITS_DEF_HAS_PUBLIC_METHOD( _N_, _F_ )                                                        \
    template< typename _T_, typename _R_, typename ... _A_ >                                                    \
    class _N_                                                                                                   \
    {                                                                                                           \
        private:                                                                                                \
                                                                                                                \
            struct _Y { char _[ 1 ]; };                                                                         \
            struct _N { char _[ 2 ]; };                                                                         \
                                                                                                                \
            template< typename _U_, _U_ > struct _S;                                                            \
                                                                                                                \
            template< typename _C_ > static _Y & _T( _S< _R_ ( _C_::* )( _A_ ... ), ( &_C_::_F_ ) > * );        \
            template< typename _C_ > static _N & _T( ... );                                                     \
                                                                                                                \
        public:                                                                                                 \
                                                                                                                \
            enum { value = sizeof( _T< _T_ >( nullptr ) ) == sizeof( _Y ) };                                    \
    }

```



2、cppreference [std::is_member_function_pointer](https://en.cppreference.com/w/cpp/types/is_member_function_pointer)

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



3、sourcemaking [Adapter in C++: External Polymorphism](https://sourcemaking.com/design_patterns/adapter/cpp/2)

4、[Check if a class has a member function of a given signature](https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature) # [A](https://stackoverflow.com/a/87846)

```C++
template<typename T>
struct HasUsedMemoryMethod
{
    template<typename U, size_t (U::*)() const> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, &U::used_memory>*);
    template<typename U> static int Test(...);
    static const bool Has = sizeof(Test<T>(0)) == sizeof(char);
};

template<typename TMap>
void ReportMemUsage(const TMap& m, std::true_type)
{
        // We may call used_memory() on m here.
}
template<typename TMap>
void ReportMemUsage(const TMap&, std::false_type)
{
}
template<typename TMap>
void ReportMemUsage(const TMap& m)
{
    ReportMemUsage(m, 
        std::integral_constant<bool, HasUsedMemoryMethod<TMap>::Has>());
}
```

这篇文章，收录在了`Detection-idiom`章节了。



## Trait

### cppreference [std::is_member_function_pointer](https://en.cppreference.com/w/cpp/types/is_member_function_pointer)



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



### cppreference [std::is_member_pointer](https://en.cppreference.com/w/cpp/types/is_member_pointer)



```C++
template< class T >
struct is_member_pointer_helper         : std::false_type {};
 
template< class T, class U >
struct is_member_pointer_helper<T U::*> : std::true_type {};
 
template< class T >
struct is_member_pointer : 
    is_member_pointer_helper<typename std::remove_cv<T>::type> {};
```

