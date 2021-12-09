#include <tpf_output.hpp>

namespace types = tpf::types;

tpf::sstream stream;
auto endl = tpf::endl;

template<typename... Types>
using type_list_t = types::type_list_t<Types...>;

template<size_t FirstN, typename LeftList, typename RightList> struct first_n_types_list_st;

template<>
struct first_n_types_list_st<0, type_list_t<>, type_list_t<>>
{
    using type = type_list_t<>;
};

template<typename RightType, typename... RightTypes>
struct first_n_types_list_st<0, type_list_t<>, type_list_t<RightType, RightTypes...>>
{
    using type = type_list_t<>;
};

template<typename... LeftTypes>
struct first_n_types_list_st<1, type_list_t<LeftTypes...>, type_list_t<>>
{
    using type = type_list_t<LeftTypes...>;
};

template<typename... LeftTypes, typename RightType, typename... RightTypes>
struct first_n_types_list_st<1, type_list_t<LeftTypes...>, type_list_t<RightType, RightTypes...>>
{
    using type = type_list_t<LeftTypes..., RightType>;
};

template<size_t FirstN, typename... LeftTypes, typename RightType, typename... RightTypes>
struct first_n_types_list_st<FirstN, type_list_t<LeftTypes...>, type_list_t<RightType, RightTypes...>>
{
    using type_0 = type_list_t<LeftTypes...>;

    using type_1 = std::conditional_t<FirstN==1, type_list_t<LeftTypes..., RightType>,
        typename first_n_types_list_st<FirstN-1, type_list_t<LeftTypes..., RightType>, type_list_t<RightTypes...>>::type>;

    using type = std::conditional_t<FirstN==0, type_0, type_1>; 
};

template<size_t FirstN, typename... Types>
using first_n_types_list_t = 
    typename first_n_types_list_st<FirstN, type_list_t<>, Types...>::type;

template<size_t FirstN, typename... Types>
struct first_n_types_st
{
    static_assert(FirstN <= sizeof...(Types), "FirstN out of range");
    using type = 
        typename first_n_types_list_st<FirstN, type_list_t<>, type_list_t<Types...>>::type;
};

template<size_t FirstN, typename... Types>
struct first_n_types_st<FirstN, type_list_t<Types...>>
{
    static_assert(FirstN <= sizeof...(Types), "FirstN out of range");

    using type = 
        typename first_n_types_list_st<FirstN, type_list_t<>, type_list_t<Types...>>::type;
};

template<size_t FirstN, typename... Types>
using first_n_types_t = typename first_n_types_st<FirstN, Types...>::type;

void test_first_n_types_list()
{
    using types_t = type_list_t<short, int, double>;
    // using types_t = type_list_t<>;

    stream <<"Types: " << types_t{} << endl;

    using t_0_0 = first_n_types_list_t<0, types_t>;
    stream << "t_0_0: " << t_0_0{} << endl;

    using t_0_1 = first_n_types_list_t<1, types_t>;
    stream << "t_0_1: " << t_0_1{} << endl;

    using t_0_2 = first_n_types_list_t<2, types_t>;
    stream << "t_0_2: " << t_0_2{} << endl;

    using t_0_3 = first_n_types_list_t<3, types_t>;
    stream << "t_0_3: " << t_0_3{} << endl;

    using t_0_4 = first_n_types_list_t<4, types_t>;
    stream << "t_0_4: " << t_0_4{} << endl;
}

void test_first_n_types_t()
{
    using types_t = type_list_t<short, int, double>;
    // using types_t = type_list_t<>;

    stream <<"Types: " << types_t{} << endl;

    using t_0_0 = first_n_types_t<0, types_t>;
    stream << "t_0_0: " << t_0_0{} << endl;

    using t_0_1 = first_n_types_t<1, types_t>;
    stream << "t_0_1: " << t_0_1{} << endl;

    using t_0_2 = first_n_types_t<2, types_t>;
    stream << "t_0_2: " << t_0_2{} << endl;

    using t_0_3 = first_n_types_t<3, types_t>;
    stream << "t_0_3: " << t_0_3{} << endl;

    // using t_0_4 = first_n_types_t<4, types_t>;
    // stream << "t_0_4: " << t_0_4{} << endl;

    stream << endl;
    
    using t_1_0 = first_n_types_t<0, short, int, double>;
    stream << "t_1_0: " << t_1_0{} << endl;

    using t_1_1 = first_n_types_t<1, short, int, double>;
    stream << "t_1_1: " << t_1_1{} << endl;

    using t_1_2 = first_n_types_t<2, short, int, double>;
    stream << "t_1_2: " << t_1_2{} << endl;

    using t_1_3 = first_n_types_t<3, short, int, double>;
    stream << "t_1_3: " << t_1_3{} << endl;

    // using t_1_4 = first_n_types_t<4, short, int, double>;
    // stream << "t_1_4: " << t_1_4{} << endl;
}

int main()
{
    // test_first_n_types_list();

    test_first_n_types_t();
}



