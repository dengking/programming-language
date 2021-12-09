#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

template<typename Type1, typename Type2>
Type1 divide(Type1 dividend, Type2 divisor)
{
    if(divisor == 0)
    {
        tpf::sstream stream;

        stream << "Division by Zero, divisor is " << divisor
                << " is zero";

        Tpf_ThrowDebugException(stream.str());
    }
    else
    {
        return dividend / divisor;
    }
}

template<typename Type1, typename Type2, typename... Types>
Type1 divide(Type1 dividend, Type2 divisor, Types... divisors)
{
    if constexpr(sizeof...(divisors)==0)
        return divide(dividend, divisor);
    else
        return divide(divide(dividend, divisor), divisors...);
}

int main()
{
    try
    {
        auto rlt = divide(2*3*5, 3, 0);
        stream << "Result = " << rlt << endl;

    }
    catch(tpf::debug_exception& e)
    {
        stream << e.what() << endl;
    
    }
}