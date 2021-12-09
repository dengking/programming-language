#include <tpf_output.hpp>

struct dummy_type{};

template<auto FuncType, typename... ArgTypes>
constexpr bool is_noexcept_v = noexcept(FuncType(std::declval<ArgTypes>()...));

int sum(int a, int b) noexcept
{
    return a + b;
}

template<typename T>
T pro(T a, T b) noexcept
{
    return a * b;
}

template<auto FuncPtr, typename... ArgTypes>
constexpr auto func_ptr_t = FuncPtr<ArgTypes...>;

void example_is_noexcept()
{
    tpf::sstream stream;

    stream << is_noexcept_v<sum, int, int> << tpf::endl;
    
    constexpr auto pro_func = pro<int>;

    stream << pro_func(1, 2) << tpf::endl;

    stream << is_noexcept_v<pro_func, int, int> << tpf::endl;
    
}

int main()
{
    example_is_noexcept();
}