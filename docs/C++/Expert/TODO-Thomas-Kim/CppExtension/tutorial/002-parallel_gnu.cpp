#include <tpf_chrono_random.hpp>
#include <tpf_output.hpp>

// MSVC      : cl /EHsc /std:c++latest /O2 002-parallel_gnu.cpp /Femo.exe
// Intel C++ : icl /EHsc /std:c++latest /O2 002-parallel_gnu.cpp /Feio.exe
// GNU g++   : g++ -std=c++2a -fopenmp -D_GLIBCXX_PARALLEL -O3 002-parallel_gnu.cpp -o go.exe
// clang++   : clang++ -std=c++2a -O3 002-parallel_gnu.cpp -o co.exe

tpf::sstream stream; // string stream, so we have to flush to the console
                        // otherwise, we cannot see the result.

auto nl = "\n";
auto nL = "\n\n";
auto flush = tpf::output::nl();

/*
    In future sessions, we will be using random number generators and
    stop_watch all the time. So, please make yourself familiar with random number
    generator and stop_watch.

*/
void examples_for_random_stop_watch()
{
    // STEP 1. define namespace alias and some types for our use
    namespace crd = tpf::chrono_random;
    // don't ever do this if you like my code or programming tutorials
    // using namespace tpf::chrono_random;
    // never do this, using namespace std;

    // use type alias all the time
    using element_t = int;
    using container_t = std::vector<element_t>;

    // sometime later, if you want change your container type

    // STEP 2. we create a random number generator

    // we are creating a random number generator
    // that generates type element_t (or float)
    // ranging from 1 to 100 inclusive. By "inclusive,"
    // I mean that 1 and 100 can also be generated
    auto generator = crd::random_generator<element_t>(1, 100);

    size_t count = 10; // we will create count of elements of type element_t
    size_t test_count = 5; // we will test test_count times

    crd::stop_watch sw;

    for(size_t i = 0; i < test_count; ++ i)
    {
        container_t v;
        sw.reset();

        crd::random_fill(v, generator, count);
        
        stream <<"Generated randoms: " << v << ", Elapsed: " 
            << sw.elapsed_time<crd::nano_t>() << nl; 
    }

    stream << flush;
}

int main()
{
    examples_for_random_stop_watch();

}