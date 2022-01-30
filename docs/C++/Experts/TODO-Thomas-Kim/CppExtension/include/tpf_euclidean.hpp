/**
 * @file tpf_euclidean.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef _TPF_EUCLIDEAN_HPP
#define _TPF_EUCLIDEAN_HPP

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <tpf_types.hpp>
#include <utility>

#ifdef max
#define _TPF_EUCLIDEAN_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_EUCLIDEAN_MIN_DEFINED
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
     * @brief Implements Euclidean Algorithm for GCD, LCM
     * 
     */
    namespace euclidean
    { 
        using namespace types;

        // greatest common divisor
        template<typename Type>
        enable_if_in_list_t<Type, integral_list_t>
        gcd(Type a, Type b)
        {
            Type r; // remainder
            while(r = a % b)
            {
                a = b; b = r;
            }

            return b;
        }

        template<template<typename, typename...> class ContainerType, typename EleType>
        enable_if_in_list_t<EleType, integral_list_t>
        gcd(const ContainerType<EleType>& container)
        {
            if(container.empty())
                return 1;
            else
            {
                size_t size = container.size();
                
                if(size==1) 
                    return container.front();
                else if(size==2)
                    return gcd(container[0], container[1]);
                else
                {
                    auto g = gcd(container[0], container[1]);
                    for(size_t i=2; i < size; ++i)
                        g = gcd(g, container[i]);
                        
                    return g;
                }
            }
        }

        template<typename Type, typename... Types>
        auto gcd(Type arg, Types... args)
        {
            return gcd(arg, gcd(args...));
        }

        // least common multiple
        template<typename Type>
        enable_if_in_list_t<Type, integral_list_t>
        lcm(Type a, Type b)
        {
            return (a / gcd(a, b)) * b;
        }

        template<typename Type, typename... Types>
        auto lcm(Type arg, Types... args)
        {
            return lcm(arg, lcm(args...));
        }

        template<template<typename, typename...> class ContainerType, typename EleType>
        enable_if_in_list_t<EleType, integral_list_t>
        lcm(const ContainerType<EleType>& container)
        {
            if(container.empty())
                return 1;
            else
            {
                size_t size = container.size();
                
                if(size==1) 
                    return container.front();
                else if(size==2)
                    return lcm(container[0], container[1]);
                else
                {
                    auto l = lcm(container[0], container[1]);

                    for(size_t i=2; i < size; ++i)
                        l = lcm(l, container[i]);
                        
                    return l;
                }
            }
        }

        template<typename Type>
        enable_if_in_list_t<Type, integral_list_t, void>
        reduce(Type& a, Type& b)
        {
            auto g = gcd(a, b);
            a /= g; b /= g;
        }

        namespace hidden
        {
           	// https://goo.gl/o1mKYM
            // https://goo.gl/NTdu2w
            // a * x + b * y = gcd(a , b)
            // eea for Extended Euclidean Algorithm
            // do not call it directly,
            // it is helper function
            // 
            // precondition: a > b, otherwise 
            // if (b > a) std::swap(a, b);
            // if fails, return 0;
            template<typename Type>
            Type extended_euclidean_algorithm(Type a, Type b, Type& x, Type& y)
            {
                std::vector<Type> q; q.reserve(100);

                Type r; // remainder

                while (r = a % b)
                {
                    q.emplace_back(-(a / b));
                    a = b; b = r;
                }

                // 10 x + 5 y = gcd(10, 5) = 5
                // if gcd != 1, then we cannot solve
                // 10 x = 1 (mod 5) or 5 x = 1 (mod 10)
                // 10 x + 5 y = 5
                // 10 0 + 5 1 = 5
                // x = 0, y = 1
                if (q.empty())
                {
                    x = 0; y = 1; return b;
                }

                // initial condition
                x = 1; y = q.back();
                size_t cache_count = q.size();

                for (size_t i = cache_count - 2; i != 0; --i)
                {
                    Type tmp = x; x = y; y = tmp + y*q[i];
                }

                if(cache_count>1)
                {
                    Type tmp = x; x = y; y = tmp + y*q[0];
                }
                
                return b;
            }
        } // end of namespace hidden

        // https://goo.gl/o1mKYM
        // https://goo.gl/NTdu2w
        // a x + b y = gcd(a, b)
        // return gcd(a, b), x and y
        template<typename Type>
        Type extended_euclidean_algorithm(Type a, Type b, Type& x, Type& y)
        {
            Type a1 = a, b1 = b;

            // remember we have swapped a and b
            if (b1 > a1) std::swap(a1, b1);

            Type g = hidden::extended_euclidean_algorithm(a1, b1, x, y);
            
            // if x and y does not satisfy
            // a * x + b * y = gcd(a, b), i.e.,
            // a * x + b * y != gcd(a, b)
            // then we have to swap x and y
            if (a * x + b * y != g)
                std::swap(x, y);

            return g;
        }

    } // end of namespace set

} // end of namespace euclidean

#ifdef _TPF_EUCLIDEAN_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_EUCLIDEAN_MIN_DEFINED
#endif 

#ifdef _TPF_EUCLIDEAN_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_EUCLIDEAN_MAX_DEFINED
#endif 

#endif // end of file _TPF_EUCLIDEAN_HPP
