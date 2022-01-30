/*
    Author: Thomas Kim
    First Edit: Feb. 03, 2021

    Requirements: C++ compilers that supports C++17 Standards

    clang++ -std=c++17 filename.cpp -ltbb -o output.exe
    g++ -std=c++17 filename.cpp -ltbb -o output.exe
    cl /EHsc /std:c++17 filename.cpp /Fe: output.exe
    dpcpp -Qstd=c++17 filename.cpp -o output.exe
*/

#include <tpf_output.hpp>
#include <tpf_std_extensions.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto& endl = tpf::endl; // one carriage-return and flush out to console
auto& endL = tpf::endL; // two carriage-returns and flush out to console

void summary_array_tuple_converstion()
{
    stream << "\n--- File Contents Summary ---\n\n1. Conversion from std::tuple to other container classes" << endL;

    stream << "2. Conversion from std::array to other container classes" << endL;

    stream <<"For details, read the source code below." << endL;

}

void test_conversion_from_tuple_to_container()
{
    stream <<"--- Conversion from std::tuple to other container classes" << endL;

    std::tuple t1 { 1, 3.5, 4.6f, 2.3f, 3u };

    auto a1 = types::convert_to_array(t1);

    stream <<"From " << Tpf_GetTypeCategory(t1) <<" to " << Tpf_GetTypeCategory(a1) << endl;
    stream <<"std::tuple "<<t1<<" is converted to std::array " << a1 << endL;

    auto t2 = types::convert_to_tuple(a1);
    stream <<"From " << Tpf_GetTypeCategory(a1) <<" to " << Tpf_GetTypeCategory(t2) << endl;
    stream <<"std::array "<< a1 <<" is converted to std::tuple " << t2 << endL;

    auto v1 = types::convert_to_container<std::vector>(t1);
    stream <<"From " << Tpf_GetTypeCategory(t1) <<" to " << Tpf_GetTypeCategory(v1) << endl;
    stream <<"std::tuple "<< t1 <<" is converted to std::vector " << v1 << endL;

    auto d1 = types::convert_to_container<std::deque>(t1);
    stream <<"From " << Tpf_GetTypeCategory(t1) <<" to " << Tpf_GetTypeCategory(d1) << endl;
    stream <<"std::tuple "<< t1 <<" is converted to std::deque " << d1 << endL;

    // STEP 1. Convert the type of t1, tuple, to vector of variants
    using vector_of_variants_t = 
        types::tuple_to_container_of_variants_t<std::vector, decltype(t1)>;

    // STEP 2. Convert tuple t1 to vector of variants vv
    auto vv = types::convert_to_container<vector_of_variants_t>(t1);

    stream <<"From " << Tpf_GetTypeCategory(t1) <<" to\n  " << Tpf_GetTypeCategory(vv) << endl;
    stream <<"std::tuple "<< t1 <<" is converted to vector of variants " << vv << endL;

    // STEP 1. Convert the type of t1, tuple, to deque of variants
    using deque_of_variants_t = 
        types::tuple_to_container_of_variants_t<std::deque, decltype(t1)>;

    // STEP 2. Convert tuple t1 to deque of variants dv
    auto dv = types::convert_to_container<deque_of_variants_t>(t1);

    stream <<"From " << Tpf_GetTypeCategory(t1) <<" to\n  " << Tpf_GetTypeCategory(dv) << endl;
    stream <<"std::tuple "<< t1 <<" is converted to deque of variants " << dv << endL;

}

void test_conversion_from_array_to_container()
{
    stream <<"--- Conversion from std::array to other container classes" << endL;

    std::array a1 { 1.3, 3.5, 4.6, 2.3, 3.0 };

    auto t1 = types::convert_to_tuple(a1);

    stream <<"From " << Tpf_GetTypeCategory(a1) <<" to " << Tpf_GetTypeCategory(t1) << endl;
    stream <<"std::array "<< a1 <<" is converted to std::tuple " << t1 << endL;

    auto v1 = types::convert_to_container<std::vector>(a1);
    stream <<"From " << Tpf_GetTypeCategory(a1) <<" to " << Tpf_GetTypeCategory(v1) << endl;
    stream <<"std::array "<< a1 <<" is converted to std::vector " << v1 << endL;

    auto d1 = types::convert_to_container<std::deque>(t1);
    stream <<"From " << Tpf_GetTypeCategory(a1) <<" to " << Tpf_GetTypeCategory(d1) << endl;
    stream <<"std::array "<< a1 <<" is converted to std::deque " << d1 << endL;

    auto ms = types::convert_to_container<std::multiset>(a1);
    stream <<"From " << Tpf_GetTypeCategory(a1) <<" to " << Tpf_GetTypeCategory(ms) << endl;
    stream <<"std::array "<< a1 <<" is converted to multiset " << ms << endL;

    auto ums = types::convert_to_container<std::unordered_multiset>(a1);
    stream <<"From " << Tpf_GetTypeCategory(a1) <<" to\n  " << Tpf_GetTypeCategory(ums) << endl;
    stream <<"std::array "<< a1 <<" is converted to unordered_multiset " << ums << endL;

    stream <<"\t\t--- By Thomas Kim, Feb. 05, 2020. ---" << endl;
}

int main()
{
    summary_array_tuple_converstion();

    test_conversion_from_tuple_to_container();

    test_conversion_from_array_to_container();
}