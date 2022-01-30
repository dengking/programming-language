#include <tpf_output.hpp>
#include <future>
#include <thread>
#include <condition_variable>
#include <atomic>

constexpr int max_count = 12;
constexpr int max_players = 3; // we will create 3 threads

int whose_turn = 0;
std::atomic<int> count{0};

std::mutex mutex; // global mutex
std::condition_variable condition_variable; // global condition variable


// this is thread function
void worker(const char* name, int my_turn)
{
    // if we do not use, std::atomic, it is incorrect
    // by using std::atomic<int> count 
    while(count < max_count)
    {
        // unique_lock owns the global mutex
        std::unique_lock<std::mutex> unique_lock(mutex);
        
        condition_variable.wait(unique_lock, 
            [my_turn] { return (my_turn == whose_turn) 
                      || (count >= max_count) ; });

        if(count < max_count)
        {
            tpf::sstream stream;

            stream << count << ", " << name << " [ " 
                << std::this_thread::get_id() << " ]" << tpf::endl;
            
            ++count; ++whose_turn;

            if(whose_turn >= max_players)
                whose_turn = 0;

            // notify other threads to wakeup
            condition_variable.notify_all();
        }
        else
        {
            // notify other threads to wakeup
            condition_variable.notify_all();
            break;
        }
    }
}

void example_for_synchronization()
{
    try
    {
        // std::async() returns a std::future<>;
        auto f0 = std::async(std::launch::async, // start thread immediately
                                // if fails, it throws std::system_error
            worker, // thread function
            "A", 0 // worker's arguments
        );

        auto f1 = std::async(std::launch::async, worker, "B", 1);
        auto f2 = std::async(std::launch::async, worker, "C", 2);

        f0.get(); // wait until the thread ends (or returns)
        f1.get();
        f2.get();
    }
    catch(const std::system_error& e)
    {
        tpf::sstream stream;
        stream << e << tpf::endl;
    }

}

int main()
{
    example_for_synchronization();
}
