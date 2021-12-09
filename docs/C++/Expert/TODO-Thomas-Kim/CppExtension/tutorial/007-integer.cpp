#include <tpf_output.hpp>
#include <tpf_safe_type.hpp>
#include <numeric>

tpf::sstream stream;
auto nl = tpf::nl;
auto endl = tpf::endl;

namespace types = tpf::types;
namespace safe = tpf::safe_type;

void display_integral_types()
{
    // this function displays integral type information
    safe::integral_type_info();

    // I removed bool type from the integral type list
    stream <<"Integral types are: " << 
        types::integral_list_t{} // this displays integral types
        << endl;

    stream << "Signed integral types are: "
        << types::signed_integral_list_t{} << endl;

    stream << "Unsigned integral types are: "
        << types::unsigned_integral_list_t{} << endl;

    stream <<"Integer types are: " << 
        types::integer_list_t{} // this displays integer types
        << endl;

    stream << "Signed integer types are: "
        << types::signed_integer_list_t{} << endl;

    stream << "Unsigned integer types are: "
        << types::unsigned_integer_list_t{} << endl;
}

template<typename Type> // template parameter clause
types::enable_if_signed_integral_t<Type> 
    // for SFINAE out, SFINAE: Substitution Failure Is Not an Error
    // Allowed Types: char, short, int, long, long long
safe_add(Type a, Type b)
{
    /*
        lower_bound <= a + b <= upper_bound ... should hold

        where lower_bound = std::numeric_limits<Type>::min(),
              upper_bound = std::numeric_limits<Type>::max()

        when a > 0 && b > 0, that is, both are positive

            a + b <= upper_bound should hold
            a <= upper_bound - b (1)

        when a < 0 && b < 0, that is, both are negative
        lower_bound <= a + b should hold

        lower_bound - b <= a (2)
    */

//    constexpr auto lower_bound = std::numeric_limits<Type>::min();
//    
constexpr auto upper_bound = std::numeric_limits<Type>::max();
    constexpr auto lower_bound = safe::limits_min<Type>;
    constexpr auto upper_bound = safe::limits_max<Type>;


   if( a > 0 && b > 0) // both are positive
   {
        if(a <= upper_bound - b)
            return a + b;
        else // overflow
        {
            tpf::sstream os;
            os << "overflow: a = " << a
                <<", b = " << b;

            Tpf_ThrowDebugException(os);
        }
    }
   else if ( a < 0 && b < 0) // both are negative
   {
        if(lower_bound - b <= a)
            return a + b;
        else // overflow
        {
            tpf::sstream os;
            os << "overflow: a = " << a
                <<", b = " << b;

            Tpf_ThrowDebugException(os);
        }
   }
   else // a and b have different sign, 
        // one is positive and the other negative
        return a + b;
}

template<typename Type> // template parameter clause
types::enable_if_signed_integral_t<Type> 
    // for SFINAE out, SFINAE: Substitution Failure Is Not an Error
    // Allowed Types: char, short, int, long, long long
safe_sub(Type a, Type b)
{
    return safe_add(a, -b);
}

template<typename Type> // template parameter clause
types::enable_if_unsigned_integral_t<Type> 
    // for SFINAE out, SFINAE: Substitution Failure Is Not an Error
    // Allowed Types: unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long
safe_add(Type a, Type b)
{
    /*
        0 <= a + b <= upper_bound

        a + b <= upper_bound 
        
        =>  a <= upper_bound - b should hold
    */

    // constexpr auto upper_bound = std::numeric_limits<Type>::max();
    constexpr auto upper_bound = safe::limits_max<Type>;


    if(a <= upper_bound - b)
        return a + b;
    else // overflow
    {
        tpf::sstream os;
        os << "overflow: a = " << a
            <<", b = " << b;

        Tpf_ThrowDebugException(os);
    }
}

template<typename Type> // template parameter clause
types::enable_if_unsigned_integral_t<Type> 
    // for SFINAE out, SFINAE: Substitution Failure Is Not an Error
    // Allowed Types: unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long
safe_sub(Type a, Type b)
{
    /*
        0 <= a - b should hold
        b <= a ... should hold
    */

    if(b <= a)
        return a - b;
    else // overflow
    {
        tpf::sstream os;
        os << "overflow: a = " << a
            <<", b = " << b;

        Tpf_ThrowDebugException(os);
    }
}

template<typename Type>
types::enable_if_integral_t<Type> // return type, '
                                  // in case of multiplication, we don't 
                                  // need to classify signed and unsigned separately
safe_mul(Type a, Type b)
{
    /*
        auto rlt = a * b;

        (1)
        if b == 0, we can return rlt == 0
        
        (2)
        if b != 0 and rlt / b == a

        (3)
        otherwise, overflow
    */
   auto rlt = a * b;

    if( (b == 0) || (rlt / b == a) ) // we are using short-circuit feature of C++
        return rlt; // in C++,  A || B, if A is true, B is not executed
                    // B is only executed when A is false,
                    // this property is called "short-circuit"
    else // overflow
    {
        tpf::sstream os;
        os << "overflow: a = " << a
            <<", b = " << b;

        Tpf_ThrowDebugException(os);
    }
}

template<typename Type>
types::enable_if_integral_t<Type> // return type, '
                                  // in case of multiplication, we don't 
                                  // need to classify signed and unsigned separately
safe_div(Type a, Type b)
{
    if(b != 0)
        return a / b;
    else // overflow
    {
        tpf::sstream os;
        os << "division by zero: a = " << a
            <<", b = " << b;

        Tpf_ThrowDebugException(os);
    }
}

void examples_for_safe_arithmetic()
{
    try
    {
        // constexpr auto int_lower_bound = std::numeric_limits<int>::min();
        // constexpr auto int_upper_bound = std::numeric_limits<int>::max();

        // constexpr auto unsigned_lower_bound = std::numeric_limits<unsigned>::min();
        // constexpr auto unsigned_upper_bound = std::numeric_limits<unsigned>::max();

        constexpr auto int_lower_bound = safe::limits_min<int>;
        constexpr auto int_upper_bound = safe::limits_max<int>;

        constexpr auto unsigned_lower_bound = safe::limits_min<unsigned>;
        constexpr auto unsigned_upper_bound = safe::limits_max<unsigned>;


        auto signed_a = 3;
        auto signed_b = 2;

        auto signed_add = safe_add(signed_a, signed_b);
        stream <<signed_a << " + "<< signed_b << " = " << signed_add << endl;

        auto signed_zero = 1;

        auto signed_div = safe_div(signed_a, signed_zero);
        stream <<signed_a << " / "<< signed_zero << " = " << signed_div << endl;

        // auto signed_c = int_upper_bound;
        auto signed_c = 5;
        auto signed_d = 2;

        auto signed_mul = safe_mul(signed_c, signed_d);
        stream <<signed_c << " * "<< signed_d << " = " << signed_mul << endl;
    
    } catch(std::exception& e)
    {
        stream << e << endl;
    }

} 

int main()
{
    // display_integral_types();
    examples_for_safe_arithmetic();
}