#include <iostream>
#include <tuple>
#include <variant>

auto& stream = std::cout;
auto endl = "\n";

// C++ does not allow multiple variadic parameter packs
// template<typename... Types_1, typename... Types_2>
// class multiple_packs
// {

// };

template<typename... Types>
struct type_list_t { };

// primary template declaration
// typename... Types ---> means
// multiple_packs can take zero or more parameters
template<typename... Types> class multiple_packs;

template<typename... Types_1, typename... Types_2>
class multiple_packs< type_list_t<Types_1...>, type_list_t<Types_2...> >
{
    public:
        using tuple_t = std::tuple<Types_1...>;
        using variant_t = std::variant<Types_2...>;
    
    public:
        tuple_t m_tuple;
        variant_t m_variant;

};

void test_multiple_packs()
{
    using types_for_tuple = type_list_t<int, short>;
    using types_for_variant = type_list_t<float, double>;

    multiple_packs<types_for_tuple, types_for_variant> mp;
}

void how_to_use_multiple_packs()
{
    using types_for_tuple = type_list_t<int, short>;
    using types_for_variant = type_list_t<double, const char*>;

    multiple_packs<types_for_tuple, types_for_variant> 
        mp{ {5, 6}, { "Thomas Kim" } };

    auto& [int_value, short_value ] = mp.m_tuple;

    stream <<"int_value = " << int_value << endl;
    stream <<"short_value = " << short_value << endl;

    std::visit( [](auto& value) 
    {
        stream << "the value of variant: "
            << value << endl;
    }, mp.m_variant);
   
   mp.m_variant = 22.0 /7.0;

    std::visit( [](auto& value) 
    {
        stream << "the value of variant: "
            << value << endl;
    }, mp.m_variant);
   
}

int main()
{
    how_to_use_multiple_packs();
}