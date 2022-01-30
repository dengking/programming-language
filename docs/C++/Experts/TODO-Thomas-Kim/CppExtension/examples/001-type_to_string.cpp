/**
 * @file 001-type_to_string.cpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief Examples for type::type_to_string(), Tpf_GetTypeName(), Tpf_GetTypeCategory(), Tpf_GetValueCategory().
 *
 * @version 0.1
 * @date 2019-04-10
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <iostream>
#include <tpf_types.hpp>

// Examples for type::type_to_string(), Tpf_GetTypeName(),
// Tpf_GetTypeCategory(), Tpf_GetValueCategory() 
void example_type_to_string()
{
    // namespace alias for tpf::types
    namespace types = tpf::types;

    // an instance of type int
    int n;

    // when a type type_arg is passed as argument
    // to Tpf_GetTypeName(type_arg), it returns
    // string name of the type_arg
    std::cout << "Type type name of type int : "
        << Tpf_GetTypeName(int) << std::endl;
    
    // type category of an object n of type int
    using type_category_of_n_t = decltype(n);
    
    // value category of an object n of type int
    using value_category_of_n_t = decltype((n));

    // when a type type_arg is passed as a template parameter
    // types::type_to_string<type_arg>() returns
    // string name of the type_arg
    std::cout << "Type category of n: " 
        << types::type_to_string<type_category_of_n_t>() << std::endl;

    // when a type type_arg is passed as a template parameter
    // types::type_to_string<type_arg>() returns
    // string name of the type_arg
    std::cout << "Value category of n: " 
        << types::type_to_string<value_category_of_n_t>() << std::endl;

    // when a type type_arg is passed as a template parameter
    // types::type_to_string<type_arg>() returns
    // string name of the type_arg
    std::cout << "Type category of n: "
        << Tpf_GetTypeCategory(n) << std::endl;

    // when a type type_arg is passed as a template parameter
    // types::type_to_string<type_arg>() returns
    // string name of the type_arg
    std::cout << "Value category of n: "
        << Tpf_GetValueCategory(n) << std::endl;

    // when a type type_arg is passed as a template parameter
    // types::type_to_string<type_arg>() returns
    // string name of the type_arg
    std::cout << "Type category of n: "
        << Tpf_GetTypeName(decltype(n)) << std::endl;

    // when a type type_arg is passed as a template parameter
    // types::type_to_string<type_arg>() returns
    // string name of the type_arg
    std::cout << "Value category of n: "
        << Tpf_GetTypeName(decltype((n))) << std::endl;
}

int main()
{
    example_type_to_string();
}