#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

template<typename Type> // Type - is called template parameter
void function(Type arg) // arg - is called function-call (template) parameter
                        // function-call parameter
{

}

void test_function_call()
{

    int m = 5;

    function<int> // type in the angle bracket <int> is called template argument
        (m); // m is called function-call argument

    double d = 5.0;

    function<double&>(d); 

    // <double&> is template argument
    // d is function-call argument

}
/*
    In modern C++, parameter and argument are used differently.
    In older C/C++, parameter and argument are synonyms.
 */

template<typename Type> // Type is template parameter
class NonReferenceMember
{
    private:
        Type m_member; // The type of Type and the type of m_member
                        // are always... always the same.

    /*
        The type of Type and the type of m_member are always the same.

        The allowed types for Type and m_member are

        int - non-reference type
        int& - lvalue reference type

        of course, const int, const double, etc. allowed
        but int&& - rvalue refernce is not allowed.
    */
    public:
        NonReferenceMember(Type member): // int&& 10
            m_member { member } { }

        void report()
        {
            stream << "Type of Type: " << Tpf_GetTypeName(Type) << endl;
            stream << "Type of m_member: " << Tpf_GetTypeCategory(m_member) << endl;
        }
};

template<typename Type>
class NonReferenceMemberWithForwardConstructor
{
    private:
        Type m_member; // the type of Type and the type of m_member
                        // are always the same

        /*
        
        Again, the type of Type and the type of m_member
        are always the same.

        All possible types in C++ are allowed for Type and m_member type.
        
        */

    public:
        NonReferenceMemberWithForwardConstructor(Type member):
            m_member { 
                    // std::forward<Type>(member)
                    std::forward< decltype(member) >(member) 
            } { }

      void report()
        {
            stream << "Type of Type: " << Tpf_GetTypeName(Type) << endl;
            stream << "Type of m_member: " << Tpf_GetTypeCategory(m_member) << endl;
        }
};

void test_non_reference_member_classes()
{
    // this works successfully
    NonReferenceMember<int> m1{10};
    m1.report();

    // this does not work.
    // NonReferenceMember<int&&> m2{10};

    /*
         NonReferenceMember(Type member): 
                    m_member { member } { }
     */

    int&& member = 10; // the type of member is rvalue reference
                        // but its value category is lvalue

    // this does not work,
    // int&& m_member = member; // you are assigning lvalue to rvalue reference
                             // this is illegal in C++

    // int&& m_member = std::forward<int>(member); 
    int&& m_member = std::forward<int&&>(member);
    // int&& m_member = std::forward< decltype(member) > (member);
    // int&& m_member = std::move(member);

    // it works ... 
    NonReferenceMemberWithForwardConstructor<int&&> m3{10};
    m3.report();

}

void comparison_of_two_classes()
{
    int m = 5;
    
    NonReferenceMember<int> m1{m};
    m1.report();

    NonReferenceMember<int&> m2{m};
    m2.report();

    // it does not work
    // NonReferenceMember<int&&> m3{ 5 };

    stream << endl;

    NonReferenceMemberWithForwardConstructor<int> m4{m};
    m4.report();

    NonReferenceMemberWithForwardConstructor<int&> m5{m};
    m5.report();

    NonReferenceMemberWithForwardConstructor<int&&> m6{ 6 };
    m6.report();
}

template<typename Type>
class LValueReferenceMember
{
    private:
        Type& m_member; // At first glance, the type of Type
                        // and the type of m_member might seem always different.

                        // But that is not true...
                        // the type of Type can be int&,
                        // the type of m_member can also be int&
                        //
                        // that is, the type of Type and the type of m_member can be the same

        /*
        
            One thing we can say for sure is that

            both the type of Type and the type of m_member can never be 
            rvalue reference type.
        
         */
        
        /*
            If the type of Type is int,

            Type = int,

            int& m_member,

            So, the type of Type is int
            the type of m_member is int&


         */

};

template<typename Type>
class RValueReferenceMember
{
    private:
        Type&& m_member;

        // The type of Type can be int, int&, int&&.
        // the type of m_member can be int& and int&&, but can never be int.

};


int main()
{
    // test_non_reference_member_classes();

    comparison_of_two_classes();
}
