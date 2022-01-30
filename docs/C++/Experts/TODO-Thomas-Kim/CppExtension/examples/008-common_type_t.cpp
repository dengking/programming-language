/**
 * @file 008-common_type_t.cpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief Examples for tpf::types::common_type_t, tpf::types::common_type_v
 * 
 * @version 0.1
 * @date 2019-04-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <iostream>
#include <vector>

#include <tpf_types.hpp>
#include <tpf_output.hpp>

/**
 * @brief Examples for tpf::types::common_type_t<Type1, Type2>
 * 
 */
void examples_common_type_t()
{
    // using namespace tpf::output;
    // introduces stream output operators << for type name display
    using namespace tpf::output;

    // namespace alias
    namespace types = tpf::types;

    auto& cout = std::cout;
    auto nl = "\n";
    cout << std::boolalpha;

    // define two types alias

    using common_type1_t = types::common_type_t<
    
}
/**
 * @brief Examples for type::types::common_type_v<Type1, Type2>
 * 
 */
void examples_common_type_v()
{

}

int main()
{
    examples_common_type_t();
    examples_common_type_v();
}