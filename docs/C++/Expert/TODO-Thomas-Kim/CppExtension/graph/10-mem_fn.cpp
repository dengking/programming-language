#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

template<typename ReturnType, typename ClassType, typename... Types>
class mem_func_call_wrapper
{
    private:
        ReturnType (ClassType::*mem_ptr)(Types...);
    public:
        mem_func_call_wrapper( ReturnType (ClassType::*mptr)(Types...) ):
            mem_ptr {mptr} { }

    template<typename TypeClass, typename... ArgTypes>
    ReturnType operator()(TypeClass&& obj, ArgTypes&&... args) const
    {
        return (obj.*mem_ptr)( std::forward<ArgTypes>(args)...);
    }
};

template<typename ReturnType, typename ClassType, typename... Types>
mem_func_call_wrapper<ReturnType, ClassType, Types...> mem_fn(ReturnType (ClassType::*mptr)(Types...))
{
    return { mptr };
}

class DummyClass
{
    private:
        int m_value;
    public:

    DummyClass(int value) : m_value { value } { }

        int sum(int a, int b)
        {
            return m_value * ( a + b );
        }

        int pro(int a, int b)
        {
            return m_value * (a * b);
        }
};

void test_mem_fn()
{
    DummyClass d{10};

    auto mf = mem_fn( &DummyClass::sum );

    auto rlt = mf(d, 2, 3); // d.sum(2, 3);

    stream << "rlt = " << rlt << endl;

    auto mf_pro = mem_fn( &DummyClass::pro );


    stream << "d.pro(2, 3) = " << mf_pro(d, 2, 3) << endl; 

}

int main()
{
    test_mem_fn();
}


