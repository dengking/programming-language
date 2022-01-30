/**
 * @file tpf_chrono_random.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TPF_CHRONO_RANDOM_HPP
#define _TPF_CHRONO_RANDOM_HPP

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <chrono>
#include <ratio>
#include <random>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <execution>

#include "tpf_types.hpp"

#ifdef max
#define _TPF_CHRONO_RANDOM_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_CHRONO_RANDOM_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

namespace tpf
{
    /**
     * @brief Implements random number generator and stop watch
     * 
     */
    namespace chrono_random
    {
        using nano_t = std::nano;
        using micro_t = std::micro;
        using milli_t = std::milli;
        using second_t = std::ratio<1>;
        using minute_t = std::ratio<60>;
        using hour_t = std::ratio<3600>;

        template<typename TimeUnit>
        using duration_t = std::chrono::duration<double, TimeUnit>;
        
        using nanoseconds_t = duration_t<nano_t>;
        using microseconds_t = duration_t<micro_t>;
        using milliseconds_t = duration_t<milli_t>;
        using seconds_t = duration_t<second_t>;
        using minutes_t = duration_t<minute_t>;
        using hours_t = duration_t<hour_t>;
       
        using high_resolution_clock_t = std::chrono::high_resolution_clock;
        using time_point_t = std::chrono::time_point<high_resolution_clock_t>;
        auto& now = high_resolution_clock_t::now;

        inline unsigned int seed() 
            { return high_resolution_clock_t::now().time_since_epoch().count(); }

        // used to initialize random engine
        unsigned int get_current_tick()
        {
            using std::chrono::duration;
            using std::chrono::duration_cast;

            auto current = 
                duration_cast<std::chrono::milliseconds>(now().time_since_epoch());
            
            return current.count();
        }

        // to convert time (in TimeUnit) to type double
        template<typename TimeUnit>
        double time_difference_in_unit(
            const time_point_t& start_time,
            const time_point_t& end_time)
        {
            using std::chrono::duration;
            using std::chrono::duration_cast;

            if constexpr(std::is_same_v<TimeUnit, second_t>)
            {
                auto period_of_time =
                    duration_cast<std::chrono::duration<double, second_t>>(end_time - start_time);
                return period_of_time.count();
            }
            else
            {
                auto period_of_time =
                    duration_cast<std::chrono::duration<double, TimeUnit>>(end_time - start_time);
                
                return period_of_time.count();
            }            
        }
        
        using random_engine_t = std::default_random_engine;

        template<typename IntegralType, 
            typename Type = std::enable_if_t<types::is_integral_v<IntegralType>>>
        using random_uniform_integral_distribution =
            std::uniform_int_distribution<IntegralType>;

        template<typename RealType,
            typename Type = std::enable_if_t<types::is_real_number_v<RealType>>>
        using random_uniform_real_distribution = 
            std::uniform_real_distribution<RealType>;

        class stop_watch
        {
            private:
                mutable high_resolution_clock_t m_clock;
                mutable time_point_t m_start_time;
            
            public:
                stop_watch(): m_start_time{now()}
                { }

                void reset() const { this->m_start_time = now(); }

                template<typename TimeUnit = milli_t>
                auto elapsed(bool bReset = true) const
                {
                    auto rlt = time_difference_in_unit<TimeUnit>(this->m_start_time, now());
                    if(bReset) reset(); 
                    return rlt;
                }

                template<typename TimeUnit = milli_t>
                std::string elapsed_time(bool bReset = true, TimeUnit dummy_time = TimeUnit{}) const
                {
                    std::ostringstream os;

                    os << time_difference_in_unit<TimeUnit>(this->m_start_time, now());

                    if constexpr(std::is_same_v<TimeUnit, nano_t>)
                        os << " nano-seconds";
                    else if constexpr(std::is_same_v<TimeUnit, micro_t>)
                        os << " micro-seconds";
                    else if constexpr(std::is_same_v<TimeUnit, milli_t>)
                        os << " milli-seconds";
                    else if constexpr(std::is_same_v<TimeUnit, second_t>)
                        os << " seconds";
                    else if constexpr(std::is_same_v<TimeUnit, minute_t>)
                        os << " minutes";
                    else if constexpr(std::is_same_v<TimeUnit, hour_t>)
                        os << " hours";

                    if(bReset) reset(); 
                    return os.str();            
                }        
        };

        template<typename Type,
            typename = std::enable_if_t<tpf::types::is_integral_v<Type>>>
        class fair_die
        {
            private:
            
                std::random_device m_rd;
                std::mt19937 m_randomizer;
                typename std::vector<Type>::iterator m_next;
                std::vector<Type> m_array;

            public:
                // [start, end)
                fair_die(Type start, Type end, Type instance = 1): 
                    m_array((end-start)*instance), m_rd{}, m_randomizer{ m_rd() }
                {
                    auto diff = end - start;

                    for(size_t i = 0; i < m_array.size(); ++i )
                        m_array[i] = (i % diff) + start;
                    
                    shuffle();               
                }

                typename std::vector<Type>::iterator begin() { return this->m_array.begin(); };
                typename std::vector<Type>::iterator end() { return this->m_array.end(); };
                typename std::vector<Type>::iterator next() { return this->m_next; };

                size_t size() { return m_array.size(); }

                void shuffle()
                {
                    std::shuffle(m_array.begin(), m_array.end(), m_randomizer);
                    this->m_next = m_array.begin();
                }

                Type operator()()
                {
                    if(this->m_next == m_array.end())
                        shuffle();
                        
                    Type v = *this->m_next;
                    ++this->m_next;
                    return v;
                }

                const std::vector<Type>& array()
                {
                    return this->m_array;
                }
        };

        template<typename Type,
                typename DistributionType, typename EngineType>
        class random_t
        {
            public:
                using engine_type = EngineType;
                using distribution_type = DistributionType;
            private:
                mutable Type m_range_start;
                mutable Type m_range_end;
                mutable engine_type m_engine;
                mutable distribution_type m_distribution;
              
            public:
                random_t(Type range_start = Type{}, 
                    Type range_end = Type{100}, 
                    unsigned int seed = 0): 
                    m_range_start{range_start},
                    m_range_end{range_end},
                    m_engine{seed == 0 ? get_current_tick(): seed},
                    m_distribution{range_start, range_end} {}

                random_t(const random_t&) = default;
                random_t& operator=(const random_t&) = default;
                
                Type operator()() const
                {
                    return m_distribution(m_engine);
                }

                template<typename SizeType>
                auto clone(SizeType size) const;

                template<typename SizeType>
                auto clone_pair(SizeType size) const;
        };

        template<typename IntegralType,
            typename Type = std::enable_if_t<types::is_integral_v<IntegralType>>>
        using random_uniform_integer_t = random_t<IntegralType, 
                random_uniform_integral_distribution<IntegralType>, random_engine_t>;
                
        template<typename RealType,
            typename Type = std::enable_if_t<types::is_real_number_v<RealType>>>
        using random_uniform_real_t = random_t<RealType, 
                random_uniform_real_distribution<RealType>, random_engine_t>;

        template<typename ValueType, typename RangeType1, typename RangeType2>
            auto random_generator(RangeType1 range_start, RangeType2 range_end)
        {
            if constexpr(tpf::types::is_integral_v<ValueType>)
            {
                random_uniform_integer_t<ValueType> 
                    random_generator{(ValueType)range_start, (ValueType)range_end};
                
                return random_generator;
            }
            else if constexpr(tpf::types::is_real_number_v<ValueType>)
            {
                random_uniform_real_t<ValueType> 
                    random_generator{(ValueType)range_start, (ValueType)range_end};

                return random_generator;
            }
        }

        template<typename Type,
                typename DistributionType, typename EngineType>
        template<typename SizeType>
        auto random_t<Type, DistributionType, EngineType>::clone(SizeType size) const
        {
            using random_type = 
                random_t<Type, DistributionType, EngineType>;
            
            std::vector<random_type> randoms;
            randoms.reserve((size_t)size);

            auto generator = 
                random_generator<unsigned>((unsigned)1, 
                    std::numeric_limits<unsigned>::max());
            
            std::set<unsigned> seeds;
            while(seeds.size() != (size_t)size)
            {
                seeds.insert(generator());
            }

            for(auto seed: seeds)
                randoms.emplace_back(random_type{this->m_range_start,
                    this->m_range_end, seed});

            return randoms;
        } 

        template<typename Type,
                typename DistributionType, typename EngineType>
        template<typename SizeType>
        auto random_t<Type, DistributionType, EngineType>::clone_pair(SizeType size) const
        {
            using random_type = 
                random_t<Type, DistributionType, EngineType>;
            using random_pair = std::pair<size_t, random_type>;

            std::vector<random_pair> randoms;
            randoms.reserve((size_t)size);

            auto generator = 
                random_generator<unsigned>((unsigned)1, 
                    std::numeric_limits<unsigned>::max());
            
            std::set<unsigned> seeds;
            while(seeds.size() != (size_t)size)
            {
                seeds.insert(generator());
            }

            size_t index{};

            for(auto seed: seeds)
                randoms.emplace_back(index++, random_type{this->m_range_start,
                    this->m_range_end, seed});

            return randoms;
        }

        template<template<typename, typename...> class ContainerType,
            typename Type, typename... Types,
            template<typename, typename...> class RandomGeneratorType, typename... RTypes>
        void random_fill(ContainerType<Type, Types...>& container,
            const RandomGeneratorType<Type, RTypes...>& random_generator)
        {
            if(container.empty())
                return;
                
            for(auto& e: container)
                e = random_generator();    
        }

        template<template<typename, typename...> class ContainerType,
            typename Type, typename... Types, typename RandomGeneratorType>
        void random_fill(ContainerType<Type, Types...>& container,
            RandomGeneratorType&& random_generator)
        {
            if(container.empty())
                return;
                
            for(auto& e: container)
                e = random_generator();    
        }
       
        template<template<typename, typename...> class ContainerType,
            typename Type, typename... Types,
            template<typename, typename...> class RandomGeneratorType, typename... RTypes>
        void random_parallel_fill(ContainerType<Type, Types...>& container,
            const RandomGeneratorType<Type, RTypes...>& random_generator)
        {
            if(container.empty()) return;
                
            size_t container_size = container.size();
            size_t generator_size = std::thread::hardware_concurrency();
            size_t span = container_size / generator_size;

            if(span < 100) 
            {
                random_fill(container, random_generator); 
                return;
            }
            
            if(container_size % generator_size ) ++generator_size;

            auto generator_pairs = random_generator.clone_pair(generator_size);          

            std::for_each(std::execution::par, 
                generator_pairs.begin(), generator_pairs.end(),
                [&](auto& pair)
                {
                    auto index_start = pair.first * span;
                    auto index_end = index_start + span;

                    if(index_end > container_size) index_end = container_size;

                    for(size_t i = index_start; i < index_end; ++i)
                        container[i] = pair.second();
                });
        }

        template<template<typename, typename...> class ContainerType,
            typename Type, typename... Types, 
            typename RandomGeneratorType>
        void random_parallel_fill(ContainerType<Type, Types...>& container,
            RandomGeneratorType&& random_generator)
        {
            if(container.empty()) return;
                
            size_t container_size = container.size();
            size_t generator_size = std::thread::hardware_concurrency();
            size_t span = container_size / generator_size;

            if(span < 100) 
            {
                random_fill(container, random_generator); 
                return;
            }
            
            if(container_size % generator_size ) ++generator_size;

            auto generator_pairs = random_generator.clone_pair(generator_size);          

            std::for_each(std::execution::par, 
                generator_pairs.begin(), generator_pairs.end(),
                [&](auto& pair)
                {
                    auto index_start = pair.first * span;
                    auto index_end = index_start + span;

                    if(index_end > container_size) index_end = container_size;

                    for(size_t i = index_start; i < index_end; ++i)
                        container[i] = pair.second();
                });
        }

        /**
         * @brief Fills the /a container with random numbers.
         * If the /a container is /a set or /a unordered_set, the range of
         * the random_generator should be sufficiently big,
         * otherwise it can fall into infinity loop.  
         * 
         * @tparam ContainerType 
         * @tparam Type 
         * @tparam Types 
         * @tparam SizeType 
         * @tparam RandomGeneratorType 
         * @tparam RTypes 
         * @param container 
         * @param random_generator 
         * @param size 
         */
        template<template<typename, typename...> class ContainerType,
            typename Type, typename... Types, typename SizeType,
            template<typename, typename...> class RandomGeneratorType, typename... RTypes>
        void random_fill(ContainerType<Type, Types...>& container, 
            const RandomGeneratorType<Type, RTypes...>& random_generator, SizeType size)
        {
            if constexpr (types::is_resize_available_v<ContainerType<Type, Types...>>)
            {
                container.resize((size_t)size);

                for(auto& e: container)
                    e = random_generator();
            }
            else
            {
                container.clear();
                
                while(container.size() != size)
                {
                   types::push_back(container, random_generator());
                }
            }
        }

        template<typename CharType, size_t Size>
        class random_words
        {
            public:

                const CharType* m_alphabet;
                random_uniform_integer_t<size_t> m_character_selector;
                random_uniform_integer_t<size_t> m_length_selector;
                random_uniform_integer_t<size_t> m_wordcount_selector;
                
            public:
                template<typename Type, size_t ArraySize>
                random_words(size_t minimum, size_t maximum, size_t max_words, 
                    const Type(&alphabet)[ArraySize]): 
                    m_alphabet{alphabet}, 
                    m_character_selector{ random_generator<size_t>(size_t{}, ArraySize-2)},
                    m_length_selector{ random_generator<size_t>(minimum, maximum) },
                    m_wordcount_selector{ random_generator<size_t>(1, max_words) } { }

                std::basic_string<CharType> operator()()
                {
                    std::basic_ostringstream<CharType> stream;

                    auto word_count = m_wordcount_selector();

                    for(size_t j = 0; j < word_count; ++j)
                    {
                        auto length = this->m_length_selector();
                        
                        std::basic_string<CharType> text(length, ' ');

                        for(size_t i=0; i < length; ++i)
                            text[i] = m_alphabet[m_character_selector()];

                        stream << text;

                        if(j + 1 != (size_t)word_count)
                        {
                            if constexpr(std::is_same_v<CharType, char>)
                                stream <<' ';
                            else
                                stream <<L' ';
                        }
                    }

                    return stream.str();
                }
        };

        template<typename Type, size_t ArraySize>
        random_words(int, int, int, const Type(&)[ArraySize]) -> random_words<Type, ArraySize>;


    } // end of namespace chrono_random

} // end of namespace tpf

#ifdef _TPF_CHRONO_RANDOM_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_CHRONO_RANDOM_MIN_DEFINED
#endif 

#ifdef _TPF_CHRONO_RANDOM_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_CHRONO_RANDOM_MAX_DEFINED
#endif 

#endif // end of file _TPF_CHRONO_RANDOM_HPP
