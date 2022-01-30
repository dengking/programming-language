#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace types = tpf::types;

void test_what_is_type_list()
{
    using integer_types_t = 
        types::type_list_t<short, int, unsigned, long, unsigned long, long long, unsigned long long>;

    using real_types_t =
        types::type_list_t<float, double>;


    stream <<"is int in integer_types_t ? "
        << types::is_in_list_v<int, integer_types_t> << endl;

    stream <<"is int in real_types_t ? "
        << types::is_in_list_v<int, real_types_t> << endl;


    stream <<"is float in integer_types_t ? "
        << types::is_in_list_v<float, integer_types_t> << endl;

    stream <<"is float in real_types_t ? "
        << types::is_in_list_v<float, real_types_t> << endl;

}

template<typename Type, typename... Types>
using enable_if_in_types_t = std::enable_if_t<types::is_in_list_v<Type, Types...>, Type>;

template<typename Type> // for integral type
enable_if_in_types_t<Type, short, unsigned short, int, unsigned, long, unsigned long, long long, unsigned long long>
remainder(Type n, Type d)
{
    return n % d;
}

template<typename Type> // for real type
enable_if_in_types_t<Type, float, double>
remainder(Type n, Type d)
{
    long long q = (long long) n/d;

    return (n - q*d);
}

void test_remainder()
{
    stream << "The remainder of 7.0 divided by 4.0 is "
        << remainder(7.0, 4.0) << endl;

    stream << "The remainder of 7 divided by 4 is "
        << remainder(7, 4) << endl;
}


template<typename Type>
Type remainder_fn(Type n, Type d)
{
    using integral_types = types::type_list_t<short, unsigned short,
        int, unsigned, long, unsigned long, long long, unsigned long long>;

    using real_types = types::type_list_t<float, double>;

    // if Type is of integral types
    if constexpr(types::is_in_list_v<Type, integral_types>)
    {
        return n % d;
    }
    else if constexpr(types::is_in_list_v<Type, real_types>)
    {
        long long q = (long long) n / d;
        return (n - q * d);
    }
    else
    {
        static_assert(types::is_in_list_v<Type, real_types>
            || types::is_in_list_v<Type, integral_types>, "Type should be either integral of floating-point type");
    }
    
}

void test_remainder_fn()
{
    stream << "The remainder of 7.0 divided by 4.0 is "
        << remainder_fn(7.0, 4.0) << endl;

    stream << "The remainder of 7 divided by 4 is "
        << remainder_fn(7, 4) << endl;

}
int main()
{
    // test_what_is_type_list();

    // test_remainder();

    test_remainder_fn();
}