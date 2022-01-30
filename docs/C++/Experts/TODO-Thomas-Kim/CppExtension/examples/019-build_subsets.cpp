#include <tpf_set.hpp>
#include <tpf_chrono_random.hpp>
#include <tpf_ncrnpr.hpp>
#include <tpf_output.hpp>

namespace types = tpf::types;
namespace set = tpf::set;
namespace crs = tpf::chrono_random;
namespace cpr = tpf::ncrnpr;

void examples_for_build_subsets()
{
    
    tpf::sstream stream;
    
    using set_tag = tpf::set_tag<std::vector, short>;
    using set_t = tpf::set_t<set_tag>;
    using sets_t = tpf::sets_t<set_tag>;
    using set_of_sets_t = tpf::set_of_sets_t<set_tag>;
    
    set_t myset{  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                  30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                  40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                  50};

    size_t select_count = 6;
    auto cmb = cpr::ncr(myset.size(), select_count);

    stream << myset.size() << "_C_" << select_count <<" = " << cmb << tpf::endl;


    crs::stop_watch sw;

    auto subsets = set::build_subsets(myset, select_count);

    stream << "Elapsed: " << sw.elapsed_time<crs::second_t>() << tpf::endl;
    //stream << subsets << tpf::endl;
}

int main()
{
    examples_for_build_subsets();
}