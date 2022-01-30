/**
 * @file tpf_types.hpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief Type functions are implemented
 * @version 0.1
 * @date 2019-04-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TPF_TYPES_HPP
#define _TPF_TYPES_HPP

#ifndef Tpf_FormatWidth
#define Tpf_FormatWidth 12
#endif // end of Tpf_FormatWidth

#ifndef Tpf_FormatPrecision
#define Tpf_FormatPrecision 6
#endif // end of Tpf_FormatWidth

#ifndef Tpf_ToleranceFloat
    #define Tpf_ToleranceFloat 1.0e-7
#endif

#ifndef Tpf_ToleranceDouble
    #define Tpf_ToleranceDouble 1.0e-12
#endif

#ifndef Tpf_ToleranceLongDouble
    #define Tpf_ToleranceLongDouble 1.0e-12
#endif

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
#include <iomanip>
#include <sstream>
#include <numeric>
#include <type_traits>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <exception>
#include <variant>
#include <tuple>
#include <array>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <list>
#include <any>
#include <functional>
#include <future>
#include <thread>
#include <atomic>
#include <mutex>
#include <memory>
#include <iterator>
#include <execution>
#include <ratio>
#include <optional>
#include <string_view>

#ifdef max
#define _TPF_TYPES_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_TYPES_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

#ifdef _MSC_VER
    #define __FUNCTION_NAME__ __FUNCSIG__
#else
    #define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#endif

/**
 * @brief Root namespace for C++ Extension Library
 * 
 */
namespace tpf
{
    enum class direction_t{ left, right };

     /**
     * @brief Remove const volatile reference from Type.
     * 
     * @tparam Type for remove const volatile and reference.
     */
    template<typename Type>
    using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<Type>>;

    template<typename Type>
    constexpr bool is_const_v = std::is_const_v<std::remove_reference_t<Type>>;

    /**
     * @brief Decay, remove const, volatile, reference
     * 
     * @tparam Type 
     */
    template<typename Type>
    using decay_remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<std::decay_t<Type>>>;

    /**
     * @brief Decay, remove reference
     * 
     * @tparam Type 
     */
    template<typename Type>
    using decay_remove_ref_t = std::remove_reference_t<std::decay_t<Type>>;

    /**
     * @brief Decay, remove const, volatile, reference, then remove pointer and remove const and volatile
     * 
     * @tparam Type 
     */
    template<typename Type>
    using primitive_type_t = remove_cv_ref_t<std::remove_pointer_t<decay_remove_cv_ref_t<Type>>>;

    template<typename Type_1, typename Type_2>
    auto maximum(Type_1 a, Type_2 b)
    {
        return a >= b ? a : b;
    }

    template<typename Type_1, typename Type_2, typename... Types>
    auto maximum(Type_1 a, Type_2 b, Types... args)
    {
        if constexpr(sizeof...(args)==0)
            return maximum(a, b);
        else
            return maximum(maximum(a, b), maximum(b, args...));
    }

    template<typename Type_1, typename Type_2>
    auto minimum(Type_1 a, Type_2 b)
    {
        return b < a ? b : a;
    }

    template<typename Type_1, typename Type_2, typename... Types>
    auto minimum(Type_1 a, Type_2 b, Types... args)
    {
        if constexpr(sizeof...(args)==0)
            return minimum(a, b);
        else
            return minimum(minimum(a, b), minimum(b, args...));
    }

    /**
     * @brief Test if Type is reference to const object
     * 
     * @tparam Type 
     */
    template<typename Type>
    constexpr bool is_const_reference_v =
        std::is_reference_v<Type> && std::is_const_v<std::remove_reference_t<Type>>;
             
   /**
    * @brief This class implements all debugging requirements for C++ Library Extension
    * 
    */
    class debug_exception: public std::exception
    {
        private:
            std::string m_message;
            int m_lineno;
            std::string m_function_name;
            std::string m_file_name;
            std::string m_what_msg;

        public:
            debug_exception(std::string message,
                int lineno, std::string function_name,
                std::string file_name):
                m_lineno(lineno), m_message(message),
                m_function_name(function_name), m_file_name(file_name)
            { 
                std::ostringstream os;

                os << "debug_exception - file [" << this->m_file_name << "]\n";
                os << "thread id [" << std::this_thread::get_id() << "] - ";
                os << "line [" << this->m_lineno<<"] - ";
                os << "function [" << this->m_function_name<<"]\n\n";
                os << "message: " << this->m_message;
                
                this->m_what_msg = os.str();
            }

            virtual const char* what() const noexcept override
            {
                return this->m_what_msg.c_str();
            }

    }; // end of class debug_exception

    template<typename Type>
    constexpr unsigned long long two_power_n(Type n)
    {
        unsigned long long two_power = 1;
        return (two_power << (unsigned long long)n);
    }

     /**
     * @brief Type to string name conversions are defined.
     * 
     */
    namespace types
    {
        template<typename Type>
        using remove_cvref_t = remove_cv_ref_t<Type>;
        
		template<size_t N1, size_t N2, typename T, size_t N,
			typename = std::enable_if_t<N1* N2 == N>>
			inline decltype(auto) cast_array(T(&array)[N])
		{
			using array2_t = T[N1][N2];

			return *reinterpret_cast<array2_t*>(array);
		}

		template<size_t N1, size_t N2, typename T, size_t N = N1 * N2>
		inline decltype(auto) cast_array(T(&array)[N1][N2])
		{
			using array_t = T[N];

			return *reinterpret_cast<array_t*>(array);
		}

		template<size_t N1, size_t N2, size_t N3, typename T, size_t N,
			typename = std::enable_if_t<N1* N2* N3 == N>>
			inline decltype(auto) cast_array(T(&array)[N])
		{
			using array3_t = T[N1][N2][N3];

			return *reinterpret_cast<array3_t*>(array);
		}

		template<size_t N1, size_t N2, size_t N3, typename T, size_t N = N1 * N2* N3>
		inline decltype(auto) cast_array(T(&array)[N1][N2][N3])
		{
			using array_t = T[N];

			return *reinterpret_cast<array_t*>(array);
		}

		template<typename T, size_t N>
			inline decltype(auto) cast_array(std::array<T, N>& array)
		{
			using c_array_t = T[N];

			return *reinterpret_cast<c_array_t*>(array.data());
		}

		template<size_t N1, size_t N2, typename T, size_t N,
			typename = std::enable_if_t< N1 * N2 == N>>
		inline decltype(auto) cast_array(std::array<T, N>& array)
		{
			using c_array_t = T[N1][N2];

			return *reinterpret_cast<c_array_t*>(array.data());
		}
		
		template<size_t N1, size_t N2, size_t N3, typename T, size_t N,
			typename = std::enable_if_t< N1 * N2 * N3 == N>>
			inline decltype(auto) cast_array(std::array<T, N>& array)
		{
			using c_array_t = T[N1][N2][N3];

			return *reinterpret_cast<c_array_t*>(array.data());
		}

        // always returns lvalue reference
        template<auto Index, typename... ArgTypes,
        typename = std::enable_if_t<(Index < sizeof...(ArgTypes))>>
        decltype(auto) get_nth_argument(ArgTypes&&... args)
        {
            auto arguments = std::forward_as_tuple( std::forward<ArgTypes>(args)... );

            using element_t = decltype(std::get<Index>(arguments));

            return std::forward<element_t>(std::get<Index>(arguments));
        }

        /**
         * @brief This type is used to manipulate type list.
         * 
         * @tparam Types for ZERO or more variadic template parameters.
         */
        template<typename... Types> struct type_list_t{};
        template<auto... Ints> struct numbers_t{};

        template<typename T, size_t Size>
        auto convert_to_tuple(const std::array<T, Size>& array);
        
        template<typename Type, auto Size>
        using create_tuple_t = decltype(types::convert_to_tuple(std::array<Type, Size>{}));
        
        namespace hidden
        {
            template<typename Type>
            struct st_is_vector_v
            {
                static constexpr bool value = false;
            };

            template<typename Type, typename... Types>
            struct st_is_vector_v<std::vector<Type, Types...>>
            {
                static constexpr bool value = true;
            };
           
            template<typename Type>
            struct st_is_integer_sequence
            {
                static constexpr bool value = false;
            };
            
            template<typename Type, auto... Indices>
            struct st_is_integer_sequence<std::integer_sequence<Type, Indices...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr bool is_integer_sequence_v = st_is_integer_sequence<Type>::value;

            template<typename T>
            struct st_is_type_list
            {
                static constexpr bool value = false;
            };

            template<typename... Ts>
            struct st_is_type_list<type_list_t<Ts...>>
            {
                static constexpr bool value = true;
            };

            template<typename T>
            constexpr bool is_type_list_v = st_is_type_list<remove_cv_ref_t<T>>::value;

            template<typename T>
            struct st_recursive_type_list
            {
                using type = T;
            };

            template<template<typename...> class TemplateType, typename... Types>
            struct st_recursive_type_list<TemplateType<Types...>>
            {
                using type = type_list_t<typename st_recursive_type_list<Types>::type...>;
            };

            template<typename T>
            using to_recursive_type_list_t = typename st_recursive_type_list<T>::type;

        } // end of namespace hidden        

        template<typename Type>
        constexpr bool is_vector_v = hidden::st_is_vector_v<remove_cvref_t<Type>>::value;

        template<typename Type>
        constexpr bool is_integer_sequence_v = hidden::is_integer_sequence_v<remove_cvref_t<Type>>;

        template<typename T>
        constexpr bool is_type_list_v = hidden::is_type_list_v<remove_cv_ref_t<T>>;

        template<typename T>
        using to_recursive_type_list_t = hidden::to_recursive_type_list_t<remove_cv_ref_t<T>>;

        /**
         * @brief Create type_list_t by removing const, volatile, reference
         * 
         * @tparam Types 
         */
        template<typename... Types>
        using remove_cv_ref_type_list_t =  type_list_t<tpf::remove_cv_ref_t<Types>...>;

        /**
         * @brief Create type_list_t by decaying and removing const, volatile, reference
         * 
         * @tparam Types 
         */
        template<typename... Types>
        using decay_remove_cv_ref_type_list_t =  type_list_t<tpf::decay_remove_cv_ref_t<Types>...>;

        /**
         * @brief Create type_list_t by decaying and removing reference
         * 
         * @tparam Types 
         */
        template<typename... Types>
        using decay_remove_ref_type_list_t =  type_list_t<tpf::decay_remove_ref_t<Types>...>;

        enum class TemplateCategory { Unknown, NonTemplate = Unknown, Type, Type_Value, Value, Value_Type };

        template<template<typename...> class TemplateType>
        constexpr auto template_category() { return TemplateCategory::Type; }

        template<template<typename T, auto...> class TemplateType>
        constexpr auto template_category() { return TemplateCategory::Type_Value; }

        template<template<auto...> class TemplateType>
        constexpr auto template_category() { return TemplateCategory::Value; }

        template<template<auto, typename...> class TemplateType>
        constexpr auto template_category() { return TemplateCategory::Value_Type; }

        template<typename> 
        constexpr auto template_category(...) { return TemplateCategory::Unknown; }

        namespace hidden
        {
            template<typename Type>
            struct st_is_template_type
            {
                static constexpr bool value = false;
            };

            template<template<typename...> class TemplateType, typename... Types>
            struct st_is_template_type<TemplateType<Types...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename T, auto...> class TemplateType, typename T, T... args>
            struct st_is_template_type<TemplateType<T, args...>>
            {
                static constexpr bool value = true;
            };

            template<template<auto...> class TemplateType, auto... args>
            struct st_is_template_type<TemplateType<args...>>
            {
                static constexpr bool value = true;
            };

            template<template<auto, typename...> class TemplateType, auto arg, typename... Types>
            struct st_is_template_type<TemplateType<arg, Types...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr bool is_template_type_v = st_is_template_type<Type>::value;
        }

        template<typename Type>
            constexpr bool is_template_type_v = hidden::is_template_type_v<remove_cvref_t<Type>>;

        namespace hidden
        {
            template<typename... Types>
            struct st_type_list_to_tuple_of_vectors
            {
                using type = std::tuple< std::vector<Types>... >;
            };

            template<typename... Types>
            struct st_type_list_to_tuple_of_vectors<type_list_t<Types...>>
            {
                using type = std::tuple<std::vector<Types>... >;
            };

            template<template<typename...> class CntrType, typename... Types>
            struct st_type_list_to_tuple_of_vectors<CntrType<Types...>>
            {
                using type = std::tuple< std::vector<Types>... >;
            };

            template<typename TupleType>
            struct st_tuple_to_type_list;

            template<typename... Types>
            struct st_tuple_to_type_list<std::tuple<Types...>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename TupleType>
            using tuple_to_type_list_t = typename st_tuple_to_type_list<TupleType>::type; 

            template<typename TypeList>
            struct st_type_list_to_tuple;

            template<typename... Types>
            struct st_type_list_to_tuple<type_list_t<Types...>>
            {
                using type = std::tuple<Types...>;
            };

            template<typename TypeList>
            using type_list_to_tuple_t = typename st_type_list_to_tuple<TypeList>::type; 

            template<typename TupleType>
            struct st_variant_to_type_list;

            template<typename... Types>
            struct st_variant_to_type_list<std::variant<Types...>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename VarType>
            using variant_to_type_list_t = typename st_variant_to_type_list<VarType>::type; 

        } // end of namespace hidden

        template<typename TupleType>
        using tuple_to_type_list_t = hidden::tuple_to_type_list_t<remove_cvref_t<TupleType>>; 

        template<typename TypeList>
        using type_list_to_tuple_t = hidden::type_list_to_tuple_t<remove_cvref_t<TypeList>>;

        template<typename VarType>
        using variant_to_type_list_t = hidden::variant_to_type_list_t<remove_cvref_t<VarType>>; 

        template<typename... Types>
        using tuple_of_vectors_t = typename hidden::st_type_list_to_tuple_of_vectors<Types...>::type;

        namespace hidden
        {
            template<std::size_t ...I>
            struct st_vector
            {
                using type = std::vector<std::size_t>;
            };

            template<typename Type> struct st_index_tuple_vector;
            
            template<std::size_t ...Is>
            struct st_index_tuple_vector<std::index_sequence<Is...>>
            {
                using type = std::tuple<typename st_vector<Is>::type...>;
            };
        
        } // end of namespace hidden
        
        template<std::size_t N>
        using index_tuple_vector_t = typename hidden::st_index_tuple_vector<std::make_index_sequence<N>>::type;

        template<typename PointerType, typename SizeType>
        struct array_wrapper_t: public std::pair<PointerType, SizeType>
        {
            public:
                using base_type = std::pair<PointerType, SizeType>;
               
                template<typename ElementType, size_t ElementCount>
                array_wrapper_t(ElementType(&array)[ElementCount]) noexcept:
                    std::pair<ElementType*, size_t>{array, ElementCount} { }
        };

        template<typename ElementType, size_t ElementCount>
        array_wrapper_t(ElementType(&)[ElementCount]) -> array_wrapper_t<ElementType*, size_t>;

        template<typename ElementType, size_t ElementSize>
        auto array_wrapper(ElementType(&array)[ElementSize]) noexcept
        {
            return array_wrapper_t<ElementType*, size_t>{array};
        }

        template<typename Type>
        auto to_ref(const Type& value) { return std::ref(const_cast<Type&>(value)); }
        
        template<typename Type>
        constexpr bool is_array_v = std::is_array_v<remove_cv_ref_t<Type>>;      

        template<typename ElementType, size_t ElementSize>
        constexpr size_t array_size(ElementType(&array)[ElementSize]) noexcept { return (size_t) ElementSize; }
              
        // forward declaration
        template<typename ContainerType> class reverse_st;
        template<typename ContainerType> auto reverse(ContainerType&& container);
        template<typename ElementType, size_t ElementCount> auto reverse(ElementType(&array)[ElementCount]);

        template<typename Type, typename... Types> auto reverse(Type&& arg, Types&&... args);
        template<typename Type, typename... Types> auto make_vector(Type&& arg, Types&&... args);
        template<typename Type, typename... Types> auto make_container(Type&& arg, Types&&... args);

        template<typename ContainerType, typename IndexType>
        decltype(auto) get_element(ContainerType container, IndexType index);

        #if !defined(__clang_major__)
            template<typename Type, typename... Types> auto make_variants(Type&& arg, Types&&... args);
        #endif

        template<typename Type>
        decltype(auto) decay(Type&& arg) 
        { 
            using type = std::remove_reference_t<Type>;

            if constexpr(std::is_array_v<type> || std::is_function_v<type>)
                return decay_remove_cv_ref_t<type>(arg);
            else
                return std::forward<Type>(arg);
        }

        /**
         * @brief Returns Type's string name.
         * 
         * @tparam Type &mdash; Constraints: NONE
         * @return std::string &mdash; if fails, DOES NOT FAIL
         * @sa <a target ="_blank" href="001-type__to__string_8cpp_source.html">001-type_to_string.cpp</a>
         */
        template<typename Type>
        std::string type_to_string()
        {
            #ifdef __FUNCSIG__
                std::string fname(__FUNCSIG__);
                const char* to_str = "to_string<";
                size_t len = strlen(to_str);
                auto pos = fname.find("to_string<");
                fname = fname.substr(pos+len);
                return fname.substr(0, fname.find_last_of('>'));
            #else
                
                std::string fname(__PRETTY_FUNCTION__);
                
                #ifdef __clang_major__
                    const char* ftext = "[Type = ";
                    auto pos = fname.find(ftext) + strlen(ftext);
                    fname = fname.substr(pos);
                    fname.pop_back();
                    return fname;

                #elif defined(__ICL)
                    const char* ftext = "type_to_string<";
                    auto pos = fname.find(ftext) + strlen(ftext);
                    fname = fname.substr(pos);
                    pos = fname.find_last_of('>');
                    return fname.substr(0, pos);
                #else
                    const char* ftext = "[with Type = ";
                    auto pos = fname.find(ftext) + strlen(ftext);
                    fname = fname.substr(pos);
                    pos = fname.find_first_of(';');
                    return fname.substr(0, pos);
                 #endif
             #endif

        } // end of type_to_string()

    } // end of namespace types

} // end of namespace tpf

#define Tpf_DebugException(debug_message) tpf::debug_exception{debug_message, __LINE__, __FUNCTION_NAME__, __FILE__}


/**
 * @brief Throw a debug_exception with \a message as argument
 * 
 */
#define Tpf_ThrowDebugException(debug_message) throw tpf::debug_exception{debug_message, __LINE__, __FUNCTION_NAME__, __FILE__}

 /**
  * @brief A macro that returns type_arg's string name
  * @sa <a target ="_blank" href="001-type__to__string_8cpp_source.html">001-type_to_string.cpp</a>
  */
#define Tpf_GetTypeName(type_arg) tpf::types::type_to_string<type_arg>()
 
 /**
  * @brief A macro that returns instance_arg's type category string name 
  * @sa <a target ="_blank" href="001-type__to__string_8cpp_source.html">001-type_to_string.cpp</a>
  */
#define Tpf_GetTypeCategory(instance_arg) Tpf_GetTypeName(decltype(instance_arg))
 
/**
 * @brief A macro that returns instance_arg's value category string name 
 * @sa <a target ="_blank" href="001-type__to__string_8cpp_source.html">001-type_to_string.cpp</a>
 */
#define Tpf_GetValueCategory(instance_arg) Tpf_GetTypeName(decltype((instance_arg)))

namespace tpf::types
{
    template<typename Type, int Tag=0>
    class tag_type
    {
        public:

        using type = Type;
        static constexpr int tag = Tag;

        protected:
            type value;
        
        public:
                
        tag_type(Type v = Type{}): value{ std::move(v) } { }

        template<typename ArgType, typename... ArgTypes>
        tag_type(ArgType&& arg, ArgTypes&&... args): value{ std::move(arg), std::move(args)... } { }

        tag_type(const tag_type&) = default;
        tag_type& operator=(const tag_type&) = default;

        tag_type(tag_type&&) = default;
        tag_type& operator=(tag_type&&) = default;
        
        const Type& get() const { return value; }
        Type& get() { return value; }

        operator const Type&() const { return get(); }
        operator Type&() { return get(); }
    };

    namespace hidden
    {
        template<typename Type>
        struct st_is_tag_type_v
        {
            constexpr static bool value = false;
        };

        template<typename Type, int Tag>
        struct st_is_tag_type_v<tag_type<Type, Tag>>
        {
            constexpr static bool value = true;
        };

        template<typename Type>
        constexpr bool is_tag_type_v = st_is_tag_type_v<Type>::value;

        template<typename Type>
        struct st_is_numbers_type_v
        {
            constexpr static bool value = false;
        };

        template<template<auto...> class ContainerType, auto... Ints>
        struct st_is_numbers_type_v<ContainerType<Ints...>>
        {
            constexpr static bool value = true;
        };

    } // end of namespace hidden

    template<typename Type>
    constexpr bool is_tag_type_v = hidden::is_tag_type_v<remove_cvref_t<Type>>;

    template<typename Type>
    constexpr bool is_numbers_type_v = hidden::st_is_numbers_type_v<remove_cvref_t<Type>>::value;

    template<typename Type>
    decltype(auto) get_value(Type&& arg)
    {
        if constexpr(is_tag_type_v<Type>)
        {
            if constexpr(std::is_rvalue_reference_v<decltype(arg)>)
                return std::move(arg.value);
            else
                return arg.get();
        }
        else
            return std::forward<Type>(arg);
    }

    namespace hidden
    {
        // assume non-primitive type
        template<typename Type, auto Tag, bool = false>
        struct st_make_non_class_wrapper
        {
            using type = Type;
        };

        template<typename Type, auto Tag>
        struct st_make_non_class_wrapper<Type, Tag, true>
        {
            using type = tag_type<Type, Tag>;
        };

        template<typename Type, auto Tag>
        using non_class_wrapper_t = typename
            st_make_non_class_wrapper<Type, Tag, std::is_class_v<Type>>::type;

    } // end of namespace hidden

    template<typename Type, auto Tag>
        using non_class_wrapper_t = hidden::non_class_wrapper_t<Type, Tag>;
            
} // end of namespace tpf::types

/**
 * @brief Root namespace for C++ Extension Library
 * 
 */
namespace tpf
{
    template<typename IndexType, typename ContainerType,
    typename container_t = tpf::remove_cv_ref_t<ContainerType>,
    typename iterator_type = typename container_t::iterator>
    auto index_to_iterator(ContainerType&& cntr, IndexType&& index);

    template<typename IndexType,  typename ContainerType,
    typename container_t = tpf::remove_cv_ref_t<ContainerType>,
     typename reverse_iterator_type = typename container_t::reverse_iterator>
    auto index_to_reverse_iterator(ContainerType&& cntr, IndexType&& offset);

    template<typename ContainerType, 
    typename container_t = tpf::remove_cv_ref_t<ContainerType>,
    typename iterator_type = typename container_t::iterator>
    auto iterator_to_index(ContainerType&& cntr, iterator_type&& offset);

    template<typename ContainerType,
    typename container_t = tpf::remove_cv_ref_t<ContainerType>,
    typename reverse_iterator_type = typename container_t::reverse_iterator>
    auto reverse_iterator_to_index(ContainerType&& cntr, reverse_iterator_type&& offset);

    template<typename ReverseIteratorType,
        typename reverse_iterator_type = std::remove_reference_t<ReverseIteratorType>>
    inline auto reverse_iterator_to_iterator(ReverseIteratorType&& itr)
    {
        return itr.base() - 1;
    }

    template<typename IteratorType,
        typename iterator_type = std::remove_reference_t<IteratorType>>
    inline auto iterator_to_reverse_iterator(IteratorType&& itr)
    {
        return std::make_reverse_iterator(std::forward<IteratorType>(itr));
    }

    template<direction_t direction = direction_t::left, 
    typename ContainerType = std::vector<int>,
    typename container_t = tpf::remove_cv_ref_t<ContainerType>,
    typename iterator_type = typename container_t::iterator,
    typename reverse_iterator_type = typename container_t::reverse_iterator>
    auto make_rotator(ContainerType&& cntr);

    template<direction_t direction = direction_t::left, 
    typename ContainerType = std::vector<int>,
    typename container_t = tpf::remove_cv_ref_t<ContainerType>,
    typename iterator_type = typename container_t::iterator,
    typename reverse_iterator_type = typename container_t::reverse_iterator,
    typename execution_policy_type = std::execution::parallel_unsequenced_policy>
    auto make_rotator(ContainerType&& cntr, execution_policy_type policy);

    template<typename ForwardIterator, typename EleType, typename CompareCallbackType = std::less<>>
    auto find_range_iterators(ForwardIterator first, ForwardIterator last, 
        const EleType& value, CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        auto range = std::equal_range(first, last, value, std::forward<CompareCallbackType>(compare_callback));
        
        return std::make_tuple(std::distance(range.first, range.second), range.first, range.second);
    }

    template<typename ForwardIterator, typename EleType, typename CompareCallbackType = std::less<>>
    auto find_range_indices(ForwardIterator first, ForwardIterator last, 
        const EleType& value, CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        auto range = std::equal_range(first, last, value, std::forward<CompareCallbackType>(compare_callback));
        
        return std::make_tuple(std::distance(range.first, range.second),
                std::distance(first, range.first), std::distance(first, range.second));
    }

    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types,
        typename CompareCallbackType = std::less<>>
    auto find_range_iterators(const ContainerType<EleType, Types...>& container, const EleType& value, 
        CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        return find_range_iterators(container.cbegin(), container.cend(),
            value, std::forward<CompareCallbackType>(compare_callback));
    }

    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types,
        typename CompareCallbackType = std::less<>>
    auto find_range_indices(const ContainerType<EleType, Types...>& container, const EleType& value, 
        CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        return find_range_indices(container.cbegin(), container.cend(),
            value, std::forward<CompareCallbackType>(compare_callback));
    }

    template<typename ForwardIterator, typename EleType, typename CompareCallbackType = std::less<>>
    auto binary_find_iterator(ForwardIterator first, ForwardIterator last, 
        const EleType& value, CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        auto start = std::lower_bound(first, last, value, compare_callback);
        
        return start != last && !compare_callback(value, *start) ? start : last;
    }

    template<typename ForwardIterator, typename EleType, typename CompareCallbackType = std::less<>>
    auto binary_find_index(ForwardIterator first, ForwardIterator last, 
        const EleType& value, CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        auto start = std::lower_bound(first, last, value, compare_callback);
      
        auto iterator = (start != last && !compare_callback(value, *start)) ? start : last;

        return (iterator != last) ? 
            std::distance(first, iterator) : std::distance(first, last);   
    }

    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types,
        typename CompareCallbackType = std::less<>>
    auto binary_find_iterator(const ContainerType<EleType, Types...>& container, const EleType& value, 
        CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        return binary_find_iterator(container.cbegin(), container.cend(), value,
            std::forward<CompareCallbackType>(compare_callback));
    }

     template<typename ForwardIterator, typename EleType, typename CompareCallbackType = std::less<>>
    auto binary_find_bool_iterator_pair(ForwardIterator first, ForwardIterator last, 
        const EleType& value, CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        auto iterator = binary_find_iterator(first, last, value,
            std::forward<CompareCallbackType>(compare_callback));

        return (iterator != last) ? 
            std::make_pair(true, iterator) : std::make_pair(false, iterator);
    }

    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types,
        typename CompareCallbackType = std::less<>>
    auto binary_find_bool_iterator_pair(const ContainerType<EleType, Types...>& container, const EleType& value, 
        CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        return binary_find_bool_iterator_pair(container.cbegin(), container.cend(), value,
            std::forward<CompareCallbackType>(compare_callback));
    }

    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types,
        typename CompareCallbackType = std::less<> >
    auto binary_find_index(const ContainerType<EleType, Types...>& container, const EleType& value, 
        CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        return binary_find_index(container.cbegin(), container.cend(), value,
            std::forward<CompareCallbackType>(compare_callback));
    }

    template<typename ForwardIterator, typename EleType, typename CompareCallbackType = std::less<>>
    auto binary_find_bool_index_pair(ForwardIterator first, ForwardIterator last, 
        const EleType& value, CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        auto start = std::lower_bound(first, last, value, compare_callback);

        auto iterator = (start != last && !compare_callback(value, *start)) ? start : last;

        return (iterator != last) ? 
            std::make_pair(true,  std::distance(first, iterator)) :
            std::make_pair(false, std::distance(first, iterator));
    }

    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types,
        typename CompareCallbackType = std::less<>>
    auto binary_find_bool_index_pair(const ContainerType<EleType, Types...>& container, const EleType& value, 
        CompareCallbackType&& compare_callback = CompareCallbackType{})
    {
        return binary_find_bool_index_pair(container.cbegin(), container.cend(), value,
            std::forward<CompareCallbackType>(compare_callback));
    }

