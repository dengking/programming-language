#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

void test_make_variant()
{
    auto v = tpf::types::make_variants(5.0f, "I love it", "So Cool");

    // stream <<"Type of v: " << Tpf_GetTypeCategory(v) << endl;

    stream << v << endl;

}

int main()
{
    test_make_variant();
}