#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

/*
    type of enable_if_st is defined only when
    the first non-type value is true
 */
template<bool predicate, typename ReturnType = void>
struct enable_if // we assume predicate == false
{
    // using type = void;
};

template<typename ReturnType> // we cannot provide default value (or type) for specialization
struct enable_if<true, ReturnType> // for specialization
{
    using type = ReturnType;
};

template<bool predicate, typename ReturnType=void>
using enable_if_t = typename enable_if<predicate, ReturnType>::type;

template<typename Type>
using enable_if_int_t = enable_if_t<std::is_same_v<Type, int>, Type>;

template<typename Type>
using enable_if_double_t = enable_if_t<std::is_same_v<Type, double>, Type>;

// this function is enabled or exists only when Type == int
template<typename Type>
enable_if_int_t<Type>  // return type
remainder(Type n, Type d)
{
    return n % d;
} 

// this function exists only when Type == double
template<typename Type>
enable_if_double_t<Type>  // return type
remainder(Type n, Type d)
{
    long long q = (long long) n / d;

    return (n - q * d);
} 

void test_remainder()
{
    auto d = remainder(7.0, 4.0);
    auto i = remainder(7, 4);

    stream << "The remainder of 7.0 divided by 4.0 = "
        << d <<", type of d = " << Tpf_GetTypeCategory(d) << endl;

    stream << "The remainder of 7 divided by 4 = "
        << i <<", type of i = " << Tpf_GetTypeCategory(i) << endl;

}

int main()
{
    test_remainder();
}

