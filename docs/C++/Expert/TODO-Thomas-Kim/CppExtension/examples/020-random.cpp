#include <tpf_chrono_random.hpp>
#include <tpf_output.hpp>

// cl /EHsc /std:c++latest /O2 020-random.cpp /Fem.exe
// icl /EHsc /std:c++latest /O2 020-random.cpp /Fem.exe
// g++ -std=c++2a -fopenmp -D_GLIBCXX_PARALLEL -O3 020-random.cpp -o g.exe
// clang++ -std=c++2a -O3 020-random.cpp -o c.exe

#if defined(_MSC_VER) || defined(__ICL)
    #include <execution> // as of today, April 28, 2019
                     // only MSVC version 12.7 implements C++2a Parallel Algorithm
                     // possibly latest version of INTEL C++ compiler too
                     // GNU g++ version 9.0 /clang++ version 9.0 will be
                     // released very soon, then we can try this program too.
#else
    #include <parallel/algorithm>
#endif

tpf::sstream stream;
auto nl = "\n";
auto nL = "\n\n";
auto flush = tpf::output::nl();

template<typename ContainerType, typename StopwatchType>
void print_container(size_t index, 
    const ContainerType& container,
    const StopwatchType& sw, bool bShow=true)
    {
        stream << index;

        if(bShow) 
            stream <<" : " << container << nl;
        
        stream << "- Minimum: " << container.front()
               << ", Maximum: " << container.back()
               << ", Elapsed: " << sw.elapsed_time() << nL;
    }

void examples_for_parallel_sort()
{
    // STEP 1. define namespace alias
    // and some types we will be using

    namespace crd = tpf::chrono_random;

    using element_t = double;
    using container_t = std::vector<element_t>;

    // STEP 2. create a random number generator
    // we create a random number generator
    // that generates type element_t (or double)
    // range of generated doubles are from 1 to 100
    auto generator = crd::random_generator<element_t>(1, 100); // from 1 to 100 inclusive

    // STEP 3. create an object of type container_t, std::vector<double>
    container_t v;

    size_t size = 1'000'000;
    size_t test_count = 5;

    // STEP 4. fill the container with random numbers
    // using the random number generator created at STEP 2.
    // we generate 20 elements of type element_t, or double
    crd::random_fill(v, generator, size); // 10 elements of type element_t

    crd::stop_watch sw;

    auto generators = generator.clone(test_count);

    stream << "========== Parallel Sort ==========" << nL;
    for(size_t i = 0; i < test_count; ++i)
    {
        container_t sorted_container;
        crd::random_fill(sorted_container, generators[i], size);
        
        if(sorted_container.size() < 11)
            stream <<i<<" : " << sorted_container << nl;

        sw.reset();
        
        #if defined(_MSC_VER) || defined(__ICL)
            std::sort(std::execution::par_unseq, sorted_container.begin(), sorted_container.end());
        #elif defined(_GLIBCXX_PARALLEL)
            __gnu_parallel::sort(sorted_container.begin(), sorted_container.end());
        #else
            std::sort(sorted_container.begin(), sorted_container.end());
        #endif
            print_container(i, sorted_container, sw, sorted_container.size() < 11);
    }

    // if you don't flush, you cannot see anything
    stream << flush;
}

int main()
{
    examples_for_parallel_sort();
}