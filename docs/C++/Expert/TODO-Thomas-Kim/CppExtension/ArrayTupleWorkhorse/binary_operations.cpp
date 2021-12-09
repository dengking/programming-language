/*
    Author: Thomas Kim
    First Edit: Feb. 03, 2021

    Requirements: C++ compilers that supports C++17 Standards

    clang++ -std=c++17 filename.cpp -ltbb -o c.exe
    g++ -std=c++17 binary_operations.cpp -ltbb -o g.exe
    cl /EHsc /std:c++17 binary_operations.cpp /Fe: m.exe
    dpcpp -Qstd=c++17 /EHsc binary_operations.cpp -o d.exe    
*/

#include <tpf_output.hpp>
#include <tpf_std_extensions.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto& endl = tpf::endl; // one carriage-return and flush out to console
auto& endL = tpf::endL; // two carriage-returns and flush out to console

void summary_for_binary_operators_for_array_tuple()
{
    stream <<"\n--- Summary for Binary Operators for std::array and std::tuple --- "<<endL;

    stream <<"1. tuple-tuple binary operators +, -, *, and / are defined !!" << endL;

    stream <<"2. scalar-tuple, tuple-scalar binary operators +, -, *, and / are defined !!" << endL;

    stream <<"3. array-array binary operators +, -, *, and / are defined !!" << endL;

    stream <<"4. scalar-array, array-scalar binary operators +, -, *, and / are defined !!" << endL;

    stream <<"5. operators ==, !=, >, <, >=, <= are defined for the same type of std::array<T, Size> in C++ Standard."<<endl;
    stream <<"   But I implemented them for differnt types of std::array< T, Size1 > and std::array< S, Size2 >."<<endL;
}

void test_array_binary_operations()
{
    stream <<"--- Array Binary Operations ---" << endL;

    std::array a{1.0, 2.5, 3.5};
    std::array b{1,   2,   3};
    std::array c{2.0f, 2.5f, 3.2f};

    stream <<"std::array a = " << a << endl;
    stream <<"std::array b = " << b << endl;
    stream <<"std::array c = " << c << endL;

    stream<<" 1. Simple Binary Operations with Different Types" << endL;
    stream << a << " + " << b << " = " << (a + b) << endl;
    stream << a << " - " << b << " = " << (a - b) << endl;
    stream << a << " * " << b << " = " << (a * b) << endl;
    stream << a << " / " << b << " = " << (a / b) << endL;

    std::array aa{2.0, 3.5, 7.5};
    std::array bb{3,   1,   2};
    std::array cc{3.0f, 1.5f, 2.2f};

    std::array d{a, aa};
    std::array e{b, bb};
    std::array f{c, cc};

    stream <<" d = " << d << endl;
    stream <<" e = " << e << endl;
    stream <<" f = " << f << endL;

    stream <<" 2. Nested Binary Operation with Different Types" << endL;
    stream << d << " + " << e << " = " <<(d + e) << endl;
    stream << d << " - " << e << " = " <<(d - e) << endl;
    stream << d << " * " << f << " = " <<(d * f) << endl;
    stream << d << " / " << f << " = " <<(d / f) << endL;

    stream <<" 3. Simple Scalar-Tuple, Tuple-Scalar Operations" << endL;
    stream <<" 2 + "<< a <<" = " << (2 + a) << endL;
    stream <<" " << a <<" + "<< 3 << " = " << (a + 3) << endL;
    stream <<" 2 - "<< a <<" = " << (2 - a) << endL;
    stream <<" " << a <<" - "<< 3 << " = " << (a - 3) << endL;
    stream <<" 2 * "<< a <<" = " << (2 * a) << endL;
    stream <<" " << a <<" * "<< 3 << " = " << (a * 3) << endL;
    stream <<" 2 / "<< a <<" = " << (2 / a) << endL;
    stream <<" " << a <<" / "<< 3 << " = " << (a / 3) << endL;

    stream <<" 4. Nested Scalar-Tuple, Tuple-Scalar Operations" << endL;
    stream <<" 2 + "<< d <<" = " << (2 + d) << endL;
    stream <<" " << d <<" + "<< 3 << " = " << (d + 3) << endL;
    stream <<" 2 - "<< d <<" = " << (2 - d) << endL;
    stream <<" " << d <<" - "<< 3 << " = " << (d - 3) << endL;
    stream <<" 2 * "<< d <<" = " << (2 * d) << endL;
    stream <<" " << d <<" * "<< 3 << " = " << (d * 3) << endL;
    stream <<" 2 / "<< d <<" = " << (2 / d) << endL;
    stream <<" " << d <<" / "<< 3 << " = " << (d / 3) << endL;
}

