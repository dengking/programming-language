#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

void test_reverse_range_based_for_loop()
{
    // make_container() creates either vector or tuple
    auto v = tpf::make_container(1, 2, 3, 4.0f, 5.0, 6, 7, 8, 9, 10);

    stream << "The type of v: " << Tpf_GetTypeCategory(v) << endl;

    stream << "v = " << v << endl;

    stream << "reversed v = " << tpf::reverse(v).clone() << endl;

    for(auto& e: v)
    {
        stream << e << ", " ;
    }

    stream << endl;

    for(auto& e: tpf::reverse(v) )
    {
        stream << e << ", " ;
    }

    stream << endl;
}

void test_reverse_range_for()
{
    for(auto& e: tpf::reverse(2, 3, 5, 7, 11, 13, 17, 19))
    {
        stream <<e << ", ";
    }

    stream << endl;

    std::list<int> lst{1, 2, 3, 4, 5};

    for(auto& e: lst)
    {
        stream << e << ", ";
    }

    stream << endl;

    for(auto& e: tpf::reverse(lst))
    {
        ++e;
        stream << e << ", ";
    }

    stream << endl;

     for(auto& e: lst)
    {
        stream << e << ", ";
    }

    stream << endl;

}

void test_make_variants_containers()
{
    // make_variants() creates a vector
    // when arguments have a common type
    // in this case, the common type of 1, 2, 3.0f, 5.0 is double
    // so, it creates std::vector<double>
    auto c1 = tpf::make_variants(1, 2, 3.0f, 5.0);

    stream <<"Type of c1: " << Tpf_GetTypeCategory(c1) << endl;
    stream <<"The contents of c1: " << c1 << endl;

    // when arguments do not have common type,
    // then make_variants() creates a vector<std::variant>
    auto c2 = tpf::make_variants(2, 3.0f, "James Dean", std::string("Sophie Turner"));

    stream <<"Type of c2: " << Tpf_GetTypeCategory(c2) << endl;
    stream <<"The contents of c2: " << c2 << endl;

    // when arguments have a common type,
    // make_container() creates std::vector<common type>
    // otherwise, creates std::tuple<>
    // in this case, the common type is double
    // so, it creates std::vector<double>
    auto c3 = tpf::make_container(1, 2.0f, 3.0);
    
    stream <<"Type of c3: " << Tpf_GetTypeCategory(c3) << endl;
    stream <<"The contents of c3: " << c3 << endl;

    // when arguments do not have common type,
    // make_container() creates a tuple
    auto c4 = tpf::make_container("James Dean", "Sophie Turner", 3.14);

    stream <<"Type of c4: " << Tpf_GetTypeCategory(c4) << endl;
    stream <<"The contents of c4: " << c4 << endl;
}

template<typename Type>
decltype(auto) decay(Type&& arg)
{
    using type = std::remove_reference_t<Type>;

    if constexpr(std::is_array_v<type> || std::is_function_v<type>)
    {
        // std::cout << "Decaying Type " << std::endl;

        return (std::decay_t<type>)(arg);
    }
    else
    {
        // std::cout << "Decaying Type Bypassed " << std::endl;
        return std::forward<Type>(arg);
    }
}

void test_decay()
{
    int array[]{1, 2, 3};

    stream << "Type of array: " << Tpf_GetTypeCategory(array) << endl;
    
    auto decayed_array = decay(array);

    stream <<"Type of decayed_array: " << Tpf_GetTypeCategory(decayed_array) << endl;
}

// this type function
// decays, remove reference, remove const volatile
template<typename Type>
using decay_remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<std::decay_t<Type>>>;

