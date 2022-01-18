/*
    Please be warned in advance that this episode is quite advanced. I will use
    type functions that I created by myself. That is, C++ Standard is not much useful in this implementtation.
*/

#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

template<typename... Types>
using pack_t = tpf::types::type_list_t<Types...>;

// this is our Primary Class Template.
// Primary Class Template determins the count of the type parameters.
// class template TwoPacks takes two types, Pack1 and Pack2
template<typename Pack1, typename Pack2> struct TwoPacks;

// this is a specialization of TwoPacks
template<typename... PackType_1, typename... PackType_2>
struct TwoPacks< pack_t<PackType_1...> , pack_t<PackType_2...> >
{
    static void take_two_parameter_packs( PackType_1&&... args1, PackType_2&&... args2)
    {
        if constexpr( tpf::types::is_same_v<tpf::remove_cv_ref_t<PackType_1>...>)
        {
            // all types of args1... are the same type
            std::vector pack1{ std::forward<PackType_1>(args1)... };
            stream << pack1 << endl;
        }
        else
        {
            // types of args1... are not the same
            std::tuple pack1 { std::forward<PackType_1>(args1)... };
            stream << pack1 << endl;
        }

        if constexpr( tpf::types::is_same_v<tpf::remove_cv_ref_t<PackType_2>...>)
        {
            // all types of args2... are the same type
            std::vector pack2{ std::forward<PackType_2>(args2)... };
            stream << pack2 << endl;
        }
        else
        {
            // types of args2... are not the same
            std::tuple pack2 { std::forward<PackType_2>(args2)... };
            stream << pack2 << endl;
        }

        stream << endl;
    }
};

using str_t = const char*;
using int_t = int;

void test_multiple_parameter_packs_1()
{
    using pack_1 = pack_t<str_t, str_t, str_t>;
    using pack_2 = pack_t<int_t, int_t, int_t, int_t>;

    TwoPacks<pack_1, pack_2>::take_two_parameter_packs("Thomas", "Sophia", "Anne", 21, 19, 23, 40);
}

void test_multiple_parameter_packs_2()
{
    using pack_1 = pack_t<str_t, int_t, str_t>;
    using pack_2 = pack_t<int_t, int_t, str_t>;

    TwoPacks<pack_1, pack_2>::take_two_parameter_packs("Thomas", 10, "Sophia", 21, 19, "Anne");
}

int main()
{
    test_multiple_parameter_packs_1();

    test_multiple_parameter_packs_2();
}