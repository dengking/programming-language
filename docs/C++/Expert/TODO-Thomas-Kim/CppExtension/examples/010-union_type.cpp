#include <tpf_types.hpp>
#include <tpf_output.hpp>

void examples_union_type_t()
{
    using list1 = tpf::types::type_list_t<int, float>;
    using list2 = tpf::types::type_list_t<short, double>;

    tpf::ostream stream;
    auto nl = "\n";

    stream << tpf::types::union_type_t<list1, list2>{} << nl;

    stream << tpf::types::union_type_t<char, list1>{} << nl;
    stream << tpf::types::union_type_t<int, list1>{} << nl;

}

void examples_intersection_type_t()
{
    using list1 = tpf::types::type_list_t<int, short, float>;
    using list2 = tpf::types::type_list_t<short, double, float>;

    tpf::ostream stream;
    auto nl = "\n";

    stream << tpf::types::intersection_type_t<list1, list2>{} << nl;
}

void examples_remove_type_t()
{
    using list1 = tpf::types::type_list_t<int, short, float>;
    using list2 = tpf::types::type_list_t<short, double, float>;

    tpf::ostream stream;
    auto nl = "\n";

    stream << tpf::types::remove_type_t<int, list1>{} << nl;
    
    stream << tpf::types::remove_type_t<short, list1>{} << nl;
}

void examples_difference_type_t()
{
    using list1 = tpf::types::type_list_t<int, short, float>;
    using list2 = tpf::types::type_list_t<short, double, float>;

    tpf::ostream stream;
    auto nl = "\n";

    stream << tpf::types::difference_type_t<list1, list2>{} << nl;
    stream << tpf::types::difference_type_t<list2, list1>{} << nl;
}

void examples_type_list_equivalent_type_t()
{
    using list1 = tpf::types::type_list_t<int, short, float>;
    using list2 = tpf::types::type_list_t<float, int, short>;

    tpf::ostream stream;
    auto nl = "\n";
    stream.std() << std::boolalpha;

    stream << tpf::types::is_type_list_equivalent_v<list1, list2> << nl;
    stream << tpf::types::is_type_list_equivalent_v<list2, list1> << nl;
}

int main()
{
    // examples_union_type_t();
    //  examples_intersection_type_t();
    // examples_remove_type_t();
    // examples_difference_type_t();
    examples_type_list_equivalent_type_t();
}