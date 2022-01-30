#include <tpf_ncrnpr.hpp>
#include <tpf_chrono_random.hpp>
#include <tpf_output.hpp>

#include <future>
#include <atomic>

// (n, k) = (n-1, k-1) + (n-1, k)
struct cross_thread_recursion
{
    using big_int_t = long long;

    static constexpr int factor = 1;

    // "inline static" was introduced to C++17 standard
    inline static int max_thread_count = factor * 
        std::thread::hardware_concurrency();

    inline static std::atomic<int> thread_count{0};

    static constexpr auto async = std::launch::async;
    static constexpr auto deferred = std::launch::deferred; 

    static big_int_t 
        single_thread_combination_recursion(big_int_t n, big_int_t k)
    {
        if  (k == 0 || n == k)
        {
            return 1;
        }
        else if (k == 1 || k == (n - 1))
        {
            return n;
        }
        else
        {
           // nCk = nCn-k, (n, k) == (n, n-k)
           if(k > (n-k) ) k = n-k;
           // (n, k) = (n-1, k-1) + (n-1, k)

            return single_thread_combination_recursion(n-1, k-1) // (n-1, k-1)
                    + single_thread_combination_recursion(n-1, k); // (n-1, k)
        }
    }

    //////////////////////////
    static big_int_t 
        cross_thread_combination_recursion(big_int_t n, big_int_t k)
    {
        if  (k == 0 || n == k)
        {
            return 1;
        }
        else if (k == 1 || k == (n - 1))
        {
            return n;
        }
        else
        {
           // nCk = nCn-k, (n, k) == (n, n-k)
           if(k > (n-k) ) k = n-k;
           // (n, k) = (n-1, k-1) + (n-1, k)

            try
            {
                // we want to limit the number of currently running
                // threads less than max_thread_count
                if(thread_count < max_thread_count)
                {
                    // (n-1, k-1) - calculate in a different new thread simultaneously
                    auto future_ncr_n_1_k_1
                        = std::async(async, cross_thread_combination_recursion, n-1, k-1);
                    
                    ++thread_count; // this is wrong

                    // (n-1, k) - calculate in the current thread
                    auto current_thread_n_1_k = cross_thread_combination_recursion(n-1, k);

                    auto rlt = current_thread_n_1_k + future_ncr_n_1_k_1.get();

                    --thread_count; // this is wrong, but I will explain why in future sessions

                    return rlt;
                }
                else
                {
                    // if concurrently running threads are too many,
                    // then use current thread only
                    return cross_thread_combination_recursion(n-1, k-1) // (n-1, k-1)
                         + cross_thread_combination_recursion(n-1, k); // (n-1, k)
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
               
                // this is to suppress warning
                return {};
            }
        }
    }

    //////////////////////////////////////
    //////////////////////////
    static big_int_t 
        smart_cross_thread_combination_recursion(big_int_t n, big_int_t k)
    {
        if  (k == 0 || n == k)
        {
            return 1;
        }
        else if (k == 1 || k == (n - 1))
        {
            return n;
        }
        else
        {
           // nCk = nCn-k, (n, k) == (n, n-k)
           if(k > (n-k) ) k = n-k;
           // (n, k) = (n-1, k-1) + (n-1, k)

            try
            {
                // we want to limit the number of currently running
                // threads less than max_thread_count
                if(thread_count < max_thread_count)
                {
                    // (n-1, k-1) - calculate in a different new thread simultaneously
                    auto future_ncr_n_1_k_1
                        = std::async(async, smart_cross_thread_combination_recursion, n-1, k-1);
                    
                    ++thread_count; // this is wrong

                    // (n-1, k) - calculate in the current thread
                    auto current_thread_n_1_k = smart_cross_thread_combination_recursion(n-1, k);

                    auto rlt = current_thread_n_1_k + future_ncr_n_1_k_1.get();

                    --thread_count; // this is wrong, but I will explain why in future sessions

                    return rlt;
                }
                else
                {
                    // if concurrently running threads are too many,
                    // then use current thread only
                    return single_thread_combination_recursion(n-1, k-1) // (n-1, k-1)
                         + single_thread_combination_recursion(n-1, k); // (n-1, k)
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
               
                // this is to suppress warning
                return {};
            }
        }
    }
};

void loop_vs_recursion()
{
    using ctr = cross_thread_recursion;
    
    ctr::big_int_t n = 40;
    ctr::big_int_t k = 12;

    // https://en.wikipedia.org/wiki/Binomial_coefficient
    // https://en.wikipedia.org/wiki/Combination
    // (n, k) == (40, 12) <- binomial coefficient

    tpf::chrono_random::stop_watch sw;

    std::cout << "tpf::ncrnpr::ncr                    : "
        << tpf::ncrnpr::ncr(n, k)
        << ", Elapsed: " << sw.elapsed_time() << std::endl;

    std::cout << "single_thread_combination_recursion : "
        << ctr::single_thread_combination_recursion(n, k)
        << ", Elapsed: " << sw.elapsed_time() << std::endl;
}

void single_vs_multiple_thread_recursion()
{
    using ctr = cross_thread_recursion;
    
    ctr::big_int_t n = 40;
    ctr::big_int_t k = 12;

    // https://en.wikipedia.org/wiki/Binomial_coefficient
    // https://en.wikipedia.org/wiki/Combination
    // (n, k) == (40, 12) <- binomial coefficient

    tpf::chrono_random::stop_watch sw;

    std::cout << "smart_cross_thread_combination_recursion  : "
        << ctr::smart_cross_thread_combination_recursion(n, k)
        << ", Elapsed: " << sw.elapsed_time() << std::endl;

    std::cout << "cross_thread_combination_recursion        : "
        << ctr::cross_thread_combination_recursion(n, k)
        << ", Elapsed: " << sw.elapsed_time() << std::endl;

    std::cout << "single_thread_combination_recursion       : "
        << ctr::single_thread_combination_recursion(n, k)
        << ", Elapsed: " << sw.elapsed_time() << std::endl;
}

int main()
{
    std::cout << "========= single thread : loop vs recursion ==== " << std::endl;
    loop_vs_recursion();

    std::cout << "\n\n========= single thread vs. multithread recursion ==== " << std::endl;

    single_vs_multiple_thread_recursion();
}

