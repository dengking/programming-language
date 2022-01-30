#include <tpf_types.hpp>
#include <tpf_output.hpp>

void examples_is_tuple()
{
    tpf::ostream stream;
    auto nl = "\n";

    using tuple_t = std::tuple<>;

    stream << tpf::types::is_tuple_v<tuple_t> << nl;

    using variant_t = std::variant<>;

    stream << tpf::types::is_variant_v<variant_t> << nl;

    using any_t = std::any;

    stream << tpf::types::is_any_v<any_t> << nl;

    using vctr1_t = std::vector<int>;
    using vctr2_t = std::vector<double>;

    stream << tpf::types::is_same_template_type_v<vctr1_t, vctr2_t> << nl;
    stream << tpf::types::is_same_template_type_v<vctr1_t, vctr1_t> << nl;
    stream << tpf::types::is_same_template_type_v<int, int> << nl;
    stream << tpf::types::is_same_template_v<vctr1_t, vctr2_t> << nl;

    using vvctr_t = std::vector<std::vector<int>>;

    stream << tpf::types::is_template_template_v<vvctr_t> << nl;
    

}

void examples_is_integers()
{
    tpf::ostream stream;
    auto nl = "\n";

    using typelist = tpf::types::type_list_t<int, short, double>;

    stream << typelist{} << " are all numerical numbers: " 
        << tpf::types::are_numerical_numbers_v<typelist> << nl;

    stream << tpf::types::numerical_number_list_t{} << nl;
}

int main()
{
    examples_is_integers();
}
