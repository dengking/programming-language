#include <tpf_ncrnpr.hpp>
#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>

void performance_test_for_permutations()
{
    namespace cmb = tpf::ncrnpr;
    namespace crs = tpf::chrono_random;

    tpf::sstream stream;
    auto nl = tpf::nl;
    auto endl = tpf::endl;

    using number_t = cmb::permutation::number_t;

    number_t n = 11;
    number_t r = 10;
    auto max_mth = cmb::npr(n,r);

    stream << n<<"P"<<r<<" = " << max_mth << endl;

    crs::stop_watch sw;
    for(number_t m_th = 0; m_th < max_mth; ++m_th)
    {
        cmb::enum_permutation(n, r, m_th) ;
    }

    auto v_time = sw.elapsed();
    stream <<"using vector: " << v_time << endl;

    for(number_t m_th = 0; m_th < max_mth; ++m_th)
    {
        cmb::enum_permutation(n, r, m_th) ;
    }

    auto l_time = sw.elapsed();
    stream <<"using list: " << l_time << endl;
}

void test_split_range()
{
    namespace cmb = tpf::ncrnpr;
    namespace crs = tpf::chrono_random;

    tpf::sstream stream;
    auto nl = tpf::nl;
    auto endl = tpf::endl;

    auto range = cmb::split_range_count(3, 0, 11);

    stream << range << endl;
}

void test_for_each()
{
    namespace cmb = tpf::ncrnpr;
    cmb::range_t range{1, 10};

    tpf::sstream stream;
    std::mutex mutex;
    
    tpf::visit_permutations_parallel(1,  3, 3, [&stream, &mutex](auto permu)
    {
        std::unique_lock<std::mutex> lock(mutex);
        stream << permu << tpf::endl;
    });
}

void performance_test_for_permutations_parallel(int factor, int n, int r)
{
    namespace cmb = tpf::ncrnpr;
    namespace crs = tpf::chrono_random;

    tpf::sstream stream;
    auto nl = tpf::nl;
    auto endl = tpf::endl;

    using number_t = cmb::permutation::number_t;
    
    number_t max_mth = cmb::npr(n,r); 

    std::mutex mutex;

    auto permu_handler = [&stream, &mutex](auto permu)
    { 
       // std::unique_lock<std::mutex> lock(mutex);
       // stream << permu << " - " << std::this_thread::get_id() << tpf::endl;
    };

    crs::stop_watch sw;
    
    stream <<"Enumerating in parallel visit ... " << tpf::endl;

    tpf::parallel_visit_permutations({n, r}, permu_handler);

    stream <<"Elapsed: " << sw.elapsed_time<crs::second_t>() << endl << endl;

    #if defined(_MSC_VER)
        stream <<"Enumerating in visit parallel ... " << tpf::endl;
        tpf::visit_permutations_parallel(factor, n, r, permu_handler);
        stream <<"Elapsed: " << sw.elapsed_time<crs::second_t>() << endl << endl;
        stream << "Enumerating in sequence ..." << tpf::endl;
    #endif

    auto p = cmb::npr(n-1, r-1);

    for(number_t m_th = 0; m_th < max_mth; ++m_th)
    {
        permu_handler(cmb::enum_permutation_static(p, n, r, m_th));
    }
    stream <<"Elapsed: " << sw.elapsed_time<crs::second_t>() << endl;

}

int main(int argc, char** argv)
{
    unsigned int thread_count = 
        std::thread::hardware_concurrency();
    
    int n, r;

    tpf::sstream stream;
    
    if(argc > 3)
    {
        thread_count *= atoi(argv[1]);
        n = atoi(argv[2]); r = atoi(argv[3]);

        using number_t = tpf::ncrnpr::permutation::number_t;
        number_t permu;

        try
        {
            permu = tpf::ncrnpr::npr(n, r);
        }
        catch(const std::exception&)
        {
            stream << "Number Too Big"<< tpf::endl;
            return 0;
        }

        stream << "Enumerating " << n << "_P_" <<r 
            <<" = " << permu <<" permutations with " 
            << thread_count << " threads" << tpf::endl;
        performance_test_for_permutations_parallel(thread_count, n, r);
    }
    else
    {
        std::cout <<"Usage: " << argv[0] << std::endl;
        std::cout <<"> " << argv[0] << " thread_count_factor  n   r" <<std::endl;
        std::cout <<"\tthread_count = hardware_thread x thread_count_factor" <<std::endl;
        std::cout <<"\tn P r" << std::endl;
    }
}