
#include <tpf_types.hpp>
#include <tpf_output.hpp>

#ifdef __FUNCSIG__
    #define FUNCTION_NAME __FUNCSIG__
#else
    #define FUNCTION_NAME __PRETTY_FUNCTION__
#endif 

/*
    1. We will define a macro for function name.
        In case of MSVC or Microsoft Visual C++ Compiler, the macro 
        __FUNCSIG__ is defined inside of a function. This macro is
        defined only inside of a function.

        In case of GNU g++/clang++, the macro __PRETTY_FUNCTION__ is 
        defined inside of a function.

        For compatibility for Microsoft Visual C++ and GNU g++/clang++,
        we defined a common macro named FUNCTION_NAME

    2. We will learn how to use FUNCTION_NAME macro.

    3. According to the Creator of C++, Bjarne Stroustrup, in his book
    The C++ Programming Language, he mentions "the interface" of a class.

    "The interface" of a class means the public member functions. In our MyClass,
    the interface of this class are

    int& get_data();
    const int& get_data() const

    He also mentions "the implemenation" of a class. By the "implementation," he means
    the data members or data fields. In our case of MyClass,

    "the implementation" is

    int m_data;

    And, I will call member functions without trailing "const" specifier as "non-const" member function.
    So, 
        int& get_data() is non-const member function, or non-constant inferface.

    I will call the member function with trailing "const" specifier as "const" member functions.
    In our case,

        const int& get_data() const is a const member function, or constant interface.

    "implementation", "interface", "non-const" member function or non-constant interface,
    "const" member function or const interface.

    4. The type of "this" in non-const member functions is 

        ClassType* const; // this is constant pointer that points to non-const object.

        The type of "this" in const member functions is

        const ClassType* const; // this is constant pointer that points to const object.

        "const" in "ClassType* const" means we cannot modify the value of the pointer itself.
        It does not mean we cannot modify the value the const pointer is pointing to.

        The "trailing const" in "const ClassType* const" means that we cannot modify 
        the value of the pointer itself. It does not mean we cannot modify the value of the object
        the constant pointer is pointing to.

        In this case, the "leading const" in "const ClassType* const" means that we cannot modify
        the value of the object that the const pointer is pointing to.

        In a non-const member functions,
        such as int& get_data(), the type of "this" is "MyClass* const" which means
        the const pointer "this" points to a non-const object, so we can modify the value of the object
        that "this" pointer is pointing to. That is, we can alter the state or implementation of the object
        in the non-const member functions.

        In a const member functions,
        such as const int& get-data() const, the type of "this" is "const MyClass* const" which means
        the const pointer "this" points to a const object, so we cannot modify the value of the const object
        that "this" pointer is pointing to. That is, we cannot alter the state or implementation of the object
        in the const member functions.
        
    5. Let me clear away the term "const pointer."

    6. References and pointers in machine-level, they are synonymous or even identical.
       If you want to return reference or pointer, that object that you want return as a reference or a pointer
       SHOULD OUTLIVE its enclosing scope.

    7. To return an object as a reference or a pointer, the object should outlive its enclosing scope.

    That being said "outlive its enclosing scope," we cannot return a local stack-oriented object
    either as a reference or a pointer.




    

*/
class MyClass
{
    private:
        int m_data{};

    public:

        MyClass() = default;

        // returns lvalue reference, and does not have trailing const specifier
        int& get_data() // no const specifier here
        {
            tpf::console cout;
            auto nl = tpf::output::nl();
            cout << "Inside the function: " << FUNCTION_NAME << nl;
            cout << "The type of \"this\": " << Tpf_GetTypeCategory(this) << nl;
            return this->m_data;
        }

        // it returns const lvalue reference, and has trailing const specifier
        const int& get_data() const // const here
        {
            tpf::console cout;
            auto nl = tpf::output::nl();
            cout << "Inside the function: " << FUNCTION_NAME << nl;
             cout << "The type of \"this\": " << Tpf_GetTypeCategory(this) << nl;

            return this->m_data;
        }

        // it returns pure rvalue int, also has const specifier
        // int get_data(int offset) const // trailing const specifier
        // {
        //     return this->m_data + offset;
        // }

         // it returns pure rvalue int, also has const specifier
        int get_data(int offset) const // trailing const specifier
        {
            int r = this->m_data + offset;

            return r; // we are returning a local stack variable
                        // so, we cannot return a reference.
        }   

        ~MyClass()
        {
            std::cout <<"Okay, I am safely destroyed..." << std::endl;
        }
};

void test_which_member_function_gets_called()
{
    MyClass nco; // non-const MyClass object

    nco.get_data(); // int& get_data() is called

    const MyClass co; // const MyClass object

    co.get_data(); // const int& get_data() const is called
}

void test_pointer_that_points_to_const_object()
{
    MyClass nco; // non-const object;

    // ptr is "non-const pointer in true sense of const"
    // ptr is a pointer that points to a const object
    const MyClass* ptr; // is this a const pointer?
                        // SSTS: this is not a const pointer.
                        //
                        // But we the C++ programmers call it "const pointer"
                        // why?

    // MyClass* const cptr; // does this work?
                        // NO! It does not work. Why?
                        //
                        // In our previous episode, we learned that anything that is "const" or "reference"
                        // should be initialized at definition.
                        // We have not initialized it at definition, so the C++ compiler does not compile it at all.

    // cptr is "a const pointer in true sense of const"
    // cptr is a const pointer that points to a non-const object.
    MyClass* const cptr = &nco; // we now initialized const pointer cptr with non-const object nco.

    // When we say "const pointer," do we mean "a pointer that points to a const object, as with ptr,"
    // or "a const pointer that points to a non-const object as with cptr?"
    // which one do we mean "by const pointer?"
    
    /*
        Pointers are meant to be used by freely changing its value, or the address that it points to.
        If we cannot change the value of the pointer, the address it points to, what's the use of the pointer?

        "const pointer in true-sense of const as with cptr" is rarely used in C++ programming. For such purpose,
        we can use reference. As I said in my previous videos, "all references" are constant, which means
        once intialized at definition, it can never change the object that it references to.

        So, when we say "const pointer," we usually do not mean "const pointer in true-sense of const as with cptr,"
        but we rather mean the pointer that points to an object, through this pointer we do not want to modify
        the value of the object that it points to.
    */

}

MyClass& get_a_reference()
{
    MyClass obj; // this is a stack oriented object.
                // it gets destroyed as soon as this block or scope ends.
                // so we cannot return this stack based object as reference.

    // dynamically allocated memory outlives this function block.
    // so, we can safely return this object as reference.
    MyClass *ptr = new MyClass;

    return *ptr;
}

void test_get_a_reference()
{
    MyClass& ref = get_a_reference();

    std::cout << "Okay, I caught a referenced!!" << std::endl;

    // destory the memory in the free-store, or is called "heap memory."
    delete &ref;
}

MyClass* legal_return_value(MyClass& object)
{
    return &object; // legal, because object is a reference,
                    // this object will not get destoried at the end of this function block.
}

MyClass& legal_return_value_again(MyClass* ptr)
{
    return *ptr; // legal, because ptr outlives this function block.
}

MyClass& legal_but_suspicious(MyClass&& rvalue)
{
    return rvalue; // legal, but not desirable.
}

MyClass& legal_but_dangerous(const MyClass& const_lvalue_ref)
{
    // this is possible, but highly dangerous.
    return const_cast<MyClass&>(const_lvalue_ref);
}

int main()
{
    // test_which_member_function_gets_called();

    //test_pointer_that_points_to_const_object();

    test_get_a_reference();
}