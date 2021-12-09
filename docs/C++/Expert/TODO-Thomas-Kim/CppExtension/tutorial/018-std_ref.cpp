#include <tpf_output.hpp>

/*
    We will first learn how to use std::ref().
*/

tpf::sstream stream;
auto endl = tpf::endl;

template<typename Type>
void increment(Type n)
{
    ++n;
}

template<typename Type>
void decrement(Type&& n)
{
    --n;
}

void examples_for_std_ref()
{
    int n = 5;

    stream << "Before calling increment(n): " << n << endl;
    increment(n);
    stream << "After calling increment(n): " << n << endl;

    stream << "Before calling increment<int&>(n): " << n << endl;
    increment<int&>(n);
    stream << "After calling increment<int&>(n): " << n << endl;

    stream << "Before calling increment(std::ref(n)): " << n << endl;
    increment(std::ref(n));
    stream << "After calling increment(std::ref(n)): " << n << endl;

    // thread t runs immediately
    std::thread t{ increment<int>, n};

    t.join(); // wait until thread t returns

    stream << "After the thread t finished: " << n << endl;

     // thread s runs immediately
    std::thread s{ increment<int&>, std::ref(n)};

    s.join(); // wait until thread t returns

    stream << "After the thread s finished: " << n << endl;
    
    /*
        But most of C++ Standard Library functions are 
        built using forwarding references.
    */

    // thread u runs immediately
    std::thread u{ decrement<int& /* template parameter argument */>, 
                std::ref(n) // function-call argument
        };

    u.join(); // wait until thread t returns

    stream << "After the thread u finished: " << n << endl;

    // std::thread v { [](auto& arg){ return arg *= arg; }, std::ref(n)};

    // this works in C++2a Standard, as of today, May 25, 2019
    // only GNU g++ compiler supports
    // g++ -std=c++2a 018-std_ref.cpp
    // std::thread v { []<typename Type>(Type&& arg) { return arg *= arg;},
    //         // C++2a Standard will introduce template parameter specification
    //         // at lambda expression, https://en.cppreference.com/w/cpp/language/lambda
    //      std::ref(n)};

    //  v.join(); // wait until thread v returns

    // stream << "After the thread v finished: " << n << endl;

    // thread w runs immediately
    std::thread w{ [](auto&& arg){ return arg *= arg; }, 
                std::ref(n) // function-call argument
        };

    w.join(); // wait until thread t returns

    stream << "After the thread w finished: " << n << endl;

}

template<typename Type>
class reference_wrapper
{
    private:
        Type *m_ptr;

    public:

        template<typename UType>
        reference_wrapper(UType&& val)
        {
            Type& lvalue = std::forward<UType>(val);

            // this->m_ptr = &lvalue; 
            // I will talk more about std::addressof() in some future sessions
            // it returns the address of object (or lvalue)
            this->m_ptr = std::addressof(lvalue);
        }

        operator Type&() const noexcept
        { return *this->m_ptr; }

        Type& get() const noexcept
        { return *this->m_ptr; }
};

template<typename Type>
reference_wrapper<Type> ref(Type& val) noexcept
{
    return reference_wrapper<Type>(val);
}

void examples_for_custom_ref()
{
    int n = 5;

    stream << "Before calling increment(n): " << n << endl;
    increment(n);
    stream << "After calling increment(n): " << n << endl;

    stream << "Before calling increment<int&>(n): " << n << endl;
    increment<int&>(n);
    stream << "After calling increment<int&>(n): " << n << endl;

    stream << "Before calling increment(std::ref(n)): " << n << endl;
    increment(ref(n));
    stream << "After calling increment(std::ref(n)): " << n << endl;

    // thread t runs immediately
    std::thread t{ increment<int>, n};

    t.join(); // wait until thread t returns

    stream << "After the thread t finished: " << n << endl;

     // thread s runs immediately
    std::thread s{ increment<int&>, ref(n)};

    s.join(); // wait until thread t returns

    stream << "After the thread s finished: " << n << endl;
    
    /*
        But most of C++ Standard Library functions are 
        built using forwarding references.
    */

    // thread u runs immediately
    std::thread u{ decrement<int& /* template parameter argument */>, 
                ref(n) // function-call argument
        };

    u.join(); // wait until thread t returns

    stream << "After the thread u finished: " << n << endl;

    // std::thread v { [](auto& arg){ return arg *= arg; }, std::ref(n)};

    // this works in C++2a Standard, as of today, May 25, 2019
    // only GNU g++ compiler supports
    // g++ -std=c++2a 018-std_ref.cpp
    // std::thread v { []<typename Type>(Type&& arg) { return arg *= arg;},
    //         // C++2a Standard will introduce template parameter specification
    //         // at lambda expression, https://en.cppreference.com/w/cpp/language/lambda
    //      std::ref(n)};

    //  v.join(); // wait until thread v returns

    // stream << "After the thread v finished: " << n << endl;

    // thread w runs immediately
    std::thread w{ [](auto&& arg){ return arg *= arg; }, 
                ref(n) // function-call argument
        };

    w.join(); // wait until thread t returns

    stream << "After the thread w finished: " << n << endl;

}


int main()
{
    // examples_for_std_ref();
    examples_for_custom_ref();
}