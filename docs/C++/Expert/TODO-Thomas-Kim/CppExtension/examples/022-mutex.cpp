#include <tpf_output.hpp>
#include <future>
#include <condition_variable>
#include <thread>

constexpr int max_count = 12;
constexpr int max_players = 3;

int whose_turn = 0;
std::atomic<int> count{0};

std::mutex mutex;
std::condition_variable condition_variable;

void worker(const char* name, int my_turn)
{
    while(count < max_count)
    {
        std::unique_lock<std::mutex> unique_lock(mutex);
        condition_variable.wait(unique_lock, 
            [my_turn]{ return (my_turn == whose_turn) || (count >= max_count); });

        if(count < max_count)
        {
            tpf::sstream stream;
            stream << count <<", " << name <<" [" 
                << std::this_thread::get_id()<<"]"<<tpf::endl;

            ++count; ++whose_turn;

            if(whose_turn == max_players)
                whose_turn = 0;

            condition_variable.notify_all();
        }
        else
        {
            condition_variable.notify_all();
            break;
        }
    }
}

void example_for_synchronization()
{
    try
    {
        auto f0 = std::async(std::launch::async, // start thread immediately,
                                                // if fails to launch the thread,
                                                // it throws std::system_error
            worker, // thread routine
            "A", 0 /* arguments for worker routine */);

        auto f1 = std::async(std::launch::async, worker, "B", 1);
        auto f2 = std::async(std::launch::async, worker, "C", 2);

        f0.get(); // wait until f0 ends
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