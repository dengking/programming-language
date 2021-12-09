#include <iostream>
#include <tbb/tbb.h>

template<typename Type>
Type fibonacci_loop(Type n)
{
    if(n < 2) // n = 0 or 1
        return n;
    else
    {
        Type fn_2 = 0, fn_1 = 1, fn;

        for(Type i = 2; i <= n; ++i)
        {
            fn = fn_2 + fn_1;

            fn_2 = fn_1; fn_1 = fn;
        }

        return fn;
    }
}

template<typename Type>
Type fibonacci_recursion(Type n)
{
    if( n < 2)
        return n;
    else
        return fibonacci_recursion(n-2) + fibonacci_recursion(n-1);
}

template<typename Type, auto go_serial = Type{16} >
class fibonacci: public tbb::task
{
    private:
        Type m_n;
        Type* const m_fn_ptr;

        using base = tbb::task;

    public:
        fibonacci(Type n, Type* const fn_ptr): m_n{n}, m_fn_ptr{fn_ptr} { }

        base* execute() override
        {
            if( m_n < go_serial)
            {
                *m_fn_ptr = fibonacci_loop(m_n);
            }
            else
            {
                Type fn_2, fn_1;

                auto& task_n_2 = *new(base::allocate_child()) fibonacci{m_n-2, &fn_2};
                auto& task_n_1 = *new(base::allocate_child()) fibonacci{m_n-1, &fn_1};

                base::set_ref_count(3);

                base::spawn(task_n_1);
                base::spawn_and_wait_for_all(task_n_2);

                *m_fn_ptr = fn_2 + fn_1;
            }

            return nullptr;
        }
};

template<typename Type>
Type parallel_fibonacci(Type n)
{
    Type fn;
    
    auto& task = *new(tbb::task::allocate_root()) fibonacci{n, &fn};
    
    tbb::task::spawn_root_and_wait(task);
    
    return fn;
}

int main()
{
    for(int i=0; i <= 10; ++i)
    {
        std::cout << parallel_fibonacci(i) << std::endl;
    }
}