#include <tpf_output.hpp>
#include <thread>
#include <exception>

#ifdef __FUNCSIG__
    #define Ugly_Function_Name __FUNCSIG__
#else
    #define Ugly_Function_Name __PRETTY_FUNCTION__
#endif 

/*
    We are going to extend C++ standard class using inheritance.
    When we extend our class using inheritance, we should especially careful of "slicing problem."

    For less experience C++ programmers, this "slicing problem" is one of the major cause of resource leak.

    If the base class does not have virtual destructor,
        We should NOT add data field to our custom class, otherwise we cannot avoid memory leak.

    Only when the base class has virtual destructor,
        we can safely add data field to our derived class.
*/

void test_virtual_destructor()
{
    tpf::sstream stream;

    stream <<"Has std::exception virtual destructor ? "
            << std::has_virtual_destructor_v< std::exception > << tpf::endl;

    stream << "Has std::vector<int> virtual destructor ? "
            << std::has_virtual_destructor_v< std::vector<int> > << tpf::endl;
}

/*
    Since std::exception has a virtual destructor,  we can safely add
    data fields to the derived class.
    In case of std::vector, it does not have a virtual destructor,
    so when we add data fields to the derived class from std::vector,
    we have to extra careful about memory leaks due to "slicing problem."
    This slicing problem occurs only when we dynamically allocate.
*/

class exception_debug: public std::exception
{
    private:
        std::string m_message;
        int m_lineno;
        std::string m_function_name;
        std::string m_file_name;
        std::string m_what_msg;

    public:
        exception_debug(std::string message = "",
        int lineno = 0, std::string function_name ="",
        std::string file_name = ""):
            m_message{message}, m_lineno{lineno}, 
            m_function_name{function_name}, m_file_name{file_name}
            {
                std::ostringstream os;

                os << "exception_debug - file [" << this->m_file_name << "] \n";
                os << "thread id [" << std::this_thread::get_id() << "] - ";
                os << "line number [" << this->m_lineno << "] - ";
                os << "function [" << this->m_function_name << "]\n";
                os << "message: " << this->m_message;

                this->m_what_msg = os.str();
            }

        // this is C++98 Standard, but C++11 Standard has changed.
        virtual const char* what() const noexcept override
        {
            // we can return const char*, because this->m_what_msg outlives
            // this function block scope
            return this->m_what_msg.c_str();
        }
};

// do not put semi colon in macro definition
#define UglyNamed_ThrowException(message) throw exception_debug(message, __LINE__, Ugly_Function_Name, __FILE__)

int division( int a, int b)
{
    if(b == 0)
    {
        UglyNamed_ThrowException("b is zero");
    }

    return a  / b;
}

void example_for_throw_debug_exception()
{
    try
    {
        division(5, 0);
        tpf::sstream stream;

        stream <<"Operation successful"<<tpf::endl;
    }
    catch(std::exception& e)
    {
        tpf::sstream stream;
        stream << e << tpf::endl;
    }
}
int main()
{
    // test_virtual_destructor();
    example_for_throw_debug_exception();
}