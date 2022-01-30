#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

/*
    We will learn how to pass unique_ptr by reference.
    Then, we will learn how to return unique_ptr from a function,
    then from a member function of a class.
 */

void take_unique_ptr_by_reference(const std::unique_ptr<int>& ptr)
{
    stream << "The value of the object the ptr is pointing to = " << *ptr << endl;
}

void test_pass_unique_ptr_by_reference()
{
    auto ptr = std::make_unique<int>(10);

    // we are passing unique_ptr ptr to a function by reference
    take_unique_ptr_by_reference(ptr); 

    // since we passed ptr by reference,
    // ptr is still valued.

    if(ptr)
    {
        stream << "Ptr is still valid: " << *ptr << endl;
    }
    else
    {
        stream << "Ptr is invalid" << endl;
    }
}

class DummyClass
{
    public:
        std::unique_ptr<int> m_ptr;

    public:

    DummyClass(int value = 0): 
        m_ptr{ std::make_unique<int>(value) } { }

    // this function returns reference to a member unique_ptr
    const std::unique_ptr<int>& return_unique_ptr_reference()
    {
        // since we are returning reference to m_ptr
        // this->m_ptr is valid after this function returns
        return this->m_ptr;
    }

    // this function returns class member unique_ptr
    std::unique_ptr<int> return_unique_ptr_of_member_data()
    {
        // this does not work
        // return this->m_ptr;

        // we are returning the ownership of 
        // this->m_ptr, after this call
        // this->m_ptr is invalid
        return std::move(this->m_ptr);

        // WARNING: after move, this->m_ptr is invalid
    }

    // returns local instance of unique_ptr
    // this function returns local instance of
    // unique_ptr
    std::unique_ptr<int> return_unique_ptr(int value)
    {
        auto ptr = std::make_unique<int>(value);

        return ptr;
    }

};

void test_return_unique_ptr_reference()
{
    DummyClass dummy{10};

    // constness is preserved

    auto& ptr1 = dummy.return_unique_ptr_reference();

    decltype(auto) ptr2 = dummy.return_unique_ptr_reference();

    stream <<"The type of ptr1: "
        << Tpf_GetTypeCategory(ptr1) << endl;

    stream << "The value of ptr1: "
        << *ptr1 << endl;

    stream <<"The type of ptr2: "
        << Tpf_GetTypeCategory(ptr2) << endl;

    stream << "The value of ptr2: "
        << *ptr2 << endl;
}

std::unique_ptr<int> make_int_ptr(int value)
{
    auto ptr = std::make_unique<int>(value);
    return ptr;
}

// never do this, this does not work
// it does not work. You are causing trouble by returning local variable
std::unique_ptr<int>&& return_rvalue_ref_unique_ptr(int value)
{
    auto ptr = std::make_unique<int>(value);

    return std::move(ptr); // std::move(ptr) is not the cause the problem.
}

void test_member_function_returning_unique_ptr()
{
    DummyClass dummy{10};

    auto ptr = dummy.return_unique_ptr(10);

    stream <<"the value of the object ptr is pointing to: "
        << *ptr << endl;
}

void test_member_function_returning_member_unique_ptr()
{
    DummyClass dummy{10};

        // return_unique_ptr_of_member_data()
        // moves the ownership of m_ptr to ptr
        // after this call, dummy.m_ptr is invalid
    auto ptr = dummy.return_unique_ptr_of_member_data();

    stream << "the value of the object ptr is pointing to: "
        << *ptr << endl;

    if(dummy.m_ptr)
    {
        stream << "dummy.m_ptr is still valid: "
            << *dummy.m_ptr << endl;
    }
    else
    {
        stream <<"dummy.m_ptr is invalid" << endl;
    }
    
}

// never do this, this does not work
// it does not work. You are causing trouble by returning local variable
// std::unique_ptr<int>& return_lvalue_ref_unique_ptr(int value)
// {
//     auto ptr = std::make_unique<int>(value);

//     return std::move(ptr); // std::move(ptr) is not the cause the problem.
// }


void get_unique_ptr_from_a_function()
{
    auto p = make_int_ptr(20);

    stream <<"The value of the object the ptr p is pointing to: " 
        << *p << endl;
}

void cause_of_trouble()
{
    auto p = return_rvalue_ref_unique_ptr(10);

    stream <<"*p is " << *p << endl;
} 

// void cause_of_trouble_with_reference()
// {
//     auto& p = return_lvalue_ref_unique_ptr(10);

//     stream <<"*p is " << *p << endl;
// } 

int main()
{
    // test_pass_unique_ptr_by_reference();

    // get_unique_ptr_from_a_function();

    // cause_of_trouble();

    // cause_of_trouble_with_reference();

    // test_member_function_returning_unique_ptr();

    // test_member_function_returning_member_unique_ptr();

    test_return_unique_ptr_reference();
}