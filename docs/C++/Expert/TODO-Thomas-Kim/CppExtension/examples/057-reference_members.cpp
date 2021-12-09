#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

template<typename ElementType>
class NonReferenceMembered
{
    private:
        ElementType m_member;

    public:
        NonReferenceMembered(ElementType member):
            m_member{ std::forward<ElementType>(member) } { }

        void report() const
        {
            stream << "--- NonReferenceMembered ---\n\n";
            stream << "Type of ElementType: " << Tpf_GetTypeName(ElementType) << endl;
            stream << "Type of m_member: " << Tpf_GetTypeCategory(m_member) << endl << endl;
        }
};

template<typename ElementType>
class LvalueReferenceMembered
{
    private:
        ElementType& m_member;

    public:
        LvalueReferenceMembered(ElementType& member):
            m_member{ member } { }

         void report() const
        {
            stream << "--- LvalueReferenceMembered ---\n\n";
            stream << "Type of ElementType: " << Tpf_GetTypeName(ElementType) << endl;
            stream << "Type of m_member: " << Tpf_GetTypeCategory(m_member) << endl << endl;
        }
};


template<typename ElementType>
class RvalueReferenceMembered
{
    private:
        ElementType&& m_member;

    public:
        RvalueReferenceMembered(ElementType&& member):
            m_member{ std::forward<ElementType>(member) } { }

         void report() const
        {
            stream << "--- RvalueReferenceMembered ---\n\n";
            stream << "Type of ElementType: " << Tpf_GetTypeName(ElementType) << endl;
            stream << "Type of m_member: " << Tpf_GetTypeCategory(m_member) << endl << endl;
        }
};

void test_non_reference_membered()
{
    NonReferenceMembered<int> non_ref_init_int{ 5 };

    int m = 6;
    NonReferenceMembered<int&> non_ref_init_lvalue{ m };

    NonReferenceMembered<int&&> non_ref_init_rvalue { 5 };

    non_ref_init_int.report();
    non_ref_init_lvalue.report();
    non_ref_init_rvalue.report();
}

void test_lvalue_reference_membered()
{
    //LvalueReferenceMembered<int> lvalue_ref_init_int{ 5 };
    // LvalueReferenceMembered<int&&> lvalue_ref_init_int{ 5 };

    int m = 6;
    LvalueReferenceMembered<int> lvalue_ref_init_int{ m };
    lvalue_ref_init_int.report();

    LvalueReferenceMembered<int&> lvalue_ref_init_lvalue_int{ m };
    lvalue_ref_init_lvalue_int.report();

    LvalueReferenceMembered<int&&> lvalue_ref_init_rvalue_lvalue_int{ m };
    lvalue_ref_init_rvalue_lvalue_int.report();    
}

void test_rvalue_reference_membered()
{
    RvalueReferenceMembered<int> rvalue_ref_init_rvalue{5};
    rvalue_ref_init_rvalue.report();

    // RvalueReferenceMembered<int&> rvalue_ref_lvalue_init_rvalue{5};

    int m = 5;

    // RvalueReferenceMembered<int> rvalue_ref_init_lvalue{ m };
    
    RvalueReferenceMembered<int&> rvalue_ref_lvalue_int_lvalue{ m };
    rvalue_ref_lvalue_int_lvalue.report();

    // RvalueReferenceMembered<int&&> rvalue_ref_rvalue_init_rvalue{ m };
    // RvalueReferenceMembered<int&&> rvalue_ref_rvalue_init_lvalue{m};

    RvalueReferenceMembered<int&&> rvalue_ref_rvalue_init_rvalue{ std::move(m) };
    rvalue_ref_rvalue_init_rvalue.report();
    
}

int main()
{
    // test_non_reference_membered();
    // test_lvalue_reference_membered();
    test_rvalue_reference_membered();
}