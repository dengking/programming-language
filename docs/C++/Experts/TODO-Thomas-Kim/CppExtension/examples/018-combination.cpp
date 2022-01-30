#include <tpf_ncrnpr.hpp>
#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>

void performance_test_for_combination_parallel(unsigned int thread_count, int n, int r)
{
    namespace cmb = tpf::ncrnpr;
    namespace crs = tpf::chrono_random;

    tpf::sstream stream;
    std::mutex mutex;

    auto nl = tpf::nl;
    auto endl = tpf::endl;

    using number_t = cmb::combination::number_t;
    
    auto max_mth = cmb::ncr(n,r);

    auto combination_handler = [&stream, &mutex](auto&& combination)
    {
        // std::unique_lock<std::mutex> lock{mutex};
        // stream << combination << tpf::endl;
    };

    crs::stop_watch sw;
    stream <<"Enumerating in parallel with selected only ... " << tpf::endl;
     tpf::parallel_visit_combinations(n, r, combination_handler);
    stream <<"Elapsed: " << sw.elapsed_time<crs::second_t>() << endl << endl; 

    stream <<"Enumerating in parallel with complement ... " << tpf::endl;
    tpf::parallel_visit_combinations_and_complements(n, r, combination_handler);
    stream <<"Elapsed: " << sw.elapsed_time<crs::second_t>() << endl << endl; 

    stream << "Enumerating in sequence ..." << tpf::endl;
    for(number_t m_th = 0; m_th < max_mth; ++m_th)
    {
        combination_handler(cmb::enumerate_combination_and_complement(m_th, n, r));
    }
    stream <<" Elapsed: " << sw.elapsed_time<crs::second_t>() << endl;

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

        tpf::ncrnpr::combination::number_t cmb;

        try
        {
            cmb = tpf::ncrnpr::ncr(n, r);
        } 
        catch(const std::exception&)
        {
            stream << "Number Too Big!" << tpf::endl;
            return 0;
        }
        
        stream << "Enumerating " << n << "_C_" <<r 
            <<" = " << cmb 
            <<" combinations with " << thread_count 
            << " threads" << tpf::endl;

        performance_test_for_combination_parallel(thread_count, n, r);
    }
    else
    {
        std::cout <<"Usage: " << argv[0] << std::endl;
        std::cout <<"> " << argv[0] << " thread_count_factor  n   r" <<std::endl;
        std::cout <<"\tthread_count = hardware_thread x thread_count_factor" <<std::endl;
        std::cout <<"\tn P r" << std::endl;
    }
}