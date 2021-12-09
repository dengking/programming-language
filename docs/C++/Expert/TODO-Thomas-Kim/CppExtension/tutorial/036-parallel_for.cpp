#include <iostream>
#include <mutex>
#include <thread>
#include <execution> // for C++ Standard Parallel Algorithm

#if defined(_MSVC_LANG)

    #include <ppl.h> // for Microsoft Parallel Pattern Library
    namespace ccy = concurrency; // ccy is short for namespace concurrency
#else // for GNU g++ / clang++ / Intel Compilers

    #include <tbb/tbb.h> // for Intel's Threading Building Blocks.
    namespace ccy = tbb; // ccy is short name for Threading Building Blocks
#endif

void test_parallel_for()
{
    std::mutex mutex;
    using lock_type = std::lock_guard<std::mutex>;

    auto callback = [&mutex](auto index)
    {
        lock_type lock(mutex);

        std::cout << "Thread ID: " << std::this_thread::get_id() 
            << "\tIndex = " << index << std::endl;
    };

    ccy::parallel_for(1, 11, callback);
}

int main()
{
    test_parallel_for();
}