    template<typename Type>
    constexpr auto type_max_v = std::numeric_limits<Type>::max();
    constexpr size_t InvalidIndex = type_max_v<size_t>;
    constexpr size_t SelectAll = InvalidIndex;

    using big_integer_t = long long;
    using big_unsigned_t = unsigned long long;

    struct thread_bundle
    {
        int max_thread_count;
        std::atomic<int> thread_count{0};
        std::mutex mutex; 
    };

    struct boolean
	{
		static constexpr bool Left = false;
		static constexpr bool Right = true;

		static constexpr bool Or = false;
		static constexpr bool And = true;

		static constexpr bool No = false;
		static constexpr bool Yes = true;

		static constexpr bool False = false;
		static constexpr bool True = true;

		static constexpr bool Before = false;
		static constexpr bool After = true;

		static constexpr bool Prepend = false;
		static constexpr bool Append = true;
	};

    namespace types
    {
        /**
         * @brief Test if a function is specified with the keyword noexcept
         * 
         * @tparam FuncType 
         * @tparam ArgTypes 
         */
        template<auto FuncType, typename... ArgTypes>
        constexpr auto is_noexcept_v = noexcept(FuncType(std::declval<ArgTypes>()...));
        
        namespace hidden
        {
            template<typename Type>
            struct is_variant_st
            {
                static constexpr bool value = false;
            };

            template<typename... Types>
            struct is_variant_st<std::variant<Types...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_variant_v = is_variant_st<Type>::value;

            template<typename Type>
            struct is_unique_ptr_st
            {
                static constexpr bool value = false;
            };

            template<typename Type, typename... Types>
            struct is_unique_ptr_st<std::unique_ptr<Type, Types...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_unique_ptr_v = is_unique_ptr_st<Type>::value;

            template<typename Type>
            struct is_pair_st
            {
                static constexpr bool value = false;
            };

            template<typename Type_1, typename Type_2>
            struct is_pair_st<std::pair<Type_1, Type_2>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_pair_v = is_pair_st<Type>::value;
            
            /////////////////////////////////
            template<typename Type>
            struct is_pair_of_variant_st
            {
                static constexpr bool value = false;
            };

            template<typename Type, typename... Types>
            struct is_pair_of_variant_st<std::pair<Type, Types...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_pair_of_variant_v 
                = is_pair_of_variant_st<Type>::value;

            template<typename Type>
            struct is_map_or_unordered_map_st
            {
                using type = Type;
                static constexpr bool value = false;
            };

            template<typename KeyType, typename ValueType, typename... Types>
            struct is_map_or_unordered_map_st<std::map<KeyType, ValueType, Types...>>
            {
                using type = std::pair<KeyType, ValueType>;
                static constexpr bool value = true;
            };

            template<typename KeyType, typename ValueType, typename... Types>
            struct is_map_or_unordered_map_st<std::unordered_map<KeyType, ValueType, Types...>>
            {
                using type = std::pair<KeyType, ValueType>;
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_map_or_unordered_map_v = is_map_or_unordered_map_st<remove_cv_ref_t<Type>>::value;

            template<typename Type>
            using map_or_unordered_map_pair_t = typename is_map_or_unordered_map_st<remove_cv_ref_t<Type>>::type;

            template<typename Type>
            struct is_set_or_unordered_set_st
            {
                using type = Type;
                static constexpr bool value = false;
            };
            
            template<typename Type, typename... Types>
            struct is_set_or_unordered_set_st<std::set<Type, Types...>>
            {
                using type = Type;
                static constexpr bool value = true;
            };

            template<typename Type, typename... Types>
            struct is_set_or_unordered_set_st<std::unordered_set<Type, Types...>>
            {
                using type = Type;
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_set_or_unordered_set_v = is_set_or_unordered_set_st<Type>::value;

            template<typename Type>
            struct is_tuple_st
            {
                static constexpr bool value = false;
                static constexpr bool empty = true;
            };

            template<>
            struct is_tuple_st<std::tuple<>>
            {
                static constexpr bool value = true;
                static constexpr bool empty = true;
            };

            template<typename Type, typename... Types>
            struct is_tuple_st<std::tuple<Type, Types...>>
            {
                static constexpr bool value = true;
                static constexpr bool empty = false;
            };

            template<typename Type>
            constexpr auto is_tuple_v = is_tuple_st<Type>::value;

            template<typename Type>
            constexpr auto is_tuple_empty_v = is_tuple_st<Type>::empty;

            template<typename Type>
            struct is_any_st
            {
                static constexpr bool value = false;
            };

            template<>
            struct is_any_st<std::any>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_any_v = is_any_st<Type>::value;

            template<typename Type>
            struct is_basic_string_st
            {
                static constexpr bool value = false;
            };

            template<typename Type, typename... Types>
            struct is_basic_string_st<std::basic_string<Type, Types...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type, typename... Types>
            struct is_basic_string_st<std::basic_string_view<Type, Types...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_basic_string_v = is_basic_string_st<Type>::value;

            template<typename Type>
            struct is_char_st
            {
                static constexpr bool value = false;
            };

            template<>
            struct is_char_st<char>
            {
                static constexpr bool value = true;
            };

            template<>
            struct is_char_st<unsigned char>
            {
                static constexpr bool value = true;
            };
            
            template<typename Type>
            constexpr bool is_char_v = is_char_st<tpf::remove_cv_ref_t<Type>>::value;

        } // end of namespace hidden

        template<typename Type>
        constexpr bool is_char_v = hidden::is_char_v<Type>;  

        template<typename Type>
        constexpr auto is_variant_v = hidden::is_variant_v<Type>;

        template<typename Type>
        constexpr auto is_unique_ptr_v = hidden::is_variant_v<Type>;

        template<typename Type>
        constexpr auto is_pair_v = hidden::is_pair_v<Type>;

        template<typename Type>
        constexpr auto is_pair_of_variant_v = hidden::is_pair_of_variant_v<Type>;

        template<typename Type>
        constexpr auto is_tuple_v = hidden::is_tuple_v<remove_cvref_t<Type>>;

        template<typename Type>
        constexpr auto is_tuple_empty_v = hidden::is_tuple_empty_v<remove_cvref_t<Type>>;

        namespace hidden
        {
            template<typename Type>
            struct st_tuple_size
            {
                static constexpr size_t value = 1;
            };

            template<typename Type, typename... ArgTypes> 
            struct st_tuple_size<std::tuple<Type, ArgTypes...>>
            {
                static constexpr size_t value = 1 + sizeof...(ArgTypes);
            };

            template<typename Type>
            constexpr auto tuple_size_v = st_tuple_size<Type>::value;
        }

        template<typename Type>
        constexpr auto tuple_size_v = hidden::tuple_size_v<remove_cvref_t<Type>>;
          
        template<typename Type>
        constexpr auto is_any_v = hidden::is_any_v<Type>;

        template<typename Type>
        constexpr auto is_map_or_unordered_map_v = hidden::is_map_or_unordered_map_v<Type>;

        template<typename Type>
        using map_or_unordered_map_pair_t = hidden::map_or_unordered_map_pair_t<Type>;

        template<typename Type>
        constexpr auto is_basic_string_v = hidden::is_basic_string_v<Type>;

        template<typename Type>
        constexpr auto is_set_or_unordered_set_v = hidden::is_set_or_unordered_set_v<Type>;

    } // end of namespace types
        
    template<template<typename, typename...> class ContainerType, typename EleType, typename... Types>
    struct set_tag
    {
        using set_element_t = EleType;
        
        using set_t = ContainerType<EleType, Types...>;
        using sets_t = ContainerType<ContainerType<EleType, Types...>>;
        using set_of_sets_t = ContainerType<ContainerType<ContainerType<EleType, Types...>>>;
        using sets_of_sets_t = ContainerType<ContainerType<ContainerType<ContainerType<EleType, Types...>>>>;

        using duet_set_element_t = std::tuple<set_element_t, set_element_t>;
        using duet_set_t = std::tuple<set_t, set_t>;
        using duet_sets_t = std::tuple<sets_t, sets_t>;
        using duet_set_of_sets_t = std::tuple<set_of_sets_t, set_of_sets_t>;
        using duet_sets_of_sets_t = std::tuple<sets_of_sets_t, sets_of_sets_t>;

        using set_of_duets_t = ContainerType<duet_set_t>;
        
        using trio_set_element_t = std::tuple<set_element_t, set_element_t, set_element_t>;
        using trio_set_t = std::tuple<set_t, set_t, set_t>;
        using trio_sets_t = std::tuple<sets_t, sets_t, sets_t>;
        using trio_set_of_sets_t = std::tuple<set_of_sets_t, set_of_sets_t, set_of_sets_t>;
        using trio_sets_of_sets_t = std::tuple<sets_of_sets_t, sets_of_sets_t, sets_of_sets_t>;
    };

    template<typename SetTagType = set_tag<std::vector, int>>
    using set_element_t = typename SetTagType::set_element_t;

    template<typename SetTagType = set_tag<std::vector, int>>
    using set_t = typename SetTagType::set_t;
    
    template<typename SetTagType = set_tag<std::vector, int>>
    using sets_t = typename SetTagType::sets_t;

    template<typename SetTagType = set_tag<std::vector, int>>
    using set_of_sets_t = typename SetTagType::set_of_sets_t;
    
    template<typename SetTagType = set_tag<std::vector, int>>
    using sets_of_sets_t = typename SetTagType::sets_of_sets_t;
    
    template<typename SetTagType = set_tag<std::vector, int>>
    using duet_set_element_t = typename SetTagType::duet_set_element_t;
    
    template<typename SetTagType = set_tag<std::vector, int>>
    using duet_set_t = typename SetTagType::duet_set_t;
    
    template<typename SetTagType = set_tag<std::vector, int>>
    using duet_sets_t = typename SetTagType::duet_sets_t;
    
    template<typename SetTagType = set_tag<std::vector, int>>
    using duet_set_of_sets_t = typename SetTagType::duet_set_of_sets_t;

    template<typename SetTagType = set_tag<std::vector, int>>
    using duet_sets_of_sets_t = typename SetTagType::duet_sets_of_sets_t;

    template<typename SetTagType = set_tag<std::vector, int>>
    using set_of_duets_t = typename SetTagType::set_of_duets_t;
    
    template<typename SetTagType = set_tag<std::vector, int>>
    using trio_set_element_t = typename SetTagType::trio_set_element_t;

    template<typename SetTagType = set_tag<std::vector, int>>
    using trio_set_t = typename SetTagType::trio_set_t;

    template<typename SetTagType = set_tag<std::vector, int>>
    using trio_sets_t = typename SetTagType::trio_sets_t;

    template<typename SetTagType = set_tag<std::vector, int>>
    using trio_set_of_sets_t = typename SetTagType::trio_set_of_sets_t;
    
    template<typename SetTagType = set_tag<std::vector, int>>
    using trio_sets_of_sets_t = typename SetTagType::trio_sets_of_sets_t;
    
     /**
     * @brief Type functions are defined.
     * 
     */
    namespace types
    {
        /**
         * @brief This type is used to test validity of a type.
         * 
         */
        struct no_type_t{}; // "zero" in C++ type system
            // no_type_t plays the role of zero in C++ type system.
        
        using no_throw_t = no_type_t;
        using invalid_t = no_type_t;

        template<typename Type>
        constexpr bool is_no_type_v = std::is_same_v<Type, no_type_t>;
        
        template<typename CharType>
        std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, const no_type_t&)
        {
            os << "invalid"; return os; 
        }

        template<typename CharType>
        std::basic_ostream<CharType>&
        operator>>(std::basic_ostream<CharType>& os, const no_type_t&)
        {
            os << "no_type_t"; return os; 
        }

        namespace hidden
        {
            template<typename T, typename S>
            auto addition_vaild_fn(T t, S s) -> decltype(t + s);
            no_type_t addition_vaild_fn(...);

            template<typename T, typename S>
                constexpr bool is_addition_valid_v = 
                    !is_no_type_v<decltype(addition_vaild_fn(std::declval<T>(), std::declval<S>()))>;

            template<typename T, typename S>
            auto subtraction_vaild_fn(T t, S s) -> decltype(t - s);
            no_type_t subtraction_vaild_fn(...);

            template<typename T, typename S>
                constexpr bool is_subtraction_valid_v = 
                    !is_no_type_v<decltype(subtraction_vaild_fn(std::declval<T>(), std::declval<S>()))>;

            template<typename T, typename S>
            auto multiplication_vaild_fn(T t, S s) -> decltype(t * s);
            no_type_t multiplication_vaild_fn(...);

            template<typename T, typename S>
                constexpr bool is_multiplication_valid_v = 
                    !is_no_type_v<decltype(multiplication_vaild_fn(std::declval<T>(), std::declval<S>()))>;

            template<typename T, typename S>
            auto division_vaild_fn(T t, S s) -> decltype(t / s);
            no_type_t division_vaild_fn(...);

            template<typename T, typename S>
                constexpr bool is_division_valid_v = 
                    !is_no_type_v<decltype(division_vaild_fn(std::declval<T>(), std::declval<S>()))>;

            template<typename T, typename S>
                constexpr bool is_arithmetic_valid_v = is_addition_valid_v<T, S> && is_subtraction_valid_v<T, S> 
                    && is_multiplication_valid_v<T, S> && is_division_valid_v<T, S>;
        }

        template<typename T, typename S>
            constexpr bool is_addition_valid_v = 
                hidden::is_addition_valid_v<remove_cvref_t<T>, remove_cvref_t<S>>;

        template<typename T, typename S>
            constexpr bool is_subtraction_valid_v = 
                hidden::is_subtraction_valid_v<remove_cvref_t<T>, remove_cvref_t<S>>;

        template<typename T, typename S>
            constexpr bool is_multiplication_valid_v = 
                hidden::is_multiplication_valid_v<remove_cvref_t<T>, remove_cvref_t<S>>;

        template<typename T, typename S>
            constexpr bool is_division_valid_v = 
                hidden::is_division_valid_v<remove_cvref_t<T>, remove_cvref_t<S>>;

        template<typename T, typename S>
            constexpr bool is_arithmetic_valid_v = 
                hidden::is_arithmetic_valid_v<remove_cvref_t<T>, remove_cvref_t<S>>;
        
        struct void_type_t{};
        
        namespace hidden
        {
            template<typename Type> struct return_type_st 
            {
                using type = Type;
            };

            template<> struct return_type_st<void>
            {
                using type = void_type_t;
            };

            template<typename Type>
            using return_type_t = typename return_type_st<Type>::type;

        } // end of namespace hidden

        template<typename Type>
        using return_type_t = hidden::return_type_t<Type>;

        template<typename Type>
        constexpr bool is_valid_type_v = !is_no_type_v<Type>;

        inline constexpr no_type_t operator+(no_type_t, no_type_t) noexcept { return {}; }
        inline constexpr no_type_t operator-(no_type_t, no_type_t) noexcept { return {}; }
        inline constexpr no_type_t operator*(no_type_t, no_type_t) noexcept { return {}; }
        inline constexpr no_type_t operator/(no_type_t, no_type_t) noexcept { return {}; }

