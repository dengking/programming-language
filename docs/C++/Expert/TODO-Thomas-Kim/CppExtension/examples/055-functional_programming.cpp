#include <iostream>

/*
    In functional programming paradigm,

    1. The state of data is immutable.

    n C r = n-1 C r-1  +  n-1 C r
 */

template<size_t N, size_t R>
struct nCr
{
    constexpr static size_t value = nCr< N-1, R-1 >::value + nCr< N-1, R>::value;  
};

template<size_t N>
struct nCr<N, N>
{
    constexpr static size_t value = 1;
};

template<size_t N>
struct nCr<N, 0>
{
    constexpr static size_t value = 1;
};

template<size_t N>
struct nCr<N, 1>
{
    constexpr static size_t value = N;
};

template<size_t N, size_t R>
constexpr size_t combinations = nCr<N, R>::value;

template<size_t Start_R, size_t End_R>
struct static_loop
{
	template<size_t N>
    static void print_combinations()
    {
        if constexpr(Start_R <= End_R)
        {
            std::cout << N << " C " << Start_R << " = " 
				<< combinations<N, Start_R> << std::endl;
        }

        if constexpr(Start_R + 1 <= End_R)
        {
            static_loop<Start_R+1, End_R>:: template print_combinations<N>();
        }
    }
};

template<size_t N>
void test_combinations()
{
    static_loop<0, N>:: template print_combinations<N>();
}

int main()
{
    test_combinations<5>();
}