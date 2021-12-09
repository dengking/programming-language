#include <tpf_output.hpp>


tpf::sstream stream;
auto endl = tpf::endl;

int which_func(const char* name)
{
    stream << name << endl;
    return {};
}

void which_is_called_first(int a, int b)
{

}

void test_which_called_first()
{
    which_is_called_first(which_func("Left"), which_func("Right"));
}

int main()
{
    test_which_called_first();
}