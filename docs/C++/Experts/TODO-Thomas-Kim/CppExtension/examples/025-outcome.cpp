#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>
#include <future>

class thread_interface_space
{
    public:

    using summation_return_t = long long;
    using division_return_t = double;

    using mutex_t = std::mutex;
    using lock_t = std::unique_lock<mutex_t>;

    static constexpr auto async = std::launch::async;
    static constexpr auto deferred = std::launch::deferred;

    private:

    static inline mutex_t mutex;

    public:
    
    static 
    summation_return_t
    summation(int a, int b)
    {
        tpf::sstream stream;
        stream << "In summation - thread ID: " << std::this_thread::get_id();
        
        {
            lock_t lock(mutex);
            std::cout << stream.str() << std::endl;
        }

        summation_return_t s = 0;
        for(int i=a; i < b; ++i)
            s += i;

        return s;
    }

    static 
    division_return_t
    division(double p, double q)
    {
        tpf::sstream stream;
        stream << "In division - thread ID: " << std::this_thread::get_id();
        
        {
            lock_t lock(mutex);
            std::cout << stream.str() << std::endl;
        }

        division_return_t rlt = p / q;

        if(rlt != rlt) 
        {
            tpf::sstream stream;

            stream << "invalid_operation: p ="
                    << p <<", " << q;

            Tpf_ThrowDebugException(stream);
        }
    }
};

auto single_thread_summation(int max)
{
    using tis = thread_interface_space;
    
    return tis::summation(1, max)
        + tis::summation(1, max);
}

auto multiple_thread_summation(int max)
{
    using tis = thread_interface_space;
   
    try
    {
        std::future<tis::summation_return_t>
            summation = std::async(tis::async, tis::summation, 1, max);
        
        auto s = tis::summation(1, max);

        return s + summation.get(); 
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        
        return tis::summation_return_t{};
    }
}

int main()
{
    using stop_watch_t = tpf::chrono_random::stop_watch;
    tpf::sstream stream;

    stop_watch_t sw;

    stream <<"Single thread summation: "
            << single_thread_summation(1'000'000'000) 
            << " - elapsed: " << sw.elapsed_time() << tpf::endl;

    stream <<"Multiple thread summation: "
            << multiple_thread_summation(1'000'000'000) 
            << " - elapsed: " << sw.elapsed_time() << tpf::endl;
            
}
