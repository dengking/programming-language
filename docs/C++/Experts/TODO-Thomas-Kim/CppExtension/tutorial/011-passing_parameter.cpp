#include <tpf_output.hpp>
#include <future>
#include <thread>
#include <memory>

/*
    If you are my audience, do NOT pollute global scope.
    Instead use static member fields or static member functions.

    Always enclose multithreading code in try-catch construct.

    Please understand the intentions of the C++ Standard Committee.
    If you ever have to pass the parameter using call-by-reference semantic,
    use std::cref() for const reference,
    use std::ref() for non-const reference.
    In such case, the lifetime of the parameter should
    OUTLIVE the scope of the thread function.

    We cannot PASS std::unique_ptr or std::shared_ptr 
    to a thread function in NORMAL function-call syntax.

    Because the smart pointers such as 
    std::unique_ptr and std::shared_ptr do not have copy-constructor,
    and std::async's function-call parameters should be
    convertible to Rvalue Reference, 
    So, we can try it using std::move. std::move() typecasts to
    rvalue reference. After std::move(), regard that parameter
    as INVALID.

    If we want to use the smart pointers such as std::unique_ptr, 
    std::shared_ptr after passing to a thread function,
    then we can pass the smart pointer using call-by-reference.

    NOTE THAT:
        We are passing parameters from one thread to another thread.
        We will learn more about fundamentals of C++ Threading Technology in future sessions.
        
*/

struct stupidly_long_descriptive_class_name
{
    using return_t = int;

    static return_t call_by_value_sum(int a, int b)
    {
        return a + b;
    } 

    static return_t call_by_const_reference_value_sum(const int& a, const int& b)
    {
        return a + b;
    }

    static return_t call_by_reference_sum(int& a, int& b)
    {
        return a + b;
    }

    static return_t call_by_pointer(int* ap, int* bp)
    {
        return (*ap) * (*bp);
    }

    static return_t call_by_unique_ptr(std::unique_ptr<int> ua, 
        std::unique_ptr<int> ub)
    {
        return (*ua) / (*ub);   
    }

    static return_t call_by_shared_ptr(std::shared_ptr<int> sa, 
        std::shared_ptr<int> sb)
    {
        return (*sa) / (*sb);   
    }

    static return_t passing_raw_pointer_the_better_way(int* ra, int* rb)
    {
        std::unique_ptr<int> ua{ra};
        std::unique_ptr<int> ub{rb};

        return *ua + *ub;

        // after this block, memory for ra and rb are deleted.
    }

    static return_t passing_unique_ptr_by_reference(std::unique_ptr<int>& ua,
        std::unique_ptr<int>& ub)
        {
            return (*ua) * (*ub);
        }
};

void examples_for_passing_parameters()
{
    try
    {
        using sld = stupidly_long_descriptive_class_name;

        auto call_by_value_sum = std::async(&sld::call_by_value_sum, 1, 2);


        int n = 4, m = 5;

        // this is okay, but it is call-by-value, not call-by-const-reference
        // auto call_by_const_reference_value_sum = std::async(&sld::call_by_const_reference_value_sum, n, m);

        // we are now passing n and m by call_by_const_reference
        auto call_by_const_reference_value_sum = 
            std::async(&sld::call_by_const_reference_value_sum, std::cref(n), std::cref(m));
        
        // this does not compile
        // auto call_by_reference_sum = std::async(&sld::call_by_reference_sum, n, m);

        // Passing parameters call-by-reference is A DANGEROUS PRACETICE
        // So, C++ committee wanted C++ programmers pay special attention
        // when passing parameters by reference to thread functions.
        //
        // But still we may need to pass parameters to thread functions
        // using call-by-reference semantic. To address this issue,
        // C++ committee created std::cref() for const reference,
        //                       std::ref() for non-const reference
        //                       to pass parameter call-by-reference semantic

        // now we are passing n and m using call-by-reference
        auto call_by_reference_sum = 
            std::async(&sld::call_by_reference_sum, std::ref(n), std::ref(m));

        tpf::sstream stream;

        stream << "call_by_value_sum with 1, 2: " 
            << call_by_value_sum.get() << tpf::endl;

        stream << "call_by_const_reference_value_sum with 4, 5: " 
            << call_by_const_reference_value_sum.get() << tpf::endl;

        stream << "call_by_reference_sum with 4, 5: " 
            << call_by_reference_sum.get() << tpf::endl;

        auto call_by_pointer = std::async( &sld::call_by_pointer, &n, &m);

        std::unique_ptr<int> ua{new int{12}};
        std::unique_ptr<int> ub{new int{6}};

        auto call_by_unique_ptr = std::async(&sld::call_by_unique_ptr, 
            std::move(ua), std::move(ub));

        std::shared_ptr<int> sa{new int{12}};
        std::shared_ptr<int> sb{new int{6}};

        auto call_by_shared_ptr = 
            std::async(&sld::call_by_shared_ptr, std::move(sa), std::move(sb));

        stream << "call_by_pointer with 4, 5: " 
            << call_by_pointer.get() << tpf::endl;

        stream << "call_by_unique_ptr with 12, 6: " 
            << call_by_unique_ptr.get() << tpf::endl;

        /*
            If you pass unique_ptr to another function,
            that pass parameter is INVALID after the function call.
            So, at this point of the code

            Dereferencing unque_ptr after passing to another function,
            that unique_ptrs are INVALID.

            *ua, *ub are INVALID operation.

            In case of unique_ptr, after std::move(),
            the parameter is INVALID.
        */

        stream << "call_by_shared_ptr with 12, 6: " 
            << call_by_shared_ptr.get() << tpf::endl;

        /*
            If you want to access the object the pointer is pointing to,
            you have to pass the pointer using shared_ptr.

            So, after passing shared_ptr, you can still access that parameter.

            After passing shared_ptrs to another function,
            you can still access that shared_ptrs.

            *sa, *sb are VALID.

            In case of shared_ptr, Yes, shared_ptr also has move constructor,
            so, after std::move(shared_ptr), that parameter is also INVALID.

        */


        // this is okay, no memory leaks
        auto passing_raw_pointer_the_better_way = 
            std::async( &sld::passing_raw_pointer_the_better_way, new int{5}, new int{4});

        stream << "passing_raw_pointer_the_better_way with 5, 4: " 
            << passing_raw_pointer_the_better_way.get() << tpf::endl;


        std::unique_ptr<int> uua{ new int{5} };
        std::unique_ptr<int> uub{ new int{4} };

        auto passing_unique_ptr_by_reference = 
            std::async( &sld::passing_unique_ptr_by_reference, std::ref(uua), std::ref(uub));   

        stream << "passing_unique_ptr_by_reference with 12, 6: " 
            << passing_unique_ptr_by_reference.get() << tpf::endl;

        /*
            *ua, *ub are still VALID.
            Because we passed ua and ub as lvalue reference,
            we have not disowned ua and ub using std::move().           
        */

       *uua = 10;
       *uub = 2;

       stream <<"*uua, *uub are still VALID operation: " << (*uua + *uub) << tpf::endl;
    }
    catch(const std::exception& e)
    {
        tpf::sstream stream;
        stream << e << tpf::endl;
    }

}

int main()
{
    examples_for_passing_parameters();
}