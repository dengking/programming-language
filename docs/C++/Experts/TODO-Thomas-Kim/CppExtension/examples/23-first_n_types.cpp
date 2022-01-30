#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto endl = tpf::endl;

void test_first_n_types_t()
{
    using types_t = types::type_list_t<int, short, double>;

    using t0 = types::first_n_types_t<0, int, short, double>;
    using t1 = types::first_n_types_t<1, int, short, double>;
    using t2 = types::first_n_types_t<2, int, short, double>;
    using t3 = types::first_n_types_t<3, int, short, double>;
    using t4 = types::first_n_types_t<4, int, short, double>;

    stream <<"t0 = " << Tpf_GetTypeName(t0) << endl;
    stream <<"t1 = " << Tpf_GetTypeName(t1) << endl;
    stream <<"t2 = " << Tpf_GetTypeName(t2) << endl;
    stream <<"t3 = " << Tpf_GetTypeName(t3) << endl;
    stream <<"t4 = " << Tpf_GetTypeName(t4) << endl;
}

void test_first_n_types_t2()
{
    using types_t = types::type_list_t<int, short, double>;

    using t0 = types::first_n_types_t<0, types_t>;
    using t1 = types::first_n_types_t<1, types_t>;
    using t2 = types::first_n_types_t<2, types_t>;
    using t3 = types::first_n_types_t<3, types_t>;
    using t4 = types::first_n_types_t<4, types_t>;

    stream <<"t0 = " << Tpf_GetTypeName(t0) << endl;
    stream <<"t1 = " << Tpf_GetTypeName(t1) << endl;
    stream <<"t2 = " << Tpf_GetTypeName(t2) << endl;
    stream <<"t3 = " << Tpf_GetTypeName(t3) << endl;
    stream <<"t4 = " << Tpf_GetTypeName(t4) << endl;
}

void test_last_n_types_1()
{
    using t0 = types::last_n_types_t<0, short, int, double>;
    stream << "t0 = " << t0{} << endl;

    using t1 = types::last_n_types_t<1, short, int, double>;
    stream << "t1 = " << t1{} << endl;

    using t2 = types::last_n_types_t<2, short, int, double>;
    stream << "t2 = " << t2{} << endl;

    using t3 = types::last_n_types_t<3, short, int, double>;
    stream << "t3 = " << t3{} << endl;

    using t4 = types::last_n_types_t<4, short, int, double>;
    stream << "t4 = " << t4{} << endl;

}

void test_last_n_types_2()
{
    using types_t =  types::type_list_t<short, int, double>;

    using t0 = types::last_n_types_t<0, types_t>;
    stream << "t0 = " << t0{} << endl;

    using t1 = types::last_n_types_t<1, types_t>;
    stream << "t1 = " << t1{} << endl;

    using t2 = types::last_n_types_t<2, types_t>;
    stream << "t2 = " << t2{} << endl;

    using t3 = types::last_n_types_t<3, types_t>;
    stream << "t3 = " << t3{} << endl;

    using t4 = types::last_n_types_t<4, types_t>;
    stream << "t4 = " << t4{} << endl;
}


void test_is_class_template()
{
    using type_t = types::types_to_template_class_t<std::vector<double>, int>;

    stream << Tpf_GetTypeName(type_t) << endl;
}

void test_nth_type_to_template_class()
{
    using tuple_t = std::tuple<>;
    
    using t0 = types::nth_type_to_template_class_t<0, tuple_t, int, short, double>;
    stream <<"t0 = " << Tpf_GetTypeName(t0) << endl;

    using t1 = types::nth_type_to_template_class_t<1, tuple_t, int, short, double>;
    stream <<"t1 = " << Tpf_GetTypeName(t1) << endl;

    using types_t = types::type_list_t<int, short, double>;

    using t2 = types::nth_type_to_template_class_t<2, tuple_t, types_t>;
    stream <<"t2 = " << Tpf_GetTypeName(t2) << endl;

    //using t3 = types::nth_type_to_template_class_t<3, tuple_t, types_t>;
    //stream <<"t3 = " << Tpf_GetTypeName(t3) << endl;   
}

void test_first_n_types_to_template_class()
{
    using tuple_t = std::tuple<>;
    
    using t0 = types::first_n_types_to_template_class_t<0, tuple_t, int, short, double>;
    stream <<"t0 = " << Tpf_GetTypeName(t0) << endl;

    using t1 = types::first_n_types_to_template_class_t<1, tuple_t, int, short, double>;
    stream <<"t1 = " << Tpf_GetTypeName(t1) << endl;

    using types_t = types::type_list_t<int, short, double>;

    using t2 = types::first_n_types_to_template_class_t<2, tuple_t, types_t>;
    stream <<"t2 = " << Tpf_GetTypeName(t2) << endl;

    using t3 = types::first_n_types_to_template_class_t<3, tuple_t, types_t>;
    stream <<"t3 = " << Tpf_GetTypeName(t3) << endl;

    // using t4 = types::first_n_types_to_template_class_t<4, tuple_t, types_t>;
    // stream <<"t4 = " << Tpf_GetTypeName(t4) << endl;


    //using t3 = types::nth_type_to_template_class_t<3, tuple_t, types_t>;
    //stream <<"t3 = " << Tpf_GetTypeName(t3) << endl;   
}

int main()
{
    // test_first_n_types_t();

    // test_first_n_types_t2();

    // test_last_n_types_1();

    // test_last_n_types_2();

    // test_is_class_template();
    
    // test_nth_type_to_template_class();

    test_first_n_types_to_template_class();
}
