#include <tpf_output.hpp>
#include <future>
#include <thread>


int global_sum(int a, int b)
{
    return a + b;
}

class SquareSum
{
    public:
    int square_sum(int a, int b)
    {
        return a * a + b * b;
    }

    static int sum_square(int a, int b)
    {
        return (a+b)*(a+b);
    }

    // do not forget trailing const specifier
    // in some case, if you do not use const specifier 
    // your compiler may complain or refuse compiling at all
    int operator()(int n) const
    {
        return n*n + n*n;
    }
};


void example_for_thread_functions()
{
    try
    {
        /*
            Always put your multithread code in 
            try - catch  construct
        */

        // using global function as thread function
        auto global_thread_func = std::async(global_sum, 2, 3);
        
        SquareSum square_sum_object;

        // using non-static member function as thread function
        auto member_square_sum_fun = std::async( &SquareSum::square_sum, // member function's function pointer - relative address
                &square_sum_object, // the object that contains member function square_sum
                3, 4 // arguments for SquareSum::square_sum(a, b)
        );

        // using static member function as thread function
        auto static_member_function = std::async(&SquareSum::sum_square, 2, 3);

        // using functor as a thread function
        // "by functor" I mean a class or struct
        // that defined operator()()
        auto functor_thread_func = std::async(SquareSum{}, 5);

        // using lambda as a thread function
        auto lambda_thread_func = std::async([](int a, int b, int c)
            { return a*a + b*b + c*c; }, 1, 2, 3);

        tpf::sstream stream;

        stream << "global_thread_func - sum of 2, 3: " << global_thread_func.get() << tpf::endl;
        stream << "member_square_sum_fun - square_sum of 3, 4: " << member_square_sum_fun.get() << tpf::endl;
        stream << "static_member_function - sum_square of 3, 4: " << static_member_function.get() << tpf::endl;
        stream << "functor_thread_func - square sum of 5: " << functor_thread_func.get() << tpf::endl;
        stream << "lambda_thread_func - sum of squares if 1, 2, 3 :" << lambda_thread_func.get() <<  tpf::endl;
    }
    catch(const std::exception& e)
    {
        tpf::sstream stream;

        stream << e << tpf::endl;
    }

}

void example_for_thread_functions_async()
{
    try
    {
        /*
            Always put your multithread code in 
            try - catch  construct
        */

        // using global function as thread function
        auto global_thread_func = std::async(std::launch::async, global_sum, 2, 3);
        
        SquareSum square_sum_object;

        // using non-static member function as thread function
        auto member_square_sum_fun = std::async(std::launch::async,  &SquareSum::square_sum, // member function's function pointer - relative address
                &square_sum_object, // the object that contains member function square_sum
                3, 4 // arguments for SquareSum::square_sum(a, b)
        );

        // using static member function as thread function
        auto static_member_function = std::async(std::launch::async, &SquareSum::sum_square, 2, 3);

        // using functor as a thread function
        // "by functor" I mean a class or struct
        // that defined operator()()
        auto functor_thread_func = std::async(std::launch::async, SquareSum{}, 5);

        // using lambda as a thread function
        auto lambda_thread_func = std::async(std::launch::async, [](int a, int b, int c)
            { return a*a + b*b + c*c; }, 1, 2, 3);

        tpf::sstream stream;

        stream << "global_thread_func - sum of 2, 3: " << global_thread_func.get() << tpf::endl;
        stream << "member_square_sum_fun - square_sum of 3, 4: " << member_square_sum_fun.get() << tpf::endl;
        stream << "static_member_function - sum_square of 3, 4: " << static_member_function.get() << tpf::endl;
        stream << "functor_thread_func - square sum of 5: " << functor_thread_func.get() << tpf::endl;
        stream << "lambda_thread_func - sum of squares if 1, 2, 3 :" << lambda_thread_func.get() <<  tpf::endl;
    }
    catch(const std::exception& e)
    {
        tpf::sstream stream;

        stream << e << tpf::endl;
    }

}


int main()
{
   // example_for_thread_functions();
    example_for_thread_functions_async();
}