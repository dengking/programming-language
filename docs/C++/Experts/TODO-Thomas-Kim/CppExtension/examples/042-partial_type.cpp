#include <iostream>

template<typename ElementTypeB> class TypeB;

template<typename ElementTypeA>
class TypeA
{
    template<typename ElementTypeB> friend class TypeB;

    private:
        ElementTypeA m_data{};

    public:
    
    template<typename ElementTypeB>
    void operation_over_type_b(TypeB<ElementTypeB> b);
};

template<typename ElementTypeB>
class TypeB
{
    template<typename ElementTypeA> friend class TypeA;

    private:
        ElementTypeB m_data{};
    
    public:

    template<typename ElementTypeA>
    void operation_over_type_a(TypeA<ElementTypeA> a)
    {
        std::cout << a.m_data << std::endl;
    }

};

template<typename ElementTypeA>
    template<typename ElementTypeB>
void TypeA<ElementTypeA>::operation_over_type_b(TypeB<ElementTypeB> b)
{
    std::cout << b.m_data << std::endl;
}

int main()
{
    TypeA<int> a;
    
    TypeB<int> b;

    a.operation_over_type_b(b);

    b.operation_over_type_a(a);
}
