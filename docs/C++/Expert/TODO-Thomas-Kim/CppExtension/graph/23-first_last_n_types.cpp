#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto endl = tpf::endl;

void test_first_n_types()
{
    using types_t = types::type_list_t<short, int, float, double>;
    stream <<"Types: " << types_t{} << endl;

    using first_0_types_1 = types::first_n_types_t<0, short, int, float, double>;
    using first_1_types_1 = types::first_n_types_t<1, short, int, float, double>;
    using first_2_types_1 = types::first_n_types_t<2, short, int, float, double>;
    using first_3_types_1 = types::first_n_types_t<3, short, int, float, double>;
    using first_4_types_1 = types::first_n_types_t<4, short, int, float, double>;
    using first_5_types_1 = types::first_n_types_t<5, short, int, float, double>;

    stream << "First 0 types: " << first_0_types_1{} << endl;
    stream << "First 1 types: " << first_1_types_1{} << endl;
    stream << "First 2 types: " << first_2_types_1{} << endl;
    stream << "First 3 types: " << first_3_types_1{} << endl;
    stream << "First 4 types: " << first_4_types_1{} << endl;
    stream << "First 5 types: " << first_5_types_1{} << endl;

    stream << endl;

    using first_0_types_2 = types::first_n_types_t<0, types_t>;
    using first_1_types_2 = types::first_n_types_t<1, types_t>;
    using first_2_types_2 = types::first_n_types_t<2, types_t>;
    using first_3_types_2 = types::first_n_types_t<3, types_t>;
    using first_4_types_2 = types::first_n_types_t<4, types_t>;
    using first_5_types_2 = types::first_n_types_t<5, types_t>;

    stream << "First 0 types: " << first_0_types_2{} << endl;
    stream << "First 1 types: " << first_1_types_2{} << endl;
    stream << "First 2 types: " << first_2_types_2{} << endl;
    stream << "First 3 types: " << first_3_types_2{} << endl;
    stream << "First 4 types: " << first_4_types_2{} << endl;
    stream << "First 5 types: " << first_5_types_2{} << endl;
}

///////////////////////////////////

void test_last_n_types()
{
    using types_t = types::type_list_t<short, int, float, double>;
    stream <<"Types: " << types_t{} << endl;

    using last_0_types_1 = types::last_n_types_t<0, short, int, float, double>;
    using last_1_types_1 = types::last_n_types_t<1, short, int, float, double>;
    using last_2_types_1 = types::last_n_types_t<2, short, int, float, double>;
    using last_3_types_1 = types::last_n_types_t<3, short, int, float, double>;
    using last_4_types_1 = types::last_n_types_t<4, short, int, float, double>;
    using last_5_types_1 = types::last_n_types_t<5, short, int, float, double>;

    stream << "last 0 types: " << last_0_types_1{} << endl;
    stream << "last 1 types: " << last_1_types_1{} << endl;
    stream << "last 2 types: " << last_2_types_1{} << endl;
    stream << "last 3 types: " << last_3_types_1{} << endl;
    stream << "last 4 types: " << last_4_types_1{} << endl;
    stream << "last 5 types: " << last_5_types_1{} << endl;

    stream << endl;

    using last_0_types_2 = types::last_n_types_t<0, types_t>;
    using last_1_types_2 = types::last_n_types_t<1, types_t>;
    using last_2_types_2 = types::last_n_types_t<2, types_t>;
    using last_3_types_2 = types::last_n_types_t<3, types_t>;
    using last_4_types_2 = types::last_n_types_t<4, types_t>;
    using last_5_types_2 = types::last_n_types_t<5, types_t>;

    stream << "last 0 types: " << last_0_types_2{} << endl;
    stream << "last 1 types: " << last_1_types_2{} << endl;
    stream << "last 2 types: " << last_2_types_2{} << endl;
    stream << "last 3 types: " << last_3_types_2{} << endl;
    stream << "last 4 types: " << last_4_types_2{} << endl;
    stream << "last 5 types: " << last_5_types_2{} << endl;
}

int main()
{
    // test_first_n_types();

    test_last_n_types();
}