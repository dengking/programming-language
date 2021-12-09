#include <tpf_types.hpp>
#include <tpf_output.hpp>


void examples_select_nth_type()
{
    using typelist = tpf::types::type_list_t<short, int, double>;

    using nth_t = tpf::types::select_nth_type_t<1, typelist>;

    tpf::sstream cout;
    auto nl = tpf::flush;

    cout << tpf::types::type_list_t<nth_t>{}<< nl <<"  "; 

    cout << typelist{} << nl;
}


int main()
{
    examples_select_nth_type();

    tpf::types::type_to_string<int>();
}