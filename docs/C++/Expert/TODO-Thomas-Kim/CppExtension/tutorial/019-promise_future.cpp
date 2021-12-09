#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

// thread function
void division(std::promise<int>& promise, int a, int b)
{
    try
    {
        if(b == 0)
        {
            Tpf_ThrowDebugException("Division By Zero");
        }
        else
        {
            // this is return value of division()
            promise.set_value(a / b);
        }
    }
    catch(...)
    {
        promise.set_exception( std::current_exception() );
    }
}

/*
    This is the canonical use of std::promise, std::future, std::thread.

    The C++2a Standard will introduce coroutine, which is based on
    std::promise, std::future, std::thread.

    If you have concrete understanding of these things now,
    you are prepared for your future.

    Also, std::async, std::packaged_task are also
    implemented using std::promise, std::future, std::thread.

    Without having solid understanding of these fundamentals,
    all your efforts for C++ Multithreading will turn out in vain. 
*/
void examples_for_promise_future()
{
    using return_t = int;
    
    // by declaring an instance of promise,
    // this promise dynamically allocates memory on free-store,
    // this memory is shared among different threads.
    // this memory is called "shared state"
    std::promise<return_t> promise;

    // promise.get_future() returns an instance of future<return_t>,
    // which has pointer to the shared memory dynamically allocated by std::promise<return_t>
    // so, std::promise<return_t> and std::future<return_t> share "shared state"
    // std::future<return_t> future = promise.get_future();
    auto future = promise.get_future();

    // we create a thread that runs immediately.
    std::thread t { division, std::ref(promise), 8, 2};

    t.detach(); // by detaching an instance of std::thread,
                // the thread function, division() runs in the background thread.

    std::promise<return_t> failed_promise;
    auto failed_future = failed_promise.get_future();

    std::thread failed_thread { division, std::ref(failed_promise), 8, 0};
    failed_thread.detach(); // now the thread runs in the background

    try
    {
        stream << "8 / 2 =  " << future.get() << endl;

        stream << "8 / 0 = " << failed_future.get() << endl;
    }
    catch(const std::exception& e)
    {
        stream << e << endl;
    }

}

int main()
{
    examples_for_promise_future();
}