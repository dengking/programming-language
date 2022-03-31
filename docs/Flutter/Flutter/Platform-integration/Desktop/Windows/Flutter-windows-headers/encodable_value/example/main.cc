#include <vector>
#include <iostream>
#include <variant>
#include "encodable_value.h"

int main()
{
    flutter::EncodableMap m;
    m[flutter::EncodableValue("flag")] = true;
    m[flutter::EncodableValue("name")] = "Thing";
    std::vector<int> v{1, 2, 3, 4};
    m[flutter::EncodableValue("values")] = v;
    if (auto *p = std::get_if<bool>(&m[flutter::EncodableValue("flag")]))
    {
        std::cout << *p << std::endl;
    }
    if (auto *p = std::get_if<std::string>(&m[flutter::EncodableValue("name")]))
    {
        std::cout << *p << std::endl;
    }
    if (auto *p = std::get_if<std::vector<int>>(&m[flutter::EncodableValue("values")]))
    {
        std::copy(p->begin(), p->end(), std::ostream_iterator<int>(std::cout, " "));
    }
}