/**
 * @file tpf_std_extensions.hpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief Type functions are implemented
 * @version 0.1
 * @date 2021-02-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _TPF_STD_EXTENSIONS_HPP
#define _TPF_STD_EXTENSIONS_HPP

#ifdef _MSVC_LANG 

	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif

#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#ifdef max
#define _TPF_STD_EXTENSIONS_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_STD_EXTENSIONS_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

#pragma warning(push)
#pragma warning(disable: 4244)

#include <tpf_types.hpp>
#include <cmath>

namespace std
{    
    namespace types = tpf::types;

    template<typename OperationType, typename... ArgTypes>
    auto apply_operation(OperationType&& operation, const std::tuple<ArgTypes...>& tuple);

    template<typename OperationType, typename T, size_t Size>
    auto apply_operation(OperationType&& operation, const std::array<T, Size>& array);

    template<typename OperationType, typename T, size_t Size>
    void apply_operation_inplace(OperationType&& operation, std::array<T, Size>& array);

    template<typename T, size_t Size1, size_t Size2, typename = std::enable_if_t<Size1 != Size2>>
    auto operator==(const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b);

    template<typename T, size_t Size1, size_t Size2, typename = std::enable_if_t<Size1 != Size2>>
    auto operator!=(const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b);

    template<typename T, typename S, size_t Size,
        typename = std::enable_if_t<!std::is_same_v<T, S>>>
    auto operator == (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b);

    template<typename T, typename S, size_t Size,
        typename = std::enable_if_t<!std::is_same_v<T, S>>>
    auto operator != (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b);

    template<typename T, typename S, size_t Size,
        typename = std::enable_if_t<!std::is_same_v<T, S>>>
    auto operator < (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b);

    template<typename T, typename S, size_t Size,
        typename = std::enable_if_t<!std::is_same_v<T, S>>>
    auto operator <= (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b);

    template<typename T, typename S, size_t Size,
        typename = std::enable_if_t<!std::is_same_v<T, S>>>
    auto operator > (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b);

    template<typename T, typename S, size_t Size,
        typename = std::enable_if_t<!std::is_same_v<T, S>>>
    auto operator >= (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b);
    
    //////////////////////////////////////
    template<typename T, size_t Size1, size_t Size2, typename = std::enable_if_t<Size1 != Size2>>
    auto operator < (const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b);

    template<typename T, size_t Size1, size_t Size2, typename = std::enable_if_t<Size1 != Size2>>
    auto operator <= (const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b);

    template<typename T, size_t Size1, size_t Size2, typename = std::enable_if_t<Size1 != Size2>>
    auto operator > (const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b);

    template<typename T, size_t Size1, size_t Size2, typename = std::enable_if_t<Size1 != Size2>>
    auto operator >= (const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b);

    template<typename T, typename S> auto fail_safe_addition(T&& a, T&& b);
    template<typename T, typename S> auto fail_safe_subtraction(T&& a, T&& b);
    template<typename T, typename S> auto fail_safe_subtraction(T&& a, T&& b);
    template<typename T, typename S> auto fail_safe_multiplication(T&& a, T&& b);

    template<typename... ArgTypes1, typename... ArgTypes2,
        typename common_type = types::tuple_operation_valid_t<std::tuple<ArgTypes1...>, std::tuple<ArgTypes2...>>,
        typename = std::enable_if_t<!std::is_same_v<common_type, std::tuple<>>>>
    auto operator+(const std::tuple<ArgTypes1...>& tuple_a, const std::tuple<ArgTypes2...>& tuple_b);

    template<typename... ArgTypes1, typename... ArgTypes2, 
        typename common_type = types::tuple_operation_valid_t<std::tuple<ArgTypes1...>, std::tuple<ArgTypes2...>>,
        typename = std::enable_if_t<!std::is_same_v<common_type, std::tuple<>>>>
    auto operator-(const std::tuple<ArgTypes1...>& tuple_a, const std::tuple<ArgTypes2...>& tuple_b);
    
    template<typename... ArgTypes1, typename... ArgTypes2,
        typename common_type = types::tuple_operation_valid_t<std::tuple<ArgTypes1...>, std::tuple<ArgTypes2...>>,
        typename = std::enable_if_t<!std::is_same_v<common_type, std::tuple<>>>>
    auto operator*(const std::tuple<ArgTypes1...>& tuple_a, const std::tuple<ArgTypes2...>& tuple_b);

    template<typename... ArgTypes1, typename... ArgTypes2,
        typename common_type = types::tuple_operation_valid_t<std::tuple<ArgTypes1...>, std::tuple<ArgTypes2...>>,
        typename = std::enable_if_t<!std::is_same_v<common_type, std::tuple<>>>>
    auto operator/(const std::tuple<ArgTypes1...>& tuple_a, const std::tuple<ArgTypes2...>& tuple_b);

    template<typename T, typename S>
    inline auto fail_safe_addition(T&& a, S&& b)
    {
        using a_t = types::remove_cvref_t<T>;
        using b_t = types::remove_cvref_t<T>;

        if constexpr(types::is_addition_valid_v<a_t, b_t>)
        {
            return a + b;
        }
        else
        {
            return types::no_type_t{};
        }
    }

    template<typename T, typename S>
    inline auto fail_safe_subtraction(T&& a, S&& b)
    {
        using a_t = types::remove_cvref_t<T>;
        using b_t = types::remove_cvref_t<T>;

        if constexpr(types::is_subtraction_valid_v<a_t, b_t>)
        {
            return a - b;
        }
        else
        {
            return types::no_type_t{};
        }
    }

    template<typename T, typename S>
    inline auto fail_safe_multiplication(T&& a, S&& b)
    {
        using a_t = types::remove_cvref_t<T>;
        using b_t = types::remove_cvref_t<T>;

        if constexpr(types::is_multiplication_valid_v<a_t, b_t>)
        {
            return a * b;
        }
        else
        {
            return types::no_type_t{};
        }
    }

    template<typename T, typename S>
    inline auto fail_safe_division(T&& a, S&& b)
    {
        using a_t = types::remove_cvref_t<T>;
        using b_t = types::remove_cvref_t<T>;

        if constexpr(types::is_division_valid_v<a_t, b_t>)
        {
            return a / b;
        }
        else
        {
            return types::no_type_t{};
        }
    }

    namespace hidden
    {
        template<typename... ArgTypes1, typename... ArgTypes2, auto... Indices>
        auto tuple_addition(const std::tuple<ArgTypes1...>& tuple_a,
            const std::tuple<ArgTypes2...>& tuple_b, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (std::get<Indices>(tuple_a) + std::get<Indices>(tuple_b))... };
        }

        template<typename T, typename S, auto Size, auto... Indices>
        auto array_equality(const std::array<T, Size>& array_a,
            const std::array<S, Size>& array_b, types::typed_sequence_t<Indices...>)
        {
            auto equal_opr = [&array_a, &array_b](auto indexer)
            {
                if constexpr(types::is_std_array_v<T> && types::is_std_array_v<S>)
                {
                    return array_a[indexer.Index] == array_b[indexer.Index] ;
                }
                else if constexpr(!types::is_std_array_v<T> && !types::is_std_array_v<T>)
                {
                    return array_a[indexer.Index] == array_b[indexer.Index];
                }
                else
                    return false;
            };

            return (true && ... && equal_opr(types::indexer_t<Indices>{}));
        }

        template<typename T, typename S, auto Size, auto... Indices>
        auto array_less(const std::array<T, Size>& array_a,
            const std::array<S, Size>& array_b, types::typed_sequence_t<Indices...>)
        {
            using common_t = types::signed_common_t<T, S>;

            auto equal_opr = [&array_a, &array_b](auto indexer)
            {
                if constexpr(types::is_std_array_v<T> && types::is_std_array_v<S>)
                {
                    return array_a[indexer.Index] < array_b[indexer.Index] ;
                }
                else if constexpr(!types::is_std_array_v<T> && !types::is_std_array_v<T>)
                {
                    return (common_t)array_a[indexer.Index] < (common_t)array_b[indexer.Index] ;
                }
                else
                {
                    std::cout <<"Wrong!!" << std::endl;

                    return false;
                }
            };

            return (true && ... && equal_opr(types::indexer_t<Indices>{}));
        }

        template<typename T, typename S, auto Size, auto... Indices>
        auto array_less_equal(const std::array<T, Size>& array_a,
            const std::array<S, Size>& array_b, types::typed_sequence_t<Indices...>)
        {
            auto equal_opr = [&array_a, &array_b](auto indexer)
            {
                if constexpr(types::is_std_array_v<T> && types::is_std_array_v<S>)
                {
                    return array_a[indexer.Index] <= array_b[indexer.Index] ;
                }
                else if constexpr(!types::is_std_array_v<T> && !types::is_std_array_v<T>)
                {
                    return array_a[indexer.Index] <= array_b[indexer.Index];
                }
                else
                    return false;
            };

            return (true && ... && equal_opr(types::indexer_t<Indices>{}));
        }

        template<typename... ArgTypes1, typename... ArgTypes2, auto... Indices>
        auto tuple_subtraction(const std::tuple<ArgTypes1...>& tuple_a,
            const std::tuple<ArgTypes2...>& tuple_b, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (std::get<Indices>(tuple_a) - std::get<Indices>(tuple_b))... };
        }

        template<typename... ArgTypes1, typename... ArgTypes2, auto... Indices>
        auto tuple_multiplication(const std::tuple<ArgTypes1...>& tuple_a,
            const std::tuple<ArgTypes2...>& tuple_b, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (std::get<Indices>(tuple_a) * std::get<Indices>(tuple_b))... };
        }

        template<typename... ArgTypes1, typename... ArgTypes2, auto... Indices>
        auto tuple_division(const std::tuple<ArgTypes1...>& tuple_a,
            const std::tuple<ArgTypes2...>& tuple_b, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (std::get<Indices>(tuple_a) / std::get<Indices>(tuple_b))... };
        }

        template<typename T, typename... ArgTypes, auto... Indices>
        auto tuple_addition(const T& scalar, const std::tuple<ArgTypes...>& tuple, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (scalar + std::get<Indices>(tuple))... };
        }

        template<typename T, typename... ArgTypes, auto... Indices>
        auto tuple_addition(const std::tuple<ArgTypes...>& tuple, const T& scalar, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (std::get<Indices>(tuple) + scalar)... };
        }

        template<typename T, typename... ArgTypes, auto... Indices>
        auto tuple_subtraction(const T& scalar, const std::tuple<ArgTypes...>& tuple, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (scalar - std::get<Indices>(tuple))... };
        }

        template<typename T, typename... ArgTypes, auto... Indices>
        auto tuple_subtraction(const std::tuple<ArgTypes...>& tuple, const T& scalar, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (std::get<Indices>(tuple) - scalar)... };
        }

        template<typename T, typename... ArgTypes, auto... Indices>
        auto tuple_multiplication(const T& scalar, const std::tuple<ArgTypes...>& tuple, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (scalar * std::get<Indices>(tuple))... };
        }

        template<typename T, typename... ArgTypes, auto... Indices>
        auto tuple_multiplication(const std::tuple<ArgTypes...>& tuple, const T& scalar, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (std::get<Indices>(tuple) * scalar)... };
        }

        template<typename T, typename... ArgTypes, auto... Indices>
        auto tuple_division(const T& scalar, const std::tuple<ArgTypes...>& tuple, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (scalar / std::get<Indices>(tuple))... };
        }

        template<typename T, typename... ArgTypes, auto... Indices>
        auto tuple_division(const std::tuple<ArgTypes...>& tuple, const T& scalar, types::typed_sequence_t<Indices...>)
        {
            return std::tuple{ (std::get<Indices>(tuple) / scalar)... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_addition(const std::array<T, Size>& array_a,
            const std::array<S, Size>& array_b, types::typed_sequence_t<Indices...>)
        {
            return std::array{ (array_a[Indices] + array_b[Indices])... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_subtraction(const std::array<T, Size>& array_a,
            const std::array<S, Size>& array_b, types::typed_sequence_t<Indices...>)
        {
            return std::array{ (array_a[Indices] - array_b[Indices])... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_multiplication(const std::array<T, Size>& array_a,
            const std::array<S, Size>& array_b, types::typed_sequence_t<Indices...>)
        {
            return std::array{ (array_a[Indices] * array_b[Indices])... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_division(const std::array<T, Size>& array_a,
            const std::array<S, Size>& array_b, types::typed_sequence_t<Indices...>)
        {
            return std::array{ (array_a[Indices] / array_b[Indices])... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_addition(const T& scalar, const std::array<S, Size>& array,
            types::typed_sequence_t<Indices...>)
        {
            return std::array{ (scalar + array[Indices])... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_addition(const std::array<S, Size>& array,
            const T& scalar, types::typed_sequence_t<Indices...>)
        {
            return std::array{ (array[Indices] + scalar)... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_subtraction(const T& scalar, const std::array<S, Size>& array,
            types::typed_sequence_t<Indices...>)
        {
            return std::array{ (scalar - array[Indices])... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_subtraction(const std::array<S, Size>& array,
            const T& scalar, types::typed_sequence_t<Indices...>)
        {
            return std::array{ (array[Indices] - scalar)... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_multiplication(const T& scalar, const std::array<S, Size>& array,
            types::typed_sequence_t<Indices...>)
        {
            return std::array{ (scalar * array[Indices])... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_multiplication(const std::array<S, Size>& array,
            const T& scalar, types::typed_sequence_t<Indices...>)
        {
            return std::array{ (array[Indices] * scalar)... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_division(const T& scalar, const std::array<S, Size>& array,
            types::typed_sequence_t<Indices...>)
        {
            return std::array{ (scalar / array[Indices])... };
        }

        template<typename T, typename S, size_t Size, auto...Indices>
        auto array_division(const std::array<S, Size>& array,
            const T& scalar, types::typed_sequence_t<Indices...>)
        {
            return std::array{ (array[Indices] / scalar)... };
        }

        // WARNING: new type is created
        template<typename OperationType, typename... ArgTypes, auto... Indices>
        auto apply_operation(OperationType&& operation,
            const std::tuple<ArgTypes...>& tuple, types::typed_sequence_t<Indices...>)
        {
            auto apply_opr = [&operation, &tuple](auto indexer)
            {
                using ele_t = std::tuple_element_t<indexer.Index, std::tuple<ArgTypes...>>;

                if constexpr(types::is_tuple_v<ele_t>)
                {
                    return std::apply_operation(operation, std::get<indexer.Index>(tuple));
                }
                else
                {
                    return operation(std::get<indexer.Index>(tuple));
                }
            };

            return std::tuple{ apply_opr( types::indexer_t<Indices>{} )... };
        }

        template<typename OperationType, typename T, size_t Size, auto... Indices>
        auto apply_operation(OperationType&& operation,
            const std::array<T, Size>& array, types::typed_sequence_t<Indices...>)
        {
            auto apply_opr = [&operation, &array](auto indexer)
            {
                using ele_t = decltype(array[indexer.Index]);
                
                if constexpr(types::is_std_array_v<ele_t>)
                {
                    return std::apply_operation(operation, array[indexer.Index]);
                }
                else
                {
                    return operation(array[indexer.Index]);
                }
            };

            return std::array{ apply_opr(types::indexer_t<Indices>{})... };
        }

        template<typename OperationType, typename T, size_t Size, auto... Indices>
        auto apply_operation_inplace(OperationType&& operation,
            std::array<T, Size>& array, types::typed_sequence_t<Indices...>)
        {
            auto workhorse = [&operation, &array](auto indexer)
            {
                using ele_t = decltype(array[indexer.Index]);
                if constexpr(types::is_std_array_v<ele_t>)
                {
                    std::apply_operation_inplace(operation, array[indexer.Index]);
                }
                else
                {
                    array[indexer.Index] = operation(array[indexer.Index]);
                }
            };

            types::drive_workhorse<Size>(workhorse);
        }

    } // end of namespace hidden

    /////////////////// tuple-tuple binary operations //////////////////
    
    template<typename... ArgTypes1, typename... ArgTypes2, typename common_type, typename>
    auto operator+(const std::tuple<ArgTypes1...>& tuple_a, const std::tuple<ArgTypes2...>& tuple_b)
    {
        constexpr auto Size = sizeof...(ArgTypes1);
        return hidden::tuple_addition(tuple_a, tuple_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename... ArgTypes1, typename... ArgTypes2, typename common_type, typename>
    auto operator-(const std::tuple<ArgTypes1...>& tuple_a, const std::tuple<ArgTypes2...>& tuple_b)
    {
        constexpr auto Size = sizeof...(ArgTypes1);
        return hidden::tuple_subtraction(tuple_a, tuple_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename... ArgTypes1, typename... ArgTypes2, typename common_type, typename>
    auto operator*(const std::tuple<ArgTypes1...>& tuple_a, const std::tuple<ArgTypes2...>& tuple_b)
    {
        constexpr auto Size = sizeof...(ArgTypes1);
        return hidden::tuple_multiplication(tuple_a, tuple_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename... ArgTypes1, typename... ArgTypes2, typename common_type, typename>
    auto operator/(const std::tuple<ArgTypes1...>& tuple_a, const std::tuple<ArgTypes2...>& tuple_b)
    {
        constexpr auto Size = sizeof...(ArgTypes1);
        return hidden::tuple_division(tuple_a, tuple_b, types::make_typed_sequence_t<Size>{});
    }

    ///////////////////// tuple-scalar, scalar-tuple binary operations //////////////////
    template<typename T, typename... ArgTypes>
    auto operator+(const T& scalar, const std::tuple<ArgTypes...>& tuple)
    {
        constexpr auto Size = sizeof...(ArgTypes);
        return hidden::tuple_addition(scalar, tuple, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename... ArgTypes>
    auto operator+(const std::tuple<ArgTypes...>& tuple, const T& scalar)
    {
        constexpr auto Size = sizeof...(ArgTypes);
        return hidden::tuple_addition(tuple, scalar, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename... ArgTypes>
    auto operator-(const T& scalar, const std::tuple<ArgTypes...>& tuple)
    {
        constexpr auto Size = sizeof...(ArgTypes);
        return hidden::tuple_subtraction(scalar, tuple, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename... ArgTypes>
    auto operator-(const std::tuple<ArgTypes...>& tuple, const T& scalar)
    {
        constexpr auto Size = sizeof...(ArgTypes);
        return hidden::tuple_subtraction(tuple, scalar, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename... ArgTypes>
    auto operator*(const T& scalar, const std::tuple<ArgTypes...>& tuple)
    {
        constexpr auto Size = sizeof...(ArgTypes);
        return hidden::tuple_multiplication(scalar, tuple, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename... ArgTypes>
    auto operator*(const std::tuple<ArgTypes...>& tuple, const T& scalar)
    {
        constexpr auto Size = sizeof...(ArgTypes);
        return hidden::tuple_multiplication(tuple, scalar, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename... ArgTypes>
    auto operator/(const T& scalar, const std::tuple<ArgTypes...>& tuple)
    {
        constexpr auto Size = sizeof...(ArgTypes);
        return hidden::tuple_division(scalar, tuple, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename... ArgTypes>
    auto operator/(const std::tuple<ArgTypes...>& tuple, const T& scalar)
    {
        constexpr auto Size = sizeof...(ArgTypes);
        return hidden::tuple_division(tuple, scalar, types::make_typed_sequence_t<Size>{});
    }

    // WARNING: new type is created
    template<typename OperationType, typename... ArgTypes>
    auto apply_operation(OperationType&& operation, const std::tuple<ArgTypes...>& tuple)
    {
        constexpr auto Size = sizeof...(ArgTypes);
        return hidden::apply_operation(operation, tuple, types::make_typed_sequence_t<Size>{});
    }

    // WARNING: no new type is created
    template<typename OperationType, typename... ArgTypes>
    void apply_operation_inplace(OperationType&& operation, std::tuple<ArgTypes...>& tuple)
    {
        constexpr auto Size = tuple_size_v<std::tuple<ArgTypes...>>;

        auto workhorse = [&operation, &tuple](auto indexer)
        {
            using ele_t = std::tuple_element_t<indexer.Index, std::tuple<ArgTypes...>>;

            if constexpr(types::is_tuple_v<ele_t>)
            {
                std::apply_operation_inplace(operation, std::get<indexer.Index>(tuple));
            }
            else
            {
                std::get<indexer.Index>(tuple) = 
                    operation(std::get<indexer.Index>(tuple));
            }
        };

        types::drive_workhorse<Size>(workhorse);
    }

    ////////////////////////// array-array binary operations ///////////////////////
    // same element type, different size
    template<typename T, size_t Size1, size_t Size2, typename>
    auto operator==(const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b)
    {
        return false;
    }
    
    template<typename T, size_t Size1, size_t Size2, typename>
    auto operator!=(const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b)
    {
        return true;
    }

    template<typename T, size_t Size1, size_t Size2, typename>
    auto operator < (const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b)
    {
        return false;
    }

    template<typename T, size_t Size1, size_t Size2, typename>
    auto operator <= (const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b)
    {
        return false;
    }

    template<typename T, size_t Size1, size_t Size2, typename>
    auto operator > (const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b)
    {
        return false;
    }

    template<typename T, size_t Size1, size_t Size2, typename>
    auto operator >= (const std::array<T, Size1>& array_a, const std::array<T, Size2>& array_b)
    {
        return false;
    }

    template<typename T, typename S, size_t Size, typename>
    auto operator==(const std::array<T, Size>& array_a, const std::array<S, Size>& array_b)
    {
        return hidden::array_equality(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size, typename>
    auto operator!=(const std::array<T, Size>& array_a, const std::array<S, Size>& array_b)
    {
        return !hidden::array_equality(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size, typename>
    auto operator < (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b)
    {
        return hidden::array_less(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size, typename>
    auto operator <= (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b)
    {
        return hidden::array_less_equal(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }
    
    template<typename T, typename S, size_t Size, typename>
    auto operator > (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b)
    {
        return !hidden::array_less_equal(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size, typename>
    auto operator >= (const std::array<T, Size>& array_a, const std::array<S, Size>& array_b)
    {
        return !hidden::array_less(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator+(const std::array<T, Size>& array_a,
        const std::array<S, Size>& array_b)
    {
        return hidden::array_addition(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator-(const std::array<T, Size>& array_a,
        const std::array<S, Size>& array_b)
    {
        return hidden::array_subtraction(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator*(const std::array<T, Size>& array_a,
        const std::array<S, Size>& array_b)
    {
        return hidden::array_multiplication(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator/(const std::array<T, Size>& array_a,
        const std::array<S, Size>& array_b)
    {
        return hidden::array_division(array_a, array_b, types::make_typed_sequence_t<Size>{});
    }

    ////////////////////////// array-scalar, scalar_array binary operations ///////////////////////
    template<typename T, typename S, size_t Size>
    auto operator+(const T& scalar, const std::array<S, Size>& array)
    {
        return hidden::array_addition(scalar, array, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator+(const std::array<S, Size>& array, const T& scalar)
    {
        return hidden::array_addition(array, scalar, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator-(const T& scalar, const std::array<S, Size>& array)
    {
        return hidden::array_subtraction(scalar, array, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator-(const std::array<S, Size>& array, const T& scalar)
    {
        return hidden::array_subtraction(array, scalar, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator*(const T& scalar, const std::array<S, Size>& array)
    {
        return hidden::array_multiplication(scalar, array, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator*(const std::array<S, Size>& array, const T& scalar)
    {
        return hidden::array_multiplication(array, scalar, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator/(const T& scalar, const std::array<S, Size>& array)
    {
        return hidden::array_division(scalar, array, types::make_typed_sequence_t<Size>{});
    }

    template<typename T, typename S, size_t Size>
    auto operator/(const std::array<S, Size>& array, const T& scalar)
    {
        return hidden::array_division(array, scalar, types::make_typed_sequence_t<Size>{});
    }
    
    template<typename OperationType, typename T, size_t Size>
    auto apply_operation(OperationType&& operation, const std::array<T, Size>& array)
    {
        return hidden::apply_operation(operation, array, types::make_typed_sequence_t<Size>{});
    }

    template<typename OperationType, typename T, size_t Size>
    void apply_operation_inplace(OperationType&& operation, std::array<T, Size>& array)
    {
        return hidden::apply_operation_inplace(operation, array, types::make_typed_sequence_t<Size>{});    
    }

    template<typename... Elements, typename commom_element_type
            = types::tuple_common_element_t<std::tuple<Elements...>>>
    auto sum_of_elements(const std::tuple<Elements...>& tuple)
    {
        commom_element_type sum{};

        constexpr auto Size = sizeof...(Elements);

        types::for_workhorse<Size>(
            [&sum, &tuple](auto indexer)
            {
                sum += std::get<indexer.Index>(tuple);
            }
         );

        return sum;
    }

    template<typename... Elements, typename commom_element_type
            = types::tuple_common_element_t<std::tuple<Elements...>>>
    double mean_of_elements(const std::tuple<Elements...>& tuple)
    {
        auto sum = sum_of_elements(tuple);
        constexpr auto Size = sizeof...(Elements);

        return sum / Size;
    }

    template<typename... Elements, typename commom_element_type
            = types::tuple_common_element_t<std::tuple<Elements...>>>
    double standard_deviation(const std::tuple<Elements...>& tuple)
    {
        double mean = mean_of_elements(tuple);

        constexpr auto Size = sizeof...(Elements);

        double denominator = 1.0 / Size;
        double deviation{};
                
        types::for_workhorse<Size>(
            [&denominator, &deviation, &mean, &tuple](auto indexer)
            {
                double difference = mean - std::get<indexer.Index>(tuple);
                difference *= difference;
                difference *= denominator;
                deviation += difference;
            }
         );

        return std::sqrt(deviation);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // if operation fails, the operation is simply ignored, does not throw

    template<typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list>, std::variant<EleTypes...>>
    operator+(const std::variant<EleTypes...>& variant_a,
        const std::variant<EleTypes...>& variant_b)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &variant_a, &variant_b](auto first)
            {
                constexpr auto Size = sizeof...(EleTypes);

                if(auto a = std::get_if<first.Index>(&variant_a))
                {
                    types::for_workhorse<Size>([&](auto second)
                    {
                        if(auto b = std::get_if<second.Index>(&variant_b))
                        {
                            result = fail_safe_addition(*a, *b);

                            // operation completed
                            throw types::no_type_t{};
                        }
                    });
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    // if operation fails, the operation is simply ignored, does not throw
    template<typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list>, std::variant<EleTypes...>>
    operator-(const std::variant<EleTypes...>& variant_a,
        const std::variant<EleTypes...>& variant_b)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &variant_a, &variant_b](auto first)
            {
                constexpr auto Size = sizeof...(EleTypes);

                if(auto a = std::get_if<first.Index>(&variant_a))
                {
                    types::for_workhorse<Size>([&](auto second)
                    {
                        if(auto b = std::get_if<second.Index>(&variant_b))
                        {
                            result = fail_safe_subtraction(*a, *b);

                            // operation completed
                            throw types::no_type_t{};
                        }
                    });
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    // if operation fails, the operation is simply ignored, does not throw
    template<typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list>, std::variant<EleTypes...>>
    operator*(const std::variant<EleTypes...>& variant_a,
        const std::variant<EleTypes...>& variant_b)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &variant_a, &variant_b](auto first)
            {
                constexpr auto Size = sizeof...(EleTypes);

                if(auto a = std::get_if<first.Index>(&variant_a))
                {
                    types::for_workhorse<Size>([&](auto second)
                    {
                        if(auto b = std::get_if<second.Index>(&variant_b))
                        {
                            result = fail_safe_multiplication(*a, *b);

                            // operation completed
                            throw types::no_type_t{};
                        }
                    });
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    // if operation fails, the operation is simply ignored, does not throw
    template<typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list>, std::variant<EleTypes...>>
    operator/(const std::variant<EleTypes...>& variant_a,
        const std::variant<EleTypes...>& variant_b)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &variant_a, &variant_b](auto first)
            {
                constexpr auto Size = sizeof...(EleTypes);

                if(auto a = std::get_if<first.Index>(&variant_a))
                {
                    types::for_workhorse<Size>([&](auto second)
                    {
                        if(auto b = std::get_if<second.Index>(&variant_b))
                        {
                            result = fail_safe_operation(*a, *b);

                            // operation completed
                            throw types::no_type_t{};
                        }
                    });
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator+(const T& v, const std::variant<EleTypes...>& variant)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    result = fail_safe_addition(v, *a);

                    // operation completed
                    throw types::no_type_t{};                        
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator+(const std::variant<EleTypes...>& variant, const T& v)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    result = fail_safe_addition(*a, v);

                    // operation completed
                    throw types::no_type_t{};                        
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator-(const T& v, const std::variant<EleTypes...>& variant)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    result = fail_safe_subtraction(v, *a);

                    // operation completed
                    throw types::no_type_t{};                        
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator-(const std::variant<EleTypes...>& variant, const T& v)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    result = fail_safe_subtraction(*a, v);

                    // operation completed
                    throw types::no_type_t{};                        
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator*(const T& v, const std::variant<EleTypes...>& variant)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    result = fail_safe_mulitplication(v, *a);

                    // operation completed
                    throw types::no_type_t{};                        
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator*(const std::variant<EleTypes...>& variant, const T& v)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    result = fail_safe_multiplication(*a, v);

                    // operation completed
                    throw types::no_type_t{};                        
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator/(const T& v, const std::variant<EleTypes...>& variant)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    result = fail_safe_division(v, *a);

                    // operation completed
                    throw types::no_type_t{};                        
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator/(const std::variant<EleTypes...>& variant, const T& v)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    result = fail_safe_division(*a, v);

                    // operation completed
                    throw types::no_type_t{};                        
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    
    // if operation fails, throws std::bad_variant_access
    template<typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list>, std::variant<EleTypes...>>
    operator+(const std::variant<EleTypes...>& variant_a,
        const std::variant<EleTypes...>& variant_b)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &variant_a, &variant_b](auto first)
            {
                constexpr auto Size = sizeof...(EleTypes);

                if(auto a = std::get_if<first.Index>(&variant_a))
                {
                    types::for_workhorse<Size>([&](auto second)
                    {
                        if(auto b = std::get_if<second.Index>(&variant_b))
                        {
                            using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                            using ele_b_t = types::remove_cvref_t<decltype(*b)>;

                            if constexpr(types::is_addition_valid_v<ele_a_t, ele_b_t>)
                            {
                                result = (*a) + (*b);

                                // operation succeeded - abort operation
                                throw types::no_type_t{};
                            }
                            else
                            {
                                // invalid operation
                                throw std::bad_variant_access{};
                            }
                        }
                    });
                }
            });

            // failed at this point - operation cannot reach this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    // if operation fails, throws std::bad_variant_access
    template<typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list>, std::variant<EleTypes...>>
    operator-(const std::variant<EleTypes...>& variant_a,
        const std::variant<EleTypes...>& variant_b)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &variant_a, &variant_b](auto first)
            {
                constexpr auto Size = sizeof...(EleTypes);

                if(auto a = std::get_if<first.Index>(&variant_a))
                {
                    types::for_workhorse<Size>([&](auto second)
                    {
                        if(auto b = std::get_if<second.Index>(&variant_b))
                        {
                            using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                            using ele_b_t = types::remove_cvref_t<decltype(*b)>;

                            if constexpr(types::is_subtraction_valid_v<ele_a_t, ele_b_t>)
                            {
                                result = (*a) - (*b);

                                // operation succeeded - abort operation
                                throw types::no_type_t{};
                            }
                            else
                            {
                                // invalid operation
                                throw std::bad_variant_access{};
                            }
                        }
                    });
                }
            });

            // failed at this point - operation cannot reach this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    // if operation fails, throws std::bad_variant_access
    template<typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list>, std::variant<EleTypes...>>
    operator*(const std::variant<EleTypes...>& variant_a,
        const std::variant<EleTypes...>& variant_b)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &variant_a, &variant_b](auto first)
            {
                constexpr auto Size = sizeof...(EleTypes);

                if(auto a = std::get_if<first.Index>(&variant_a))
                {
                    types::for_workhorse<Size>([&](auto second)
                    {
                        if(auto b = std::get_if<second.Index>(&variant_b))
                        {
                            using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                            using ele_b_t = types::remove_cvref_t<decltype(*b)>;

                            if constexpr(types::is_multiplication_valid_v<ele_a_t, ele_b_t>)
                            {
                                result = (*a) * (*b);

                                // operation succeeded - abort operation
                                throw types::no_type_t{};
                            }
                            else
                            {
                                // invalid operation
                                throw std::bad_variant_access{};
                            }
                        }
                    });
                }
            });

            // failed at this point - operation cannot reach this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    // if operation fails, throws std::bad_variant_access
    template<typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list>, std::variant<EleTypes...>>
    operator/(const std::variant<EleTypes...>& variant_a,
        const std::variant<EleTypes...>& variant_b)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &variant_a, &variant_b](auto first)
            {
                constexpr auto Size = sizeof...(EleTypes);

                if(auto a = std::get_if<first.Index>(&variant_a))
                {
                    types::for_workhorse<Size>([&](auto second)
                    {
                        if(auto b = std::get_if<second.Index>(&variant_b))
                        {
                            using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                            using ele_b_t = types::remove_cvref_t<decltype(*b)>;

                            if constexpr(types::is_division_valid_v<ele_a_t, ele_b_t>)
                            {
                                result = (*a) / (*b);

                                // operation succeeded - abort operation
                                throw types::no_type_t{};
                            }
                            else
                            {
                                // invalid operation
                                throw std::bad_variant_access{};
                            }
                        }
                    });
                }
            });

            // failed at this point - operation cannot reach this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    //////////////////////////////////////////////////////////////////////////
    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator+(const T& v, const std::variant<EleTypes...>& variant)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                    
                    if constexpr(types::is_addition_valid_v<T, ele_a_t>)
                    {
                        result = v + (*a);

                        // operation completed
                        throw types::no_type_t{};
                    }
                    else
                    {
                        // invalid operation
                        throw std::bad_variant_access{};
                    }
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator+(const std::variant<EleTypes...>& variant, const T& v)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                    
                    if constexpr(types::is_addition_valid_v<ele_a_t, T>)
                    {
                        result = (*a) + v;
                        throw types::no_type_t{};  
                    }
                    else
                    {
                        // operation completed
                        throw std::bad_variant_access{};
                    }             
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator-(const T& v, const std::variant<EleTypes...>& variant)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                    
                    if constexpr(types::is_subtraction_valid_v<T, ele_a_t>)
                    {
                        result = v - (*a);

                        // operation succeeded - abort operation
                        throw types::no_type_t{};
                    }
                    else
                    {
                        // invalid operation
                        throw std::bad_variant_access{};
                    }
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator-(const std::variant<EleTypes...>& variant, const T& v)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                    
                    if constexpr(types::is_subtraction_valid_v<ele_a_t, T>)
                    {
                        result = (*a) - v;

                        // operation succeeded - abort operation
                        throw types::no_type_t{};  
                    }
                    else
                    {
                        // operation completed
                        throw std::bad_variant_access{};
                    }             
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator*(const T& v, const std::variant<EleTypes...>& variant)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                    
                    if constexpr(types::is_multiplication_valid_v<T, ele_a_t>)
                    {
                        result = v * (*a);

                        // operation succeeded - abort operation
                        throw types::no_type_t{};
                    }
                    else
                    {
                        // invalid operation
                        throw std::bad_variant_access{};
                    }
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator*(const std::variant<EleTypes...>& variant, const T& v)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                    
                    if constexpr(types::is_multiplication_valid_v<ele_a_t, T>)
                    {
                        result = (*a) * v;

                        // operation succeeded - abort operation
                        throw types::no_type_t{};  
                    }
                    else
                    {
                        // operation completed
                        throw std::bad_variant_access{};
                    }             
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator/(const T& v, const std::variant<EleTypes...>& variant)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                    
                    if constexpr(types::is_division_valid_v<T, ele_a_t>)
                    {
                        result = v / (*a);

                        // operation succeeded - abort operation
                        throw types::no_type_t{};
                    }
                    else
                    {
                        // invalid operation
                        throw std::bad_variant_access{};
                    }
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }

    template<typename T, typename ...EleTypes, typename list = types::type_list_t<EleTypes...>>
    std::enable_if_t<!types::is_type_in_list_v<types::no_throw_t, list> && 
        types::is_type_in_list_v<T, list>, std::variant<EleTypes...>>
    operator/(const std::variant<EleTypes...>& variant, const T& v)
    {
        constexpr auto Size = sizeof...(EleTypes);

        std::variant<EleTypes...> result;

        try
        {
            types::for_workhorse<Size>([&result, &v, &variant](auto indexer)
            {
                if(auto a = std::get_if<indexer.Index>(&variant))
                {
                    using ele_a_t = types::remove_cvref_t<decltype(*a)>;
                    
                    if constexpr(types::is_division_valid_v<ele_a_t, T>)
                    {
                        result = (*a) / v;

                        // operation succeeded - abort operation
                        throw types::no_type_t{};  
                    }
                    else
                    {
                        // operation failed
                        throw std::bad_variant_access{};
                    }             
                }
            });

            // failed at this point
        }
        catch(const types::no_type_t&) {  }

        return result;
    }
    
    template<auto N1, auto D1, auto N2, auto D2>
    constexpr auto operator*(std::ratio<N1, D1>, std::ratio<N2, D2>)
    {
        /*
            N1      N2      N1     N2
          ----- x ----- = ----- x ----
            D1      D2      D2     D1
        */

       constexpr std::ratio<N1, D2> aa;
       constexpr std::ratio<N2, D1> bb;

       return std::ratio<aa.num * bb.num, aa.den * bb.den>{};
    }

    template<auto N1, auto D1, auto N2, auto D2>
    constexpr auto operator/(std::ratio<N1, D1> a, std::ratio<N2, D2>)
    {
        constexpr std::ratio<D2, N2> bb;

        return a * bb;
    }

    template<auto N1, auto D1, auto N2, auto D2>
    constexpr auto operator+(std::ratio<N1, D1>, std::ratio<N2, D2>)
    {
        /*
            N1      N2     (D2/g) * N1 + (D1/g) * N2
          ----- + ----- = ---------------------------
            D1      D2           LCM(D1, D2)
            
        */

       constexpr auto g = std::gcd(D1, D2); // GCD - Greatest Common Divisor
       constexpr auto l = std::lcm(D1, D2); // LCM - Least Common Multiple

       constexpr auto d2_g = D2 / g; // evenly divisible
       constexpr auto d1_g = D1 / g; // evenly divisible

      return std::ratio<d2_g * N1 + d1_g * N2, l>{};
    }

    template<auto N1, auto D1, auto N2, auto D2>
    constexpr auto operator-(std::ratio<N1, D1>, std::ratio<N2, D2>)
    {
        /*
            N1      N2     (D2/g) * N1 - (D1/g) * N2
          ----- + ----- = ---------------------------
            D1      D2           LCM(D1, D2)
            
        */

       constexpr auto g = std::gcd(D1, D2); // GCD - Greatest Common Divisor
       constexpr auto l = std::lcm(D1, D2); // LCM - Least Common Multiple

       constexpr auto d2_g = D2 / g; // evenly divisible
       constexpr auto d1_g = D1 / g; // evenly divisible

      return std::ratio<d2_g * N1 - d1_g * N2, l>{};
    }

} // end of namespace std

 #pragma warning(pop)

#ifdef _TPF_STD_EXTENSIONS_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_STD_EXTENSIONS_MIN_DEFINED
#endif 

#ifdef _TPF_STD_EXTENSIONS_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_STD_EXTENSIONS_MAX_DEFINED
#endif 

#endif // end of file _TPF_STD_EXTENSIONS_HPP