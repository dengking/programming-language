/**
 * @file tpf_output.hpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief Stream output operators << are implemented.
 * @version 0.1
 * @date 2019-04-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TPF_OUTPUT_HPP
#define _TPF_OUTPUT_HPP

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <tpf_conversion.hpp>
#include <tpf_any.hpp>
#include <tpf_bitwise.hpp>
#include <tpf_parallel.hpp>
#include <iostream>
#include <sstream>
#include <functional>

#if defined(TPF_USE_STD_EXTENSION_ARRAY_TUPLE)
    #include <tpf_std_extensions.hpp>
#endif 

#ifdef max
#define _TPF_OUTPUT_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_OUTPUT_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

/**
 * @brief includes subnamespace output.
 * 
 */
namespace tpf
{
    /**
     * @brief Stream output operators are defined.
     * 
     */
    namespace output
    {
        template<typename CharType, typename T>
        std::basic_ostream<CharType>& operator<<
        (std::basic_ostream<CharType>& os, const tpf::bitwise::bit_pattern<T>& a);

        template<typename CharType, typename... ElementTypes>
        std::basic_ostream<CharType>& operator<<
        (std::basic_ostream<CharType>& os, const tpf::types::any<ElementTypes...>& a);
        
        template<typename CharType, typename T>
        std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, const std::optional<T>& opt);

