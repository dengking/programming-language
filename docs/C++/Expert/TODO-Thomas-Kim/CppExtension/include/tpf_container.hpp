/**
 * @file tpf_container.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TPF_CONTAINER_HPP
#define _TPF_CONTAINER_HPP

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

#ifdef max
#define _TPF_CONTAINER_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_CONTAINER_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

namespace tpf
{
    namespace container
    {
    
    } // end of namespace container
    
} // end of namespace tpf


#ifdef _TPF_CONTAINER_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_CONTAINER_MIN_DEFINED
#endif 

#ifdef _TPF_CONTAINER_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_CONTAINER_MAX_DEFINED
#endif 

#endif // end of file _TPF_CONTAINER_HPP