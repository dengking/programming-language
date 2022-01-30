/**
 * @file 007-make_signed_t.cpp
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

void examples_make_signed_unsigned_t()
{
    namespace types = tpf::types;
    auto& cout = std::cout;
    auto nl = "\n";
    cout << std::boolalpha;

    cout << "Make [char] unsigned: " 
        << Tpf_GetTypeName(types::make_unsigned_t<char>) <<" <-----" << nl;

    cout << "Make [bool] signed: " 
        << Tpf_GetTypeName(types::make_signed_t<bool>) << " <-----" << nl;

    cout << "Make [double] signed: " 
        << Tpf_GetTypeName(types::make_signed_t<double>) << " <-----" << nl;

    cout << "Make [int] unsigned: " 
        << Tpf_GetTypeName(types::make_unsigned_t<int>) << nl;

    cout << "Make [unsigned int] signed: " 
        << Tpf_GetTypeName(types::make_signed_t<int>) << nl;
}

void examples_make_signed_unsigned_integral_t()
{
    namespace types = tpf::types;
    auto& cout = std::cout;
    auto nl = "\n";
    cout << std::boolalpha;

    cout << "Make [char] unsigned integral type: " 
        << Tpf_GetTypeName(types::make_unsigned_integral_t<char>) <<" <-----" << nl;

    cout << "Make [bool] signed integral type: " 
        << Tpf_GetTypeName(types::make_signed_integral_t<bool>) << " <-----" << nl;

    cout << "Make [double] signed integral type: " 
        << Tpf_GetTypeName(types::make_signed_integral_t<double>) << " <-----" << nl;

    cout << "Make [int] unsigned integral type: " 
        << Tpf_GetTypeName(types::make_unsigned_integral_t<int>) << nl;

    cout << "Make [unsigned int] signed integral type: " 
        << Tpf_GetTypeName(types::make_signed_integral_t<int>) << nl;

}

int main()
{
    examples_make_signed_unsigned_t();
    
    std::cout << std::endl;

    examples_make_signed_unsigned_integral_t();
}