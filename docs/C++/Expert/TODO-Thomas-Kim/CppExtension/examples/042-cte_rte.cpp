#include <tpf_output.hpp>

tpf::sstream stream;
auto endl  = tpf::endl;
auto endL = tpf::endL;

class ClsObject
{
    private:
        bool m_valid{true};

        void cleanup()
        {
            if(m_valid)
            {
                std::cout <<"Object cleaned up" << std::endl;
            }
        }
        void invalidate()
        {
            this->m_valid = false;
        }

    public:
    
    ClsObject()
    {
        std::cout << "Object created" << std::endl;
    }

    void report()
    {
        stream <<"The address is " << this << endl;
    }

    ClsObject(const ClsObject&)
    {
        std::cout << "Object copy-constructed" << std::endl;
    }
    
    ClsObject& operator = (const ClsObject& rhs)
    {
        if(this != std::addressof(rhs))
        {
            std::cout << "Object copy-assigned" << std::endl;
        }

        return *this;
    }

    ClsObject(ClsObject&& rhs)
    {
        std::cout <<"Object moved" << std::endl;
        rhs.invalidate();
    }

    ClsObject& operator=(ClsObject&& rhs)
    {
        if(this != std::addressof(rhs))
        {
            std::cout << "Object move-assigned" << std::endl;
        }

        return *this;
    }

    ~ClsObject()
    {
        cleanup();
    }
};

void traits_of_reference_one()
{
    {
        ClsObject object;

        object.report();

        stream <<"-- Before entering inner block" << endl;

        {
            ClsObject& lvalue_ref = object;

            lvalue_ref.report();
            
            ClsObject&& rvalue_ref = std::move(object);

            rvalue_ref.report();

        }

        stream << "-- After leaving inner block" << endl;
        stream << "IMPORTANT - object is still valid in this scope" << endl;
        stream << "ClsObject&& rvalue_ref = std::move(object) -- has no effect." << endl;
    }

    stream <<"\n1. object, lvalue_ref, rvalue_ref are all the same." << endl;
    stream <<"   no copy constructor, no move constructor, no copy assignment, no move assignment" << endL;
}

void traits_of_reference_two()
{
    {
        ClsObject object;

        object.report();

        stream <<"-- Before entering inner block" << endl;

        {
             ClsObject& lvalue_ref = object;

            lvalue_ref.report();
            
            ClsObject&& rvalue_ref = std::move(object);

            rvalue_ref.report();

            ClsObject local_object = std::move(lvalue_ref);

        }

        stream <<"-- After leaving inner block" << endl;

        stream << "IMPORTANT: object is now invalid at this point" << endl;
        stream << "ClsObject local_object = std::move(lvalue_ref) - has serious effect." << endl;
    }

    stream <<"\n2. object, lvalue_ref, rvalue_ref are all the same." << endl;
    stream <<"   std::move invalidates the original object." << endl;
}
    


int main()
{
    traits_of_reference_one();
    traits_of_reference_two();
}