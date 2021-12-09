#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

// #include <iostream>
// #include <memory> // for unique_ptr and shared_ptr

// auto& stream = std::cout;
// auto endl = "\n";

// template<typename Type_1, typename Type_2>
// std::ostream& operator << (std::ostream& os, const std::unique_ptr<Type_1, Type_2>& ptr)
// {
//     if(ptr)
//         os << *ptr;

//     return os;
// }

// template<typename Type>
// std::ostream& operator << (std::ostream& os, const std::shared_ptr<Type>& ptr)
// {
//     if(ptr)
//         os << *ptr;

//     return os;
// }

void test_unique_ptr_and_shared_ptr_output()
{
    auto uptr = std::make_unique<int>(10);

    auto sptr = std::make_shared<int>(100);

    if (uptr)
        stream <<"*uptr = " << uptr << endl;

    if(sptr)
        stream <<"*sptr = " << sptr << endl;
    
}

void why_operator_overloading_for_unique_ptr_and_shared_ptr()
{
    using element_t = std::tuple<int, std::string>;

    using uptr_t = std::unique_ptr<element_t>;
    using container_t = std::vector<uptr_t>;

    size_t size = 10;

    container_t uptr_container; uptr_container.reserve(size);

    for(size_t i = 0; i < size; ++i)
    {
        tpf::sstream os;
        os << "Good Man + " << i ;

        auto t = element_t{(int)i, os.str() };
        uptr_container.emplace_back( std::make_unique<element_t>( std::move(t) ) );  
    }

    stream << uptr_container << endl;
}

void test_convert_unique_ptr_to_shared_ptr()
{
    auto uptr = std::make_unique<int>(10);

    stream << "uptr = " << uptr << endl;

    auto sptr = std::shared_ptr<int>( uptr.release() );

    stream << "sptr = " << sptr << endl;

    if(uptr)
    {
        stream << "Uptr is still valid... it's wrong " << endl;
    }
    else
    {
        stream << "Now uptr is invalid" << endl;
    }

    /*
    * How can we convert shared_ptr to unique_ptr?
    * 
    * The short answer is IMPOSSIBLE!
    * 
    * The long answer is STILL IMPOSSIBLE!
    * 
    * At best, we can recreate (or clone)
    * shared_ptr to unique_ptr 
    * */

   if(sptr.use_count()==1) // not sharing the pointer with other shared_ptr
   {
       // basically, we are creating a new instance of unique_ptr
       // using the value of the object that the shared_pter owns.
       uptr = std::make_unique<int>( *sptr );
       sptr = nullptr;     
   }

   if(uptr)
   {
       stream << "Now uptr is reconstructed: " << uptr << endl;
   }
   else
   {
       stream << "uptr is still invalid" << endl;
   }

   if(sptr)
   {
       stream << "Wow, sptr is still valid: " << sptr << endl;
   }
   else
   {
       stream << "Of course, sptr should be invalid" << endl;
   }
   
}

template<typename ElementType>
class smarter_ptr
{
    public:
        using uptr_t = std::unique_ptr<ElementType>;
        using sptr_t = std::shared_ptr<ElementType>;
        using usptr_t = std::variant<uptr_t, sptr_t>;

    private:
        usptr_t m_ptr;

    public:
        smarter_ptr(ElementType value = ElementType{}):
            m_ptr { std::make_unique<ElementType>(value) } { }

        // do not privde copy constructor
        smarter_ptr(const smarter_ptr&) = delete;
        smarter_ptr& operator=(const smarter_ptr&) = delete;

        smarter_ptr(smarter_ptr&& rhs): m_ptr{ std::move(rhs.m_ptr) } { }

        smarter_ptr& operator=(smarter_ptr&& rhs)
        {
            if(this != std::addressof(rhs))
            {
                this->m_ptr = std::move(rhs.m_ptr);
                
            }

            return *this;
        }

        bool is_unique_ptr()
        {
            if(auto ptr = std::get_if<uptr_t>(&this->m_ptr))
                return true;
            else
                return false;
        }

        bool is_shared_ptr()
        {
            if(auto ptr = std::get_if<sptr_t>(&this->m_ptr))
                return true;
            else
                return false;
        }

        ElementType* get() 
        {
            if(auto uptr = std::get_if<uptr_t>(&this->m_ptr))
                return uptr->get();
            else if(auto sptr = std::get_if<sptr_t>(&this->m_ptr))
                return sptr->get();
            else
                return nullptr;
        }

        smarter_ptr& operator = (ElementType value)
        {
            if(auto uptr = std::get_if<uptr_t>(&this->m_ptr))
            {
                // stream << "type of *uptr: " << Tpf_GetTypeCategory(*uptr) << endl;

                **uptr = value; 
                return *this;   
            }
            else if(auto sptr = std::get_if<sptr_t>(&this->m_ptr))
            {
                // stream << "type of *sptr: " << Tpf_GetTypeCategory(*sptr) << endl;
                
                **sptr = value; 
                return *this;
            }
            else
               return *this;
        }

        smarter_ptr& to_shared_ptr()
        {
            if(auto sptr = std::get_if<sptr_t>(&this->m_ptr))
                return *this;
            else if(auto uptr = std::get_if<uptr_t>(&this->m_ptr))
            {
                // converting from uptr_t to sptr_t
                this->m_ptr = sptr_t(uptr->release());
                return *this;
            }
            else
                return *this;
        }

        smarter_ptr& to_unique_ptr()
        {
            if(auto uptr = std::get_if<uptr_t>(&this->m_ptr))
                return *this;
            else if(auto sptr = std::get_if<sptr_t>(&this->m_ptr))
            {
                // sptr is pointer to shared_ptr
                // *sptr is type of shared_ptr
                // **sptr is the value the shared_ptr owns
                auto uptr = std::make_unique<ElementType>(**sptr);
                *sptr = nullptr;
                
                this->m_ptr = std::move(uptr);

                return *this;
            }
            else
                return *this;
        }

        friend tpf::sstream& operator << (tpf::sstream& os, const smarter_ptr& sp)
        {
            os << sp.m_ptr;
            return os;
        }
};

void test_smarter_ptr()
{
    smarter_ptr<int> sp{10};

    stream << "sp = " << sp << endl;

    stream <<"sp.is_unique_ptr()? " << sp.is_unique_ptr() << endl;

    sp.to_shared_ptr(); // convert uptr to shared ptr

    stream << "sp.is_shared_ptr()? " << sp.is_shared_ptr() << endl;

    stream << "sp.is_unique_ptr()? " << sp.is_unique_ptr() << endl;
 
    if(sp.is_shared_ptr())
    {
        stream << "Now, shared_ptr " << sp << endl;
    }
    else
    {
        stream << "Still not shared ptr " << endl;
    }

    sp = 30;

    stream << "The value of sp now ? " << sp << endl;

    sp.to_unique_ptr();

    stream <<"Restored back from shared_ptr: " << sp << endl;
}

int main()
{
    // test_unique_ptr_and_shared_ptr_output();

    // why_operator_overloading_for_unique_ptr_and_shared_ptr();

    // test_convert_unique_ptr_to_shared_ptr();

    test_smarter_ptr();
}

