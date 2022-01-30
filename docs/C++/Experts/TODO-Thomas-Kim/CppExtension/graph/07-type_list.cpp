#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace types = tpf::types;

template<typename... Types>
void test_what_is_type_list(Types... args)
{
    using types_t = types::type_list_t<Types...>;

    stream <<"Types of arguments: " << types_t{} << endl;

    stream << "The count types: " << types::type_count_v<types_t> << endl;

    using first_t = types::select_first_type_t<types_t>;
    using second_t = types::select_nth_type_t<1, types_t>;
    using third_t = types::select_nth_type_t<2, types_t>;
    using last_t = types::select_last_type_t<types_t>;

    stream << "First type: " << Tpf_GetTypeName(first_t) << endl;
    stream << "Second type: " << Tpf_GetTypeName(second_t) << endl;
    stream << "Third type: " << Tpf_GetTypeName(third_t) << endl;
    stream << "Last type: " << Tpf_GetTypeName(last_t) << endl;

    using common_t = types::common_type_t<types_t>;

    stream << "Common type: " << Tpf_GetTypeName(common_t) << endl;
 }

template<typename... Types>
void how_to_use_type_list(Types... args)
{
    using types_t = types::type_list_t<Types...>;

    if constexpr(types::common_type_v<types_t>)
    {
        using common_t = types::common_type_t<types_t>;

        stream << "Common type exists : " << Tpf_GetTypeName(common_t) << endl; 
    }
    else
    {
        stream << "The types are : " << types_t{} << endl;

        stream << "Common type does not exist" << endl;
    }
    
    stream << "Is int in the list: " << types::is_in_list_v<int, types_t> << endl;

    stream << "All types are the same: " << types::is_same_v<types_t> << endl;
    
}

void test_advanced_type_list()
{
    using types_t = types::type_list_t<int, short, double, const char*, double, short, int>;

    stream <<"Original types: " << types_t{} << endl;

    using unique_types_t = types::unique_types_t<types_t>;

    stream <<"Unique types: " << unique_types_t{} << endl;

    using variant_t = types::to_variant_t<unique_types_t>;

    stream << "The type of variant_t: " << Tpf_GetTypeName(variant_t) << endl;

}

// template<template<typename, typename...> class ContainerType, typename... ElementTypes>
// class container_of_variant_t: public ContainerType < types::to_variant_t< 
//     types::unique_types_t<ElementTypes...> > >
// {
//     public:
//         using base_type = ContainerType < types::to_variant_t< 
//     types::unique_types_t<ElementTypes...> > >;
//         using base_type::base_type;
// };

// template<typename... ElementTypes>
// using vector_of_variants_t = container_of_variant_t<std::vector, ElementTypes...>;

// template<typename... ElementTypes>
// using deque_of_variants_t = container_of_variant_t<std::deque, ElementTypes...>;

// template<typename... ElementTypes>
// using list_of_variants_t = container_of_variant_t<std::list, ElementTypes...>;

// template<typename... ElementTypes>
// using set_of_variants_t = container_of_variant_t<std::set, ElementTypes...>;

void test_containers_of_variants()
{
    tpf::types::vector_of_variants_t<int, double, const char*, float> vv;

    vv.emplace_back(1);
    vv.emplace_back(22.0 /7.0);
    vv.emplace_back("Thomas Kim");
    vv.emplace_back("Sophie Turner");

    stream <<"vv (vector of variants) = " << vv << endl;

    tpf::types::list_of_variants_t<int, double, const char*, const wchar_t*> lv;

    lv.emplace_back(1);
    lv.emplace_back(22.0 /7.0);
    lv.emplace_back(L"Thomas Kim");
    lv.emplace_back("Sophie Turner");

    stream << "lv (list of variants) = "<< lv << endl;

    // in case of container std::set,
    // operator < () should be supported
    tpf::types::set_of_variants_t<int, double, short, long long, unsigned long> sv;

    sv.insert(1);
    sv.insert(22.0 /7.0);
    sv.insert(6ll);
    sv.insert(7ul);

    stream << "sv (set of variants) = " << sv << endl;
}


int main()
{
    // test_what_is_type_list(1, 2u, 3.0f, 4.0);

    // how_to_use_type_list(1, 2u, 3.0f, 4.0);

    // how_to_use_type_list(1, 2u, 3.0f, 4.0, "Thomas Kim");

    // how_to_use_type_list(1.0, 2.0, 4.0);

    // test_advanced_type_list();

    test_containers_of_variants();

}