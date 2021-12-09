/**
 * @file 005-is_same_v.cpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief Examples for tpf::types::is_same_v<Type, Types...>
 * @version 0.1
 * @date 2019-04-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <iostream>
#include <tpf_types.hpp>

/**
 * @brief Examples for tpf::types::is_same_v<Type, Types...>
 * 
 */
void examples_is_same_v()
{
    namespace types = tpf::types;

    auto& cout = std::cout;
    auto nl = "\n";

    cout << std::boolalpha;
    
    using type1_t = int;
    using type2_t = double;
    using type3_t = short;

    cout << "is_same_v<int>: " 
        << types::is_same_v<int> << nl;

    cout << "is_same_v<int, int>: " 
        << types::is_same_v<int, int> << nl;

    cout << "is_same_v<int, int, int>: " 
        << types::is_same_v<int, int, int> << nl;

    cout << "is_same_v<int, short>: " 
        << types::is_same_v<int, short> << nl;

    using typelist1_t = types::type_list_t<int>;
    cout << "is_same_v< type_list_t<int> > : " 
        << types::is_same_v<typelist1_t> << nl;

    using typelist2_t = types::type_list_t<int, int>;
    cout << "is_same_v< type_list_t<int, int> > : " 
        << types::is_same_v<typelist2_t> << nl;

    using typelist3_t = types::type_list_t<int, short>;
    cout << "is_same_v< type_list_t<int, short> > : " 
        << types::is_same_v<typelist3_t> << nl;

    using typelist4_t = types::type_list_t<int, int, int>;
    cout << "is_same_v< type_list_t<int, int, int> > : " 
        << types::is_same_v<typelist4_t> << nl;

    using typelist5_t = types::type_list_t<int, int, int, double>;
    cout << "is_same_v< type_list_t<int, int, int, double> > : " 
        << types::is_same_v<typelist5_t> << nl;
}

int main()
{
    examples_is_same_v();
}