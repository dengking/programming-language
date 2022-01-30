/**
 * @file tpf_matrix.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TPF_MATRIX_HPP
#define _TPF_MATRIX_HPP

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <type_traits>
#include <string>
#include <cstring>
#include <cstdlib>

#ifdef max
#define _TPF_MATRIX_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_MATRIX_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

/**
 * @brief Root namespace for C++ Extension Library
 * 
 */
namespace tpf
{
    /**
     * @brief Implements set operations
     * 
     */
    namespace matrix
    {
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
                    if constexpr (std::is_pod_v<ElementType>)
                        _aligned_free(this->m_ptr);
                    else
                        delete[] this-> m_ptr;

                    this->m_ptr = nullptr;
                }
            }

            void invalidate()
            {
                this->m_ptr = nullptr;
            }

            ElementType* new_alloc()
            {
                ElementType* ptr;

                if constexpr(std::is_pod_v<ElementType>)
                { 
                    ptr = (ElementType*) _aligned_malloc( sizeof(ElementType) * this->m_size, 16);
                }
                else
                {
                    try
                    {
                        ptr = new ElementType[this->m_size];
                    }
                    catch(const std::bad_alloc&)
                    {
                        ptr = nullptr;
                    }
                }

                return ptr;
            }

        public:
            
            // the pointer pointing to the first element
            iterator begin() 
            { 
                // return this->m_ptr; 
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
                    this->m_ptr = new_alloc();
                    
                    if(!this->m_ptr)
                        Tpf_ThrowDebugException("Dynamic Allocation Failed");
                }
            }

            // this is dangerous practice
            // if dynamic allocation fails,
            // this constructor throws std::bad_alloc exception
            explicit dynamic_array(size_t size = 1)
                : m_size{size}
            {
                this->m_ptr = new_alloc();
                
                if(!this->m_ptr)
                    Tpf_ThrowDebugException("Dynamic Allocation Failed");
            }

            // if dynamic allocation fails,
            // it throws exception
            dynamic_array(const dynamic_array &right_hand_side) : 
                m_size{right_hand_side.m_size}
            {
                this->m_ptr = new_alloc();
                
                if(!this->m_ptr)
                    Tpf_ThrowDebugException("Dynamic Allocation Failed");
                
                if constexpr(std::is_pod_v<ElementType>)
                    std::memcpy(this->m_ptr, right_hand_side.m_ptr, m_size * sizeof(ElementType));
                else
                {
                    for(size_t i = 0; i < this->m_size; ++i)
                        this->m_ptr[i] = right_hand_side.m_ptr[i];
                }
            }

            ElementType *operator&() { return this->m_ptr; }

            ElementType &operator[](size_t index) { return this->m_ptr[index]; }

            const ElementType &operator[](size_t index) const { return this->m_ptr[index]; }

            ElementType &at(size_t index)
            {
                if (index < this->m_size && this->m_ptr)
                    return this->m_ptr[index];
                else
                    Tpf_ThrowDebugException("Index Out of Range");
            }

            const ElementType &at(size_t index) const
            {
                if (index < this->m_size && this->m_ptr)
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

                        if constexpr(std::is_pod_v<ElementType>)
                            std::memcpy(this->m_ptr, right_hand_side.m_ptr, m_size * sizeof(ElementType));
                        else
                        {
                            for(size_t i = 0; i < this->m_size; ++i)
                                this->m_ptr[i] = right_hand_side.m_ptr[i];
                        }
                    }
                    else
                    {
                        // we have free existing memory
                        this->free_memory();
                        this->m_size = right_hand_side.m_size;

                        this->m_ptr = new_alloc();
                        
                        if(!this->m_ptr)
                            Tpf_ThrowDebugException("Dynamic Allocation Failed");

                        if constexpr(std::is_pod_v<ElementType>)
                            std::memcpy(this->m_ptr, right_hand_side.m_ptr, m_size * sizeof(ElementType));
                        else
                        {
                            for(size_t i = 0; i < this->m_size; ++i)
                                this->m_ptr[i] = right_hand_side.m_ptr[i];
                        }    
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

            virtual ~dynamic_array()
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

        }; // end of class dynamic_array

    } // end of namespace matrix

} // end of namespace tpf

#ifdef _TPF_MATRIX_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_MATRIX_MIN_DEFINED
#endif 

#ifdef _TPF_MATRIX_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_MATRIX_MAX_DEFINED
#endif 

#endif // end of file _TPF_MATRIX_HPP
