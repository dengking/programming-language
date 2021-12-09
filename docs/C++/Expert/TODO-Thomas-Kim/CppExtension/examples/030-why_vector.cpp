#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>

void random_initialization_test()
{
    tpf::sstream stream;

    size_t count = 100;
    using element_t = int;

    auto generator = 
        tpf::chrono_random::random_generator<element_t>(0, 1'000);

    tpf::chrono_random::stop_watch sw;

    stream << "Initializing vector with " << count << " elements." << tpf::endl;
    std::vector<int> v; v.reserve(count);

    for(size_t i=0; i < count; ++i)
        v.emplace_back(generator());

    std::sort(v.begin(), v.end());
    stream << "Elapsed: " << sw.elapsed_time() << tpf::endl;

    stream << "Initializing list with " << count << " elements." << tpf::endl;
    std::list<int> l; 
    for(size_t i=0; i < count; ++i)
    {
        l.insert(l.end(), generator());
    }

    l.sort();
    stream << "Elapsed: " << sw.elapsed_time() << tpf::endl;

    stream << "v = " << v << tpf::endl;
    stream << "l = " << l << tpf::endl;

}

int main()
{
    random_initialization_test();
}