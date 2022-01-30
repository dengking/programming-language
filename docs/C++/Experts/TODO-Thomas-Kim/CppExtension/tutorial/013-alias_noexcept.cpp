#include <tpf_output.hpp>

/*
    If you want to pass an argument as
    template parameter, that argument should be
    compile-time expression, or constexpr
*/
template<auto FuncType, typename... ArgTypes>
constexpr auto is_noexcept_v = noexcept(FuncType(std::declval<ArgTypes>()...));

int sum(int a, int b) noexcept
{
    return a + b;
}

int summation(int a, int b)
{
    return a + b;
}

// this is function template
// function template instantiate template functions.
template<typename T>
T pro(T a, T b) noexcept
{
    return a * b;
}

void examples_for_is_noexcept()
{
    tpf::sstream stream;

    stream << "sum is marked with noexcept: "
        << is_noexcept_v<sum, int, int> << tpf::endl;

    stream << "summation is marked with noexcept: "
        << is_noexcept_v<summation, int, int> << tpf::endl;

    stream << "pro<int> is marked with noexcept: "
        << is_noexcept_v<pro<int>, int, int> << tpf::endl;

    // pro_int is a template function,
    // or an instance of the function template
    auto pro_int = pro<int>;

    // it does not compile at all.
    // stream << "pro_int is marked with noexcept: "
    //     << is_noexcept_v<pro_int, int, int> << tpf::endl;

    // constexpr means the expression is compile-time expression
    constexpr auto cpro_int = pro<int>;

    stream << "cpro_int is marked with noexcept: "
        << is_noexcept_v<cpro_int, int, int> << tpf::endl;

    /*
        pro is a function template that instantiate or generate
            template functions.

        pro<int> is not a function template,
            but a template function, or
                a specialization of the function template,
                an instance of the function template.

        So, a function template and a template function are totally
            different entities.
    */

}

int main()
{
    examples_for_is_noexcept();
}