        inline constexpr bool operator==(no_type_t, no_type_t) noexcept { return true; }
        inline constexpr bool operator!=(no_type_t, no_type_t) noexcept { return false; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, bool>
        operator==(no_type_t, T&&) noexcept { return false; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, bool>
        operator!=(T&&, no_type_t) noexcept { return false; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, no_type_t>
        operator+(no_type_t, T&&) noexcept { return {}; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, no_type_t>
        operator+(T&&, no_type_t) noexcept { return {}; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, no_type_t>
        operator-(no_type_t, T&&) noexcept { return {}; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, no_type_t>
        operator-(T&&, no_type_t) noexcept { return {}; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, no_type_t>
        operator*(no_type_t, T&&) noexcept { return {}; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, no_type_t>
        operator*(T&&, no_type_t) noexcept { return {}; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, no_type_t>
        operator/(no_type_t, T&&) noexcept { return {}; }

        template<typename T>
        constexpr std::enable_if_t<!is_no_type_v<remove_cvref_t<T>>, no_type_t>
        operator/(T&&, no_type_t) noexcept { return {}; }

        namespace hidden
        {
            template<typename Type>
            struct st_is_std_array
            {
                static constexpr bool value = false;
            };

            template<typename Type, size_t Size>
            struct st_is_std_array<std::array<Type, Size>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            struct st_std_array_element
            {
                static constexpr size_t value = 0;
                using type = no_type_t;
            };

            template<typename Type, size_t Size>
            struct st_std_array_element<std::array<Type, Size>>
            {
                static constexpr size_t value = Size;
                using type = Type;
            };
        }

        template<typename Type>
        constexpr bool is_std_array_v = hidden::st_is_std_array<remove_cv_ref_t<Type>>::value;

        template<typename Type>
        constexpr size_t std_array_size_v = hidden::st_std_array_element<remove_cv_ref_t<Type>>::value;

        template<typename Type>
        using std_array_element_t = typename hidden::st_std_array_element<remove_cv_ref_t<Type>>::type;

        /////////////////////////////////////////
        template<typename T, T Value>
        struct indexer_type
        { 
            using type = T;
            static constexpr T Index = Value;
            static constexpr T Size = Value;
        };

        template<auto Index>
        using indexer_t = indexer_type< remove_cvref_t<decltype(Index)>, Index>;

        template<typename T, T RowValue, T ColumnValue>
        struct indexer_2d_type
        { 
            using type = T;
            static constexpr T Row = RowValue;
            static constexpr T RowSize = RowValue;
            static constexpr T Column = ColumnValue;
            static constexpr T ColumnSize = ColumnValue;
            
            // GNU g++, clang++, oneAPI DPC++ supports
            // but Microsoft MSVC 2019 fails
            // #if !defined(_MSVC_LANG)

                template<auto RowIndex, auto ColumnIndex> 
                constexpr auto operator()(indexer_t<RowIndex>, indexer_t<ColumnIndex>)
                {
                    return ColumnSize * RowIndex + ColumnIndex;  
                }

            // #endif 
        };

        template<auto RowValue, auto ColumnValue>
        using indexer_2d_t = indexer_2d_type< remove_cvref_t<decltype(RowValue)>, RowValue, ColumnValue>;

        template<typename T, T HeightValue, T RowValue, T ColumnValue>
        struct indexer_3d_type
        { 
            using type = T;
            static constexpr T Height = HeightValue;
            static constexpr T HeightSize = HeightValue;
            static constexpr T Row = RowValue;
            static constexpr T RowSize = RowValue;
            static constexpr T Column = ColumnValue;
            static constexpr T ColumnSize = ColumnValue;

            // GNU g++, clang++, oneAPI DPC++ supports
            // but Microsoft MSVC fails
            // #if !defined(_MSVC_LANG)
            template<auto HeightIndex, auto RowIndex, auto ColumnIndex> 
            constexpr auto operator()(indexer_t<HeightIndex>,
                 indexer_t<RowIndex>, indexer_t<ColumnIndex>)
            {
                return RowSize * ColumnSize * HeightIndex + ColumnSize * RowIndex + ColumnIndex;  
            }
            // #endif
        };

        template<auto HeightValue, auto RowValue, auto ColumnValue>
        using indexer_3d_t = indexer_3d_type< remove_cvref_t<decltype(RowValue)>, HeightValue, RowValue, ColumnValue>;

        template<auto... Indices>
        using typed_sequence_t = 
            std::integer_sequence< std::common_type_t<remove_cvref_t<decltype(Indices)>...>, Indices...>;

        template<auto Size>
        using make_typed_sequence_t =
            std::make_integer_sequence<remove_cvref_t<decltype(Size)>, Size>;

        inline bool is_stack_reversed()
        {
            auto func = [](auto a, auto b)
            {
                return &b > &a;
            };

            return func(1, 2);
        }

        inline bool is_stack_in_order()
        {
            return !is_stack_reversed();
        }

        inline constexpr bool StackInOrder = true;

        inline constexpr bool InOrderSequence = true;
        inline constexpr bool ReverseSequence = false;

        namespace hidden
        {
            // can return negative value
            template<auto Begin, auto End, auto Size>
            constexpr auto compute_span()
            {
                using type_t = decltype(Begin);

                if constexpr(Size == 0 || Size == 1)
                    return 0;
                else if constexpr(Size == 2)
                    return End - Begin;
                else 
                {
                    return (End - Begin) / (type_t) (Size - 1);
                }
            }

            template<typename T> struct st_sequence_first_element
            {
                static constexpr T value = std::numeric_limits<T>::max();
            };

            template<typename T, T First, T... Remainders>
            struct st_sequence_first_element<std::integer_sequence<T, First, Remainders...>>
            {
                static constexpr T value = First;
            };

            template<typename T>
            constexpr auto sequence_first_element_v = st_sequence_first_element<T>::value;

            ////////////////////////////////////////
            template<typename T> struct st_sequence_last_element;

            template<typename T, T First>
            struct st_sequence_last_element<std::integer_sequence<T, First>>
            {
                static constexpr T value = First;
            };

            template<typename T, T First, T... Remainders>
            struct st_sequence_last_element<std::integer_sequence<T, First, Remainders...>>
            {
                static constexpr T value = st_sequence_last_element<std::integer_sequence<T, Remainders...>>::value;
            };

            template<typename T>
            constexpr auto sequence_last_element_v = st_sequence_last_element<T>::value;

            ///////////////////////////////////////////////////////
            template<auto Nth, typename SequenceType> struct st_sequence_nth_element;

            template<typename T, T First, T...Remainders>
            struct st_sequence_nth_element<0, std::integer_sequence<T, First, Remainders...>>
            {
                static constexpr T value = First;
            };

            template<auto Nth, typename T, T First, T...Remainders>
            struct st_sequence_nth_element<Nth, std::integer_sequence<T, First, Remainders...>>
            {
                static constexpr T value = st_sequence_nth_element<Nth-1, std::integer_sequence<T, Remainders...>>::value;
            };
            
            template<auto Nth, typename SequenceType>
            constexpr auto sequence_nth_element_v = st_sequence_nth_element<Nth, SequenceType>::value;

            ///////////////////////////////////////////////
            template<typename T> struct st_sequence_element_count
            {
                static constexpr size_t value = std::numeric_limits<size_t>::max();
            };

            template<typename T, T... Elements>
            struct st_sequence_element_count<std::integer_sequence<T, Elements...>>
            {
                static constexpr size_t value = sizeof...(Elements);
            };

            template<typename T>
            constexpr auto sequence_element_count_v = st_sequence_element_count<T>::value;

            template<typename T, typename S> struct st_revese_sequence_operation;

            template<typename T, T... args>
            struct st_revese_sequence_operation<std::integer_sequence<T, args...>, std::integer_sequence<T>>
            {
                using type = std::integer_sequence<T, args...>;
            };

            template<typename T, T... args1, T first, T... args2>
            struct st_revese_sequence_operation<
                std::integer_sequence<T, args1...>,
                std::integer_sequence<T, first, args2...>>
            {
                using left = std::integer_sequence<T, first, args1...>;
                using right = std::integer_sequence<T, args2...>;

                using type = typename st_revese_sequence_operation<left, right>::type;
            };

            template<typename T> struct st_reverse_sequence;

            template<typename T, T... args>
            struct st_reverse_sequence< std::integer_sequence<T, args...>>
            {
                using type = typename st_revese_sequence_operation<
                    std::integer_sequence<T>, std::integer_sequence<T, args...> >::type;
            };

            template<typename SequenceType>
            using reverse_sequence_t = typename st_reverse_sequence<SequenceType>::type;

            template<typename T>
            struct st_sequence_element_type
            {
                using type = no_type_t;
            };

            template<typename T, T... Elements>
            struct st_sequence_element_type<std::integer_sequence<T, Elements...>>
            {
                using type = T;
            };

            template<typename T>
            using sequence_element_t = typename st_sequence_element_type<T>::type;

            template<typename T> struct st_sequence_span;

            template<typename T, T First>
            struct st_sequence_span<std::integer_sequence<T, First>>
            {
                static constexpr T value = 0;
            };

            template<typename T, T First, T Second, T... Remainders>
            struct st_sequence_span<std::integer_sequence<T, First, Second, Remainders...>>
            {
                static constexpr T value = Second - First;
            };

            template<typename SequenceType>
            constexpr auto sequence_span_v = st_sequence_span<SequenceType>::value;

            template<typename T, auto FirstValue, auto LastValue, size_t CountValue,
                bool stack_order = StackInOrder>
            struct st_sequence_info
            {
                using type = T;
                static constexpr T Begin = FirstValue;
                static constexpr T Last = LastValue;
                static constexpr T End = LastValue + 1;
                static constexpr T Span = compute_span<FirstValue, LastValue, (T)CountValue>();
                static constexpr size_t Count = CountValue;
                static constexpr bool InOrder = StackInOrder; 
            };

            template<typename T, auto FirstValue, auto LastValue, size_t CountValue>
            struct st_sequence_info<T, FirstValue, LastValue, CountValue, !StackInOrder>
            {
                using type = T;
                static constexpr T Begin = LastValue;
                static constexpr T Last = FirstValue;
                static constexpr T End = FirstValue - 1;
                static constexpr T Span = compute_span<LastValue, FirstValue, (T)CountValue>();
                static constexpr size_t Count = CountValue;
                static constexpr bool InOrder = !StackInOrder;
            };

            template<bool StackOrder, typename SequenceType>
            using sequence_info_t = st_sequence_info< sequence_element_t<SequenceType>,
                sequence_first_element_v<SequenceType>, sequence_last_element_v<SequenceType>,
                sequence_element_count_v<SequenceType>, StackOrder>; 

        } // end of namespace hidden

        template<typename SequenceType>
            using reverse_sequence_t = hidden::reverse_sequence_t<remove_cvref_t<SequenceType>>;

        template<typename SequenceType>
            constexpr auto sequence_first_element_v = hidden::sequence_first_element_v<remove_cvref_t<SequenceType>>;

        template<typename SequenceType>
            constexpr auto sequence_last_element_v = hidden::sequence_last_element_v<remove_cvref_t<SequenceType>>;

        template<auto Nth, typename SequenceType>
            constexpr auto sequence_nth_element_v = hidden::sequence_nth_element_v<Nth, remove_cvref_t<SequenceType>>;
        
        template<typename SequenceType>
            constexpr auto sequence_element_count_v = hidden::sequence_element_count_v<remove_cvref_t<SequenceType>>;

        template<typename SequenceType>
            using sequence_element_t = hidden::sequence_element_t<remove_cvref_t<SequenceType>>;

        template<typename SequenceType>
            constexpr auto sequence_span_v = hidden::sequence_span_v<remove_cvref_t<SequenceType>>;

        template<bool StackOrder, typename SequenceType>
        using sequence_info_t = typename hidden::sequence_info_t<StackOrder, remove_cvref_t<SequenceType>>;
        
        namespace hidden
        {
            template<typename T, T BeginValue, T EndValue, T IncrementValue>
            struct st_workhorse_range
            {
                using type = T;
                static constexpr type Begin = BeginValue;
                static constexpr type End = EndValue;
                static constexpr type Increment = IncrementValue;
            };

            template<typename T, T... Indices, T Begin, T End, T Increment>
            constexpr auto fn_create_sequence_in_order(std::integer_sequence<T, Indices...>,
                st_workhorse_range<T, Begin, End, Increment>)
            {
                if constexpr (Increment > 0)
                {
                    if constexpr (Begin + Increment < End)
                    {
                        using sequence_t = std::integer_sequence<T, Indices..., Begin>;
                        using range_t = st_workhorse_range<T, Begin + Increment, End, Increment>;

                        return fn_create_sequence_in_order(sequence_t{}, range_t{}); 
                    }
                    else
                    {
                        return std::integer_sequence<T, Indices..., Begin>{};
                    }
                }
                else
                {
                    if constexpr (Begin + Increment > End)
                    {
                        using sequence_t = std::integer_sequence<T, Indices..., Begin>;
                        using range_t = st_workhorse_range<T, Begin + Increment, End, Increment>;

                        return fn_create_sequence_in_order(sequence_t{}, range_t{}); 
                    }
                    else
                    {
                        return std::integer_sequence<T, Indices..., Begin>{};
                    }
                }
            }

            template<typename T, T... Indices, T Begin, T End, T Increment>
            constexpr auto fn_create_sequence_reverse_order(std::integer_sequence<T, Indices...>,
                st_workhorse_range<T, Begin, End, Increment>)
            {
                if constexpr(Increment > 0)
                {
                    if constexpr (Begin + Increment < End)
                    {
                        using sequence_t = std::integer_sequence<T, Begin, Indices...>;
                        using range_t = st_workhorse_range<T, Begin + Increment, End, Increment>;

                        return fn_create_sequence_reverse_order(sequence_t{}, range_t{}); 
                    }
                    else
                    {
                        return std::integer_sequence<T, Begin, Indices...>{};
                    }
                }
                else
                {
                    if constexpr (Begin + Increment > End)
                    {
                        using sequence_t = std::integer_sequence<T, Begin, Indices...>;
                        using range_t = st_workhorse_range<T, Begin + Increment, End, Increment>;

                        return fn_create_sequence_reverse_order(sequence_t{}, range_t{}); 
                    }
                    else
                    {
                        return std::integer_sequence<T, Begin, Indices...>{};
                    }
                }
            }

            template<bool SequenceInOrder, auto... Values>
            struct st_create_workhorse_range;

            template<bool SequenceInOrder, auto EndValue>
            struct st_create_workhorse_range<SequenceInOrder, EndValue>
            {
                using value_type = std::make_signed_t<remove_cvref_t<decltype(EndValue)>>;
                using workhorse_range_type = st_workhorse_range<value_type, (value_type)0, (value_type)EndValue, (value_type)1>;
                using type =
                    std::conditional_t<SequenceInOrder,
                        remove_cvref_t<decltype(fn_create_sequence_in_order(std::integer_sequence<value_type>{}, workhorse_range_type{}))>,
                        remove_cvref_t<decltype(fn_create_sequence_reverse_order(std::integer_sequence<value_type>{}, workhorse_range_type{}))>>; 
            };

            template<bool SequenceInOrder, auto BeginValue, auto EndValue>
            struct st_create_workhorse_range<SequenceInOrder, BeginValue, EndValue>
            {
                using value_type = std::make_signed_t<remove_cvref_t<decltype(EndValue)>>;
                using workhorse_range_type = st_workhorse_range<value_type, (value_type)BeginValue, (value_type)EndValue, (value_type)1>;
                using type =
                    std::conditional_t<SequenceInOrder,
                        remove_cvref_t<decltype(fn_create_sequence_in_order(std::integer_sequence<value_type>{}, workhorse_range_type{}))>,
                        remove_cvref_t<decltype(fn_create_sequence_reverse_order(std::integer_sequence<value_type>{}, workhorse_range_type{}))>>; 
            };

            template<bool SequenceInOrder, auto BeginValue, auto EndValue, auto IncrementValue>
            struct st_create_workhorse_range<SequenceInOrder, BeginValue, EndValue, IncrementValue>
            {
                using value_type = std::make_signed_t<remove_cvref_t<decltype(EndValue)>>;
                using workhorse_range_type = st_workhorse_range<value_type,
                    (value_type)BeginValue, (value_type)EndValue, (value_type)IncrementValue>;

                using type =
                    std::conditional_t<SequenceInOrder,
                        remove_cvref_t<decltype(fn_create_sequence_in_order(std::integer_sequence<value_type>{}, workhorse_range_type{}))>,
                        remove_cvref_t<decltype(fn_create_sequence_reverse_order(std::integer_sequence<value_type>{}, workhorse_range_type{}))>>; 
            };

        } // end of namespace hidden

        template<bool SequenceInOrder, auto... RangeValues>
        using make_sequence_t = typename hidden::st_create_workhorse_range<SequenceInOrder, RangeValues...>::type; 

        inline bool is_parameter_stack_order_reversed()
        {
            auto func = [](auto a, auto b)
            {
                return &b > &a;
            };

            return func(1, 2);
        }

        // create std::array 2d accessor
        template<auto RowValue, auto ColumnValue, typename ArrayType, 
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t< is_std_array_v<ArrayType> && Count == std_array_size_v<ArrayType>>>
        decltype(auto) array_indexer(ArrayType&& array)
        {
            return [&array, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto indexer)
            {
                return rank.Column * indexer.Row + indexer.Column;
            };
        }

        template<auto RowValue, auto ColumnValue, typename ArrayType, 
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t< is_std_array_v<ArrayType> && Count == std_array_size_v<ArrayType>>>
        decltype(auto) array_indexer_row_column(ArrayType&& array)
        {
            return [&array, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto row_index, auto column_index)
            {
                return rank.Column * row_index + column_index;
            };
        }

        // create std::array 2d accessor
        template<auto RowValue, auto ColumnValue, typename ArrayType, 
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t< is_std_array_v<ArrayType> && Count == std_array_size_v<ArrayType>>>
        decltype(auto) array_writer(ArrayType&& array)
        {
            return [&array, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto indexer, auto&& value)
            {
                array[rank.Column * indexer.Row + indexer.Column] = value;
                return value;
            };
        }

        // create std::array 2d accessor
        template<auto RowValue, auto ColumnValue, typename ArrayType, 
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t< is_std_array_v<ArrayType> && Count == std_array_size_v<ArrayType>>>
        decltype(auto) array_writer_row_column(ArrayType&& array)
        {
            return [&array, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto row_index,
                auto column_index, auto&& value)
            {
                array[rank.Column * row_index + column_index] = value; return value;
            };
        }

        // create std::array 2d accessor
        template<auto RowValue, auto ColumnValue, typename ArrayType, 
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t< is_std_array_v<ArrayType> && Count == std_array_size_v<ArrayType>>>
        decltype(auto) array_reader(ArrayType&& array)
        {
            return [&array, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto indexer)
            {
                return array[rank.Column * indexer.Row + indexer.Column];
            };
        }

        template<auto RowValue, auto ColumnValue, typename ArrayType, 
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t< is_std_array_v<ArrayType> && Count == std_array_size_v<ArrayType>>>
        decltype(auto) array_reader_row_column(ArrayType&& array)
        {
            return [&array, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto row_index, auto column_index)
            {
                return array[rank.Column * row_index + column_index];
            };
        }

        template<auto RowValue, auto ColumnValue, typename TupleType,
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t<is_tuple_v<TupleType> && Count == tuple_size_v<TupleType>>>
        decltype(auto) tuple_reader(TupleType&& tuple)
        {
            return [&tuple, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto indexer)
            {
                constexpr auto Index = rank.Column * indexer.Row + indexer.Column;
                return std::get<Index>(tuple);
            };
        }

        // create std::tuple 2d accessor
        template<auto RowValue, auto ColumnValue, typename TupleType,
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t<is_tuple_v<TupleType> && Count == tuple_size_v<TupleType>>>
        decltype(auto) tuple_reader_row_column(TupleType&& tuple)
        {
            return [&tuple, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto row_indexer, auto column_indexer)
            {
                constexpr auto Index = rank.Column * row_indexer.Index + column_indexer.Index;
                return std::get<Index>(tuple);
            };
        }

        /////////////////////////////////////////
        template<auto RowValue, auto ColumnValue, typename TupleType,
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t<is_tuple_v<TupleType> && Count == tuple_size_v<TupleType>>>
        decltype(auto) tuple_writer(TupleType&& tuple)
        {
            return [&tuple, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto indexer, auto&& value)
            {
                constexpr auto Index = rank.Column * indexer.Row + indexer.Column;
                std::get<Index>(tuple) = value;
            };
        }

        // create std::tuple 2d accessor
        template<auto RowValue, auto ColumnValue, typename TupleType,
            auto Count = RowValue * ColumnValue,
            typename = std::enable_if_t<is_tuple_v<TupleType> && Count == tuple_size_v<TupleType>>>
        decltype(auto) tuple_writer_row_column(TupleType&& tuple)
        {
            return [&tuple, rank = indexer_2d_t<RowValue, ColumnValue>{}](auto row_indexer,
                auto column_indexer, auto&& value)
            {
                constexpr auto Index = rank.Column * row_indexer.Index + column_indexer.Index;
                std::get<Index>(tuple) = value;
            };
        }

        namespace hidden
        {
            template<typename... ArgTypes>
            void process_arguments(ArgTypes&&...) { }

            // WARNING: drive_workhorse is for internal USE only
            // It works only when the sequence increments by ONE - 1,
            // otherwise, it does not work.
            template<typename WorkhorseType, auto... Indices>
            void drive_workhorse(WorkhorseType&& workhorse, typed_sequence_t<Indices...>)
            {
                auto cabin =[&workhorse](auto indexer)
                {
                    workhorse(indexer); return 0;
                };

                if(is_parameter_stack_order_reversed())
                {
                    constexpr auto Size = sizeof...(Indices) - 1;

                    process_arguments( cabin( indexer_t<Size - Indices>{} )...  );
                }
                else
                {
                    process_arguments( cabin( indexer_t<Indices>{} )...  );
                }
            }

            // WARNING: drive_workhorse is for internal USE only
            // It works only when the sequence increments by ONE - 1,
            // or the sequence is continuous.
            // otherwise, it does not work.
            // For 2D sequence with two separate indexer_t
            template<typename WorkhorseType, auto... RowIndices, auto... ColumnIndices>
            std::enable_if_t<std::is_invocable_v<WorkhorseType, indexer_t<0>, indexer_t<0>>>
            drive_workhorse(WorkhorseType&& cabin,
                typed_sequence_t<RowIndices...>, typed_sequence_t<ColumnIndices...>)
            {
                constexpr auto ColumnSize = sizeof...(ColumnIndices);
                constexpr auto RowSize = sizeof...(RowIndices);

                auto process_column = [&cabin, col_size = indexer_t<ColumnSize>{}](auto row_indexer)
                {
                    auto row_fixed = [&cabin, row_indexer](auto col)
                    {
                        cabin(row_indexer, col); return 0;
                    };

                    drive_workhorse(row_fixed, make_typed_sequence_t<col_size.Index>{});
                };

                drive_workhorse(process_column, make_typed_sequence_t<RowSize>{});
            }

            // WARNING: drive_workhorse is for internal USE only
            // It works only when the sequence increments by ONE - 1,
            // or the sequence is continuous.
            // otherwise, it does not work.
            // For 2D sequence with ONE indexer_2d_t
            template<typename WorkhorseType, auto... RowIndices, auto... ColumnIndices>
            std::enable_if_t<std::is_invocable_v<WorkhorseType, indexer_2d_t<0, 0>>>
            drive_workhorse(WorkhorseType&& cabin,
                typed_sequence_t<RowIndices...>, typed_sequence_t<ColumnIndices...>)
            {
                auto cabin_mount =[&cabin](auto indexer_row, auto index_column)
                {
                    cabin(indexer_2d_t<indexer_row.Index, index_column.Index>{}); return 0;
                };

                constexpr auto ColumnSize = sizeof...(ColumnIndices);
                constexpr auto RowSize = sizeof...(RowIndices);

                drive_workhorse(cabin_mount, 
                    make_typed_sequence_t<RowSize>{}, make_typed_sequence_t<ColumnSize>{});
            }

            // WARNING: drive_workhorse is for internal USE only
            // It works only when the sequence increments by ONE - 1,
            // or the sequence is continuous.
            // otherwise, it does not work.
            // For 3D sequence with THREE indexer_t's as its arguments
            template<typename WorkhorseType,
                auto... HeightIndices, auto... RowIndices, auto... ColumnIndices>
            std::enable_if_t<std::is_invocable_v<WorkhorseType, indexer_t<0>, indexer_t<0>, indexer_t<0>>>
            drive_workhorse(WorkhorseType&& cabin, typed_sequence_t<HeightIndices...>,
                typed_sequence_t<RowIndices...>, typed_sequence_t<ColumnIndices...>)
            {
                constexpr auto HeightSize = sizeof...(HeightIndices);
                constexpr auto RowSize = sizeof...(RowIndices);
                constexpr auto ColumnSize = sizeof...(ColumnIndices);
                
                auto process_height = [&cabin, row_size = indexer_t<RowSize>{},
                    col_size = indexer_t<ColumnSize>{}](auto height_indexer)
                {
                    auto height_fixed = [&cabin, height_indexer](auto row_indexer, auto col_indexer)
                    {
                        cabin(height_indexer, row_indexer, col_indexer); return 0;
                    };

                    drive_workhorse(height_fixed, 
                    make_typed_sequence_t<row_size.Index>{},
                    make_typed_sequence_t<col_size.Index>{});
                };

                drive_workhorse(process_height, make_typed_sequence_t<HeightSize>{});
            }

            // WARNING: drive_workhorse is for internal USE only
            // It works only when the sequence increments by ONE - 1,
            // or the sequence is continuous.
            // otherwise, it does not work.
            // For 3D sequence with ONE indexer_3d_t's as its argument.
            template<typename WorkhorseType,
                auto... HeightIndices, auto... RowIndices, auto... ColumnIndices>
            std::enable_if_t<std::is_invocable_v<WorkhorseType, indexer_3d_t<0, 0, 0>>>
            drive_workhorse(WorkhorseType&& cabin, typed_sequence_t<HeightIndices...>,
                typed_sequence_t<RowIndices...>, typed_sequence_t<ColumnIndices...>)
            {
                auto cabin_mount =[&cabin](auto height_indexer,
                    auto row_indexer, auto column_indexer)
                {
                    cabin(indexer_3d_t<height_indexer.Index, row_indexer.Index, column_indexer.Index>{}); return 0;
                };

                constexpr auto HeightSize = sizeof...(HeightIndices);
                constexpr auto RowSize = sizeof...(RowIndices);
                constexpr auto ColumnSize = sizeof...(ColumnIndices);
                
                drive_workhorse(cabin_mount, make_typed_sequence_t<HeightSize>{},
                make_typed_sequence_t<RowSize>{}, make_typed_sequence_t<ColumnSize>{} );
            }

        } // end of namespace hidden

        //////////////////////////////////////////////////////////////////////////////
        // WARNING: use drive_workhorse with care
        // It works only when the sequence increments by ONE - 1,
        // or the sequence is continuous.
        // otherwise, it does not work.
        template<size_t Count, typename WorkhorseType>
        std::enable_if_t<std::is_invocable_v<WorkhorseType, indexer_t<0>>>
        drive_workhorse(WorkhorseType&& workhorse)
        {
            // we do not need to wrap workhorse with cabin
            hidden::drive_workhorse(workhorse, make_typed_sequence_t<Count>{});
        }

        // WARNING: use drive_workhorse with care
        // It works only when the sequence increments by ONE - 1,
        // or the sequence is continuous.
        // otherwise, it does not work.
        //
        // For 2D loop -  Consider using for_workhorse() instead
        template<auto RowCount, auto ColumnCount, typename WorkhorseType>
        std::enable_if_t<
            std::is_invocable_v<WorkhorseType, indexer_t<0>, indexer_t<0>>
            || std::is_invocable_v<WorkhorseType, indexer_2d_t<0, 0> > >
        drive_workhorse(WorkhorseType&& workhorse)
        {
            hidden::drive_workhorse(workhorse, 
                make_typed_sequence_t<RowCount>{},
                make_typed_sequence_t<ColumnCount>{});
        }
        
        // WARNING: use drive_workhorse with care
        // It works only when the sequence increments by ONE - 1,
        // or the sequence is continuous.
        // otherwise, it does not work. Consider using for_workhorse() instead
        //
        // For 3D loop - Consider using for_workhorse() instead
        template<auto HeightCount, auto RowCount, auto ColumnCount, typename WorkhorseType>
        std::enable_if_t<std::is_invocable_v<WorkhorseType, indexer_3d_t<0, 0, 0>> 
        || std::is_invocable_v<WorkhorseType, indexer_t<0>, indexer_t<0>, indexer_t<0> > >
        drive_workhorse(WorkhorseType&& workhorse)
        {
            hidden::drive_workhorse(workhorse, make_typed_sequence_t<HeightCount>{},
                make_typed_sequence_t<RowCount>{}, make_typed_sequence_t<ColumnCount>{});
        }

        namespace hidden
        {
            struct DummyWorkHorse
            {
                double operator()(indexer_t<1>){ return 1.0; };
            };

            // template<typename... ArgTypes>
            // void process_arguments(ArgTypes&&...) { }

            template<typename WorkhorseType, auto... Indices>
            std::enable_if_t<std::is_invocable_v<remove_cvref_t<WorkhorseType>, indexer_t<0>>>
            run_workhorse(WorkhorseType&& workhorse, typed_sequence_t<Indices...>)
            {
                auto cabin =[&workhorse](auto indexer)
                {
                    workhorse(indexer); return 0;
                };

                process_arguments( cabin( indexer_t<Indices>{} )... );
            }

            template<typename WorkhorseType, auto... Indices,
                typename dummy_sequence = sequence_info_t<StackInOrder, typed_sequence_t<1>>>
            std::enable_if_t<std::is_invocable_v<remove_cvref_t<WorkhorseType>, indexer_t<0>, dummy_sequence>>
            run_workhorse(WorkhorseType&& workhorse, typed_sequence_t<Indices...>)
            {
                auto cabin =[&workhorse](auto indexer, auto sequence_info)
                {
                    workhorse(indexer, sequence_info); return 0;
                };

                if(is_stack_in_order())
                {
                    using sequence_type = sequence_info_t<StackInOrder, typed_sequence_t<Indices...>>;
                    process_arguments( cabin( indexer_t<Indices>{}, sequence_type{} )...  );
                }
                else
                {
                    using sequence_type = sequence_info_t<!StackInOrder, typed_sequence_t<Indices...>>;
                    process_arguments( cabin( indexer_t<Indices>{}, sequence_type{} )...  );
                }
            }
        }

        template<auto... RangeValues, typename WorkhorseType = hidden::DummyWorkHorse,
            typename dummy_sequence = make_sequence_t<StackInOrder, 1>,
            typename sequence_info = sequence_info_t<true, dummy_sequence>>
        std::enable_if_t<std::is_invocable_v<remove_cvref_t<WorkhorseType>, indexer_t<0>>||
            std::is_invocable_v<remove_cvref_t<WorkhorseType>, indexer_t<0>, sequence_info>>
        for_workhorse(WorkhorseType&& workhorse)
        {
            if(is_stack_in_order())
                hidden::run_workhorse(workhorse, make_sequence_t<StackInOrder, RangeValues...>{});
            else
                hidden::run_workhorse(workhorse, make_sequence_t<!StackInOrder, RangeValues...>{});
        }
  
        namespace hidden
        {
            template<typename... Types>
            struct type_count_st
            {
                static constexpr size_t value = sizeof...(Types);
            };

            template<typename... Types>
            struct type_count_st<type_list_t<Types...>>
            {
                static constexpr size_t value = sizeof...(Types);
            };

            template<template<typename...> class TemplateType, typename... Types>
            struct type_count_st<TemplateType<Types...>>
            {
                static constexpr size_t value = sizeof...(Types);
            };

            template<typename... Types>
            constexpr auto type_count_v = type_count_st<Types...>::value;

            // if T is an iterator, this function is enabled
            // and returns the value type of the iterator type T
            // if T is not an iterator, then this function is SFINAEd out
            template<typename T> constexpr std::enable_if_t<true,
                typename std::iterator_traits<T>::value_type>
                iterator_value_type_fn(T&&);

            // this is catch all function for iterator_value_type_fn()
            constexpr no_type_t iterator_value_type_fn(...);

            // if T is an iterator iterator_value_type_fn returns value_type of the 
            // iterator, otherwise NoTypeDummy is returned.
            // if the returned value is Not equal to NoTypeDummy,
            // then T is an iterator, otherwise not an iterator
            template<typename T>
            constexpr bool is_iterator_type_v = 
                is_valid_type_v<decltype(iterator_value_type_fn(std::declval<T>()))>;

            template<typename T>
            constexpr bool is_iterator_excluding_pointer_v = 
                is_iterator_type_v<T> && !std::is_pointer_v<T>;

            // if T is an iterator, it returns T::value_type
            // otherwise, it returns NoTypeDummy
            template<typename T>
            using iterator_value_type_t = decltype(iterator_value_type_fn(std::declval<T>()));

            // if T is an iterator, this returns T
            // otherwise, it is SFINAEd out
            template<typename Type, typename ReturnType>
            using enable_if_iterator_type_t = std::enable_if_t<is_iterator_type_v<Type>, ReturnType>;

            // if T is an iterator, this returns T::value_type
            // otherwise, it is SFINAEd out
            template<typename T>
            using enable_if_iterator_value_type_t = std::enable_if_t<is_iterator_type_v<T>, iterator_value_type_t<T>>;

            // if T is an iterator, this returns T::value_type
            // otherwise, it returns T
            template<typename T>
            using conditional_iterator_value_type_t = 
                typename std::conditional<is_iterator_type_v<T>, iterator_value_type_t<T>, T>::type;

        } // end of namespace hidden
               
        template<typename T>
        constexpr bool is_iterator_type_v = hidden::is_iterator_type_v<remove_cv_ref_t<T>>;

        template<typename T>
        constexpr bool is_iterator_excluding_pointer_v = 
            hidden::is_iterator_excluding_pointer_v<remove_cv_ref_t<T>>;

        template<typename Type, typename ReturnType>
            using enable_if_iterator_type_t = hidden::enable_if_iterator_type_t<remove_cv_ref_t<Type>, ReturnType>;

        template<typename... Types>
        constexpr auto type_count_v = hidden::type_count_v<Types...>;        
         
        template<typename... Types>
        type_list_t<Types...> type_to_string(Types&&... args)
        {
            return {};
        } 

        /**
         * @brief Type list of character types. 
         * 
         */
        using character_list_t = type_list_t<char, unsigned char, wchar_t>;
       
        /**
         * @brief Type list of integers EXCLUSING character type and boolean type. 
         * 
         */
        using integer_list_t = type_list_t<short, unsigned short,
                int, unsigned int, long, unsigned long,
                long long, unsigned long long>;

        /**
         * @brief Signed integers EXCLUDING char and boolean
         * 
         */
        using signed_integer_list_t = type_list_t<short, int, long, long long>;
        
        /**
         * @brief Unsigned integers EXCLUDING unsigned char and boolean
         * 
         */
        using unsigned_integer_list_t = type_list_t<unsigned short, unsigned int, unsigned long, unsigned long long>;
        
        /**
         * @brief Type list of integral type INCLUDING character type, but EXCLUDING boolean type. 
         * 
         */
        using integral_list_t = type_list_t<char, unsigned char, short, unsigned short,
                int, unsigned int, long, unsigned long,
                long long, unsigned long long>;

        /**
         * @brief Type list of unsigned integral type INCLUDING character type, but EXCLUDING boolean type. 
         * 
         */
        using unsigned_integral_list_t = type_list_t<unsigned char, unsigned short,
                unsigned int, unsigned long, unsigned long long>;

         /**
         * @brief Type list of signed integral type INCLUDING character type, but EXCLUDING boolean type. 
         * 
         */
        using signed_integral_list_t = type_list_t<char, short,
                int, long int, long long int>;

        /**
         * @brief Type list of real numbers (floating-point numbers).
         * 
         */
        using real_number_list_t = type_list_t<float, double, long double>;

        /**
         * @brief All numbers mathematical operations.
         * 
         */
        using numerical_number_list_t = type_list_t<char, unsigned char, short, unsigned short,
            int, unsigned int, long, unsigned long, long long, unsigned long long, float, double, long double>;

         /**
         * @brief Arithmetic numbers for mathematical analysis.
         * 
         */
        using arithmetic_number_list_t = type_list_t<char, short, int, long, long long, float, double, long double>;

        /**
         * @brief The same to real_number_list_t, real numbers.
         * 
         */
        using floating_point_list_t = real_number_list_t;

        /**
         * @brief Implementations in this scope is meant NOT to be accessed directly by the client.
         * 
         */
        namespace hidden
        {
            template<typename Type>
            struct is_template_st
            {
                using type = type_list_t<Type>;
                static constexpr bool value = false;
                static constexpr size_t count = 1;
            };

            // zero or more template parameters
            template<template<typename...>class ContainerType, typename...Types>
            struct is_template_st<ContainerType<Types...>>
            {
                using type = type_list_t<Types...>;
                static constexpr bool value = true;
                static constexpr size_t count = sizeof...(Types);
            };

            template<template<typename, auto...>class ContainerType, typename Type, auto... Args>
            struct is_template_st<ContainerType<Type, Args...>>
            {
                using type = type_list_t<Type>;
                static constexpr bool value = true;
                static constexpr size_t count = 1;
            };

            template<typename Type>
            constexpr auto is_template_v = is_template_st<Type>::value;

            template<typename Type>
            constexpr auto template_parameter_count = is_template_st<Type>::count;

            template<typename Type>
            using template_parameter_type_list_t = typename is_template_st<Type>::type;

            template<typename Type1, typename Type2>
            struct is_same_template_type_st
            {
                static constexpr bool value = false;
            };

            template<template<typename...> class ContainerType, typename... Types>
            struct is_same_template_type_st<ContainerType<Types...>, ContainerType<Types...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            struct is_same_template_type_st<ContainerType<Type, Types...>, ContainerType<Type, Types...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename, typename, typename...> class ContainerType,
                typename Type1, typename Type2, typename... Types>
            struct is_same_template_type_st<ContainerType<Type1, Type2, Types...>,
                ContainerType<Type1, Type2, Types...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename, typename, typename, typename...> class ContainerType,
                typename Type1, typename Type2, typename Type3, typename... Types>
            struct is_same_template_type_st<ContainerType<Type1, Type2, Type3, Types...>,
                ContainerType<Type1, Type2, Type3, Types...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename, typename, typename, typename...> class ContainerType,
                typename Type1, typename Type2, typename Type3, typename Type4, typename... Types>
            struct is_same_template_type_st<ContainerType<Type1, Type2, Type3, Type4, Types...>,
                ContainerType<Type1, Type2, Type3, Type4, Types...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type1, typename Type2>
            constexpr auto is_same_template_type_v = is_same_template_type_st<Type1, Type2>::value;

            template<typename Type1, typename Type2>
            struct is_same_template_st
            {
                static constexpr bool value = false;
            };

            template<template<typename...> class ContainerType,
                typename... Types1, typename... Types2>
            struct is_same_template_st<ContainerType<Types1...>, ContainerType<Types2...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename, typename...> class ContainerType,
                typename TypeA, typename... TypeAs,
                typename TypeB, typename... TypeBs>
            struct is_same_template_st<ContainerType<TypeA, TypeAs...>,
                ContainerType<TypeB, TypeBs...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename, typename, typename...> class ContainerType,
                typename TypeA1, typename TypeA2, typename... TypeAs,
                typename TypeB1, typename TypeB2, typename... TypeBs>
            struct is_same_template_st<ContainerType<TypeA1, TypeA2, TypeAs...>,
                 ContainerType<TypeB1, TypeB2, TypeBs...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename, typename, typename, typename...> class ContainerType,
                typename TypeA1, typename TypeA2, typename TypeA3, typename... TypeAs,
                typename TypeB1, typename TypeB2, typename TypeB3, typename... TypeBs>
            struct is_same_template_st<ContainerType<TypeA1, TypeA2, TypeA3, TypeAs...>,
                 ContainerType<TypeB1, TypeB2, TypeB3, TypeBs...>>
            {
                static constexpr bool value = true;
            };

            template<template<typename, typename, typename, typename...> class ContainerType,
                typename TypeA1, typename TypeA2, typename TypeA3, typename TypeA4, typename... TypeAs,
                typename TypeB1, typename TypeB2, typename TypeB3, typename TypeB4, typename... TypeBs>
            struct is_same_template_st<ContainerType<TypeA1, TypeA2, TypeA3, TypeA4, TypeAs...>,
                 ContainerType<TypeB1, TypeB2, TypeB3, TypeB4, TypeBs...>>
            {
                static constexpr bool value = true;
            };

            template<typename Type1, typename Type2>
            constexpr auto is_same_template_v = is_same_template_st<Type1, Type2>::value;
            
        } // end of namespace hidden

        /**
         * @brief Test whether the type parameter is a template type.
         * 
         * @tparam Type for test whether the type is a template type.
         * 
         */
        template<typename Type>
        constexpr auto is_template_v = hidden::is_template_v<Type>;

        template<typename Type>
        constexpr auto template_parameter_count = hidden::template_parameter_count<Type>;
       
        template<typename Type>
        using template_parameter_type_list_t = hidden::template_parameter_type_list_t<Type>;

        template<typename Type1, typename Type2>
        constexpr auto is_same_template_type_v = hidden::is_same_template_type_v<Type1, Type2>;

        template<typename Type1, typename Type2>
        constexpr auto is_same_template_v = hidden::is_same_template_v<Type1, Type2>;
        
        /**
         * @brief Implementations in this scope is meant NOT to be accessed directly by the client.
         * 
         */
        namespace hidden
        {
            template<typename Type>
            struct is_string_st
            {
                static constexpr bool value = false;
            };

            template<>
            struct is_string_st<const char*>
            {
                static constexpr bool value = true;
            };

            template<>
            struct is_string_st<const wchar_t*>
            {
                static constexpr bool value = true;
            };

            template<typename CharType>
            struct is_string_st<std::basic_string<CharType>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_string_v = is_string_st<remove_cv_ref_t<Type>>::value;          

        } // end of namespace hidden

        /**
         * @brief Test is Type is of string type.
         * 
         * If Type is one of const char*, const wchar_t*, std::string, std::wstring, 
         * Type is_string_v<Type> returns true, otherwise false.
         * 
         * @tparam Type to test if it is of string category.
         */
        template<typename Type>
        constexpr auto is_string_v = hidden::is_string_v<Type>;

        /**
         * @brief Implementation ins this scope is NOT meant to be directly accessed by the client.
         * 
         */
        namespace hidden
        { 
            template<typename Type1, typename Type2>
            constexpr auto is_operable_fn(Type1&& arg1, Type2&& arg2) 
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type1>>()) 
                    && noexcept(std::declval<remove_cv_ref_t<Type2>>())) -> decltype(true ? arg1 : arg2);

            constexpr no_type_t is_operable_fn(...) noexcept;
         
            template<typename Type1, typename Type2>
            using is_operable_t = std::remove_reference_t<decltype(is_operable_fn(std::declval<Type1>(), std::declval<Type2>()))>;

            template<typename Type1, typename Type2>
            constexpr auto is_operable_v = 
                is_valid_type_v<is_operable_t<Type1, Type2>>;

            template<typename Type>
            constexpr auto is_empty_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.empty());

            constexpr no_type_t is_empty_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_empty_available_v = 
                is_valid_type_v<decltype(is_empty_available_fn(std::declval<Type>()))>;

            template<typename Type>
            constexpr auto is_capacity_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.capacity());

            constexpr no_type_t is_capacity_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_capacity_available_v = 
                is_valid_type_v<decltype(is_capacity_available_fn(std::declval<Type>()))>;

            template<typename Type>
            constexpr auto is_shrink_to_fit_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.shrink_to_fit());

            constexpr no_type_t is_shrink_to_fit_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_shrink_to_fit_available_v = 
                is_valid_type_v<decltype(is_shrink_to_fit_available_fn(std::declval<Type>()))>;

            template<typename Type>
            constexpr auto is_size_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.size());

            constexpr no_type_t is_size_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_size_available_v = 
                is_valid_type_v<decltype(is_size_available_fn(std::declval<Type>()))>;

            template<typename Type>
            constexpr auto is_front_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.front());

            constexpr no_type_t is_front_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_front_available_v = 
                is_valid_type_v<decltype(is_front_available_fn(std::declval<Type>()))>;

            template<typename Type>
            constexpr auto is_back_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.back());

            constexpr no_type_t is_back_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_back_available_v = is_valid_type_v<decltype(is_back_available_fn(std::declval<Type>()))>;

            ///////////////////////////////////////////
            template<typename Type>
            constexpr auto is_begin_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.begin());

            constexpr no_type_t is_begin_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_begin_available_v = 
                is_valid_type_v<decltype(is_begin_available_fn(std::declval<Type>()))>;

            ///////////////////////////////////////////
            template<typename Type>
            constexpr auto is_end_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.end());

