#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace tpf::types
{
    template<typename Type>
    struct is_pair_type_st
    {
        static constexpr bool value = false;
    };

    template<typename Type_1, typename Type_2>
    struct is_pair_type_st<std::pair<Type_1, Type_2>>
    {
        static constexpr bool value = true;
    };

    template<typename Type>
    constexpr bool is_pair_type_v = is_pair_type_st<remove_cv_ref_t<Type>>::value;

    template<typename Type>
    struct is_variant_type_st
    {
        static constexpr bool value = false;
    };

    template<typename... Types>
    struct is_variant_type_st<std::variant<Types...>>
    {
        static constexpr bool value = true;
    };

    template<typename Type>
    constexpr bool is_variant_type_v = is_variant_type_st<remove_cv_ref_t<Type>>::value;

    template<typename Type, typename ReturnType = void>
    using enable_if_pair_t = std::enable_if_t<is_pair_type_v<Type>, ReturnType>;

    template<typename Type, typename ReturnType = void>
    using enable_if_variant_t = std::enable_if_t<is_variant_type_v<Type>, ReturnType>;

    template<size_t StartIndex, size_t EndIndex>
    struct compile_time_loop
    {
        // this function is enabled only when VariantType is std::variant<Types...>
        template<typename VisitorType, typename VariantType>
        static enable_if_variant_t<VariantType>
        visit_variant(VisitorType&& visitor, VariantType&& vt)
        {
            if constexpr(StartIndex < EndIndex)
            {
                if(auto ptr = std::get_if<StartIndex>(&vt))
                {
                    // auto& visit = std::get<StartIndex>(visitor.m_visitors); visit(*ptr); 
                    std::get<StartIndex>(visitor.m_visitors)(*ptr);
                    return;
                }
            }

            if constexpr (StartIndex + 1 < EndIndex)
            {
                compile_time_loop<StartIndex + 1, EndIndex>:: template
                    visit_variant(std::forward<VisitorType>(visitor) , std::forward<VariantType>(vt));
            }
        }

        // this function is enabled only when PairType is std::pair<Type_1, Type_2>
        template<typename VisitorType, typename PairType>
        static enable_if_pair_t<PairType>
        visit_variant(VisitorType&& visitor, PairType&& pair)
        {
            if constexpr(StartIndex < EndIndex)
            {
                auto& [key, vt] = std::forward<PairType>(pair);

                if(auto ptr = std::get_if<StartIndex>(&vt))
                {
                    // auto& visit = std::get<StartIndex>(visitor.m_visitors); visit(*ptr); 
                    std::get<StartIndex>(visitor.m_visitors)(key, *ptr);
                    return;
                }
            }

            if constexpr (StartIndex + 1 < EndIndex)
            {
                compile_time_loop<StartIndex + 1, EndIndex>:: template
                    visit_variant(std::forward<VisitorType>(visitor) , std::forward<PairType>(pair));
            }
        }

    }; // end of struct compile_time_loop

    // this function is enabled only when VariantType is std::variant<Types...>
    template<typename VisitorType, typename VariantType>
    enable_if_variant_t<VariantType> 
    visit(VisitorType&& visitor, VariantType&& vt)
    {
        using variant_t = remove_cv_ref_t<VariantType>;
        // cppreference.com - std::variant_size_v
        // https://en.cppreference.com/w/cpp/utility/variant/variant_size
        constexpr size_t VariantSize = std::variant_size_v<variant_t>;
    
        compile_time_loop<0, VariantSize>:: template
            visit_variant(std::forward<VisitorType>(visitor), std::forward<VariantType>(vt));
   }

    // this function is enabled only when PairType is std::pair<Type_1, Type_2>
    template<typename VisitorType, typename PairType>
    enable_if_pair_t<PairType> 
    visit(VisitorType&& visitor, PairType&& pair)
    {
        using pair_t = remove_cv_ref_t<PairType>;
        using variant_t = typename pair_t::second_type;
        
        // cppreference.com - std::variant_size_v
        // https://en.cppreference.com/w/cpp/utility/variant/variant_size
        constexpr size_t VariantSize = std::variant_size_v<variant_t>;
    
        compile_time_loop<0, VariantSize>:: template
            visit_variant(std::forward<VisitorType>(visitor), std::forward<PairType>(pair));
   }

    template<typename... VisitorTypes>
    struct overloaded
    {
        using vistors_t = std::tuple<VisitorTypes...>;

        vistors_t m_visitors;

        overloaded(VisitorTypes... visitors): 
            m_visitors{ std::move(visitors)...}  { }

        template<typename ContainerType>
        void for_each(ContainerType&& container)
        {
            for(decltype(auto) vt: 
                std::forward<ContainerType>(container))
            {
                types::visit(*this, vt);
            }
        }
    };

    // template argument deduction guide
    // this feature was introduced to C++17 Standards
    template<typename... VisitorTypes>
    overloaded(VisitorTypes...) -> overloaded<VisitorTypes...>;

} // end of namespace tpf::types

namespace types = tpf::types;

