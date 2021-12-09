#include <tpf_output.hpp>
#include <tbb/tbb.h>
#include <thread>
#include <mutex>

tpf::sstream  stream;
auto endl = tpf::endl;

void test_threading_building_blocks()
{
    std::mutex mutex;
    using lock_type = std::lock_guard<std::mutex>;

    auto callback = [&mutex](auto index)
    {
        lock_type lock(mutex);

        stream <<"Thread ID: " << std::this_thread::get_id()
            << " - Index: " << index << endl;

    };

    tbb::parallel_for(1, 11, callback);

}

int main()
{
    stream <<"Hello, Intel Threading Building Blocks on Windows!" 
        << tpf::endl;

    test_threading_building_blocks();
}