#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>

#include <sstream>
#include <set>
#include <string>
#include <mutex>
#include <thread>
#include <execution>

tpf::sstream stream;
auto endl = tpf::endl;

void test_threads_in_parallel_algorithm()
{
    size_t count = 10;
    using element_t = double;
    std::vector<element_t> v(count);

    // this is a random number generator
    // that generates elements for vector v, of type element_t or int,
    // ranging from 0 up to count, inclusive.
    auto generator = tpf::chrono_random::random_generator<element_t>(0, count);

    // we generate random numbers and initialize container v
    for(size_t i = 0; i < count; ++i)
        v[i] = generator();

    stream << "Before: " << v << endl;

    std::mutex mutex;
    using lock_type = std::lock_guard<std::mutex>;

    std::set<std::string> threads_set;

    std::for_each(std::execution::par_unseq, v.begin(), v.end(), 
        [lambda_init = int{}, &mutex, &threads_set](auto&& e)
        { 
            std::stringstream os; // os is a local variable
                                 // we do not need synchronize local variables

            int local_int;

            os << "Thread Id = " << std::this_thread::get_id() 
                << "\t &lambda_init: " << &lambda_init;

            // we should always narrow down the locked scope
            // to prevent dead-lock as well as to improve the performance
            {
                // std::lock_guard<std::mutex> lock(mutex)
                lock_type lock(mutex);

                threads_set.insert( os.str() );

                // std::cout is a global console output stream
                // this cout is shared among multiple threads
                // so, it should be enclosed in the locked scope
                std::cout << os.str() << std::endl;
            }
        } );

    // this is out of parallel algorithm
    stream << "\n Number of threads used: " << threads_set.size() << endl;

    stream << "Threads Info: " << threads_set << endl;

}

void test_threads_in_parallel_algorithm_for_locals()
{
    size_t count = 10;
    using element_t = double;
    std::vector<element_t> v(count);

    // this is a random number generator
    // that generates elements for vector v, of type element_t or int,
    // ranging from 0 up to count, inclusive.
    auto generator = tpf::chrono_random::random_generator<element_t>(0, count);

    // we generate random numbers and initialize container v
    for(size_t i = 0; i < count; ++i)
        v[i] = generator();

    stream << "Before: " << v << endl;

    std::mutex mutex;
    using lock_type = std::lock_guard<std::mutex>;

    std::set<std::string> threads_set;

    std::for_each(std::execution::par_unseq, v.begin(), v.end(), 
        [lambda_init = int{}, &mutex, &threads_set](auto&& e)
        { 
            std::stringstream os; // os is a local variable
                                 // we do not need synchronize local variables

            int local_int; // each thread has its own copy of local_int for all compilers

            // In case of lambda_init
            // microsoft compiler - lambda_init is shared among threads
            // g++ and clang++ - each thread has its copy of lambda_init

            os << "Thread Id = " << std::this_thread::get_id() 
                << "\t &lambda_init: " << &lambda_init 
                << "\t&local_int: " << &local_int;

            // we should always narrow down the locked scope
            // to prevent dead-lock as well as to improve the performance
            {
                // std::lock_guard<std::mutex> lock(mutex)
                lock_type lock(mutex);

                threads_set.insert( os.str() );

                // std::cout is a global console output stream
                // this cout is shared among multiple threads
                // so, it should be enclosed in the locked scope
                std::cout << os.str() << std::endl;
            }
        } );

    // this is out of parallel algorithm
    stream << "\n Number of threads used: " << threads_set.size() << endl;

    stream << "Threads Info: " << threads_set << endl;

}

int thread_count{0};

class functor
{
    private:
        int lambda_init;

        
        inline static std::mutex mutex;

        std::set<std::string> &threads_set;
        // inline static is introduced to C++17 standard

        using lock_type = std::lock_guard<std::mutex>;

    public:
        functor(std::set<std::string>& threads): threads_set{threads}
        {
            lock_type lock(mutex);

            std::cout << "Default constructor is called" << std::endl;
            ++thread_count;
        }
        functor(const functor& rhs): threads_set{rhs.threads_set}
        {
            lock_type lock(mutex);

            ++thread_count;
            std::cout << "Copy constructor is called" << std::endl;
        }

        functor(functor&& rhs): threads_set{rhs.threads_set}
        {
            lock_type lock(mutex);
            // ++thread_count;
            std::cout << "Move constructor is called" << std::endl;
        }

        template<typename Type>
        void operator()(Type&& e) const // this trailing const specifier is IMPORTANT
        { 
            std::stringstream os; // os is a local variable
                                 // we do not need synchronize local variables

            int local_int; // each thread has its own copy of local_int for all compilers

            // In case of lambda_init
            // microsoft compiler - lambda_init is shared among threads
            // g++ and clang++ - each thread has its copy of lambda_init

            os << "Thread Id = " << std::this_thread::get_id() 
                << "\t &lambda_init: " << &lambda_init 
                << "\t&local_int: " << &local_int;

            // we should always narrow down the locked scope
            // to prevent dead-lock as well as to improve the performance
            {
                // std::lock_guard<std::mutex> lock(mutex)
                lock_type lock(mutex);

                threads_set.insert( os.str() );

                // std::cout is a global console output stream
                // this cout is shared among multiple threads
                // so, it should be enclosed in the locked scope
                std::cout << os.str() << std::endl;
            }
        }

        ~functor()
        {
            // --thread_count
        }
};

void test_threads_in_parallel_algorithm_with_functor()
{
    size_t count = 10;
    using element_t = double;
    std::vector<element_t> v(count);

    // this is a random number generator
    // that generates elements for vector v, of type element_t or int,
    // ranging from 0 up to count, inclusive.
    auto generator = tpf::chrono_random::random_generator<element_t>(0, count);

    // we generate random numbers and initialize container v
    for(size_t i = 0; i < count; ++i)
        v[i] = generator();

    stream << "Before: " << v << endl;

    std::mutex mutex;
    using lock_type = std::lock_guard<std::mutex>;

    std::set<std::string> threads_set;

    std::for_each(std::execution::par_unseq, v.begin(), v.end(), functor{threads_set});

    // // this is out of parallel algorithm
    stream << "\n Number of threads used: " << threads_set.size() << endl;
    stream << "Threads Info: " << threads_set << endl;

    stream << "thread_count - constructor count - " << thread_count << endl;

}

int main()
{
    // test_threads_in_parallel_algorithm();
    // test_threads_in_parallel_algorithm_for_locals();
    test_threads_in_parallel_algorithm_with_functor();
}