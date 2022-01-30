#include <iostream>

/*
    In C++, we always have to INITIALIZED const and reference entities.

    Once again, we always have to INITIALIZED const entities and reference entities.

    An entity is an object or not an object.

    An object in C++ has its own memory.

    ONCE again, if you remember that const entities and reference entities should
    always be initialized at definition.

    If you do not initialize at definition, that is neither a const nor a reference.
*/

void examples_for_const_pointer()
{
    int n; // we have not initialized
            // n is neither const nor reference

    // const int c; // syntax error.
                // because const entity should be initialized
                // but not initialized at definition

    const int c = 5; // c is an entity, not yet an object
                      // in C++, const entity may or may not exist at run-time.

    /*
        In C++, keyword "const" was first introduced to replace macros, which are
        type-agnostic or type-ignorant.

        Anything that should be initialized at definition is in some way CONSTANT.

        That is, all reference types are constant... because they should always be
        initialized at definition.
    */

    // int& r; // syntax error, because reference is CONSTANT
    int& r = n;

    // const int& cr; // syntax error, because reference is CONSTANT

    const int& cr = n; // what does "const" in "const int&" means?

    /*
        "const" int "const int&" does not mean the reference is constant.
        because all reference in C++ are already constant.
        "const" in "const int&" does not mean cr can reference to const int.

        "const" in "const int&" MEANS the value that the reference cr
        references to cannot be modified.

        Once again, "const" in "const int&" DOES NOT MEAN 
        the reference cr references to "const int", but it rather means
        the value that the reference cr references to is NOT MODIFIABLE.

        Understanding this point is REALLY IMPORTANT.
    */

    // cr = 5; // syntax error, because cr references to an int n
            // but it was declared with const int&,
            // the "const" in "const int&" means
            // the value cr references to cannot be modified.

        
    // All references in C++, be it lvalue reference or rvalue reference,
    // they cannot be modified once declared and initialized.

    double d1, d2;

    double& rd1 = d1;

    rd1 = d2; // ? what does it mean?
            // it means, copy the value of d2 to the object d1 to which the
            // reference rd1 references.
            // Here, we copied the value and assigned to rd1, which
            // references to d1, we modified the value the reference rd1 references to.
            // We have NOT modified the value of rd1.
            // We changed the value that the reference rd1 references to.

    // rd1 = &d2; // it is syntax error, the ampersand & is address-of operator,
                // which returns the address of d2,
                // we cannot assign the address of d2 to rd1.
    rd1 = (int&) d2; // this is not a syntax error,
                     // but rd1 = (int&)d2 is equivalent to rd1 = d2,
                     // that is, we copied the value of d2 to the object
                     // that the reference rd1 references to, which is
                     // d1.

    /*
        Once and for all, all reference types is initialized at definition,
        and cannot reference to something else.
        In this sense, all references are CONSTANT, which does not mean
        we cannot change the value that the reference references to.

        Do not get confused.
    */ 

    float f1 ;
    const float cf = 5.0f; // At this point, cf is NOT an object,
                            // that is, no memory is allocated for cf.
                            // keyword "const" was originally introduced to C++
                            // to replace type-agnostic #define macros.
                            // Initial C++ design for keyword "const"
                            // was meant to be used in place of #define macros

    const float& cfr1 = f1; // "const float&" can be initialized with non-const float
    const float& cfr2 = cf; // "const float&" can be initialized with const float

    float& rf1 = f1; // okay, "float&" can be initialized with non-const float
    // float& rf2 = cf; // syntax error. we cannot normally initialize 
                     // "non-const" reference, float&, with const float.
    /*
        If we want to initialize "non-const" reference with const entity,
        we HAVE TO USE type cast, const_cast<type&>(entity) syntax, as below:
    */

   float& rf2 = const_cast<float&>(cf); // PERFECT.
                                        // Now at this point, some magic happens.
                                        // Since const float cf is now "referenced"
                                        // here "referenced" means the address of the 
                                        // "referenced" entity is required, so
                                        // C++ compiler generates memory for 
                                        // const float cf. From now on, cf is
                                        // a full-blown C++ object, because it now has its
                                        // own memory.

    /*
        In the C++ Community, the colloquial term "non-const reference" means that
        we can alter the value the reference is referencing to. It does not mean
        the reference is referencing to a non-const object.

        In the same manner, the colloquial term "const reference" means 
        we cannot alter the value the reference is referencing to. It does NOT mean
        the reference is referencing to a const object.

        In C++ Language and Community, misnomers and ill-coined technical terms are 
        ABUNDANT. Possibly because of historical reasons. For example, "volatile" objects
        are not volatile at all. It rather means the CPU should read or fetch the 
        value of the object from MEMORY (or RAM) each time it needs access to it.

        "Const expression" rather means "compile-time expression."

        
    */                          
   
}

int main()
{
    examples_for_const_pointer();
}