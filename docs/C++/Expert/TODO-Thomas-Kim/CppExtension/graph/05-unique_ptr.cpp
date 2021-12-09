#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

template<typename ElementType>
class DummyClass
{
    public:
        using uptr_t = std::unique_ptr<ElementType>;
        using lref_uptr_t = std::unique_ptr<ElementType>&;
        using const_lref_uptr_t = const std::unique_ptr<ElementType>&;
        using rref_uptr_t = std::unique_ptr<ElementType>&&;

    private:
        uptr_t m_member;

    public:
        DummyClass(ElementType value = ElementType{}):
            m_member{ std::make_unique<ElementType>(value) } { }

    // we introduced template parameter Type
    // which breaks the tie between DummyClass ('s template parameter ElementType)
    // and its member get_member()
    template<typename Type = const_lref_uptr_t>
    Type get_member()
    {
        return static_cast<Type>(this->m_member);
    }
};

void test_get_member()
{
    using dummy_t = DummyClass<double>;
    
    dummy_t dummy{ 22.0 / 7.0 };


    // auto is copy semantic.
    auto& p1 = dummy.get_member();

    stream <<"the value of the object that p1 is pointing to is "
        << *p1 << endl;

    stream << "The type of p1: "
        << Tpf_GetTypeCategory(p1) << endl;

    auto& p2 = dummy.get_member<dummy_t::lref_uptr_t>();

    stream <<"the value of the object that p2 is pointing to is "
        << *p2 << endl;

    stream << "The type of p2: "
        << Tpf_GetTypeCategory(p2) << endl;

    
    // now, it is "move semantic"
    auto p3 = dummy.get_member<dummy_t::rref_uptr_t>();

    stream <<"the value of the object that p3 is pointing to is "
        << *p3 << endl;

    stream << "The type of p3: "
        << Tpf_GetTypeCategory(p3) << endl;
}

int main()
{
    test_get_member();
}