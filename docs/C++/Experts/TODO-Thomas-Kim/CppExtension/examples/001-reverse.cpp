#include <tpf_types.hpp>

void test_reverse()
{
    std::vector<int> v{0, 1, 2, 3, 4, 5};
    
    tpf::reverse_t<std::vector<int>> rv{v};

    for(auto e: rv)
    {
        std::cout << e << std::endl;
    }
}

int main()
{
    test_reverse();
}