        template<typename CharType, auto N, auto D>
        std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, const std::ratio<N, D>& r)
        {
            if(r.num == 0)
                os <<"< 0 >";
            else if(r.den == 1)
                os <<"< "<< r.num <<" >";
            else
                os <<"< " << r.num << " / " << r.den <<" > ";

            return os;
        }

        template<typename CharType>
        std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, const std::byte& b)
        {
            os << tpf::bitwise::to_bits<char>(std::to_integer<char>(b));
            
            return os;
        }
       
        template<typename CharType, auto IndexValue>
        std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, 
            const types::indexer_t<IndexValue>& indexer)
        {
            os << "< " << indexer.Index << " >" ; return os;
        }

        template<typename CharType, auto RowValue, auto ColumnValue>
        std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, 
            const types::indexer_2d_t<RowValue, ColumnValue>& indexer)
        {
            os << "< " << indexer.Row <<", " << indexer.Column<<" >"; return os;
        }

        template<typename CharType, auto HeightValue, auto RowValue, auto ColumnValue>
        std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, 
            const types::indexer_3d_t<HeightValue, RowValue, ColumnValue>& indexer)
        {
            os << "< " << indexer.Height <<", "<< indexer.Row <<", " << indexer.Column<<" >"; return os;
        }

        template<typename CharType, typename Type, int Tag>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os,
            const types::tag_type<Type, Tag>& ncw);

        template<typename CharType, typename Type>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os,
            const std::initializer_list<Type>& init_list);

        template<typename CharType, template<auto...>class ContainerType,
            auto... Ints> std::basic_ostream<CharType>&
        operator>>(std::basic_ostream<CharType>& os, const ContainerType<Ints...>& seq);

        template<typename CharType, typename T, auto FirstValue, auto LastValue, size_t CountValue, bool stack_order>
        std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, 
            const types::hidden::st_sequence_info<T, FirstValue, LastValue, CountValue, stack_order>& seq)
        {
            os << "{ " << Tpf_GetTypeName(T)<<", Begin: " << seq.Begin<<", Last: " 
                << seq.Last<<", End: "<< seq.End <<", Span: " << seq.Span
                <<", Count: " << seq.Count<<", InOrder: " << seq.InOrder <<" }";

            return os; 
        }

        template<typename CharType,
            typename T, T... Ints> std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, const std::integer_sequence<T, Ints...>& seq)
        {
            constexpr auto Size = sizeof...(Ints);

            if(Size == 0)
            {
                os <<"< >";
            }
            else
            {
                std::array<T, Size> array{ Ints...};

                types::indexer_t<Size> dimension;

                os <<"< ";

                for(size_t i = 0; i < Size; ++i)
                {
                    os << array[i];

                    if(i + 1 != Size) os <<", ";
                }

                os <<" >";
            }

            return os;
        }

        template<typename CharType, template<auto...>class ContainerType,
            auto... Ints> std::basic_ostream<CharType>&
        operator>>(std::basic_ostream<CharType>& os, const ContainerType<Ints...>& seq)
        {
            constexpr auto Size = sizeof...(Ints);

            std::array array{ Ints...};

            types::indexer_t<Size> dimension;

            if(Size == 0)
            {
                os <<"< >";
            }
            else
            {
                os << "< " ;

                for(size_t i = 0; i < Size; ++i)
                {
                    os << array[i];

                    if(i + 1 != Size) os <<", ";
                }

                os <<" >";
            }

            return os;
        }


        template<typename CharType,
            typename T, T... Ints> std::basic_ostream<CharType>&
        operator>>(std::basic_ostream<CharType>& os, const std::integer_sequence<T, Ints...>& seq)
        {
            constexpr auto Size = sizeof...(Ints);

            std::array array{ Ints...};

            types::indexer_t<Size> dimension;

            if(Size == 0)
            {
                os <<"{ "<<Tpf_GetTypeName(T)<<" }";
            }
            else
            {
                os <<"{ "<<Tpf_GetTypeName(T)<<": ";

                for(size_t i = 0; i < Size; ++i)
                {
                    os << array[i];

                    if(i + 1 != Size) os <<", ";
                }

                os <<" }";
            }

            return os;
        }

        template<typename CharType> std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, types::TemplateCategory tc)
        {
            switch(tc)
            {
                case types::TemplateCategory::Type: os <<"Type"; return os;

                case types::TemplateCategory::Type_Value: os <<"Type_Value"; return os;

                case types::TemplateCategory::Value: os <<"Value"; return os;

                case types::TemplateCategory::Value_Type: os <<"Value_Type"; return os;

                default:
                    os <<"Unknown"; return os;
            }
        }
      
        /**
         * @brief Feed std::wstring to std::ostream.
         * 
         * @param os for std::ostream
         * @param str for std::wstring type
         * @return std::ostream& 
         */
        inline std::ostream& operator<<(std::ostream& os, const std::wstring& str)
        {
            os << conversion::source_to_target<char, wchar_t>(str);
            return os;
        }

        /**
         * @brief Feed const wchar_t* to std::ostream
         * 
         * @param os of type std::ostream
         * @param str of const wchar_t* type
         * @return std::ostream& 
         */
        inline std::ostream& operator<<(std::ostream& os, const wchar_t* str)
        {
            os << conversion::source_to_target<char, wchar_t>(str);
            return os;
        }

        /**
         * @brief Feed std::string to std::wostream
         * 
         * @param os of type std::wostream type
         * @param str of std::string type
         * @return std::wostream& 
         */
        inline std::wostream& operator<<(std::wostream& os, const std::string& str)
        {
            os << conversion::source_to_target<wchar_t, char>(str);
            return os;
        }
        
        /**
         * @namespace tpf::output::hidden
         * @brief Implementations in this scope is meant NOT to be accessed directly by the client.
         * @internal
         */
        namespace hidden
        {
            template<typename CharType, template<typename, typename...> class ContainerType,
                typename Type, typename... Types>
            using enable_stream_if_not_string_t = std::enable_if_t<
                types::is_empty_available_v<ContainerType<Type, Types...>> && 
                    !types::is_basic_string_v<ContainerType<Type, Types...>>, 
                std::basic_ostream<CharType>&>;

        } // end of namespace hidden    

        template<typename CharType, typename FirstType, typename SecondType>
        std::basic_ostream<CharType>& 
        operator<<(std::basic_ostream<CharType>& os, const std::pair<FirstType, SecondType>& pr);

        template<typename CharType, typename... Types>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const std::tuple<Types...>& tuple);

        template<typename CharType, typename Type, typename... Types>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const std::variant<Type, Types...>& variant);

        template<typename CharType, typename Type_1, typename Type_2>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const std::unique_ptr<Type_1, Type_2>& ptr);

        template<typename CharType, typename Type>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const std::shared_ptr<Type>& ptr);

        template<typename CharType, typename ElementType, size_t Size>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os,
            const std::array<ElementType, Size>& array);

        /**
         * @brief Feed STL container to std::basic_stream<CharType>
         * 
         * @tparam CharType for std::basic_stream<CharType>
         * @tparam ContainerType for STL container
         * @tparam Type for the first template parameter
         * @tparam Types for the rest of the template parameter
         * @param os for output stream, either std::ostream or std::wostream
         * @param container of STL container class, ContainerType<Type, Types...>
         * @return enable_stream_if_not_string_t<CharType, ContainerType, Type, Types...> 
         */
        template<typename CharType, template<typename, typename...> class ContainerType,
            typename Type, typename... Types>
        hidden::enable_stream_if_not_string_t<CharType, ContainerType, Type, Types...>
        operator<<(std::basic_ostream<CharType>& os, const ContainerType<Type, Types...>& container)
        {
            using namespace conversion;

            if(container.empty())
            {
                os << smart_encode("{ }"); 
                return os;
            }
            else
            {
                auto last_element = container.cend();
                
                os << "{ ";

                for(auto itr = container.cbegin(); itr != container.cend(); )
                    {
                        os << *itr;
                        
                        ++itr;

                        if(itr == container.cend())
                            os << " }";
                        else
                            os << ", ";
                    }

                return os;
            }
        }

        template<typename CharType, typename ElementType, size_t Size>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, const std::array<ElementType, Size>& array)
        {
            if(array.empty())
            {
                os <<"{ }"; return os;
            }
            else
            {
                auto last = array.end();
                --last;

                os << "{ ";
                for(auto itr = array.begin(); itr != last; ++itr)
                    os << *itr <<", "; // if the type of *itr or ElementType is std::array
                                        // operator<<() is called recursively

                os << *last <<" }";

                return os;
            }
        }      

        template<typename CharType, typename Type, int Tag>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os,
            const types::tag_type<Type, Tag>& ncw)
        {
            os << ncw.get(); return os;
        }

        /**
         * @brief Feed std::pair<FirstType, SecondType> to std::basic_ostream<CharType>
         * 
         * @tparam CharType for std::basic_ostream<CharType>
         * @tparam FirstType for std::pair<FirstType, SecondType>
         * @tparam SecondType  for std::pair<FirstType, SecondType>
         * @param os &mdash; an instance of std::basic_stream<CharType>
         * @param pr &mdash; an instance of std::pair<FirstType, SecondType>
         * @return std::basic_ostream<CharType>& 
         */
        template<typename CharType, typename FirstType, typename SecondType>
        std::basic_ostream<CharType>& 
        operator<<(std::basic_ostream<CharType>& os, const std::pair<FirstType, SecondType>& pr)
        {
            os << "{ " << pr.first << ", " << pr.second << " }";
            return os;
        }

        template<typename CharType, typename FirstType, typename SecondType>
        std::basic_ostream<CharType>& 
        operator<<(std::basic_ostream<CharType>& os, 
            const types::array_wrapper_t<FirstType, SecondType>& array)
        {
            auto [ptr, size] = array;
            
            if(types::is_string_v<FirstType> && 
                tpf::primitive_type_t<FirstType>{} == ptr[size-1])
                os << ptr ;
            else
            {
                os << "{ ";
            
                for(size_t i = 0; i < (size_t)size-1; ++i)
                    os << *(ptr+i) << ", ";
            
                os << *(ptr+size-1) << " }";
            }

            return os;
        }

        template<typename CharType, typename... Types>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const types::type_list_t<Types...>& typelist);

        namespace hidden
        {
            // dummy function
            template<typename CharType>
            static void print_type_list(std::basic_ostream<CharType>& os) { }

            template<typename CharType, typename Type, typename... Types>
            static void print_type_list(std::basic_ostream<CharType>& os)
            {
                if constexpr (types::is_type_list_v<Type>)
                {
                    os << Type{};
                }
                else
                {
                    os << Tpf_GetTypeName(Type);
                }
                if constexpr(sizeof...(Types) != 0)
                {
                    os << ", ";
                    print_type_list<CharType, Types...>(os);
                }
            }

            template<typename CharType, typename... Types>
            void print_type_list(std::basic_ostream<CharType>& os, 
                const types::type_list_t<Types...>& typelist)
            {
                if constexpr(sizeof...(Types)==0)
                    os << conversion::smart_encode("< >");
                else
                {
                    os << "< ";
                    print_type_list<CharType, Types...>(os);
                    os<< " >";
                }
            }

        } // end of namespace hidden

        template<typename CharType, typename... Types>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const types::type_list_t<Types...>& typelist)
        {
            hidden::print_type_list(os, typelist);
            return os;
        }

        template<typename CharType>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const std::exception& expt)
        {
            os << expt.what();
            return os;
        }

        template<typename CharType, typename... Types>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const std::tuple<Types...>& tuple)
        {
            constexpr int Size = sizeof...(Types);

            if constexpr(Size == 0)
            {
                os <<"{ }"; return os;
            }
            else
            {
                os <<"{ ";

                types::for_workhorse<Size>([&os, &tuple](auto i, auto info)
                {
                    os << std::get<i.Index>(tuple);

                    if(i.Index != info.Last)
                        os <<", ";
                });

                os <<" }";

                return os;
            }
        }

        template<typename CharType, typename Type, typename... Types>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const std::variant<Type, Types...>& variant)
        {

            if(std::variant_npos == variant.index())
                return os;
            else
            {
                constexpr int Size = sizeof...(Types) + 1;

                types::for_workhorse<Size>([&os, &variant](auto i)
                {
                    if(i.Index == variant.index())
                    {
                        if constexpr( std::is_same_v<std::monostate,
                            std::variant_alternative_t<i.Index, std::variant<Type, Types...>>>)
                        {
                            os << "monostate";
                        }
                        else
                        {
                            os << std::get<i.Index>(variant);
                        }
                    }
                });

                return os;
            }
        }

        template<typename CharType, typename Type_1, typename Type_2>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const std::unique_ptr<Type_1, Type_2>& ptr)
        {
            if(ptr) os << *ptr;
            return os;
        }

        template<typename CharType, typename Type>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const std::shared_ptr<Type>& ptr)
        {
            if(ptr) os << *ptr;
            return os;
        }

        template<typename CharType, template<typename, typename...> class ContainerType,
            typename... ElementTypes>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const types::hidden::container_of_variants_class<ContainerType, ElementTypes...>& container)
        {
            using namespace conversion;

            if(container.empty())
            {
                os << smart_encode("{ }"); 
                return os;
            }
            else
            {
                auto last_element = container.cend();
                
                os << "{ ";

                for(auto itr = container.cbegin(); itr != container.cend(); )
                    {
                        os << *itr;
                        
                        ++itr;

                        if(itr == container.cend())
                            os << " }";
                        else
                            os << ", ";
                    }

                return os;
            }
        }

        template<typename CharType, template<typename, typename...> class ContainerType,
            typename... ElementTypes>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const types::hidden::container_of_tuples_class<ContainerType, ElementTypes...>& container)
        {
            using namespace conversion;

            if(container.empty())
            {
                os << smart_encode("{ }"); 
                return os;
            }
            else
            {
                auto last_element = container.cend();
                
                os << "{ ";

                for(auto itr = container.cbegin(); itr != container.cend(); )
                    {
                        os << *itr;
                        
                        ++itr;

                        if(itr == container.cend())
                            os << " }";
                        else
                            os << ", ";
                    }

                return os;
            }
        }

        template<typename CharType, typename ContainerType>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const types::reverse_st<ContainerType>& container)
        {
            using namespace conversion;

            if(container.empty())
            {
                os << smart_encode("{ }"); 
                return os;
            }
            else
            {
                auto last_element = container.cend();
                
                os << "{ ";

                for(auto itr = container.cbegin(); itr != container.cend(); )
                    {
                        os << *itr;
                        
                        ++itr;

                        if(itr == container.cend())
                            os << " }";
                        else
                            os << ", ";
                    }

                return os;
            }
        }

        template<typename CharType, size_t ElementCount>
        std::basic_ostream<CharType>& 
        operator<<(std::basic_ostream<CharType>& os, const std::byte(&array)[ElementCount])
        {
            using namespace conversion;

            if(ElementCount == 0)
            {
                os << smart_encode(" "); 
                return os;
            }
            else
            {
                auto last_idx = ElementCount-1;

                for(int idx = last_idx; idx > 0; --idx )
                {
                    os << array[idx] << '\'';
                }

                os << array[0];

                return os;
            }
        }

        template<typename CharType, typename T>
        std::basic_ostream<CharType>& operator<<
        (std::basic_ostream<CharType>& os, const tpf::bitwise::bit_pattern<T>& a)
        {
            if constexpr(std::is_same_v<T, unsigned char> ||  std::is_same_v<T, char> )
                os << (unsigned short)a.n <<"\t(" << tpf::bitwise::to_hex(a.n) <<")\t"<<a.b;
            else
                os << a.n <<"\t(" << tpf::bitwise::to_hex(a.n) <<")\t"<<a.b;

            return os;
        }

        template<typename CharType, typename ElementType, size_t ElementCount>
        std::enable_if_t<!types::is_char_v<ElementType>, std::basic_ostream<CharType>&>
        operator<<(std::basic_ostream<CharType>& os, const ElementType(&array)[ElementCount])
        {
            using namespace conversion;

            if(ElementCount == 0)
            {
                os << smart_encode("{ }"); 
                return os;
            }
            else
            {
                os << "{ ";
                
                auto last_idx = ElementCount-1;

                for(size_t idx = 0; idx < last_idx; ++idx )
                {
                    os << array[idx] << ", ";
                }

                os << array[last_idx] << " }";

                return os;
            }
        }

        // template<typename ElementType, size_t ElementCount>
        // class reverse_st<ElementType(&)[ElementCount]>
        template<typename CharType, typename ElementType, size_t ElementCount>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, 
            const types::reverse_st<ElementType(&)[ElementCount]>& container)
        {
            using namespace conversion;

            if(container.empty())
            {
                os << smart_encode("{ }"); 
                return os;
            }
            else
            {
                auto last_element = container.cend();
                
                os << "{ ";

                for(auto itr = container.cbegin(); itr != container.cend(); )
                    {
                        os << *itr;
                        
                        ++itr;

                        if(itr == container.cend())
                            os << " }";
                        else
                            os << ", ";
                    }

                return os;
            }
        }

        template<typename CharType, typename Type>
        std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os,
            const std::initializer_list<Type>& init_list)
        {
            std::vector<Type> vctr{init_list.begin(), init_list.end()};
            os << vctr; return os;
        }

        struct new_line{};
        struct new_Line{};
        struct flush_stream{};
        struct new_line_flush{};
        struct new_Line_flush{};

        constexpr new_line nl(){ return new_line{}; }
        constexpr new_Line nL(){ return new_Line{}; }
        constexpr flush_stream flush() { return flush_stream{}; };
        constexpr new_line_flush endl() { return new_line_flush{}; };
        constexpr new_Line_flush endL() { return new_Line_flush{}; };

        class string_stream: public std::ostringstream
        {
            public:
                using base_t = std::ostringstream;
                                
            public:
                
                base_t& std() 
                { 
                    return static_cast<base_t&>(*this); 
                }

                const base_t& std() const 
                { 
                    return static_cast<const base_t&>(*this); 
                }    
                
                string_stream()
                {
                    std() << std::boolalpha;
                    // static_cast<base_t&>(*this).imbue(std::locale(""));
                }

                string_stream(const string_stream&) = default;
                
                string_stream& operator=(const string_stream&) = default;

                string_stream& operator << (const char* arg)
                {
                    std() << conversion::smart_encode(arg);
                    return *this;
                }

                string_stream& operator << (const wchar_t* arg)
                {
                    std() << conversion::smart_encode(arg);
                    return *this;
                }
                
                template<typename ArgCharType> 
                string_stream& operator << (const std::basic_string<ArgCharType>& arg)
                {
                    std() << arg.c_str();
                    return *this;
                }

                template<typename ArgCharType> 
                string_stream& operator << (const ArgCharType& arg)
                {
                    std() << arg; 
                    return *this; 
                }

                string_stream& operator << (const new_line&)
                {
                    std() << "\n";
                    return *this; 
                }

                string_stream& operator << (const new_Line&)
                {
                    std() << "\n\n";
                    return *this; 
                }

                string_stream& clear()
                {
                    std().clear(); std().str("");
                    return *this; 
                }

                string_stream& operator << (const flush_stream&)
                {
                    std::cout << std().str(); std::cout.flush();
                    std().clear(); std().str("");
                    return *this; 
                }

                string_stream& operator << (const new_line_flush&)
                {
                    std::cout << std().str() << std::endl;

                    std().clear(); std().str("");
                    
                    return *this; 
                }

                string_stream& operator << (const new_Line_flush&)
                {
                    std() << "\n";
                    std::cout << std().str() << std::endl;
                    std().clear(); std().str("");
                    return *this; 
                }

                operator std::string() const 
                {
                    return this->std().str();
                }

                std::string str() const
                {
                    return this->std().str();
                }

                operator std::wstring() const 
                {
                    return conversion::utf8_to_utf16(this->std().str());
                }

                operator const char*() const
                {
                    return this->std().str().c_str();
                }
        };

    } // end of namespace output

    using sstream = output::string_stream;
    constexpr auto flush = output::flush_stream{};
    constexpr auto nl = output::new_line{};
    constexpr auto nL = output::new_Line{};
    constexpr auto endl = output::new_line_flush{};
    constexpr auto endL = output::new_Line_flush{};

    namespace output
    {
        using namespace tpf::types;
                      
        template< typename T, typename = std::enable_if_t<!(types::is_tag_type_v<T> || types::is_numbers_type_v<T>)>>
        sstream& operator >> (sstream& os, T arg)
        {
            os << "(decayed)" << Tpf_GetTypeName(T);
            
            return os; 
        }

        template< template<typename...> class TemplateType, typename... Types>
        sstream& operator >> (sstream& os, const TemplateType<Types...>& arg)
        {
            using element_list_t = types::to_recursive_type_list_t<TemplateType<Types...>>;
            
            os <<element_list_t{} ;
            
            return os; 
        }

        template<typename Type, int Tag>
        sstream& operator>>(sstream& os, const types::tag_type<Type, Tag>&)
        {
            os <<"tag< "<< Tpf_GetTypeName(Type) 
                << ", " << Tag <<" >";  return os;
        }

        template<template<auto...> class ContainerType, auto... Ints>
        sstream& operator>>(sstream& os, const ContainerType<Ints...>& arg)
        {
            os.std() >> arg ;  return os;
        }

        template< template<typename T, T...> class TemplateType, typename Type, Type... Args,
            typename = std::enable_if_t<!types::is_tag_type_v<TemplateType<Type, Args...>>>>
        sstream& operator >> (sstream& os, const TemplateType<Type, Args...>&)
        {
            os.std() >> std::integer_sequence<Type, Args...>{};
            return os;
        }

        template<typename Type>
        string_stream& operator < (string_stream& os, Type&& arg)
        {
            os << arg <<", "; return os;
        }

        template<typename Type>
        string_stream& operator > (string_stream& os, Type&& arg)
        {
            os << arg ; return os;
        }

        template<typename CharType, typename T>
        std::basic_ostream<CharType>&
        operator<<(std::basic_ostream<CharType>& os, const std::optional<T>& opt)
        {
            if(opt.has_value())
            {
                os << *opt;
            }
            
            return os;
        }

        template<typename CharType, typename... ElementTypes>
        std::basic_ostream<CharType>& operator<<
        (std::basic_ostream<CharType>& os, const tpf::types::any<ElementTypes...>& a)
        {
            if(!a.std().has_value())
            {
                os << "no_value"; return os;
            }
            else
            {
                constexpr int Size = sizeof...(ElementTypes);

                try 
                {
                    types::for_workhorse<Size>([&os, &a](auto i)
                    {
                        using element_types_t = typename any<ElementTypes...>::element_types_t;

                        using element_t = select_nth_type_t<i.Index, element_types_t>;
                        
                        if(auto ptr = std::any_cast<element_t>(&a.std()))
                        {
                            os << *ptr;

                            // operation successful
                            throw types::no_type_t{};
                        }
                    });

                }catch(const types::no_type_t&) { }

                return os;
            }
        }
         
        template<typename Type, size_t CacheSize>
        sstream& operator << (sstream& os, 
            const tpf::parallel::cache_wrapper<Type, CacheSize>& cw)
        {
            os.std() << cw.get(); return os;
        }
        
    } // end of namespace output

} // end of namespace tpf

#ifdef _TPF_OUTPUT_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_OUTPUT_MIN_DEFINED
#endif 

#ifdef _TPF_OUTPUT_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_OUTPUT_MAX_DEFINED
#endif 

#endif // end of file _TPF_OUTPUT_HPP
