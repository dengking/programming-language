#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>
#include <tpf_set.hpp>

namespace ncrnpr = tpf::ncrnpr;

tpf::sstream stream;
auto& endl = tpf::endl;
auto nl = tpf::nl;

void test_permutation()
{
    auto permutations = tpf::set::build_permutations(10, 3, 3);

    for(auto p: permutations)
    {
        stream << p << ": " << tpf::set::hidden::inversion(p) << nl;
    }

    stream << endl;
    
}

int main()
{
    test_permutation();
}