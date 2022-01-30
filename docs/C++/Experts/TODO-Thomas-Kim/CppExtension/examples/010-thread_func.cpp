#include <tpf_output.hpp>
#include <future>
#include <thread>

using return_t = int;

return_t sum(int a, int b)
{
    tpf::sstream stream;

    stream << __FUNCTION_NAME__ << tpf::endl;
    stream << "Thread ID: " << std::this_thread::get_id() << " " << tpf::endl;

    return a + b;
}

class MyClass
{

    public:

        return_t pro(int a, int b)
        {
            tpf::sstream stream;
    
            stream << __FUNCTION_NAME__ << tpf::endl;
            stream << "Thread ID: " << std::this_thread::get_id() << " " << tpf::endl;

            return a * b;
        }

        static return_t square(int a)
        {
            return a * a;
        }
};

void example_for_callable_thread_functions()
{
    std::future<return_t> sum_1_2 = 
        std::async( std::launch::async | std::launch::deferred, sum, 1, 2);

    MyClass class_object;

    std::future<return_t> pro_2_3 =
        std::async( std::launch::async | std::launch::deferred,
            &MyClass::pro, &class_object, 2, 3);

    tpf::sstream stream;

    stream << "(1+2) + (2*3) = " << sum(sum_1_2.get(), pro_2_3.get()) << tpf::endl;
}

class SquareSum
{

    public:
    int operator()(int a, int b) const
    {
        return a*a + b*b;
    }
};

void more_examples_for_thread_function()
{
    try
    {
        auto product = std::async([](auto a, auto b) { return a + b; }, // lambda taking two ints
             2, 3 // arguments for the lambda
             );

        auto sum_closure = [](auto a, auto b) { return a + b; };

        auto sum = std::async(sum_closure, 3, 4);

        auto square_sum = std::async(SquareSum{}, 3, 4);

        tpf::sstream  stream ;

        stream <<"product of 2 and 3 : " << product.get() << tpf::endl;
        stream <<"sum of 3 and 4: " << sum.get() << tpf::endl;
        stream <<"square_sum of 3 and 4: " << square_sum.get() << tpf::endl;
    }
    catch(const std::exception& e)
    {
        tpf::sstream stream;
        stream << e << tpf::endl;
    }
}
int main()
{
    // example_for_callable_thread_functions();
    more_examples_for_thread_function();
}

