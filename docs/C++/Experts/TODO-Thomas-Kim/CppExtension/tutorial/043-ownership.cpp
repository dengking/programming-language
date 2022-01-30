#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

/*

    Originally, lvalue reference was introduced to C++ to unburden 
    C++ programmers from the uninitialized pointers.

    For example,

        int* ptr;

        int s = *ptr;

        or

        *ptr = 5;

    is the shortcut to application crash.

    To prevent such misfortune, C++ Standard Committee introduced 
    lvalue reference, which enforces initialization at definition as below;

    int& lvalue_ref = &s;

    Now, let's think about rvalue reference.  Rvalue reference was introduced
    to C++11 Standard for move semantic.

    Rvalue reference is also "a reference." That is, it should also be
    initialized at the definition.

    int&& rvalue_ref = 5;

    or

    For your convenience,

    int s = 5;

    // it is valid operation.
    int&& rvalue_reference = std::move(s);

 */

void test_rvalue_lvalue_reference()
{
    // The name s names an object on the stack.
    int s = 5;

    // int&& rvalue_ref = s; // it does not work.

    // std::move is simply type cast to rvalue reference
    // int&& rvalue_ref = std::move(s);

    // The name rvalue_ref references to the object
    // named by s.
    int&& rvalue_ref = (int&&)s;

    // The name lvalue_ref references to the object
    // named by s
    int& lvalue_ref = s;

    /*
        There is no difference between line# 52
        and line# 54, no difference.
     */

    stream << "The address of s: " << &s << endl;
    stream << "The address of rvalue_ref: " << &rvalue_ref << endl;
    stream << "The address of lvalue_ref: " << &lvalue_ref << endl;

    /*
        s, rvalue_ref, lvalue_ref are actually the same object.

        lvalue_ref and rvalue_ref are references to s.

        The lifetime of the object named by s
        is owned by the name s.

        What about lvalue_ref and rvalue_ref ?

     */
}


class ClsType
{
    private:
        bool m_valid{true};

    public:
        ClsType()
        {
            stream <<"ClsType() called" << endl;
        }

        ClsType(const ClsType& rhs)
        {
            stream << "Copy Constructor called" << endl;
        }

        ClsType(ClsType&& rhs)
        {
            rhs.m_valid = false;
            stream << "Move Constructor called" << endl;
        }

        void some_operation()
        {

        }

        ~ClsType()
        {
            if(this->m_valid)
            {
                stream << "Object cleaned up" << endl;
            }
        }

};

void understand_the_concept_of_ownership()
{
    ClsType object;

    stream << "Before entering local block" << endl;

    {
        // this code does not work.
        // ClsType&& rvalue_ref = object;
        
        // this line of code does NOT construct a new instance of type ClsType
        ClsType && rvalue_ref = std::move(object);

        // this line of code does NOT construct a new instance of type ClsType
        ClsType & lvalue_ref = rvalue_ref;

        // in this block, both rvalue_ref and lvalue_ref
        // are simply alias for object
        //
        // both rvalue_ref and lvalue_ref
        // do not own the object.
        // the object's lifetime is independent
        // of lvalue_ref and rvalue_ref

    }

    stream << "After leaving local block" << endl;

    // the object defined at line# 122 is
    // cleaned up after leaving this function block
}

void object_life_time_of_const_and_rvalue_ref()
{

    ClsType object1; // we create an object of type ClsType,
                    // so, the constructor of ClsType() should be called at this point.

    stream << "Before Entering the local block" << endl;

    {
                    // at this point, obj_ref is simply a reference
                    // std::move() is a typecast to ClsType&&
                    // We are not creating an instance of ClsType
                    // so, no copy, no move constructor is called.
        ClsType && obj_ref = std::move(object1);

            // Now, this is a totally difference story.
            // we are creating a new instance object2 of type ClsType
            // object2 is not a reference, and we type cast object1 to ClsType&&
            // Since we are creating an instance of ClsType,
            // a constructor of ClsType should be called at this point.
            // Since we applied std::move(object1)
            // the ownership of the object named by object1
            // is moved to object2

        ClsType object2 = std::move(object1); // we are moving the ownership of the object
                                                // originally named by object1.
                                                // after move, object1 is invalid.
        // Don't get me wrong.
        // the destructor of object1 will be still called
        // what I mean by "invalid" is that
        // "class invariant" of object1 is INVALID.
        // its state is destroyed or moved to object2.

        // Now object2 owns the object named by object1,
        // the ownership moved from object1 to object2.
        // at the end of this block,
        // when object2 goes off, 
        // the object originally named by object1,
        // is moved to object2, then is destroyed at the end of this block.
    }

    stream << "After leaving the local block" << endl;

}

void bomb_lurking_in_your_code(ClsType& obj)
{
    ClsType moved_object = std::move(obj);
}

void bomb_lurking_in_your_code_rvalue_ref(ClsType&& obj)
{
    ClsType moved_object = std::move(obj);
}

// this kind of coding fails 9 out of 10 cases.
void you_should_be_extra_careful_when_using_std_move()
{

    ClsType object;

    ClsType& obj_ref = object;

    bomb_lurking_in_your_code(object);

    obj_ref.some_operation();
}

// your program crashes 9 out of 10 cases.
void you_should_be_extra_careful_when_using_ref_move()
{

    ClsType object;

    ClsType& obj_ref = object;

    bomb_lurking_in_your_code_rvalue_ref(std::move(object));

    obj_ref.some_operation();
}

// std::move() is blind. It is a bomb lurking in your code.
// when you use std::move(), you have to be extra careful.

int main()
{
    // test_rvalue_lvalue_reference();

    // understand_the_concept_of_ownership();

    // object_life_time_of_const_and_rvalue_ref();

    you_should_be_extra_careful_when_using_ref_move();
}