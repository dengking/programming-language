#include <tpf_parallel.hpp>
#include <tpf_output.hpp>
#include <tpf_safe_type.hpp>
#include <tpf_chrono_random.hpp>

#include <execution>
#include <thread>
#include <sstream>

tpf::sstream stream;
auto endl = tpf::endl;

void how_many_threads()
{
    size_t count = 100;
    std::vector<int> v(count);

    auto generator = tpf::chrono_random::random_generator<int>(0, 100);

    tpf::chrono_random::random_fill(v, generator);

    std::set<std::string> threads;

    tpf::sstream output;
    auto endl = tpf::endl;
    
    std::mutex mutex;

    auto callback = [member = int{}, &output, &endl, &threads, &mutex](auto& v)
    {
        std::stringstream os; 
        os << std::this_thread::get_id() << " - &member: " << &member ;
        threads.insert(os.str());

        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "Thread ID: " << std::this_thread::get_id() << " - \t" << &member << std::endl;
    };

    Tpf_GoParallel_VoidAlgorithm_VoidCallback(for_each, callback,
        std::execution::par, v.begin(), v.end());
    
    stream << "Threads used: " << threads.size() << endl; 

    stream << threads << endl;
    
}

template<typename Type>
struct functor
{
    inline static thread_local int m_thread_local;
    
    inline static std::mutex m_mutex;

    int m_instance_member;

    std::set<std::string>& threads;

    functor(std::set<std::string>& set_threads):
        threads{set_threads} 
        { 
            // std::cout << "constructor called" << std::endl;
        }

    functor(const functor& lhs): threads{lhs.threads}
    {
        // std::cout << "Copy constructor called" << std::endl;
    }

    functor(functor&& lhs): threads{lhs.threads}
    {
        // std::cout << "Move constructor called" << std::endl;
    }

    void operator()(Type& e) const
    {
        std::stringstream os; 
        os << std::this_thread::get_id() << " - &m_instance_member: " << &m_instance_member;
        threads.insert(os.str());

        std::lock_guard<std::mutex> lock(functor::m_mutex);
        std::cout << "Thread ID: " << std::this_thread::get_id() 
            << " - \t" << &functor::m_thread_local << std::endl;
    }

    ~functor()
    {
        // std::cout << "Destructor called" << std::endl;
    }
};

void how_many_threads_with_functor()
{
    size_t count = 100;
    std::vector<int> v(count);

    auto generator = tpf::chrono_random::random_generator<int>(0, 100);

    tpf::chrono_random::random_fill(v, generator);

    std::set<std::string> threads;

    tpf::sstream output;
    auto endl = tpf::endl;

    auto callback = [member=int{}, &output, &endl, &threads](auto& v)
    {
        std::stringstream os; 
        os << std::this_thread::get_id();
        threads.insert(os.str());
    };

    std::for_each(std::execution::par_unseq, v.begin(), v.end(), functor<int>{threads});
    
    stream << "Threads used: " << threads.size() << endl; 

    stream << threads << endl;
    
}


void test_for_each()
{
     size_t count = 100;
    std::vector<int> v(count);

    auto generator = tpf::chrono_random::random_generator<int>(0, 100);

    tpf::chrono_random::random_fill(v, generator);

    auto callback = [](auto& e)
    {
        ++e;
    };

    stream << "Before: " << v << endl;

    auto operation = tpf::parallel::safe_for_each(callback, std::execution::par_unseq, v.begin(), v.end());

    if(operation)
        stream << "After: " << v << endl;
    else
        stream <<"Operation failed" << endl;
}

void test_transform()
{
     size_t count = 100;
    std::vector<int> v(count);

    auto generator = tpf::chrono_random::random_generator<int>(0, 100);

    tpf::chrono_random::random_fill(v, generator);

    auto callback = [](const auto& e)
    {
        return e + 1;
    };

    stream << "Before: " << v << endl;

    auto operation = 
        tpf::parallel::safe_transform(callback, std::execution::par_unseq, v.begin(), v.end(), v.begin());

    if(operation)
        stream << "After: " << v << endl;
    else
        stream <<"Operation failed" << endl;
}

void test_for_each_throw()
{
     size_t count = 100;
    std::vector<int> v(count);

    auto generator = tpf::chrono_random::random_generator<int>(0, 100);

    tpf::chrono_random::random_fill(v, generator);

    auto callback = [](auto& e)
    {
        ++e;
    };

    stream << "Before: " << v << endl;

    try
    {
        tpf::parallel::throw_for_each(callback, std::execution::par_unseq, v.begin(), v.end());
        
        stream << "After: " << v << endl;
    }
    catch(const std::exception& e)
    {
        stream << e << endl;
    }

}

void test_transform_throw()
{
     size_t count = 100;
     using element_t = double;

    std::vector<element_t> v(count);

    auto generator = tpf::chrono_random::random_generator<element_t>(-10, 10);

    tpf::chrono_random::random_fill(v, generator);
    
    std::mutex mutex;

    auto callback = [&mutex, &generator](const auto& e)
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto d = generator();

        return tpf::safe_type::safe_div(e, d);
    };

    stream << "Before: " << v << endl;

    try
    {
        tpf::parallel::throw_transform(callback, std::execution::par_unseq, v.begin(), v.end(), v.begin());
        stream << "After: " << v << endl;
    }
    catch(const std::exception& e)
    {
        stream << "Operation failed" << endl;
    }
    
}


void test_find_if()
{
     size_t count = 100;
     using element_t = int;

    std::vector<element_t> v(count);

    auto generator = tpf::chrono_random::random_generator<element_t>(-10, 10);

    tpf::chrono_random::random_fill(v, generator);
    
    std::mutex mutex;

    auto callback = [](const auto& e)
    {
        return e % 5 == 0;
    };

    stream << "Before: " << v << endl;

    auto [opr, rlt] = tpf::parallel::safe_find_if(callback, std::execution::par_unseq, v.begin(), v.end());

    if(opr)
    {
        stream <<"Found: " <<  *rlt << endl;
    }
    else
    {
        stream << "Operation failed" << endl;
    }

}


int main()
{
    // how_many_threads();
    how_many_threads_with_functor();

    // test_for_each();
    // test_transform();

    // test_for_each_throw();

    // test_transform_throw();

    // test_find_if();
}