#include <tpf_output.hpp>

tpf::sstream stream;
auto nl = tpf::nl;
auto endl = tpf::endl;

template<typename Type1, typename Type2, typename... Types>
auto Divide(Type1 dividend, Type2 divisor, Types... args)
{
    auto divisors = 
        std::vector<Type1>{ static_cast<Type1>(args)... };

    dividend /= divisor;

    for(auto& d : divisors)
        dividend /= d;

    return dividend;
}

void test_divide()
{
    stream << "divide 2*3*5 with 3, 5 = " 
            << Divide(2*3*5, 3, 5) << endl;
}

template<typename Type1, typename... Types>
auto divide_fold(Type1 dividend, Types... args)
{
    return (dividend / ... / args);
}

void test_divide_fold()
{
    stream << "divide 2*3*5 with 3, 5 = " 
            << divide_fold(2*3*5, 3, 5) << endl;
}

int main()
{
    // test_divide();
    test_divide_fold();
}