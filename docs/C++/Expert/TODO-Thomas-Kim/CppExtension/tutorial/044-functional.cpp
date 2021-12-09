#include <iostream>

auto& cout = std::cout;
auto endl = "\n";
auto endL = "\n\n";

/*

    For your convenience, this is Pascal's Triangle formula

    n C r = n-1 C r-1 + n-1 C r
    
 */

// N and R are constant expression or compile-time expressions
// we cannot modify the values of N and R, as in functional programming paradigm
// once the values of N and R are initialized, we cannot change their states.
//
// Functional Programming is based on Recursion.

// this is primary template.
// primary template determines the number of template parameters.
template<size_t N, size_t R>
struct pascal_triangle
{
    // in functional programming paradigm, once we initialize an object
    // we cannot modify its state. So, keyword constexpr makes value immutable.
    constexpr static size_t value = pascal_triangle< N-1, R-1 >::value // n-1 C r-1
                                     +                                 // + 
                                     pascal_triangle< N-1, R >::value;    // n-1 C r
};

template<size_t N>
struct pascal_triangle<N, N> // nCn = 1
{
    constexpr static size_t value = 1;
};

template<size_t N>
struct pascal_triangle<N, 0> // nC0 = 1
{
    constexpr static size_t value = 1;
};

template<size_t N>
struct pascal_triangle<N, 1> // nC1 = N
{
    constexpr static size_t value = N;
};

template<size_t N, size_t R>
constexpr size_t pascal = pascal_triangle<N, R>::value;

void test_pascal_triangle()
{
    cout << "5 C 0 = " << pascal<5, 0> << endl;
    cout << "5 C 1 = " << pascal<5, 1> << endl;
    cout << "5 C 2 = " << pascal<5, 2> << endl;
    cout << "5 C 3 = " << pascal<5, 3> << endl;
    cout << "5 C 4 = " << pascal<5, 4> << endl;
    cout << "5 C 5 = " << pascal<5, 5> << endl;
}

template<size_t Start_R, size_t End_R>
struct static_loop
{
    template<size_t N>
    static void print_pascal_triangle()
    {
        if constexpr(Start_R <= End_R)
        {
            cout << N << " C " << Start_R << " = "
                << pascal<N, Start_R> << endl;
        }

        if constexpr(Start_R + 1 <= End_R)
        {
            // we are calling print_pascal_triangle<N>()
            // recursively
            static_loop<Start_R+1, End_R>:: 
                template print_pascal_triangle<N>();
        }
    }

};

template<size_t N>
void print_pascals()
{
    static_loop<0, N>::
        template print_pascal_triangle<N>();
}

/*
    Fibonacci numbers are defined as below

    f0 = 0
    f1 = 1;

    if n > 1,

    f(n) = f(n-2) + f(n-1)

    pascal's tree
    C(n, r) = C(n-1, r-1) + C(n-1, r)
 */
int main()
{
    // test_pascal_triangle();

    cout << "Combinations with 4 elements " << endL;

    print_pascals<4>();

    cout << "Combinations with 5 elements " << endL;

    print_pascals<5>();

    cout << "Combinations with 6 elements " << endL;

    print_pascals<6>();

    cout << "Combinations with 7 elements " << endL;

    print_pascals<7>();

    cout << "Combinations with 8 elements " << endL;

    print_pascals<8>();

}

