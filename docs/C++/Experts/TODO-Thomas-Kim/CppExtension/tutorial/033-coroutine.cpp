#include "033-coroutine.hpp"

//ResumableType<int> 
resumable_type<int> addition(int a, int b)
{
    // co_return expression
    // calls PromiseType::return_value(expression)
    co_return a + b;
}

void test_co_return()
{
    auto add = addition(2, 3);

    add.resume();

    std::cout << add.get() << std::endl;
}

// ResumableType<int> 
resumable_type<int> summation(int max)
{
    int s = 0;
    for(int i=1; i < max; ++i)
    {
        s += i;

        co_yield s;
    }

    co_return s;
}

void test_co_yield()
{
    auto co_sum = summation(11);

    while(co_sum.resume())
    {
        std::cout << co_sum.get() << std::endl;
    }
}

// ResumableType<int> 
resumable_type<int> factorial(int max)
{
    int f = 1;

    for(int i=1; i < max; ++i)
    {
        f *= i;

        auto s = co_await f;

        std::cout << s << std::endl;
    }

    std::cout << f << std::endl;
    
    co_return f;
}

void test_co_await()
{
    auto f = factorial(11);

    while(f.resume());
}

// ResumableType<void>
resumable_type<>
    no_return_value_coroutine(int max)
{
    for(int i=0; i < max; ++i)
    {
        co_await i;

        std::cout << i << std::endl;   
    }
    
    co_return;
}

void test_no_return_value_coroutine()
{
    auto no_return = no_return_value_coroutine(11);

    while(no_return.resume());
}

int main()
{
    test_co_return();
    
    test_co_yield();

    test_co_await();

    test_no_return_value_coroutine();
}