#include <tpf_output.hpp>
#include <tpf_chrono_random.hpp>

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
        if (m_ptr)
        {
            if constexpr(std::is_pod_v<ElementType>)
                _aligned_free(this->m_ptr);
            else
                delete[] m_ptr;

            this->m_ptr = nullptr;
            this->m_size = 0;
        }
    }

    // Before we call this function,
    // existing memory should be released, otherwise memory leaks.
    //
    // the this->m_size should be greater than 0,
    // otherwise, UNDEFINED BEHAVIOR
    void new_alloc()
    {
        if constexpr( std::is_pod_v<ElementType>)
        {
            this->m_ptr = (ElementType*) _aligned_malloc(this->m_size * sizeof(ElementType), 16);

            if(!this->m_ptr)
                Tpf_ThrowDebugException("Allocation Failed");
        }
        else
        {
            try
            {
                // operator new throws std::bad_alloc if allocation fails
                this->m_ptr = new ElementType[this->m_size];
            }
            catch(const std::bad_alloc&)
            {
                Tpf_ThrowDebugException("Allocation Failed");
            }
        }
    }
 
    void invalidate()
    {
        this->m_ptr = nullptr;
    }

    inline void copy_memory(ElementType* right_hand_side_m_ptr)
    {
        if constexpr ( std::is_pod_v<ElementType> )
            // if ElementType is a POD, then we can use memcpy()
            std::memcpy(this->m_ptr, right_hand_side_m_ptr, m_size * sizeof(ElementType));
        else
        {
            // if ElementType is NOT a POD
            // then we have to copy element-wise
            for(size_t i = 0; i < this->m_size; ++i)
                this->m_ptr[i] = right_hand_side_m_ptr[i];
        }
    }

