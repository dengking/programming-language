#include <tpf_output.hpp>
#include <cstdlib>
#include <iterator>

tpf::sstream stream;
auto endl = tpf::endl;

template<typename ElementType>
class dynamic_array
{
public:
    using iterator = ElementType*;

    // don't do this
    // using const_iterator = const iterator; 
    // it will define ElementType* const, it is wrong

    using const_iterator = const ElementType*;

private:
    size_t m_size;
    ElementType *m_ptr;

    void free_memory()
    {
        if (this->m_size != 0 || m_ptr != nullptr)
        {
            delete[] m_ptr;
            this->m_ptr = nullptr;
            this->m_size = 0;
        }
    }

    void invalidate()
    {
        this->m_ptr = nullptr;
        this->m_size = 0;
    }

public:
    
    // the pointer pointing to the first element
    iterator begin() 
    { 
        // return this->m_ptr; 
        return &this->m_ptr[0];
    }
    
    // the pointer pointing to the element after the last element
    iterator end() 
    { 
        // return (this->m_ptr + this->m_size); 
        return &this->m_ptr[this->m_size];
    }

    const_iterator cbegin()
    {
        // we actually do NOT need the cast,
        // but I want to make the syntax clear
        return static_cast<const_iterator>(&this->m_ptr[0]);
    }

    const_iterator cend()
    {
        // we actually do NOT need the cast,
        // but I want to make the syntax clear
        return static_cast<const_iterator>(&this->m_ptr[this->m_size]);
    }

    auto rbegin()
    {
        return std::reverse_iterator(end());
    }

    auto rend()
    {
        return std::reverse_iterator(begin());
    }

    auto crbegin()
    {
        return std::reverse_iterator(cend());
    }
    
    auto crend()
    {
        return std::reverse_iterator(cbegin());
    }

    size_t size() const { return this->m_size; }

    void resize(size_t count)
    {
        if(count < 1)
            Tpf_ThrowDebugException("count should be greater than 0");
        else if(count == this->m_size)
            return;
        else
        {
            // free existing memory
            this->free_memory();

            try
            {
                this->m_size = count;
                this->m_ptr = new ElementType[this->m_size];
            }
            catch(...)
            {
                Tpf_ThrowDebugException("Dynamic Allocation Failed");
            }
        }
    }

    // this is dangerous practice
    // if dynamic allocation fails,
    // this constructor throws std::bad_alloc exception
    explicit dynamic_array(size_t size = 1)
        : m_size{size}
    {
        stream << "Default constructor called" << endl;

        try
        {
            this->m_ptr = size > 0 ? new ElementType[size] : nullptr;
        }
        catch(...)
        {
            // it reports details about allocation failture
            Tpf_ThrowDebugException("Dynamic Allocation Failed");
        }       
    }

    // if dynamic allocation fails,
    // it throws exception
    dynamic_array(const dynamic_array &right_hand_side) : 
        m_size{right_hand_side.m_size}
    {
        stream << "Copy constructor called" << endl;

        try
        {
            this->m_ptr = this->m_size > 0 ? new ElementType[this->m_size] : nullptr;

            if (m_size > 0)
                std::memcpy(this->m_ptr, right_hand_side.m_ptr, m_size * sizeof(ElementType));
        }
         catch(...)
        {
            // it reports details about allocation failture
            Tpf_ThrowDebugException("Dynamic Allocation Failed");
        }

    }

    ElementType *operator&() { return this->m_ptr; }

    ElementType &operator[](size_t index) { return this->m_ptr[index]; }

    const ElementType &operator[](size_t index) const { return this->m_ptr[index]; }

    ElementType &at(size_t index)
    {
        if (index < this->m_size && this->m_ptr != nullptr)
            return this->m_ptr[index];
        else
            Tpf_ThrowDebugException("Index Out of Range");
    }

    const ElementType &at(size_t index) const
    {
        if (index < this->m_size && this->m_ptr != nullptr)
            return this->m_ptr[index];
        else
            Tpf_ThrowDebugException("Index Out of Range");
    }

    // if allocation fails,
    // it can throw std::bad_alloc exception
    dynamic_array &operator=(const dynamic_array &right_hand_side)
    {
        stream << "Copy assignment operator() called" << endl;

        // this code is wrong, because address of operator&()
        // is defined for class dynamic_array,
        // so, we cannot use the following code any longer
        // if(this != &right_hand_side)

        if (this != std::addressof(right_hand_side))
        {
            if (this->m_size == right_hand_side.m_size)
            {
                // we do not need to reallocate memory
                // and also, we do not need to free memory
                // this->free_memory()

                if (m_size > 0)
                    std::memcpy(this->m_ptr, right_hand_side.m_ptr, m_size * sizeof(ElementType));
            }
            else
            {
                // we have free existing memory
                this->free_memory();
                this->m_size = right_hand_side.m_size;

                if (this->m_size > 0)
                {
                    try
                    {
                        // and we have to reallocate new memory
                        this->m_ptr = new ElementType[this->m_size];
                        std::memcpy(this->m_ptr, right_hand_side.m_ptr, m_size * sizeof(ElementType));
                    }
                    catch(...)
                    {
                        // it reports details about allocation failture
                        Tpf_ThrowDebugException("Dynamic Allocation Failed");
                    }
                }
            }
        }

        return *this;
    }

