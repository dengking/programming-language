#include <tpf_chrono_random.hpp>
#include <tpf_output.hpp>

#if defined(_GLIBCXX_PARALLEL)
    #include <parallel/algorithm>

#elif defined (_MSC_VER) || defined (__ICL) 
    // C++2a Standard header file for Parallel Algorithm
    #include <execution>
#else
    #include <algorithm>
#endif 


// MSVC      : cl /EHsc /std:c++latest /O2 003-parallel_sort_gnu.cpp /Femo.exe
// Intel C++ : icl /EHsc /std:c++latest /O2 003-parallel_sort_gnu.cpp /Feio.exe
// GNU g++   : g++ -std=c++2a -fopenmp -D_GLIBCXX_PARALLEL -O3 003-parallel_sort_gnu.cpp -o go.exe
// clang++   : clang++ -std=c++2a -O3 003-parallel_sort_gnu.cpp -o co.exe

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
void examples_for_parallel_sort_algorithm()
{
    #if defined(_MSC_VER) || defined(__ICL)
        stream <<"Testing either MSVC or Intel C++ compiler"<<nL;
    #else
        stream <<"Testing either GNU g++ or clang++"<<nL;
    #endif 

    // STEP 1. define namespace alias and some types for our use
    namespace crd = tpf::chrono_random;
    // don't ever do this if you like my code or programming tutorials
    // using namespace tpf::chrono_random;
    // never do this, using namespace std;

    // use type alias all the time
    using element_t = double;
    using container_t = std::vector<element_t>;

    // sometime later, if you want change your container type

    // STEP 2. we create a random number generator

    // we are creating a random number generator
    // that generates type element_t (or float)
    // ranging from 1 to 100 inclusive. By "inclusive,"
    // I mean that 1 and 100 can also be generated
    auto generator = crd::random_generator<element_t>(1, 100);

    size_t count = 1'000'000; // we will create count of elements of type element_t
    size_t test_count = 5; // we will test test_count times

    crd::stop_watch sw;

    for(size_t i = 0; i < test_count; ++ i)
    {
        stream <<i  << " - Filling the container with random numbers..."<<nl;
        container_t v;
        crd::random_fill(v, generator, count);

        stream <<"Before sort: front - " << v.front() << ", back - " << v.back() << nl;

        // reset stopwatch
        sw.reset(); 

        #if defined(_MSC_VER) || defined(__ICL)
            std::sort(std::execution::par_unseq, v.begin(), v.end());
        #else
            std::sort(v.begin(), v.end());
        #endif 

        stream <<"After sort: front - " << v.front() << ", back - " << v.back() 
            << ", Elapsed: " << sw.elapsed_time() << nL;
    }

    stream << flush;
}

int main()
{
    examples_for_parallel_sort_algorithm();
}