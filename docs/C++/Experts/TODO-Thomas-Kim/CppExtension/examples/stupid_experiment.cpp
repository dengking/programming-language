#include <tpf_ncrnpr.hpp>
#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>
#include <future>
#include <atomic>

using big_int_t = long long;

big_int_t single_thread_ncr_recursion(big_int_t n, big_int_t k)
{
    if(k == 1)
        return n;
    else if(n == k)
        return 1;
    else
    {
        return single_thread_ncr_recursion(n-1, k-1) + 
                single_thread_ncr_recursion(n-1, k);
    }    
}

constexpr int factor = 2;
int max_thread_count = 
    factor * std::thread::hardware_concurrency();
 
std::atomic<int> thread_count{0};

big_int_t multiple_thread_ncr_recursion(big_int_t n, big_int_t k)
{
    if(k == 1)
        return n;
    else if(n == k)
        return 1;
    else
    {
        if(thread_count < max_thread_count)
        {
            ++thread_count;
            auto ncr_n_1_k_1 = std::async(single_thread_ncr_recursion, n-1, k-1);
            auto ncr_n_1_k = single_thread_ncr_recursion(n-1, k);
            auto rlt = ncr_n_1_k + ncr_n_1_k_1.get();
            
            --thread_count; 
            
            return rlt;
        }
        else
        {
            return single_thread_ncr_recursion(n-1, k-1)
                + single_thread_ncr_recursion(n-1, k);
        }
    }    
}

void examples_for_combination()
{
    big_int_t n = 40;
    big_int_t k = 10;

    tpf::chrono_random::stop_watch sw;

    std::cout << "tpf::ncrnpr::ncr   loop: " 
        << tpf::ncrnpr::ncr(n, k) 
        << ", elapsed: " << sw.elapsed_time<tpf::chrono_random::second_t>() << std::endl;

    std::cout << "single_thread_recursion: " 
        << single_thread_ncr_recursion(n, k)
        << ", elapsed: " << sw.elapsed_time<tpf::chrono_random::second_t>() << std::endl;
}

void examples_for_single_vs_multithread_recursion()
{
    big_int_t n = 40;
    big_int_t k = 12;

    tpf::chrono_random::stop_watch sw;

    std::cout << "multiple_thread_ncr_recursion: " 
        << multiple_thread_ncr_recursion(n, k)
        << ", elapsed: " << sw.elapsed_time<tpf::chrono_random::second_t>() << std::endl;
        
    std::cout << "single_thread_recursion      : " 
        << single_thread_ncr_recursion(n, k)
        << ", elapsed: " << sw.elapsed_time<tpf::chrono_random::second_t>() << std::endl;
}

int main()
{
    // examples_for_combination();
    examples_for_single_vs_multithread_recursion();
}