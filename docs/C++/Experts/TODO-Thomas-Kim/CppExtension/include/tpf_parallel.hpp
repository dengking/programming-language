/**
 * @file tpf_parallel.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-10-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TPF_PARALLEL_HPP
#define _TPF_PARALLEL_HPP

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
#endif // end of TBB_SUPPRESS_DEPRECATED_MESSAGES

#if !defined(DEBUGGING_MODE)
    #if defined(_DEBUG)
        #define DEBUGGING_MODE 1
    #elif defined(TBB_USE_DEBUG)
        #if TBB_USE_DEBUG==1
            #define DEBUGGING_MODE 1
        #else
            #define DEBUGGING_MODE 0
        #endif // TBB_USE_DEBUG==1
    #else
        #define DEBUGGING_MODE 0
    #endif // defined(_DEBUG)
#endif // !defined(DEBUGGING_MODE)

#ifndef LIB_INTERNAL_PutDebugMessageAndExit
    #define LIB_INTERNAL_PutDebugMessageAndExit(error_msg) \
    { std::cerr<<"File name ["<<__FILE__<< "]\n" \
    << "Line " << __LINE__ << " : " << error_msg; \
    std::exit(1); }
#endif // end of LIB_INTERNAL_PutDebugMessageAndExit

#ifndef Tpf_PutDebugMessageAndExit
    #define Tpf_PutDebugMessageAndExit(error_msg) LIB_INTERNAL_PutDebugMessageAndExit(error_msg)
#endif // end of Tpf_PutDebugMessageAndExit

#include <tpf_types.hpp>
#include <atomic>
#include <algorithm>
#include <execution>
#include <tbb/tbb.h>

#ifdef max
#define _TPF_PARALLEL_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_PARALLEL_MIN_DEFINED
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
    namespace parallel
    {
         /*
            Feature testing (C++20)
            https://en.cppreference.com/w/cpp/feature_test
        */

        /*
            014 - Cache-Aligned Elements of C++ STL Container with cache_wrapper class
            https://www.youtube.com/watch?v=oNHvpH1eZiM&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=15

            013 - Cache-Aligned STL Container with Intel TBB cache_aligned_allocator, C++20 Feature testing
            https://www.youtube.com/watch?v=__XCCzSes0k&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=14
            
            012 - Cache aligned pointer with operator new( std::align_val_t{64}, std::nothrow)
            https://www.youtube.com/watch?v=hJ3di5bq3xU&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=13
            
            011 - std::jthread The C++20 Thread Launcher, Experiment on alignas
            https://www.youtube.com/watch?v=KUwXn_axEME&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=12

            010 - C++ Threading Books, Parallel Algorithm, atomic, mutex, scoped_lock, ranges, anonymous union
            https://www.youtube.com/watch?v=DLyQ_VEFguI&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=11
            
            009 - How to use 100% of C++ placement new, delete, nothrow new, delete operators!
            https://www.youtube.com/watch?v=EEBuoFG3q4M&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=10
            
            008 - Overload Class-specific operator new / delete, Memory Leak Detection on Command Line
            https://www.youtube.com/watch?v=19Xg55hI6wg&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=9
            
            007 - Experiment on Global operator new, dynamic allocation with Zero Byte
            https://www.youtube.com/watch?v=gfZL5nDthFQ&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=8
            
            006 - Benchmarking std::map vs. std::unordered_map - random string generator: random_words
            https://www.youtube.com/watch?v=eS1hwYS2FQk&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=7
            
            005 - Experiment on Polymorphic Memory Resource (PMR) - C++17 - The Complete Guide
            https://www.youtube.com/watch?v=5gA3YbcLzKI&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=5
        */

        #ifdef __cpp_lib_hardware_interference_size
            inline constexpr auto cache_line_size = std::hardware_constructive_interference_size;
        #else
            inline constexpr auto cache_line_size = 64;
        #endif // end of __cpp_lib_hardware_interference_size

        inline constexpr size_t default_alignment() 
            { return __STDCPP_DEFAULT_NEW_ALIGNMENT__; }
    
        template<typename ElementType, typename Operation>
        inline void atomic_operation(std::atomic<ElementType>& atom, Operation&& opr)
        {
            ElementType old_value = atom;
            ElementType new_value = opr(old_value);

            while(!atom.compare_exchange_strong(old_value, new_value))
            {
                old_value = atom;
                new_value = opr(old_value);
            }
        }

        #ifndef _TPF_ITERATOR
        #define _TPF_ITERATOR

        #ifdef __TBB_iterators_H
            
            template <typename IntType>
            using counting_iterator = tbb::counting_iterator<IntType>;

        #else
            // By courtesy of Intel's Threading Building Blocks team!!
            // With best respect and gratitude for Intel Corporation.
            template <typename IntType>
            class counting_iterator {
                static_assert(std::numeric_limits<IntType>::is_integer, "Cannot instantiate counting_iterator with a non-integer type");
            public:
                typedef typename std::make_signed<IntType>::type difference_type;
                typedef IntType value_type;
                typedef const IntType* pointer;
                typedef const IntType& reference;
                typedef std::random_access_iterator_tag iterator_category;

                counting_iterator() : my_counter() {}
                explicit counting_iterator(IntType init) : my_counter(init) {}

                reference operator*() const { return my_counter; }
                value_type operator[](difference_type i) const { return *(*this + i); }

                difference_type operator-(const counting_iterator& it) const { return my_counter - it.my_counter; }

                counting_iterator& operator+=(difference_type forward) { my_counter += forward; return *this; }
                counting_iterator& operator-=(difference_type backward) { return *this += -backward; }
                counting_iterator& operator++() { return *this += 1; }
                counting_iterator& operator--() { return *this -= 1; }

                counting_iterator operator++(int) {
                    counting_iterator it(*this);
                    ++(*this);
                    return it;
                }
                counting_iterator operator--(int) {
                    counting_iterator it(*this);
                    --(*this);
                    return it;
                }

                counting_iterator operator-(difference_type backward) const { return counting_iterator(my_counter - backward); }
                counting_iterator operator+(difference_type forward) const { return counting_iterator(my_counter + forward); }
                friend counting_iterator operator+(difference_type forward, const counting_iterator it) { return it + forward; }

                bool operator==(const counting_iterator& it) const { return *this - it == 0; }
                bool operator!=(const counting_iterator& it) const { return !(*this == it); }
                bool operator<(const counting_iterator& it) const {return *this - it < 0; }
                bool operator>(const counting_iterator& it) const { return it < *this; }
                bool operator<=(const counting_iterator& it) const { return !(*this > it); }
                bool operator>=(const counting_iterator& it) const { return !(*this < it); }

            private:
                IntType my_counter;

            }; // end of class counting_iterator
        #endif // end of __TBB_iterators_H

        #endif // end of _TPF_ITERATOR

        // returns the least power of 2 greater than or equal to n
        inline constexpr unsigned long long
        least_power_of_2_ge_n(unsigned long long n)
        {
            if(n == 1) return 0;

            size_t power_of_2 = 1;

            while(power_of_2 < n)
            {
                power_of_2 <<= 1; // power_of_2 *= 2
            }
            return power_of_2;
        }

        inline constexpr unsigned long long 
            fast_integer_power(unsigned long long x, unsigned long long n)
        {
            if(n == 0) return 1;

            unsigned long long b = 1; 

            while(n)
            {
                if( n & 1 ) b *= x;

                n >>= 1; x *= x;
            }

            return b;
        }

        template<typename Type, size_t CacheSize = cache_line_size>
        struct alignas(CacheSize) cache_wrapper
        {
            private:
                Type m_value;

            public:
                cache_wrapper(Type v = Type{}): m_value{ std::move(v) } { }

                cache_wrapper(const cache_wrapper&) = default;
                cache_wrapper& operator=(const cache_wrapper&) = default;

                cache_wrapper(cache_wrapper&&) = default;
                cache_wrapper& operator=(cache_wrapper&&) = default;

                const Type& get() const noexcept
                {
                    return this->m_value;
                }

                operator const Type&() const noexcept
                {
                    return this->m_value;
                }
                
                operator Type&() noexcept
                {
                    return this->m_value;
                }

                Type* operator&() noexcept
                {
                    return &this->m_value;
                }

                const Type* operator&() const noexcept
                {
                    return &this->m_value;
                }

                friend bool operator < (const cache_wrapper& left, const cache_wrapper& right)
                {
                    return left.m_value < right.m_value;
                }

                friend bool operator <= (const cache_wrapper& left, const cache_wrapper& right)
                {
                    return left.m_value <= right.m_value;
                }

                friend bool operator > (const cache_wrapper& left, const cache_wrapper& right)
                {
                    return left.m_value > right.m_value;
                }

                friend bool operator >= (const cache_wrapper& left, const cache_wrapper& right)
                {
                    return left.m_value >= right.m_value;
                }

                friend bool operator == (const cache_wrapper& left, const cache_wrapper& right)
                {
                    return left.m_value == right.m_value;
                }

                friend bool operator != (const cache_wrapper& left, const cache_wrapper& right)
                {
                    return left.m_value == right.m_value;
                }
                            
        }; // end of class cache_wrapper

        template<typename T, bool = false>
        struct alignas(cache_line_size) st_cache_wrapper // non-class type
        {
            using type = cache_wrapper<T, cache_line_size>;
        };

        template<typename T>
        struct alignas(cache_line_size) st_cache_wrapper<T, true> // class type
        {
            using type = T;
        };

        template<typename T>
        using cache_wrapper_t = typename st_cache_wrapper<T, std::is_class_v<T>>::type;

        template<typename Type, template<typename, typename...> class Container>
        using cache_aligned_container_t = Container< cache_wrapper_t<Type>,
            tbb::cache_aligned_allocator<cache_wrapper_t<Type>>>;

        template<typename Type>
        using cache_aligned_vector_t = cache_aligned_container_t<Type, std::vector>;

        template<typename Type, size_t Size>
        struct alignas(cache_line_size) st_cache_aligned_array:
            public std::array<cache_wrapper_t<Type>, Size> { };

        template<typename Type, size_t Size>
        using cache_aligned_array_t = st_cache_aligned_array<Type, Size>;

        template<typename Type>
        bool is_cache_aligned(Type* ptr)
        {
            /*
                012 - Cache aligned pointer with operator new( std::align_val_t{64}, std::nothrow)
                https://www.youtube.com/watch?v=hJ3di5bq3xU&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=13
        
                011 - std::jthread The C++20 Thread Launcher, Experiment on alignas
                https://www.youtube.com/watch?v=KUwXn_axEME&list=PL1_C6uWTeBDFiAKI2NWGVV8J2_U9-4GXl&index=12
            */

            return (((std::size_t)(char*)ptr) % cache_line_size) == 0;
        }

        template<typename Type1, typename Type2>
        bool is_cache_aligned(Type1* ptr1, Type2* ptr2)
        {
            return (((std::size_t)((char*)ptr1 - (char*)ptr2))% cache_line_size) == 0;
        }

        template<typename Type1, typename Type2>
        bool is_cache_aligned(Type1& value1, Type2& value2)
        {
            return is_cache_aligned(&value1, &value2);
        }

        template<typename Type>
        bool is_cache_aligned(Type& value)
        {
            return is_cache_aligned(&value);
        }
        // The programmer can modify or adjust this value
        constexpr size_t ConcurrencyLimit = 10;

        // The programmer should not modify this value
        constexpr size_t ForceSequential = ConcurrencyLimit - 1; 
        constexpr size_t ForceParallel = ConcurrencyLimit + 1; 

        inline bool go_parallel(size_t element_count, size_t greater_than)
        {
            return (element_count > std::thread::hardware_concurrency() * greater_than);
        }

        template<typename IteratorType>
        inline bool go_parallel(IteratorType begin, IteratorType end, size_t greater_than)
        {
            auto distance = std::distance(begin, end);
            if(distance < 0) distance = - distance;

            size_t element_count = (size_t)distance;

            // return (element_count > std::thread::hardware_concurrency() * greater_than);
            return go_parallel(element_count, greater_than);
        }

        template<typename ContainerType>
        inline bool go_parallel(ContainerType&& container, size_t greater_than)
        {
            size_t element_count = container.size();
            
            //return (element_count > std::thread::hardware_concurrency()*greater_than);
            return go_parallel(element_count, greater_than);
        }

        template<typename IndexType, typename CallbackType>
        void for_index(IndexType begin_index, IndexType end_index, CallbackType&& callback,
            size_t greater_than = ConcurrencyLimit)
        {
            auto counting_begin = counting_iterator<size_t>{(size_t)begin_index};
            auto counting_end = counting_iterator<size_t>{(size_t)end_index};

            size_t minimum = begin_index < end_index ? begin_index : end_index;
            size_t maximum = begin_index < end_index ? end_index : begin_index;

            if(go_parallel(maximum - minimum, greater_than))
                std::for_each(std::execution::par_unseq,
                counting_begin, counting_end, std::forward<CallbackType>(callback));
            else
                std::for_each(counting_begin, counting_end, std::forward<CallbackType>(callback));          
       }

       template<typename ContainerType, typename CallbackType>
        void for_index(ContainerType&& container, CallbackType&& callback,
            size_t greater_than = ConcurrencyLimit)
        {
            for_index(size_t{}, container.size(),
                std::forward<CallbackType>(callback), greater_than);
        }

       template<typename IndexType, typename InitType,
            typename SumupType, typename HandleParallelType>
        auto reduce_index(IndexType begin_index, IndexType end_index, InitType&& init_value,
            SumupType&& sum_up, HandleParallelType&& handle_parallel,
            size_t greater_than = ConcurrencyLimit)
        {
            auto counting_begin = counting_iterator<size_t>{(size_t)begin_index};
            auto counting_end = counting_iterator<size_t>{(size_t)end_index};

            size_t minimum = begin_index < end_index ? begin_index : end_index;
            size_t maximum = begin_index < end_index ? end_index : begin_index;

            if(go_parallel(maximum - minimum, greater_than))
                return std::transform_reduce(std::execution::par_unseq,
                    counting_begin, counting_end, std::forward<InitType>(init_value),
                    std::forward<SumupType>(sum_up),
                    std::forward<HandleParallelType>(handle_parallel));
            else
                return std::transform_reduce(std::execution::seq,
                    counting_begin, counting_end, std::forward<InitType>(init_value),
                    std::forward<SumupType>(sum_up),
                    std::forward<HandleParallelType>(handle_parallel));          
       }

       template<typename ContainerType, typename InitType,
            typename SumupType, typename HandleParallelType>
        auto reduce_index(ContainerType&& container, InitType&& init_value,
             SumupType&& sum_up, HandleParallelType&& handle_parallel,
            size_t greater_than = ConcurrencyLimit)
        {
            return reduce_index(size_t{}, container.size(), 
                std::forward<InitType>(init_value),
                std::forward<SumupType>(sum_up),
                std::forward<HandleParallelType>(handle_parallel));         
        }

       // We will add more algorithms to augment C++ Standard library
       
    } // end of namespace parallel

} // end of namespace tpf

#ifdef _TPF_PARALLEL_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_PARALLEL_MIN_DEFINED
#endif 

#ifdef _TPF_PARALLEL_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_PARALLEL_MAX_DEFINED
#endif 

#endif // end of file _TPF_PARALLEL_HPP
