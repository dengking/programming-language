/*
    Author: Thomas Kim
    First Edit: Feb. 03, 2021

    Requirements: C++ compilers that supports C++17 Standards

    clang++ -std=c++17 filename.cpp -ltbb -o c.exe
    g++ -std=c++17 binary_operations.cpp -ltbb -o g.exe
    cl /EHsc /std:c++17 binary_operations.cpp /Fe: m.exe
    dpcpp -Qstd=c++17 /EHsc binary_operations.cpp -o d.exe    
*/

#include <tpf_std_extensions.hpp>
#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto& endl = tpf::endl; // one carriage-return and flush out to console
auto& endL = tpf::endL; // two carriage-returns and flush out to console

void test_tuple_variant_operations()
{
    stream <<"--- Tuple-Variant Binary Operations ---" << endL;

    std::tuple t{1, 2.5, 3.6f};

    using variant_t = std::variant<int, double, float>;

    variant_t v;

    v = 4; 
    auto a = (t - v) - v; stream << "a = " << a << ", Type: " >> a << endl;

    auto av = types::convert_to_container<std::vector>(a);

    stream << av >> av << endl;

    stream <<"\t\t--- By Thomas Kim, Feb. 05, 2020. ---" << endl;
}

int main()
{
    test_tuple_variant_operations();
    
}