#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;
auto nl = tpf::nl;

void test_make_vector()
{
    int a = 5;
    const int c = 6;
    int& lref = a;
    int&& rref = 7;

    auto v1 = tpf::make_vector(1, a, c, lref, rref);

    stream << "Type of v1: " << Tpf_GetTypeCategory(v1) << nl
        << "V1 = " << v1 << endl;

    double d = 5;
    const double e = 6.0;
    double& f = d;
    double&& g = 7.0;

    auto v2 = tpf::make_vector(a, c, lref, rref, d, e, f, g);

    stream << "Type of v2: " << Tpf_GetTypeCategory(v2) << nl
        << "V2 = " << v2 << endl;

    auto v3 = tpf::make_vector("James Dean", "Sophie Turner", "Members");
    
    stream << "Type of v3: " << Tpf_GetTypeCategory(v3) << nl
        << "V3 = " << v3 << endl;

    int array1[]{1, 2, 3};
    int array2[]{4, 5, 6};
    int array3[]{7, 8, 9, 10};

    auto v4 = tpf::make_vector(array1, array2, array3);

     stream << "Type of v4: " << Tpf_GetTypeCategory(v4) << nl
        << "V4 = " << v4 << endl;

    std::pair<int, double> p;
    
}

int main()
{
    test_make_vector();
}