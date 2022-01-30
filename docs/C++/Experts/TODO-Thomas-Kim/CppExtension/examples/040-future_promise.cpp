#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

void division(std::promise<int>& promise, int a, int b)
{
    try
    {
        if(b==0)
        {
            Tpf_ThrowDebugException("Division By Zero");
        }
        else
        {
            promise.set_value(a/b);
            return;
        }

    }catch(...)
    {
        promise.set_exception(std::current_exception());
        return;
    }
}

void example_for_promise()
{
    std::promise<int> promise;
    auto future = promise.get_future();

    // running in the background
    std::thread t{division, std::ref(promise), 8, 0};
    t.detach();

    
    try
    {
        stream << "8 / 2 = " << future.get() << endl;
    }
    catch(const std::exception& e)
    {
        stream << e << endl;
    }
}

int main()
{
    example_for_promise();
    stream << "Okay ..." << endl;
}