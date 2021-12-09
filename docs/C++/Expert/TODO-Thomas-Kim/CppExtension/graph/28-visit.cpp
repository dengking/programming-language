#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace tpf::types
{
    template<size_t StartIndex, size_t EndIndex>
    struct compile_time_loop
    {
        template<typename VisitorType, typename VariantType>
        static void visit_variant(VisitorType&& visitor, VariantType&& vt)
        {
            if constexpr(StartIndex < EndIndex)
            {
                if(auto ptr = std::get_if<StartIndex>(&vt))
                {
                    std::forward<VisitorType>(visitor)(*ptr);
                    return;
                }
            }

            if constexpr (StartIndex + 1 < EndIndex)
            {
                compile_time_loop<StartIndex + 1, EndIndex>:: template
                    visit_variant(std::forward<VisitorType>(visitor) , std::forward<VariantType>(vt));
            }
        }

    }; // end of struct compile_time_loop

    template<typename VisitorType, typename VariantType>
    void visit(VisitorType&& visitor, VariantType&& vt)
    {
        using variant_t = remove_cv_ref_t<VariantType>;
        // cppreference.com - std::variant_size_v
        // https://en.cppreference.com/w/cpp/utility/variant/variant_size
        constexpr size_t VariantSize = std::variant_size_v<variant_t>;
    
        compile_time_loop<0, VariantSize>:: template
            visit_variant(std::forward<VisitorType>(visitor), std::forward<VariantType>(vt));
   }

    template<typename... VisitorTypes>
    struct overloaded: public VisitorTypes... // mixin
    {
        using VisitorTypes::operator()...;

        overloaded(VisitorTypes... visitors): 
            VisitorTypes{visitors}...  { }
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

    types::overloaded handle_info_const_ref
    {
        [&stream, &endl](const name_t& name)
        {
            stream <<"Name is " << name << endl;
        },

        [&stream, &endl](const age_t& age)
        {
            stream <<"Age is " << age << endl;
        },

        [&stream, &endl](const weight_t& weight)
        {
            stream << "Weight is " << weight << endl;
        },

        // catch all if failed
        [&stream, &endl](auto&& value)
        {
            stream << "Unclassified: " << value << endl;
            stream << "Type: " << Tpf_GetTypeCategory(value) << endl;
        }
    };

    types::overloaded handle_info_ref
    {
        [&stream, &endl](name_t& name)
        {
            stream <<"Name is " << name << endl;
        },

        [&stream, &endl](age_t& age)
        {
            stream <<"Age is " << age << endl;
        },

        [&stream, &endl](weight_t& weight)
        {
            stream << "Weight is " << weight << endl;
        },

        // catch all if failed
        [&stream, &endl](auto&& value)
        {
            stream << "Unclassified: " << value << endl;
            stream << "Type: " << Tpf_GetTypeCategory(value) << endl;
        }
    };

    for(const auto& vt: info)
    {
        types::visit(handle_info_const_ref, vt);
    }
    
    stream << endl;

    for(auto& vt: info)
    {
        types::visit(handle_info_ref, vt);
    }

    stream << endl;

    /////////////////////////////
    for(const auto& vt: info)
    {
        std::visit(handle_info_const_ref, vt);
    }
    
    stream << endl;

    for(auto& vt: info)
    {
        std::visit(handle_info_ref, vt);
    }

    stream << endl;
}

int main()
{
    test_visit_basic();
}