#include <tpf_output.hpp>

/*
    1. We will first learn what is "Copy Elision."

    2. If we are to use Named Return Value Optimization technique,
        we have to return the same named instance from all branches in the function.

    Using "Copy Elision" and "Named Return Value Optimization," we can
    significantly improve the performance of our algorithm.

    Its impact on our algorithm is by no means small.

    3. If a class defines both copy constructor and move constructor, and if
    we return an instance of that class from a function as pure rvalue or prvalue,
    the move constructor take precedence over the copy constructor.

    4. C++ compiler generates totally different binary code for debugging build and release build
    depending on commandline switches.

    Your debugging build successfully works but your release build can fail.
    So, I almost never use C++ debugger.

*/
tpf::sstream stream;
auto endl = tpf::endl;

class CopyOrMove
{
    private:
        int m_member;

    public:

        void SetValue(int m) { this->m_member = m; }

        CopyOrMove(int m = int{}): m_member{m}
        {
            stream << "Default constructor called" << endl;
        }

        CopyOrMove(const CopyOrMove& right_hand_side):
            m_member{right_hand_side.m_member}
        {
            stream << "Copy constructor called" << endl;   
        }

        CopyOrMove& operator=(const CopyOrMove& right_hand_side)
        {
            stream << "Copy assignment operator called"<< endl;

            // I will talk more about std::addressof() function
            // in some future sessions
            if(this != std::addressof(right_hand_side))
            {
                this->m_member = right_hand_side.m_member;
            }

            return *this;
        }

        CopyOrMove(CopyOrMove&& right_hand_side):
            m_member{ std::move(right_hand_side.m_member) }
        {
            stream << "Move constructor called" << endl;
        }

        CopyOrMove& operator=(CopyOrMove&& right_hand_side)
        {
            stream << "Move assignment operator called" << endl;
            if(this != std::addressof(right_hand_side))
            {
                this->m_member = std::move(right_hand_side.m_member);
            }

            return *this;
        }

        ~CopyOrMove()
        {
            stream << "Destructor is called" << endl;
        }
};

// this returns an instance of CopyOrMove, a prvalue
// we are using copy elision
CopyOrMove make_object_copy_elision(int n)
{
    // we created an instance here
    // and return it to its called.
    // Once the constructor called here
    return { n };
}

void examples_for_copy_elision()
{
    // this assignment operator =() is never called.
    // If we are using C++98 standard,
    // then the constructor should have been called twice.
    // then the constructor should have been called here too.
    // and copy assignment operator=() should also have been called.

    // But in modern C++, if we can use "Copy Elision"
    // we do not create temporary to initialize our object.
    CopyOrMove obj = make_object_copy_elision(3);
}

CopyOrMove make_copy_or_move_Named_Return_Value_Optimization(int n)
{
    // calls default constructor
    CopyOrMove obj{n}; // obj is named

    // CopyOrMove defines both copy constructor and move constructor
    // When both are defined, then C++ prefers "Move Constructor" over Copy Constructor
    // when passing prvalue, or pure rvalue as return value.

    return obj; // we are returning named local object as prvalue.
}

// if we give optimization switch /O2 in case of MSVC,
// Named Return Value Optimization kicks in.

// Another point is that... giving optimization switch generates
// totally different binary executable.
//
// If you ever watched my videos for a while, you probably have noticed that
// I almost never use DEBUGGER...
// The reason is ... generated code for debugging
// is very different from release build.
void example_for_named_return_value_optimization()
{
    // initializing using move constructor of CopyOrMove
    auto obj = make_copy_or_move_Named_Return_Value_Optimization(5);
}

// it failed Named Return Value Optimization
// because the return value of this function is different
// object depending of if-branch
CopyOrMove failed_named_return_value_optimization(int a, int b)
{
    CopyOrMove result { a }; // result is a named instance of CopyOrMove

    if(a < b)
        return result; // we return a named value
    else
        return CopyOrMove{ b }; // or we return an unnamed return value
}

void example_for_failed_named_return_value_optimization()
{
    auto obj = failed_named_return_value_optimization(5, 6);
}

// we should return the same instance of the return value or object
// we are returning a named instance of CopyOrMove
// taking advantage of Named Return Value Optimization technique
// in this case, we have to Return The Same Named Instance
// in all branches in the function().
CopyOrMove successful_named_return_value_optimization(int a, int b)
{
    CopyOrMove result; // result is a named instance of CopyOrMove

    if(a < b)
    {
        result.SetValue(a); return result;
    }
    else
    {
        result.SetValue(b); return result;
    }
}

void example_for_successful_named_return_value_optimization()
{
    auto obj = successful_named_return_value_optimization(5, 6);
}

int main()
{
    // examples_for_copy_elision();

    // example_for_named_return_value_optimization();

    stream <<"Failed Named Return Value Optimization" << endl;

    example_for_failed_named_return_value_optimization();

    stream <<"\n\nSuccessful Named Return Value Optimization" << endl;
    
    example_for_successful_named_return_value_optimization();
}