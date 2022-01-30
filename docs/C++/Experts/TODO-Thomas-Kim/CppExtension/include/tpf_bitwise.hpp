#ifndef _TPF_BITWISE_HPP
#define _TPF_BITWISE_HPP

#include <iostream>
#include <type_traits>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>  
#include <cstddef> // for std::byte
#include <bitset>
#include <cmath>

#ifdef max
#define _TPF_BITWISE_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_BITWISE_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

namespace tpf::bitwise
{
	namespace hidden
	{
		template<typename T> struct st_is_character
		{
			static const bool value = false;
		};

		template<> struct st_is_character<char>
		{
			static const bool value = true;
		};

		template<> struct st_is_character<unsigned char>
		{
			static const bool value = true;
		};

		template<> struct st_is_character<wchar_t>
		{
			static const bool value = true;
		};
	}

	template<typename T>
	constexpr bool is_char_v = hidden::st_is_character<T>::value;

	template<typename T> constexpr T limits_max = std::numeric_limits<T>::max();
	template<typename T> constexpr T limits_min = std::numeric_limits<T>::min();
	
	// count bits in the mask
	// mask ( 7)      : 0000 0111,  should return 3
	// mask (-1)      : 1111 1111,  should return 8
	// mask (0x80)    : 1000 0000,  should return 1
	// mask (0)       : 0000 0000,  should return 0
	// mask (1)       : 0000 0001,  should return 1

	template<typename T>
	using enable_if_integral = std::enable_if_t<std::is_integral<T>::value>;

	template<typename T, typename = enable_if_integral<T>>
	using unsigned_t = std::make_unsigned_t<T>;

	template<typename T, typename = enable_if_integral<T>>
	using signed_t = std::make_signed_t<T>;

	template<typename S, typename T>
	using common_t = std::common_type_t<S, T>;

	template<typename S, typename T,
		typename = enable_if_integral<S>,
		typename = enable_if_integral<T>>
		using signed_common_t = signed_t<common_t<S, T>>;

	template<typename S, typename T,
		typename = enable_if_integral<S>,
		typename = enable_if_integral<T>>
		using unsigned_common_t = unsigned_t<common_t<S, T>>;

	template<typename T>
	constexpr int sizeof_bits = sizeof(T) * 8;

	template<typename T>
	constexpr unsigned_t<T> high_bit_mask
		= unsigned_t<T>(1) << (sizeof_bits<T>-1);

	template<typename T, typename = enable_if_integral<T>>
	int count_set_bits(T bits)
	{
		unsigned_t<T> mask = bits;

		int count = 0;

		for (; mask; mask >>= 1)
			count += (int)(mask & 1);

		return count;
	}
			
	template<typename T, typename = enable_if_integral<T>>
	std::string to_bits(T bits)
	{
		unsigned_t<T> mask = bits;

		int count = 0; std::string str;

		for (int pos = sizeof_bits<T>; pos; mask <<= 1, --pos)
		{
			if (!str.empty() && (pos % 4 == 0))
				str.push_back('\'');

			str.push_back((mask & high_bit_mask<T>) ? '1' : '0');
		}

		return str;
	}

	template<typename T, typename = enable_if_integral<T>>
	std::string to_hex(T v)
	{
		std::string str;
					
		if (is_char_v<T>)
		{
			std::ostringstream os;

			os << std::setfill('0') << std::setw(sizeof(T) * 2)
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

			os << std::setfill('0') << std::setw(sizeof(T) * 2)
				<< std::uppercase << std::hex <<v;
			
			str = os.str();
		}

		return str;
	}

	template<typename T, typename = enable_if_integral<T>>
	std::string to_dec(T v)
	{
		std::ostringstream os;
		
		if (is_char_v<T>)
			os << (short)v;
		else
			os << v;

		return os.str();
	}

	template<typename T, typename = enable_if_integral<T>>
	int field_with(T v)
	{
		std::ostringstream os;

		if (is_char_v<T>)
			os << (short)v;
		else
			os << v;

		return (int) os.str().size();
	}
	
	template<typename T, typename = enable_if_integral<T>>
	int numeric_width()
	{
		int a = field_with(std::numeric_limits<T>::max());
		int b = field_with(std::numeric_limits<T>::min());

		return a > b ? a : b;
	}

	template<typename T, typename = enable_if_integral<T>>
	std::string to_dec_width(T v)
	{
		std::ostringstream os;
		
		int max_field = numeric_width<T>();

		if (is_char_v<T>)
			os << std::setw(max_field)<<(short)v;
		else
			os << std::setw(max_field)<<v;

		return os.str();
	}

