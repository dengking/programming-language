#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace types = tpf::types;

template<typename... Types>
using type_list_t = types::type_list_t<Types...>;

template<template<typename, typename...> class ContainerType, typename... ElementTypes>
class container_of_tuples_t : public ContainerType< std::tuple<ElementTypes...> >
{
    public:
    using tuple_t = std::tuple<ElementTypes...>;

    using base_type = ContainerType< std::tuple<ElementTypes...> >;

    using base_type::base_type;

    friend tpf::sstream& operator<<(tpf::sstream& os, const container_of_tuples_t& ct)
    {
        if(ct.empty())
        {
            os << "{ }"; return os;
        }
        else
        {
            auto last_element = ct.cend();
            std::advance(last_element, -1);

            for(auto itr=ct.cbegin(); itr != last_element; std::advance(itr, 1))
                os << *itr << ", ";

            os << *last_element;

            return os;
        }
    }
};

template<typename... ElementTypes>
using vector_of_tuples_t = container_of_tuples_t<std::vector, ElementTypes...>;

template<typename... ElementTypes>
using deque_of_tuples_t = container_of_tuples_t<std::deque, ElementTypes...>;

template<typename... ElementTypes>
using list_of_tuples_t = container_of_tuples_t<std::list, ElementTypes...>;

template<typename... ElementTypes>
using set_of_tuples_t = container_of_tuples_t<std::set, ElementTypes...>;

void test_container_of_tuples()
{
    using vctr_tuples_t = vector_of_tuples_t<int, float, const char*>;
    using tuple_t = typename vctr_tuples_t::tuple_t;

    vctr_tuples_t vt;

    vt.emplace_back(tuple_t{10, 30.0f, "Thomas Kim"});
    vt.emplace_back(tuple_t{40, 50.0f, "Sophie Turner"});
    vt.emplace_back(tuple_t{20, 60.0f, "Steven King"});

    stream <<"vt (vector of tuples ) = " << vt << endl;
}

void test_set_of_tuples()
{
    using set_tuples_t = set_of_tuples_t<int, float, const char*>;
    using tuple_t = typename set_tuples_t::tuple_t;

    set_tuples_t st;

    st.insert(tuple_t{10, 30.0f, "Thomas Kim"});
    st.insert(tuple_t{40, 50.0f, "Sophie Turner"});
    st.insert(tuple_t{20, 60.0f, "Steven King"});

    stream <<"st (set of tuples ) = " << st << endl;

    stream << endl;

    for(auto& e: st)
    {
        stream << e << endl;
    }
}

void test_list_of_tuples()
{
    using list_tuples_t = list_of_tuples_t<int, float, const char*>;
    using tuple_t = typename list_tuples_t::tuple_t;

    list_tuples_t lt;

    lt.emplace_back(tuple_t{10, 30.0f, "Thomas Kim"});
    lt.emplace_back(tuple_t{40, 50.0f, "Sophie Turner"});
    lt.emplace_back(tuple_t{20, 60.0f, "Steven King"});

    stream <<"lt (list of tuples ) = " << lt << endl;

    stream << endl;

    for(auto& e: lt)
    {
        stream << e << endl;
    }

    stream << endl;

    for(auto& e: tpf::reverse(lt))
    {
        stream << e << endl;
    }


}

int main()
{
    // test_container_of_tuples();
    
    // test_set_of_tuples();

    test_list_of_tuples();
}


