/**
 * @file tpf_set.hpp
 * @author your name (you@domain.com)
 * @brief Implements set operations.
 * @version 0.1
 * @date 2019-04-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef _TPF_SET_HPP
#define _TPF_SET_HPP

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <deque>
#include <exception>
#include <cassert>
#include <sstream>
#include <string>
#include <algorithm>

#include <tpf_types.hpp>
#include <tpf_output.hpp>
#include <tpf_ncrnpr.hpp>

#ifdef max
#define _TPF_SET_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_SET_MIN_DEFINED
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
    namespace set
    {
        // using set_tag = tpf::set_tag<int, std::vector>;

        // using element_t = set_tag::element_t;
        // using set_t = set_tag::set_t;
        // using sets_t = set_tag::sets_t;
        // using set_of_sets_t = set_tag::set_of_sets_t;
        // using sets_of_sets_t = set_tag::sets_of_sets_t;
        
        enum class sort_order { ascending, descending };
        enum class sort_method { dictionary, size };

        template<typename Type>
        constexpr auto count_of_subsets(Type element_count)
        {
            return tpf::two_power_n(element_count);
        }
      
        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, void>
        smart_shrink_to_fit(ContainerType<EleType, Types...>& container, size_t factor = 2)
        {
            using container_t = ContainerType<EleType, Types...>;

            if constexpr(types::is_shrink_to_fit_available_v<container_t>
                && types::is_capacity_available_v<container_t>)
                {
                    if(container.empty())
                    {
                        container.shrink_to_fit();
                    }
                    else
                    {
                        if(container.capacity() > (container.size() * factor + container.size()/factor))
                            container.shrink_to_fit();
                    }
                }
        }

        // if same, returns 0
        // if left < right, returns -1
        // otherwise, returns +1
        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, int>
        compare_sets_dictionary(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
        {
            size_t left_size = left.size();
            size_t right_size = right.size();

            // if element counts are equal
            if (left_size == right_size)
            {
                if (left_size == 0) // if empty, the are the same
                    return 0;
                else
                {
                    for (size_t i = 0; i < left_size; ++i)
                    {
                        if (left[i] < right[i])
                            return -1;
                        else if (left[i] > right[i])
                            return 1;
                    }

                    return 0;
                }
            }
            else // if element counts are NOT equal
            {
                size_t size = left_size < right_size ? left_size : right_size;

                for (size_t i = 0; i < size; ++i)
                {
                    if (left[i] < right[i])
                        return -1;
                    else if (left[i] > right[i])
                        return 1;
                }

                return left_size < right_size ? -1 : 1;
            }
        }

        // if same, returns 0
        // if left < right, returns -1
        // otherwise, returns +1
        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, int>
        compare_sets_size(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
        {
            size_t left_size = left.size();
            size_t right_size = right.size();

            // if element counts are equal
            if (left_size == right_size)
            {
                if (left_size == 0) // if empty, the are the same
                    return 0;
                else
                {
                    for (size_t i = 0; i < left_size; ++i)
                    {
                        if (left[i] < right[i])
                            return -1;
                        else if (left[i] > right[i])
                            return 1;
                    }

                    return 0;
                }
            }
            else // if element counts are NOT equal
            {
                return left_size < right_size ? -1 : 1;
            }
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        operator == (const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
        {
            // if element count is different
            if(left.size() != right.size())
                return false;
            else // if element counts are the same
            {
                if(left.empty())
                    return true;
                else
                {
                    // the element counts of left and right are the same
                     size_t size = left.size();
                     for(size_t i= 0; i <size; ++i)
                     {
                         if(left[i] != right[i])
                            return false;
                     }

                     return true;
                }
            }
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        operator != (const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
        {
            return !(left == right);
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        compare_less_dictionary(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_dictionary(left, right) < 0 ? true : false;
	    }
        
        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        compare_less_equal_dictionary(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_dictionary(left, right) <= 0 ? true : false;
	    }

	    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        compare_greater_dictionary(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_dictionary(left, right) > 0 ? true : false;
	    }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        compare_greater_equal_dictionary(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_dictionary(left, right) >= 0 ? true : false;
	    }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        compare_less_size(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_size(left, right) < 0 ? true : false;
	    }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        compare_less_equal_size(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_size(left, right) < 0 ? true : false;
	    }

	    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        compare_greater_size(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_size(left, right) > 0 ? true : false;
	    }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        compare_greater_equal_size(const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_size(left, right) >= 0 ? true : false;
	    }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        operator < (const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_size(left, right) < 0 ? true : false;
	    }

	    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        operator > (const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_size(left, right) > 0 ? true : false;
	    }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        operator <= (const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_size(left, right) <= 0 ? true : false;
	    }

	    template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        operator >= (const ContainerType<EleType, Types...>& left, const ContainerType<EleType, Types...>& right)
	    {
		    return compare_sets_size(left, right) >= 0 ? true : false;
	    }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        is_in_container(const EleType& ele, const ContainerType<EleType, Types...>& container)
        {
            return std::binary_search(container.cbegin(), container.cend(), ele);
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
        is_in_unsorted_container(const EleType& ele, const ContainerType<EleType, Types...>& container)
        {
            if(container.empty()) return false;

            for(const auto& e: container)
            {
                if(e == ele) return true;
            }

            return false;
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool>
	    is_sorted(const ContainerType<EleType, Types...>& container, 
            sort_order order = sort_order::ascending, 
            sort_method method=sort_method::size)
        {
            using container_t = const ContainerType<EleType, Types...>&;

            if(container.empty()) return true;

            if constexpr(tpf::types::is_container_type_v<EleType>)
            {
                auto sort_predicate = [&order, &method](const auto& left, const auto& right)
                {
                    if(method == sort_method::dictionary)
                    {
                        if(order == sort_order::ascending)
                        {
                            return compare_less_equal_dictionary(left, right); 
                        }
                        else
                        { // sort_order::descending
                            return compare_greater_equal_dictionary(left, right);
                        }
                    }
                    else // sort_method::size
                    {
                        if(order == sort_order::ascending)
                        {
                            return compare_less_equal_size(left, right); 
                        }
                        else
                        { // sort_order::descending
                            return compare_greater_equal_size(left, right);
                        }
                    }
                };

                for(const auto& e: container)
                    if(!is_sorted(e)) return false;   
                
                size_t size = container.size();

                for(size_t i=1; i<size; ++i)
                {
                    // should be optimized for list type
                    if(!sort_predicate(tpf::get_element<container_t>(container, i-1),
                        tpf::get_element<container_t>(container, i)))
                        return false;
                }

                return true;
            }
            else
            {
                size_t size = container.size();
                
                for(size_t i = 1; i < size; ++i)
                {
                    if(tpf::get_element<container_t>(container, i-1) > tpf::get_element<container_t>(container, i))
                        return false;
                }

                return true;    
            }
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    types::enable_if_container_type_t<ContainerType<EleType, Types...>, bool> 
        sort_in_place(ContainerType<EleType, Types...>& container, 
            sort_order order = sort_order::ascending, 
            sort_method method = sort_method::size)
	    {
            if(container.empty()) return false; // do need to sort?
            
            if(is_sorted(container)) return false;  // do need to sort?
            
            if constexpr(tpf::types::is_container_type_v<EleType>)
            {
                auto sort_predicate = [&order, &method](const auto& left, const auto& right)
                {
                    if(method == sort_method::dictionary)
                    {
                        if(order == sort_order::ascending)
                        {
                            return compare_less_equal_dictionary(left, right); 
                        }
                        else
                        { // sort_order::descending
                            return compare_greater_equal_dictionary(left, right);
                        }
                    }
                    else // sort_method::size
                    {
                        if(order == sort_order::ascending)
                        {
                            return compare_less_equal_size(left, right); 
                        }
                        else
                        { // sort_order::descending
                            return compare_greater_equal_size(left, right);
                        }
                    }
                };

                bool bNeedToSort = false;
                // sort elements themselves
                for(auto& e: container)
                {
                    if(sort_in_place(e, order, method))
                        bNeedToSort = true;
                }
                
                if(!bNeedToSort) return false;
                
                // outer most sort
                std::sort(container.begin(), container.end(), sort_predicate);
                container.erase(std::unique(container.begin(), container.end()), container.end());
                smart_shrink_to_fit(container);
                return true;
            }
            else
            {
                std::sort(container.begin(), container.end());
                container.erase(std::unique(container.begin(), container.end()), container.end());
                smart_shrink_to_fit(container);
                return true;
            }        
   	    }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
	    ContainerType<EleType> sort(ContainerType<EleType, Types...> container,
            sort_order order = sort_order::ascending, 
            sort_method method = sort_method::size)
	    {
            sort_in_place(container, order, method);
            return container;        
   	    }

        template<template<typename, typename...> class ContainerType,
            typename EleType, typename RightContainerType, typename... Types>
        std::enable_if_t<std::is_same_v<ContainerType<EleType, Types...>, remove_cv_ref_t<RightContainerType>>>
            union_in_place(ContainerType<EleType, Types...>& left, RightContainerType&& right)
        {
            if constexpr(std::is_rvalue_reference_v<decltype(right)>)
            {
                left.insert(left.end(), 
                    std::make_move_iterator(right.begin()), 
                    std::make_move_iterator(right.end()));
                sort_in_place(left);
            }
            else
            {
                left.insert(left.end(), right.cbegin(), right.cend());
                sort_in_place(left);
            }
        }

        template<template<typename, typename...> class ContainerType,
            typename EleType, typename RightContainerType1, typename RightContainerType2, typename... Types>
        std::enable_if_t<std::is_same_v<ContainerType<EleType, Types...>, remove_cv_ref_t<RightContainerType1>>&&
                        std::is_same_v<ContainerType<EleType, Types...>, remove_cv_ref_t<RightContainerType2>>>
            union_in_place(ContainerType<EleType, Types...>& left, 
                RightContainerType1&& right1, RightContainerType2&& right2)
        {
            if constexpr(std::is_rvalue_reference_v<decltype(right1)>)
                left.insert(left.end(),
                    std::make_move_iterator(right1.begin()), 
                    std::make_move_iterator(right1.end()));
            else
                left.insert(left.end(), right1.cbegin(), right1.cend());

            if constexpr(std::is_rvalue_reference_v<decltype(right2)>)
                left.insert(left.end(),
                    std::make_move_iterator(right2.begin()), 
                    std::make_move_iterator(right2.end()));
            else
                left.insert(left.end(), right2.cbegin(), right2.cend());

            sort_in_place(left);    
        }

        template<template<typename, typename...> class ContainerType,
            typename EleType, typename RightContainerType, typename... Types>
        std::enable_if_t<std::is_same_v<ContainerType<EleType, Types...>, 
            remove_cv_ref_t<RightContainerType>>, ContainerType<EleType, Types...>>
        union_sets(ContainerType<EleType, Types...> left, RightContainerType&& right)
        {
            union_sets_in_place(left, std::forward<RightContainerType>(right));
            return left;
        }

        template<template<typename, typename...> class ContainerType,
            typename EleType, typename RightContainerType1, typename RightContainerType2, typename... Types>
        std::enable_if_t<std::is_same_v<ContainerType<EleType, Types...>, remove_cv_ref_t<RightContainerType1>>&&
                        std::is_same_v<ContainerType<EleType, Types...>, remove_cv_ref_t<RightContainerType2>>,
                        ContainerType<EleType, Types...>>
        union_sets(ContainerType<EleType, Types...> left, 
                RightContainerType1&& right1, RightContainerType2&& right2)
        {
            if constexpr(std::is_rvalue_reference_v<decltype(right1)>)
                left.insert(left.end(),
                    std::make_move_iterator(right1.begin()), 
                    std::make_move_iterator(right1.end()));
            else
                left.insert(left.end(), right1.cbegin(), right1.cend());

            if constexpr(std::is_rvalue_reference_v<decltype(right2)>)
                left.insert(left.end(),
                    std::make_move_iterator(right2.begin()), 
                    std::make_move_iterator(right2.end()));
            else
                left.insert(left.end(), right2.cbegin(), right2.cend());

            sort_in_place(left); return left;
        }

        template<typename EleType, template<typename, typename...> class ContainerType,
            typename... Types1, typename... Types2>
        ContainerType<EleType, Types1...> union_flat(const ContainerType<ContainerType<EleType, Types1...>, Types2...>& sets)
        {
            ContainerType<EleType, Types1...> set;

            for(const auto& s: sets)
                set.insert(set.begin(), s.cbegin(), s.cend());

            union_sets_in_place(set); return set;
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        ContainerType<EleType, Types...> intersection(const ContainerType<EleType, Types...>& left,
            const ContainerType<EleType, Types...>& right)
        {
            if(left.empty() || right.empty())
                return {};

            ContainerType<EleType, Types...> container;

            for(const auto& s: left)
            {
                if(is_in_container(s, right))
                    container.push_back(s);
            }

            return container;
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        ContainerType<EleType, Types...> intersection(const ContainerType<EleType, Types...>& left,
            const ContainerType<EleType, Types...>& right1, const ContainerType<EleType, Types...>& right2)
        {
            if(left.empty() || right1.empty() || right2.empty())
                return {};

            ContainerType<EleType, Types...> container;

            for(const auto& s: left)
            {
                if(is_in_container(s, right1) && is_in_container(s, right2))
                    container.push_back(s);
            }

            return container;
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        ContainerType<EleType, Types...> difference(const ContainerType<EleType, Types...>& left,
            const ContainerType<EleType, Types...>& right)
        {
            ContainerType<EleType, Types...> container;

            for(const auto& s: left)
            {
                if(!is_in_container(s, right))
                    container.push_back(s);
            }

            return container;
        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        ContainerType<EleType, Types...> difference(const ContainerType<EleType, Types...>& left,
            const ContainerType<EleType, Types...>& right1, const ContainerType<EleType, Types...>& right2)
        {
            ContainerType<EleType, Types...> container;

            for(const auto& s: left)
            {
                if(!is_in_container(s, right1) && !is_in_container(s, right2))
                    container.push_back(s);
            }

            return container;
        }

        namespace hidden
	    {
            template<template<typename, typename...> class ContainerType, 
                typename EleType, typename... Types, typename... OuterTypes>
            void build_subsets(ContainerType<ContainerType<EleType, Types...>, OuterTypes...>& SS,
                ContainerType<EleType, Types...> R, ContainerType<EleType, Types...> S, size_t count)
            {
                using set_t = ContainerType<EleType, Types...>;
                using sets_t = ContainerType<ContainerType<EleType, Types...>, OuterTypes...>;

                if (count > S.size()) { return; } // RULE 1
                else if (count == 0 && !R.empty()) // RULE 4
                {
                    SS.emplace_back(std::move(R)); return;
                }
                else if (S.size() == count) // RULE 2
                {
                    R.insert(R.end(), std::make_move_iterator(S.begin()),
                                      std::make_move_iterator(S.end()));
                                      
                    SS.emplace_back(std::move(R));

                    return;
                }
                else
                {
                    // make a room for new element
                    R.emplace_back(int{});

                    while (count <= S.size()) // RULE 2
                    {
                        // copy first element of S
                        // to the end of R
                        R.back() = types::pop_front(S);
                        build_subsets(SS, R, S, count - 1);
                    }
                }
            }
            

            template<template<typename, typename...> class ContainerType, typename EleType, typename... Types>
            void build_permutations(ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R, size_t r);

            template<template<typename, typename...> class ContainerType, 
                typename EleType, typename... Types, typename... OuterTypes>
            void build_permutations(ContainerType<ContainerType<EleType, Types...>, OuterTypes...>& permutations,
                ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R, size_t r);

            template<template<typename, typename...> class ContainerType, typename EleType, typename... Types>
            void build_permutations_remainder(ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R, size_t r);

            template<template<typename, typename...> class ContainerType, 
                typename EleType, typename... Types, typename... OuterTypes>
            void build_permutations_remainder(ContainerType<ContainerType<EleType, Types...>, OuterTypes...>& permutations,
                ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R, size_t r);

            template<template<typename, typename...> class ContainerType, 
                typename EleType, typename... Types, typename... OuterTypes>
            void build_permutations(tpf::thread_bundle& tb, ContainerType<ContainerType<EleType, Types...>, OuterTypes...>& permutations,
                ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R, size_t r)
                {
                    using set_t = ContainerType<EleType, Types...>;
                    using sets_t = ContainerType<ContainerType<EleType, Types...>, OuterTypes...>;
                    using smart_set_t = std::shared_ptr<set_t>;
                    using smart_sets_t = std::shared_ptr<sets_t>;

                    if(r==0 || L.empty())
                    {
                        permutations.emplace_back(R);
                    }
                    else
                    {
                        size_t size = L.size();
                        
                        // make a room for last element
                        R.emplace_back(EleType{});

                        // if an exception is thrown before R.pop_back() is reached
                        // this program does not work properly
                        // we need to handle this issue in EXCEPTION-SAFE manner
                        
                        auto r_pop_back = [](auto ptr) { ptr->pop_back(); };
                        using r_type_t = std::remove_reference_t<decltype(R)>;

                        // clean_up is also a local variable
                        std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                            clean_up(&R, r_pop_back);
                        
                        using future_t = std::future<sets_t>;
                        using futures_t = std::vector<future_t>;
                        
                        futures_t futures;

                        for(size_t i_th = 0; i_th < size; ++i_th)
                        {
                            // copy L to copied_L
                            auto copied_L = L;

                            // remove i_th element from copied_L
                            copied_L.erase(copied_L.begin()+i_th);

                            // move i_th element from L to R
                            // R.emplace_back( L[i_th] );
                            R.back() = L[i_th];

                            if(size < 7 || tb.thread_count >= tb.max_thread_count)
                                build_permutations(permutations, copied_L, R, r-1);
                            else
                            {
                               // smart_set_t shared_L{new set_t{copied_L}};
                               // smart_set_t shared_R{new set_t{R}};
                               // smart_sets_t shared_permutation{new sets_t{}};

                               auto go_parallel = [&tb, permutation = sets_t{},
                                L = copied_L, R = R, r=r]() mutable
                               {
                                  build_permutations(tb, permutation, L, R, r-1);
                                  
                                  return permutation;
                               };

                               futures.emplace_back(std::async(std::launch::async, go_parallel));
                               ++tb.thread_count;
                            }
                        }

                        for(auto& f: futures)
                        {
                            auto lps = f.get(); --tb.thread_count;

                            permutations.insert(permutations.end(),
                                std::make_move_iterator(lps.begin()),
                                std::make_move_iterator(lps.end()));
                        }
                    }  

                } // end of build_permutations()

            template<template<typename, typename...> class ContainerType, 
                typename EleType, typename... Types, typename... OuterTypes>
            void build_permutations(ContainerType<ContainerType<EleType, Types...>, OuterTypes...>& permutations,
                ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R)
                {
                    using set_t = ContainerType<EleType, Types...>;
                    using sets_t = ContainerType<ContainerType<EleType, Types...>, OuterTypes...>;
                    using smart_set_t = std::shared_ptr<set_t>;
                    using smart_sets_t = std::shared_ptr<sets_t>;

                    if(L.empty())
                    {
                        permutations.emplace_back(R);
                    }
                    else
                    {
                        size_t size = L.size();
                        
                        // make a room for last element
                        R.emplace_back(EleType{});

                        // if an exception is thrown before R.pop_back() is reached
                        // this program does not work properly
                        // we need to handle this issue in EXCEPTION-SAFE manner
                        
                        auto r_pop_back = [](auto ptr) { ptr->pop_back(); };
                        using r_type_t = std::remove_reference_t<decltype(R)>;

                        // clean_up is also a local variable
                        std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                            clean_up(&R, r_pop_back);
                        
                        for(size_t i_th = 0; i_th < size; ++i_th)
                        {
                            // copy L to copied_L
                            auto copied_L = L;

                            // remove i_th element from copied_L
                            copied_L.erase(copied_L.begin()+i_th);

                            // move i_th element from L to R
                            // R.emplace_back( L[i_th] );
                            R.back() = L[i_th];

                            build_permutations(permutations, copied_L, R);
                        }    
                    }  

                } // end of build_permutations()
              
                template<template<typename, typename...> class ContainerType, 
                typename EleType, typename... Types, typename... OuterTypes>
            void build_permutations(ContainerType<ContainerType<EleType, Types...>, OuterTypes...>& permutations,
                ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R, size_t r)
                {
                    using set_t = ContainerType<EleType, Types...>;
                    using sets_t = ContainerType<ContainerType<EleType, Types...>, OuterTypes...>;
                    using smart_set_t = std::shared_ptr<set_t>;
                    using smart_sets_t = std::shared_ptr<sets_t>;

                    if(r == 0 || L.empty())
                    {
                        permutations.emplace_back(R);
                    }
                    else
                    {
                        size_t size = L.size();
                        
                        // make a room for last element
                        R.emplace_back(EleType{});

                        // if an exception is thrown before R.pop_back() is reached
                        // this program does not work properly
                        // we need to handle this issue in EXCEPTION-SAFE manner
                        
                        auto r_pop_back = [](auto ptr) { ptr->pop_back(); };
                        using r_type_t = std::remove_reference_t<decltype(R)>;

                        // clean_up is also a local variable
                        std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                            clean_up(&R, r_pop_back);
                        
                        for(size_t i_th = 0; i_th < size; ++i_th)
                        {
                            // copy L to copied_L
                            auto copied_L = L;

                            // remove i_th element from copied_L
                            copied_L.erase(copied_L.begin()+i_th);

                            // move i_th element from L to R
                            // R.emplace_back( L[i_th] );
                            R.back() = L[i_th];

                            build_permutations(permutations, copied_L, R, r-1);
                        }    
                    }  

                } // end of build_permutations()


            template<template<typename, typename...> class ContainerType, 
            typename EleType, typename... Types, typename... OuterTypes>
            void build_permutations_remainder(ContainerType<ContainerType<EleType, Types...>, OuterTypes...>& permutations,
                ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R, size_t r)
                {
                    using set_t = ContainerType<EleType, Types...>;
                    using sets_t = ContainerType<ContainerType<EleType, Types...>, OuterTypes...>;
                    using smart_set_t = std::shared_ptr<set_t>;
                    using smart_sets_t = std::shared_ptr<sets_t>;

                    if(r == 0 || L.empty())
                    {
                        auto S = R;
                        if(!L.empty())
                        {
                            for(auto e: L)
                                S.push_back(e);
                        }
                        
                        permutations.emplace_back(S);
                    }
                    else
                    {
                        size_t size = L.size();
                        
                        // make a room for last element
                        R.emplace_back(EleType{});

                        // if an exception is thrown before R.pop_back() is reached
                        // this program does not work properly
                        // we need to handle this issue in EXCEPTION-SAFE manner
                        
                        auto r_pop_back = [](auto ptr) { ptr->pop_back(); };
                        using r_type_t = std::remove_reference_t<decltype(R)>;

                        // clean_up is also a local variable
                        std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                            clean_up(&R, r_pop_back);
                        
                        for(size_t i_th = 0; i_th < size; ++i_th)
                        {
                            // copy L to copied_L
                            auto copied_L = L;

                            // remove i_th element from copied_L
                            copied_L.erase(copied_L.begin()+i_th);

                            // move i_th element from L to R
                            // R.emplace_back( L[i_th] );
                            R.back() = L[i_th];

                            build_permutations_remainder(permutations, copied_L, R, r-1);
                        }    
                    }  

                } // end of build_permutations_remainder()

/////////////////////////////////////////////////////////////////////////////////////////////////////
            template<template<typename, typename...> class ContainerType, typename EleType, typename... Types>
            void build_permutations(tpf::thread_bundle& tb, ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R)
                {
                    using set_t = ContainerType<EleType, Types...>;
                    using smart_set_t = std::shared_ptr<set_t>;
               
                    if(L.empty())
                    {
                        // permutations.emplace_back(R);
                    }
                    else
                    {
                        size_t size = L.size();
                        
                        // make a room for last element
                        R.emplace_back(EleType{});

                        // if an exception is thrown before R.pop_back() is reached
                        // this program does not work properly
                        // we need to handle this issue in EXCEPTION-SAFE manner
                        
                        auto r_pop_back = [](auto ptr) { ptr->pop_back(); };
                        using r_type_t = std::remove_reference_t<decltype(R)>;

                        // clean_up is also a local variable
                        std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                            clean_up(&R, r_pop_back);
                        
                        using future_t = std::future<void>;
                        using futures_t = std::vector<future_t>;
                        
                        futures_t futures;

                        for(size_t i_th = 0; i_th < size; ++i_th)
                        {
                            // copy L to copied_L
                            auto copied_L = L;

                            // remove i_th element from copied_L
                            copied_L.erase(copied_L.begin()+i_th);

                            // move i_th element from L to R
                            // R.emplace_back( L[i_th] );
                            R.back() = L[i_th];

                            if(size < 7 || tb.thread_count >= tb.max_thread_count)
                                build_permutations(copied_L, R);
                            else
                            {
                               smart_set_t shared_L{new set_t{copied_L}};
                               smart_set_t shared_R{new set_t{R}};
                               
                               auto go_parallel = [&tb, shared_L, shared_R]()
                               {
                                  build_permutations(tb, *shared_L, *shared_R);
                               };

                               futures.emplace_back(std::async(std::launch::async, go_parallel));
                               ++tb.thread_count;
                            }
                        }

                        for(auto& f: futures)
                        {
                            f.wait(); --tb.thread_count;
                        }
                    }  

                } // end of build_permutations()

            template<template<typename, typename...> class ContainerType, typename EleType, typename... Types>
            void build_permutations(ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R)
                {
                    using set_t = ContainerType<EleType, Types...>;
                    using smart_set_t = std::shared_ptr<set_t>;
                  
                    if(L.empty())
                    {
                        // permutations.emplace_back(R);
                    }
                    else
                    {
                        size_t size = L.size();
                        
                        // make a room for last element
                        R.emplace_back(EleType{});

                        // if an exception is thrown before R.pop_back() is reached
                        // this program does not work properly
                        // we need to handle this issue in EXCEPTION-SAFE manner
                        
                        auto r_pop_back = [](auto ptr) { ptr->pop_back(); };
                        using r_type_t = std::remove_reference_t<decltype(R)>;

                        // clean_up is also a local variable
                        std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                            clean_up(&R, r_pop_back);
                        
                        for(size_t i_th = 0; i_th < size; ++i_th)
                        {
                            // copy L to copied_L
                            auto copied_L = L;

                            // remove i_th element from copied_L
                            copied_L.erase(copied_L.begin()+i_th);

                            // move i_th element from L to R
                            // R.emplace_back( L[i_th] );
                            R.back() = L[i_th];
                            build_permutations(copied_L, R);
                        }    
                    }  

                } // end of build_permutations()

            template<template<typename, typename...> class ContainerType, typename EleType, typename... Types>
            void build_permutations(ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R, size_t r)
                {
                    using set_t = ContainerType<EleType, Types...>;
                    using smart_set_t = std::shared_ptr<set_t>;
                  
                    if(r == 0 || L.empty())
                    {
                        // permutations.emplace_back(R);
                    }
                    else
                    {
                        size_t size = L.size();
                        
                        // make a room for last element
                        R.emplace_back(EleType{});

                        // if an exception is thrown before R.pop_back() is reached
                        // this program does not work properly
                        // we need to handle this issue in EXCEPTION-SAFE manner
                        
                        auto r_pop_back = [](auto ptr) { ptr->pop_back(); };
                        using r_type_t = std::remove_reference_t<decltype(R)>;

                        // clean_up is also a local variable
                        std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                            clean_up(&R, r_pop_back);
                        
                        for(size_t i_th = 0; i_th < size; ++i_th)
                        {
                            // copy L to copied_L
                            auto copied_L = L;

                            // remove i_th element from copied_L
                            copied_L.erase(copied_L.begin()+i_th);

                            // move i_th element from L to R
                            // R.emplace_back( L[i_th] );
                            R.back() = L[i_th];
                            build_permutations(copied_L, R, r-1);
                        }    
                    }  

                } // end of build_permutations()

            template<template<typename, typename...> class ContainerType, typename EleType, typename... Types>
            void build_permutations_remainder(ContainerType<EleType, Types...>& L, ContainerType<EleType, Types...>& R, size_t r)
                {
                    using set_t = ContainerType<EleType, Types...>;
                    using smart_set_t = std::shared_ptr<set_t>;
                  
                    if(r == 0)
                    {
                        if(!L.empty())
                        {
                            for(auto e: L) R.push_back(e);   
                        }
                    }
                    else
                    {
                        size_t size = L.size();
                        
                        // make a room for last element
                        R.emplace_back(EleType{});

                        // if an exception is thrown before R.pop_back() is reached
                        // this program does not work properly
                        // we need to handle this issue in EXCEPTION-SAFE manner
                        
                        auto r_pop_back = [](auto ptr) { ptr->pop_back(); };
                        using r_type_t = std::remove_reference_t<decltype(R)>;

                        // clean_up is also a local variable
                        std::unique_ptr<r_type_t, decltype(r_pop_back)> 
                            clean_up(&R, r_pop_back);
                        
                        for(size_t i_th = 0; i_th < size; ++i_th)
                        {
                            // copy L to copied_L
                            auto copied_L = L;

                            // remove i_th element from copied_L
                            copied_L.erase(copied_L.begin()+i_th);

                            // move i_th element from L to R
                            // R.emplace_back( L[i_th] );
                            R.back() = L[i_th];
                            build_permutations_remainder(copied_L, R, r-1);
                        }    
                    }  

                } // end of build_permutations_remainder()

        } // end of namespace hidden

        template<typename ThreadCountType, typename NType, typename RType>
        auto build_permutations(ThreadCountType thread_count, NType n, RType r)
        {
            using set_tag = tpf::set_tag<std::vector, short>;
            using set_t = tpf::set_t<set_tag>;
            using sets_t = tpf::sets_t<set_tag>;

            set_t L; L.reserve(n);
            for(size_t i= 0; i < (size_t)n; ++i)
                L.emplace_back((short)i);

            set_t R; R.reserve(n);

            sets_t permutations;

            tpf::thread_bundle tb;
            tb.max_thread_count = thread_count;
            tb.thread_count = 0;

            tpf::sstream stream;

            try
            {
                hidden::build_permutations(tb, permutations, L, R, r);

            }catch(const std::exception& e)
            {
                stream << e << tpf::endl;
            }
            return permutations;
        }

        template<typename NType, typename RType>
        auto build_permutations(NType n, RType r)
        {
            using set_tag = tpf::set_tag<std::vector, NType>;
            using set_t = tpf::set_t<set_tag>;
            using sets_t = tpf::sets_t<set_tag>;

            set_t L; L.reserve(n);
            for(size_t i= 0; i < (size_t)n; ++i)
                L.emplace_back((NType)i);

            set_t R; R.reserve(n);

            sets_t permutations;

            tpf::sstream stream;

            try
            {
                hidden::build_permutations(permutations, L, R, r);

            }catch(const std::exception& e)
            {
                stream << e << tpf::endl;
            }
            return permutations;
        }

        template<typename NType, typename RType>
        auto build_permutations_remainder(NType n, RType r)
        {
            using set_tag = tpf::set_tag<std::vector, NType>;
            using set_t = tpf::set_t<set_tag>;
            using sets_t = tpf::sets_t<set_tag>;

            set_t L; L.reserve(n);
            for(size_t i= 0; i < (size_t)n; ++i)
                L.emplace_back((NType)i);

            set_t R; R.reserve(n);

            sets_t permutations;

            tpf::sstream stream;

            try
            {
                hidden::build_permutations_remainder(permutations, L, R, r);

            }catch(const std::exception& e)
            {
                stream << e << tpf::endl;
            }
            return permutations;
        }

        /////////////////////////////////////////////////////////////////////////////////////
        
        template<typename ThreadCountType, typename NType, typename RType>
        auto build_permutations_flat(ThreadCountType thread_count, NType n, RType r)
        {
            using set_tag = tpf::set_tag<std::vector, NType>;
            using set_t = tpf::set_t<set_tag>;
            using sets_t = tpf::sets_t<set_tag>;

            set_t L; L.reserve(n);
            for(size_t i= 0; i < (size_t)n; ++i)
                L.emplace_back((NType)i);

            set_t R; R.reserve(n);

            tpf::thread_bundle tb;
            tb.max_thread_count = thread_count;
            tb.thread_count = 0;

            tpf::sstream stream;
            try
            {
                hidden::build_permutations(tb, L, R);

            }catch(const std::exception& e)
            {
                stream << e << tpf::endl;
            }
            
        }

        template<typename NType, typename RType>
        auto build_permutations_flat(NType n, RType r)
        {
            using set_tag = tpf::set_tag<std::vector, NType>;
            using set_t = tpf::set_t<set_tag>;
            using sets_t = tpf::sets_t<set_tag>;

            set_t L; L.reserve(n);
            for(size_t i= 0; i < (size_t)n; ++i)
                L.emplace_back((NType)i);

            set_t R; R.reserve(n);

            tpf::sstream stream;

            try
            {
                hidden::build_permutations(L, R);

            }catch(const std::exception& e)
            {
                stream << e << tpf::endl;
            }

            return R;
          
        }

        template<template<typename, typename...> class ContainerType, 
            typename EleType, typename... Types>
        ContainerType<ContainerType<EleType, Types...>> 
            build_subsets(const ContainerType<EleType, Types...>& S, size_t count = InvalidIndex)
        {
            using set_t = ContainerType<EleType>;
            using sets_t = ContainerType<ContainerType<EleType>>;

            sets_t subsets;

            if (count == 0 || S.empty())
            {
                subsets.emplace_back(set_t{});
                return subsets;
            }

            constexpr size_t limit = std::numeric_limits<size_t>::max();

            // generate all subsets
            if (count == limit)
            {
                // empty set
                subsets.emplace_back(set_t{});
                size_t size = S.size();
                    
                auto go_parallel = [&subsets, &S](auto i)
                {
                    set_t R{};
                    hidden::build_subsets(subsets, R, S, i);
                };

                #if defined(_GLIBCXX_PARALLEL)
                    #pragma omp parallel for
                    for (size_t i = 1; i < size; ++i)
                    {
                        set_t R{};
                        hidden::build_subsets(subsets, R, S, i);
                    }
                #elif defined(_MSC_VER)

                    concurrency::parallel_for((size_t)1, size, go_parallel);

                #elif defined(__ICL)
                    tbb::parallel_for((size_t)1, size, go_parallel);
                #else
                    for (size_t i = 1; i < size; ++i)
                    {
                        set_t R{};
                        hidden::build_subsets(subsets, R, S, i);
                    }
                #endif
                subsets.emplace_back(S);
            }
            else
            {
                assert(count <= S.size());
                set_t R{}; hidden::build_subsets(subsets, R, S, count);
            }

            return subsets;
        }

        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::deque<Type> ele should be in order
        // ele = { 0, 1, 2, 3 }
        template<template<typename, typename...> class ReturnContainer = std::vector,
            template<typename, typename...> class SetContainer = std::list, typename... ContainerTails,
            typename EleType = int, typename CountType = int>
		ReturnContainer<ReturnContainer<EleType>> 
        enumerate_combination_with_exclude(
                SetContainer<EleType, ContainerTails...> exclude_set,
                SetContainer<EleType, ContainerTails...> from_set, CountType r)
        {

        }

        template<typename EleType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, size_t>
        minimum_value_index(const ContainerType<EleType, Types...>& container)
        {
            if(container.empty()) return tpf::type_max_v<size_t>;
            
            EleType minimum = tpf::type_max_v<EleType>;
            
            auto iterator = container.cend();

            for(auto itr = container.cbegin(); itr != container.cend(); std::advance(itr, 1))
            {
                if(*itr < minimum)
                {
                    iterator = itr; minimum = *itr;
                }
            }

            return (iterator != container.cend()) ? 
                std::distance(container.cbegin(), iterator) : tpf::type_max_v<size_t>;   
        }

        template<typename EleType, typename IndexType, template<typename, typename...> class ContainerType, typename... Types>
        types::enable_if_container_type_t<ContainerType<EleType, Types...>, size_t>
        minimum_value_index(const ContainerType<EleType, Types...>& container, 
            const std::vector<IndexType>& exclude_set)
        {
            if(container.empty()) return tpf::type_max_v<size_t>;
            
            EleType minimum = tpf::type_max_v<EleType>;
            
            auto iterator = container.cend();

            for(auto itr = container.cbegin(); itr != container.cend(); std::advance(itr, 1))
            {
                auto index = std::distance(container.cbegin(), itr);
                
                if(is_in_unsorted_container((IndexType)index, exclude_set)) continue;

                if(*itr < minimum)
                {
                    iterator = itr; minimum = *itr;
                }
            }

            return (iterator != container.cend()) ? 
                std::distance(container.cbegin(), iterator) : tpf::type_max_v<size_t>;   
        }

    } // end of namespace set

} // end of namespace tpf

#ifdef _TPF_SET_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_SET_MIN_DEFINED
#endif 

#ifdef _TPF_SET_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_SET_MAX_DEFINED
#endif 

#endif // end of file _TPF_SET_HPP
