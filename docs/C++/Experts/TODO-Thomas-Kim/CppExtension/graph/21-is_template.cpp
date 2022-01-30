#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto endl = tpf::endl;

template<typename Type>
void is_template_class(Type value)
{
    stream <<"The type of Type: " << Tpf_GetTypeName(Type) << endl;
    stream <<"Is type a template? " << types::is_template_v<Type> << endl;
    stream << "The value : " << value << endl << endl;
}


void test_if_template()
{
    int i = 50;
    is_template_class(i);

    std::vector<int> v{1, 2, 3, 4, 5};

    is_template_class(v);
}

template<typename Type1, typename Type2>
void extract_template_arguments(Type1 arg1, Type2 arg2)
{
    if constexpr(types::is_template_v<Type1>)
    {
        stream << "Type1 is a template class " << endl;
        stream << "Type of Type1: " << Tpf_GetTypeName(Type1) << endl;
        stream << "The first template argment of Type1 is "
            << Tpf_GetTypeName( types::first_parameter_type_t<Type1> ) << endl;
        
        stream << "All template arguments: "
            << types::template_parameter_type_list_t<Type1>{} << endl;
    }
    else
    {
        stream << "Type1 " << Tpf_GetTypeName(Type1) 
            << " is not a template class." << endl;
    }
    
    stream << endl;

   if constexpr(types::is_template_v<Type2>)
    {
        stream << "Type2 is a template class " << endl;
        stream << "Type of Type2: " << Tpf_GetTypeName(Type2) << endl;
        stream << "The first template argment of Type2 is "
            << Tpf_GetTypeName( types::first_parameter_type_t<Type2> ) << endl;
        
        stream << "All template arguments: "
            << types::template_parameter_type_list_t<Type2>{} << endl;
    }
    else
    {
        stream << "Type2 " << Tpf_GetTypeName(Type2) 
            << " is not a template class." << endl;
    }

    stream << endl;
    
}

void test_extract_template_arguments()
{
    std::vector<int> v { 1, 2, 3, 4, 5};
    std::map<std::string, double> m;

    m["apple price"] = 100.0;
    m["peach price"] = 300.0;

    extract_template_arguments(v, m);
}

void test_template_argument_extraction()
{
    using tuple_t = std::tuple<int, double, short, const char*>;

    // extract template arguments from tuple_t
    using template_arguments = types::template_parameter_type_list_t<tuple_t>;

    using first_t = types::select_first_type_t<template_arguments>;
    using last_t = types::select_last_type_t<template_arguments>;

    stream <<"The type of tuple_t: " << Tpf_GetTypeName(tuple_t) << endl;
    stream << "The first argument: " << Tpf_GetTypeName(first_t) << endl;
    stream << "The last argument: " << Tpf_GetTypeName(last_t) << endl;
    stream << "All template arguments: " << template_arguments{} << endl;

    stream << endl;

    using variant_t = std::variant<int, double, short, const char*, long long>;

    // extract template arguments from tuple_t
    using template_arguments1 = types::template_parameter_type_list_t<variant_t>;

    using first_t1 = types::select_first_type_t<template_arguments1>;
    using last_t1 = types::select_last_type_t<template_arguments1>;

    stream <<"The type of variant_t: " << Tpf_GetTypeName(variant_t) << endl;
    stream << "The first argument: " << Tpf_GetTypeName(first_t1) << endl;
    stream << "The last argument: " << Tpf_GetTypeName(last_t1) << endl;
    stream << "All template arguments: " << template_arguments1{} << endl;

}

int main()
{
    // test_if_template();

    // test_extract_template_arguments();

    test_template_argument_extraction();
}