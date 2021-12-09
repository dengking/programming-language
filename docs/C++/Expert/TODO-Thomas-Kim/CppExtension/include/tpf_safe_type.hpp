/**
 * @file tpf_safe_type.hpp
 * @author your name (you@domain.com)
 * @brief This file implements safe arithmetic
 * @version 0.1
 * @date 2019-05-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef _TPF_SAFE_TYPE_HPP
#define _TPF_SAFE_TYPE_HPP

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <iostream>
#include <type_traits>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>  
#include <cstdlib>

#include "tpf_types.hpp"

#ifdef max
#define _TPF_SAFE_TYPE_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_SAFE_TYPE_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

namespace tpf
{
	/**
	 * @brief Defines safe type operation
	 * 
	 */
	namespace safe_type
	{
		/**
		 * @brief Upperbound number the Type can represent
		 * 
		 * @tparam Type 
		 */
		template<typename Type> constexpr Type limits_max = 
			std::numeric_limits<Type>::max();
		
		/**
		 * @brief Lowerbound number the Type can represent
		 * 
		 * @tparam Type 
		 */
		template<typename Type> constexpr Type limits_min = 
			std::numeric_limits<Type>::min();
		
		// count bits in the mask
		// mask ( 7)      : 0000 0111,  should return 3
		// mask (-1)      : 1111 1111,  should return 8
		// mask (0x80)    : 1000 0000,  should return 1
		// mask (0)       : 0000 0000,  should return 0
		// mask (1)       : 0000 0001,  should return 1

		namespace hidden
		{
			template<typename T, typename = types::enable_if_integral_t<T>>
			using unsigned_integral_t = types::make_unsigned_integral_t<T>;

			template<typename T, typename = types::enable_if_integral_t<T>>
			using signed_integral_t = types::make_signed_integral_t<T>;

			template<typename S, typename T>
			using common_t = types::common_type_t<S, T>;

			template<typename S, typename T,
				typename = types::enable_if_integral_t<S>,
				typename = types::enable_if_integral_t<T>>
				using signed_common_t = signed_integral_t<common_t<S, T>>;

			template<typename S, typename T,
				typename = types::enable_if_integral_t<S>,
				typename = types::enable_if_integral_t<T>>
				using unsigned_common_t = unsigned_integral_t<common_t<S, T>>;

		} // end of namespace hidden
		
		/**
		 * @brief Returns bit count of type T
		 * 
		 * @tparam T for querying bit count
		 */
		template<typename T>
		constexpr int sizeof_bits = sizeof(T) * 8;

		/**
		 * @brief Return's bit mask for the highest bit
		 * 
		 * @tparam T 
		 */
		template<typename T>
		constexpr auto high_bit_mask = 
			hidden::unsigned_integral_t<T>(1) << (sizeof_bits<T>-1);

		/**
		 * @brief Counts set bits.
		 * 
		 * @tparam Type 
		 * @param bits 
		 * @return types::enable_if_integral_t<Type, int> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, int> 
		count_set_bits(Type bits)
		{
			hidden::unsigned_integral_t<Type> mask = bits;

			int count = 0;

			for (; mask; mask >>= 1)
				count += (int)(mask & 1);

			return count;
		}

		/**
		 * @brief Returns binary bit pattern
		 * 
		 * @tparam Type 
		 * @param bits 
		 * @return types::enable_if_integral_t<Type, std::string> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, std::string> 
		to_bits(Type bits)
		{
			hidden::unsigned_integral_t<Type> mask = bits;

			int count = 0; std::string str;

			for (int pos = sizeof_bits<Type>; pos; mask <<= 1, --pos)
			{
				if (!str.empty() && (pos % 4 == 0))
					str.push_back(' ');

				str.push_back((mask & high_bit_mask<Type>) ? '1' : '0');
			}

			return str;
		}

		/**
		 * @brief Returns hexidecimal representation
		 * 
		 * @tparam Type 
		 * @param v 
		 * @return types::enable_if_integral_t<Type, std::string> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, std::string> 
		to_hex(Type v)
		{
			std::string str;
						
			if constexpr (types::is_char_v<Type>)
			{
				std::ostringstream os;

				os << std::setfill('0') << std::setw(sizeof(Type) * 2)
					<< std::uppercase << std::hex << (short)v;

				str = os.str();

				if (str.size()==4)
				{
					if (v > 0)
					{
						str.pop_back();
						str.pop_back();
					}
					else
					{
						std::string ss;

						ss.push_back(str[2]);
						ss.push_back(str[3]);
						
						str = ss;
					}
				}
			}
			else
			{
				std::ostringstream os;

				os << std::setfill('0') << std::setw(sizeof(Type) * 2)
					<< std::uppercase << std::hex <<v;
				
				str = os.str();
			}

			return str;
		}

		/**
		 * @brief Returns decimal representation
		 * 
		 * @tparam Type 
		 * @param v 
		 * @return types::enable_if_integral_t<Type, std::string> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, std::string> 
		to_dec(Type v)
		{
			std::ostringstream os;
			
			if constexpr (types::is_char_v<Type>)
				os << (short)v;
			else
				os << v;

			return os.str();
		}

		/**
		 * @brief Returns' field width
		 * 
		 * @tparam Type 
		 * @param v 
		 * @return types::enable_if_integral_t<Type, int> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, int> 
		field_width(Type v)
		{
			std::ostringstream os;

			if constexpr (types::is_char_v<Type>)
				os << (short)v;
			else
				os << v;

			return (int) os.str().size();
		}
		
		/**
		 * @brief Return's maximum numeric field width
		 * 
		 * @tparam Type 
		 * @return types::enable_if_integral_t<Type, int> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, int> 
		numeric_width()
		{
			int a = field_width(std::numeric_limits<Type>::max());
			int b = field_width(std::numeric_limits<Type>::min());

			return a > b ? a : b;
		}

		/**
		 * @brief Returns decimal field width
		 * 
		 * @tparam Type 
		 * @param v 
		 * @return types::enable_if_integral_t<Type, std::string> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, std::string> 
		to_dec_width(Type v)
		{
			std::ostringstream os;
			
			int max_field = numeric_width<Type>();

			if constexpr (types::is_char_v<Type>)
				os << std::setw(max_field)<<(short)v;
			else
				os << std::setw(max_field)<<v;

			return os.str();
		}

		/**
		 * @brief Displays how integral types are represented
		 * 
		 * @tparam Type 
		 * @param v 
		 * @return types::enable_if_integral_t<Type, std::string> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, std::string> 
		numeric_base(Type v)
		{
			std::ostringstream os;

			os << to_dec_width(v) << " (" << to_hex<Type>(v) << "): " << to_bits<Type>(v);

			return os.str();
		}

		/**
		 * @brief Displays integral type's numerical information
		 * 
		 * @tparam Type 
		 * @return types::enable_if_integral_t<Type, std::string> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, std::string>
		numeric_type_info(Type)
		{
			std::ostringstream os;

			constexpr auto minimum = std::numeric_limits<Type>::min();
			constexpr auto maximum = std::numeric_limits<Type>::max();

			os << "Type name: " << Tpf_GetTypeName(Type)
				<< ",\tByte size: " << sizeof(Type) << ",\tBit count: " << sizeof_bits<Type>
				<< "\nMinimum: "<< numeric_base(minimum)
				<< "\nMaximum: "<< numeric_base(maximum);

			return os.str();
		}

		/**
		 * @brief Displays all integral types' representation
		 * 
		 */
		template<typename = void>
		void integral_type_info()
		{
			auto nl = "\n\n";

			std::cout << numeric_type_info((char)0) << nl;
			std::cout << numeric_type_info((unsigned char)0) << nl;
			std::cout << numeric_type_info((short)0) << nl;
			std::cout << numeric_type_info((unsigned short)0) << nl;
			std::cout << numeric_type_info((int)0) << nl;
			std::cout << numeric_type_info((unsigned)0) << nl;
			std::cout << numeric_type_info((long)0) << nl;
			std::cout << numeric_type_info((unsigned long)0) << nl;
			std::cout << numeric_type_info((long long)0) << nl;
			std::cout << numeric_type_info((unsigned long long)0) << std::endl;
		}

		/**
		 * @brief Displays Twos complement 
		 * 
		 * @tparam Type 
		 * @param c 
		 * @return types::enable_if_integral_t<Type, void> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type, void> 
		twos_complement(Type c)
		{
			std::cout << numeric_type_info<Type>((Type)0) << "\n";

			Type c1 = ~c;   // 1's complement of s1
			Type c2 = ~c + 1; // 2's complement of s1
			Type c3 = c + c2; // c + (~c+1)
			std::cout << "original value  :      c     = " << numeric_base(c)  << "\n";
			std::cout << "1's complement  :     ~c     = " << numeric_base(c1) << "\n";
			std::cout << "2's complement  :   ~c + 1   = " << numeric_base(c2) << "\n";
			std::cout << "complement check: c + (~c+1) = " << numeric_base(c3) << std::endl;
		}

		// signed: a + b
		/**
		 * @brief Safe addition of the signed integral types
		 * 
		 * @tparam Type 
		 * @param a 
		 * @param b 
		 * @return types::enable_if_signed_integral_t<Type> 
		 */
		template<typename Type>
		types::enable_if_signed_integral_t<Type>
		safe_add(Type a, Type b)
		{
			if(a < 0 && b < 0)
			{
				// when both a and b are negative
				// limits_min<Type> <= a + b ... (1a)
				// limits_min<Type> - b <= a ... (2a)

				if(limits_min<Type> - b <= a)
					return a + b;
				else
				{
					std::ostringstream os;
					os << "overflow: a = " << a 
						<< ", b = " << b;

					Tpf_ThrowDebugException(os.str());	
				}
			}
			else if(a > 0 && b > 0)
			{
				/*
					when both a and b are positive
					limits_max<Type> >= a + b ... (1b)
					limits_max<Type> - b >= a ... (2b)
				*/
				if( limits_max<Type> - b >= a)
					return a + b;
				else
				{
					std::ostringstream os;
					os << "overflow: a = " << a 
						<< ", b = " << b;

					Tpf_ThrowDebugException(os.str());	
				}
			}
			else
				return a + b;
		}

		// signed: a - b
		/**
		 * @brief Safe subtraction of signed integral types
		 * 
		 * @tparam Type 
		 * @param a 
		 * @param b 
		 * @return types::enable_if_signed_integral_t<Type> 
		 */
		template<typename Type>
		types::enable_if_signed_integral_t<Type>
		safe_sub(Type a, Type b)
		{
			return safe_add(a, -b);
		}

		// unsigned: a + b
		/**
		 * @brief Safe addition of the unsigned integral types
		 * 
		 * @tparam Type 
		 * @param a 
		 * @param b 
		 * @return types::enable_if_unsigned_integral_t<Type> 
		 */
		template<typename Type>
		types::enable_if_unsigned_integral_t<Type>
		safe_add(Type a, Type b)
		{
			/*
				0<= a + b <= limits_max<Type>
				a <= limits_max<Type> - b 
			*/
			if(a <= limits_max<Type> - b)
				return a + b;
			else
			{
				std::ostringstream os;
				os << "overflow: a = " << a 
					<< ", b = " << b;

				Tpf_ThrowDebugException(os.str());	
			}
		}

		// unsigned: a - b
		/**
		 * @brief Safe subtraction of unsigned integral types
		 * 
		 * @tparam Type 
		 * @param a 
		 * @param b 
		 * @return types::enable_if_unsigned_integral_t<Type> 
		 */
		template<typename Type>
		types::enable_if_unsigned_integral_t<Type>
		safe_sub(Type a, Type b)
		{
			/*
				0 <= a - b
				b <= a ... 
			*/
			if(b <= a)
				return a - b;
			else
			{
				std::ostringstream os;
				os << "overflow: a = " << a 
					<< ", b = " << b;

				Tpf_ThrowDebugException(os.str());	
			}
		}

		/**
		 * @brief Safe multiplication of integral types
		 * 
		 * @tparam Type 
		 * @param a 
		 * @param b 
		 * @return types::enable_if_integral_t<Type> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type>
		safe_mul(Type a, Type b)
		{
			auto rlt = a * b;

			if( (b == 0) || (rlt / b == a))
				return rlt;
			else
			{
				std::ostringstream os;
				os << "overflow: a = " << a 
					<< ", b = " << b;

				Tpf_ThrowDebugException(os.str());	
			}
		}

		/**
		 * @brief Safe division of integral types
		 * 
		 * @tparam Type 
		 * @param a 
		 * @param b 
		 * @return types::enable_if_integral_t<Type> 
		 */
		template<typename Type>
		types::enable_if_integral_t<Type>
		safe_div(Type a, Type b)
		{
			if( b != 0)
				return a / b;
			else
			{
				std::ostringstream os;
				os << "division by zero - a = " << a 
					<< ", b = " << b;

				Tpf_ThrowDebugException(os.str());	
			}
		}

		template<typename Type>
		types::enable_if_real_number_t<Type>
		safe_add(Type a, Type b)
		{
			auto rlt = a + b;
			if(rlt != rlt)
			{
				std::ostringstream os;
				os << "floating-point operation failed - a = "
					<< a <<", b = " << b;

				Tpf_ThrowDebugException(os.str());
			}

			return rlt;
		}

		template<typename Type>
		types::enable_if_real_number_t<Type>
		safe_sub(Type a, Type b)
		{
			auto rlt = a - b;
			if(rlt != rlt)
			{
				std::ostringstream os;
				os << "floating-point operation failed - a = "
					<< a <<", b = " << b;

				Tpf_ThrowDebugException(os.str());
			}

			return rlt;
		}

		template<typename Type>
		types::enable_if_real_number_t<Type>
		safe_mul(Type a, Type b)
		{
			auto rlt = a * b;
			if(rlt != rlt)
			{
				std::ostringstream os;
				os << "floating-point operation failed - a = "
					<< a <<", b = " << b;

				Tpf_ThrowDebugException(os.str());
			}

			return rlt;
		}

		template<typename Type>
		types::enable_if_real_number_t<Type>
		safe_div(Type a, Type b)
		{
			auto rlt = a / b;
			if(rlt != rlt)
			{
				std::ostringstream os;

				os << "floating-point operation failed - a = "
					<< a <<", b = " << b;

				Tpf_ThrowDebugException(os.str());
			}

			return rlt;
		}

	} // end of namespace bitwise

} // end of namespace tpf

#ifdef _TPF_SAFE_TYPE_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_SAFE_TYPE_MIN_DEFINED
#endif 

#ifdef _TPF_SAFE_TYPE_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_SAFE_TYPE_MAX_DEFINED
#endif 

#endif // end of file
