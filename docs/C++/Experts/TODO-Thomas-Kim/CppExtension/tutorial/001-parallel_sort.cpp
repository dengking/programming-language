#include <tpf_chrono_random.hpp>
#include <tpf_output.hpp>



tpf::sstream stream;
auto nl = "\n";
auto nL = "\n\n";
auto flush = tpf::output::nl();

void examples_for_random_stopwatch()
{
    // STEP 1. define namespace alias
    // and some types we will be using

    namespace crd = tpf::chrono_random;

    using element_t = int;
    using container_t = std::vector<element_t>;

    // STEP 2. create a random number generator
    // we create a random number generator
    // that generates type element_t (or double)
    // range of generated doubles are from 1 to 100
    auto generator = crd::random_generator<element_t>(1, 20); // from 1 to 20 inclusive

    // STEP 3. create an object of type container_t, std::vector<double>
    container_t v;

    // STEP 4. fill the container with random numbers
    // using the random number generator created at STEP 2.
    // we generate 20 elements of type element_t, or double
    crd::random_fill(v, generator, 10); // 10 elements of type element_t

    stream <<"Random numbers : " << v << nl;
    
    // if you don't end stream with flush here
    // you cannot see your result. The reason is for optimization
    stream << v.size() 
        << " random numbers are generated" << flush;
}

template<typename ContainerType, typename StopwatchType>
void print_constainer(size_t index, 
    const ContainerType& container,
    const StopwatchType& sw)
    {
        stream << index <<"- Minimum: " << container.front()
                        << ", Maximum: " << container.back()
                        << ", Elapsed: " << sw.elapsed_time() << nl;
    }

void examples_for_serial_sort()
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

    stream << "========== Serial Sort ==========" << nL;
    for(size_t i = 0; i < test_count; ++i)
    {
        auto sorted_container = v;
        sw.reset();
        std::sort(sorted_container.begin(), sorted_container.end());
        print_constainer(i, sorted_container, sw);
    }

    // if you don't flush, you cannot see anything
    stream << flush;
}

int main()
{
    // examples_for_random_stopwatch();
    examples_for_serial_sort();
}