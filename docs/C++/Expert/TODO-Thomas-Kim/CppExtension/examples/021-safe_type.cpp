#include <tpf_safe_type.hpp>
#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

namespace sft = tpf::safe_type;
namespace tps = tpf::types;

void test_safe_add()
{
    try
    {
        int a = sft::limits_min<int>;
        int b = sft::limits_min<int>;
        
        stream << a  << " + " << b 
            << " = " << sft::safe_add(a, b) << endl;
    }
    catch(std::exception& e)
    {
        stream << e << endl;
    }
}

void test_safe_mul()
{
    try
    {
        int a = sft::;
        int b = 3;
        
        stream << a  << " * " << b 
            << " = " << sft::safe_mul(a, b) << endl;
    }
    catch(std::exception& e)
    {
        stream << e << endl;
    }
}

int main()
{
   // test_safe_add();
   test_safe_mul();
}