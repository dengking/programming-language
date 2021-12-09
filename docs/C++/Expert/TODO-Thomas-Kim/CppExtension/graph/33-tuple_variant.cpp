#include <tpf_output.hpp>

namespace types = tpf::types;
tpf::sstream stream;
auto endl = tpf::endl;

void test_tuple_of_variants()
{
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    using name_age_vt_t = std::variant<name_t, age_t>;
    using age_weight_vt_t = std::variant<age_t, weight_t>;

    // tuple of variants
    using tuple_t = std::tuple<name_age_vt_t, age_weight_vt_t>;

    tuple_t tpl { name_age_vt_t{ "Sophie Turner" },  age_weight_vt_t{ 22.0 / 7.0 } };

    stream << "tpl = " << tpl << endl;

    auto& name_age = std::get<name_age_vt_t>(tpl);

    name_age = 30;

    stream << "tpl = " << tpl << endl;
}

void test_variant_of_tuples()
{
    using name_t = std::string;
    using age_t = int;
    using weight_t = double;

    using name_age_tpl_t = std::tuple<name_t, age_t>;
    using age_weight_tpl_t = std::tuple<age_t, weight_t>;

    // a variant of tuples
    using variant_t = std::variant<name_age_tpl_t, age_weight_tpl_t>;

    variant_t vt { name_age_tpl_t{"Sophie", 20 } };

    stream << "vt = " << vt << endl;

    vt = age_weight_tpl_t{40, 45.6 };

    stream << "vt = " << vt << endl;
}

int main()
{
    // test_tuple_of_variants();

    test_variant_of_tuples();

}