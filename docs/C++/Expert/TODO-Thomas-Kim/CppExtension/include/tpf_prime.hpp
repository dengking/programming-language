/**
 * @file tpf_prime.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef _TPF_PRIME_HPP
#define _TPF_PRIME_HPP

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

#ifdef max
#define _TPF_PRIME_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_PRIME_MIN_DEFINED
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
    namespace prime
    {
        

    } // end of namespace set

} // end of namespace tpf

#ifdef _TPF_PRIME_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_PRIME_MIN_DEFINED
#endif 

#ifdef _TPF_PRIME_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_PRIME_MAX_DEFINED
#endif 

#endif // end of file _TPF_PRIME_HPP
