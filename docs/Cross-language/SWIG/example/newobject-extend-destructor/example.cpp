#include "example.h"
#include <iostream>
MyStruct *foo()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return new MyStruct;
}