void test_tuple_binary_operations()
{
    stream << "--- Tuple Binary Operations ---" << endL;

    std::tuple a{1,   2.5, 3.5};
    std::tuple b{1.5, 2,   3};
    std::tuple c{1, 2, 3 };

    stream <<" a = " << a << endl;
    stream <<" b = " << b << endl;
    stream <<" c = " << c << endL;

    stream<<" 1. Simple Binary Operations with Different Types" << endL;
    stream << a << " + " << b << " = " << (a + b) << endl;
    stream << a << " - " << b << " = " << (a - b) << endl;
    stream << a << " * " << b << " = " << (a * b) << endl;
    stream << a << " / " << b << " = " << (a / b) << endL;

    std::tuple d{a, b};
    std::tuple e{a, c};
    std::tuple f{b, c};

    stream <<" d = " << d << endl;
    stream <<" e = " << e << endl;
    stream <<" f = " << f << endL;

    stream<<" 2. Nested Binary Operation with Different Types" << endL;
    stream <<" d + e = "<<(d + e) << endl;
    stream <<" d - e = "<<(d - e) << endl;
    stream <<" e * f = "<<(e * f) << endl;
    stream <<" e / f = "<<(e / f) << endL;

    stream <<" 3. Simple Scalar-Tuple, Tuple-Scalar Operation" << endL;
    stream <<" 2 + "<< a <<" = " << (2 + a) << endL;
    stream <<" "<< a <<" + "<< 3 << " = " << (a + 3) << endL;
    stream <<" 2 - "<< a <<" = " << (2 - a) << endL;
    stream <<" "<< a <<" - "<< 3 << " = " << (a - 3) << endL;
    stream <<" 2 * "<< a <<" = " << (2 * a) << endL;
    stream <<" "<< a <<" * "<< 3 << " = " << (a * 3) << endL;
    stream <<" 2 / "<< a <<" = " << (2 / a) << endL;
    stream <<" "<< a <<" / "<< 3 << " = " << (a / 3) << endL;

    stream <<" 4. Nested Scalar-Tuple, Tuple-Scalar Operation" << endL;
    stream <<" 2 + "<< d <<" = " << (2 + d) << endL;
    stream <<" "<< d <<" + "<< 3 << " = " << (d + 3) << endL;
    stream <<" 2 - "<< d <<" = " << (2 - d) << endL;
    stream <<" "<< d <<" - "<< 3 << " = " << (d - 3) << endL;
    stream <<" 2 * "<< d <<" = " << (2 * d) << endL;
    stream <<" "<< d <<" * "<< 3 << " = " << (a * 3) << endL;
    stream <<" 2 / "<< d <<" = " << (2 / d) << endL;
    stream <<" "<< d <<" / "<< 3 << " = " << (d / 3) << endL;
}

void test_tuple_equality_comparison_binary_operations()
{
    stream << "--- File Contents Summary ---\n\n--- Tuple Equality Comparison Operators - defined in Standard C++! " << endL;

    std::tuple a{1,   2.5, 3.5};
    std::tuple b{1.5, 2,   3};
    std::tuple<double, double, double> c{1,   2.5, 3.5};
    
    stream <<" std::tuple<int, double, double>     a = " << a << endL;
    stream <<" std::tuple<double, int, int>        b = " << b << endL;
    stream <<" std::tuple<double, double, double>  c = " << c << endL;
    
    stream<<" 1. Simple Equality Operators" << endL;
    stream << " (a == a) ? " << (a == a) << endl;
    stream << " (a == b) ? " << (a == b) << " <- Comparison Between Differet Types" << endl;
    stream << " (a != a) ? " << (a != a) << endl;
    stream << " (a != b) ? " << (a != b) << " <- Comparison Between Differet Types" << endl;
    stream << " (a == c) ? " << (a == c) << " <- Comparison Between Differet Types" << endL;
    
    stream<<" 2. Nested Equality Operators" << endL;
        
    std::tuple d{a, b};
    std::tuple e{a, c};

    stream <<" d = " << d << endl;
    stream <<" e = " << e << endl;
    
    stream <<" (d == d) ? " << (d == d) << endl;
    stream <<" (d == e) ? " << (d == e) << endl;
    stream <<" (d != d) ? " << (d != d) << endl;
    stream <<" (d != e) ? " << (d != e) << endL;

    stream<<" 3. Inequality Operators" << endL;
        
    stream <<" d = " << d << endl;
    stream <<" e = " << e << endl;
    
    stream <<" (d > d) ? " << (d > d) << endl;
    stream <<" (d > e) ? " << (d > e) << endl;
    stream <<" (d < e) ? " << (d < e) << endL;    
}

