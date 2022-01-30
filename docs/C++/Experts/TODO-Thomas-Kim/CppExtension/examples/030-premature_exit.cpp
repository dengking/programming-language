#include "030-premature_exit.hpp"

tpf::sstream stream;
auto endl = tpf::endl;

void serial_algorithm()
{
    size_t count = 10;

    auto generator = tpf::chrono_random::random_generator<int>(0, 10);

    std::vector<int> v; v.reserve(count);

    for(size_t i = 0; i < count; ++i)
        v.emplace_back(generator());

    stream << v << endl;
    std::atomic<bool> failed(false);

    std::transform(std::execution::par_unseq, v.begin(), v.end(), v.begin(), 
        [&generator, &failed](auto& e)
        { 
            auto rlt = e;
            
            try
            {
                rlt = tpf::safe_type::safe_div(e, generator());
                
            }
            catch(std::exception&)
            {
                // stream << e << endl;
                failed = true;
            }

            return rlt;
        }
        
        );

    stream << "Okay done " << failed << endl;
}

template<typename Policy, typename IteratorBegin, typename IteratorEnd, typename Callback>
bool my_for_each(Policy&& policy, IteratorBegin&& begin, IteratorEnd&& end, Callback&& callback)
{
    std::atomic<bool> operation_result{true};

    std::for_each(std::forward<Policy>(policy),
        std::forward<IteratorBegin>(begin),
        std::forward<IteratorEnd>(end),
        [&callback, &operation_result](auto&& e)
        {
            try
            {
                callback(std::forward<decltype(e)>(e));
            }
            catch(...)
            {
                operation_result = false;
            }
        });

    return operation_result;
}


void safe_serial_algorithm()
{
    size_t count = 10;

    auto generator = tpf::chrono_random::random_generator<int>(0, 10);

    std::vector<int> v; v.reserve(count);

    for(size_t i = 0; i < count; ++i)
        v.emplace_back(generator());

    // tpf::sstream stream;
    // auto endl = tpf::endl;

    auto result = 
        my_for_each(std::execution::par_unseq, v.begin(), v.end(),
            [&generator](auto& e)
            { 
                e = tpf::safe_type::safe_div(e, generator());
            });

    if(result)        
    {
        stream << "Okay successful!" << endl;
    }
    else
    {
        stream << "Operation failed" << endl;
    }
}

template<typename Policy, typename IteratorBegin, typename IteratorEnd, typename Callback>
void safe_for_each(Policy&& policy, IteratorBegin&& begin, IteratorEnd&& end, Callback&& callback)
{
    std::exception_ptr exception_ptr{nullptr};

    std::for_each(std::forward<Policy>(policy),
        std::forward<IteratorBegin>(begin),
        std::forward<IteratorEnd>(end),
        [&callback, &exception_ptr](auto&& e)
        {
            try
            {
                callback(std::forward<decltype(e)>(e));
            }
            catch(...)
            {
                exception_ptr = std::current_exception();
            }
        });

    if(exception_ptr) std::rethrow_exception(exception_ptr);
}

template<typename Policy, typename IteratorBegin, typename IteratorEnd, typename Callback>
void go_parallel(Policy&& policy, IteratorBegin&& begin, IteratorEnd&& end, Callback&& callback)
{
    std::exception_ptr exception_ptr{nullptr};

    std::for_each(std::forward<Policy>(policy),
        std::forward<IteratorBegin>(begin),
        std::forward<IteratorEnd>(end),
        [&callback, &exception_ptr](auto&& e)
        {
            try
            {
                callback(std::forward<decltype(e)>(e));
            }
            catch(...)
            {
                exception_ptr = std::current_exception();
            }
        });

    if(exception_ptr) std::rethrow_exception(exception_ptr);
}

void safe_parallel_algorithm()
{
    size_t count = 10;

    auto generator = tpf::chrono_random::random_generator<int>(0, 10);

    std::vector<int> v; v.reserve(count);

    for(size_t i = 0; i < count; ++i)
        v.emplace_back(generator());

    stream << "Before: " << v << endl;

    try
    {
        safe_for_each(std::execution::par_unseq, v.begin(), v.end(),
            [&generator](auto& e)
            { 
                e = tpf::safe_type::safe_div(e, generator());
            });

        stream << "After: " << v << endl;
    }
    catch(const std::exception& e)
    {
        stream << "---- Operation Failed with Exception ----" << endl;

        stream << e << endl;
    }
}

void safe_parallel_algorithm_bool()
{
    size_t count = 10;

    auto generator = tpf::chrono_random::random_generator<int>(0, 10);

    std::vector<int> v; v.reserve(count);

    for(size_t i = 0; i < count; ++i)
        v.emplace_back(generator());

    stream << "Before: " << v << endl;
    
    auto callback = [&generator](auto& e)
        { 
            e = tpf::safe_type::safe_div(e, generator());
        };

    Tpf_GoParallel(success, std::for_each, callback,
        std::execution::par_unseq, v.begin(), v.end());
        
    if(success)
    {
        stream << "After: " << v << endl;
    }
    else
    {
        stream <<"Operaion failed" << endl;
    }
}

void safe_parallel_algorithm_return()
{
    size_t count = 10;

    auto generator = tpf::chrono_random::random_generator<int>(0, 10);

    std::vector<int> v; v.reserve(count);

    for(size_t i = 0; i < count; ++i)
        v.emplace_back(generator());

    stream << "Before: " << v << endl;
    
    auto find_value = v.back();

    auto callback = [&find_value](auto& e)
        { 
            return find_value == e;
        };

    Tpf_GoParallel_Return(success, itr, std::find_if, callback,
        std::execution::par_unseq, v.begin(), v.end());
        
    if(success)
    {
        stream << "Found: " << *itr << endl;
    }
    else
    {
        stream <<"Operaion failed" << endl;
    }
}


void safe_parallel_algorithm_throw()
{
    size_t count = 10;

    auto generator = tpf::chrono_random::random_generator<int>(0, 10);

    std::vector<int> v; v.reserve(count);

    for(size_t i = 0; i < count; ++i)
        v.emplace_back(generator());

    stream << "Before: " << v << endl;
    
    auto callback = [&generator](auto& e)
        { 
            e = tpf::safe_type::safe_div(e, generator());
        };

    try
    {
        Tpf_GoParallel_Throw(std::for_each, callback,
            std::execution::par_unseq, v.begin(), v.end());

        stream << "After: " << v << endl;
    }
    catch(const std::exception& e)
    {
        stream << "\n" << e << endl;
    }   
    
}


int main()
{
    serial_algorithm();
    safe_serial_algorithm();
    safe_parallel_algorithm();

    safe_parallel_algorithm_bool();
    safe_parallel_algorithm_throw();

    safe_parallel_algorithm_return();

 
}