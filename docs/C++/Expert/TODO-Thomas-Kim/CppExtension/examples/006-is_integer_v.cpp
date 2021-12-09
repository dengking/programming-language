/**
 * @file 006-is_integer_v.cpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <iostream>
#include <tpf_types.hpp>

void examples_is_integer_v()
{
    namespace types = tpf::types;
    auto& cout = std::cout;
    auto nl = "\n";
    cout << std::boolalpha;

    cout <<"Is [char] an integer: " << types::is_integer_v<char> << " <--------" << nl;
    cout <<"Is [int] an integer: " << types::is_integer_v<int> << nl;
    cout <<"Is [bool] an integer: " << types::is_integer_v<bool> << " <--------" << nl;
    cout <<"Is [double] an integer: " << types::is_integer_v<double> << nl;
    cout <<"Is [long long] an integer: " << types::is_integer_v<long long> << nl;

    cout <<"Is [short] signed: " << types::is_signed_integer_v<short> << nl;
    cout <<"Is [unsigned short] signed: " << types::is_signed_integer_v<unsigned short> << nl;

    cout <<"Is [int] signed: " << types::is_signed_integer_v<int> << nl;
    cout <<"Is [unsigned int] signed: " << types::is_signed_integer_v<unsigned int> << nl;

    cout << "Is [double] signed: " << types::is_signed_integer_v<double> << nl;
    cout << "Is [float] signed: " << types::is_signed_integer_v<float> << nl;

    cout <<"Is [short] unsigned: " << types::is_unsigned_integer_v<short> << nl;
    cout <<"Is [unsigned short] unsigned: " << types::is_unsigned_integer_v<unsigned short> << nl;

    cout <<"Is [int] unsigned: " << types::is_unsigned_integer_v<int> << nl;
    cout <<"Is [unsigned int] unsigned: " << types::is_unsigned_integer_v<unsigned int> << nl;

    cout << "Is [double] unsigned: " << types::is_unsigned_integer_v<double> << " <--------" << nl;
    cout << "Is [float] unsigned: " << types::is_unsigned_integer_v<float> << " <--------" << nl;
}

void examples_is_integral_v()
{
    namespace types = tpf::types;
    auto& cout = std::cout;
    auto nl = "\n";
    cout << std::boolalpha;

    cout <<"Is [char] an integral type: " << types::is_integral_v<char> << " <--------" << nl;
    cout <<"Is [int] an integral type: " << types::is_integral_v<int> << nl;
    cout <<"Is [bool] an integral type: " << types::is_integral_v<bool> << " <--------" << nl;
    cout <<"Is [double] an integral type: " << types::is_integral_v<double> << nl;
    cout <<"Is [long long] an integral type: " << types::is_integral_v<long long> << nl;

    cout <<"Is [short] signed integral type: " << types::is_signed_integral_v<short> << nl;
    cout <<"Is [unsigned short] signed integral type: " << types::is_signed_integral_v<unsigned short> << nl;

    cout <<"Is [int] signed integral type: " << types::is_signed_integral_v<int> << nl;
    cout <<"Is [unsigned int] signed integral type: " << types::is_signed_integral_v<unsigned int> << nl;

    cout << "Is [double] signed integral type: " << types::is_signed_integral_v<double> << nl;
    cout << "Is [float] signed integral type: " << types::is_signed_integral_v<float> << nl;

    cout <<"Is [short] unsigned integral type: " << types::is_unsigned_integral_v<short> << nl;
    cout <<"Is [unsigned short] unsigned integral type: " << types::is_unsigned_integral_v<unsigned short> << nl;

    cout <<"Is [int] unsigned integral type: " << types::is_unsigned_integral_v<int> << nl;
    cout <<"Is [unsigned int] unsigned integral type: " << types::is_unsigned_integral_v<unsigned int> << nl;

    cout << "Is [double] unsigned integral type: " << types::is_unsigned_integral_v<double> << " <--------" << nl;
    cout << "Is [float] unsigned integral type: " << types::is_unsigned_integral_v<float> << " <--------" << nl;
}


int main()
{
    examples_is_integer_v();
    
    std::cout << std::endl;

    examples_is_integral_v();
}