#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

class DummyClass
{
    public:
        enum class Operation: int { as_const_lvalue_ref, as_lvalue_ref, as_move };

    private:
        std::unique_ptr<int> m_member;

    public:
        DummyClass(int value=0): 
            m_member{ std::make_unique<int>(value) } { }

    // we have to break the tie between member function get_member
    // with its enclosing class, DummyClass. How?

    // now we have broken the tie between DummyClass and get_member()
    template<auto Opr = Operation::as_const_lvalue_ref>
    decltype(auto) get_member()
    {
        // this is default return type
        if constexpr(Opr == Operation::as_const_lvalue_ref)
        {
             return static_cast<const std::unique_ptr<int>& >(this->m_member);
        }
        else if constexpr(Opr == Operation::as_lvalue_ref)
        {
            return static_cast< std::unique_ptr<int>&>(this->m_member);
        }
        else
        {
            // after this, this->m_member is invalid or valid
            // depending on its caller
            return std::move(this->m_member);
        }
    }
};

void test_advanced_return_type()
{
    DummyClass dummy{ 10 };

    constexpr auto const_lvalue_ref = DummyClass::Operation::as_const_lvalue_ref;
    constexpr auto lvalue_ref = DummyClass::Operation::as_lvalue_ref;
    constexpr auto as_move = DummyClass::Operation::as_move;
      
    decltype(auto) p1 = dummy.get_member();

    stream << "The type of p1: " << Tpf_GetTypeCategory(p1) 
        <<"\nits value is " << *p1 << endl;
    
    decltype(auto) p2 = dummy.get_member<lvalue_ref>();

    stream << "The type of p2: " << Tpf_GetTypeCategory(p2) 
        <<"\nits value is " << *p2 << endl;

    // at this point, we move dummy.m_member
    // to p3. After this call dummy.m_member is invalid
    auto p3 = dummy.get_member<as_move>();

    stream << "The type of p3: " << Tpf_GetTypeCategory(p3) 
        <<"\nits value is " << *p3 << endl;

    // at this point, p3 now has the ownership of the memory
    // that was previously owned by dummy.

    // WARNING: Since we move the ownership of dummy.m_member 
    // to p3, now p1 and p2 are invalid.

    if(p1)
    {
        stream <<"p1 is valid " << *p1 << endl;
    }
    else
    {
        stream << "p1 is invalid" << endl;
    }

    if(p2)
    {
        stream <<"p2 is valid " << *p2 << endl;
    }
    else
    {
        stream << "p2 is invalid" << endl;
    }

    // p4 is now raw pointer of type int*
    auto p4 = p3.release(); 

    if(p1)
    {
        stream <<"p1 is valid " << *p1 << endl;
    }
    else
    {
        stream << "p1 is invalid" << endl;
    }

    if(p2)
    {
        stream <<"p2 is valid " << *p2 << endl;
    }
    else
    {
        stream << "p2 is invalid" << endl;
    }

    // restore p2 with p4, the raw pointer of type int*
    p2.reset(p4); 

    if(p1)
    {
        stream <<"p1 is valid " << *p1 << endl;
    }
    else
    {
        stream << "p1 is invalid" << endl;
    }

    if(p2)
    {
        stream <<"p2 is valid " << *p2 << endl;
    }
    else
    {
        stream << "p2 is invalid" << endl;
    }

}

int main()
{
    test_advanced_return_type();
}