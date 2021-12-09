#include <tpf_euclidean.hpp>
#include <tpf_output.hpp>

using namespace tpf;

void examples_euclidean()
{
    ostream stream;
    auto nl = "\n";

    // stream  << euclidean::gcd(6, 10, 12) << nl;
    stream  << euclidean::lcm(6, 10, 12) << nl;

    std::vector v{6, 10, 12};

    // stream << euclidean::gcd(v) << nl;
    stream << euclidean::lcm(v) << nl;

}

int main()
{
    examples_euclidean();
}