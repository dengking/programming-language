#include <tpf_set.hpp>
#include <tpf_chrono_random.hpp>
#include <tpf_ncrnpr.hpp>
#include <tpf_output.hpp>

namespace types = tpf::types;
namespace set = tpf::set;
namespace crs = tpf::chrono_random;
namespace cpr = tpf::ncrnpr;

tpf::sstream stream;

namespace tuv // tutorial video
{
    struct thread_bundle
    {
        // this will be used for read-only
        // so, we do not safeguard this variable
        int max_thread_count;

        // this variable will be shared among threads
        // so, we have to safeguard from HALF-READ
        std::atomic<int> current_thread_count;

        // to synchronize stream console output
        std::mutex mutex;
    };

    template<template<typename, typename...> class ContainerType,
        typename EleType, typename... Types, typename... OuterTypes>
    void enum_permu(ContainerType<ContainerType<EleType, Types...>, OuterTypes...>& permutations,
        ContainerType<EleType, Types...>& L, 
        ContainerType<EleType, Types...>& R)
    {
        if(L.empty())
        {
            permutations.emplace_back(R);
        }
        else
        {
            size_t size = L.size();
            
            // make a room for last element
            R.emplace_back(EleType{});

            /*
                if an exception is thrown before R.pop_back() is reached
                this program does not work properly

                we need to handle this issue in EXCEPTION-SAFE manner
            */

            auto r_pop_back = [&R](auto ptr) { R.pop_back(); };

            using r_type_t = std::remove_reference_t<decltype(R)>;

            // clean_up is also a local variable
            std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                clean_up(&R, r_pop_back);

            for(size_t i_th = 0; i_th < size; ++i_th)
            {
                // copy L to copied_L
                auto copied_L = L;

                // remove i_th element from copied_L
                copied_L.erase(copied_L.begin()+i_th);

                // move i_th element from L to R
                // R.emplace_back( L[i_th] );
                R.back() = L[i_th];

                enum_permu(permutations,copied_L, R);
            }
        }  
    }

    template<typename NRType>
    auto build_permutations(NRType n)
    {
        using element_t = short;
        using container_t = std::vector;

        using set_tag = tpf::set_tag<container_t, element_t>;
        using set_t = tpf::set_t<set_tag>;
        using sets_t = tpf::sets_t<set_tag>;

        set_t L; L.reserve(n);
        for(size_t i = 0; i < (size_t)n; ++i)
            L.emplace_back((element_t)i);

        set_t R; R.reserve(n);

        sets_t permutations;
        
        enum_permu(permutations, L, R);

        return permutations;
    }

    ////////////////////////////////////////////////////////
    template<typename CallbackType, template<typename, typename...> class ContainerType,
        typename EleType, typename... Types>
    void enum_permu(CallbackType&& callback, // callback function
        ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R)
    {
        if(L.empty())
        {
            callback(R);
        }
        else
        {
            size_t size = L.size();
            
            // make a room for last element
            R.emplace_back(EleType{});

            /*
                if an exception is thrown before R.pop_back() is reached
                this program does not work properly

                we need to handle this issue in EXCEPTION-SAFE manner
            */

            auto r_pop_back = [&R](auto ptr) { R.pop_back(); };

            using r_type_t = std::remove_reference_t<decltype(R)>;

            // clean_up is also a local variable
            std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                clean_up(&R, r_pop_back);

            for(size_t i_th = 0; i_th < size; ++i_th)
            {
                // copy L to copied_L
                auto copied_L = L;

                // remove i_th element from copied_L
                copied_L.erase(copied_L.begin()+i_th);

                // move i_th element from L to R
                // R.emplace_back( L[i_th] );
                R.back() = L[i_th];

                enum_permu(std::forward<CallbackType>(callback), copied_L, R);
            }
        }  
    }

    template<typename CallbackType, typename NRType>
    void build_permutations(CallbackType&& callback, NRType n)
    {
        using element_t = short;
        using container_t = std::vector;

        using set_tag = tpf::set_tag<container_t, element_t>;
        using set_t = tpf::set_t<set_tag>;
        using sets_t = tpf::sets_t<set_tag>;

        set_t L; L.reserve(n);
        for(size_t i = 0; i < (size_t)n; ++i)
            L.emplace_back((element_t)i);

        set_t R; R.reserve(n);

        enum_permu(std::forward<CallbackType>(callback), L, R);
    }

