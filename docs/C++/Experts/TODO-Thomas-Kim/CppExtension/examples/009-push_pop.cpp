#include <tpf_types.hpp>
#include <tpf_output.hpp>

void examples_push_pop()
{
    tpf::ostream stream;
    auto nl = "\n";

    using typelist = tpf::types::type_list_t<short, int, long, double>;

    stream << tpf::types::push_front_type_t<char, typelist>{} << nl;
    stream << tpf::types::push_back_type_t<char, typelist>{} << nl;

    stream << tpf::types::pop_front_type_t<typelist>{} << nl;
    stream << tpf::types::pop_back_type_t<typelist>{} << nl;
    

}

int main()
{
    examples_push_pop();
}