void test_array_equality_comparison_same_size_operations()
{
    stream <<"--- Array Equality Comparison Operators With Different Types With the Same Size! " << endL;

    std::array a{1, 2, 3};
    std::array b{1.0, 2.0, 3.0};
    std::array c{1.0, 2.5, 3.0};

    stream <<" std::array<int, 3>    a = " << a << endl;
    stream <<" std::array<double, 3> b = " << b << endl;
    stream <<" std::array<double, 3> c = " << c << endL;

    stream << "1. Array Equality with Different Type, of Same Size! " << endL;

    stream <<"<int, 3>"<< a <<" == "<< b << "  <double, 3>  (all elements pairwise equal) ? "
        << (a == b) << endl;

    stream <<"<int, 3>"<< a <<" == "<< c << "<double, 3>  (all elements pairwise equal) ? "
        << (a == c) << endl;

    stream <<"<int, 3>"<< a <<" != "<< b << "  <double, 3> !(all elements pairwise equal) ? "
        << (a != b) << endl;

    stream <<"<int, 3>"<< a <<" != "<< c << "<double, 3> !(all elements pairwise equal) ? "
        << (a != c) << endL;

    stream << "2. Array Comparison with Different Type, of Same Size! " << endL;

    stream <<"<int, 3> "<< a << " >  " << b <<" <double, 3>  (all elements pairwise >  ) ? "
        << (a > b) << endl;

    stream <<"<int, 3> "<< a << " >= " << b <<" <double, 3>  (all elements pairwise >= ) ? "
        << (a >= b) << endl;

    stream <<"<int, 3> "<< a << " <  " << b <<" <double, 3>  (all elements pairwise <  ) ? "
        << (a < b) << endl;

    stream <<"<int, 3> "<< a << " <= " << b <<" <double, 3>  (all elements pairwise <= ) ? "
        << (a <= b) << endl;

    stream <<"<int, 3> "<< a << " <  " << c <<"<double, 3> (all elements pairwise <  ) ? "
        << (a < c) << endl;

    stream <<"<int, 3> "<< a << " <= " << c <<"<double, 3> (all elements pairwise <= ) ? "
        << (a <= c) << endl;

    stream <<"<int, 3> "<< a << " >  " << c <<"<double, 3> (all elements pairwise >  ) ? "
        << (a > c) << endl;

    stream <<"<int, 3> "<< a << " >= " << c <<"<double, 3> (all elements pairwise >= ) ? "
        << (a >= c) << endL;
}

void test_array_equality_comparison_different_size_operations()
{
    stream << "--- Array Equality Comparison Operators With Same Types With DIFFERENT Size! " << endL;

    std::array a{1, 2, 3};
    std::array b{1, 2, 3, 4};
    
    stream <<" std::array<int, 3> a = " << a << endl;
    stream <<" std::array<int, 4> b = " << b << endL;
    
    stream << a << " == " << b << " ? " << (a == b) << endl;
    stream << a << " != " << b << " ? " << (a != b) << endl;

    stream << a << "  < " << b << " ? " << (a < b) << endl;
    stream << a << " <= " << b << " ? " << (a <= b) << endl;
    stream << a << "  > " << b << " ? " << (a > b) << endl;
    stream << a << " >= " << b << " ? " << (a >= b) << endL;

    stream <<"\t\t--- By Thomas Kim, Feb. 05, 2020. ---" << endl;
}

int main()
{
    summary_for_binary_operators_for_array_tuple();

    test_array_binary_operations();

    test_tuple_binary_operations();

    test_tuple_equality_comparison_binary_operations();

    test_array_equality_comparison_same_size_operations();

    test_array_equality_comparison_different_size_operations();
}