#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto endl = tpf::endl;

void test_type_functions()
{
    using types_t = types::type_list_t<int, short, double>;

    using t0 = types::pop_back_type_t<types_t>;
    using t1 = types::pop_front_type_t<types_t>;

    stream << "t0 = " << t0{} << endl;
    stream << "t1 = " << t1{} << endl;

    using t2 = types::pop_back_type_t<int, short, double>;
    using t3 = types::pop_front_type_t<int, short, double>;

    stream << "t2 = " << t2{} << endl;
    stream << "t3 = " << t3{} << endl;
}

void test_push_function()
{
    using t0 = types::push_front_type_t<int, short, double>;
    using t1 = types::push_back_type_t<int, short, double>;

    stream << "t0 = " << t0{} << endl;
    stream << "t1 = " << t1{} << endl;

    using types_t = types::type_list_t<short, double>;

    using t2 = types::push_front_type_t<int, types_t>;
    using t3 = types::push_back_type_t<int, types_t>;

    stream << "t2 = " << t0{} << endl;
    stream << "t3 = " << t1{} << endl;
}

void test_remove_type_function()
{
    using t0 = types::remove_type_t<int, short, double, int>;
    using t1 = types::remove_type_t<short, short, double, int>;

    stream << "t0 = " << t0{} << endl;
    stream << "t1 = " << t1{} << endl;

    using types_t = types::type_list_t<short, double, int>;

    using t2 = types::remove_type_t<short, types_t>;
    using t3 = types::remove_type_t<short, types_t>;

    stream << "t2 = " << t2{} << endl;
    stream << "t3 = " << t3{} << endl;
}

int main()
{
    // test_type_functions();

    // test_push_function();

    test_remove_type_function();
}