    ///////////////////////////// Going Parallel ///////////////////////////
    template<typename CallbackType, template<typename, typename...> class ContainerType,
        typename EleType, typename... Types>
    void enum_permu(thread_bundle& tb, CallbackType&& callback, // callback function
        ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R)
    {
        using set_t = ContainerType<EleType, Types...>;

        if(L.empty())
        {
            callback(R);
        }
        else
        {
            size_t size = L.size();
            
            // make a room for last element
            R.emplace_back(EleType{});

            /*
                if an exception is thrown before R.pop_back() is reached
                this program does not work properly

                we need to handle this issue in EXCEPTION-SAFE manner
            */

            auto r_pop_back = [&R](auto ptr) { R.pop_back(); };

            using r_type_t = std::remove_reference_t<decltype(R)>;

            // clean_up is also a local variable
            std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                clean_up(&R, r_pop_back);

            using future_t = std::future<void>; // void, beause enum_permu() does not return
            using futures_t = std::vector<future_t>;

            futures_t futures; // std::vector<std::future<void>>

            for(size_t i_th = 0; i_th < size; ++i_th)
            {
                // copy L to copied_L
                auto copied_L = L;

                // remove i_th element from copied_L
                copied_L.erase(copied_L.begin()+i_th);

                // move i_th element from L to R
                // R.emplace_back( L[i_th] );
                R.back() = L[i_th];

                if(size < 7 || tb.current_thread_count >= tb.max_thread_count)
                    enum_permu(tb, std::forward<CallbackType>(callback), copied_L, R);
                else
                {
                    auto go_parallel = [&tb, &callback, L = set_t{copied_L}, R = set_t{R}]() mutable
                    {
                        enum_permu(tb, callback, L, R);
                    };

                    futures.emplace_back(std::async(std::launch::async, go_parallel));
                    ++tb.current_thread_count;
                }
            }

            for(auto& f: futures)
            {
                f.wait(); --tb.current_thread_count;
            }
        }  
    }

    template<typename CallbackType, typename NRType>
    void build_permutations(thread_bundle& tb, CallbackType&& callback, NRType n)
    {
        using element_t = short;
        using container_t = std::vector;

        using set_tag = tpf::set_tag<container_t, element_t>;
        using set_t = tpf::set_t<set_tag>;
        using sets_t = tpf::sets_t<set_tag>;

        set_t L; L.reserve(n);
        for(size_t i = 0; i < (size_t)n; ++i)
            L.emplace_back((element_t)i);

        set_t R; R.reserve(n);

        enum_permu(tb, std::forward<CallbackType>(callback), L, R);
    }

} // end of namespace tuv or tutorial video

void test_build_permutations(int n)
{
    auto permu = cpr::npr(n, n);
    stream <<"Generating "<< permu << " permutations!" << tpf::endl;

    crs::stop_watch sw;
    auto permutations = tuv::build_permutations(n);
    stream << "Elapsed: " << sw.elapsed_time<crs::second_t>() << tpf::endl;

    for(auto& p: permutations)
        stream << p << tpf::nl;
    
    // if we don't flush, we cannot see the result
    stream << tpf::flush;
}

template<typename ThreadFactorType>
void performance_test_build_permutations(ThreadFactorType factor, int n)
{
    auto permu = cpr::npr(n, n);
    stream <<"Generating "<< permu << " permutations!" << tpf::endl;

    tpf::sstream output;

    tuv::thread_bundle tb{ (int) std::thread::hardware_concurrency() * factor };

    auto callback_parallel = [&tb, &output](auto p)
    {
        if(p.size() < 6)
        {
            std::unique_lock<std::mutex> lock(tb.mutex);
            output << p << " - " << std::this_thread::get_id() << tpf::endl;
        }

        // more real code based of permutation p
    };

     auto callback_sequential = [&output](auto p)
    {
        if(p.size() < 6)
        {
            output << p << " - " << std::this_thread::get_id() << tpf::endl;
        }
        // more real code based of permutation p
    };

    crs::stop_watch sw;

    stream <<"Generating permutations in parallel." << tpf::endl;
    tuv::build_permutations(tb, callback_parallel, n);
    stream << "Elapsed: " << sw.elapsed_time<crs::second_t>() << tpf::endl;
    
    stream <<"Generating permutations in sequence." << tpf::endl;
    tuv::build_permutations(callback_sequential, n);
    stream << "Elapsed: " << sw.elapsed_time<crs::second_t>() << tpf::endl;
}

int main(int argc, char* argv[])
{
    if(argc > 2) // two or more commandline arguments
    {
        auto factor = atoi(argv[1]);
        auto n = atoi(argv[2]);
        
        stream << "Using " << factor* std::thread::hardware_concurrency()
            << " threads..."<< tpf::endl;

        // test_build_permutations(n);
        performance_test_build_permutations(factor, n);
    }
    else
    {
        stream <<"Usage: " << argv[0] <<" factor  n" << tpf::endl;
        stream <<"\tfactor is for hardware thread multiplier" << tpf::endl;
        stream <<"\tn is for factorial size" << tpf::endl;
    }
}

		
