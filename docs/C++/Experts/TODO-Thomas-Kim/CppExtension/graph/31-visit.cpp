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
    struct is_pair_of_variant_type_st
    {
        static constexpr bool value = false;
    };

    template<typename KeyType, typename... Types>
    struct is_pair_of_variant_type_st<std::pair<KeyType, std::variant<Types...>>>
    {
        static constexpr bool value = true;
    };

    template<typename Type>
    constexpr bool is_pair_of_variant_type_v = 
        is_pair_of_variant_type_st<remove_cv_ref_t<Type>>::value;

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
    using enable_if_pair_of_variant_t = std::enable_if_t<is_pair_of_variant_type_v<Type>, ReturnType>;

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
        static enable_if_pair_of_variant_t<PairType>
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
    enable_if_pair_of_variant_t<PairType> 
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

        template<typename VariantType>
        enable_if_variant_t<VariantType> 
        operator()(VariantType&& vt)
        {
             types::visit(*this, std::forward<VariantType>(vt));
        }

        template<typename PairType>
        enable_if_pair_of_variant_t<PairType> 
        operator()(PairType&& pvt)
        {
             types::visit(*this, std::forward<PairType>(pvt));
        }
    };

    // template argument deduction guide
    // this feature was introduced to C++17 Standards
    template<typename... VisitorTypes>
    overloaded(VisitorTypes...) -> overloaded<VisitorTypes...>;

    template<typename... VisitorTypes>
    overloaded<remove_cv_ref_t<VisitorTypes>...>
    make_overloaded(VisitorTypes&&... visitors)
    {
        return { std::forward<VisitorTypes>(visitors)... };
    }

} // end of namespace tpf::types

namespace types = tpf::types;

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

    types::make_overloaded
    (
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

    ).for_each(info);

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

    types::make_overloaded
    (
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
    ).for_each(info);

    stream << endl;
}

void test_visit_variant_of_variants()
{
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;

    using inner_vt_t = std::variant<name_t, weight_t>;
    
    using variant_t = std::variant<name_t, age_t, weight_t, inner_vt_t>;

    using container_t = std::vector<variant_t>;

    container_t info;

    info.emplace_back("Thomas Kim");
    info.emplace_back(30);
    info.emplace_back(60.5);
    info.emplace_back("Sophie Turner");
    info.emplace_back(20);
    info.emplace_back(56.7);
    info.emplace_back(inner_vt_t{ "James Dean"} );
    info.emplace_back(inner_vt_t{ 65.6 } );
    info.emplace_back("Sophie Kim");

    tpf::sstream stream;
    auto endl = tpf::endl;

    types::make_overloaded
    (
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
        },
        
        [&stream, &endl](auto& vt) // inner_vt_t
        {
            types::make_overloaded
            (
                [&stream, &endl](auto&& name)
                {
                    stream << "Inner Name is " << name << endl;
                },

                [&stream, &endl](auto&& weight)
                {
                    stream << "Inner Weight is " << weight << endl;
                }

            )(std::forward<decltype(vt)>(vt));           
        }

    ).for_each(info);

    stream << endl;
}

void test_visit_map_of_variants_of_map()
{
    using key_t = const char*;
    using name_t = const char*;
    using age_t = int;
    using weight_t = double;
    
    using map_t = std::map<key_t, name_t>;

    using map_vt_t = types::map_of_variants_t<key_t, age_t, weight_t>;

    using variant_t = std::variant<name_t, age_t, weight_t, map_t, map_vt_t>;

    using container_t = std::map<key_t, variant_t>;

    container_t info;

    info["Programmer"] = "Thomas Kim";
    info["Age"] = 30;
    info["Weight"] = 60.5;
    info["Actress"] = "Sophie Turner";
    info["Her age"] = 20;
    info["Her weight"] = 56.7;
    info["Map of Map"] = map_t{ {"Friend", "James Dean"}, {"Buddy", "Albert Kim"} };
    info["Map of Variants"] = map_vt_t{ {"Friend's Age", 30}, {"Buddy's Weight", 56.7} };
    
    tpf::sstream stream;
    auto endl = tpf::endl;

    types::make_overloaded
    (
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
        },

        [&stream, &endl](auto&& key, auto&& map) // map_t
        {
           for(auto& m: map)
           {
               auto& [inner_key, value] = m;

               stream << "Outer Key - " << key
                    << ", inner_key: " << inner_key
                    <<", value: " << value << endl;
           }
        },

        [&stream, &endl](auto&& key, auto&& map_vt) // map_vt_t
        {
           types::make_overloaded
           (
               [&stream, &endl, &key](auto&& inner_key, auto&& age)
               {
                   stream << "Outer key: " << key 
                    << ", inner_key: " << inner_key<<", age: " << age << endl;
               },

               [&stream, &endl, &key](auto&& inner_key, auto&& weight)
               {
                   stream << "Outer key: " << key 
                    << ", inner_key: " << inner_key<<", weight: " << weight << endl;
               }

           ).for_each(std::forward<decltype(map_vt)>(map_vt));
        }

    ).for_each(info);

    stream << endl;
}


int main()
{
    // test_visit_simplified();
    
    // test_visit_simplified_map();

    // test_visit_variant_of_variants();

    test_visit_map_of_variants_of_map();
}