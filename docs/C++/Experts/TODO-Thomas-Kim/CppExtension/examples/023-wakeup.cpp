#include <tpf_output.hpp>
#include <future>
#include <thread>
#include <windows.h>
#include <atomic>
#include <map>

struct threads
{
    using id_t = int;
    using handle_t = HANDLE;

    using id_handle_t = std::pair<id_t, handle_t>;
    using active_threads_t = std::map<id_t, handle_t>;
   
    using mutex_t = std::mutex;
    using recursive_mutex_t = std::recursive_mutex;
    
    inline static mutex_t mutex;
    
    using lock_guard_t = std::lock_guard<mutex_t>;
    using unique_lock_t = std::unique_lock<mutex_t>;

    inline static std::condition_variable cv_register;
    inline static std::condition_variable cv_unregister;

    inline static std::atomic<int> kill_id{-1}; 
    inline static active_threads_t active_threads;
    
    static void WINAPI apc_wakeup(ULONG_PTR dwParam)
    { 
        tpf::sstream stream;
        stream << "APC called " << tpf::endl;
    }

    static void register_me(int worker_id)
    {
        active_threads.insert(std::make_pair(worker_id, GetCurrentThread()));
    }

    static void unregister_me(int worker_id)
    {
        auto itr = active_threads.find(worker_id);
        
        if(itr != active_threads.end())
            active_threads.erase(itr);
    }

    struct register_thread
    {
        private:
            id_t m_id;
        public:
        register_thread(int worker_id): m_id{worker_id}
        {
            unique_lock_t lock(mutex);
            threads::register_me(m_id);
            threads::cv_register.notify_all();
        }
        ~register_thread()
        {
            unique_lock_t lock(mutex);
            threads::unregister_me(m_id);
            threads::cv_register.notify_all();
        }
    };

    static void WINAPI worker(int my_id)
    {
        register_thread register_me{my_id};
        
        while(kill_id != my_id)
        {
            tpf::sstream stream;
            stream << "I am a worker thread [" 
                << std::this_thread::get_id() <<"],";
            stream << " falling into deep sleep."<<tpf::nl;

            {
                unique_lock_t unique_lock(mutex);
                stream << tpf::flush;
            }

            // std::this_thread::sleep_for(std::chrono::seconds(1));   

            if(WAIT_IO_COMPLETION == ::SleepEx(100, TRUE))
            {
                tpf::sstream stream;
                stream <<"Wakeup --- " << tpf::endl;
                break;
            }
        }

        tpf::sstream stream;
         stream << "I am a worker thread [" 
                << std::this_thread::get_id() <<"],";
        stream << " now exiting."<<tpf::nl;

        {
            unique_lock_t unique_lock(mutex);
            stream << tpf::flush;
        }
    }
};

void example_for_wakeup()
{
    try
    {
        size_t size = 0;

        auto f0 = std::async(std::launch::async, &threads::worker, 0);
        {
            threads::unique_lock_t lock(threads::mutex);

            threads::cv_register.wait(lock, [size](){ return size !=
                threads::active_threads.size(); });
           
            size = threads::active_threads.size();


        }
        auto f1 = std::async(std::launch::async, &threads::worker, 1);
        {
            threads::unique_lock_t lock(threads::mutex);

            threads::cv_register.wait(lock, [size](){ return size !=
                threads::active_threads.size(); });
           
            size = threads::active_threads.size();
        }
        auto f2 = std::async(std::launch::async, &threads::worker, 2);
        {
            threads::unique_lock_t lock(threads::mutex);

            threads::cv_register.wait(lock, [size](){ return size !=
                threads::active_threads.size(); });
           
            size = threads::active_threads.size();
        }
    
        int count = 10;

        while(count > 0)
        {
            tpf::sstream  stream;
            stream <<"I am the main thread [" << std::this_thread::get_id() 
                << "], doing my own work." << tpf::nl;

            {
                threads::unique_lock_t unique_lock(threads::mutex);
                stream << tpf::flush;
            }

            auto hHandle = GetCurrentThread();

            std::this_thread::sleep_for(std::chrono::seconds(1));

            --count;

            if(count == 3)
            { 
                if(::QueueUserAPC(threads::apc_wakeup, threads::active_threads[0], 0))
                {
                    tpf::sstream stream;
                    stream <<count <<" - "<< 3 <<"Okay queued" << tpf::endl;
                }
                else
                {
                    tpf::sstream stream;
                    stream <<"Sorry failed to queue" << tpf::endl;
                }

                f0.wait();
            }
            else if(count == 5) 
            { 
                 if(::QueueUserAPC(threads::apc_wakeup, threads::active_threads[1], 0))
                    {
                        tpf::sstream stream;
                        stream <<count <<" - "<< 5 <<"Okay queued" << tpf::endl;
                    }
                    else
                    {
                        tpf::sstream stream;
                        stream <<"Sorry failed to queue" << tpf::endl;
                    }

                f1.wait();
            }

            else if(count == 7)
            {
                 if(::QueueUserAPC(threads::apc_wakeup, threads::active_threads[2], 0))
                {
                    tpf::sstream stream;
                    stream <<count <<" - "<< 2 <<"Okay queued" << tpf::endl;
                }
                else
                {
                    tpf::sstream stream;
                    stream <<"Sorry failed to queue" << tpf::endl;
                }

                f2.wait();
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main()
{
    example_for_wakeup();
}