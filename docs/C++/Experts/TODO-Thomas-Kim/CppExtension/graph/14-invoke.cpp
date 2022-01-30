#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

int sum(int a, int b)
{
    return a + b;
}

struct Product
{
    int operator()(int a, int b) const
    {
        return a * b;
    }

    int summation(int a, int b, int c)
    {
        return a + b + c;
    }

};

void test_invoke()
{

    stream <<"sum(2, 3) = " << std::invoke(sum, 2, 3) << endl;

    stream << "Product(3, 4) = "
        << std::invoke(Product(), 3, 4) << endl;

    stream << "&Product::summation(1, 2, 3) = "
        << std::invoke(&Product::summation, Product{}, 1, 2, 3) << endl;

    stream <<"Lambda(3, 4) = "
        << std::invoke([](int a, int b){ return a * b; } , 3, 4) << endl;

    auto closure = [](int a, int b, int c)
        {
            return (a + b) * c;
        } ;

    stream <<"closure(3, 3, 2) = " << std::invoke(closure, 3, 3, 2) << endl;
}

int main()
{
    test_invoke();
}