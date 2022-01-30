#include <iostream>
#include <new>
#include <type_traits>
#include <memory>

constexpr size_t buffer_size = 10;

int gBuffer[buffer_size];

void test_placement_new_operator()
{
    // int* ptr1 = new ( &gBuffer[0] ) int{10};
    int* ptr1 = new ( gBuffer + 0 ) int{10};

    std::cout << "*ptr1 = " << *ptr1 << std::endl;

    double* dptr = new (ptr1 + 1) double{3.14};

    std::cout << "*dptr = " << *dptr << std::endl;

    double* dptr2 = new ( dptr + 1 ) double {22.0/7.0};

    std::cout << "*dptr2 = " << *dptr2 << std::endl;

    long long* lptr = new (dptr2 + 1) long long{ 7ull };

    std::cout << "*lptr = " << *lptr << std::endl;
}

void placement_new_using_local_stack_buffer()
{
    constexpr size_t element_count = 10;

    int local_buffer[element_count];

    int& a = *new (local_buffer + 0) int {0};

    std::cout <<"a = " << a << std::endl;

    double& d = *new(&a + 1) double {22.0 / 7.0};

    std::cout << "d = " << d << std::endl;

    short& s = *new ( &d + 1) short { 4 };

    std::cout << "s = " << s << std::endl;

}

template<typename Type>
void placement_delete(Type*& ptr)
{
    if constexpr( std::is_class_v<Type> && std::is_destructible_v<Type>)
    {
        if(ptr)
        {
            ptr->~Type(); // we are calling destructor explicitly
            ptr = nullptr;
        }
    }
}

class ClsType
{
    private:
        int m_member;
    public:
        ClsType(int member = int{}):
            m_member{ member }
        {
            std::cout << "ClsType() called" << std::endl;
        }
        ~ClsType()
        {
            std::cout << "~ClsType() called" << std::endl;
        }

    friend std::ostream& operator << (std::ostream& os, const ClsType& t)
    {
        os << t.m_member;
        return os;
    }
};

void advanced_placement_new_delete()
{
    size_t element_count = 10;

    // we are dynamically allocate 10 instance of ClsType
    std::unique_ptr<char[]> buffer( new char [ element_count * sizeof(ClsType) ] );

    ClsType *ptr1 = new ( buffer.get() ) ClsType{0};
    
    ClsType *ptr2 = new ( ptr1 + 1 ) ClsType{1};

    std::cout << *ptr1 << std::endl;
    std::cout << *ptr2 << std::endl;

    placement_delete(ptr1);

    placement_delete(ptr2);

    ptr1 = new (buffer.get() ) ClsType{3};

    std::cout << *ptr1 << std::endl;

    placement_delete(ptr1);

    // the point is that 
    // don't do delete ptr1, or delete ptr2
}

int main()
{
    // test_placement_new_operator();
    
    // placement_new_using_local_stack_buffer();

    advanced_placement_new_delete();
}