#include <tpf_output.hpp>
#include <tpf_safe_type.hpp>
#include <cmath>

tpf::sstream stream;
auto endl = tpf::endl;

void examine_integer_float_difference()
{
    /*
        In case of integral types such as char, int, short, long, etc.
        Division By Zero crashes your application.
    */

   int a = 5;
   int b = 1;

   auto r1 = a / b; // it causes your application to crash.
   stream <<"Wow, not crashed! " << endl;

   /*
        In case of floating-point types such as float, double or long double,
        Division-By-Zero is VALID (INFINITY) or INVALID (NaN).
   */

  double d1 = 5.0;
  double d2 = 0.0;

  auto r2 = d1 / d2; // is it valid operation, yes!!!!!

  stream << "Wow, floating-point division by zero!! r2 = " << r2 << endl;

    if(r2 == INFINITY)
    {
        stream << "Result is INFINITY" << endl;
    }
    else
    {
        stream << "Result is NOT INFINITY" << endl;
    }

    double d3 = 0.0;
    double d4 = 0.0;

    auto r3 = d3 / d4; // 0/0

    /*
        In case of floating-point operation,
        your application does not crash. It works NORMALLY.
    */
    if(r3 != r3) //  NaN != NaN, operation failed
    {
        stream << "Indeterminate State - INVALID floating point operation" << endl;
    }
    else
    {
        stream << "Determinate State - VALID floating-point operation" << endl;
    }

    stream << "The value of r3 = " << r3 << endl;

}

namespace types = tpf::types;
namespace safe = tpf::safe_type;

template<typename Type>
types::enable_if_real_number_t<Type>
safe_add(Type a, Type b)
{
    auto rlt = a + b;
    if(rlt != rlt)
    {
        tpf::sstream os;

        os << "floating-point operation failed - a = "
            << a <<", b = " << b;

        Tpf_ThrowDebugException(os);
    }

    return rlt;
}

template<typename Type>
types::enable_if_real_number_t<Type>
safe_sub(Type a, Type b)
{
    auto rlt = a - b;
    if(rlt != rlt)
    {
        tpf::sstream os;

        os << "floating-point operation failed - a = "
            << a <<", b = " << b;

        Tpf_ThrowDebugException(os);
    }

    return rlt;
}

template<typename Type>
types::enable_if_real_number_t<Type>
safe_mul(Type a, Type b)
{
    auto rlt = a * b;
    if(rlt != rlt)
    {
        tpf::sstream os;

        os << "floating-point operation failed - a = "
            << a <<", b = " << b;

        Tpf_ThrowDebugException(os);
    }

    return rlt;
}

template<typename Type>
types::enable_if_real_number_t<Type>
safe_div(Type a, Type b)
{
    auto rlt = a / b;
    if(rlt != rlt)
    {
        tpf::sstream os;

        os << "floating-point operation failed - a = "
            << a <<", b = " << b;

        Tpf_ThrowDebugException(os);
    }

    return rlt;
}

void examples_for_safe_floating_point_arithmetic()
{
    try
    {
        double d1 = INFINITY;
        double d2 = INFINITY;
        double d3 = -INFINITY;

        auto r1 = safe_add(d1, d2);  // infinity + infinity = infinity
        stream << d1 << " + " << d2 <<" = " << r1 << endl;

        auto r2 = safe_sub(d1, d3);  // infinity - (-infinity) = infinity
        stream << d1 << " - " << d3 <<" = " << r2 << endl;

        double d4 = 1.0;
        double d5 = 0.0;

        auto r3 = safe_div(d4, d5);  // 1.0 / 0.0 = INFINITY
        stream << d4 << " / " << d5 <<" = " << r3 << endl;
    }
    catch(std::exception& e)
    {
        stream << e << endl;
    }


}
int main()
{
    // examine_integer_float_difference();
    examples_for_safe_floating_point_arithmetic();
}