#include <tpf_set.hpp>
#include <tpf_chrono_random.hpp>
#include <tpf_ncrnpr.hpp>
#include <tpf_output.hpp>

namespace types = tpf::types;
namespace set = tpf::set;
namespace crs = tpf::chrono_random;
namespace cpr = tpf::ncrnpr;

tpf::sstream stream;

void examples_for_build_permutations(unsigned thread_count, int n, int r)
{  
    
    using set_tag = tpf::set_tag<std::vector, char>;
    using set_t = tpf::set_t<set_tag>;
    using sets_t = tpf::sets_t<set_tag>;
    
    crs::stop_watch sw;
    stream <<"Going in parallel ..." << tpf::endl;
    auto permutations = set::build_permutations(thread_count, n, r);
    stream << "Elapsed: " << sw.elapsed_time<crs::second_t>() << tpf::endl;
    stream << permutations << tpf::endl;

    stream <<"Going in sequence ..." << tpf::endl;
    sw.reset();
    permutations = set::build_permutations(n, r);
    stream << "Elapsed: " << sw.elapsed_time<crs::second_t>() << tpf::endl;
    stream << permutations << tpf::endl;

}

void examples_for_build_permutations_flat(unsigned thread_count, int n, int r)
{  
    
    using set_tag = tpf::set_tag<std::vector, char>;
    using set_t = tpf::set_t<set_tag>;
    using sets_t = tpf::sets_t<set_tag>;
    
    crs::stop_watch sw;
    stream <<"Going in parallel ..." << tpf::endl;
    set::build_permutations_flat(thread_count, n, r);
    stream << "Elapsed: " << sw.elapsed_time<crs::second_t>() << tpf::endl;

    stream <<"Going in sequence ..." << tpf::endl;
    set::build_permutations_flat(n, r);
    stream << "Elapsed: " << sw.elapsed_time<crs::second_t>() << tpf::endl;

}

int main(int argc, char* argv[])
{
    if(argc == 4)
    {
        auto thread_count = atoi(argv[1]) * std::thread::hardware_concurrency();
        auto n = atoi(argv[2]);
        auto r = atoi(argv[3]);
                
        try
        {
            auto permu = cpr::npr(n, r);
            stream << "Computing "<< n << "_P_" << r <<" = "
                << permu << " permutations..."<<tpf::endl;

            // examples_for_build_permutations_flat(thread_count, n, r);
            examples_for_build_permutations(thread_count, n, r);
        }
        catch(const std::exception&)
        {
            stream << "Number too big!" << tpf::endl;
        }
    }
    else
    {
        stream << argv[0] <<" n r" << tpf::endl;
    }
}