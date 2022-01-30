#include <tpf_output.hpp>

/*
    Lambda is nothing but a function object, or is called "functor"
 */
tpf::sstream stream;
auto endl = tpf::endl;

class Functor
{
    private:
        int m_offset;

    public:

        Functor(int offset): m_offset{ offset } {} 

        // function call operator
        int operator()(int a, int b) const
        {
            return m_offset * (a + b);
        }
};

void test_functor()
{
    Functor f{10};

    stream <<"50 will be returned: " << f(2, 3) << endl;

    Functor g{20};

    stream << "100 will be returned: " << g(2, 3) << endl;

    auto h = [offset = 10](int a, int b) 
        { 
            return offset *(a+b); 
        };

    stream << "calling lambda(2, 3) : " << h(2, 3) << endl;

    auto j = [offset = 25](int a, int b)
    {
        return offset * (a * b);
    };

    stream << "calling j(2, 2) ? " << j(2, 2) << endl;

}

class DummyClass
{
    private:
        int m_data;

    public:
        DummyClass(int value = 0):
            m_data{value} { }

        int sum(int a, int b)
        {
            return m_data*(a + b);
        }

        int pro(int a, int b)
        {
            return m_data*(a * b);
        }

        double square(double v)
        {
            return v * v / m_data;
        }
};

// we defined type alias for member function
// that belongs to DummyClass
// returns int, and takes two ints
using mem_function_t = int(DummyClass::*)(int, int);

int call_mem(DummyClass& d, mem_function_t fptr, int a, int b)
{
    return (d.*fptr)(a, b);
}

int call_mem_through_obj_ptr(DummyClass* dptr, mem_function_t fptr, int a, int b)
{
    return (dptr->*fptr)(a, b);
}

void test_call_mem()
{
    DummyClass d{20};

    auto rlt = call_mem(d, &DummyClass::sum, 2, 3);

    stream << "rlt: " << rlt << endl;

}

void test_call_mem_through_obj_ptr()
{
    DummyClass d{20};

    auto rlt = call_mem_through_obj_ptr(&d, &DummyClass::pro, 2, 3);

    stream << "rlt: " << rlt << endl;
}

void test_member_function_pointer()
{
    // the type of function sum
    // int(int, int);
    
    // here fptr is a pointer instance
    // int* fptr (int, int); // this is a function declaration that
                            // return a int*, and takes two ints

    // fptr is un-initialized function pointer declaration
    int (*fptr) (int, int); // fptr is a function pointer that points to a function
                            // that returns an int, and takes two ints and parameters(arguments)

    // now, mfptr is a member function pointer
    // that belongs to DummyClass, 
    // that return int, and takes two ints
    int ( DummyClass:: *mfptr )(int, int);

    DummyClass d1{10};

    // we assigned the address of member function sum
    // that belongs to DummyClass
    mfptr = &DummyClass::sum;

    auto r = (d1.*mfptr)(2, 3); // d1.sum(2, 3);

    stream << "r = " << r << endl;

    mfptr = &DummyClass::pro;

    r = (d1.*mfptr)(2, 3); // d1.pro(2, 3);

    stream << "r = " << r << endl;
}

int main()
{
    // test_functor();

    // test_member_function_pointer();

    // test_call_mem();

    test_call_mem_through_obj_ptr();
}