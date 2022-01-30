#include <iostream>

/*
    Partial Type:

        Types that are declared but not defined yet.

        For example,

        template<typename ElementTypeA> class TypeA; 

        int a[]; // declaration, but we have not defined the element count of array of type int.

        So, a is still a partial type.

        In C++, type void is also regarded as a partial type.

        In C++, name should be declared before its use.

        In C++, template members that are not used are not instantiated.

        When the template member is complex such that C++ compiler fails to parse
        the syntax properly, in such case, you have to prepend keyword template
        in front of the member template as below.
*/

// this is partial declaration, the type of TypeB
// is not yet defined
template<typename ElementTypeB> class TypeB;

template<typename ElementTypeA> // template clause
class TypeA
{
    template<typename ElementType> friend class TypeB;

    private:
        ElementTypeA m_data{};

    public:
        TypeA(ElementTypeA v = ElementTypeA{}):
            m_data{v} { }

    // in-class template member definition
    template<typename ElementType>
    void operation_over_type_b(TypeB<ElementType> b)
    {
        std::cout << b.m_data << std::endl;
    }

    // member template, declared but not defined
    template<typename Type> auto sum(Type a, Type b);

    // in-class static member function definition
    template<typename Type, typename... Types>
    static auto summation(Type first, Types... args)
    {
        return (first + ... + args);
    }

};

template<typename ElementTypeB> // template clause
class TypeB
{
    // this is syntax error
    // keyword friend should not come before template clause
    // friend template<typename ElementType> class TypeA;
    template<typename ElementType> friend class TypeA;

    private:
        ElementTypeB m_data{};

    public:
        TypeB(ElementTypeB v = ElementTypeB{} ): m_data { v }
            { }

    // in-class template member definition
    template<typename ElementType>
    void operation_over_type_a( TypeA<ElementType> a)
    {
        std::cout << a.m_data << std::endl;
    }

    // static member declaration
    template<typename Type, typename... Types>
    static auto products(Type first, Types... args);
};

template<typename Type>
struct element_t
{
    Type m_data;
};

// out-of-class definition of TypeA<ElementTypeA>::sum(Type, Type)
template<typename ElementTypeA>
    template<typename Type> 
    auto TypeA<ElementTypeA>::sum(Type a, Type b)
{
    return element_t<Type>{ a + b };
}

// out-of-class static member function definition
template<typename ElementType>
    template<typename Type, typename... Types>
    auto TypeB<ElementType>::products(Type first, Types... args)
    {
        return (first * ... * args);
    }

void test_template_non_static_member_function()
{
    TypeA<int> a;

    TypeB<int> b;

    a.operation_over_type_b(b);
    
    b.operation_over_type_a(a);
}

void test_out_of_class_definition()
{
    TypeA<int> a;

    auto rlt = a.sum(4, 6);

    std::cout << "sum of 4 and 6 = " << rlt.m_data << std::endl;

}

void test_out_of_class_static_member_functions()
{
    auto summation = TypeA<double>::template summation(1, 2, 3, 4, 5);
    auto product = TypeB<int>::template products(1, 2, 3, 4, 5);

    std::cout << "summation 1 to 5 = " << summation << std::endl;
    std::cout << "product 1 to 5 = " << product << std::endl;
}

int main()
{
    // test_template_non_static_member_function();
    // test_out_of_class_definition();
    test_out_of_class_static_member_functions();
}