#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace types = tpf::types;

void test_map_of_tuples()
{
    using key_type = const char*;
    using age_t = int;
    using weight_t = double;
    using name_t = std::string;

    using map_t = types::map_of_tuples_t<key_type, name_t, age_t, weight_t>;
    using tuple_t = typename map_t::tuple_type;

    map_t info;

    info["programmer"] = std::make_tuple("Thomas Kim", 20, 56.7);
    info["president"] = std::make_tuple("Moon Jae In", 65, 56.7);
    info["actress"] = std::make_tuple("Sophie Turner", 20, 56.7);
    
    stream << info << endl;
    stream << types::reverse(info) << endl;

    for(auto& p: types::reverse(info))
    {
        auto& [key, value] = p;
        auto& [name, age, weight] = value;

        stream <<key <<": " << name <<", " << age<<", "<<weight<< endl;
    }
}

void test_reverse_array()
{
    int array[] {1, 2, 3, 4, 5};

    auto msg = "I love it";
    
    stream << msg << endl;

    stream << array << endl;

    stream << tpf::reverse(array) << endl;
}

int main()
{
    // test_map_of_tuples();

    test_reverse_array();
}