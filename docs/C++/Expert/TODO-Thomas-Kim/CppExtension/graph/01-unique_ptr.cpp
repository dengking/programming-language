#include <tpf_output.hpp>

#include <memory> // for std::unique_ptr

/*
    In this episode, we will learn how to pass / return std::unique_ptr to / from a function.

    Please note that std::unique_ptr is declared in the header <memory>
 */

tpf::sstream stream;
auto endl = tpf::endl;

// this function take_unique_ptr_by_value() takes a parameter
// of type unique_ptr by value.
// this function is called "the callee"
void take_unique_ptr_by_value(std::unique_ptr<int> uptr)
{

    stream <<"The deferenced value of uptr = " << *uptr << endl;

    // uptr goes of this function block,
    // its memory is destroyed.
}

// this function pass_unique_ptr_to_a_funcion_by_value() is called
// the caller.
void pass_unique_ptr_to_a_funcion_by_value()
{
    auto p1 = std::make_unique<int>(1);

    // std::unique_ptr does not provide copy constructor / copy assignment
    // so this line of code does not work
    // take_unique_ptr_by_value(p1);

    // we have to transfer the ownership of unique_ptr
    // when we pass a unique_ptr to a function by value.
    take_unique_ptr_by_value( std::move(p1) );
    // because we call take_unique_ptr_by_value( std::move(p1) )
    // by value, transferring the ownership of p1

    // WARNING: After passing unique_ptr to a function,
    // the caller, in this case, pass_unique_ptr_to_a_funcion_by_value()
    // should not access the unique_ptr any longer.

    // So, you are causing trouble in the following line of code

    // int a = *p1; // this code causes application crash.

    // stream <<"Wow, not crashed! Really?" << endl;


    // if you want to test the validity of unique_ptr
    // you can do this

    if(p1) // test if p1 is valid
    {
        stream <<"Wow, p1 is valid" << endl;
    }
    else
    {
        stream << "unique_ptr p1 is no longer valid" << endl;
    }
    
}

void test_cloning_unique_ptr()
{
    auto p1 = std::make_unique<int>(10);

    // clone p1 and pass the cloned p1 to 
    // take_unique_ptr_by_value() 
    take_unique_ptr_by_value( std::make_unique<int>( *p1 ) );

    // since we passed cloned p1 to take_unique_ptr_by_value()
    // the original p1 is still valid at this line of code

    stream <<"p1 is still valid: " << *p1 << endl;

    // we are cloning p1 and assign the cloned to p2
    // p1 and p2 are distinct object.
    auto p2 = std::make_unique<int>( *p1 );

    stream <<"the value of p2 after cloned: " << *p2 << endl;
    
    *p2 = 20;

    stream <<"the value of p1: " << *p1 << endl;
    stream <<"the value of p2: " << *p2 << endl;

}
int main()
{
    // pass_unique_ptr_to_a_funcion_by_value();

    test_cloning_unique_ptr();
}