void test_visit_basic()
{
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;
    using variant_t = std::variant<name_t, age_t, weight_t>;

    using container_t = std::vector<variant_t>;

    container_t info;

    info.emplace_back("Thomas Kim");
    info.emplace_back(30);
    info.emplace_back(60.5);
    info.emplace_back("Sophie Turner");
    info.emplace_back(20);
    info.emplace_back(56.7);

    tpf::sstream stream;
    auto endl = tpf::endl;

    // we now have to concern about 
    // the order of the lambda functions
    types::overloaded handle_info_auto_ref
    {
        [&stream, &endl](auto& name)
        {
            stream <<"Name is " << name << endl;
        },

        [&stream, &endl](auto& age)
        {
            stream <<"Age is " << age << endl;
        },

        [&stream, &endl](auto& weight)
        {
            stream << "Weight is " << weight << endl;
        }
    };

    types::overloaded handle_info_const_auto_ref
    {
        [&stream, &endl](const auto& name)
        {
            stream <<"Name is " << name << endl;
        },

        [&stream, &endl](const auto& age)
        {
            stream <<"Age is " << age << endl;
        },

        [&stream, &endl](const auto& weight)
        {
            stream << "Weight is " << weight << endl;
        }
    };

    for(const auto& vt: info)
    {
        types::visit(handle_info_const_auto_ref, vt);
    }
    
    stream << endl;

    for(auto& vt: info)
    {
        types::visit(handle_info_auto_ref, vt);
    }

    stream << endl;
}

void test_visit_simplified()
{
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;
    using variant_t = std::variant<name_t, age_t, weight_t>;

    using container_t = std::vector<variant_t>;

    container_t info;

    info.emplace_back("Thomas Kim");
    info.emplace_back(30);
    info.emplace_back(60.5);
    info.emplace_back("Sophie Turner");
    info.emplace_back(20);
    info.emplace_back(56.7);

    tpf::sstream stream;
    auto endl = tpf::endl;

    // we now have to concern about 
    // the order of the lambda functions
    types::overloaded handle_info
    {
        [&stream, &endl](auto&& name)
        {
            stream <<"Name is " << name << endl;
            stream <<"Type: " << Tpf_GetTypeCategory(name) << endl;
        },

        [&stream, &endl](auto&& age)
        {
            stream <<"Age is " << age << endl;
            stream <<"Type: " << Tpf_GetTypeCategory(age) << endl;
        },

        [&stream, &endl](auto&& weight)
        {
            stream << "Weight is " << weight << endl;
            stream <<"Type: " << Tpf_GetTypeCategory(weight) << endl;
        }
    };


    handle_info.for_each(info);
    
    stream << endl;
}

void test_visit_simplified_set()
{
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;
    using variant_t = std::variant<name_t, age_t, weight_t>;

    using container_t = std::set<variant_t>;

    container_t info;

    info.emplace("Thomas Kim");
    info.emplace(30);
    info.emplace(60.5);
    info.emplace("Sophie Turner");
    info.emplace(20);
    info.emplace(56.7);

    tpf::sstream stream;
    auto endl = tpf::endl;

    // we now have to concern about 
    // the order of the lambda functions
    types::overloaded handle_info
    {
        [&stream, &endl](auto&& name)
        {
            stream <<"Name is " << name << endl;
            stream <<"Type: " << Tpf_GetTypeCategory(name) << endl;
        },

        [&stream, &endl](auto&& age)
        {
            stream <<"Age is " << age << endl;
            stream <<"Type: " << Tpf_GetTypeCategory(age) << endl;
        },

        [&stream, &endl](auto&& weight)
        {
            stream << "Weight is " << weight << endl;
            stream <<"Type: " << Tpf_GetTypeCategory(weight) << endl;
        }
    };

    handle_info.for_each(info);
    
    stream << endl;

    // C++2a Standard: Fundamentals of C++ Template Metaprogramming 1/N (039)
    // https://www.youtube.com/watch?v=nJuXE4VEYLI&t=237s
    handle_info.for_each( types::reverse(info) );
    
    stream << endl;

}

void test_visit_simplified_map()
{
    using key_t = const char*;
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;
    using variant_t = std::variant<name_t, age_t, weight_t>;

    using container_t = std::map<key_t, variant_t>;

    container_t info;

    info["Programmer"] = "Thomas Kim";
    info["Age"] = 30;
    info["Weight"] = 60.5;
    info["Actress"] = "Sophie Turner";
    info["Her age"] = 20;
    info["Her weight"] = 56.7;

    tpf::sstream stream;
    auto endl = tpf::endl;

    // we now have to concern about 
    // the order of the lambda functions
    types::overloaded handle_info
    {
        [&stream, &endl](auto&& key, auto&& name)
        {
            stream << key << " is " << name << endl;
        },

        [&stream, &endl](auto&& key, auto&& age)
        {
            stream << key << " is " << age << endl;
        },

        [&stream, &endl](auto&& key, auto&& weight)
        {
           stream << key << " is " << weight << endl;
        }
    };

    handle_info.for_each(info);

    stream << endl;
}

int main()
{
    // stream <<"container vector " << endl;
    // test_visit_simplified();

    // stream <<"\ncontainer set " << endl;
    // test_visit_simplified_set();

    test_visit_simplified_map();
}