            constexpr no_type_t is_end_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_end_available_v = 
                is_valid_type_v<decltype(is_end_available_fn(std::declval<Type>()))>;

            ///////////////////////////////////////////
            template<typename Type>
            constexpr auto is_rbegin_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.rbegin());

            constexpr no_type_t is_rbegin_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_rbegin_available_v = 
                is_valid_type_v<decltype(is_rbegin_available_fn(std::declval<Type>()))>;

            ///////////////////////////////////////////
            template<typename Type>
            constexpr auto is_rend_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.rend());

            constexpr no_type_t is_rend_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_rend_available_v = 
                is_valid_type_v<decltype(is_rend_available_fn(std::declval<Type>()))>;
            
            ///////////////////////
            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_push_front_valid_fn(ContainerType<Type, Types...>&& arg)
                noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.push_front(std::declval<Type>()));

            constexpr no_type_t is_push_front_valid_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_push_front_available_v = 
                is_valid_type_v<decltype(is_push_front_valid_fn(std::declval<Type>()))>;

            ////////////////////////////////////////////////
            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_index_operator_valid_fn(ContainerType<Type, Types...>&& arg)
                noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.operator[]((size_t)1));

            constexpr no_type_t is_index_operator_valid_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_index_operator_available_v = 
                is_valid_type_v<decltype(is_index_operator_valid_fn(std::declval<Type>()))>;

            ////////////////////////////////////////////////
            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_push_back_valid_fn(ContainerType<Type, Types...>&& arg)
                noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.push_back(std::declval<Type>()));

            constexpr no_type_t is_push_back_valid_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_push_back_available_v = 
                is_valid_type_v<decltype(is_push_back_valid_fn(std::declval<Type>()))>;

            /////////////////////////////////////////

            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_erase_valid_fn(ContainerType<Type, Types...>&& arg)
                ->decltype(arg.erase(arg.cbegin()));

            constexpr no_type_t is_erase_valid_fn(...);

            template<typename Type>
            constexpr bool is_erase_available_v = 
                is_valid_type_v<decltype(is_erase_valid_fn(std::declval<Type>()))>;

            /////////////////////////////////////////

            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_reserve_valid_fn(ContainerType<Type, Types...>&& arg)
                ->decltype(arg.reserve((size_t)0));

            constexpr no_type_t is_reserve_valid_fn(...);

            template<typename Type>
            constexpr bool is_reserve_available_v = 
                is_valid_type_v<decltype(is_reserve_valid_fn(std::declval<Type>()))>;
            
            /////////////////////////////////////////            
            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_insert_valid_fn(ContainerType<Type, Types...>&& arg)
                noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.insert(std::declval<Type>()));

            constexpr no_type_t is_insert_valid_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_insert_available_v = 
                is_valid_type_v<decltype(is_insert_valid_fn(std::declval<tpf::remove_cv_ref_t<Type>>()))>;

            // /////////////////////////////////////////
            // template<template<typename, typename...> class ContainerType,
            //     typename Type, typename... Types>
            // constexpr auto is_index_operator_valid_fn(ContainerType<Type, Types...>&& arg)
            //     noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.operator[](std::declval<size_t>()));

            // constexpr no_type_t is_index_operator_valid_fn(...) noexcept;

            // template<typename Type>
            // constexpr bool is_index_operator_available_v = 
            //     is_valid_type_v<decltype(is_index_operator_valid_fn(std::declval<tpf::remove_cv_ref_t<Type>>()))>;

            //////////////////////////////////////////
            
            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_insert_iterator_valid_fn(ContainerType<Type, Types...>&& arg)
                noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.insert(arg.cend(), std::declval<Type>()));

            constexpr no_type_t is_insert_iterator_valid_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_insert_iterator_available_v = 
                is_valid_type_v<decltype(is_insert_iterator_valid_fn(std::declval<tpf::remove_cv_ref_t<Type>>()))>;

            ////////////////////////////////////////////////
            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_resize_valid_fn(ContainerType<Type, Types...>&& arg)
                noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.resize(std::declval<size_t>()));

            constexpr no_type_t is_resize_valid_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_resize_available_v = 
                is_valid_type_v<decltype(is_resize_valid_fn(std::declval<Type>()))>;

            ////////////////////////////////////////
            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_emplace_front_valid_fn(ContainerType<Type, Types...>&& arg)
                noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.emplace_front(std::declval<Type>()));

            constexpr no_type_t is_emplace_front_valid_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_emplace_front_available_v = 
                is_valid_type_v<decltype(is_emplace_front_valid_fn(std::declval<Type>()))>;

            ////////////////////////////////////////////////
            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_emplace_back_valid_fn(ContainerType<Type, Types...>&& arg)
                noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.emplace_back(std::declval<Type>()));

            constexpr no_type_t is_emplace_back_valid_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_emplace_back_available_v = 
                is_valid_type_v<decltype(is_emplace_back_valid_fn(std::declval<Type>()))>;

            ////////////////////////////////////////

            template<template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            constexpr auto is_emplace_valid_fn(ContainerType<Type, Types...>&& arg)
                noexcept(noexcept(std::declval<ContainerType<Type, Types...>>()))->decltype(arg.emplace(arg.cbegin(), std::declval<Type>()));

            constexpr no_type_t is_emplace_valid_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_emplace_available_v = 
                is_valid_type_v<decltype(is_emplace_valid_fn(std::declval<Type>()))>;

            ////////////////////////////////////////////

            template<typename Type>
            constexpr auto is_pop_front_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.pop_front());

            constexpr no_type_t is_pop_front_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_pop_front_available_v = 
                is_valid_type_v<decltype(is_pop_front_available_fn(std::declval<Type>()))>;
            
            template<typename Type>
            constexpr auto is_pop_back_available_fn(Type&& arg)
                noexcept(noexcept(std::declval<remove_cv_ref_t<Type>>()))->decltype(arg.pop_back());

            constexpr no_type_t is_pop_back_available_fn(...) noexcept;

            template<typename Type>
            constexpr bool is_pop_back_available_v = 
                is_valid_type_v<decltype(is_pop_back_available_fn(std::declval<Type>()))>;

            ///////////////////////////////////////////////////

            template<typename Type>
            constexpr bool is_container_type_v = 
                is_empty_available_v<Type> && is_size_available_v<Type> && 
                is_begin_available_v<Type>;
               
            template<typename Type, typename ReturnType = void>
            using enable_if_container_type_t = std::enable_if_t< is_container_type_v<remove_cv_ref_t<Type>>, ReturnType>;

            template<typename Type, typename... Types>
            struct is_same_st;

            template<typename Type>
            struct is_same_st<Type>
            {
                static constexpr bool value = true;
            };

            template<typename Type1, typename Type2>
            struct is_same_st<Type1, Type2>
            {
                static constexpr bool value = std::is_same_v<Type1, Type2>;
            };

            template<typename Type1, typename Type2, typename... Types>
            struct is_same_st<Type1, Type2, Types...>
            {
                static constexpr bool value = is_same_st<Type1, Type2>::value && 
                    is_same_st<Type2, Types...>::value;
            };

            template<typename Type>
            struct is_same_st<type_list_t<Type>>
            {
                static constexpr bool value = true;
            };

            template<typename Type1, typename Type2>
            struct is_same_st<type_list_t<Type1, Type2>>
            {
                static constexpr bool value = is_same_st<Type1, Type2>::value;
            };

            template<typename Type1, typename Type2, typename... Types>
            struct is_same_st<type_list_t<Type1, Type2, Types...>>
            {
                static constexpr bool value = 
                    is_same_st<Type1, Type2, Types...>::value;
            };

            template<typename Type, typename... Types>
            constexpr auto is_same_v = is_same_st<Type, Types...>::value;

            // test if TestType is in the list
            template<typename TestType, typename... Types>
            struct is_in_list_st
            {
                static constexpr bool value = false;
            };

            template<typename TestType>
            struct is_in_list_st<TestType>
            {
                static constexpr bool value = false;
            };

            template<typename TestType, typename Type>
            struct is_in_list_st<TestType, Type>
            {
                static constexpr bool value = std::is_same_v<TestType, Type>;
            };

            template<typename TestType, typename Type, typename... Types>
            struct is_in_list_st<TestType, Type, Types...>
            {
                static constexpr bool value = 
                    std::is_same_v<TestType, Type> 
                        || is_in_list_st<TestType, Types...>::value;
            };

            template<typename TestType>
            struct is_in_list_st<TestType, type_list_t<>>
            {
                static constexpr bool value = false;
            };

            template<typename TestType, typename Type>
            struct is_in_list_st<TestType, type_list_t<Type>>
            {
                static constexpr bool value = is_in_list_st<TestType, Type>::value;
            };

            template<typename TestType, typename Type, typename... Types>
            struct is_in_list_st<TestType, type_list_t<Type, Types...>>
            {
                static constexpr bool value = is_in_list_st<TestType, Type, Types...>::value;
            };

            template<typename TestType, typename... Types>
            constexpr bool is_in_list_v = is_in_list_st<TestType, Types...>::value;

            template<typename TypeList, typename... TestTypes>
            struct are_all_in_list_st;

            template<typename... Types>
            struct are_all_in_list_st<type_list_t<Types...>>
            {
                static constexpr bool value = false;
            };

            template<typename... Types, typename TestType>
            struct are_all_in_list_st<type_list_t<Types...>, TestType>
            {
                static constexpr bool value = is_in_list_v<TestType, type_list_t<Types...>>; 
            };

            template<typename... Types, typename TestType, typename... TestTypes>
            struct are_all_in_list_st<type_list_t<Types...>, TestType, TestTypes...>
            {
                static constexpr bool tmp_value = is_in_list_v<TestType, type_list_t<Types...>>; 

                static constexpr bool value = sizeof...(TestTypes) == 0 ? tmp_value :
                    tmp_value && are_all_in_list_st<type_list_t<Types...>, TestTypes...>::value;
            };

            template<typename... Types>
            struct are_all_in_list_st<type_list_t<Types...>, type_list_t<>>
            {
                static constexpr bool value = false;
            };

            template<typename... Types, typename TestType>
            struct are_all_in_list_st<type_list_t<Types...>, type_list_t<TestType>>
            {
                static constexpr bool value = is_in_list_v<TestType, type_list_t<Types...>>; 
            };

            template<typename... Types, typename TestType, typename... TestTypes>
            struct are_all_in_list_st<type_list_t<Types...>, type_list_t<TestType, TestTypes...>>
            {
                static constexpr bool value = 
                    are_all_in_list_st<type_list_t<Types...>, TestType, TestTypes...>::value;
            };

            template<typename TypeList, typename... TestTypes>
            constexpr auto are_all_in_list_v = are_all_in_list_st<TypeList, TestTypes...>::value;

            template<typename Type>
            constexpr auto is_integer_v = is_in_list_v<Type, integer_list_t>;
                        
            template<typename Type>
            constexpr auto is_unsigned_integer_v = is_in_list_v<Type, unsigned_integer_list_t>;
            
            template<typename Type>
            constexpr auto is_signed_integer_v = is_in_list_v<Type, signed_integer_list_t>;

            template<typename Type>
            constexpr auto is_integral_v = is_in_list_v<Type, integral_list_t>;

            template<typename Type>
            constexpr auto is_unsigned_integral_v = is_in_list_v<Type, unsigned_integral_list_t>;

            template<typename Type>
            constexpr auto is_signed_integral_v = is_in_list_v<Type, signed_integral_list_t>;

            template<typename Type>
            constexpr auto is_real_number_v = is_in_list_v<Type, real_number_list_t>;

            template<typename Type>
            constexpr auto is_numerical_number_v = is_in_list_v<Type, numerical_number_list_t>;

            template<typename... Types>
            constexpr auto are_integers_v = are_all_in_list_v<integer_list_t, Types...>;

            template<typename... Types>
            constexpr auto are_unsigned_integers_v = are_all_in_list_v<unsigned_integer_list_t, Types...>;
            
            template<typename... Types>
            constexpr auto are_signed_integers_v = are_all_in_list_v<signed_integer_list_t, Types...>;

            template<typename... Types>
            constexpr auto are_integrals_v = are_all_in_list_v<integral_list_t, Types...>;

            template<typename... Types>
            constexpr auto are_unsigned_integrals_v = are_all_in_list_v<unsigned_integral_list_t, Types...>;

            template<typename... Types>
            constexpr auto are_signed_integrals_v = are_all_in_list_v<signed_integral_list_t, Types...>;

            template<typename... Types>
            constexpr auto are_real_numbers_v = are_all_in_list_v<real_number_list_t, Types...>;

            template<typename... Types>
            constexpr auto are_numerical_numbers_v = are_all_in_list_v<numerical_number_list_t, Types...>;
           
            template<typename Type, typename ReturnType = Type>
			using enable_if_integral_t = 
				std::enable_if_t<is_integral_v<Type>, ReturnType>;

			template<typename Type, typename ReturnType = Type>
			using enable_if_signed_integral_t = 
				std::enable_if_t<is_signed_integral_v<Type>, ReturnType>;

			template<typename Type, typename ReturnType = Type>
			using enable_if_unsigned_integral_t = 
				std::enable_if_t<is_unsigned_integral_v<Type>, ReturnType>;

            template<typename Type, typename ReturnType = Type>
			using enable_if_integer_t = 
				std::enable_if_t<is_integer_v<Type>, ReturnType>;

            template<typename Type, typename ReturnType = Type>
			using enable_if_not_integer_t = 
				std::enable_if_t<!is_integer_v<Type>, ReturnType>;

			template<typename Type, typename ReturnType = Type>
			using enable_if_signed_integer_t = 
				std::enable_if_t<is_signed_integer_v<Type>, ReturnType>;

			template<typename Type, typename ReturnType = Type>
			using enable_if_unsigned_integer_t = 
				std::enable_if_t<is_unsigned_integer_v<Type>, ReturnType>;

            template<typename Type, typename ReturnType = Type>
			using enable_if_real_number_t = 
                std::enable_if_t<is_real_number_v<Type>, ReturnType>;

            template<typename Type, typename ReturnType = Type>
			using enable_if_numerical_number_t = 
                std::enable_if_t<is_numerical_number_v<Type>, ReturnType>;
	           
            template<typename Type, bool bInteger = false>
            struct make_signed_st
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct make_signed_st<Type, true>
            {
                using type = std::make_signed_t<Type>;
            };

            template<typename Type>
            using make_signed_t = typename make_signed_st<Type, is_integer_v<Type>>::type;

            template<typename Type, bool bInteger = false>
            struct make_unsigned_st
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct make_unsigned_st<Type, true>
            {
                using type = std::make_unsigned_t<Type>;
            };

            template<typename Type>
            using make_unsigned_t = typename make_unsigned_st<Type, is_integer_v<Type>>::type;

            template<typename Type, bool bIntegral = false>
            struct make_signed_integral_st
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct make_signed_integral_st<Type, true>
            {
                using type = std::make_signed_t<Type>;
            };

            template<typename Type>
            using make_signed_integral_t = typename make_signed_integral_st<Type, is_integral_v<Type>>::type;

            template<typename Type, bool bIntegral = false>
            struct make_unsigned_integral_st
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct make_unsigned_integral_st<Type, true>
            {
                using type = std::make_signed_t<Type>;
            };

            template<typename Type>
            using make_unsigned_integral_t = typename make_unsigned_integral_st<Type, is_integral_v<Type>>::type;

            template<typename Type, typename... Types>
            struct common_type_st;

            template<typename Type>
            struct common_type_st<Type>
            {
                using type = Type;
            };

            template<typename Type1, typename Type2>
            struct common_type_st<Type1, Type2>
            {
                using type =  is_operable_t<Type1, Type2>;
            };

            template<typename Type1, typename Type2, typename... Types>
            struct common_type_st<Type1, Type2, Types...>
            {
                using type =  is_operable_t<typename common_type_st<Type1, Type2>::type,
                    typename common_type_st<Type2, Types...>::type>;
            };

            template<>
            struct common_type_st<type_list_t<>>
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct common_type_st<type_list_t<Type>>
            {
                using type = Type;
            };

            template<typename Type1, typename Type2>
            struct common_type_st<type_list_t<Type1, Type2>>
            {
                using type = typename common_type_st<Type1, Type2>::type;
            };

            template<typename Type1, typename Type2, typename... Types>
            struct common_type_st<type_list_t<Type1, Type2, Types...>>
            {
                using type = typename common_type_st<Type1, Type2, Types...>::type;
            };

            template<typename... Types>
            struct st_common_type_solver;

            template<>
            struct st_common_type_solver<>
            {
                using type = no_type_t;
            };

            template<>
            struct st_common_type_solver<type_list_t<>>
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct st_common_type_solver<Type>
            {
                using type = Type;
            };

            template<typename Type>
            struct st_common_type_solver<type_list_t<Type>>
            {
                using type = Type;
            };

            template<typename Type, typename... Types>
            struct st_common_type_solver<Type, Types...>
            {
                using type = typename common_type_st<Type, Types...>::type;
            };

            template<typename Type, typename... Types>
            struct st_common_type_solver<type_list_t<Type, Types...>>
            {
                using type = typename common_type_st<Type, Types...>::type;
            };

            template<typename... Types>
            using common_type_t = typename st_common_type_solver<Types...>::type;

            template<typename... Types>
            constexpr auto common_type_v = is_valid_type_v<common_type_t<Types...>>;
               
            template<typename... Types>
            struct select_first_type_st;

            template<>
            struct select_first_type_st<>
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct select_first_type_st<Type>
            {
                using type = Type;
            };

            template<typename Type, typename... Types>
            struct select_first_type_st<Type, Types...>
            {
                using type = Type;
            };

            template<>
            struct select_first_type_st<type_list_t<>>
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct select_first_type_st<type_list_t<Type>>
            {
                using type = Type;
            };

            template<typename Type, typename... Types>
            struct select_first_type_st<type_list_t<Type, Types...>>
            {
                using type = Type;
            };
        
            template<typename... Types>
            using select_first_type_t = typename select_first_type_st<Types...>::type;

            template<typename... Types>
            using first_type_t = select_first_type_t<Types...>;

            template<typename... Types>
            using front_type_t = select_first_type_t<Types...>;
            
            template<typename... Types>
            struct select_last_type_st;

            template<>
            struct select_last_type_st<>
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct select_last_type_st<Type>
            {
                using type = Type;
            };

            template<typename Type, typename... Types>
            struct select_last_type_st<Type, Types...>
            {
                using type = std::conditional_t<sizeof...(Types)==0, 
                    Type, typename select_last_type_st<Types...>::type>;
            };

            template<>
            struct select_last_type_st<type_list_t<>>
            {
                using type = no_type_t;
            };

            template<typename Type>
            struct select_last_type_st<type_list_t<Type>>
            {
                using type = Type;
            };

            template<typename Type, typename... Types>
            struct select_last_type_st<type_list_t<Type, Types...>>
            {
                using type = std::conditional_t<sizeof...(Types)==0, 
                    Type, typename select_last_type_st<Types...>::type>;
            };

            template<typename... Types>
            using select_last_type_t = typename select_last_type_st<Types...>::type;
    
            template<typename... Types>
            using last_type_t = select_last_type_t<Types...>;

            template<typename... Types>
            using back_type_t = select_last_type_t<Types...>;

            template<auto SelectIndex, typename... Types>
            struct select_nth_type_st;

            template<auto SelectIndex>
            struct select_nth_type_st<SelectIndex>
            {
                using type = no_type_t;
            };

            template<typename Type, typename... Types>
            struct select_nth_type_st<0, Type, Types...>
            {
                using type = Type;
            };

            template<auto SelectIndex, typename Type, typename... Types>
            struct select_nth_type_st<SelectIndex, Type, Types...>
            {
                using type = std::conditional_t<SelectIndex==0, 
                    Type, typename select_nth_type_st<SelectIndex-1, Types...>::type>;
            };

            template<auto SelectIndex>
            struct select_nth_type_st<SelectIndex, type_list_t<>>
            {
                using type = no_type_t;
            };

            template<typename Type, typename... Types>
            struct select_nth_type_st<0, type_list_t<Type, Types...>>
            {
                using type = Type;
            };

            template<auto SelectIndex, typename Type, typename... Types>
            struct select_nth_type_st<SelectIndex, type_list_t<Type, Types...>>
            {
                using type = std::conditional_t<SelectIndex==0, 
                    Type, typename select_nth_type_st<SelectIndex-1, Types...>::type>;
            };

            template<typename Type>
            struct is_reference_wrapper_st
            {
                static constexpr bool value = false;
            };

            template<typename Type>
            struct is_reference_wrapper_st<std::reference_wrapper<Type>>
            {
                static constexpr bool value = true;
            };

            template<typename Type>
            constexpr auto is_reference_wrapper_v = 
                is_reference_wrapper_st<remove_cv_ref_t<Type>>::value;

            template<auto SelectIndex, typename... Types>
            using select_nth_type_t = typename select_nth_type_st<SelectIndex, Types...>::type;

            template<auto SelectIndex, typename... Types>
            using nth_type_t = select_nth_type_t<SelectIndex, Types...>;

            template<typename Type>
            using first_parameter_type_t = first_type_t<template_parameter_type_list_t<Type>>;

            template<typename Type>
            constexpr auto is_template_template_v = is_template_v<first_parameter_type_t<Type>>;

            template<typename Type>
            using last_parameter_type_t = last_type_t<template_parameter_type_list_t<Type>>;

            template<auto SelectIndex, typename Type>
            using nth_parameter_type_t = nth_type_t<SelectIndex, template_parameter_type_list_t<Type>>;
           
            template<typename ArgType, typename... Types>
            struct push_front_type_st
            {
                using type = type_list_t<ArgType, Types...>;
            };

            template<typename ArgType, typename... Types>
            struct push_front_type_st<ArgType, type_list_t<Types...>>
            {
                using type = type_list_t<ArgType, Types...>;
            };

            template<typename ArgType, typename... Types>
            using push_front_type_t = typename push_front_type_st<ArgType, Types...>::type;

            template<typename ArgType, typename... Types>
            struct push_back_type_st
            {
                using type = type_list_t<Types..., ArgType>;
            };
            
            template<typename ArgType, typename... Types>
            struct push_back_type_st<ArgType, type_list_t<Types...>>
            {
                using type = type_list_t<Types..., ArgType>;
            };

            template<typename ArgType, typename... Types>
            using push_back_type_t = typename push_back_type_st<ArgType, Types...>::type;

            template<typename ListType>
            struct pop_front_type_st;

            template<>
            struct pop_front_type_st<type_list_t<>>
            {
                using type = type_list_t<>;
            };

            template<typename Type, typename... Types>
            struct pop_front_type_st<type_list_t<Type, Types...>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename... Types> 
            struct pop_front_wrapper_st
            {
                using type = typename pop_front_type_st<type_list_t<Types...>>::type;
            };

            template<typename... Types> 
            struct pop_front_wrapper_st<type_list_t<Types...>>
            {
                using type = typename pop_front_type_st<type_list_t<Types...>>::type;
            };
        
            template<typename... Types>
            using pop_front_type_t = typename pop_front_wrapper_st<Types...>::type;

            template<typename LeftList, typename RightList>
            struct pop_back_type_st;

            template<typename... LeftTypes>
            struct pop_back_type_st<type_list_t<LeftTypes...>, type_list_t<>>
            {
                using type = type_list_t<LeftTypes...>;
            };

            template<typename... LeftTypes, typename Type, typename... RightTypes>
            struct pop_back_type_st<type_list_t<LeftTypes...>, type_list_t<Type, RightTypes...>>
            {
                using left_list = push_back_type_t<Type, type_list_t<LeftTypes...>>;
                using right_list = type_list_t<RightTypes...>;

                using type = std::conditional_t<sizeof...(RightTypes) == 0,
                    type_list_t<LeftTypes...>, typename pop_back_type_st<left_list, right_list>::type>;
            };

            template<typename... Types>
            struct pop_back_type_wrapper_st
            {
                using type = typename pop_back_type_st<type_list_t<>, type_list_t<Types...>>::type;
            };

            template<typename... Types>
            struct pop_back_type_wrapper_st<type_list_t<Types...>>
            {
                using type = typename pop_back_type_st<type_list_t<>, type_list_t<Types...>>::type;
            };

            template<typename... Types>
            using pop_back_type_t = typename pop_back_type_wrapper_st<Types...>::type;

            //////////////////////////////////////////////////
            template<size_t FirstN, typename LeftList, typename RightList> struct first_n_types_list_st;

            template<>
            struct first_n_types_list_st<0, type_list_t<>, type_list_t<>>
            {
                using type = type_list_t<>;
            };

            template<typename RightType, typename... RightTypes>
            struct first_n_types_list_st<0, type_list_t<>, type_list_t<RightType, RightTypes...>>
            {
                using type = type_list_t<>;
            };

            template<typename... LeftTypes>
            struct first_n_types_list_st<1, type_list_t<LeftTypes...>, type_list_t<>>
            {
                using type = type_list_t<LeftTypes...>;
            };

            template<typename... LeftTypes, typename RightType, typename... RightTypes>
            struct first_n_types_list_st<1, type_list_t<LeftTypes...>, type_list_t<RightType, RightTypes...>>
            {
                using type = type_list_t<LeftTypes..., RightType>;
            };

            template<size_t FirstN, typename... LeftTypes, typename RightType, typename... RightTypes>
            struct first_n_types_list_st<FirstN, type_list_t<LeftTypes...>, type_list_t<RightType, RightTypes...>>
            {
                // when FirstN == 0
                using type_list_0 = type_list_t<LeftTypes...>;
                
                // when FirstN > 0, FirstN = 1, 2, 3...
                using type_list_1 =  std::conditional_t<FirstN == 1, type_list_t<LeftTypes..., RightType>,
                    typename first_n_types_list_st<FirstN-1, type_list_t<LeftTypes..., RightType>, type_list_t<RightTypes...>>::type>;

                using type = std::conditional_t<FirstN == 0, type_list_0, type_list_1>;
            };

            template<size_t FirstN, typename... Types> struct first_n_types_st;
            
            template<size_t FirstN, typename... RightTypes>
            struct first_n_types_st<FirstN, type_list_t<>, RightTypes...>
            {
                // static_assert(FirstN <= sizeof...(RightTypes), "FirstN out of range");

                using type = std::conditional_t<FirstN == 0, type_list_t<>,
                    std::conditional_t<FirstN >= sizeof...(RightTypes), type_list_t<RightTypes...>,
                    typename first_n_types_list_st<FirstN, type_list_t<>, type_list_t<RightTypes...>>::type>>;
            };

            template<size_t FirstN, typename... RightTypes>
            struct first_n_types_st<FirstN, type_list_t<>, type_list_t<RightTypes...>>
            {
                // static_assert(FirstN <= sizeof...(RightTypes), "FirstN out of range");

                using type = std::conditional_t<FirstN == 0, type_list_t<>,
                    std::conditional_t<FirstN >= sizeof...(RightTypes), type_list_t<RightTypes...>,
                    typename first_n_types_list_st<FirstN, type_list_t<>, type_list_t<RightTypes...>>::type>>;
            };

            template<size_t FirstN, typename... RightTypes>
            using first_n_types_t = typename first_n_types_st<FirstN, type_list_t<>, RightTypes...>::type;

            template<size_t FirstN, typename... RightTypes>
            using select_first_n_types_t = first_n_types_t<FirstN, RightTypes...>;

            ////////////////////////////////////////////////////////////////////////////////////////////////
            template<size_t LastN, typename LeftList, typename RightList> struct last_n_types_list_st;

            template<> 
            struct last_n_types_list_st<0, type_list_t<>, type_list_t<>>
            {
                using type = type_list_t<>;
            };

            template<typename LeftType, typename... LeftTypes> 
            struct last_n_types_list_st<0, type_list_t<LeftType, LeftTypes...>, type_list_t<>>
            {
                using type = type_list_t<>;
            };

            template<typename... RightTypes> 
            struct last_n_types_list_st<1, type_list_t<>, type_list_t<RightTypes...>>
            {
                using type = type_list_t<>;
            };

            template<typename LeftType, typename... LeftTypes, typename... RightTypes> 
            struct last_n_types_list_st<1, type_list_t<LeftType, LeftTypes...>, type_list_t<RightTypes...>>
            {
                using type = type_list_t<select_last_type_t<type_list_t<LeftType, LeftTypes...>>, RightTypes...>;
            };

            template<size_t LastN, typename LeftType, typename... LeftTypes, typename... RightTypes> 
            struct last_n_types_list_st<LastN, type_list_t<LeftType, LeftTypes...>, type_list_t<RightTypes...>>
            {
                // LastN == 0
                using type0 = type_list_t<RightTypes...>;

                using new_right = type_list_t<select_last_type_t<type_list_t<LeftType, LeftTypes...>>, RightTypes...>;
                using new_left = pop_back_type_t<type_list_t<LeftType, LeftTypes...>>;

                // LastN = 1, 2, 3
                using type1 = std::conditional_t<LastN == 1, new_right, 
                    typename last_n_types_list_st<LastN-1, new_left, new_right>::type>;

                using type = std::conditional_t<LastN==0, type0, type1>;
            };

            template<size_t LastN, typename... LeftTypes> 
            struct last_n_types_st
            {
                // static_assert(FirstN <= sizeof...(LeftTypes), "FirstN out of range");

                using type = std::conditional_t<LastN==0, type_list_t<>, 
                    std::conditional_t<LastN >= sizeof...(LeftTypes), type_list_t<LeftTypes...>, 
                    typename last_n_types_list_st<LastN, type_list_t<LeftTypes...>, type_list_t<>>::type>>;
            };

            template<size_t LastN, typename... LeftTypes> 
            struct last_n_types_st<LastN, type_list_t<LeftTypes...>>
            {
                // static_assert(FirstN <= sizeof...(LeftTypes), "FirstN out of range");

                using type = std::conditional_t<LastN==0, type_list_t<>, 
                    std::conditional_t<LastN >= sizeof...(LeftTypes), type_list_t<LeftTypes...>, 
                    typename last_n_types_list_st<LastN, type_list_t<LeftTypes...>, type_list_t<>>::type>>;
            };

            template<size_t LastN, typename... LeftTypes> 
            using last_n_types_t = typename last_n_types_st<LastN, LeftTypes...>::type;

            template<size_t LastN, typename... LeftTypes> 
            using select_last_n_types_t = last_n_types_t<LastN, LeftTypes...>;

            template<typename ArgType, typename... Types>
            struct is_type_in_list_st;

            template<typename ArgType>
            struct is_type_in_list_st<ArgType>
            {
                static constexpr bool value = false;
            };

            template<typename ArgType, typename Type, typename... RightTypes>
            struct is_type_in_list_st<ArgType, Type, RightTypes...>
            {
                static constexpr bool value = std::is_same_v<ArgType, Type> ? true :
                    is_type_in_list_st<ArgType, RightTypes...>::value;
            };

            template<typename ArgType>
            struct is_type_in_list_st<ArgType, type_list_t<>>
            {
                static constexpr bool value = false;
            };

            template<typename ArgType, typename Type, typename... RightTypes>
            struct is_type_in_list_st<ArgType, type_list_t<Type, RightTypes...>>
            {
                static constexpr bool value = is_type_in_list_st<ArgType, Type, RightTypes...>::value;
            };

            template<typename ArgType, typename... Types>
            constexpr auto is_type_in_list_v = is_type_in_list_st<ArgType, Types...>::value;

            template<typename TestType, typename TypeList, typename ReturnType = TestType>
            using enable_if_in_list_t = std::enable_if_t<is_type_in_list_v<TestType, TypeList>, ReturnType>;

            template<typename TestTypeList, typename TypeList, typename ReturnType = void>
            using enable_if_all_in_list_t = std::enable_if_t<are_all_in_list_v<TypeList, TestTypeList>, ReturnType>;

            template<typename ListType, typename... Types>
            struct unique_types_st;

            template<typename... Types>
            struct unique_types_st<type_list_t<Types...>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename... Types, typename Type>
            struct unique_types_st<type_list_t<Types...>, Type>
            {
                using list = type_list_t<Types...>;

                using type = std::conditional_t<is_type_in_list_v<Type, list>,
                    list, type_list_t<Types..., Type>>;
            };

            template<typename... Types, typename Type, typename... RightTypes>
            struct unique_types_st<type_list_t<Types...>, Type, RightTypes...>
            {
                using list = typename unique_types_st<type_list_t<Types...>, Type>::type;

                using type = typename unique_types_st<list, RightTypes...>::type;
            };

            template<typename... Types, typename... RightTypes>
            struct unique_types_st<type_list_t<Types...>, type_list_t<RightTypes...>>
            {
                using type = typename unique_types_st<type_list_t<Types...>, RightTypes...>::type;
            };

            template<typename... Types>
            using unique_types_t = typename unique_types_st<type_list_t<>, Types...>::type;

            template<typename Type, typename... Types>
            struct to_variant_st
            {
                // using list = unique_types_t<Type, Types...>;
                using type = std::variant<Type, Types...>;
            };

            template<typename Type, typename... Types>
            struct to_variant_st<type_list_t<Type, Types...>>
            {
                using type = std::variant<Type, Types...>;
            };

            template<typename Type, typename... Types>
            struct to_variant_st<std::tuple<Type, Types...>>
            {
                using list_t = unique_types_t<Type, Types...>;
                using type = typename to_variant_st<list_t>::type;
            };            

            template<typename Type, typename... Types>
            using to_variant_t = typename to_variant_st<Type, Types...>::type;    

            template<typename... Types>
            struct to_tuple_st
            {
                using type = std::tuple<Types...>;
            };

            template<typename... Types>
            struct to_tuple_st<type_list_t<Types...>>
            {
                using type = std::tuple<Types...>;
            };

            template<typename... Types>
            struct to_tuple_st<std::variant<Types...>>
            {
                using type = std::tuple<Types...>;
            };    

            template<typename... Types>
            using to_tuple_t = typename to_tuple_st<Types...>::type;    

            template<typename ArgType, typename RightList>
            struct prepend_type_st;

            template<typename ArgType, typename... RightTypes>
            struct prepend_type_st<ArgType, type_list_t<RightTypes...>>
            {
                using type = type_list_t<ArgType, RightTypes...>;
            };

            template<typename... ArgTypes, typename... RightTypes>
            struct prepend_type_st<type_list_t<ArgTypes...>, type_list_t<RightTypes...>>
            {
                using type = type_list_t<ArgTypes..., RightTypes...>;
            };

            template<typename ArgType, typename RightList>
            using prepend_type_t = typename prepend_type_st<ArgType, RightList>::type;

            template<typename LeftList, typename RightList>
            struct append_type_st;

            template<typename ArgType, typename... RightTypes>
            struct append_type_st<ArgType, type_list_t<RightTypes...>>
            {
                using type = type_list_t<RightTypes..., ArgType>;
            };

            template<typename... ArgTypes, typename... RightTypes>
            struct append_type_st<type_list_t<ArgTypes...>, type_list_t<RightTypes...>>
            {
                using type = type_list_t<RightTypes..., ArgTypes...>;
            };

            template<typename ArgType, typename RightList>
            using append_type_t = typename append_type_st<ArgType, RightList>::type;

            template<typename LeftList, typename... Types>
            struct union_type_st;
            
            template<typename... Types>
            struct union_type_st< type_list_t<Types...> >
            {
                using type = type_list_t<Types...>;
            };

            template<typename... Types, typename Type>
            struct union_type_st< type_list_t<Types...>, Type>
            {
                using list = type_list_t<Types...>;

                using type = std::conditional_t<is_type_in_list_v<Type, list>,
                    list, type_list_t<Types..., Type>>;
            };

            template<typename... Types, typename Type, typename... RightTypes>
            struct union_type_st<type_list_t<Types...>, Type, RightTypes...>
            {
                using list = typename union_type_st<type_list_t<Types...>, Type>::type;

                using type = std::conditional_t<sizeof...(RightTypes)==0,
                    list, typename union_type_st<list, RightTypes...>::type>;
            };

            template<typename ArgType, typename... Types>
            struct union_type_st<ArgType, type_list_t<Types...>>
            {
                using list = type_list_t<Types...>;
                using type = std::conditional_t<is_in_list_v<ArgType, list>,
                    list, type_list_t<ArgType, Types...>>;
            };

            template<typename... Types>
            struct union_type_st<type_list_t<Types...>, type_list_t<>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename... Types, typename Type>
            struct union_type_st< type_list_t<Types...>, type_list_t<Type>>
            {
                using type = typename union_type_st<type_list_t<Types...>, Type>::type;
            };

            template<typename... Types, typename Type, typename... RightTypes>
            struct union_type_st<type_list_t<Types...>, type_list_t<Type, RightTypes...>>
            {
                using type = typename union_type_st<type_list_t<Types...>, Type, RightTypes...>::type;
            };

            template<typename LeftList, typename RightList>
            using union_type_t = typename union_type_st<LeftList, RightList>::type;

            template<typename LeftType, typename... Types>
            struct intersection_type_st;

            template<typename... Types>
            struct intersection_type_st<type_list_t<Types...>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename... Types, typename Type>
            struct intersection_type_st<type_list_t<Types...>, Type>
            {
                using list = type_list_t<Types...>;

                using type = std::conditional_t<is_type_in_list_v<Type, list>,
                    type_list_t<Type>, type_list_t<>>;
            };

            template<typename... Types, typename Type, typename... RightTypes>
            struct intersection_type_st<type_list_t<Types...>, Type, RightTypes...>
            {
                using left_list = type_list_t<Types...>;
                using list = typename intersection_type_st<left_list, Type>::type;

                using type = std::conditional_t<sizeof...(RightTypes) == 0,
                    list, append_type_t<list, typename intersection_type_st<left_list, RightTypes...>::type>>;
            };
            
            template<typename... Types>
            struct intersection_type_st<type_list_t<Types...>, type_list_t<>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename... Types, typename Type>
            struct intersection_type_st<type_list_t<Types...>, type_list_t<Type>>
            {
                using type = typename intersection_type_st<type_list_t<Types...>, Type>::type;
            };

            template<typename... Types, typename Type, typename... RightTypes>
            struct intersection_type_st<type_list_t<Types...>, type_list_t<Type, RightTypes...>>
            {
                using type = typename intersection_type_st<type_list_t<Types...>, Type, RightTypes...>::type;
            };

            template<typename LeftList, typename RightList>
            using intersection_type_t = typename intersection_type_st<LeftList, RightList>::type;

            template<typename ArgType, typename... Types>
            struct remove_type_st;

            template<typename ArgType>
            struct remove_type_st<ArgType>
            {
                using type = type_list_t<>;
            };

            template<typename ArgType, typename Type>
            struct remove_type_st<ArgType, Type>
            {
                using type = std::conditional_t<std::is_same_v<ArgType, Type>,
                    type_list_t<>, type_list_t<Type>>;
            };

            template<typename ArgType, typename Type, typename... RightTypes>
            struct remove_type_st<ArgType, Type, RightTypes...>
            {
                using list = typename remove_type_st<ArgType, Type>::type;

                using type = std::conditional_t<sizeof...(RightTypes) == 0,
                    list, prepend_type_t<list, typename remove_type_st<ArgType, RightTypes...>::type>>;
            };

            template<typename ArgType>
            struct remove_type_st<ArgType, type_list_t<>>
            {
                using type = type_list_t<>;
            };

            template<typename ArgType, typename Type>
            struct remove_type_st<ArgType, type_list_t<Type>>
            {
                using type = typename remove_type_st<ArgType, Type>::type;
            };

            template<typename ArgType, typename Type, typename... RightTypes>
            struct remove_type_st<ArgType, type_list_t<Type, RightTypes...>>
            {
                using type = typename remove_type_st<ArgType, Type, RightTypes...>::type;
            };

            template<typename ArgType, typename... Types>
            struct remove_type_wrapper_st
            {
                using type = typename remove_type_st<ArgType, type_list_t<Types...>>::type;
            };

            template<typename ArgType, typename... Types>
            struct remove_type_wrapper_st<ArgType, type_list_t<Types...>>
            {
                using type = typename remove_type_st<ArgType, type_list_t<Types...>>::type;
            };

            template<typename ArgType, typename... Types>
            using remove_type_t = 
                typename remove_type_wrapper_st<ArgType, Types...>::type;

            template<typename LeftList, typename... Types>
            struct difference_type_st;

            template<typename... Types>
            struct difference_type_st<type_list_t<Types...>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename... Types, typename Type>
            struct difference_type_st<type_list_t<Types...>, Type>
            {
                using type = remove_type_t<Type, type_list_t<Types...>>;
            };

            template<typename... Types, typename Type, typename... RightTypes>
            struct difference_type_st<type_list_t<Types...>, Type, RightTypes...>
            {
                using list = typename difference_type_st<type_list_t<Types...>, Type>::type;

                using type = std::conditional_t<sizeof...(RightTypes) == 0,
                    list, typename difference_type_st<list, RightTypes...>::type>;
            };

            template<typename... Types>
            struct difference_type_st<type_list_t<Types...>, type_list_t<>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename... Types, typename Type>
            struct difference_type_st<type_list_t<Types...>, type_list_t<Type>>
            {
                using type = typename difference_type_st<type_list_t<Types...>, Type>::type;
            };

            template<typename... Types, typename Type, typename... RightTypes>
            struct difference_type_st<type_list_t<Types...>, type_list_t<Type, RightTypes...>>
            {
                using type = typename difference_type_st<type_list_t<Types...>, Type, RightTypes...>::type;
            };

            template<typename LeftList, typename RightList>
            using difference_type_t = typename difference_type_st<LeftList, RightList>::type;

            template<typename NewType, typename OldType, typename... Types>
            struct replace_type_st;

            template<typename NewType, typename OldType>
            struct replace_type_st<NewType, OldType>
            {
                using type = type_list_t<>;
            };

            template<typename NewType, typename OldType, typename Type>
            struct replace_type_st<NewType, OldType, Type>
            {
                using type = std::conditional_t< std::is_same_v<OldType, Type>,
                    type_list_t<NewType>, type_list_t<Type>>;             
            };

            template<typename NewType, typename OldType, typename Type, typename... RightTypes>
            struct replace_type_st<NewType, OldType, Type, RightTypes...>
            {
                using list = typename replace_type_st<NewType, OldType, Type>::type;

                using type = std::conditional_t< sizeof...(RightTypes), list,
                    prepend_type_t<list, typename replace_type_st<NewType, OldType, RightTypes...>::type>>;        
            };

            template<typename NewType, typename OldType>
            struct replace_type_st<NewType, OldType, type_list_t<>>
            {
                using type = type_list_t<>;
            };

            template<typename NewType, typename OldType, typename Type>
            struct replace_type_st<NewType, OldType, type_list_t<Type>>
            {
                using type = typename replace_type_st<NewType, OldType, Type>::type;             
            };

            template<typename NewType, typename OldType, typename Type, typename... RightTypes>
            struct replace_type_st<NewType, OldType, type_list_t<Type, RightTypes...>>
            {
                using type = typename replace_type_st<NewType, OldType, Type, RightTypes...>::type;        
            };

            template<typename NewType, typename OldType, typename TypeList>
            using replace_type_t =  typename replace_type_st<NewType, OldType, TypeList>::type;

            template<typename Type, typename... Types>
            struct is_type_list_equivalent_st;

            template<typename... LeftTypes>
            struct is_type_list_equivalent_st<type_list_t<LeftTypes...>>
            {
                static constexpr bool value = true; 
            };

            template<typename... LeftTypes, typename Type>
            struct is_type_list_equivalent_st<type_list_t<LeftTypes...>, Type>
            {
                static constexpr bool value = 
                    is_type_in_list_v<Type, type_list_t<LeftTypes...>>; 
            };

            template<typename... LeftTypes, typename Type, typename... RightTypes>
            struct is_type_list_equivalent_st<type_list_t<LeftTypes...>, Type, RightTypes...>
            {
                static constexpr bool tmp_value = 
                    is_type_list_equivalent_st< type_list_t<LeftTypes...>, Type>::value;

                static constexpr bool value = sizeof...(RightTypes)==0 ? tmp_value :
                    tmp_value && is_type_list_equivalent_st<type_list_t<LeftTypes...>, RightTypes...>::value;
            };

            template<typename... LeftTypes, typename... RightTypes>
            struct is_type_list_equivalent_st<type_list_t<LeftTypes...>, type_list_t<RightTypes...>>
            {
                static constexpr bool value = sizeof...(LeftTypes) != sizeof...(RightTypes) ?
                    false : is_type_list_equivalent_st<type_list_t<LeftTypes...>, RightTypes...>::value;
            };

            template<typename LeftList, typename RightList>
            constexpr auto is_type_list_equivalent_v = is_type_list_equivalent_st<LeftList, RightList>::value;

        } // end of namespace hidden

        template<typename Type>
        constexpr auto is_reference_wrapper_v = hidden::is_reference_wrapper_v<Type>;

        /**
         * @brief Test if all types are the same
         * 
         * @tparam Type for the first type
         * @tparam Types for the rest of types
         * 
         */
        template<typename Type, typename... Types>
        constexpr auto is_same_v = hidden::is_same_v<Type, Types...>;
        
        template<typename Type, typename... Types>
        using flat_type_list_t = tpf::types::type_list_t<decay_remove_cv_ref_t<Type>, decay_remove_cv_ref_t<Types>...>;

        template<typename Type, typename... Types>
        constexpr auto is_same_flat_v = is_same_v<flat_type_list_t<Type, Types...>>;

        template<typename ReturnType, typename Type, typename... Types>
        using enable_if_same_t = std::enable_if_t< is_same_v<Type, Types...>, ReturnType>;

        template<typename ReturnType, typename Type, typename... Types>
        using enable_if_same_flat_t = std::enable_if_t< is_same_flat_v<Type, Types...>, ReturnType>;


        /**
         * @brief test if TestType exists in Types...
         * 
         * @tparam TestType for type to test its existance
         * @tparam Types for type list to test against
         * 
         */
        template<typename TestType, typename... Types>
        constexpr auto is_in_list_v = hidden::is_in_list_v<TestType, Types...>;

        template<typename Type>
        constexpr auto is_integer_v = hidden::is_integer_v<Type>;

        template<typename Type>
        constexpr auto is_signed_integer_v = hidden::is_signed_integer_v<Type>;

        template<typename Type>
        constexpr auto is_unsigned_integer_v = hidden::is_unsigned_integer_v<Type>;

        template<typename Type>
        constexpr auto is_integral_v = hidden::is_integral_v<Type>;

        template<typename Type>
        constexpr auto is_signed_integral_v = hidden::is_signed_integral_v<Type>;

        template<typename Type>
        constexpr auto is_unsigned_integral_v = hidden::is_unsigned_integral_v<Type>;

        template<typename Type>
        constexpr auto is_real_number_v = hidden::is_real_number_v<Type>;

        template<typename Type>
        constexpr auto is_numerical_number_v = hidden::is_numerical_number_v<Type>;

        template<typename TypeList, typename... TestTypes>
        constexpr auto are_all_in_list_v = hidden::are_all_in_list_v<TypeList, TestTypes...>;

        template<typename... Types>
        constexpr auto are_integers_v = hidden::are_integers_v<Types...>;

        template<typename... Types>
        constexpr auto are_signed_integers_v = hidden::are_signed_integers_v<Types...>;

        template<typename... Types>
        constexpr auto are_unsigned_integers_v = hidden::are_unsigned_integers_v<Types...>;

        template<typename... Types>
        constexpr auto are_integrals_v = hidden::are_integrals_v<Types...>;

        template<typename... Types>
        constexpr auto are_signed_integrals_v = hidden::are_signed_integrals_v<Types...>;

        template<typename... Types>
        constexpr auto are_unsigned_integrals_v = hidden::are_unsigned_integrals_v<Types...>;

        template<typename... Types>
        constexpr auto are_real_numbers_v = hidden::are_real_numbers_v<Types...>;

        template<typename... Types>
        constexpr auto are_numerical_numbers_v = hidden::are_numerical_numbers_v<Types...>;
      
        template<typename Type, typename ReturnType = Type>
        using enable_if_integral_t = hidden::enable_if_integral_t<Type, ReturnType>;
        
        template<typename Type, typename ReturnType = Type>
        using enable_if_signed_integral_t = hidden::enable_if_signed_integral_t<Type, ReturnType>;
        
        template<typename Type, typename ReturnType = Type>
        using enable_if_unsigned_integral_t = hidden::enable_if_unsigned_integral_t<Type, ReturnType>;
        
        template<typename Type, typename ReturnType = Type>
        using enable_if_integer_t = hidden::enable_if_integer_t<Type, ReturnType>;

        template<typename Type, typename ReturnType = Type>
        using enable_if_not_integer_t = hidden::enable_if_not_integer_t<Type, ReturnType>;
        
        template<typename Type, typename ReturnType = Type>
        using enable_if_signed_integer_t = hidden::enable_if_signed_integer_t<Type, ReturnType>;
        
        template<typename Type, typename ReturnType = Type>
        using enable_if_unsigned_integer_t = hidden::enable_if_unsigned_integer_t<Type, ReturnType>;

        template<typename Type, typename ReturnType = Type>
        using enable_if_real_number_t = hidden::enable_if_real_number_t<Type, ReturnType>;
        
        template<typename Type, typename ReturnType = Type>
        using enable_if_numerical_number_t = hidden::enable_if_numerical_number_t<Type, ReturnType>;
        
        template<typename Type>
        using make_unsigned_t = hidden::make_unsigned_t<Type>;

        template<typename Type>
        using make_signed_t = hidden::make_signed_t<Type>;

        template<typename Type>
        using make_unsigned_integral_t = hidden::make_unsigned_integral_t<Type>;

        template<typename Type>
        using make_signed_integral_t = hidden::make_signed_integral_t<Type>;

        template<typename Type1, typename Type2>
        constexpr auto is_operable_v = hidden::is_operable_v<Type1, Type2>;

        template<typename... Types>
        using common_type_t = hidden::common_type_t<Types...>;

        template<typename... Types>
        using make_common_unsigned_t = make_unsigned_t<common_type_t<Types...>>;
        
        template<typename... Types>
        using make_common_signed_t = make_signed_t<common_type_t<Types...>>;

        template<typename... Types>
        constexpr auto common_type_v = hidden::common_type_v<Types...>;

        template<typename Type>
        constexpr bool is_empty_available_v = hidden::is_empty_available_v<Type>;

        template<typename Type>
        constexpr bool is_capacity_available_v = hidden::is_capacity_available_v<Type>;

        template<typename Type>
        constexpr bool is_shrink_to_fit_available_v = hidden::is_shrink_to_fit_available_v<Type>;

        template<typename Type>
        constexpr bool is_index_operator_available_v = hidden::is_index_operator_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_push_front_available_v = hidden::is_push_front_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_push_back_available_v = hidden::is_push_back_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_begin_available_v = hidden::is_begin_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_end_available_v = hidden::is_end_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_rbegin_available_v = hidden::is_rbegin_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_rend_available_v = hidden::is_rend_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_erase_available_v = hidden::is_erase_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_resize_available_v = hidden::is_resize_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_reserve_available_v = hidden::is_reserve_available_v<remove_cv_ref_t<Type>>;
        
        template<typename Type>
        constexpr bool is_size_available_v = hidden::is_size_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_insert_available_v = hidden::is_insert_available_v<Type>; 

        template<typename Type>
        constexpr bool is_insert_iterator_available_v = hidden::is_insert_iterator_available_v<Type>; 

        template<typename Type>
        constexpr bool is_emplace_front_available_v = hidden::is_emplace_front_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_emplace_back_available_v = hidden::is_emplace_back_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_emplace_available_v = hidden::is_emplace_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_front_available_v = hidden::is_front_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_back_available_v = hidden::is_back_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_pop_front_available_v = hidden::is_pop_front_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_pop_back_available_v = hidden::is_pop_back_available_v<remove_cv_ref_t<Type>>;

        template<typename Type>
        constexpr bool is_container_type_v = hidden::is_container_type_v<remove_cv_ref_t<Type>>;

        namespace hidden
        {
            template<typename FuncType, typename... ArgTypes>
            auto function_return_type_fn(FuncType&& func, ArgTypes&&... args) -> decltype(func(args...));
            no_type_t function_return_type_fn(...);

            template<typename FuncType, typename... ArgTypes>
            using function_return_type_t = 
                remove_cvref_t<decltype(function_return_type_fn(std::declval<FuncType>(), std::declval<ArgTypes>()...))>;

            template<typename FuncType, typename... ArgTypes>
            constexpr bool is_callable_v = !is_no_type_v<function_return_type_t<FuncType, ArgTypes...>>;

            template<typename FuncType, typename... ArgTypes>
            constexpr bool is_void_return_type_v = is_callable_v<FuncType, ArgTypes...>
                && std::is_void_v<function_return_type_t<FuncType, ArgTypes...>>;
        }

        template<typename FuncType, typename... ArgTypes>
        constexpr bool is_callable_v = hidden::is_callable_v<FuncType, ArgTypes...>;

        template<typename FuncType, typename... ArgTypes>
        using function_return_type_t = hidden::function_return_type_t<FuncType, ArgTypes...>;

        template<typename FuncType, typename... ArgTypes>
        constexpr bool is_void_return_type_v = hidden::is_void_return_type_v<FuncType, ArgTypes...>;

        ////////////////////////////////////////
        namespace hidden
        {
            template<typename T, typename... Types>
            struct st_pairwise_common;

            template<typename T>
            struct st_pairwise_common<T>
            {
                static constexpr bool value = false;
            };

            template<typename T, typename S>
            struct st_pairwise_common<T, S>
            {
                static constexpr bool value = common_type_v<T, S>;
            };

            template<typename T, typename S, typename... Types>
            struct st_pairwise_common<T, S, Types...>
            {
                static constexpr bool value = common_type_v<T, S> && st_pairwise_common<T, Types...>::value;
            };

            template<typename... ArgTypes1, typename... ArgTypes2>
            auto tuple_addition_operator(const std::tuple<ArgTypes1...>& tuple_a,
                const std::tuple<ArgTypes2...>& tuple_b);

            template<typename... ArgTypes1, typename... ArgTypes2, auto... Indices>
            auto tuple_addition(const std::tuple<ArgTypes1...>& tuple_a,
                const std::tuple<ArgTypes2...>& tuple_b, types::typed_sequence_t<Indices...>)
            {
                auto tuple_opr =[](auto a, auto b)
                {
                    using type_a = types::remove_cvref_t<decltype(a)>;
                    using type_b = types::remove_cvref_t<decltype(b)>;

                    if constexpr(types::is_tuple_v<type_a> && types::is_tuple_v<type_b>)
                    {
                        return tuple_addition_operator(a, b); // recursion
                    }
                    else
                    {
                        return types::common_type_t<type_a, type_b>{};
                    }
                };

                return std::tuple{ tuple_opr(std::get<Indices>(tuple_a), std::get<Indices>(tuple_b))... };
            }

            template<typename... ArgTypes1, typename... ArgTypes2>
            auto tuple_addition_operator(const std::tuple<ArgTypes1...>& tuple_a,
                const std::tuple<ArgTypes2...>& tuple_b)
            {
                constexpr auto Size1 = sizeof...(ArgTypes1);
                constexpr auto Size2 = sizeof...(ArgTypes2);

                if constexpr(Size1 == Size2)
                {
                    if constexpr(Size1 == 0)
                        return tuple_a;
                    else
                        return tuple_addition(tuple_a, tuple_b, types::make_typed_sequence_t<Size1>{});
                }
                else
                {
                    return std::tuple<no_type_t>{};
                }
            }

            template<typename Type> // not tuple
            struct st_contain_no_type_in_tuple
            {
                static constexpr bool value = false;
            };
            
            template<> // empty tuple
            struct st_contain_no_type_in_tuple<std::tuple<>>
            {
                static constexpr bool value = false;
            };

            template<typename Type> // single element tuple
            struct st_contain_no_type_in_tuple<std::tuple<Type>> 
            {
                static constexpr bool value = (is_tuple_v<Type> ? st_contain_no_type_in_tuple<Type>::value : is_no_type_v<Type>);
            };

            template<typename Type, typename... Types> // tuple with 1 or more element
            struct st_contain_no_type_in_tuple<std::tuple<Type, Types...>>
            {
                static constexpr bool value = (is_tuple_v<Type> ? st_contain_no_type_in_tuple<Type>::value : is_no_type_v<Type>)
                    || st_contain_no_type_in_tuple<std::tuple<Types...>>::value;
            };

            template<typename T, typename S>
            struct st_tuple_common_type
            {
                static constexpr bool value = false;
                using type = no_type_t;
            };

            template<typename... ArgTypes1, typename... ArgTypes2>
            struct st_tuple_common_type<std::tuple<ArgTypes1...>, std::tuple<ArgTypes2...>>
            {
                using T = std::tuple<ArgTypes1...>;
                using S = std::tuple<ArgTypes2...>;

                using type =  decltype(tuple_addition_operator(std::declval<T>(), std::declval<S>()));

                static constexpr bool value = !st_contain_no_type_in_tuple<type>::value;
            };

        } // end of namespace hidden

        template<typename T, typename... Types>
        constexpr bool pairwise_common_type_v = hidden::st_pairwise_common<remove_cvref_t<T>, remove_cvref_t<Types>...>::value;

        template<typename T, typename... Types>
        using enable_pairwise_common_type_t = std::enable_if_t< pairwise_common_type_v <T, Types...> >;
        
        template<typename T, typename S>
            using tuple_common_type_t = typename hidden::st_tuple_common_type<remove_cvref_t<T>, remove_cvref_t<S>>::type;

        template<typename T, typename S>
            constexpr bool tuple_common_type_v = hidden::st_tuple_common_type<remove_cvref_t<T>, remove_cvref_t<S>>::value;

        template<typename T, typename S>
        using tuple_operation_valid_t = std::enable_if_t<tuple_common_type_v<T, S>, tuple_common_type_t<T, S>>;
        ///////////////////////////////////////

        template<typename Type, typename ReturnType = void>
        using enable_if_container_type_t = hidden::enable_if_container_type_t<Type, ReturnType>;

        template<typename... Types>
        using select_first_type_t = hidden::select_first_type_t<Types...>;

        template<typename... Types>
        using first_type_t = hidden::first_type_t<Types...>;

        template<typename... Types>
        using front_type_t = hidden::front_type_t<Types...>;

        template<typename... Types>
        using select_last_type_t = hidden::select_last_type_t<Types...>;

        template<typename... Types>
        using last_type_t = hidden::last_type_t<Types...>;

        template<typename... Types>
        using back_type_t = hidden::back_type_t<Types...>;

        template<auto SelectIndex, typename... Types>
        using select_nth_type_t = hidden::select_nth_type_t<SelectIndex, Types...>;

        template<auto SelectIndex, typename... Types>
        using nth_type_t = hidden::nth_type_t<SelectIndex, Types...>;

        template<typename Type>
        using first_parameter_type_t = hidden::first_parameter_type_t<Type>;

        template<typename Type>
        using last_parameter_type_t = hidden::last_type_t<Type>;

        template<auto SelectIndex, typename Type>
        using nth_parameter_type_t = hidden::nth_type_t<SelectIndex, Type>;

        template<typename ArgType, typename... Types>
        using push_front_type_t = hidden::push_front_type_t<ArgType, Types...>;

        template<typename ArgType, typename... Types>
        using push_back_type_t = hidden::push_back_type_t<ArgType, Types...>;

        template<typename... Types>
        using pop_front_type_t = hidden::pop_front_type_t<Types...>;

        template<typename... Types>
        using pop_back_type_t = hidden::pop_back_type_t<Types...>;

        template<size_t FirstN, typename... Types>
        using first_n_types_t = hidden::first_n_types_t<FirstN, Types...>;

        template<size_t FirstN, typename... Types>
        using select_first_n_types_t = hidden::select_first_n_types_t<FirstN, Types...>;

        template<size_t LastN, typename... Types> 
        using last_n_types_t = hidden::last_n_types_t<LastN, Types...>;

        template<size_t LastN, typename... Types> 
        using select_last_n_types_t = hidden::select_last_n_types_t<LastN, Types...>;

        template<typename ArgType, typename... Types>
        constexpr auto is_type_in_list_v = hidden::is_type_in_list_v<ArgType, Types...>;

        template<typename... Types>
        using unique_types_t = hidden::unique_types_t<Types...>;

        template<typename Type, typename... Types>
        using to_variant_t = hidden::to_variant_t<Type, Types...>;

        template<typename TupleType>
        using tuple_to_variant_t = hidden::to_variant_t<TupleType>;

        template<template<typename, typename...> class Container, typename TupleType>
        using tuple_to_container_of_variants_t = Container<types::to_variant_t<remove_cvref_t<TupleType>>>;

        template<typename... Types>
        using to_tuple_t = hidden::to_tuple_t<Types...>;

        template<typename VarType>
        using variant_to_tuple_t = hidden::to_tuple_t<VarType>;

        template<typename ArgType, typename RightList>
        using prepend_type_t = hidden::prepend_type_t<ArgType, RightList>;

        template<typename ArgType, typename RightList>
        using append_type_t = hidden::append_type_t<ArgType, RightList>;

        template<typename LeftList, typename RightList>
        using union_type_t = hidden::union_type_t<unique_types_t<LeftList>, unique_types_t<RightList>>;

        template<typename LeftList, typename RightList>
        using intersection_type_t = hidden::intersection_type_t<unique_types_t<LeftList>, unique_types_t<RightList>>;

        template<typename ArgType, typename... Types>
        using remove_type_t = hidden::remove_type_t<ArgType, Types...>;

        template<typename LeftList, typename RightList>
        using difference_type_t = hidden::difference_type_t<LeftList, RightList>;

        template<typename NewType, typename OldType, typename TypeList>
        using replace_type_t =  hidden::replace_type_t<NewType, OldType, TypeList>;

        template<typename LeftList, typename RightList>
        constexpr auto is_type_list_equivalent_v = 
            hidden::is_type_list_equivalent_v<unique_types_t<LeftList>, unique_types_t<RightList>>;

        namespace hidden
        {
            template<typename Type>
            struct st_has_tuple_common_type
            {
                static constexpr bool value = false;
            };

            template<typename... Types>
            struct st_has_tuple_common_type<std::tuple<Types...>>
            {
                using arg_types = type_list_t<Types...>;
                static constexpr bool value = common_type_v<arg_types>;
                using type = common_type_t<arg_types>;
            };

            template<typename TupleType>
            constexpr bool has_tuple_common_type_v = st_has_tuple_common_type<TupleType>::value;

            template<typename TupleType>
            using tuple_common_element_t = typename st_has_tuple_common_type<TupleType>::type;

            template<typename TupleType>
            using tuple_to_std_array_t = 
                std::array< tuple_common_element_t<TupleType>, tuple_size_v<TupleType> >;

        } // end of namespace hidden

        template<typename TupleType>
            constexpr bool has_tuple_common_type_v = hidden::has_tuple_common_type_v<remove_cvref_t<TupleType>>;

        template<typename TupleType>
            using tuple_common_element_t = hidden::tuple_common_element_t<remove_cvref_t<TupleType>>;

        template<typename TupleType>
            using tuple_to_std_array_t = hidden::tuple_to_std_array_t<remove_cvref_t<TupleType>>;

        namespace hidden
        {
            template<typename ArrayType, typename TupleType, auto...Indices>
            auto set_tuple_to_array(TupleType&& tuple, typed_sequence_t<Indices...>)
            {
                constexpr auto Size = sizeof...(Indices);

                ArrayType array;
                auto workhorse =[&array, &tuple](auto indexer)
                {
                    array[indexer.Index] = std::get<indexer.Index>(tuple);
                };

                types::drive_workhorse<Size>(workhorse);

                return array;
            }

            template<typename ContainerType, typename TupleType, auto...Indices>
            auto set_tuple_to_container(TupleType&& tuple, typed_sequence_t<Indices...>)
            {
                constexpr auto Size = sizeof...(Indices);
                ContainerType container;

                auto workhorse = [&container, &tuple](auto indexer)
                {
                    if constexpr(types::is_emplace_back_available_v<ContainerType>)
                    {
                        container.emplace_back(std::get<indexer.Index>(tuple));
                    }
                    else if constexpr(types::is_insert_available_v<ContainerType>)
                    {
                        container.insert(std::get<indexer.Index>(tuple));
                    }
                };

                types::drive_workhorse<Size>(workhorse);

                return container;
            }

            template<typename T, size_t Size, auto... Indices>
            auto set_array_to_tuple(const std::array<T, Size>& array, typed_sequence_t<Indices...>)
            {
                return std::tuple{ (array[Indices])... };   
            }

            // template<typename ContainerType, typename T, size_t Size, auto... Indices>
            // auto set_array_to_container(const std::array<T, Size>& array, typed_sequence_t<Indices...>)
            // {
            //     using ele_t = types::first_parameter_type_t<ContainerType>;

            //     return ContainerType{ ((ele_t)array[Indices])... };   
            // }
        }

        template<typename Type, typename... Types,
            auto Size = sizeof...(Types) + 1,
            typename common_type = std::common_type_t<Type, Types...>,
            typename array_type = std::array<common_type, Size>>
        auto convert_to_array(const std::tuple<Type, Types...>& tuple)
        {
            return hidden::set_tuple_to_array<array_type>(tuple, make_typed_sequence_t<Size>{});
        }

        template<template<typename, typename...>class ContainerType,
            typename Type, typename... Types,
            auto Size = sizeof...(Types) + 1,
            typename common_type = std::common_type_t<Type, Types...>,
            typename container_type = ContainerType<common_type>>
        auto convert_to_container(const std::tuple<Type, Types...>& tuple)
        {
            return hidden::set_tuple_to_container<container_type>(tuple, make_typed_sequence_t<Size>{});
        }

        template<typename ContainerType,
            typename Type, typename... Types,
            auto Size = sizeof...(Types) + 1>
        auto convert_to_container(const std::tuple<Type, Types...>& tuple)
        {
            return hidden::set_tuple_to_container<ContainerType>(tuple, make_typed_sequence_t<Size>{});
        }
        
        template<template<typename, typename...>class ContainerType,
            typename Type, typename... Types, size_t Size,
            typename container_type = ContainerType<Type, Types...>>
        auto convert_to_container(const std::array<Type, Size>& array)
        {
            // return hidden::set_array_to_container<container_type>(array, make_typed_sequence_t<Size>{});

            container_type container;

            for(size_t i = 0; i < Size; ++i)
            {
                if constexpr(types::is_emplace_back_available_v<container_type>)
                {
                    container.emplace_back(array[i]);
                }
                else if constexpr(types::is_insert_available_v<container_type>)
                {
                    container.insert(array[i]);
                }
            }

            return container;
        }

        template<typename T, size_t Size>
        auto convert_to_tuple(const std::array<T, Size>& array)
        {
            return hidden::set_array_to_tuple(array, make_typed_sequence_t<Size>{});
        }

        template<int ArgumentIndex, typename FuncType, typename ArgType, typename... ArgTypes,
            typename = std::enable_if_t< (ArgumentIndex < sizeof...(ArgTypes) + 1) >>
        auto freeze_parameter(FuncType&& func, ArgType arg, ArgTypes... args)
        {
            return [&func, arguments = std::tuple{ arg, args...}](auto&& x) mutable
            {
                std::get<ArgumentIndex>(arguments) = std::forward< decltype(x) >(x);
                
                return std::apply(func, arguments);
            };
        }

        template<int ArgumentIndex, typename FuncType, typename ArgType, typename... ArgTypes,
            typename = std::enable_if_t< (ArgumentIndex < sizeof...(ArgTypes) + 1) >>
        auto freeze_parameter(FuncType&& func, const std::tuple<ArgType, ArgTypes...>& arguments)
        {
            return [&func, arguments = arguments](auto&& x) mutable
            {
                std::get<ArgumentIndex>(arguments) = std::forward< decltype(x) >(x);
                
                return std::apply(func, arguments);
            };
        }

        template<int FuncIndex, int ArgumentIndex,
            typename FuncType, typename... FuncTypes, typename ArgFirst, typename... ArgTypes,
                typename = std::enable_if_t< ( FuncIndex < (sizeof...(FuncTypes) + 1) )
                 && (ArgumentIndex < sizeof...(ArgTypes) + 1 )> >
        auto freeze_parameter(const std::tuple<FuncType, FuncTypes...>& func_tuple, ArgFirst arg, ArgTypes... args)
        {
            return [&func = std::get<FuncIndex>(func_tuple), arguments =
                std::tuple{arg, args...}](auto&& x) mutable
            {
                std::get<ArgumentIndex>(arguments) = std::forward<>(x);

                return func(arguments);
            };
        }

        template<int FuncIndex, int ArgumentIndex,
            typename FuncType, typename... FuncTypes, typename ArgFirst, typename... ArgTypes,
            typename = std::enable_if_t< ( FuncIndex < sizeof...(FuncTypes) + 1 ) 
                && (ArgumentIndex < sizeof...(ArgTypes)+1)>>
        auto freeze_parameter(const std::tuple<FuncType, FuncTypes...>& func_tuple,
            const std::tuple<ArgFirst, ArgTypes...>& arguments)
        {
            return [&func = std::get<FuncIndex>(func_tuple), arguments = arguments](auto&& x) mutable
            {
                std::get<ArgumentIndex>(arguments) = std::forward<>(x);

                return func(arguments);
            };
        }

        namespace hidden
        {
            template< template<typename, size_t> class ReturnClass,
                  typename TupleType, auto... Indices, typename... ArgTypes>
            auto evaluate_lambdas(typed_sequence_t<Indices...>, TupleType&& tuple, ArgTypes&&... args)
            {
                return ReturnClass{ (std::get<Indices>(tuple)(std::forward<ArgTypes>(args)...))... };
            }

            template< template<typename...> class ReturnClass,
                  typename TupleType, auto... Indices, typename... ArgTypes>
            auto evaluate_lambdas(typed_sequence_t<Indices...>, TupleType&& tuple, ArgTypes&&... args)
            {
                return ReturnClass{ (std::get<Indices>(tuple)(std::forward<ArgTypes>(args)...))... };
            }

            template< template<typename, size_t> class ReturnClass,
                  typename TupleType, auto... Indices, typename ArgTuple>
            auto evaluate_lambdas_tuple(typed_sequence_t<Indices...>, TupleType&& tuple, ArgTuple&& arguments)
            {
                return ReturnClass{ std::apply(std::get<Indices>(tuple), std::forward<ArgTuple>(arguments))... };
            }

            template< template<typename...> class ReturnClass,
                  typename TupleType, auto... Indices, typename ArgTuple>
            auto evaluate_lambdas_tuple(typed_sequence_t<Indices...>, TupleType&& tuple, ArgTuple&& arguments)
            {
                return ReturnClass{ std::apply(std::get<Indices>(tuple), std::forward<ArgTuple>(arguments))... };
            }

        } // end of namespace hidden

        template< template<typename, size_t> class ReturnClass,
            typename FuncType, typename... FuncTypes, typename ArgFirst,  typename... ArgTypes>
        auto evaluate_lambdas(const std::tuple<FuncType, FuncTypes...>& tuple, ArgFirst&& arg, ArgTypes&&... args)
        {
            constexpr auto Size = sizeof...(FuncTypes) + 1;

            return hidden::evaluate_lambdas<ReturnClass>(make_typed_sequence_t<Size>{},
                tuple, std::forward<ArgFirst>(arg), std::forward<ArgTypes>(args)...);
        }

        template< template<typename...> class ReturnClass,
            typename FuncType, typename... FuncTypes, typename ArgFirst,  typename... ArgTypes>
        auto evaluate_lambdas(const std::tuple<FuncType, FuncTypes...>& tuple, ArgFirst&& arg, ArgTypes&&... args)
        {
            constexpr auto Size = sizeof...(FuncTypes) + 1;

            return hidden::evaluate_lambdas<ReturnClass>(make_typed_sequence_t<Size>{},
                tuple, std::forward<ArgFirst>(arg), std::forward<ArgTypes>(args)...);
        }

        template< template<typename, size_t> class ReturnClass,
            typename FuncType, typename... FuncTypes, typename ArgFirst>
        auto evaluate_lambdas(const std::tuple<FuncType, FuncTypes...>& tuple, ArgFirst&& arg)
        {
            constexpr auto Size = sizeof...(FuncTypes) + 1;

            if constexpr(is_tuple_v<decltype(arg)>)
            {
                return hidden::evaluate_lambdas_tuple<ReturnClass>(make_typed_sequence_t<Size>{},
                    tuple, std::forward<ArgFirst>(arg));
            }
            else
            {
                return hidden::evaluate_lambdas<ReturnClass>(make_typed_sequence_t<Size>{},
                    tuple, std::forward<ArgFirst>(arg));
            }
        }

        template< template<typename...> class ReturnClass,
            typename FuncType, typename... FuncTypes, typename ArgFirst>
        auto evaluate_lambdas(const std::tuple<FuncType, FuncTypes...>& tuple, ArgFirst&& arg)
        {
            constexpr auto Size = sizeof...(FuncTypes) + 1;

            if constexpr(is_tuple_v<decltype(arg)>)
            {
                return hidden::evaluate_lambdas_tuple<ReturnClass>(make_typed_sequence_t<Size>{},
                    tuple, std::forward<ArgFirst>(arg));
            }
            else
            {
                return hidden::evaluate_lambdas<ReturnClass>(make_typed_sequence_t<Size>{},
                    tuple, std::forward<ArgFirst>(arg));
            }
        }

        namespace hidden
        {
            template<typename... Types>
            struct arg_list{ };

            template<typename T>
            struct st_is_arg_list 
            {
                using type = T;
            };

            template<typename... Ts>
            struct st_is_arg_list<arg_list<Ts...>>
            {
                using type = type_list_t<Ts...>;
            };

            template<typename Type>
            using arg_to_type_t = typename st_is_arg_list<Type>::type;

            template<typename CallbackType, typename... ArgTypes> struct st_is_invocable;
            
            template<typename CallbackType, typename... ArgTypes>
            struct st_is_invocable<CallbackType, arg_list<ArgTypes...>>
            {
                static constexpr bool value = std::is_invocable_v<CallbackType, ArgTypes...>;
            };

        } // end of namespace hidden

        template<typename CallbackType, typename... Types>
        constexpr bool is_invocable_v = hidden::st_is_invocable<CallbackType, Types...>::value;

        namespace hidden
        {
            template<typename CallbackType, typename... Types> struct st_is_callable;
            
            template<typename CallbackType, typename... CallableTypes, typename... NonCallableTypes> 
            struct st_is_callable<CallbackType, type_list_t<CallableTypes...>, type_list_t<NonCallableTypes...>>
            {
                private:
                    static constexpr bool value = is_invocable_v<CallbackType>;

                    using callable_list = type_list_t<CallableTypes...>;
                    using non_callable_list = type_list_t<NonCallableTypes...>;
                
                public:

                using callables = std::conditional_t<value, types::push_back_type_t<void, callable_list>, callable_list>;
                using non_callables = std::conditional_t<!value, types::push_back_type_t<void, non_callable_list>, non_callable_list>;
            };

            template<typename CallbackType, typename... CallableTypes, typename... NonCallableTypes, typename Type> 
            struct st_is_callable<CallbackType, type_list_t<CallableTypes...>, type_list_t<NonCallableTypes...>, Type>
            {
                private:
                    static constexpr bool value = is_invocable_v<CallbackType, Type>;
                    using callable_list = type_list_t<CallableTypes...>;
                    using non_callable_list = type_list_t<NonCallableTypes...>;

                public:

                using callables = std::conditional_t<value, types::push_back_type_t<Type, callable_list>, callable_list>;
                using non_callables = std::conditional_t<!value, types::push_back_type_t<Type, non_callable_list>, non_callable_list>;
            };

            template<typename CallbackType, typename... CallableTypes, typename... NonCallableTypes, typename Type, typename... Types> 
            struct st_is_callable<CallbackType, type_list_t<CallableTypes...>, type_list_t<NonCallableTypes...>, Type, Types...>
            {
                private:
                    static constexpr bool value = is_invocable_v<CallbackType, Type>;

                    using callable_list = type_list_t<CallableTypes...>;
                    using non_callable_list = type_list_t<NonCallableTypes...>;

                    using callables_tmp = std::conditional_t<value, types::push_back_type_t<Type, callable_list>, callable_list>;
                    using non_callables_tmp = std::conditional_t<!value, types::push_back_type_t<Type, non_callable_list>, non_callable_list>;

                    using base_type = st_is_callable<CallbackType, callables_tmp, non_callables_tmp, Types...>;
                public:

                using callables = typename base_type::callables;
                using non_callables = typename base_type::non_callables;
            };

            template<typename CallbackType, typename... CallableTypes, typename... NonCallableTypes, typename... Types> 
            struct st_is_callable<CallbackType, type_list_t<CallableTypes...>, type_list_t<NonCallableTypes...>, type_list_t<Types...>>
            {
                using base_type = st_is_callable<CallbackType, type_list_t<CallableTypes...>, type_list_t<NonCallableTypes...>, Types...>;

                using callables = typename base_type::callables;
                using non_callables = typename base_type::non_callables;
            };

            template<typename... Types>  struct st_build_arg_types;

            template<typename... Heads, typename... Tails>
            struct st_build_arg_types<type_list_t<Heads...>, type_list_t<Tails...>>
            {
                using type = type_list_t<hidden::arg_list<Heads, Tails...>...>;
            };

            template<typename... Types> struct st_arg_to_type_list;

            template<typename... Types>
            struct st_arg_to_type_list<arg_list<Types...>>
            {
                using type = type_list_t<Types...>;
            };

            template<typename... Types>
            struct st_arg_to_type_list<type_list_t<Types...>>
            {
                using type = type_list_t<arg_to_type_t<Types>...>;
            };

        } // end of namespace hidden

        template<typename... Types>
        using arg_to_type_list_t = typename hidden::st_arg_to_type_list<Types...>::type;

        template<typename Heads, typename Tails>
        using arg_list_t = typename hidden::st_build_arg_types<Heads, Tails>::type;

        template<typename CallbackType, typename... Types>
        using callable_list_t = typename hidden::st_is_callable<CallbackType, type_list_t<>, type_list_t<>, Types...>::callables;

        template<typename CallbackType, typename... Types>
        using non_callable_list_t = typename hidden::st_is_callable<CallbackType, type_list_t<>, type_list_t<>, Types...>::non_callables;

        template<typename CallbackType, typename... Types>
        constexpr bool is_all_callable_v = types::type_count_v<callable_list_t<CallbackType, Types...>> == types::type_count_v<Types...>;

        template<typename CallbackType, typename... Types>
        constexpr bool is_any_callable_v = types::type_count_v<callable_list_t<CallbackType, Types...>> != 0;

        //////////////////////////////////////////////////////////////////////////////////////////

        template<typename... Types>
		struct concate_type_st;

        template<> struct concate_type_st<>
        {
            using type = type_list_t<>;
        };

        template<> struct concate_type_st<type_list_t<>>
        {
            using type = type_list_t<>;
        };

        template<typename Type> struct concate_type_st<Type>
		{
			using type = type_list_t<Type>;
		};

        template<typename Type> struct concate_type_st<type_list_t<Type>>
		{
			using type = type_list_t<Type>;
		};

        template<typename Type, typename... Types>
		struct concate_type_st<Type, Types...>
		{
			using type = type_list_t<Type, Types...>;
		};

        template<typename Type, typename... Types>
		struct concate_type_st<type_list_t<Type, Types...>>
		{
			using type = type_list_t<Type, Types...>;
		};

		template<typename Head, typename...Tails, typename... Types>
		struct concate_type_st<type_list_t<Head, Tails...>, Types...>
		{
			using type = type_list_t<Head, Tails..., Types...>;
		};
		
		template<typename Type, typename Head2, typename... Tails2>
		struct concate_type_st<Type, type_list_t<Head2, Tails2...>>
		{
			using type = type_list_t<Type, Head2, Tails2...>;
		};

		template<typename... Types1, typename... Types2>
			struct concate_type_st<type_list_t<Types1...>, type_list_t<Types2...>>
		{
			using type = type_list_t<Types1..., Types2...>;
		};

        template<typename... Types>
	    using concate_type_t = typename concate_type_st<Types...>::type;

        template<typename Type>
        constexpr auto is_template_template_v = hidden::is_template_template_v<Type>;

        template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
		struct type_over_set_st;

		template<bool LeftRight,
			typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
		struct type_over_set_st<boolean::Or, LeftRight, Type, BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::Or, LeftRight, Type, BinaryPredicate, ListHead>::value
				|| type_over_set_st<boolean::Or, LeftRight, Type, BinaryPredicate, ListTails...>::value;
		};

		template<bool LeftRight,
			typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
		struct type_over_set_st<boolean::And, LeftRight, Type, BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::And, LeftRight, Type, BinaryPredicate, ListHead>::value
				&& type_over_set_st<boolean::And, LeftRight, Type, BinaryPredicate, ListTails...>::value;
		};

		template<typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
		struct type_over_set_st<boolean::Or, boolean::Left, Type, BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::Or, boolean::Left, Type, BinaryPredicate, ListHead>::value
				|| type_over_set_st<boolean::Or, boolean::Left, Type, BinaryPredicate, ListTails...>::value;
		};

		template<typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
		struct type_over_set_st<boolean::And, boolean::Left, Type, BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::And, boolean::Left, Type, BinaryPredicate, ListHead>::value
				&& type_over_set_st<boolean::And, boolean::Left, Type, BinaryPredicate, ListTails...>::value;
		};

		template<typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
		struct type_over_set_st<boolean::Or, boolean::Right, Type, BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::Or, boolean::Right, Type, BinaryPredicate, ListHead>::value
				|| type_over_set_st<boolean::Or, boolean::Right, Type, BinaryPredicate, ListTails...>::value;
		};

		template<typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
		struct type_over_set_st<boolean::And, boolean::Right, Type, BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::And, boolean::Right, Type, BinaryPredicate, ListHead>::value
				&& type_over_set_st<boolean::And, boolean::Right, Type, BinaryPredicate, ListTails...>::value;
		};

		template<typename Type, template<typename, typename...> class BinaryPredicate, typename ListHead>
		struct type_over_set_st<boolean::Or, boolean::Left, Type, BinaryPredicate, ListHead>
		{
			static constexpr bool value = BinaryPredicate<Type, ListHead>::value;
		};

		template<typename Type, template<typename, typename...> class BinaryPredicate, typename ListHead>
		struct type_over_set_st<boolean::And, boolean::Left, Type, BinaryPredicate, ListHead>
		{
			static constexpr bool value = BinaryPredicate<Type, ListHead>::value;
		};

		template<typename Type, template<typename, typename...> class BinaryPredicate, typename ListHead>
		struct type_over_set_st<boolean::Or, boolean::Right, Type, BinaryPredicate, ListHead>
		{
			static constexpr bool value = BinaryPredicate<ListHead, Type>::value;
		};

		template<typename Type, template<typename, typename...> class BinaryPredicate, typename ListHead>
		struct type_over_set_st<boolean::And, boolean::Right, Type, BinaryPredicate, ListHead>
		{
			static constexpr bool value = BinaryPredicate<ListHead, Type>::value;
		};

        template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
		struct type_opr_over_set_st
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate, ListHead, ListTails...>::value;
		};

		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
		struct type_opr_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate, 
            type_list_t<ListHead, ListTails...>>
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate, ListHead, ListTails...>::value;
		};

		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename Head, typename ListHead, typename... ListTails>
		struct type_opr_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
			Head, type_list_t<ListHead, ListTails...>>
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
				Head, ListHead, ListTails...>::value;
		};

		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails, typename Head, typename... Tails>
		struct type_opr_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
			type_list_t<ListHead, ListTails...>, Head, Tails...>
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
				ListHead, ListTails..., Head, Tails...>::value;
		};

		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails, typename Head, typename... Tails>
		struct type_opr_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
			type_list_t<ListHead, ListTails...>, type_list_t<Head, Tails...>>
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
				ListHead, ListTails..., Head, Tails...>::value;
		};

        template<typename Type,
		typename ListHead, typename... ListTails>
		constexpr bool is_type_in_the_set_v = type_opr_over_set_st<boolean::Or, boolean::Left,
		    Type, std::is_same, ListHead, ListTails...>::value;

	    template<typename Type,
		typename ListHead, typename... ListTails>
		constexpr bool is_constructible_over_the_set_v = type_opr_over_set_st<boolean::Or, boolean::Right,
            Type, std::is_constructible, ListHead, ListTails...>::value;

	    template<typename Type,
		typename ListHead, typename... ListTails>
		constexpr bool is_assignable_to_the_set_v = type_opr_over_set_st<boolean::Or, boolean::Right,
		    std::add_lvalue_reference_t<Type>, std::is_assignable, ListHead, ListTails...>::value;

	    template<typename Type,
		typename ListHead, typename... ListTails>
		constexpr bool is_assignable_from_the_set_v = type_opr_over_set_st<boolean::Or, boolean::Left,
		    std::add_lvalue_reference_t<Type>, std::is_assignable, ListHead, ListTails...>::value;
	
        template<typename TestType, typename TypeList, typename ReturnType = TestType>
        using enable_if_in_list_t = hidden::enable_if_in_list_t<TestType, TypeList, ReturnType>;

        template<typename TestTypeList, typename TypeList, typename ReturnType = void>
        using enable_if_all_in_list_t = hidden::enable_if_all_in_list_t<TestTypeList, TypeList, ReturnType>;

        namespace hidden
        {
            template<template<typename, typename...> class ContainerType, typename... ElementTypes>
            class container_of_variants_class: public ContainerType<std::variant<ElementTypes...>>
            {
                public:
                    using base_type = ContainerType<std::variant<ElementTypes...>>;
                    using base_type::base_type;
                    using value_type = typename base_type::value_type;
                    using variant_type = value_type; 
                    using element_types_t = type_list_t<ElementTypes...>;

                    decltype(auto) crbegin() const noexcept
                    {
                        return base_type::rcbegin();
                    }

                    decltype(auto) crend()  const noexcept
                    {
                        return base_type::crend();
                    }
            };

            template<typename KeyType, typename... ElementTypes>
            class container_of_variants_class<std::pair, KeyType, ElementTypes...>: 
                public std::pair<KeyType, std::variant<ElementTypes...>>
            {
                public:
                    using base_type = std::pair<KeyType, std::variant<ElementTypes...>>;
                    using base_type::base_type;
                    
                    using first_type  = typename base_type::first_type;
                    using second_type = typename base_type::second_type;

                    using element_types_t = type_list_t<ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
            class container_of_variants_class<std::map, KeyType, ElementTypes...>: 
                public std::map<KeyType, std::variant<ElementTypes...>>
            {
                public:
                    using base_type = std::map<KeyType, std::variant<ElementTypes...>>;
                    using base_type::base_type;
                    using value_type = typename base_type::value_type;
                    using variant_type = value_type; 
                    using element_types_t = type_list_t<ElementTypes...>;

                    decltype(auto) crbegin() const noexcept
                    {
                        return base_type::rcbegin();
                    }

                    decltype(auto) crend()  const noexcept
                    {
                        return base_type::crend();
                    }
            };

            template<typename KeyType, typename... ElementTypes>
            class container_of_variants_class<std::multimap, KeyType, ElementTypes...>: 
                public std::multimap<KeyType, std::variant<ElementTypes...>>
            {
                public:
                    using base_type = std::multimap<KeyType, std::variant<ElementTypes...>>;
                    using base_type::base_type;
                    using value_type = typename base_type::value_type;
                    using variant_type = value_type; 
                    using element_types_t = type_list_t<ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
            class container_of_variants_class<std::unordered_map, KeyType, ElementTypes...>: 
                public std::unordered_map<KeyType, std::variant<ElementTypes...>>
            {
                public:
                    using base_type = std::unordered_map<KeyType, std::variant<ElementTypes...>>;
                    using base_type::base_type;
                    using value_type = typename base_type::value_type;
                    using variant_type = value_type; 
                    using element_types_t = type_list_t<ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
            class container_of_variants_class<std::unordered_multimap, KeyType, ElementTypes...>: 
                public std::unordered_multimap<KeyType, std::variant<ElementTypes...>>
            {
                public:
                    using base_type = std::unordered_multimap<KeyType, std::variant<ElementTypes...>>;
                    using base_type::base_type;
                    using value_type = typename base_type::value_type;
                    using variant_type = value_type; 
                    using element_types_t = type_list_t<ElementTypes...>;
            };

            template<template<typename, typename...> class ContainerType, typename... ElementTypes>
                struct container_of_variants_st
            {
                using type = container_of_variants_class<ContainerType, ElementTypes...>;
            };

            template<template<typename, typename...> class ContainerType, typename... ElementTypes>
                struct container_of_variants_st<ContainerType, type_list_t<ElementTypes...>>
            {
                using type = container_of_variants_class<ContainerType, ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
                struct container_of_variants_st<std::pair, KeyType, type_list_t<ElementTypes...>>
            {
                using type = container_of_variants_class<std::pair, KeyType, ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
                struct container_of_variants_st<std::map, KeyType, type_list_t<ElementTypes...>>
            {
                using type = container_of_variants_class<std::map, KeyType, ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
            struct container_of_variants_st<std::multimap, KeyType, type_list_t<ElementTypes...>>
            {
                using type = container_of_variants_class<std::multimap, KeyType, ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
            struct container_of_variants_st<std::unordered_map, KeyType, type_list_t<ElementTypes...>>
            {
                using type = container_of_variants_class<std::unordered_map, KeyType, ElementTypes...>;
            };
            
            template<typename KeyType, typename... ElementTypes>
            struct container_of_variants_st<std::unordered_multimap, KeyType, type_list_t<ElementTypes...>>
            {
                using type = container_of_variants_class<std::unordered_multimap, KeyType, ElementTypes...>;
            };
            
            template<template<typename, typename...> class ContainerType, typename... ElementTypes>
            using container_of_variants_t = 
                typename container_of_variants_st<ContainerType, unique_types_t<ElementTypes...>>::type; 

            template<template<typename, typename...> class ContainerType,
                typename KeyType, typename... ElementTypes>
            using container_map_of_variants_t = 
                typename container_of_variants_st<ContainerType, KeyType, unique_types_t<ElementTypes...>>::type; 

            template<typename... ElementTypes>
            using vector_of_variants_t = container_of_variants_t<std::vector, ElementTypes...>;

            template<typename... ElementTypes>
            using deque_of_variants_t = container_of_variants_t<std::deque, ElementTypes...>;

            template<typename... ElementTypes>
            using list_of_variants_t = container_of_variants_t<std::list, ElementTypes...>;

            template<typename... ElementTypes>
            using set_of_variants_t = container_of_variants_t<std::set, ElementTypes...>;

            template<typename... ElementTypes>
            using multiset_of_variants_t = container_of_variants_t<std::multiset, ElementTypes...>;

            template<typename... ElementTypes>
            using unordered_set_of_variants_t = container_of_variants_t<std::unordered_set, ElementTypes...>;

            template<typename... ElementTypes>
            using unordered_multiset_of_variants_t = container_of_variants_t<std::unordered_multiset, ElementTypes...>;

            template<typename... ElementTypes>
            using multiset_of_variants_t = container_of_variants_t<std::multiset, ElementTypes...>;

            template<typename KeyType, typename... ElementTypes>
            using pair_of_variants_t = container_map_of_variants_t<std::pair, KeyType, ElementTypes...>;

            template<typename KeyType, typename... ElementTypes>
            using map_of_variants_t = container_map_of_variants_t<std::map, KeyType, ElementTypes...>;

            template<typename KeyType, typename... ElementTypes>
            using multimap_of_variants_t = container_map_of_variants_t<std::multimap, KeyType, ElementTypes...>;

            template<typename KeyType, typename... ElementTypes>
            using unordered_map_of_variants_t = container_map_of_variants_t<std::unordered_map, KeyType, ElementTypes...>;

            template<typename KeyType, typename... ElementTypes>
            using unordered_multimap_of_variants_t = container_map_of_variants_t<std::unordered_multimap, KeyType, ElementTypes...>;

        } // end of namespace hidden

        template<typename Type1, typename Type2,
            typename common_t = common_type_t<Type1, Type2>, // get the common type of type1_t and type2_t
            bool is_integral_type = is_integral_v<common_t>, // test if common_t is of integral type
            typename result_type = std::conditional_t< 
                is_integral_type, // is common type integral?
                make_signed_t<common_t>, // if integral type, then get signed integeral type
                common_t> >
        using signed_common_t = result_type;

        template<typename Type1, typename Type2,
            typename common_t = common_type_t<Type1, Type2>, // get the common type of type1_t and type2_t
            bool is_integral_type = is_integral_v<common_t>, // test if common_t is of integral type
            typename result_type = std::conditional_t< 
                is_integral_type, // is common type integral?
                make_unsigned_t<common_t>, // if integral type, then get signed integeral type
                common_t> >
        using unsigned_common_t = result_type;

        template<template<typename, typename...> class ContainerType, typename... ElementTypes>
        using container_of_variants_t = hidden::container_of_variants_t<ContainerType, ElementTypes...>;
        
        template<typename... ElementTypes>
        using vector_of_variants_t = hidden::vector_of_variants_t<ElementTypes...>;

        template<typename... ElementTypes>
        using deque_of_variants_t = hidden::deque_of_variants_t<ElementTypes...>;

        template<typename... ElementTypes>
        using list_of_variants_t = hidden::list_of_variants_t<ElementTypes...>;

        template<typename... ElementTypes>
        using set_of_variants_t = hidden::set_of_variants_t<ElementTypes...>;

        template<typename... ElementTypes>
        using multiset_of_variants_t = hidden::multiset_of_variants_t<ElementTypes...>;

        template<typename... ElementTypes>
        using unordered_set_of_variants_t = hidden::unordered_set_of_variants_t<ElementTypes...>;

        template<typename... ElementTypes>
        using unordered_multiset_of_variants_t = hidden::unordered_multiset_of_variants_t<ElementTypes...>;

        template<typename... ElementTypes>
        using multiset_of_variants_t = hidden::multiset_of_variants_t<ElementTypes...>;

        template<typename KeyType, typename... ElementTypes>
        using map_of_variants_t = hidden::map_of_variants_t<KeyType, ElementTypes...>;

        template<typename KeyType, typename... ElementTypes>
        using multimap_of_variants_t = hidden::multimap_of_variants_t<KeyType, ElementTypes...>;

        template<typename KeyType, typename... ElementTypes>
        using unordered_map_of_variants_t = hidden::unordered_map_of_variants_t<KeyType, ElementTypes...>;

        template<typename KeyType, typename... ElementTypes>
        using unordered_multimap_of_variants_t = hidden::unordered_multimap_of_variants_t<KeyType, ElementTypes...>;

        ////////////////////////////////////////////////////////////////
        namespace hidden
        {
            template<typename Type, typename... Types> 
            struct types_to_template_class_st
            {
                using type = no_type_t;
            };

            template<template<typename...>class ClassTemplate, 
                typename... InnerTypes, typename... Types>
            struct types_to_template_class_st<ClassTemplate<InnerTypes...>, Types...>
            {
                using type = ClassTemplate<Types...>;
            };

            template<template<typename...>class ClassTemplate, 
                typename... InnerTypes, typename... Types>
            struct types_to_template_class_st<ClassTemplate<InnerTypes...>,
                type_list_t<Types...>>
            {
                using type = ClassTemplate<Types...>;
            };

            template<typename Type, typename... Types> 
            struct types_to_template_class_wrapper_st
            {
                using type = typename 
                    types_to_template_class_st<Type, Types...>::type;
            };

            template<template<typename...>class ClassTemplate, 
                typename... InnerTypes, typename... Types>
            struct types_to_template_class_wrapper_st<ClassTemplate<InnerTypes...>, Types...>
            {
                using type = typename 
                    types_to_template_class_st<ClassTemplate<InnerTypes...>, Types...>::type;
            };

            template<typename... InnerTypes, typename... Types>
            struct types_to_template_class_wrapper_st<std::variant<InnerTypes...>, Types...>
            {
                using type = typename 
                    types_to_template_class_st<std::variant<InnerTypes...>, unique_types_t<Types...>>::type;
            };
            
            template<template<typename...>class ClassTemplate, 
                typename... InnerTypes, typename... Types>
            struct types_to_template_class_wrapper_st<ClassTemplate<InnerTypes...>,
                type_list_t<Types...>>
            {
                using type = 
                    typename types_to_template_class_st<ClassTemplate<InnerTypes...>, Types...>::type;
            };

            template<typename... InnerTypes, typename... Types>
            struct types_to_template_class_wrapper_st<std::variant<InnerTypes...>, type_list_t<Types...>>
            {
                using type = 
                    typename types_to_template_class_st<std::variant<InnerTypes...>, unique_types_t<Types...>>::type;
            };

            template<typename Type, typename... Types>
            using types_to_template_class_t = 
                typename types_to_template_class_wrapper_st<Type, Types...>::type;

            template<template<typename...>class ClassTemplate, typename... Types>
            struct type_list_to_template_class_st
            {
                using type = ClassTemplate<Types...>;
            };

            template<template<typename...>class ClassTemplate, typename... Types>
            struct type_list_to_template_class_st<ClassTemplate, type_list_t<Types...>>
            {
                using type = ClassTemplate<Types...>;
            };

            template<template<typename...>class ClassTemplate, typename... Types>
            struct type_list_to_template_class_wrapper_st
            {
                using type = typename type_list_to_template_class_st<ClassTemplate, Types...>::type;
            };

            template<template<typename...>class ClassTemplate, typename... Types>
            struct type_list_to_template_class_wrapper_st<ClassTemplate, type_list_t<Types...>>
            {
                using type = typename type_list_to_template_class_st<ClassTemplate, Types...>::type;
            };

            template<typename... Types>
            struct type_list_to_template_class_wrapper_st<std::variant, Types...>
            {
                using type = typename type_list_to_template_class_st<std::variant,
                        unique_types_t<Types...>>::type;
            };

            template<typename... Types>
            struct type_list_to_template_class_wrapper_st<std::variant, type_list_t<Types...>>
            {
                using type = typename type_list_to_template_class_st<std::variant,
                        unique_types_t<Types...>>::type;
            };

            template<template<typename...>class ClassTemplate, typename... Types>
            using type_list_to_template_class_t = 
                typename type_list_to_template_class_wrapper_st<ClassTemplate, Types...>::type;

            //////////////////////////////////////////////////////////////////////////
            template<size_t TypeIndex, typename TemplateClass, typename... Types>
            struct nth_type_to_template_class_st;

            template<size_t TypeIndex, 
                template<typename...> class TemplateClass,
                typename... InnerTypes, typename... Types> 
            struct nth_type_to_template_class_st<TypeIndex, TemplateClass<InnerTypes...>, Types...>
            {
                static_assert(TypeIndex < sizeof...(Types), "TypeIndex is out of range");

                using nth_type = select_nth_type_t<TypeIndex, type_list_t<Types...>>;               
                using type = std::conditional_t<is_valid_type_v<nth_type>, TemplateClass<nth_type>, nth_type>;
            };

            template<size_t TypeIndex, 
                template<typename...> class TemplateClass,
                typename... InnerTypes, typename... Types> 
            struct nth_type_to_template_class_st<TypeIndex, TemplateClass<InnerTypes...>, type_list_t<Types...>>
            {
                static_assert(TypeIndex < sizeof...(Types), "TypeIndex is out of range");
                
                using nth_type = select_nth_type_t<TypeIndex, type_list_t<Types...>>;
                using type = std::conditional_t<is_valid_type_v<nth_type>, TemplateClass<nth_type>, nth_type>;
            };
            
            template<size_t TypeIndex, typename TemplateClass, typename... Types>
            using nth_type_to_template_class_t = 
                typename nth_type_to_template_class_st<TypeIndex, TemplateClass, Types...>::type;

            ////////////////////////////////////////////////////////////////////////////////////////
            template<size_t FirstN,
                template<typename...> typename TemplateClass, typename... Types> 
            struct first_n_types_list_to_template_class_st
            {
                static_assert(FirstN <= sizeof...(Types), "FirstN is out of range");
                using n_types = first_n_types_t<FirstN, type_list_t<Types...>>;
                
                using class_t = std::conditional_t<is_valid_type_v<n_types>, 
                    type_list_to_template_class_t<TemplateClass, n_types>, no_type_t>;

                using type = std::conditional_t<is_valid_type_v<class_t>, class_t, no_type_t>;
            };

            template<size_t FirstN, template<typename...> class TemplateClass, typename... Types> 
            struct first_n_types_list_to_template_class_st<FirstN, TemplateClass, type_list_t<Types...>>
            {
                static_assert(FirstN <= sizeof...(Types), "FirstN is out of range");
                using n_types = first_n_types_t<FirstN, type_list_t<Types...>>;
                
                using class_t = std::conditional_t<is_valid_type_v<n_types>, 
                    type_list_to_template_class_t<TemplateClass, n_types>, no_type_t>;

                using type = std::conditional_t<is_valid_type_v<class_t>, class_t, no_type_t>;
            };
            
            template<size_t FirstN, template<typename...> typename TemplateClass, typename... Types>
            using first_n_types_list_to_template_class_t = 
                typename first_n_types_list_to_template_class_st<FirstN, TemplateClass, Types...>::type;

            /////////////////////////////////////////////////////////////////////////////////////////
            template<size_t FirstN, typename TemplateClass, typename... Types>
            struct first_n_types_to_template_class_st;

            template<size_t FirstN, 
                template<typename...> class TemplateClass,
                typename... InnerTypes, typename... Types> 
            struct first_n_types_to_template_class_st<FirstN, TemplateClass<InnerTypes...>, Types...>
            {
                static_assert(FirstN <= sizeof...(Types), "FirstN is out of range");
                using n_types = first_n_types_t<FirstN, type_list_t<Types...>>;
                
                using class_t = std::conditional_t<is_valid_type_v<n_types>, 
                    type_list_to_template_class_t<TemplateClass, n_types>, no_type_t>;

                using type = std::conditional_t<is_valid_type_v<class_t>, class_t, no_type_t>;
            };

            template<size_t FirstN, 
                template<typename...> class TemplateClass,
                typename... InnerTypes, typename... Types> 
            struct first_n_types_to_template_class_st<FirstN, TemplateClass<InnerTypes...>, type_list_t<Types...>>
            {
                static_assert(FirstN <= sizeof...(Types), "FirstN is out of range");
                using n_types = first_n_types_t<FirstN, type_list_t<Types...>>;
                
                using class_t = std::conditional_t<is_valid_type_v<n_types>, 
                    type_list_to_template_class_t<TemplateClass, n_types>, no_type_t>;

                using type = std::conditional_t<is_valid_type_v<class_t>, class_t, no_type_t>;
            };
            
            template<size_t FirstN, typename TemplateClass, typename... Types>
            using first_n_types_to_template_class_t = 
                typename first_n_types_to_template_class_st<FirstN, TemplateClass, Types...>::type;
                
            //////////////////////////////////////////////////


            template<template<typename, typename...> class ContainerType, typename... ElementTypes>
            class container_of_tuples_class : public ContainerType< std::tuple<ElementTypes...> >
            {
                public:

                using base_type = ContainerType< std::tuple<ElementTypes...> >;
                using base_type::base_type;

                using value_type = typename base_type::value_type;
                using tuple_type = value_type; 
                
                using element_types_t = type_list_t<ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
            class container_of_tuples_class<std::map, KeyType, ElementTypes...> 
                : public std::map<KeyType, std::tuple<ElementTypes...>>
            {
                public:

                using base_type = std::map<KeyType, std::tuple<ElementTypes...>>;
                using base_type::base_type;

                using value_type = typename base_type::value_type;
                using tuple_type = std::tuple<ElementTypes...>; 
                
                using element_types_t = type_list_t<ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
            class container_of_tuples_class<std::multimap, KeyType, ElementTypes...> 
                : public std::multimap<KeyType, std::tuple<ElementTypes...>>
            {
                public:

                using base_type = std::multimap<KeyType, std::tuple<ElementTypes...>>;
                using base_type::base_type;

                using value_type = typename base_type::value_type;
                using tuple_type = std::tuple<ElementTypes...>; 
                
                using element_types_t = type_list_t<ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
            class container_of_tuples_class<std::unordered_map, KeyType, ElementTypes...> 
                : public std::unordered_map<KeyType, std::tuple<ElementTypes...>>
            {
                public:

                using base_type = std::unordered_map<KeyType, std::tuple<ElementTypes...>>;
                using base_type::base_type;

                using value_type = typename base_type::value_type;
                using tuple_type = std::tuple<ElementTypes...>; 
                
                using element_types_t = type_list_t<ElementTypes...>;
            };

            template<typename KeyType, typename... ElementTypes>
            class container_of_tuples_class<std::unordered_multimap, KeyType, ElementTypes...> 
                : public std::unordered_multimap<KeyType, std::tuple<ElementTypes...>>
            {
                public:

                using base_type = std::unordered_multimap<KeyType, std::tuple<ElementTypes...>>;
                using base_type::base_type;

                using value_type = typename base_type::value_type;
                using tuple_type = std::tuple<ElementTypes...>; 
                
                using element_types_t = type_list_t<ElementTypes...>;
            };

            template<template<typename, typename...> class ContainerType, typename... ElementTypes>
                struct container_of_tuples_st
            {
                using type = container_of_tuples_class<ContainerType, ElementTypes...>;
            };

            template<template<typename, typename...> class ContainerType, typename... ElementTypes>
                struct container_of_tuples_st<ContainerType, type_list_t<ElementTypes...>>
            {
                using type = container_of_tuples_class<ContainerType, ElementTypes...>;
            };

            template<template<typename, typename...> class ContainerType, typename... ElementTypes>
            using container_of_tuples_t = typename container_of_tuples_st<ContainerType, ElementTypes...>::type;

            template<typename... ElementTypes>
            using vector_of_tuples_t = container_of_tuples_t<std::vector, ElementTypes...>;

            template<typename... ElementTypes>
            using deque_of_tuples_t = container_of_tuples_t<std::deque, ElementTypes...>;

            template<typename... ElementTypes>
            using list_of_tuples_t = container_of_tuples_t<std::list, ElementTypes...>;

            template<typename... ElementTypes>
            using set_of_tuples_t = container_of_tuples_t<std::set, ElementTypes...>;

            template<typename KeyType, typename... ElementTypes>
            using map_of_tuples_t = container_of_tuples_t<std::map, KeyType, ElementTypes...>;

            template<typename KeyType, typename... ElementTypes>
            using multimap_of_tuples_t = container_of_tuples_t<std::multimap, KeyType, ElementTypes...>;

            template<typename KeyType, typename... ElementTypes>
            using unordered_map_of_tuples_t = container_of_tuples_t<std::unordered_map, KeyType, ElementTypes...>;

            template<typename KeyType, typename... ElementTypes>
            using unordered_multimap_of_tuples_t = container_of_tuples_t<std::unordered_multimap, KeyType, ElementTypes...>;

        } // end of namespace hidden

        template<typename Type, typename... Types>
        using types_to_template_class_t = hidden::types_to_template_class_t<Type, Types...>;

        template<template<typename...> class ClassTemplate, typename... Types>
        using type_list_to_template_class_t = hidden::type_list_to_template_class_t<ClassTemplate, Types...>;

        template<size_t SelectTypeIndex, typename TemplateClass, typename... Types>
        using nth_type_to_template_class_t = 
            hidden::nth_type_to_template_class_t<SelectTypeIndex, TemplateClass, Types...>;

        template<size_t FirstN, typename TemplateClass, typename... Types>
        using first_n_types_to_template_class_t = 
            hidden::first_n_types_to_template_class_t<FirstN, TemplateClass, Types...>;

        template<size_t FirstN, template<typename...> typename TemplateClass, typename... Types>
        using first_n_types_list_to_template_class_t = 
            hidden::first_n_types_list_to_template_class_t<FirstN, TemplateClass, Types...>;

        template<template<typename, typename...> class ContainerType, typename... ElementTypes>
        using container_of_tuples_t = hidden::container_of_tuples_t<ContainerType, ElementTypes...>;

        template<typename... ElementTypes>
        using vector_of_tuples_t = hidden::vector_of_tuples_t<ElementTypes...>;

        template<typename... ElementTypes>
        using deque_of_tuples_t = hidden::deque_of_tuples_t<ElementTypes...>;

        template<typename... ElementTypes>
        using list_of_tuples_t = hidden::list_of_tuples_t<ElementTypes...>;

        template<typename... ElementTypes>
        using set_of_tuples_t = hidden::set_of_tuples_t<ElementTypes...>;

        template<typename KeyType, typename... ElementTypes>
        using map_of_tuples_t = hidden::map_of_tuples_t<KeyType, ElementTypes...>;

        template<typename KeyType, typename... ElementTypes>
        using multimap_of_tuples_t = hidden::multimap_of_tuples_t<KeyType, ElementTypes...>;

        template<typename KeyType, typename... ElementTypes>
        using unordered_map_of_tuples_t = hidden::unordered_map_of_tuples_t<KeyType, ElementTypes...>;

        template<typename KeyType, typename... ElementTypes>
        using unordered_multimap_of_tuples_t = hidden::unordered_multimap_of_tuples_t<KeyType, ElementTypes...>;

        template<template<typename, typename...> class ContainerType,
            typename EleType, typename... Types>
        auto erase(ContainerType<EleType, Types...>& container, size_t index)
        {
            auto itr = container.begin();
            std::advance(itr, index);
            return container.erase(itr);
        }

        template<template<typename, typename...> class ContainerType, typename Type, typename... Types>
        auto pop_front(ContainerType<Type, Types...>& container)
        {
            if(container.empty())
            {
                Tpf_ThrowDebugException("container empty");
            }

            if constexpr (is_front_available_v<decltype(container)>)
            {
                auto front = container.front();

                if constexpr(is_pop_front_available_v<decltype(container)>)
                    container.pop_front();
                else
                {
                    auto front_itr = container.begin();
                    container.erase(front_itr);
                }
                
                return front;
            }
            else
            {
                if constexpr (is_pop_front_available_v<decltype(container)>)
                {
                    auto front = *container.begin();
                    container.pop_front();
                    return front;
                }
                else
                {
                    auto front_itr = container.begin();
                    auto front = *front_itr;
                    container.erase(front_itr);
                    return front;
                }
            }
        }

        template<template<typename, typename...> class ContainerType, typename Type, typename... Types>
        auto pop_back(ContainerType<Type, Types...>& container)
        {
            if(container.empty())
            {
                Tpf_ThrowDebugException("container empty");
            }

            auto back = container.back();
            container.pop_back();
            return back;
        }
        
        template<template<typename, typename...> class ContainerType,
            typename EleType, typename Type, typename... Types>
        void push_front(ContainerType<Type, Types...>& container, EleType&& ele)
        {
            if constexpr(is_push_front_available_v<decltype(container)>)
            {
                container.push_front(std::forward<EleType>(ele));
            }
            else if constexpr(is_insert_iterator_available_v<decltype(container)>)
            {
                container.insert(container.cbegin(), std::forward<EleType>(ele));
            }
            else if constexpr(is_insert_available_v<decltype(container)>)
            {
                 container.insert(std::forward<EleType>(ele));
            }
            else
            {
                Tpf_ThrowDebugException("Cannot push front");
            }
        }

        template<template<typename, typename...> class ContainerType,
        typename EleType, typename Type, typename... Types>
        void push_back(ContainerType<Type, Types...>& container, EleType&& ele)
        {
            if constexpr(is_push_back_available_v<decltype(container)>)
            {
                container.push_back(std::forward<EleType>(ele));
            }
            else if constexpr(is_insert_iterator_available_v<decltype(container)>)
            {
                container.insert(container.cend(), std::forward<EleType>(ele));
            }
            else if constexpr(is_insert_available_v<decltype(container)>)
            {
                container.insert(std::forward<EleType>(ele));
            }
            else
            {
                Tpf_ThrowDebugException("Cannot push back");
            }
        }

        template<template<typename, typename...> class ContainerType,
            typename... EleTypes, typename Type, typename... Types>
        void emplace_front(ContainerType<Type, Types...>& container, EleTypes&&... eles)
        {
            if constexpr(is_emplace_front_available_v<decltype(container)>)
            {
                container.emplace_front(std::forward<EleTypes>(eles)...);
            }
            else if constexpr(is_emplace_available_v<decltype(container)>)
            {
                container.emplace(container.cbegin(), std::forward<EleTypes>(eles)...);
            }
            else
            {
                container.insert(container.cbegin(), std::forward<EleTypes>(eles)...);
            }
        }

        template<template<typename, typename...> class ContainerType,
        typename... EleTypes, typename Type, typename... Types>
        void emplace_back(ContainerType<Type, Types...>& container, EleTypes&&... eles)
        {
            if constexpr(is_emplace_back_available_v<decltype(container)>)
            {
                container.emplace_back(std::forward<EleTypes>(eles)...);
            }
            else if constexpr(is_emplace_available_v<decltype(container)>)
            {
                container.emplace_back(container.cend(), std::forward<EleTypes>(eles)...);
            }
            else
            {
                container.insert(container.cend(), std::forward<EleTypes>(eles)...);
            }
        }

        template<template<typename, typename...> class ContainerType, typename Type, typename... Types>
        ContainerType<Type, Types...> reverse_order(const ContainerType<Type, Types...>& container)
        {
            return {container.crbegin(), container.crend()};
        }

        template<template<typename, typename...> class ContainerType, typename Type, typename... Types>
        void reverse_order_in_place(ContainerType<Type, Types...>& container)
        {
            container = ContainerType<Type, Types...>{container.crbegin(), container.crend()};
        }
        
        template<bool bReverseOrder, typename TargetContainerType, typename SourceContainerType>
        void append_to_container(TargetContainerType& target_container, SourceContainerType&& source_container)
        {
            if constexpr(std::is_rvalue_reference_v<decltype(source_container)>)
            {
                if constexpr (bReverseOrder)
                {
                    target_container.insert(target_container.cend(), 
                        std::make_move_iterator(source_container.rbegin()),
                        std::make_move_iterator(source_container.rend()));
                }
                else
                {
                    target_container.insert(target_container.cend(), 
                        std::make_move_iterator(source_container.begin()),
                        std::make_move_iterator(source_container.end()));
                }
            }
            else
            {
                if constexpr(bReverseOrder)
                {
                    target_container.insert(target_container.cend(), 
                        source_container.crbegin(),
                        source_container.crend());
                }
                else
                {
                    target_container.insert(target_container.cend(), 
                        source_container.cbegin(),
                        source_container.cend());
                }
            }
        }

        template<bool bReverseOrder, typename TargetContainerType, typename SourceContainerType>
        void prepend_to_container(TargetContainerType& target_container, SourceContainerType&& source_container)
        {
            if constexpr(std::is_rvalue_reference_v<decltype(source_container)>)
            {
                if constexpr(bReverseOrder)
                {
                    target_container.insert(target_container.cbegin(), 
                        std::make_move_iterator(source_container.rbegin()),
                        std::make_move_iterator(source_container.rend()));
                }
                else
                {
                    target_container.insert(target_container.cbegin(), 
                        std::make_move_iterator(source_container.begin()),
                        std::make_move_iterator(source_container.end()));
                }
            }
            else
            {
                if constexpr (bReverseOrder)
                {
                    target_container.insert(target_container.cbegin(), 
                        source_container.crbegin(),
                        source_container.crend());
                }
                else
                {
                    target_container.insert(target_container.cbegin(), 
                        source_container.cbegin(),
                        source_container.cend());
                }
            }
        }
        
        template<template<typename, typename...> class ContainerType, typename Type, typename... Types>
        auto make_random_access_container(Type&& arg, Types&&... args)
        {
            using parameter_types = decay_remove_cv_ref_type_list_t<Type, Types...>;
            using common_type = common_type_t<parameter_types>;

            static_assert(common_type_v<parameter_types>, "Common Type Does Not Exist");

            if constexpr(is_same_v<parameter_types>)
            {
                using container_t = ContainerType<common_type>;
                
                return container_t{ std::forward<Type>(arg), std::forward<Types>(args)... };
            }
            else if constexpr(tpf::types::is_integral_v<common_type>)
            {
                // test if all types are of unsigned integral types
                if constexpr (are_unsigned_integrals_v<parameter_types>)
                {
                    using container_t = ContainerType<common_type>;
                                    
                    return container_t{ static_cast<common_type>(std::forward<Type>(arg) ), 
                            static_cast<common_type>( std::forward<Types>(args) )...};
                }
                else
                {
                    using element_t = std::make_signed_t<common_type>;
                    using container_t = ContainerType<element_t>;
                                    
                    return container_t{ 
                        static_cast<element_t>(std::forward<Type>(arg)), 
                        static_cast<element_t>(std::forward<Types>(args))... };
                }
            }
            else 
            {
                using element_t = common_type;
                using container_t = ContainerType<element_t>;
                
                return container_t{ static_cast<element_t>(std::forward<Type>(arg)), 
                                    static_cast<element_t>(std::forward<Types>(args))... };
            }
        }

        template<typename Type, typename... Types>
        auto make_vector(Type&& arg, Types&&... args)
        {
            using parameter_types = decay_remove_cv_ref_type_list_t<Type, Types...>;
            using common_type = common_type_t<parameter_types>;

            static_assert(common_type_v<parameter_types>, "Common Type Does Not Exist");

            if constexpr(is_same_v<parameter_types>)
            {
                using container_t = std::vector<common_type>;
                
                return container_t{ std::forward<Type>(arg), std::forward<Types>(args)... };
            }
            else if constexpr(tpf::types::is_integral_v<common_type>)
            {
                // test if all types are of unsigned integral types
                if constexpr (are_unsigned_integrals_v<parameter_types>)
                {
                    using container_t = std::vector<common_type>;
                                    
                    return container_t{ static_cast<common_type>(std::forward<Type>(arg) ), 
                            static_cast<common_type>( std::forward<Types>(args) )...};
                }
                else
                {
                    using element_t = std::make_signed_t<common_type>;
                    using container_t = std::vector<element_t>;
                                    
                    return container_t{ 
                        static_cast<element_t>(std::forward<Type>(arg)), 
                        static_cast<element_t>(std::forward<Types>(args))... };
                }
            }
            else 
            {
                using element_t = common_type;
                using container_t = std::vector<element_t>;
                
                return container_t{ static_cast<element_t>(std::forward<Type>(arg)), 
                                    static_cast<element_t>(std::forward<Types>(args))... };
            }
        }

        template<typename Type, typename... Types>
        auto make_deque(Type&& arg, Types&&... args)
        {
            using parameter_types = decay_remove_cv_ref_type_list_t<Type, Types...>;
            using common_type = common_type_t<parameter_types>;

            static_assert(common_type_v<parameter_types>, "Common Type Does Not Exist");

            if constexpr(is_same_v<parameter_types>)
            {
                using container_t = std::deque<common_type>;
                
                return container_t{ std::forward<Type>(arg), std::forward<Types>(args)... };
            }
            else if constexpr(tpf::types::is_integral_v<common_type>)
            {
                // test if all types are of unsigned integral types
                if constexpr (are_unsigned_integrals_v<parameter_types>)
                {
                    using container_t = std::deque<common_type>;
                                    
                    return container_t{ static_cast<common_type>(std::forward<Type>(arg) ), 
                            static_cast<common_type>( std::forward<Types>(args) )...};
                }
                else
                {
                    using element_t = std::make_signed_t<common_type>;
                    using container_t = std::deque<element_t>;
                                    
                    return container_t{ 
                        static_cast<element_t>(std::forward<Type>(arg)), 
                        static_cast<element_t>(std::forward<Types>(args))... };
                }
            }
            else 
            {
                using element_t = common_type;
                using container_t = std::deque<element_t>;
                
                return container_t{ static_cast<element_t>(std::forward<Type>(arg)), 
                                    static_cast<element_t>(std::forward<Types>(args))... };
            }
        }

        template<typename Type, typename... Types>
        auto make_container(Type&& arg, Types&&... args)
        {
            using parameter_types = decay_remove_cv_ref_type_list_t<Type, Types...>;
            
            if constexpr(is_same_v<parameter_types> || common_type_v<parameter_types>)
            {
                return make_vector(std::forward<Type>(arg), std::forward<Types>(args)...);
            }
            else
            {
                return std::make_tuple(std::forward<Type>(arg), std::forward<Types>(args)...);
            }
        }

        #if !defined(__clang_major__)

        template<typename Type, typename... Types>
        auto make_variants(Type&& arg, Types&&... args)
        {
            using parameter_types = decay_remove_cv_ref_type_list_t<Type, Types...>;

            if constexpr(is_same_v<parameter_types> || common_type_v<parameter_types>)
            {
                return make_vector(std::forward<Type>(arg), std::forward<Types>(args)...);
            }
            else
            {
                using element_t = types::to_variant_t<parameter_types>;
              
                return std::vector<element_t>{ element_t{ std::forward<Type>(arg) },
                   element_t{ std::forward<Types>(args) }... };
            }
        }
        
        #endif
        
        template<typename ContainerType>
        class reverse_st
        {
            private:
                ContainerType m_container;
            
            public:

            reverse_st(ContainerType container): 
                m_container{ container } { }

            auto empty() const noexcept { return m_container.empty(); }
            auto size() const noexcept { return m_container.size(); }

            decltype(auto) begin() noexcept //(noexcept(m_container.rbegin()))
            {
                static_assert(!is_const_reference_v<ContainerType>, "--- use const auto&");

                return m_container.rbegin();
            }

            decltype(auto) end() noexcept // (noexcept(m_container.rend()))
            {
                static_assert(!is_const_reference_v<ContainerType>, "--- use const auto&");
                return m_container.rend();
            }

            decltype(auto) cbegin() const noexcept //(noexcept(m_container.crbegin()))
            {
                return m_container.crbegin();
            }

            decltype(auto) cend() const noexcept // (noexcept(m_container.crend()))
            {
                return m_container.crend();
            }

            auto clone() const
            {
                using container_type = tpf::remove_cv_ref_t<ContainerType>;
                
                return container_type{ m_container.crbegin(), m_container.crend() };
            }
        };

        template<typename ElementType, size_t ElementCount>
        class reverse_st<ElementType(&)[ElementCount]>
        {
            private:
                using ContainerType = ElementType(&)[ElementCount];
                
                ContainerType m_container;
            public:

                auto empty() const noexcept { return false; }
                auto size() const noexcept { return ElementCount; }

                reverse_st(ContainerType container): m_container{container} { }

                using reverse_iterator = std::reverse_iterator<ElementType*>;
                                   
                decltype(auto) begin() noexcept
                {
                    return reverse_iterator{&m_container[ElementCount]};
                }

                decltype(auto) end() noexcept
                {
                    return reverse_iterator{&m_container[0]};
                }
                
                decltype(auto) cbegin() const noexcept
                {
                    return reverse_iterator{&m_container[ElementCount]};
                }

                decltype(auto) cend() const noexcept
                {
                    return reverse_iterator{&m_container[0]};
                }

                auto clone() const
                {
                    using element_t = std::remove_cv_t<ElementType>;
                    return std::vector<element_t>{ cbegin(), cend() };
                }
        };

        template<typename ContainerType>
        auto reverse(ContainerType&& container)
        {
            using reserve_t = reverse_st<ContainerType>;

            return reserve_t{ std::forward<ContainerType>(container) };
        }
        
        template<typename ElementType, size_t ElementCount> 
        auto reverse(ElementType(&array)[ElementCount])
        {
            using array_type = ElementType(&)[ElementCount];

            using reverse_t = reverse_st<array_type>;
            
            return reverse_t{array};
        }

        template<typename Type, typename... Types>
        auto reverse(Type&& arg, Types&&... args)
        {
            using parameter_types = decay_remove_cv_ref_type_list_t<Type, Types...>;

            static_assert(common_type_v<parameter_types>, "Common Type Does Not Exist");

            auto v = make_vector(std::forward<Type>(arg), std::forward<Types>(args)...);

            return reverse_st<decltype(v)>{ std::move(v) };
        }

        template<typename ContainerType, typename IndexType>
        decltype(auto) get_element(ContainerType container, IndexType index)
        {
            using container_t = tpf::remove_cv_ref_t<ContainerType>;

            if constexpr(tpf::types::is_index_operator_available_v<container_t>)
            {
                return container[(size_t)index];
            }
            else
            {
                if constexpr(std::is_const_v<ContainerType>)
                {
                    auto itr = container.cbegin();
                    std::advance(itr, index);
                    return *itr; 
                }
                else
                {
                    auto itr = container.begin();
                    std::advance(itr, index);
                    return *itr; 
                }
            }
        }

        template<size_t StartIndex, size_t EndIndex>
        struct static_loop_struct
        {
            /////////////////////////////////////////////////////
            template<typename VisitorType, typename PairType>
            static std::enable_if_t<is_pair_of_variant_v<remove_cv_ref_t<PairType>>>       
            visit_variant(VisitorType&& visitors, PairType&& vpr)
            {
                if constexpr(StartIndex < EndIndex)
                {
                    auto& [key, vt] = vpr;
                    
                    if(auto ptr = std::get_if<StartIndex>(&vt))
                    {
                        std::get<StartIndex>(visitors.m_visitors)(key, *ptr); 
                        return;
                    }
                }

                if constexpr(StartIndex + 1 < EndIndex)
                {
                    static_loop_struct<StartIndex+1, EndIndex>:: 
                        template visit_variant(std::forward<VisitorType>(visitors),
                            std::forward<PairType>(vpr));
                }
            }

            template<typename VisitorType, typename VariantType>
            static std::enable_if_t<is_variant_v<remove_cv_ref_t<VariantType>>>
            visit_variant(VisitorType&& visitors, VariantType&& vpr)
            {
                if constexpr(StartIndex < EndIndex)
                {
                    if(auto ptr = std::get_if<StartIndex>(&vpr))
                    {
                        std::get<StartIndex>(visitors.m_visitors)(*ptr);
                        return;
                    }
                }

                if constexpr(StartIndex + 1 < EndIndex)
                {
                    static_loop_struct<StartIndex+1, EndIndex>:: 
                        template visit_variant(std::forward<VisitorType>(visitors),
                            std::forward<VariantType>(vpr));
                }
            }

            //////////////////////////////////////////////
            /////////////////////////////////////////////////////
            template<typename VisitorType, typename IteratorType, typename PairType>
            static std::enable_if_t<is_pair_of_variant_v<remove_cv_ref_t<PairType>>>       
            visit_variant(VisitorType&& visitors, IteratorType&& itr, PairType&& vpr)
            {
                if constexpr(StartIndex < EndIndex)
                {
                    auto& [key, vt] = vpr;
                    
                    if(auto ptr = std::get_if<StartIndex>(&vt))
                    {
                        std::get<StartIndex>(visitors.m_visitors)(std::forward<IteratorType>(itr), key, *ptr); 
                        return;
                    }
                }

                if constexpr(StartIndex + 1 < EndIndex)
                {
                    static_loop_struct<StartIndex+1, EndIndex>:: 
                        template visit_variant(std::forward<VisitorType>(visitors),
                        std::forward<IteratorType>(itr), std::forward<PairType>(vpr));
                }
            }

            template<typename VisitorType, typename IteratorType, typename VariantType>
            static std::enable_if_t<is_variant_v<remove_cv_ref_t<VariantType>>>
            visit_variant(VisitorType&& visitors, IteratorType&& itr, VariantType&& vpr)
            {
                if constexpr(StartIndex < EndIndex)
                {
                    if(auto ptr = std::get_if<StartIndex>(&vpr))
                    {
                        std::get<StartIndex>(visitors.m_visitors)(std::forward<IteratorType>(itr), *ptr);
                        return;
                    }
                }

                if constexpr(StartIndex + 1 < EndIndex)
                {
                    static_loop_struct<StartIndex+1, EndIndex>:: 
                        template visit_variant(std::forward<VisitorType>(visitors),
                        std::forward<IteratorType>(itr), std::forward<VariantType>(vpr));
                }
            }
            
        }; // end of static_loop_struct
        
        template<typename VisitorType, typename PairType>
        std::enable_if_t<is_pair_of_variant_v<remove_cv_ref_t<PairType>>> 
        visit_variant(VisitorType&& visit, PairType&& vpr)
        {
            using pair_t = remove_cv_ref_t<PairType>;
            using second_type = typename pair_t::second_type;

            static_loop_struct<0, std::variant_size_v<second_type>>:: template
                visit_variant(std::forward<VisitorType>(visit), std::forward<PairType>(vpr));
        }

        template<typename VisitorType, typename VariantType>
        std::enable_if_t<is_variant_v<remove_cv_ref_t<VariantType>>> 
            visit_variant(VisitorType&& visit, VariantType&& vt)
        {
            using variant_t = remove_cv_ref_t<VariantType>;
            
            static_loop_struct<0, std::variant_size_v<variant_t>>:: template
                visit_variant(std::forward<VisitorType>(visit),
                    std::forward<VariantType>(vt));
        }

        template<typename VisitorType, typename IteratorType, typename PairType>
        std::enable_if_t<is_pair_of_variant_v<remove_cv_ref_t<PairType>>> 
        visit_variant(VisitorType&& visit, IteratorType&& itr, PairType&& vpr)
        {
            using pair_t = remove_cv_ref_t<PairType>;
            using second_type = typename pair_t::second_type;

            static_loop_struct<0, std::variant_size_v<second_type>>:: template
                visit_variant(std::forward<VisitorType>(visit),
                    std::forward<IteratorType>(itr), std::forward<PairType>(vpr));
        }

        template<typename VisitorType, typename IteratorType, typename VariantType>
        std::enable_if_t<is_variant_v<remove_cv_ref_t<VariantType>>> 
            visit_variant(VisitorType&& visit, IteratorType&& itr, VariantType&& vt)
        {
            using variant_t = remove_cv_ref_t<VariantType>;
            
            static_loop_struct<0, std::variant_size_v<variant_t>>:: template
                visit_variant(std::forward<VisitorType>(visit),
                    std::forward<IteratorType>(itr), std::forward<VariantType>(vt));
        }

        ///////////////////////////////////////////////////
        template<typename... CallbackTypes>
        struct variant_visitors
        {
            using visitors_t = std::tuple<CallbackTypes...>;
            
            visitors_t m_visitors;

            variant_visitors(CallbackTypes... visitors):
                m_visitors{ std::move(visitors)... } { }
            
            template<typename Type>
            decltype(auto) handle (Type&& item)
            {
                return visit_variant(*this, std::forward<Type>(item));
            }

            template<typename ContainerType>
            void for_each(ContainerType&& container) 
            {
                for(decltype(auto) item: std::forward<ContainerType>(container))
                {
                    visit_variant(*this, std::forward<decltype(item)>(item));
                }
            }

            template<typename ContainerType>
            void for_each_reverse(ContainerType&& container) 
            {
                for(decltype(auto) item: reverse(std::forward<ContainerType>(container)))
                {
                    visit_variant(*this, std::forward<decltype(item)>(item));
                }
            }

            ///////////////////////////////////////////////////
            template<typename ContainerType>
            void for_each_iterator(ContainerType&& container) 
            {
                if constexpr (is_const_reference_v<ContainerType>)
                {
                    for(decltype(auto) itr = container.cbegin(); itr != container.cend(); ++itr)
                        visit_variant(*this, std::forward<decltype(itr)>(itr),
                            std::forward<decltype(*itr)>(*itr));
                }
                else
                {
                    for(decltype(auto) itr = container.begin(); itr != container.end(); ++itr)
                        visit_variant(*this, std::forward<decltype(itr)>(itr),
                            std::forward<decltype(*itr)>(*itr));
                }
            }

            ///////////////////////////
            template<typename ContainerType>
            void for_each_index(ContainerType&& container) 
            {
                if constexpr (is_const_reference_v<ContainerType>)
                {
                    for(decltype(auto) itr = container.cbegin(); 
                        itr != container.cend(); ++itr)
                    {
                        auto index = std::distance(container.cbegin(), itr);
                        visit_variant(*this, index, std::forward<decltype(*itr)>(*itr));
                    }
                }
                else
                {
                    for(decltype(auto) itr = container.begin(); 
                        itr != container.end(); ++itr)
                    {
                        auto index = std::distance(container.begin(), itr);
                        visit_variant(*this, index, std::forward<decltype(*itr)>(*itr));
                    }
                }
            }
            
            template<typename ContainerType>
            void for_each_reverse_iterator(ContainerType&& container) 
            {
                for_each_iterator(types::reverse(std::forward<ContainerType>(container)));
            }

            template<typename ContainerType>
            void for_each_reverse_index(ContainerType&& container) 
            {
                 for_each_index(types::reverse(std::forward<ContainerType>(container)));
            }

            /////////////////////////////////
            template<typename VariantType>
            std::enable_if_t<is_variant_v<remove_cv_ref_t<VariantType>>>
            operator()(VariantType&& vt)
            {
                visit_variant(*this, std::forward<VariantType>(vt));
            }

            template<typename PairType>
            std::enable_if_t<is_pair_of_variant_v<remove_cv_ref_t<PairType>>>
            operator()(PairType&& vt)
            {
                visit_variant(*this, std::forward<PairType>(vt));
            }
        };
        
        template<typename... CallbackTypes>
        variant_visitors(CallbackTypes...) -> variant_visitors<CallbackTypes...>;

        template<typename... CallbackTypes>
        variant_visitors<remove_cv_ref_t<CallbackTypes>...> 
        make_variant_visitors(CallbackTypes&& ... visitors)
        {
            return { std::forward<CallbackTypes>(visitors)... };
        }

    } // end of namespace types

    template<typename ContainerType, typename IndexType>
        decltype(auto) get_element(ContainerType container, IndexType index)
    {
        return tpf::types::get_element<ContainerType, IndexType>(container, index);
    }

    template<template<typename, typename...> class ContainerType, typename Type, typename... Types>
    decltype(auto) make_random_access_container(Type&& arg, Types&&... args)
    {
        return types::make_random_access_container<ContainerType>(std::forward<Type>(arg), std::forward<Types>(args)...);
    }

    template<typename Type, typename... Types>
    decltype(auto) make_vector(Type&& arg, Types&&... args)
    {
        return types::make_vector(std::forward<Type>(arg), std::forward<Types>(args)...);
    }

     template<typename Type, typename... Types>
    decltype(auto) make_deque(Type&& arg, Types&&... args)
    {
        return types::make_deque(std::forward<Type>(arg), std::forward<Types>(args)...);
    }

    template<typename Type, typename... Types>
    decltype(auto) make_container(Type&& arg, Types&&... args)
    {
        return types::make_container(std::forward<Type>(arg), std::forward<Types>(args)...);
    }

    #if !defined(__clang_major__)
    
    template<typename Type, typename... Types>
    decltype(auto) make_variants(Type&& arg, Types&&... args)
    {
        return types::make_variants(std::forward<Type>(arg), std::forward<Types>(args)...);
    }
    
    #endif

    template<typename ContainerType>
    decltype(auto) reverse(ContainerType&& container)
    {
        return types::reverse(std::forward<ContainerType>(container));
    }

    template<typename ElementType, size_t ElementCount> 
    decltype(auto) reverse(ElementType(&array)[ElementCount])
    {
       return types::reverse(array);
    }

    template<typename Type, typename... Types>
    decltype(auto) reverse(Type&& arg, Types&&... args)
    {
        return types::reverse(std::forward<Type>(arg), std::forward<Types>(args)...);
    }

    template<typename IndexType, typename ContainerType, typename iterator_type>
    inline auto index_to_iterator(ContainerType&& cntr, IndexType&& index)
    {
        using offset_t = tpf::remove_cv_ref_t<IndexType>;
        using diff_t = typename iterator_type::difference_type;
        auto pos = cntr.begin(); std::advance(pos, (diff_t)index);
        return pos;
    }

    template<typename IndexType, typename ContainerType, typename reverse_iterator_type>
    inline auto index_to_reverse_iterator(ContainerType&& cntr, IndexType&& index)
    {
        using offset_t = tpf::remove_cv_ref_t<IndexType>;
        using diff_t = typename reverse_iterator_type::difference_type;
        auto pos = cntr.begin(); std::advance(pos, (diff_t)index);
        
        return reverse_iterator_type{ pos };
    }

    template<typename ContainerType, typename iterator_type>
    inline auto iterator_to_index(ContainerType&& cntr, iterator_type&& itr)
    {
        return (size_t)std::distance(cntr.begin(), itr);
    }

    template<typename ContainerType, typename reverse_iterator_type>
    inline auto reverse_iterator_to_index(ContainerType&& cntr, reverse_iterator_type&& rev_itr)
    {
        auto itr = typename ContainerType::iterator_type{ rev_itr.base() };
        return (size_t)std::distance(cntr.begin(), itr);
    }

    template<direction_t direction, 
    typename ContainerType,
    typename container_t,
    typename iterator_type,
    typename reverse_iterator_type>
    auto make_rotator(ContainerType&& cntr)
    {
		using container_type = decltype(cntr);
		
        if constexpr(direction == direction_t::left)
        {
            return [](iterator_type begin, auto&& offset, iterator_type end)
            {
                using offset_t = tpf::remove_cv_ref_t<decltype(offset)>;

                if constexpr(tpf::types::is_integral_v<offset_t>)
                {   
                    using diff_t = typename iterator_type::difference_type;
                    auto offset_pos = begin; std::advance(offset_pos, (diff_t)offset);

                    return std::rotate(begin, offset_pos, end);
                }
                else if constexpr(std::is_same_v<reverse_iterator_type, offset_t>)
                {
                    return std::rotate(begin, offset.base()-1, end);
                }
                else
                    return std::rotate(begin, offset, end);
            };
        }
        else
        {
            return [](iterator_type begin, auto&& offset, iterator_type end)
            {
                using offset_t = tpf::remove_cv_ref_t<decltype(offset)>;
                                
                if constexpr(tpf::types::is_integral_v<offset_t>)
                {
                    using diff_t = typename reverse_iterator_type::difference_type;

                    auto offset_pos = reverse_iterator_type{end};
                    std::advance(offset_pos, (diff_t)offset);

                    return std::rotate(reverse_iterator_type{end}, 
                        offset_pos, reverse_iterator_type{begin});
                }
                else if constexpr(std::is_same_v<reverse_iterator_type, offset_t>)
                    return std::rotate(reverse_iterator_type{end}, offset, reverse_iterator_type{begin});
                else
                    return std::rotate(reverse_iterator_type{end}, 
                        reverse_iterator_type{offset}, reverse_iterator_type{begin});
            };
        }
    }

    template<direction_t direction, 
    typename ContainerType,
    typename container_t,
    typename iterator_type,
    typename reverse_iterator_type,
    typename execution_policy_type>
    auto make_rotator(ContainerType&& cntr, execution_policy_type policy)
    {
		using container_type = decltype(cntr);
		
        if constexpr(direction == direction_t::left)
        {
            return [policy](iterator_type begin, auto&& offset, iterator_type end)
            {
                using offset_t = tpf::remove_cv_ref_t<decltype(offset)>;

                if constexpr(tpf::types::is_integral_v<offset_t>)
                {   
                    using diff_t = typename iterator_type::difference_type;
                    auto offset_pos = begin; std::advance(offset_pos, (diff_t)offset);

                    return std::rotate(policy, begin, offset_pos, end);
                }
                else if constexpr(std::is_same_v<reverse_iterator_type, offset_t>)
                {
                    return std::rotate(policy, begin, offset.base()-1, end);
                }
                else
                    return std::rotate(policy, begin, offset, end);
            };
        }
        else
        {
            return [policy](iterator_type begin, auto&& offset, iterator_type end)
            {
                using offset_t = tpf::remove_cv_ref_t<decltype(offset)>;
                                
                if constexpr(tpf::types::is_integral_v<offset_t>)
                {
                    using diff_t = typename reverse_iterator_type::difference_type;

                    auto offset_pos = reverse_iterator_type{end};
                    std::advance(offset_pos, (diff_t)offset);

                    return std::rotate(policy, reverse_iterator_type{end}, 
                        offset_pos, reverse_iterator_type{begin});
                }
                else if constexpr(std::is_same_v<reverse_iterator_type, offset_t>)
                    return std::rotate(policy, reverse_iterator_type{end}, offset, reverse_iterator_type{begin});
                else
                    return std::rotate(policy, reverse_iterator_type{end}, 
                        reverse_iterator_type{offset}, reverse_iterator_type{begin});
            };
        }
    }

} // end of namespace tpf

/**
 * @brief A macro that tells if type_arg is a template type
 * 
 */
#define Tpf_IsTemplateV(type_arg) tpf::types::is_template_v<type_arg>

/**
 * @brief A macro that tells if instance_arg is of a template type.
 * 
 */
#define Tpf_IsTemplateInstanceV(instance_arg) tpf::types::is_template_v<decltype(instance_arg)>

#ifdef _TPF_TYPES_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_TYPES_MIN_DEFINED
#endif 

#ifdef _TPF_TYPES_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_TYPES_MAX_DEFINED
#endif 

#endif // end of file _TPF_TYPES_HPP