template<typename Type, typename... Types>
auto make_vector(Type&& arg, Types&&... args)
{
    // we test if the types of arg, args... are the same
    constexpr auto all_types_same
        = tpf::types::is_same_v<decay_remove_cv_ref_t<Type>, decay_remove_cv_ref_t<Types>...>;

    // we test if there exists a common for arg and args...
    constexpr auto common_type_exists
        = tpf::types::common_type_v<decay_remove_cv_ref_t<Type>, decay_remove_cv_ref_t<Types>...>;

    static_assert(common_type_exists, "Common Type Does Not Exist");

    if constexpr(all_types_same)
    {
        using element_t = decay_remove_cv_ref_t<Type>;
        using container_t = std::vector<element_t>;

        return container_t{ std::forward<Type>(arg),
            std::forward<Types>(args)...};
    }
    else
    {
         // common type exists
         using common_type =
            tpf::types::common_type_t<decay_remove_cv_ref_t<Type>, decay_remove_cv_ref_t<Types>...>;

        // if all types are of integral type
        if constexpr(tpf::types::is_integral_v<common_type>)
        {
            // we test all elements are of unsigned type
            constexpr auto all_unsigned
                = tpf::types::are_unsigned_integrals_v<decay_remove_cv_ref_t<Type>, decay_remove_cv_ref_t<Types>...>;
        
            if constexpr(all_unsigned)
            {
                using element_t = common_type;
                using container_t = std::vector<element_t>;

                return container_t{ static_cast<element_t>(arg),
                    static_cast<element_t>(args)...};
            }
            else
            {
                using element_t = std::make_signed_t<common_type>;
                using container_t = std::vector<element_t>;

                return container_t{ static_cast<element_t>(arg),
                    static_cast<element_t>(args)...};
            }
        }
        else
        {
            using element_t = common_type;
            using container_t = std::vector<element_t>;
            
            return container_t{ static_cast<element_t>(arg),
                    static_cast<element_t>(args)...};
        }
    }
}

void test_make_vector()
{
    auto v1 = make_vector(1, 2, 3, 4, 5.0f, 6.0);

    stream << "Type of v1: " << Tpf_GetTypeCategory(v1) << endl;
    stream <<" v1 = " << v1 << endl; 

    // when arguments are composed of signed and unsigned,
    // the common type is NOT unsigned, but signed
    auto v2 = make_vector(2, 3, 4, 5u);

    stream << "Type of v2: " << Tpf_GetTypeCategory(v2) << endl;
    stream <<" v2 = " << v2 << endl; 

}

template<typename Type, typename... Types>
auto make_container(Type&& arg, Types&&... args)
{
    // we test if the types of arg, args... are the same
    constexpr auto all_types_same
        = tpf::types::is_same_v<decay_remove_cv_ref_t<Type>, decay_remove_cv_ref_t<Types>...>;

    // we test if there exists a common for arg and args...
    constexpr auto common_type_exists
        = tpf::types::common_type_v<decay_remove_cv_ref_t<Type>, decay_remove_cv_ref_t<Types>...>;

    if constexpr(all_types_same || common_type_exists)
    {
        return make_vector( std::forward<Type>(arg), std::forward<Types>(args)...);
    }
    else
    {
        // when arguments do not have common type
        // we create a tuple
        return std::make_tuple(std::forward<Type>(arg), std::forward<Types>(args)...);
    }
}


void test_make_container()
{
    auto v1 = make_container(1, 2, 3, 4, 5.0f, 6.0);

    stream << "Type of v1: " << Tpf_GetTypeCategory(v1) << endl;
    stream <<" v1 = " << v1 << endl; 

    // when arguments are composed of signed and unsigned,
    // the common type is NOT unsigned, but signed
    auto v2 = make_container("Sophie Turner", 2, 3, 4, 5u);

    stream << "Type of v2: " << Tpf_GetTypeCategory(v2) << endl;
    stream <<" v2 = " << v2 << endl; 

}

int main()
{
    test_reverse_range_based_for_loop();

    test_reverse_range_for();

    test_make_variants_containers();

    test_decay();

    test_make_vector();

    test_make_container();
}