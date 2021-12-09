#include <iostream>
#include <type_traits>
#include <tuple>
#include <variant>

/*
    When you overload operators for C++ class,
    you should be EXTREMELY CAREFUL for recursive function call.
    Most of the time, it is valid operation. But in some rare cases,
    it can cause trouble to your application development.

    Operator overloading at global scope is VERY DANGEROUS PRACTICE.
    You should almost always prevent operator overloading at global scope.
*/
namespace tutorial
{
    // remove reference and const volatile from type Type
    template<typename Type>
    using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<Type>>;

    template<typename Type>
    struct is_variant_st
    {
        static constexpr bool value = false;
    };

    template<typename... Types>
    struct is_variant_st<std::variant<Types...>>
    {
        static constexpr bool value = true;
    };

    template<typename Type>
    constexpr bool is_variant_v = is_variant_st<remove_cv_ref_t<Type>>::value;

    template<typename Type>
    struct is_tuple_st
    {
        static constexpr bool value = false;
    };

    template<typename... Types>
    struct is_tuple_st<std::tuple<Types...>>
    {
        static constexpr bool value = true;
    };

    template<typename Type>
    constexpr bool is_tuple_v = is_tuple_st<remove_cv_ref_t<Type>>::value;

    template<typename... Types>
    void print_tuple(std::ostream& os, const std::tuple<Types...>& tpl);

    template<typename Type, typename... Types>
    void print_variant(std::ostream& os, const std::variant<Type, Types...>& vrt);

    template<size_t StartIndex, size_t EndIndex>
    struct static_loop
    {
        template<typename TupleType>
        static void print_tuple(std::ostream& os, const TupleType& tpl)
        {
            if constexpr( StartIndex != EndIndex )
            {
                auto& e = std::get<StartIndex>(tpl);
                // When you use operator overloading, you have to be extremely careful
                // of recurive function-call
                // os << e; <-- is recursive function-call
                using ele_t = decltype(e);

                if constexpr(is_tuple_v<ele_t>)
                {
                    tutorial::print_tuple(os, e);
                }
                else if constexpr(is_variant_v<ele_t>)
                {
                    tutorial::print_variant(os, e);
                }
                else
                {
                    os << e;
                }
            }

            if constexpr(StartIndex + 1 < EndIndex)
            {
                os << ", ";

                static_loop<StartIndex+1, EndIndex>::template print_tuple(os, tpl);
            }
        }

        template<typename VariantType>
        static void print_variant(std::ostream& os, const VariantType& vrt)
        {
            if constexpr(StartIndex != EndIndex)
            {
                if(auto ptr = std::get_if<StartIndex>(&vrt))
                {
                    // os << *ptr;  <-- is recursive function-call
                    using ele_t = decltype(*ptr);

                    if constexpr(is_variant_v<ele_t>)
                    {
                        tutorial::print_variant(os, *ptr); // recursion
                    }
                    else if constexpr(is_tuple_v<ele_t>)
                    {
                        tutorial::print_tuple(os, *ptr); // recursion
                    }
                    else
                    {
                        os << *ptr; // this is not recursion
                    }
                    
                    return;
                }
            }

            if constexpr(StartIndex + 1 < EndIndex )
            {
                static_loop<StartIndex + 1, EndIndex>::template print_variant(os, vrt);
            }
        }

    };

    template<typename... Types>
    void print_tuple(std::ostream& os, const std::tuple<Types...>& tpl)
    {
        if constexpr(sizeof...(Types) == 0)
        {
            os << "{ }"; 
        }
        else
        {
            os << "{";
            static_loop<0, sizeof...(Types)>::template print_tuple(os, tpl);
            os << "}";
        }
        
    }

    template<typename Type, typename... Types>
    void print_variant(std::ostream& os, const std::variant<Type, Types...>& vrt)
    {
        // if vrt is not initialized
        // we cannot access the element
        if(vrt.index() != std::variant_npos)
        static_loop<0, sizeof...(Types) + 1>::template print_variant(os, vrt);
    }

    template<typename... Types>
    std::ostream& operator<<(std::ostream& os, const std::tuple<Types...>& tpl)
    {
        print_tuple(os, tpl);
        return os;
    }

    template<typename Type, typename... Types>
    std::ostream& operator<<(std::ostream& os, const std::variant<Type, Types...>& vrt)
    {
        print_variant(os, vrt);
        
        return os;
    }

} // end of namespace tutorial

void test_simple_tuple_variant()
{
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    using tuple_t = std::tuple<name_t, age_t, weight_t>;
    using variant_t = std::variant<name_t, age_t, weight_t>;

    tuple_t t{ "Sophie Turner", 20, 56.5};

    variant_t v { "Thomas Kim"};

    {
        using namespace tutorial;

        std::cout << "t = " << t << std::endl;
        std::cout << "v = " << v << std::endl;
    }
}

void test_advanced_tuple_variant()
{
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    using tpl_t = std::tuple<name_t, age_t, weight_t>;
    using vrt_t = std::variant<name_t, age_t, weight_t>;

    // tuple_t is a tuple with name_t, another tuple tpl_t, a variant vrt_t as
    // its element types
    using tuple_t = std::tuple<name_t, tpl_t, vrt_t>;

    // variant_t is a variant with name_t, a tuple tpl_t, another variant vrt_t
    // as its element types
    using variant_t = std::variant<name_t, tpl_t, vrt_t>;

    tuple_t t { "Advanced Tuple", tpl_t{"Sophie Turner", 20, 56.7}, vrt_t{"Steven Lee"} };

    variant_t v { tpl_t{ "Advanced Variant",  1, 22.0 / 7.0} };

    // do not do this
    // using namespace tutorial;

    {
        // always confine using declaraction of the namespace
        using namespace tutorial;

        std::cout << "t = " << t << std::endl;
        std::cout << "v = " << v << std::endl; 
    }
}

int main()
{
    // test_simple_tuple_variant();

    test_advanced_tuple_variant();
}

