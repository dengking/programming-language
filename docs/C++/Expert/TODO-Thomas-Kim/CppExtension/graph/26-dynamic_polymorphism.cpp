#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

class BaseWithVD // base with a virtual destructor
{
    public:

    virtual ~BaseWithVD() = default;
};

class BaseWithoutVD
{
    public:

    ~BaseWithoutVD() = default;
};

class A: public BaseWithVD
{
    public:
    
    // we have not defined virtual destructor for A
    // but, since BaseWithVA has a virtual destructor
    // all inherited classes also have a virtual destructor
    // So, class A gets a virtual destructor by default
};

class B: public BaseWithoutVD
{
    public:
};

void test_virtual_destructor()
{
    // std::is_base_of_v<>
    stream << "Is A a base of B ? "
        << std::is_base_of_v<A, B> << endl;

    stream << "Is BaseWithVD a base of A ? "
        << std::is_base_of_v<BaseWithVD, A> << endl;

    stream << "Is A a base of BaseWithVD ? "
        << std::is_base_of_v<A, BaseWithVD> << endl;

    stream << "Is BaseWithoutVD a base of B ?"
        << std::is_base_of_v<BaseWithoutVD, B> << endl;

    stream << "Is B a base of BaseWithoutVD ?"
        << std::is_base_of_v<B, BaseWithoutVD> << endl;

    stream << "Has A a virtual destructor ? "
        << std::has_virtual_destructor_v<A> << endl;

    stream << "Has B a virtual destructor ? "
        << std::has_virtual_destructor_v<B> << endl;
}

int main()
{
    test_virtual_destructor();
}