#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

class copy_or_move
{
   private:
    mutable bool m_invalid{false};

    void invalidate() 
    {
        this->m_invalid = true;
    }

    void cleanup()
    {
        if(this->m_invalid == false)
            stream <<"Cleaned up" << endl;
    }

   public:
    copy_or_move()
    {
        stream <<"Default constructor called" << endl;
    }

    copy_or_move(int n)
    {
        stream <<"Constructor(int n) called" << endl;
    }

    copy_or_move(const copy_or_move& rhs)
    {
        stream << "Copy constructor called" << endl;
    }

    copy_or_move& operator=(const copy_or_move& rhs)
    {
        stream <<"Copy assignment operator called" << endl;
        return *this;
    }

    copy_or_move(copy_or_move&& rhs) noexcept
    {
        stream << "Move constructor called" << endl;
        rhs.invalidate();
    }

    copy_or_move& operator=(copy_or_move&& rhs)
    {
        stream <<"Move assignment operator called" << endl;
        rhs.invalidate();

        return *this;
    }

    ~copy_or_move()
    {
        cleanup();
    }
};

copy_or_move copy_elision()
{
    return {1};
}

void test_copy_elision()
{
    auto obj =  copy_elision();
}

copy_or_move return_prvalue(int n = 2, int m = 1)
{
    auto obj = copy_elision();

    if(n > m)
    {
        return obj;
    }
    else
        return {1}; 
}

void test_which_constructor_gets_called()
{
    auto obj = return_prvalue();
}

template<typename ElementType, template<typename, typename...> class ContainerType>
class stl_extension: public ContainerType<ElementType>
{
    public:

        using base_container_type = ContainerType<ElementType>; 
        using ContainerType<ElementType>::ContainerType;
        using iterator = typename ContainerType<ElementType>::iterator;
        using size_type = typename ContainerType<ElementType>::size_type;

    public:

        auto begin_move_iterator() 
        { 
            return std::make_move_iterator<iterator>(base_container_type::begin());
        }

        auto end_move_iterator() 
        { 
            return std::make_move_iterator<iterator>(base_container_type::end());
        }

        auto index_to_iterator(size_type index) noexcept
        {
            auto itr = base_container_type::begin();
            std::advance(itr, index);
            return itr;
        }

        auto iterator_to_index(iterator itr)
        {
            return std::distance(base_container_type::begin(), itr);
        }

        auto interator_to_move_iterator(iterator itr) noexcept
        {
            return std::make_move_iterator<decltype(itr)>(itr);
        }      

        auto index_to_move_iterator(size_type index) noexcept
        {
            auto itr = base_container_type::begin();
            std::advance(itr, index);
            
            return std::make_move_iterator<decltype(itr)>(itr);
        }

    private:

        decltype(auto) internal_move_item(iterator itr)
        {
            return *std::make_move_iterator<decltype(itr)>(itr);
        }

        decltype(auto) internal_move_item(size_type index)
        {
            auto itr = base_container_type::begin();
            std::advance(itr, index);
            return *std::make_move_iterator<decltype(itr)>(itr);
        }

    public:     
    
        template<typename Type = ElementType>
        std::enable_if_t< std::is_same_v<Type, ElementType>
            && std::is_move_constructible_v<ElementType>, ElementType>
            move_front()
        {
            auto itr = base_container_type::begin();
            auto named_return_value = internal_move_item(itr);

            if constexpr( tpf::types::is_pop_front_available_v<base_container_type> )
                base_container_type::pop_front();
            else
                base_container_type::erase(itr);

            return named_return_value; // named return value optimization
        }      

        template<typename Type = ElementType>
        std::enable_if_t< std::is_same_v<Type, ElementType>
            && std::is_move_constructible_v<ElementType>, ElementType>
            move_back()
        {
            auto itr = base_container_type::end();
            std::advance(itr, -1); // last element

            auto named_return_value  = internal_move_item(itr);

            if constexpr( tpf::types::is_pop_back_available_v<base_container_type>)
                base_container_type::pop_back();
            else
                base_container_type::erase(itr);
                        
            return named_return_value; // named return value optimization
        }

        template<typename Type = ElementType>
        std::enable_if_t< std::is_same_v<Type, ElementType>
            && std::is_move_constructible_v<ElementType>, ElementType>
            move(typename base_container_type::iterator itr)
        {
            auto named_return_value = internal_move_item(itr);
            base_container_type::erase(itr);

            return named_return_value; // named return value optimization
        }

        template<typename Type = ElementType>
        std::enable_if_t< std::is_same_v<Type, ElementType>
            && std::is_move_constructible_v<ElementType>, ElementType>
            move(typename base_container_type::size_type index) 
        {
            return this->move(to_iterator(index)); // copy elision
        }

        template<typename Type = ElementType>
        std::enable_if_t< std::is_same_v<Type, ElementType>
            && std::is_move_constructible_v<ElementType>, ElementType>
            move(typename base_container_type::iterator itr_begin,
                 typename base_container_type::iterator itr_end)
        {
            auto named_return_value = 
                base_container_type{ make_move_iterator(itr_begin), make_move_iterator(itr_end) };

            base_container_type::erase(itr_begin, itr_end);

            return named_return_value; // named return value optimization
        }

        template<typename Type = ElementType>
        std::enable_if_t< std::is_same_v<Type, ElementType>
            && std::is_move_constructible_v<ElementType>, ElementType>
            move(typename base_container_type::size_type index_begin,
                 typename base_container_type::size_type index_end) noexcept(false) // CAN throw exception
        {
            return this->move(to_iterator(index_begin), to_iterator(index_end)); 
        }
};

template<typename ElementType, template<typename, typename...> class ContainerType>
using stl_extension_t = stl_extension<ElementType, ContainerType>;


void test_stl_extension()
{
    using ext_vector_t = stl_extension_t<copy_or_move, std::vector>;

    ext_vector_t vctr; vctr.reserve(10);

    vctr.emplace_back(1);
    vctr.emplace_back(2);

    stream << endl << endl;

    auto back = vctr.move_back();

    auto front = vctr.move_front();

    stream << "Size: " << vctr.size() << endl;
    
}



int main()
{
    // test_copy_elision();
    // test_which_constructor_gets_called();
    test_stl_extension();

    std::vector<int> v;

    v.back();

    v.pop_back();
}