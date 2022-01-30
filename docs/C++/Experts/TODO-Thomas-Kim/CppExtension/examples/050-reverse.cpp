#include <tpf_output.hpp>
#include <iostream>
#include <iterator>
#include <variant>
#include <vector>
#include <initializer_list>
#include <type_traits>

tpf::sstream stream;
auto endl = tpf::endl;

void test_make_vector()
{
    auto v = tpf::make_vector("James", "Thomas Kim", "You");

    stream << v << endl;
}

void test_make_container()
{
    auto v = tpf::make_container(5, 7.0f, "James", "Thomas Kim", "You");

    stream << "Type: " << Tpf_GetTypeCategory(v) << " \n" << v << endl;
}

void test_make_variants()
{
    auto v = tpf::make_variants(2, 4.0f, "I love it");

    stream << v << endl;
}

void test_reverse()
{
    int a = 5;
    int f = 4.6f;

    auto v = tpf::make_vector(3, a, 5, f, 7.0f);

    for(auto& e: tpf::reverse(v))
    {
        ++e;

        std::cout << e << ", ";
    }

    auto rv = tpf::reverse(v).clone();

    std::cout << std::endl;

    stream << rv << endl;


    //std::cout << rv << std::endl;
}

void test_make_tuple()
{
    using tuple_t = std::tuple<int, const char*>;
    
    tuple_t tuple{5, "i love it"};

    stream << tuple << endl;
}

void test_make_smart_container()
{
    auto c1 = tpf::make_container(4, 5, 6.0f, 4.5);

    stream << "c1 - " << Tpf_GetTypeCategory(c1) << endl;
    stream << "\t" << c1 << endl;

    auto c2 = tpf::make_container("James", "Sophia Kim", 4, 6.0f, 4.5);
    
    stream << "c2 - " << Tpf_GetTypeCategory(c2) << endl;
    stream << "\t" << c2 << endl;

    auto c3 = tpf::make_container(5u, 6);

    stream << "c3 - " << Tpf_GetTypeCategory(c3) << endl;
    stream << "\t" << c3 << endl;

}
int main()
{
    // test_make_vector();
    
    // test_make_container();

    // test_make_variants();

    // test_reverse();

    // test_make_tuple();

    test_make_smart_container();

}