#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>

#include <cstdlib>

tpf::sstream stream;

auto nl = tpf::nl; // "\n"
auto nL = tpf::nL; // "\n\n"
auto flush = tpf::flush; // flushes the contents of the stream to std::cout
auto endl = tpf::endl; // equivalent to std::endl

using stop_watch = tpf::chrono_random::stop_watch;

// from this example, we conclude that 
template<typename ElementType>
void dynamic_allocation_test(size_t test_count, size_t element_count)
{
    stream << "-------- Dynamic Allocation Test" << nl << endl;

    stop_watch sw_external, sw_internal;
    
    std::allocator<ElementType> alloc;

    stream << "Dynamically allocate " << element_count 
        << " elements of type " << Tpf_GetTypeName(ElementType)
        <<" using std::allocator() " << nl;
    sw_external.reset();
    for(size_t i = 0; i < test_count; ++i)
    {
        auto ptr_d = (ElementType*)alloc.allocate(element_count);
        
        sw_internal.reset();
        for(size_t i = 0; i < element_count; ++i)
            ptr_d[i] = (ElementType)i;

        stream << sw_internal.elapsed_time() << " - " << ptr_d[i] << nl;

        alloc.deallocate(ptr_d, element_count);
    }
    stream << "elapsed: " << sw_external.elapsed_time() << nl << endl;
}

void benchmarking_new_vs_malloc()
{
    dynamic_allocation_test<int>(5, 1'000'000);
}

int main()
{
    std::shared_ptr<int[]> ptr = std::make_shared<int[]>((size_t)50);

    benchmarking_new_vs_malloc();
}