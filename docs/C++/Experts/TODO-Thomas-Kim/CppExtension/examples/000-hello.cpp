/**
 * @file 000-hello.cpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.cm)
 * @brief An example code to test system environment variables settings for C++ compilers
 * @version 0.1
 * @date 2019-04-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <iostream>
#include <tpf_type.hpp>

int main()
{
    // declare an instance of type int
    int instance;

    std::cout << "The type category of instance : "
        << Tpf_GetTypeCategory(instance) << std::endl;

    std::cout << "The value category of instance: "
        << Tpf_GetValueCategory(instance) << std::endl;
}