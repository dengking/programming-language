#include <iostream>
#include <type_traits>
#include <thread>
#include <mutex>
#include <stack>

struct pod_type
{
    int m;
};

class aggregate_type
{
    public:
    int m;
};

void test_nonthrowing_copy_move_constructors_and_assignment()
{
    std::cout << std::boolalpha;

    std::cout << "Does a primitive type support a nonthrowing copy constructor? "
        << std::is_nothrow_copy_constructible_v<int> << std::endl;

    std::cout << "Does a primitive type support a nonthrowing move constructor? "
        << std::is_nothrow_move_constructible_v<int> << std::endl;

     std::cout << "Does a primitive type support a nonthrowing copy assignment? "
        << std::is_nothrow_copy_assignable_v<int> << std::endl;

     std::cout << "Does a primitive type support a nonthrowing move assignment? "
        << std::is_nothrow_move_assignable_v<int> << std::endl << std::endl;

    ///////////////////////////////
    std::cout << "Does a pod type support a nonthrowing copy constructor? "
        << std::is_nothrow_copy_constructible_v<pod_type> << std::endl;

    std::cout << "Does a pod type support a nonthrowing move constructor? "
        << std::is_nothrow_move_constructible_v<pod_type> << std::endl;

     std::cout << "Does a pod type support a nonthrowing copy assignment? "
        << std::is_nothrow_copy_assignable_v<pod_type> << std::endl;

     std::cout << "Does a pod type support a nonthrowing move assignment? "
        << std::is_nothrow_move_assignable_v<pod_type> << std::endl << std::endl;

    ///////////////////////////////
    std::cout << "Does an aggregate type support a nonthrowing copy constructor? "
        << std::is_nothrow_copy_constructible_v<aggregate_type> << std::endl;

    std::cout << "Does an aggregate type support a nonthrowing move constructor? "
        << std::is_nothrow_move_constructible_v<aggregate_type> << std::endl;

     std::cout << "Does an aggregate type support a nonthrowing copy assignment? "
        << std::is_nothrow_copy_assignable_v<aggregate_type> << std::endl;

     std::cout << "Does an aggregate type support a nonthrowing move assignment? "
        << std::is_nothrow_move_assignable_v<aggregate_type> << std::endl << std::endl;
}

class NonThrowingMovable
{
    private:
        bool m_invalid{false};

        void cleanup()
        {
            if(this->m_invalid == false)
                std::cout << "Destructor cleaned up" << std::endl;
        }
    public:
        NonThrowingMovable()
        {
            std::cout << "Default constructor called" << std::endl;
        }

        NonThrowingMovable(int v)
        {
            std::cout << "NonThrowingMovable(int v) called" << std::endl;
        }

        NonThrowingMovable(const NonThrowingMovable& rhs)
        {
            std::cout << "Copy constructor called" << std::endl;
        }

        NonThrowingMovable(NonThrowingMovable&& rhs) noexcept
        {
            std::cout << "Move constructor called" << std::endl;
            rhs.m_invalid = true;
        }

        NonThrowingMovable& operator=(const NonThrowingMovable& rhs)
        {
            if(this != std::addressof(rhs))
            {

            }

            std::cout << "Copy assignment called" << std::endl;

            return *this;
        }

        NonThrowingMovable& operator=(NonThrowingMovable&& rhs) noexcept
        {
            if(this != std::addressof(rhs))
            {
                rhs.m_invalid = true;
            }

            std::cout << "Move assignment called" << std::endl;
            
            return *this;
        }

        ~NonThrowingMovable()
        {
            cleanup();
        }
};

template<typename ElementType>
class thread_safe_stack
{
    private:
        std::stack<ElementType> m_data;
        mutable std::mutex m_mutex;

    public:
        thread_safe_stack()
        {  }

        // void push(ElementType value)
        // {
        //     std::lock_guard<std::mutex> lock(m_mutex);
        //     m_data.push(std::move(value));
        // }

        template<typename... Types>
        void push(Types&&... args)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_data.emplace(std::forward<Types>(args)...);
        }

        // no expensive copy constructor of ElementType,
        // no copy assignment
        // no move assignment
        // named return value optimization kicks in
        // only one move constructor, which does not allocate dynamic memory.
        ElementType pop()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_data.empty()) throw std::exception{};

            // the real return statement occurs at this point.
            //
            // what if the type ElementType does not support MOVE constructor?
            // Luckily, most of modern C++ Standard container classes
            // support NON-THROWING MOVE constructor.
            // Also, primitive types and POD, aggregate types also support 
            // non-throwing move constructor. So the chances of
            // failure of the following line of code is very slim.

            // Even if ElementType does not support move constructor,
            // such that copy constructor kicks in, and the dynamic allocation
            // failed, this pop() function still holds thanking to 
            // Named Return Value Optimization (and Copy Elision in some cases)
            auto named_return_value = std::move(m_data.top());

            // if an exception is thrown while copy-constructing
            // named_return_value, "The Invariant of m_data"
            // still maintained.

            std::cout << "The address of named_return_value: "
                << & named_return_value << std::endl;

            // because, this m_data.pop() will not executed in the event of exception.
            m_data.pop();

            return named_return_value;

            // When Named Return Value Optimization kicks in,
            // the object name in the return statements only
            // informs the C++ compiler of the object that is 
            // designated as return value.
        }

        // this function is redundant
        bool empty()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_data.empty();
        }
};

void test_optimal_thread_safe_stack()
{
    thread_safe_stack<NonThrowingMovable> stack;

    stack.push(1);

    auto top = stack.pop();

    std::cout << "The address of top: " << &top << std::endl;

}

int main()
{
    // test_nonthrowing_copy_move_constructors_and_assignment();

    test_optimal_thread_safe_stack();
}