#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

template<typename Type>
using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<Type>>;

template<typename Type>
using decay_remove_cv_ref_t = remove_cv_ref_t<std::decay_t<Type>>;

template<typename Type>
struct to_variant_st
{
    using type = std::variant<Type>;
};

template<typename Type>
struct to_variant_st<tpf::types::type_list_t<Type>>
{
    using type = std::variant<Type>;
};

template<typename Type, typename... Types>
struct to_variant_st<tpf::types::type_list_t<Type, Types...>>
{
    using type = std::variant<Type, Types...>;
};

template<typename Type, typename... Types>
using to_variant_t = typename to_variant_st< tpf::types::unique_types_t<Type, Types...> >::type;

template<typename Type, typename... Types>
auto make_variants(Type&& arg, Types&&... args)
{
    using parameter_types = 
        tpf::types::type_list_t<decay_remove_cv_ref_t<Type>, decay_remove_cv_ref_t<Types>...>;

    if constexpr(tpf::types::common_type_v<parameter_types>)
    { 
        // common type exists
        using common_type = tpf::types::common_type_t<parameter_types>;

        return std::vector<common_type>{ 
            static_cast<common_type>(std::forward<Type>(arg)),
            static_cast<common_type>(std::forward<Types>(args))... };

    }
    else
    {
        // common type does not exists

        using element_t = to_variant_t<parameter_types>;

        return std::vector<element_t>{ element_t{std::forward<Type>(arg)},
                                      element_t{ std::forward<Types>(args) }... };
    }
  
}

void test_make_variants()
{
    const int a = 5;
    // auto v = make_variants(a);

    auto v2 = make_variants("String Text", 4, 4.5f, 5.6);
}

void test_make_variants_two()
{
    // common type is double
    auto v1 = make_variants(1, 2.0f, 3.14, 3.6, 5u, 6ull);

    // stream << "Type of v1: " << Tpf_GetTypeCategory(v1) << endl;
    stream << "v1: " << v1 << endl << endl;

    // common type is double
    auto v2 = make_variants("New String", 1, 2.0f, 3.14, "String Literal", 
        std::string("I love it"), L"Wide Character String");

    // stream << "Type of v2: " << Tpf_GetTypeCategory(v2) << endl;
    stream << "v2: " << v2 << endl;
}

int main()
{
    // test_make_variants();

    test_make_variants_two();
}