public:
    
    // the pointer pointing to the first element
    iterator begin() 
    { 
        return this->m_ptr; 
    }
    
    // the pointer pointing to the element after the last element
    iterator end() 
    { 
        return (this->m_ptr + this->m_size); 
    }

    const_iterator cbegin()
    {
        // we actually do NOT need the cast,
        // but I want to make the syntax clear
        return static_cast<const_iterator>(this->m_ptr);
    }

    const_iterator cend()
    {
        // we actually do NOT need the cast,
        // but I want to make the syntax clear
        return static_cast<const_iterator>(this->m_ptr + this->m_size);
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

    // we assume count is greater than zero
    void resize(size_t count)
    {
        if(count == this->m_size)
            return;
        else
        {
            // free existing memory
            this->free_memory();
            this->m_size = count;
            
            // if allocation fails, this function throws exception
            new_alloc();
        }
    }

    // this is dangerous practice
    // if dynamic allocation fails,
    // this constructor throws std::bad_alloc exception
    explicit dynamic_array(size_t size = 1)
        : m_size{size}
    {
        new_alloc();
    }

    // if dynamic allocation fails,
    // it throws exception
    dynamic_array(const dynamic_array &right_hand_side) : 
        m_size{right_hand_side.m_size}
    {
        new_alloc();
        copy_memory(right_hand_side.m_ptr);
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

                copy_memory(right_hand_side.m_ptr);
            }
            else
            {
                // we have free existing memory
                this->free_memory();
                this->m_size = right_hand_side.m_size;
                
                new_alloc();
                copy_memory(right_hand_side.m_ptr);
            }
        }

        return *this;
    }

    dynamic_array(dynamic_array &&right_hand_side) noexcept
        : m_size{right_hand_side.m_size},
          m_ptr{right_hand_side.m_ptr}
    {
        // IMPORTANT: invalidate right_hand_side
        // after move, right_hand_side is invalide
        // so, we should NOT access to right_hand_side after move operation
        right_hand_side.invalidate();
    }

    dynamic_array &operator=(dynamic_array &&right_hand_side) noexcept
    {
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
       this->free_memory();
    }

    friend std::ostream& operator<<(std::ostream& os, const dynamic_array& da)
    {
        size_t size = da.size() - 1;
        os << "{ ";

        for(size_t i = 0; i < size; ++i)
            os << da[i] << ", ";

        os << da[size] << " }";
    
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

/*
    POD: Plain Old Data Type

    POD is C style struct or C++ Primitive Data Type
*/

// if we are to use SSE, we need to align our data 16 bytes
struct alignas(16) vector_3d_pod
{
    float m_x, m_y, m_z, m_color;
    
    // POD type can have non-virtual member function
    // so, this struct is still a POD type
    void set(float x, float y, float z)
    {
        m_x = x; m_y = y; m_z = z;
    }
};

std::ostream& operator<<(std::ostream& os, const vector_3d_pod& v)
{
    os << "< " << v.m_x << ", " << v.m_y << ", " << v.m_z <<" >";
    return os;
}

struct alignas(16) vector_3d_not_pod
{
    float m_x, m_y, m_z, m_color;
    
    virtual void set(float x, float y, float z)
    {
        m_x = x; m_y = y; m_z = z;
    }

    // if a class or struct has a custom destructor
    // other than default destructor provided by the compiler
    // then it is not a POD
    ~vector_3d_not_pod()
    { }
};

std::ostream& operator<<(std::ostream& os, const vector_3d_not_pod& v)
{
    os << "< " << v.m_x << ", " << v.m_y << ", " << v.m_z <<" >";
    return os;
}

struct it_is_still_not_pod
{
    size_t m_x, m_y, m_z, m_color;

    // if a class or struct has a data member
    // that is not a POD, that the class or struct is also NOT a POD
    std::unique_ptr<int> ptr;
};

void test_pod_types()
{
    stream << "vector_3d_pod is pod: " << std::is_pod_v<vector_3d_pod> << endl;
    stream << "vector_3d_not_pod is pod: " << std::is_pod_v<vector_3d_not_pod> << endl;
    stream << "it_is_still_not_pod is pod: " << std::is_pod_v<it_is_still_not_pod> << endl;
    stream << "int* is pod: " << std::is_pod_v<int*> << endl;

    /*
        C++ primitive types such as char, int, double, double*, etc. 
        are POD type.

        we can use memcpy() to copy one POD to another POD.
    */
}

template<typename ElementType>
void benchmarking_with_primitive_type(size_t test_count, size_t element_count)
{
    stream << "Benchmarking with type: "
        << tpf::types::type_list_t<ElementType>{} << endl << endl;

    stream << "With std::vector"<< tpf::types::type_list_t<ElementType>{}
        << " -  Test Count: " << test_count
        << " - Element Count: " << element_count << endl;

    // define stop watch
    tpf::chrono_random::stop_watch sw;

    for(size_t i = 0; i < test_count; ++i)
    {
        // allocate memory at one sweep
        std::vector<ElementType> v(element_count);

        for(size_t j = 0; j < element_count; ++j)
            v[j] = (ElementType)j;

        if(element_count < 21)
            stream << v << endl;
    }

    stream << "... Elasped: " << sw.elapsed_time() << endl << endl;

    /////////////////////////////////////////////////////////
    stream << "With dynamic_array"<< tpf::types::type_list_t<ElementType>{}
        << " -  Test Count: " << test_count
        << " - Element Count: " << element_count << endl;

    // reset stop watch
    sw.reset();

    for(size_t i = 0; i < test_count; ++i)
    {
        // allocate memory at one sweep
        dynamic_array<ElementType> v(element_count);

        for(size_t j = 0; j < element_count; ++j)
            v[j] = (ElementType)j;

        if(element_count < 21)
            stream << v << endl;
    }

    stream << "... Elasped: " << sw.elapsed_time() << endl << endl;
}

template<typename ElementType>
void benchmarking_with_class_type(size_t test_count, size_t element_count)
{
    stream << "Benchmarking with type: "
        << tpf::types::type_list_t<ElementType>{} << endl << endl;

    stream << "With std::vector"<< tpf::types::type_list_t<ElementType>{}
        << " -  Test Count: " << test_count
        << " - Element Count: " << element_count << endl;

    // define stop watch
    tpf::chrono_random::stop_watch sw;

    for(size_t i = 0; i < test_count; ++i)
    {
        // allocate memory at one sweep
        std::vector<ElementType> v(element_count);

        for(size_t j = 0; j < element_count; ++j)
            v[j].set((float)j, (float)j, (float)j);

        if(element_count < 21)
            stream << v << endl;
    }

    stream << "... Elasped: " << sw.elapsed_time() << endl << endl;

    /////////////////////////////////////////////////////////
    stream << "With dynamic_array"<< tpf::types::type_list_t<ElementType>{}
        << " -  Test Count: " << test_count
        << " - Element Count: " << element_count << endl;

    // reset stop watch
    sw.reset();

    for(size_t i = 0; i < test_count; ++i)
    {
        // allocate memory at one sweep
        dynamic_array<ElementType> v(element_count);

        for(size_t j = 0; j < element_count; ++j)
             v[j].set((float)j, (float)j, (float)j);

        if(element_count < 21)
            stream << v << endl;
    }

    stream << "... Elasped: " << sw.elapsed_time() << endl << endl;
}

int main()
{
    // examples_for_dynamic_array();
    // do_not_do_this();

    // is_still_is_not_perfect();

   // please_use_this_method();

    // test_iterators_dynamic_array();

    // why_explicit_to_constructor();

    // test_pod_types();

    benchmarking_with_primitive_type<int>(10, 10'000'000);

    benchmarking_with_primitive_type<float>(20, 10'000'000);

    benchmarking_with_primitive_type<double>(5, 10'000'000);

    benchmarking_with_class_type<vector_3d_pod>(5, 1'000'000);

    benchmarking_with_class_type<vector_3d_not_pod>(5, 1'000'000);
}