	template<typename T, typename = enable_if_integral<T>>
	std::string numeric_base(T v)
	{
		std::ostringstream os;

		os << to_dec_width(v) << " (" << to_hex<T>(v) << "): " << to_bits<T>(v);

		return os.str();
	}

	template<typename T, typename = enable_if_integral<T>>
	std::string numeric_type_info()
	{
		std::ostringstream os;

		auto minimum = std::numeric_limits<T>::min();
		auto maximum = std::numeric_limits<T>::max();

		os << "Type name: " << Tpf_GetTypeName(T)
			<< ",\tByte size: " << sizeof(T) << ",\tBit count: " << sizeof_bits<T>
			<< "\nMinimum: "<< numeric_base(minimum)
		    << "\nMaximum: "<< numeric_base(maximum);

		return os.str();
	}

	template<typename = void>
	std::string integral_type_info()
	{
		std::ostringstream os;

		os << numeric_type_info<char>() << "\n\n";
		os << numeric_type_info<unsigned char>() << "\n\n";
		os << numeric_type_info<short>() << "\n\n";
		os << numeric_type_info<unsigned short>() << "\n\n";
		os << numeric_type_info<int>() << "\n\n";
		os << numeric_type_info<unsigned int>() << "\n\n";
		os << numeric_type_info<long>() << "\n\n";
		os << numeric_type_info<unsigned long>() << "\n\n";
		os << numeric_type_info<long long>() << "\n\n";
		os << numeric_type_info<unsigned long long>() << "\n";

		return os.str();
	}

	template<typename T, typename = enable_if_integral<T>>
	std::string twos_complement(T c)
	{
		std::ostringstream os;
		os << numeric_type_info<T>() << "\n";

		T c1 = ~c;   // 1's complement of s1
		T c2 = ~c + 1; // 2's complement of s1
		T c3 = c + c2; // c + (~c+1)
		os << "original value  :      c     = " << numeric_base(c) <<  "\n";
		os << "1's complement  :     ~c     = " << numeric_base(c1) << "\n";
		os << "2's complement  :   ~c + 1   = " << numeric_base(c2) << "\n";
		os << "complement check: c + (~c+1) = " << numeric_base(c3) << "\n";

		return os.str();
	}

	template<typename T, 
    typename = tpf::types::enable_if_unsigned_integral_t<T>>
	struct bit_pattern
	{
		constexpr static size_t byte_size = sizeof(T);
		constexpr static T min_limit = std::numeric_limits<T>::min();
		constexpr static T max_limit = std::numeric_limits<T>::max();

		// on some machine, bit count of a byte is not 8 bits, but 16 bits
		// but I will ignore it, and assume 1 byte represents 8 bits
		constexpr static size_t bit_size = sizeof(T) * 8;

		union
		{
			// n for number or numerical interpretation
			T n{};

			// b for binary digits - b is array of type std::byte
			std::byte b[byte_size];
		};

		// most significant byte
		const std::byte& msb() const noexcept
		{
			return b[byte_size - 1];
		}
		
		// most significant byte
		std::byte& msb() noexcept
		{
			return b[byte_size - 1];
		}

		// least significant byte
		const std::byte& lsb() const noexcept
		{
			return b[0];
		}
		
		// least significant byte
		std::byte& lsb() noexcept
		{
			return b[0];
		}

		template<typename S, typename = tpf::types::enable_if_numerical_number_t<S>>
		bit_pattern& operator=(S s) noexcept
		{
			if constexpr(tpf::types::is_real_number_v<S>)
			{
				// s is a floating point type
				this->n = (T) std::round(s); // 1. round(), then 2. typecast S to T
			}
			else
			{
				// s is an integral type
				this->n = (T)s; // typecast S to T
			}
			
			return *this;
		}

		// we allow both integral and real numbers to be assigned by this class
		template<typename S, typename = tpf::types::enable_if_numerical_number_t<S>>
		operator S() const noexcept
		{
			return (S)this->n;
		}
		
	}; // end of class bit_pattern
}

#define Tpf_SignedCommonType(a, b) tpf::bitwise::signed_common_t<decltype(a), decltype(b)>
#define Tpf_UnignedCommonType(a, b) tpf::bitwise::unsigned_common_t<decltype(a), decltype(b)>

#ifdef _TPF_BITWISE_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_BITWISE_MIN_DEFINED
#endif 

#ifdef _TPF_BITWISE_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_BITWISE_MAX_DEFINED
#endif 

#endif // end of file _TPF_BITWISE_HPP
