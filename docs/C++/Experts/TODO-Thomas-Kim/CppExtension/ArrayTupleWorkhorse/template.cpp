/*
    Author: Thomas Kim
    First Edit: Feb. 03, 2021

    Requirements: C++ compilers that supports C++17 Standards

    clang++ -std=c++17 filename.cpp -ltbb -o output.exe
    g++ -std=c++17 filename.cpp -ltbb -o output.exe
    cl /EHsc /std:c++17 filename.cpp /Fe: output.exe
    dpcpp -Qstd=c++17 filename.cpp -o output.exe
*/

#include <tpf_std_extensions.hpp>
#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto& endl = tpf::endl; // one carriage-return and flush out to console
auto& endL = tpf::endL; // two carriage-returns and flush out to console

int main()
{

}