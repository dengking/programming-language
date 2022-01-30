#include <iostream>

void examples_for_pointer_to_const_object()
{
    int n = 5;
    
    int* np1; // np1 is declared int*, where the star * , or is called
                // asterisk is a declarator, or a pointer declarator
                // since np1 is not initialized, it is not a const pointer

    int& ref = n; // initialize reference with n

    // SSTS: const int* is not "CONSTANT" pointer, but we call it just "constant pointer"

    const int* cp1; // is this a constant pointer?
                    // since we did not initialize, the VSC does not not squiggle
                    // cp1 is not a const pointer

    const char* msg; // it is not a const pointer either.

    int * ptr; // the star * means ptr is a type of pointer.
                // "int" in front of the star * has 3 implications;
                // 1. Arithmetic operation over ptr is based of size(int),
                // which happens to be 4 bytes.
                // ptr+1 means add 4 (bytes) to the value of ptr
                // ptr+2 means add 8 (bytes) to the value of ptr
                // 2. when the pointer object ptr is dereferenced using star *
                // this star * is an operator, dereferencing operator
                // "int" in front of star * instructs the CPU
                // read or fetch 4 bytes from memory.
                // 3. "int*" instructs the C++ compiler to implicitly 
                // implement array indexing operator []

    ptr = &n; // we assigned the address of n to the value of ptr

                // the expression ptr+1 is not the same to *ptr + 1

    const int& crf = 6;

    const int& ccref = n;


    std::cout << "The address of n  : "  << &n << std::endl;
    std::cout << "The address of ref: "  << &ref << std::endl;
    std::cout << "The value of ref  : "   << ref << std::endl;
    std::cout << "The address of ptr: "  << &ptr << std::endl;
    std::cout << "The value of n    : "  << n << std::endl;
    std::cout << "The value the ptr pointing to: " << *ptr << std::endl;
    std::cout << "The value of the ptr: " << ptr << std::endl;
    std::cout << "The address of crf: " << &crf << std::endl;
    std::cout << "The value of crf (or it references to): " << crf << std::endl;

    std::cout << "The address of ccref: " << &ccref << std::endl;
    std::cout << "The value of ccref (or it references to): " << ccref << std::endl;

    /*
        An object in C++ has special meaning. 
        It means... it has its own independent memory.
    */

    double d1 = 5.0;
    const double cd = 6.0;

    double* dp1 = &d1;
    double* dp2 = &const_cast<double&>(cd); // If we want to assign the address of const double to
                                            // a pointer of type double*, then we 1. have to remove the constness of the const double
                                            // using typecase const_cast<double&>(cd), 2. now that the address using address-of operator &
                                            // & const_cast<double&>(cd)

    const double* cdp1 = &d1;
    const double* cdp2 = &cd;

    cdp2 = &d1; // we modified the value of cp2 with a new address of another object of type double.
                // since we modified the value of cp2, "const" in "const double*" does not mean
                // the pointer cp2 is CONSTANT.

    // *cp1 = 6.7; // this does not work.
                // the dereference operator * in front of cp1, the pointer, means
                // the value of the object the pointer cp1 is pointing to.
                //
                // cp1 - is the value of cp1, which is the address of d1, or &d1
                // *cp1 - is the value of the object that the pointer is pointing to,
                //          in this case, the value of d1

    // The question now is what does "const" in "const double*" mean?

    /*
        1. "const" in "const double*" does NOT mean we can only assign the address of the const object of type double.
        2. "const" in "const double*" does NOT mean we cannot assign a new address of another object of type double.
            that is, the pointer of type "const double*" is NOT const pointer.

        "const" in "const double*" means that we cannot modify the value of the object
            the pointer is pointing to. It does not mean we cannot modify the value of the pointer itself.
    */

   int m = 5;

   int* mptr = &m;

   *mptr = 6; // it works successfully.

   const int* cmptr = &m;

   // *cmptr = 7; // it does not work.

   /*
        Colloquial terms in C++ Community:

        const int* ptr = &n;

        Strictly speaking, technically speaking or SSTS, ptr is not a CONSTANT pointer, but we say "constant pointer",
        we usually mean a pointer declared with const type* syntax.

        SSTS, *ptr is NOT the value of ptr, or the value of the pointer. It is the value of the object
        that the pointer is pointing to. But in C++ Community, we commonly call it "the value of the pointer"

        The Korean language is ECONOMIC, SCIENTIFIC, AND friendly to new technology and science.

        포인터값: just 4 syllables correctly deliver the meaning of the "value of the pointer"
        포인터 참조값: just 6 syllables correctly mean "the value of the object the pointer is pointing to."

        In this sense, Korean is science and technology friendly, which sadly is not with the English language.
   */
}

int main()
{
    examples_for_pointer_to_const_object();
}