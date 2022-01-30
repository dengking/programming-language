#include <tpf_output.hpp>

namespace types = tpf::types;

template<typename... Types>
using type_list_t = types::type_list_t<Types...>;

tpf::sstream stream;
auto endl = tpf::endl;

template<size_t LastN, typename LeftList, typename RightList> struct last_n_types_list_st;

template<>
struct last_n_types_list_st<0, type_list_t<>, type_list_t<>>
{
    using type = type_list_t<>;
};

template<typename LeftType, typename... LeftTypes>
struct last_n_types_list_st<0, type_list_t<LeftType, LeftTypes...>, type_list_t<>>
{
    using type = type_list_t<>;
};

template<typename... RightTypes>
struct last_n_types_list_st<1, type_list_t<>, type_list_t<RightTypes...>>
{
    using type = type_list_t<RightTypes...>;
};

template<typename LeftType, typename... LeftTypes, typename... RightTypes>
struct last_n_types_list_st<1, type_list_t<LeftType, LeftTypes...>, type_list_t<RightTypes...>>
{
    using left_list = type_list_t<LeftType, LeftTypes...>;

    using type = type_list_t< types::select_last_type_t<left_list>, RightTypes...>;
};

template<size_t LastN, typename LeftType, typename... LeftTypes, typename... RightTypes>
struct last_n_types_list_st<LastN, type_list_t<LeftType, LeftTypes...>, type_list_t<RightTypes...>>
{
    using left_list = type_list_t<LeftType, LeftTypes...>;

    using new_left = types::pop_back_type_t<left_list>;
    using new_right = type_list_t<types::select_last_type_t<left_list>, RightTypes...>;

    using type_0 = type_list_t<RightTypes...>;
    using type_1 = std::conditional_t<LastN == 1, new_left,
        typename last_n_types_list_st<LastN-1, new_left, new_right>::type>;

    using type = std::conditional_t<LastN == 0, type_0, type_1>;
};

template<size_t LastN, typename... Types>
using last_n_types_list_t = typename last_n_types_list_st<LastN, Types..., type_list_t<>>::type;

template<size_t LastN, typename... Types> 
struct last_n_types_st
{
    static_assert(LastN <= sizeof...(Types), "LastN out of range");

    using type = typename last_n_types_list_st<LastN, type_list_t<Types...>, type_list_t<>>::type;
};

template<size_t LastN, typename... Types> 
struct last_n_types_st<LastN, type_list_t<Types...>>
{
    static_assert(LastN <= sizeof...(Types), "LastN out of range");
    
    using type = typename last_n_types_list_st<LastN, type_list_t<Types...>, type_list_t<>>::type;
};

template<size_t LastN, typename... Types> 
using last_n_types_t = typename last_n_types_st<LastN, Types...>::type;

template<size_t LastN, typename... Types> 
using select_last_n_types_t = last_n_types_t<LastN, Types...>;

void test_last_n_types_list_t()
{
    using types_t = type_list_t<short, int, double>;
    stream <<"Types: " << types_t{} << endl;

    using t_0_0 = last_n_types_list_t<0, types_t>;
    stream <<"t_0_0: " << t_0_0{} << endl;

    using t_0_1 = last_n_types_list_t<1, types_t>;
    stream <<"t_0_1: " << t_0_1{} << endl;

    using t_0_2 = last_n_types_list_t<2, types_t>;
    stream <<"t_0_2: " << t_0_2{} << endl;

    using t_0_3 = last_n_types_list_t<3, types_t>;
    stream <<"t_0_3: " << t_0_3{} << endl;

    using t_0_4 = last_n_types_list_t<4, types_t>;
    stream <<"t_0_4: " << t_0_4{} << endl;

}

void test_last_n_types_t()
{
    using types_t = type_list_t<short, int, double>;
    stream <<"Types: " << types_t{} << endl;

    using t_0_0 = last_n_types_t<0, types_t>;
    stream <<"t_0_0: " << t_0_0{} << endl;

    using t_0_1 = last_n_types_t<1, types_t>;
    stream <<"t_0_1: " << t_0_1{} << endl;

    using t_0_2 = last_n_types_t<2, types_t>;
    stream <<"t_0_2: " << t_0_2{} << endl;

    using t_0_3 = last_n_types_t<3, types_t>;
    stream <<"t_0_3: " << t_0_3{} << endl;

    // using t_0_4 = last_n_types_t<4, types_t>;
    // stream <<"t_0_4: " << t_0_4{} << endl;

    stream << endl;

    using t_1_0 = last_n_types_t<0, short, int, double>;
    stream <<"t_1_0: " << t_1_0{} << endl;

    using t_1_1 = last_n_types_t<1, short, int, double>;
    stream <<"t_1_1: " << t_1_1{} << endl;

    using t_1_2 = last_n_types_t<2, short, int, double>;
    stream <<"t_1_2: " << t_1_2{} << endl;

    using t_1_3 = last_n_types_t<3, short, int, double>;
    stream <<"t_1_3: " << t_1_3{} << endl;

    // using t_1_4 = last_n_types_t<4, short, int, double>;
    // stream <<"t_1_4: " << t_1_4{} << endl;
}

int main()
{
    // test_last_n_types_list_t();

    test_last_n_types_t();
}