    dynamic_array(dynamic_array &&right_hand_side) noexcept
        : m_size{right_hand_side.m_size},
          m_ptr{right_hand_side.m_ptr}
    {
        stream << "Move constructor called" << endl;

        // IMPORTANT: invalidate right_hand_side
        // after move, right_hand_side is invalide
        // so, we should NOT access to right_hand_side after move operation
        right_hand_side.invalidate();
    }

    dynamic_array &operator=(dynamic_array &&right_hand_side) noexcept
    {
        stream << "Move assignment operator() called" << endl;

        if (this != std::addressof(right_hand_side))
        {
            // delete existing memory
            this->free_memory();
            this->m_size = right_hand_side.m_size;
            this->m_ptr = right_hand_side.m_ptr;

            // IMPORTANT: invalidate right_hand_size after move assignment
            // since after move operation, right_hand_side is invalid
            // so, we should NOT access to right_hand_side after move operation
            right_hand_side.invalidate();
        }

        return *this;
    }

    ~dynamic_array()
    {
        stream << "Destructor called" << endl;

        this->free_memory();
    }

    friend std::ostream& operator<<(std::ostream& os, const dynamic_array& da)
    {
        if(da.size() < 1)
            os << "{ }";
        else
        {
            size_t size = da.size() - 1;
            os << "{ ";

            for(size_t i = 0; i < size; ++i)
                os << da[i] << ", ";

            os << da[size] << " }";
        }

        return os;
    }
};

void examples_for_dynamic_array()
{
    dynamic_array<int> a{10};

    for(size_t i = 0; i < a.size(); ++i)
        a[i] = (int)i;

    stream << "a = " << a << endl;
}

void do_not_do_this()
{
    size_t count = 2;

    std::vector< dynamic_array<int> > jagged_array(count); // don't do this

    stream << "At this point, default constructor of dynamic_array is called "
        << count << " times.\n"<< endl;

    for(size_t i = 0; i < jagged_array.size(); ++i)
        jagged_array[i] = dynamic_array<int>{ i + 1 };

    stream << "In the for loop, default constructor of dynamic_array is called "
        << count << " times, and move constructor is called " << count << " times\n" << endl;

    stream << "So, destructor is called total: " << (count + count) << " times " << endl;
}

void is_still_is_not_perfect()
{
    stream << "\nthis is better, but not perfect!!\n" << endl;

    size_t count = 2;

    std::vector< dynamic_array<int> > jagged_array;
    jagged_array.reserve(count);

    for(size_t i = 0; i < jagged_array.capacity(); ++i)
        jagged_array.emplace_back(dynamic_array<int>{ i + 1 });
}

// we never created temporary dynamic array in this function
void please_use_this_method()
{
    stream << "\nthis is PEREFECT WAY, YOU SHOULD ALWAYS USE THIS METHOD!!\n" << endl;

    size_t count = 10;

    std::vector< dynamic_array<int> > jagged_array;
    jagged_array.reserve(count);

    for(size_t i = 0; i < jagged_array.capacity(); ++i)
    {
        jagged_array.emplace_back(i + 1);  
        // i + 1 is argument for dynamic_array.

            // now std::vector creates dynamic_array
            // internally, so, no move... only count times of default
            // constructor of dynamic_array is called.

        for(size_t j = 0; j < jagged_array.back().size(); ++j)
            jagged_array.back()[j] = (int)j;
    }

    // note that the constructor of dynamic_array{size_t}

    for(auto& da: jagged_array)
    {
        stream << da << endl;
    }
}

void test_iterators_dynamic_array()
{
    // we dynamically allocated 10 elements
    dynamic_array<int> array{ 10 };

    int i = 0;

    for(auto& e: array)
        e = i++;

    stream << array << endl;

    tpf::sstream out;

    std::for_each(array.rbegin(), array.rend(),
        [&out](auto& e)
        {
            out << e << ", ";
        });

    // if you don't endl with stream, you cannot see the output
    out << endl;
}

void why_explicit_to_constructor()
{
    dynamic_array<int> array{10};

    int i = -1;

    for(auto& e: array)
        e = ++i;

    stream << array << endl;

    // what does it mean?
    // array = 20;
    // stream << array << endl;
}

int main()
{
    // examples_for_dynamic_array();
    // do_not_do_this();

    // is_still_is_not_perfect();

    // please_use_this_method();

    // test_iterators_dynamic_array();

    why_explicit_to_constructor();
}