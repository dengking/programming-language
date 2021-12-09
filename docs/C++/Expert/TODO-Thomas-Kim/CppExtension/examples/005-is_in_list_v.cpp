/**
 * @file 005-is_in_list_v.cpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief Examples for tpf::types::is_in_list_v<TestType, Types...>
 * @version 0.1
 * @date 2019-04-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <iostream>
#include <tpf_types.hpp>

void example_is_in_list_v()
{
    namespace types = tpf::types;
    auto& cout = std::cout;
    auto nl = "\n";

    cout << std::boolalpha;

    cout << "Is int in <>: "
        << types::is_in_list_v<int> << nl;

    cout << "Is int in <int>: "
        << types::is_in_list_v<int, int> << nl;

    cout << "Is int in <short>: "
        << types::is_in_list_v<int, short> << nl;

    cout << "Is char in <short, int, double>: "
        << types::is_in_list_v<char, short, int, double> << nl;

    cout << "Is int in <short, int, double>: "
        << types::is_in_list_v<int, short, int, double> << nl;

    using typelist1_t = types::type_list_t<>;
    cout << "Is int in <>: " << types::is_in_list_v<int, typelist1_t> << nl;

    using typelist2_t = types::type_list_t<int>;
    cout << "Is int in <int>: " << types::is_in_list_v<int, typelist2_t> << nl;

    using typelist3_t = types::type_list_t<int, float, double>;
    cout << "Is short in <int, float, double>: " << types::is_in_list_v<short, typelist3_t> << nl;

    cout << "Is int in <int, float, double>: " << types::is_in_list_v<int, typelist3_t> << nl;

    cout << "Is double in <int, float, double>: " << types::is_in_list_v<double, typelist3_t> << nl;


}

int main()
{
    example_is_in_list_v();
}