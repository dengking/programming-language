/*
    Author: Thomas Kim
    First Edit: Feb. 03, 2021

    Requirements: C++ compilers that supports C++17 Standards

    clang++ -std=c++17 filename.cpp -ltbb -o output.exe
    g++ -std=c++17 filename.cpp -ltbb -o output.exe
    cl /EHsc /std:c++17 filename.cpp /Fe: output.exe
    dpcpp -Qstd=c++17 filename.cpp -o output.exe

    Does CREATE a new type - the argument tuple is not changed after operation
    std::apply_operation(operation, tuple) 

    Does NOT create a new type - INPLACE operation - the argument tuple is MODIFIED after operation
    std::apply_operation_inplace(operation, tuple)

    Does CREATE a new type - the argument array is not changed after operation
    std::apply_operation(operation, array)

    Does NOT create a new type - INPLACE operation - the argument array is MODIFIED after operation
    std::apply_operation_inplace(operation, array)
*/

#include <tpf_output.hpp>
#include <tpf_std_extensions.hpp>
#include <cmath>

namespace types = tpf::types;

tpf::sstream stream;
auto& endl = tpf::endl; // one carriage-return and flush out to console
auto& endL = tpf::endL; // two carriage-returns and flush out to console

void summary_apply_binary_operation_to_array_tuple()
{
    stream <<"\n--- Apply operations to std::array and std::tuple --- " << endL;

    stream <<"1. std::apply_operation(operation, tuple) - NON-INPLACE operation" << endL;
    stream <<"\tApply operation to elements of tuple." << endl;
    stream <<"\tThe argument tuple is NOT modified after operation." << endl;
    stream <<"\tA new instance of std::tuple is Created and Returned to its caller." << endL;

    stream <<"2. std::apply_operation_inplace(operation, tuple) - INPLACE operation" << endL;
    stream <<"\tApply operation to elements of tuple." << endl;
    stream <<"\tThe argument tuple is MODIFIED after operation." << endL;

    stream <<"3. std::apply_operation(operation, array) - NON-INPLACE operation" << endL;
    stream <<"\tApply operation to elements of array." << endl;
    stream <<"\tThe argument array is NOT modified after operation." << endl;
    stream <<"\tA new instance of std::array is Created and Returned to its caller." << endL;

    stream <<"4. std::apply_operation_inplace(operation, array) - INPLACE operation" << endL;
    stream <<"\tApply operation to elements of array." << endl;
    stream <<"\tThe argument array is MODIFIED after operation." << endL;
}

void test_apply_operation_to_tuple()
{
    stream <<"--- Apply Batch Operation to Tuple Elements---" << endL;

    std::tuple a{1, 2.4, 4};

    stream<< "1. Apply Element-Wise Operation to SIMPLE Tuple - std::apply_operation(operation, tuple)" << endL;

    stream <<" std::tuple a = " << a << endL;

    stream << " WARNING: does CREATE a new type!!" << endL;

    stream <<" a * a = " << 
        std::apply_operation( [](auto&& x){ return x * x; }, a) << endL;

    stream <<" sin(a) = " << 
        std::apply_operation( [](auto&& x){ return std::sin(x); }, a) << endL;

    stream<< "2. Apply Element-wise IN-PLACE Operation to SIMPLE Tuple - std::apply_operation_inplace(operation, tuple)" << endL;
    stream << " WARNING: does NOT create a new type!!" << endL;
    
    stream <<" Before a = " << a << endL;

    // in-place operation - tuple a is modified
    std::apply_operation_inplace( [](auto&& x){ return std::sin(x); }, a);

    stream <<" After sin(a) = " << a << endL;

    stream <<" !! WARNGING !! sin(1) == sin(4) == 0, because 1 and 4 are integral types" << endL;

    stream<< "3. Apply Element-wise Operation to Nested Tuple - std::apply_operation(operation, tuple)" << endL;

    std::tuple b{1.5, std::tuple{1, 2.4, 4}, std::tuple{1, 2.4, std::tuple{3.5, 5.6} } };

    stream << " std::tuple b = " << b << endL;

    stream <<" sin(b) = " << 
        std::apply_operation( [](auto&& x){ return std::sin(x); }, b) << endL;

    stream <<" WARNING: a new type is created!!" << endL;

    stream<< "4. Apply Element-wise IN-PLACE Operation to Nested Tuple - std::apply_operation_inplace(operation, tuple)" << endL;
    stream << " WARNING!! does NOT create a new type!!" << endL;

    std::tuple c{1, std::tuple{2, 2.4, 4.3}, std::tuple{3, 2.4, std::tuple{3.5, 5.6} } };

    stream <<" Before c = " << c << endL;

    // in-place operation - tuple c is modified
    std::apply_operation_inplace( [](auto&& x){ return std::sin(x); }, c);

    stream <<" After sin(c) = " << c << endL;

    stream << " WARNING!! does NOT create a new type!!" << endL;

    stream <<" !! WARNGING !! sin(1) == sin(2) == sin(3) == 0, because 1, 2, and 3 are integral types" << endL;
}

void test_apply_operation_to_array()
{
    stream << "--- Apply Batch Operation to Array Elements ---" << endL;

    stream<< "0. WARNING!! The types of the elements of a std::array should be IDENTICAL!!" << endL;

    std::array a{1, 2, 3};

    stream<< "1. Apply Operation to SIMPLE, Not Nested array - std::apply_operation(lambda, a)" << endL;
 
    stream << " WARNING: namespace std - does create a new type" << endL;
 
    stream <<" std::array a = " << a << endL;

    stream <<" a * 1.3 = " << 
        std::apply_operation( [](auto&& x){ return x * 1.3; }, a) << endL;

    stream <<" sin(a) = " << 
        std::apply_operation( [](auto&& x){ return std::sin(x); }, a) << endL;

    stream << " WARNING: a new type is created in the above!!" << endL;

    stream<< "2. Apply Operation IN-PLACE to array - std::apply_operation_inplace(lambda, a)" << endL;
    stream<< " WARNING: New type is not created!" << endL;

    stream <<" Before a = " << a << endL;

    // in-place operation - tuple a is modified
    std::apply_operation_inplace( [](auto&& x){ return std::sin(x); }, a);

    stream <<" After sin(a) = " << a << endL;

    stream <<" !! WARNGING !! sin(1) == sin(2) == sin(3) == 0, because 1, 2, and 3 are integral types" << endL;

    stream<< "3. Apply Operation to Nested Array - std::apply_operation(lambda, b)"<<endL;

    std::array b{ std::array{1, 2, 3}, std::array{4, 5, 6} };

    stream << " std::array b = " << b << endL;

    stream <<" sin(b) = " << 
        std::apply_operation( [](auto&& x){ return std::sin(x); }, b) << endL;

    stream <<" WARNING: a new type is created!!" << endL;

    stream<< "4. Apply IN-PLACE Operation to Nested Array - std::apply_operation_inplace(lambda, c)"<<endL;

    std::array c{ std::array{1.2, 2.4, 3.5}, std::array{4.3, 5.6, 6.2} };

    stream << " Before std::array c = " << c << endL;
    
    std::apply_operation_inplace( [](auto&& x){ return std::sin(x); }, c);

    stream <<" sin(c) = " << c << endL;

    stream <<" WARNING: std::array c is modified!!" << endL;

    stream <<"\t\t--- By Thomas Kim, Feb. 05, 2020. ---" << endl;
}


int main()
{
    summary_apply_binary_operation_to_array_tuple();

    test_apply_operation_to_tuple();

    test_apply_operation_to_array();
}