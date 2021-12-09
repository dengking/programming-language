#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

struct no_type_t { };

template<typename Type> 
struct is_template_st // we assume Type is not a class template
{
    using type = no_type_t;
    static constexpr bool value = false;
};

// typename, typename... means 1 or more
// template<template<typename, typename...> class ClassTemplate,
//      typename Type, typename... Types>
// struct is_template_st<ClassTemplate<Type, Types...>>
// {
//     using type = Type;
//     static constexpr bool value = true;
// };

// typename... means zero or more
template<template<typename...> class ClassTemplate, typename... Types>
struct is_template_st<ClassTemplate<Types...>>
{
    using type = no_type_t;
    static constexpr bool value = true;
};

template<typename Type>
constexpr bool is_valid_type_v =  !std::is_same_v<Type, no_type_t>;

template<typename Type> 
constexpr bool is_template_v = is_template_st<Type>::value;

template<typename Type>
using first_template_parameter_t = typename is_template_st<Type>::type;

void test_is_template_v()
{
    int a = 5;
    std::vector<int> v{1, 2, 3, 4, 5};

    stream << "The type of a: " << Tpf_GetTypeCategory(a) << endl;
    stream << "Is \'a\' a template? " << is_template_v<decltype(a)> << endl;

    stream << "The type of v: " << Tpf_GetTypeCategory(v) << endl;
    stream << "Is \'v\' a template? " << is_template_v<decltype(v)> << endl;
}

void test_first_paremter_t()
{
    int a = 5;
    std::vector<double> v{4.6, 6.7};

    if constexpr(is_template_v<decltype(a)>)
    {
        stream <<"\'a\' is a template" << endl;
    }
    else 
    {
        stream <<"\'a\' is NOT a template" << endl; 
    }
    
    if constexpr(is_template_v<decltype(v)>)
    {
        stream <<"\'v\' is a template" << endl;

        using first_t = first_template_parameter_t<decltype(v)>;

        stream <<"The type of the first parameter: "
            << Tpf_GetTypeName(first_t) << endl;

    }
    else 
    {
        stream <<"\'v\' is NOT a template" << endl; 
    }

    using tuple_t = std::tuple<>;

    if constexpr(is_template_v<tuple_t>)
    {
        stream <<"tuple_t is a template" << endl;

        using first_t = first_template_parameter_t<tuple_t>;

        if constexpr(is_valid_type_v<first_t>)
        {
            stream <<"The first parameter type of tuple_t: "
            << Tpf_GetTypeName(first_t) << endl;
        }
        else
        {
            stream <<"The first parameter type of tuple_t is EMPTY!!!"<< endl;   
        }
    }
    else
    {
        stream <<"tuple_t is NOt a template" << endl;
    }
    

}

int main()
{
    // test_is_template_v();

    test_first_paremter_t();
}