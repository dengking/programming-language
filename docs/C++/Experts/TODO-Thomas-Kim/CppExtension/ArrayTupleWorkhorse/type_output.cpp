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

void test_type_output()
{
    
    stream <<L"한글 시험입니다."<< endL;

    std::tuple a{ 1, 2.5, 3.4f};

    stream <<"a's type is " << Tpf_GetTypeCategory(a) << endL;
    stream << "element values: " << a << endl;
    stream << "element types : " >> a << endL;

    std::tuple b{ 1.5, 6.4f, 2};

    stream <<"b's type is " << Tpf_GetTypeCategory(b) << endL;
    stream << "element values: " << b << endl;
    stream << "element types : " >> b << endL;

    auto c = a + b;

    stream << a >> a <<" + " << b >> b <<"\n\n\t= " << c >> c << endL;

    std::tuple d{a, b, 3u};
    std::tuple e{b, a, 4ll};

    stream <<"d's type is " << Tpf_GetTypeCategory(d) << endL;
    stream << "element values: " << d << endl;
    stream << "element types : " >> d << endL;

    stream <<"e's type is " << Tpf_GetTypeCategory(e) << endL;
    stream << "element values: " << e << endl;
    stream << "element types : " >> e << endL;

    auto f = d + e;

    stream <<"f:"<< f << " = d:" << d << " + e:" <<e << endL;

    stream <<"f's type is " << Tpf_GetTypeCategory(f) << endL;
    stream << "element values: " << f << endl;
    stream << "element types : " >> f << endL;

    stream <<"\t\t--- By Thomas Kim, Feb. 04, 2020. ---"<<endL;
}

int main()
{
    tpf::conversion::load_default_locale();

    test_type_output();
}