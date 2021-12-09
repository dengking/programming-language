/*
    Author: Thomas Kim
    First Edit: Feb. 03, 2021

    Requirements: C++ compilers that supports C++17 Standards

    clang++ -std=c++17 filename.cpp -ltbb -o output.exe
    g++ -std=c++17 filename.cpp -ltbb -o output.exe
    cl /EHsc /std:c++17 filename.cpp /Fe: output.exe
    dpcpp -Qstd=c++17 /EHsc filename.cpp -o output.exe
*/

#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto& endl = tpf::endl; // one carriage-return and flush out to console
auto& endL = tpf::endL; // two carriage-returns and flush out to console

void summary_evaluate_lambdas()
{
    stream <<"--- File Contents Summary ---\n\n---We define lambdas as elements of std::tuple\n";
    stream <<"  then we evaluate the lambdas at one function call\n";
    stream <<"  and get the evaluation result through the container of our choice." << endL;
    stream <<"  Example: types::evaluate_lambdas<std::vector>(lambdas, 1.0, 2.0, 3.0) " << endL;
}

void test_evaluate_lambdas()
{
    std::tuple lambdas
    {
        // function 0
        [](auto&& x, auto&& y, auto&& z){ return x + y + z; } ,

        // function 1
        [](auto&& x, auto&& y, auto&& z){ return x * y + z; } ,

        // function 2
        [](auto&& x, auto&& y, auto&& z){ return x + y * z; } ,

        // function 3
        [](auto&& x, auto&& y, auto&& z){ return x - y + z; } ,

        // function 4
        [](auto&& x, auto&& y, auto&& z){ return x + y - z; } ,

        // function 5
        [](auto&& x, auto&& y, auto&& z){ return x / y + z; } ,

        // function 6
        [](auto&& x, auto&& y, auto&& z){ return x + y / z; } ,

        // function 7
        [](auto&& x, auto&& y, auto&& z){ return x / y * z; } ,

        // function 8
        [](auto&& x, auto&& y, auto&& z){ return x * y / z; } ,
        
        // function 9
        [](auto&& x, auto&& y, auto&& z){ return x - y * z; } ,

        // function 10
        [](auto&& x, auto&& y, auto&& z){ return x * y - z; } ,

        // function 11
        [](auto&& x, auto&& y, auto&& z){ return x * y * z; }
    };

    auto v_as_vector1 = types::evaluate_lambdas<std::vector>(lambdas, 1.0, 2.0, 3.0);
    stream <<"lambdas(1.0, 2.0, 3.0) as vector =\n\t" << v_as_vector1 << endL;

    auto v_as_vector2 = types::evaluate_lambdas<std::vector>(lambdas, 2.0, 1.0, 3.0);
    stream <<"lambdas(2.0, 1.0, 3.0) as vector =\n\t" << v_as_vector2 << endL;

    auto v_as_deque1 = types::evaluate_lambdas<std::deque>(lambdas, 1.0, 2.0, 3.0);
    stream <<"lambdas(1.0, 2.0, 3.0) as deque =\n\t" << v_as_deque1 << endL;

    auto v_as_deque2 = types::evaluate_lambdas<std::deque>(lambdas, 2.0, 1.0, 3.0);
    stream <<"lambdas(2.0, 1.0, 3.0) as deque =\n\t" << v_as_deque2 << endL;

    auto v_as_array1 = types::evaluate_lambdas<std::array>(lambdas, 1.0, 2.0, 3.0);
    stream <<"lambdas(1.0, 2.0, 3.0) as array =\n\t" << v_as_array1 << endL;

    auto v_as_array2 = types::evaluate_lambdas<std::array>(lambdas, 2.0, 1.0, 3.0);
    stream <<"lambdas(2.0, 1.0, 3.0) as array =\n\t" << v_as_array2 << endL;

    auto v_as_list1 = types::evaluate_lambdas<std::list>(lambdas, 1.0, 2.0, 3.0);
    stream <<"lambdas(1.0, 2.0, 3.0) as list =\n\t" << v_as_list1 << endL;

    auto v_as_list2 = types::evaluate_lambdas<std::list>(lambdas, 2.0, 1.0, 3.0);
    stream <<"lambdas(2.0, 1.0, 3.0) as list =\n\t" << v_as_list2 << endL;

    auto v_as_multiset1 = types::evaluate_lambdas<std::multiset>(lambdas, 1.0, 2.0, 3.0);
    stream <<"lambdas(1.0, 2.0, 3.0) as multiset =\n\t" << v_as_multiset1 << endL;

    auto v_as_multiset2 = types::evaluate_lambdas<std::multiset>(lambdas, 2.0, 1.0, 3.0);
    stream <<"lambdas(2.0, 1.0, 3.0) as multiset =\n\t" << v_as_multiset2 << endL;

    stream <<"\t\t--- By Thomas Kim, Feb. 05, 2020. ---" << endl;
}

void test_evaluate_lambdas_with_tuple()
{
    std::tuple lambdas
    {
        // function 0
        [](auto&& x, auto&& y, auto&& z){ return x + y + z; } ,

        // function 1
        [](auto&& x, auto&& y, auto&& z){ return x * y + z; } ,

        // function 2
        [](auto&& x, auto&& y, auto&& z){ return x + y * z; } ,

        // function 3
        [](auto&& x, auto&& y, auto&& z){ return x - y + z; } ,

        // function 4
        [](auto&& x, auto&& y, auto&& z){ return x + y - z; } ,

        // function 5
        [](auto&& x, auto&& y, auto&& z){ return x / y + z; } ,

        // function 6
        [](auto&& x, auto&& y, auto&& z){ return x + y / z; } ,

        // function 7
        [](auto&& x, auto&& y, auto&& z){ return x / y * z; } ,

        // function 8
        [](auto&& x, auto&& y, auto&& z){ return x * y / z; } ,
        
        // function 9
        [](auto&& x, auto&& y, auto&& z){ return x - y * z; } ,

        // function 10
        [](auto&& x, auto&& y, auto&& z){ return x * y - z; } ,

        // function 11
        [](auto&& x, auto&& y, auto&& z){ return x * y * z; }
    };

    std::tuple arguments1{1.0, 2.0, 3.0};
    std::tuple arguments2{1.0, 2.0, 3.0};

    auto v_as_vector1 = types::evaluate_lambdas<std::vector>(lambdas, arguments1);
    stream <<"lambdas "<< arguments1 <<" as vector =\n\t" << v_as_vector1 << endL;

    auto v_as_vector2 = types::evaluate_lambdas<std::vector>(lambdas, arguments2);
    stream <<"lambdas "<< arguments2 <<" as vector =\n\t" << v_as_vector2 << endL;
}

int main()
{
    // summary_evaluate_lambdas();
    
    // test_evaluate_lambdas();

    test_evaluate_lambdas_with_tuple();
}