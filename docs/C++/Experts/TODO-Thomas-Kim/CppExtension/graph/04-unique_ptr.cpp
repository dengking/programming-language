#include <tpf_output.hpp>

/*
    Please, please understand that std::unique_ptr should be your favorite tool
    for any C++ programming.

    It has only a single data membeer in std::unique_ptr<int>, that data member is
    int*, the raw integer pointer.

    You can convert std::unique_ptr to std::shared_ptr anytime.
    About this, I will talk in future episode.

    Keep this in mind that

    auto - is copy semantic. It calls copy constructor/copy assigment
            or, copy elision can ocur.

    auto& - is lvalue reference. It can const int& or int&.
           It can map to either const lvalue reference or just lvalue reference

    auto&& - is rvalue reference. It can map to int&&. It cannot map to lvalue reference

    But decltype(auto) can map to auto, auto&, auto&&.
    So, decltype(auto) = one of auto, auto&, auto&&
 */

tpf::sstream stream;
auto endl = tpf::endl;

class DummyClass
{
    public:

    using uptr_t = std::unique_ptr<int>;

    public:
        // I declared m_member in public section
        uptr_t m_member;

        DummyClass(int value=0):
            m_member{ std::make_unique<int>(value) } { }

        // returns as rvalue
        // auto return_as_rvalue()
        std::unique_ptr<int> return_as_rvalue()
        {
            return std::move(this->m_member);
        }

        // returns as rvalue reference
        // this is correct!!!!
        // auto&& return_as_rvalue_reference()
        std::unique_ptr<int>&& return_as_rvalue_reference()
        {
            // this->m_member still exists 
            // after this function call.
            // that is, the lifetime of this->m_member
            // outlives this function block
            return std::move(this->m_member);
        }
        
        ///////////////////////////////////////
        // returns as rvalue
        // std::unique_ptr<int> return_as_rvalue_with_auto()
        auto return_as_rvalue_with_auto()
        {
            return std::move(this->m_member);
        }

        // returns as rvalue reference
        // this is correct!!!!
        // std::unique_ptr<int>&& return_as_rvalue_reference_with_auto()
        auto&& return_as_rvalue_reference_with_auto()
        {
            // this->m_member still exists 
            // after this function call.
            // that is, the lifetime of this->m_member
            // outlives this function block
            return std::move(this->m_member);
        }

        // don't make such mistake!!!
        // sadly, C++ compiler cannot catch such mistake
        // this is wrong
        std::unique_ptr<int>&& return_local_variable()
        {
            std::unique_ptr<int> local_ptr = std::make_unique<int>(20);

            // the lifetime of local_ptr ceases
            // when local_ptr goes off this block
            return std::move(local_ptr);
        }

        // we should not return local unique_ptr
        // neither as lvalue reference nor as rvalue reference
        // this is the correct way to return local instance of unique_ptr
        // in this case, the return type is concrete data type
        std::unique_ptr<int> return_local_unique_ptr_properly()
        {
            auto local_ptr = std::make_unique<int>(10);

            // this is correct!!
           // return std::move(local_ptr); // std::move() is simply a typecast
                                         // in case of this function, std::move() has no effect

            return local_ptr;

            /*
            
                when std::move is used with return statement,
                if the function's return type is decltype(auto),
                then it matters... return std::move() does matter
                if the return type of the function is declared with decltype(auto)
             */
        }
};


void understand_uncaught_return_value()
{
    DummyClass dummy { 10 };

    // we called return_as_rvalue_reference()
    // and we have NOT caught the returned value
    dummy.return_as_rvalue_reference();

    stream <<"Afer calling dummy.return_as_rvalue_reference()" << endl;

    if(dummy.m_member)
    {
        stream <<"dummy.m_member is still valid" << endl;
    }
    else
    {
        stream <<"dummy.m_member is invalid" << endl;
    }

     // we called return_as_rvalue()
    // and we have NOT caught the returned value
    dummy.return_as_rvalue();

    stream <<"Afer calling dummy.return_as_rvalue()" << endl;
    
    if(dummy.m_member)
    {
        stream <<"dummy.m_member is still valid" << endl;
    }
    else
    {
        stream <<"dummy.m_member is invalid" << endl;
    }
}

void test_function_returning_an_rvalue_reference()
{
    DummyClass dummy{10};

    // catch the return value as rvalue reference
    std::unique_ptr<int>&& p1 = dummy.return_as_rvalue_reference();

    if(dummy.m_member)
    {
        stream << "dummy.m_member is still valid" << endl;
    }
    else
    {
        stream << "dummy.m_member is invalid" << endl;
    }

    // catch the return value as rvalue
    std::unique_ptr<int> p2 = dummy.return_as_rvalue_reference();

    if(dummy.m_member)
    {
        stream << "dummy.m_member is still valid" << endl;
    }
    else
    {
        stream << "dummy.m_member is invalid" << endl;
    }
}

void misfortune()
{
    DummyClass dummy{10};

    std::unique_ptr<int> p = dummy.return_local_variable();

    stream <<"Am I okay?" << endl;
}

void the_correct_way()
{
    DummyClass dummy{10};

    std::unique_ptr<int> p = dummy.return_local_unique_ptr_properly();

    stream <<"Am I okay?" << endl;
}

int main()
{
    // understand_uncaught_return_value();

    // test_function_returning_an_rvalue_reference();

    // misfortune();

    the_correct_way();

    stream <<"Are you okay? " << endl;

}