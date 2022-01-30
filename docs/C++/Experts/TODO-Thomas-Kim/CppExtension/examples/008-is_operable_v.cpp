/**
 * @file 008-is_operable_v.cpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief Examples for types::is_operable_v
 * @version 0.1
 * @date 2019-04-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <iostream>
#include <vector>

#include <tpf_types.hpp>
#include <tpf_output.hpp>

/**
 * @fn examples_is_operable_v()
 * @brief Test is_operable_v type function.
 * 
 */
void examples_is_operable_v()
{
    namespace types = tpf::types;
    auto& cout = std::cout;
    auto nl = "\n";
    cout << std::boolalpha;

    cout <<"Is [int] and [double] operable? " <<
        types::is_operable_v<int, double> << nl;

    cout <<"Is [int] and [int*] operable? " <<
        types::is_operable_v<int, int*> << nl;
}

void examples_is_operable_t()
{

}



int main()
{
    examples_is_operable_v();
    
    std::cout << std::endl;

    examples_common_type_t();
}