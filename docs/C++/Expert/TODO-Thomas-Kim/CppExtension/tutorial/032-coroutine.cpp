#include "032-coroutine.hpp"

// this is our first corountine
template<typename SemanticReturnType>
ResumableType<SemanticReturnType>
sum(SemanticReturnType a, SemanticReturnType b)
{
    // co_return expression
    // terminates the coroutine
    
    co_yield a * b;

    co_yield a + b;

    co_return (a + b)*( a - b);
}

void test_coroutine_basic()
{
    // at this point, our corountine co_sum
    // is initially suspended
    ResumableType<int> co_sum = sum(2, 3);

    co_sum.resume();

    std::cout <<"sum(2,3) = " 
        << co_sum.get() << std::endl;

    co_sum.resume();

    std::cout <<"sum(2,3) = " 
        << co_sum.get() << std::endl;

    co_sum.resume();

    std::cout <<"sum(2,3) = " 
        << co_sum.get() << std::endl;

}

template<typename SemanticReturnType>
ResumableType<SemanticReturnType>
summation(SemanticReturnType max)
{
    SemanticReturnType s = 0;

    for(SemanticReturnType i = 0;
        i < max; ++i)
    {
        s += i;
        co_yield s;   
    }

    co_return s;
}

void advanced_coroutine()
{
    auto cosum = summation(11);

    for(int i=0; i < 11; ++i)
    {
        std::cout << cosum.next() << std::endl;
    }
}

int main()
{
    // test_coroutine_basic();
    advanced_coroutine();
}