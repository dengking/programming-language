#include <tpf_output.hpp>
#include <tpf_safe_type.hpp>
#include <tpf_chrono_random.hpp>

#include <execution> // for parallel algorithm

/*
    1. When we use Parallel Algorithm, we should NEVER leak exceptions
    from the callback function used in the Parallel Algorithm.

    2. Any callbacks used for the Parallel Algorithm, we always have to code
    Multithread-Safe way. That is, we should prevent data race.

    3. We should narrow down the locked region as small as possible, which
    not only improve the performance of our program, but it also reduce
    the potential of deadlock.
*/

tpf::sstream stream;
auto endl = tpf::endl;

void problem_with_parallel_algorithm()
{
    size_t count = 10;
    auto generator = tpf::chrono_random::random_generator<int>(0, (int)count);

    std::vector<int> v; v.reserve(count);

    // we initialize vector v with count of elements of type int
    // with random numbers
    for(size_t i=0; i < count; ++i)
        v.emplace_back(generator());
    
    stream << "Before: " << v << endl;

    // we captured generator as reference, and it is shared
    // among multiple threads, so we need to prevent
    // data race using mutex and lock like this.

    std::mutex mutex;

    std::for_each(std::execution::par_unseq, v.begin(), v.end(),
        [&generator, &mutex](auto& e)
        { 
            // we divide each element of v
            // with random integer

            std::lock_guard<std::mutex> lock(mutex);
            e /= generator();
        } );

    // In C++ Parallel Algorithm, if an exception leaks,
    // our program chrashes. We have to preven
    // exception leaks from our parallel algorithm by all means.
    stream << "After: " << v << endl;
}

void safer_way_of_parallel_algorithm()
{
    size_t count = 10;
    auto generator = tpf::chrono_random::random_generator<int>(0, (int)count);

    std::vector<int> v; v.reserve(count);

    // we initialize vector v with count of elements of type int
    // with random numbers
    for(size_t i=0; i < count; ++i)
        v.emplace_back(generator());
    
    stream << "Before: " << v << endl;

    // we have to treat the callback in the Parallel Algorithm
    // as multithreaded function.

    std::atomic<bool> operation_success{true};
    std::mutex mutex;

    std::for_each(std::execution::par_unseq, v.begin(), v.end(),
        [&generator, &operation_success, &mutex](auto& e)
        { 
            if(operation_success)
            {
                // we divide each element of v
                // with random integer
                std::unique_lock<std::mutex> lock(mutex);
                auto d = generator();
                lock.unlock();

                if(d != 0)
                    e /= d;
                else
                    operation_success = false;
            }
        } );

    if(operation_success)
        stream << "After: " << v << endl;
    else
        stream << "Parallel Algorithm Failed" << endl;
}

void safer_way_of_parallel_algorithm_using_exception()
{
    size_t count = 10;
    auto generator = tpf::chrono_random::random_generator<int>(0, (int)count);

    std::vector<int> v; v.reserve(count);

    // we initialize vector v with count of elements of type int
    // with random numbers
    for(size_t i=0; i < count; ++i)
        v.emplace_back(generator());
    
    stream << "Before: " << v << endl;

    // we have to treat the callback in the Parallel Algorithm
    // as multithreaded function.

    std::mutex mutex;
    std::exception_ptr exception_ptr{nullptr};

    // because exception_ptr and generator are
    // shared among multiple threads,
    // we have to prevent data race over these 
    // shared resource among multiple threads
    std::for_each(std::execution::par_unseq, v.begin(), v.end(),
        [&generator, &exception_ptr, &mutex](auto& e)
        {
            try
            {
                std::unique_lock<std::mutex> lock(mutex);
                auto d = generator();

                if(exception_ptr==nullptr)
                {
                    lock.unlock();
                    
                    e = tpf::safe_type::safe_div(e, d);  
                } 
            }
            catch(...)
            {
                std::lock_guard<std::mutex> lock(mutex);
                exception_ptr = std::current_exception();
            }
        } );

    if(exception_ptr==nullptr)
        stream << "After: " << v << endl;
    else
    {
        std::rethrow_exception(exception_ptr);
    }
}

int main()
{
    // problem_with_parallel_algorithm();
    // safer_way_of_parallel_algorithm();

    try
    {
        safer_way_of_parallel_algorithm_using_exception();
    }
    catch(const std::exception& e)
    {
        stream << "\n" << e << endl;
    }

    stream << "Now, our parallel algorithm works even if exception is thrown" << endl;
}