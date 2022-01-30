#include <tpf_output.hpp>
/*
    The primary reason for type dispatch is to resolve
    conflicting function overloading.

    For example,

    template<typename RealType>
    RealType remainer(RealType n, RealType d, true_type)
    {

    }

    template<typename IntType>
    IntType remainer(IntType n, IntType d, false_type)
    {

    }

    In C++14 stanard, _v suffix was introduced as below

 */
tpf::sstream stream;
auto endl = tpf::endl;

double remainder(double n, double d, double )
{
    long long q = (long long)n/d;

    return (n - q * d);
}

void test_remainder()
{
    auto result_d = remainder(7.0, 4.0, 0.0);

    stream << "result_d = " << result_d << endl;
}

template<typename Type, Type v>
struct integral_constant
{
    static constexpr Type value = v;
    using value_type = Type;

    using type = integral_constant;

    // conversion to value_type (or Type) operator
    constexpr operator value_type() const noexcept
    {
        return value;
    }

    // function call operator()
    constexpr Type operator()() const noexcept
    {
        return value;
    }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template<typename Type_1, typename Type_2>
struct is_same // we assume Type_1 and Type_2 are different
{
    // we defined an instance value of type false_type
    static constexpr false_type value = false_type{};
    using type = false_type;
};

template<typename Type>
struct is_same<Type, Type> // Type, Type are the same
{
    // we defined an instance value of type true_type
    static constexpr true_type value = true_type{};
    using type = true_type;
};

// defined since C++14 standard
template<typename Type_1, typename Type_2>
constexpr auto is_same_v = is_same<Type_1, Type_2>::value();

constexpr bool is_same_type(true_type /* parameter name is missing */)
{
    return true;
}

constexpr bool is_same_type(false_type /* parameter name is missing */)
{
    return false;
}

void test_is_same()
{
    stream <<"int, int : " 
        <<  is_same_type( is_same<int, int>::value ) << endl;

    stream <<"int, double : " 
        <<  is_same_type( is_same<int, double>::value ) << endl;
}

template<typename RealType>
RealType remainder_impl(RealType n, RealType d, true_type)
{
    long long q = (long long)n/d;

    return (n - q * d);
}

template<typename IntType>
IntType remainder_impl(IntType n, IntType d, false_type)
{
    return n % d;
}

template<typename Type>
Type remainder(Type n, Type d)
{
    return remainder_impl(n, d, is_same<Type, double>::value);
}

void test_type_dispatch()
{
    stream << "The remainder 7.0 divided by 4.0 is "
            << remainder(7.0, 4.0) << endl;

    stream << "The remainder 7 divided by 4 is "
        << remainder(7, 4) << endl;
}

void test_is_same_v()
{
    stream << "is_same_v<int, int>: " << is_same_v<int, int> << endl;

    stream << "is_same_v<int, double>: " << is_same_v<int, double> << endl;
}

int main()
{
    // test_remainder();

    // test_is_same();

    // test_type_dispatch();

    test_is_same_v();
}