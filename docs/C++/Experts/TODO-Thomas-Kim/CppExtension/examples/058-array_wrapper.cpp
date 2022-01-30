#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;
auto nl = tpf::nl;

namespace types = tpf::types;

void test_array_wrapper()
{
    int array[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    double darray[] {0.0, 1.0, 1.3};

    tpf::types::array_wrapper_t wapper{array};

    auto& [pointer, size] = wapper;

    stream << wapper << endl;
  
    stream << tpf::types::array_wrapper(array) << endl;

    stream << tpf::types::array_wrapper(darray) << endl;

    auto msg1 = "I love it";
    auto& msg2 = "What?";
    auto v = tpf::make_vector(array);

    stream << v << endl;

    stream<< tpf::types::array_wrapper(array) << endl;
}

int main()
{
    test_array_wrapper();
}