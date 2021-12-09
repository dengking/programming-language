#include <tpf_output.hpp>

// note that sstream is a std::ostringstream based stream class
// that means we have flush to print out the result,
// otherwise we cannot see anything.
tpf::sstream stream; 
auto nl = tpf::nl; // new line without flushing for performance reason, or to use the stream as string.
auto flush = tpf::flush; // flush out the stream to console without trailing newline.
auto endl = tpf::endl; // flush out the stream with a trailing new line.

template<typename Type>
Type increment(Type arg) // note arg is template parameter
{                        // but they are not template-parameter pack
    return arg + 1;
}

template<typename Type, typename... Types>
void example_for_template_parameter_pack_expansion(Type arg, Types... args) // args is a template parameter pack
{
    auto v1 = std::vector<Type>{ arg, args... };
    auto v2 = std::vector<Type>{ increment(arg), increment(args)... };
    auto v3 = std::vector<Type>{ static_cast<Type>(arg), static_cast<Type>(args)... };
    auto v4 = std::vector<Type>{ increment(static_cast<Type>(arg)),  increment( static_cast<Type>(args) )... };

    stream <<"v1 = " << v1 << nl;
    stream <<"v2 = " << v2 << nl;
    stream <<"v3 = " << v3 << nl;
    stream <<"v4 = " << v4 << nl << flush;
}

/*
    GNU C++/clang++ strictly follows C++ Standard Specifications.
    In case of Microsoft compiler, it has more relaxed syntax.

    In a net shell, try to follow Standard Specification for maximum compability
    as well as correct behavior of your compiler.
*/

// this is now forward declaration of template function
template<typename Type>
void print_out_parameter_packs(Type arg);

// this function can take 1 or more
template<typename Type, typename... Types>
void print_out_parameter_packs(Type arg, Types... args)
{
    if(sizeof...(Types) == 0) // sizeof...(Types) == sizeof...(args)
    {
        print_out_parameter_packs(arg);
    }
    else
    {
        // the count of the parameter pack args is not zero
        stream << arg << ", ";

        // the count of args is not zero means it has 1 or more parameters
        print_out_parameter_packs(args...);
    }
}

////////////////////////////////

// Put more code in this section

/////////////////////////////////////

template<typename Type>
void print_out_parameter_packs(Type arg)
{
    stream << arg << endl;
}

/*
    To address this ugly syntax regarding to template parameter pack expansion,
    C++ Standard Committee introduced if constexpr construct as below
*/

template<typename Type, typename... Types>
void example_of_if_constexpr(Type arg, Types... args)
{
    // if constexpr() was introduced to C++17 Standard
    if constexpr(sizeof...(args) == 0) // if constexpr(const expression)
                                        // sadly, the term "const expression" is misnomer
                                        // it should be better called "compile-time expression"

    {
            // the parameter pack args... has zero elements
            stream << arg << endl;
    }
    else
    {
        // the count of the parameter pack args... is one or more
        stream << arg << ", ";
        example_of_if_constexpr(args...);
    }
}

template<typename Type>
Type square_me(Type arg)
{
    return arg * arg;
}

template<typename Type, typename... Types>
void example_of_if_constexpr_with_pack_expansion(Type arg, Types... args)
{
    // if constexpr() was introduced to C++17 Standard
    if constexpr(sizeof...(args) == 0)
    {
            // the parameter pack args... has zero elements
            stream << square_me(arg) << endl;
    }
    else
    {
        // the count of the parameter pack args... is one or more
        stream << square_me(arg) << ", ";
        example_of_if_constexpr_with_pack_expansion(args...);
    }
}

template<typename Type, typename... Types>
void example_of_pack_expansion(Type arg, Types... args)
{
    example_of_if_constexpr(square_me(arg), square_me(args)... );
}

/*
    Fold Expression was introduced to C++17 Standard.
    It can be sometimes very handy when working with parameter packs.
*/

template<typename Type, typename... Types>
Type divide_using_fold_exression(Type first, Types... args)
{
    // fold expression should be enclosed with parentheses as below.
    return ( first / ... / args );

    // ((first / next arg) / nnext arg) / nnnext arg ....
    // where arg is an element in args...
}

void example_fold_expression()
{
    auto rlt = divide_using_fold_exression(2*3*5, 2, 3);
        // ( (2*3*5 / 2) / 3 ) == 5

    stream <<"( (2*3*5 / 2) / 3 ) = " << rlt << endl;
}

template<typename Type, typename... Types>
Type divide_using_fold_exression_with_pack_expansion(Type first, Types... args)
{
    // fold expression should be enclosed with parentheses as below.
    return ( square_me(first) / ... / square_me(args) );

    // ((first / next arg) / nnext arg) / nnnext arg ....
    // where arg is an element in args...
}


void example_with_pack_expansion_using_fold_exression()
{
    auto rlt = divide_using_fold_exression_with_pack_expansion( 2*3*5, 2, 3);
        // ( ( (2*3*5)^2 / 2^2) / 3^2 ) == 5^2

    stream <<"( ( (2*3*5)^2 / 2^2) / 3^2 ) = " << rlt << endl;
}

template<typename Type, typename... Types>
void divide_fold_exression_with_pack_expansion(Type first, Types... args)
{
    stream << "Result: " 
        << divide_using_fold_exression(square_me(first), square_me(args)...)
        << endl;
}

/*
    Variadic Macro was first introduced to C++11 Standard.
*/

// do not put semicolon at the end of macro defintion
#define VeryUglyName_SquareFoldExpression(first, ...) divide_fold_exression_with_pack_expansion(first, __VA_ARGS__)

int main()
{
    // example_for_template_parameter_pack_expansion(0, 1, 2, 3, 4, 5);
    // print_out_parameter_packs(0, 1, 2, 3, 4, 5);

    // example_of_if_constexpr(0, 1, 2, 3, 4, 5);

    // example_of_if_constexpr_with_pack_expansion(0, 1, 2, 3, 4, 5);
    // example_of_pack_expansion(0, 1, 2, 3, 4, 5);

    // example_fold_expression();
    example_with_pack_expansion_using_fold_exression();
    divide_fold_exression_with_pack_expansion(2*3*5, 2, 3);
    VeryUglyName_SquareFoldExpression(2*3*5, 2, 3);
}