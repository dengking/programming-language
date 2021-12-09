#include <iostream>
#include <type_traits>
#include <mutex>
#include <thread>
#include <execution>

#if defined(_MSVC_LANG) || defined(_ICL) 
    #include <ppl.h>
    namespace ccy = concurrency;
#else
    #include <tbb/tbb.h>
    namespace ccy = tbb;
#endif


template<typename CallbackType, typename PolicyType, typename BeginType, typename EndType>
bool parallel_for(CallbackType&& callback, PolicyType&& policy,
    BeginType begin_index, EndType end_index)
{
    // std::vector<size_t> dummy_vector((size_t)1);

    // auto begin = dummy_vector.begin();
    // auto end = begin;
    // std::advance(end, (end_index-begin_index));

    // std::atomic<bool> operation_success{true};

    // auto callback_wrapper = [&operation_success, &begin, &callback](auto& itr)
    // {
    //     if(operation_success)
    //     {  
    //         try
    //         {
    //             callback((BeginType)std::distance(begin, &itr));
    //         }
    //         catch(...)
    //         {
    //             operation_success = false;
    //         }
    //     }
    // };

    // std::for_each(policy, begin, end, callback_wrapper);

    return true; // operation_success;
}

void test_parallel_for()
{
    std::mutex mutex;
    using lock_type = std::lock_guard<std::mutex>;
    
    auto callback = [&mutex](auto index)
    {
        lock_type lock(mutex);
        
        std::cout << "Thread ID = " <<std::this_thread::get_id() <<
         "\tIndex = " << index << std::endl;
    };

    ccy::parallel_for(1, 11, callback);
}

int main()
{
    test_parallel_for();
}

