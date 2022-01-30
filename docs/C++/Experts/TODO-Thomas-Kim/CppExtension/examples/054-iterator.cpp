#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

void test_reverse_iterator()
{
    int array[] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for(auto& e: tpf::reverse(array))
    {
        ++e;
        stream << e << ", ";
    }

    stream << endl;

    for(auto& e: array)
    {
        stream << e << ", ";
    }

    auto clone = tpf::reverse(array).clone();

    stream << clone << endl;
}

void test_reverse_rvalue_iterator()
{
    for(auto e: tpf::reverse(1, 2, 3, 4, 5, 6, 7, 8, 9, 10))
    {
        std::cout << e << ", ";
    }
}

int main()
{
   
    test_reverse_iterator();

    // test_reverse_rvalue_iterator();
}