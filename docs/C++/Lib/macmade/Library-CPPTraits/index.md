# [macmade](https://github.com/macmade)/**[CPPTraits](https://github.com/macmade/CPPTraits)**



## Read code

> NOTE: 
>
> 1、实现了非常generic的detection idiom，它的实现是基于 wikipedia [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) 中提出的实现方式
>
> 2、非常值得借鉴的一点是: 使用template来描述pointer to member function，这让我想起了expression template、[T.4: Use templates to express syntax tree manipulation](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rt-expr)
>
> 



|                                              |                     |      |
| -------------------------------------------- | ------------------- | ---- |
| `XS_TYPE_TRAITS_DEF_HAS_PUBLIC_METHOD`       | public method       |      |
| `XS_TYPE_TRAITS_DEF_HAS_PUBLIC_CONST_METHOD` | public const method |      |



### [CPPTraits](https://github.com/macmade/CPPTraits)/[CPPTraits](https://github.com/macmade/CPPTraits/tree/master/CPPTraits)/[include](https://github.com/macmade/CPPTraits/tree/master/CPPTraits/include)/[XS](https://github.com/macmade/CPPTraits/tree/master/CPPTraits/include/XS)/[TypeTraits.hpp](https://github.com/macmade/CPPTraits/blob/master/CPPTraits/include/XS/TypeTraits.hpp)

`XS_TYPE_TRAITS_DEF_HAS_PUBLIC_METHOD`

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

下面从以下几个点来理解上述code:

1、macro parameter

| macro parameter | 解释                                                         |
| --------------- | ------------------------------------------------------------ |
| `_N_`           | 这个是比较好理解的，它表示的是class template name            |
| `_F_`           | 这个是比较好理解的，它表示的需要detect的member function name |

2、template parameter

| template parameter | 解释                                      |
| ------------------ | ----------------------------------------- |
| `_T_`              | 它表示的是需要detect的class name          |
| `_R_`              | 它表示的是member function的return type    |
| `... _A_`          | 它表示的是member function的parameter type |

3、

`_R_ ( _C_::* )( _A_ ... )` 表示的是 pointer to member function  type，它是使用template来描述pointer to member function  type



4、

`&_C_::_F_` 表示的是取member function address，它也是使用template描述的

5、

`template< typename _U_, _U_ > struct _S;`   第二个template parameter是anonymous Non-type template parameter

#### 用法

初读上述macro definition的时候，可能会感到比较难以理解，结合它的用法、具体例子则是非常容易理解的:

macro `XS_TYPE_TRAITS_DEF_HAS_PUBLIC_METHOD` 的用法: 

```C++
XS_TYPE_TRAITS_DEF_HAS_PUBLIC_METHOD(       HasSimpleAssignmentOperator,            operator = );
XS_TYPE_TRAITS_DEF_HAS_PUBLIC_METHOD(       HasAdditionAssignmentOperator,          operator += );
```

在 [CPPTraits](https://github.com/macmade/CPPTraits)/[Unit-Tests](https://github.com/macmade/CPPTraits/tree/master/Unit-Tests)/[XS-TypeTraits.cpp](https://github.com/macmade/CPPTraits/blob/master/Unit-Tests/XS-TypeTraits.cpp) 中给出了例子:

```C++
class C1
{
public:

	C1(const C1 &o)
	{
		(void) o;
	}

	C1& operator =(const C1 &o)
	{
		(void) o;

		return *(this);
	}

	C1& operator +=(const C1 &o)
	{
		(void) o;

		return *(this);
	}

	C1& operator -=(const C1 &o)
	{
		(void) o;

		return *(this);
	}
};

TEST( XS_TypeTraits, HasSimpleAssignmentOperator )
{
    ASSERT_TRUE( 1 == ( XS::TypeTraits::HasSimpleAssignmentOperator< C1, C1 &, const C1 & >::value ) );
    ASSERT_TRUE( 0 == ( XS::TypeTraits::HasSimpleAssignmentOperator< C2, C2 &, const C2 & >::value ) );
}
```

### [CPPTraits](https://github.com/macmade/CPPTraits)/[Unit-Tests](https://github.com/macmade/CPPTraits/tree/master/Unit-Tests)/[XS-TypeTraits.cpp](https://github.com/macmade/CPPTraits/blob/master/Unit-Tests/XS-TypeTraits.cpp)

看了一下`class C1`，它是典型的overload operator example，可以作为学习overload operator 的典范；

