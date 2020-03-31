#include <cstddef>
#include <cstring>
#include <iostream>
class rule_of_three
{
    char *cstring; // raw pointer used as a handle to a dynamically-allocated memory block

    void init(const char *s)
    {
        std::size_t n = std::strlen(s) + 1;
        cstring = new char[n];
        std::memcpy(cstring, s, n); // populate
    }

public:
    rule_of_three(const char *s = "")
    {
        std::cout << "construcotr" << std::endl;
        init(s);
    }

    ~rule_of_three()
    {
        delete[] cstring; // deallocate
    }

    rule_of_three(const rule_of_three &other) // copy constructor
    {
        std::cout << "copy constructor" << std::endl;
        init(other.cstring);
    }

    rule_of_three &operator=(const rule_of_three &other) // copy assignment
    {
        std::cout << "copy assignment" << std::endl;
        if (this != &other)
        {
            delete[] cstring; // deallocate
            init(other.cstring);
        }
        return *this;
    }
};

int main()
{

    rule_of_three a("rule of three"); // constructor
    rule_of_three a1();               // constructor
    rule_of_three b(a);               // copy constructor
    rule_of_three c = a;              // copy constructor
    c = b;                            // copy assignment
}