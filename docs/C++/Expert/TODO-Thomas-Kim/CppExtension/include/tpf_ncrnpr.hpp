/**
 * @file tpf_ncrnpr.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef _TPF_NCRNPR_HPP
#define _TPF_NCRNPR_HPP

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

	#if __cplusplus < 201703
		#error This library requires C++17 Standard (GNU g++ version 8.0 or clang++ version 8.0 above)
	#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <tpf_euclidean.hpp>
#include <iostream>
#include <sstream>
#include <list>
#include <functional>
#include <iterator>
#include <future>
#include <vector>
#include <algorithm>
#include <tpf_safe_type.hpp>

#if defined(_GLIBCXX_PARALLEL)
    #include <parallel/algorithm>
#elif defined (_MSC_VER) 
    // C++2a Standard header file for Parallel Algorithm
    #include <execution>
    #include <ppl.h>
#elif defined (__ICL)
    // C++2a Standard header file for Parallel Algorithm
    #include <execution>
     #include <tbb/parallel_for.h>
#endif 


#ifdef max
#define _TPF_NCRNPR_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_NCRNPR_MIN_DEFINED
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
     * @brief Implements nCr, nPr
     * 
     */
    namespace ncrnpr
    {
        using namespace types;
        using namespace euclidean;

        /**
        * @brief Computes the inversion of a permutation
        * 
        * @param cntr for container
        * @return int, +1 for even inversion, -1 for odd inversion
        * 
        * https://en.wikipedia.org/wiki/Leibniz_formula_for_determinants
        */
        template<template<typename, typename...> class ContainerType,
            typename EleType, typename... Types>
        int sgn(const ContainerType<EleType, Types...>& cntr)
        {
            EleType count = 0, cntr_i;
            EleType size = cntr.size();
            for(EleType i = 1; i < size; ++i)
            {
                cntr_i = cntr[i];
                
                for(EleType j=0; j < i; ++j)
                    if(cntr_i < cntr[j]) ++count;
            }

            return (count%2 ? -1:1);
        }

        class range_t: public std::pair<size_t, size_t>
        {
            public:
                using index_t = size_t;
                using base_t = std::pair<size_t, size_t>;
            public:
                size_t begin() { return this->first; }
                size_t end() { return this->second; }
                size_t cbegin() const { return this->first; }
                size_t cend() const { return this->second; }
                
                base_t& base() { return static_cast<base_t&>(*this); }
                const base_t& base() const { return static_cast<const base_t&>(*this); }
                
                range_t() = default;
                range_t(const range_t&) = default;
                range_t& operator = (const range_t&) = default;
                range_t(range_t&&) = default;
                range_t& operator=(range_t&&) = default;

                template<typename Type1, typename Type2>
                range_t(Type1 start, Type2 end): 
                    std::pair<size_t, size_t>{(index_t)start, (index_t)end}
                    { }
                
                std::string str() const
                {
                    std::ostringstream os;

                    os << "[" << this->first << ", " 
                        << this->second << ")(" 
                        << (this->second - this->first)<<")";

                    return os.str();
                }

                std::wstring wstr() const
                {
                    std::wostringstream os;

                    os << L"[" << this->first << L", " 
                        << this->second << L")(" 
                        << (this->second - this->first)<<L")";

                    return os.str();
                }

            friend std::ostream& operator<<(std::ostream& os, 
                const range_t& range)
            {
                os << range.str();
                return os;
            }
        };

        using range_vector_t = std::vector<range_t>;

        /*
            st                              ed
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11

            minimum span = 3; [1, 5), [5, 8), [8, 11)

            1, 2,  3,  4, 5		-> [1, 5) : 4
            5, 6,  7,  8			-> [5, 8) : 3
            8, 9, 10,  11			-> [8, 11): 3
        */

       /**
        * @brief Split range
        * 
        * @tparam SpanType 
        * @tparam StartType 
        * @tparam EndType 
        * @param min_span 
        * @param st 
        * @param ed 
        * @return range_vector_t 
        */
        template<typename SpanType, typename StartType, typename EndType>
        range_vector_t split_range_span(SpanType min_span, StartType st, EndType ed)
        {
            size_t dist = (size_t)ed - (size_t)st;
            size_t rnd = (size_t)dist % (size_t)min_span;
            size_t count = (size_t)dist / (size_t)min_span;

            range_vector_t rng;

            if(dist <= min_span)
            {
                rng.emplace_back(range_t{(size_t)st, (size_t)ed});
                return rng;
            }
            else if (rnd == 0)
            {
                size_t prev = (size_t)st;
                for (size_t i = 0; i < count; ++i)
                {
                    rng.emplace_back(range_t{prev, prev + min_span});
                    prev += min_span;
                }
                return rng;
            }
            else
            {
                if (count >= rnd)
                {
                    size_t span1 = min_span + 1;

                    size_t prev = (size_t)st;
                    for (size_t i = 0; i < rnd; ++i)
                    {
                        rng.emplace_back(range_t(prev, prev + span1));
                        prev += span1;
                    }

                    count = count - (size_t)rnd;
                    for (size_t i = 0; i < count; ++i)
                    {
                        rng.emplace_back(range_t{prev, prev + min_span});
                        prev += min_span;
                    }
                    return rng;
                }
                else
                {
                    return split_range_span(count, st, ed);
                }
            }
        }

        template<typename CountType, typename StartType, typename EndType>
        range_vector_t split_range_count(CountType count, StartType st, EndType ed)
        {
            size_t dist = (size_t)ed - (size_t)st;
            size_t span = (size_t)dist / (size_t)count;
            size_t rnd = (size_t)dist % (size_t)count;

            range_vector_t rng;

            if (rnd == 0)
            {
                size_t prev = st;
                for (size_t i = 0; i < count; ++i)
                {
                    rng.emplace_back(range_t{prev, prev + span});
                    prev += span;
                }

                return rng;
            }
            else
            {
                size_t span1 = span + (size_t)1;

                size_t prev = st;
                for (size_t i = 0; i < rnd; ++i)
                {
                    rng.emplace_back(range_t{prev, prev + span1});
                    prev += span1;
                }

                // rnd = dist % count;
                // rnd cannot be greater than count
                count = count - rnd;
                for (size_t i = 0; i < count; ++i)
                {
                    rng.emplace_back(range_t{prev, prev + span});
                    prev += span;
                }

                return rng;
            }
        }

        template<typename NumberType, 
            typename = std::enable_if_t<is_unsigned_integer_v<NumberType>>>
        class simple_positive_rational
        {
            public:
                using number_t = NumberType;

            private:
                number_t m_p{0}; // numerator
                number_t m_q{1}; // denominator
                
                simple_positive_rational(number_t p, number_t q, bool):
                    m_p{p}, m_q{q} { }

            public:
                simple_positive_rational(){}
                simple_positive_rational(number_t p): m_p{p} { }

                template<typename Type>
                operator Type() const { return (Type)m_p / (Type)m_q; }

                simple_positive_rational(number_t p, number_t q):
                        m_p{p}, m_q{q} 
                    { 
                        euclidean::reduce(m_p, m_q);
                    }

                simple_positive_rational(const simple_positive_rational&) = default;

                simple_positive_rational& operator=(const simple_positive_rational&) = default;

                simple_positive_rational& operator*=(number_t n)
                {
                    /*      m_p                   n
                          ------- x n => m_p x -------
                            m_q                  m_q
                    */

                   euclidean::reduce(n, m_q); 
                   
                   // m_p *= n;
                   m_p = safe_type::safe_mul(m_p, n);
                   
                   return *this;
                }

                simple_positive_rational& operator*=(simple_positive_rational r)
                {
                    /*      m_p       r.m_p       m_p       r.m_p
                          ------- x -------- => ------- x --------
                            m_q       r.m_q      r.m_q       m_q
                    */

                   euclidean::reduce(m_p, r.m_q);
                   euclidean::reduce(r.m_p, m_q);

                   // m_p *= r.m_p; 
                   m_p = safe_type::safe_mul(m_p, r.m_p);

                   // m_q *= r.m_q;
                   m_q = safe_type::safe_mul(m_q, r.m_q);

                   return *this;
                }

                template<typename RationalType>
                friend decltype(auto)
                operator*(RationalType&& r, number_t n)
                {
                    static_assert(std::is_same_v<simple_positive_rational,
                        remove_cv_ref_t<RationalType>>, "r should be rational number");

                    if constexpr(std::is_rvalue_reference_v<decltype(r)>)
                    {
                      r *= n; // throws if overflow
                      
                      return std::forward<RationalType>(r);
                    }
                    else
                    {
                      auto rlt = r; rlt *= n; // throws if overflow 
                      return rlt;  
                    }
                }

                template<typename RationalType1, typename RationalType2>
                friend decltype(auto)
                    operator*(RationalType1&& r1, RationalType1&& r2)
                {
                    /*      m_p       r.m_p       m_p       r.m_p
                          ------- x -------- => ------- x --------
                            m_q       r.m_q      r.m_q       m_q
                    */
               
                    if constexpr(std::is_rvalue_reference_v<decltype(r1)>)
                    {
                        r1 *= r2; // throws if overflow

                        return std::forward<RationalType1>(r1);
                    }
                    else if constexpr(std::is_rvalue_reference_v<decltype(r2)>)
                    {
                        r2 *= r1; // throws if overflow 
                        return std::forward<RationalType2>(r2);
                    }
                    else
                    {
                        auto r = r1; r *= r2; // throws if overflow 
                        return r;
                    }
                }

                friend std::ostream& operator<<(std::ostream& os, const simple_positive_rational& r)
                {
                    os << "(" << r.m_p << ", " << r.m_q << ")";
                    return os;
                }

         }; // end of class simple_positive_rational

        template<typename Type1, typename Type2, typename ReturnType = unsigned long long>
        enable_if_all_in_list_t<types::type_list_t<Type1, Type2>, integral_list_t, ReturnType>
        ncr(Type1 nn, Type2 rr)
        {
            size_t r = (size_t)rr; size_t n = (size_t)nn;

            /*
                                              nPr       n x (n-1) x ... x (n-r+1)
                nCr x r ! = nPr;  =>  nCr = ------- = ----------------------------- 
                                               r!       r x (r-1) x ... x 1 
            */

            if  (r == 0 || n == r)
            {
                return 1;
            }
            else if (r == 1 || r == (n - 1))
            {
                return n;
            }
            else
            {
                using rational_t = simple_positive_rational<ReturnType>;

                if(r > (n-r) ) r = n-r;

                rational_t rlt{(ReturnType)n, (ReturnType)r};              
                
                for(ReturnType i = 1; i < (ReturnType)(r); ++i)
                {
                    rlt *= rational_t((ReturnType)n - i, (ReturnType)r - i); 
                }

                return rlt;
            }

        } // end of function ncr

        template<typename Type1, typename Type2, typename ReturnType = unsigned long long>
        enable_if_all_in_list_t<types::type_list_t<Type1, Type2>, integral_list_t, ReturnType>
        npr(Type1 nn, Type2 rr)
        {
            size_t n = (size_t)nn; size_t r=(size_t)rr;
            
            if (r == 0)
                return 1;
            else if (r == 1)
                return n;
            else
            {
                ReturnType rlt = n;

                for (ReturnType i = 1; i < (ReturnType)r; ++i)
                {
                    // rlt *= ((ReturnType)n - i);
                    rlt = safe_type::safe_mul(rlt, ((ReturnType)n - i));
                }

                return rlt;
            }         
        }

        template<typename Type, typename ReturnType = unsigned long long>
        enable_if_in_list_t<Type, integral_list_t, ReturnType>
        fact(Type n)
        {
            return npr(n, n);
        }

        class combination
        {
        public:
            using number_t = big_unsigned_t;
            using m_th_t = number_t; 
            using signed_m_th_t = big_integer_t;

            using rational_t = simple_positive_rational<number_t>;

        protected:
            mutable number_t m_n{}, m_r{}, m_ncr{1};
          
            template<typename Type>
            void offset_nr(Type offset)
            {
                if (offset < 0)
                {
                    while (offset < 0)
                    {
                        // WARING: the following does not work,
                        // because this->m_ncr is NOT a rational number
                        // this->m_ncr *= rational_t{this->m_r--, m_n--};

                        this->m_ncr = rational_t{this->m_r--, m_n--} * this->m_ncr;
                        
                        ++offset;
                    }
                }
                else if (offset > 0)
                {
                    while (offset > 0)
                    {
                        // WARING: the following does not work,
                        // because this->m_ncr is NOT a rational number
                        // this->m_ncr *= rational_t{++m_n, ++m_r};

                        this->m_ncr = rational_t{++m_n, ++m_r} * this->m_ncr;

                        --offset;
                    }
                }
            }

            template<typename Type>
            void offset_n(Type offset)
            {
                if (offset > 0)
                {
                    while (offset > 0)
                    {
                        // WARING: the following does not work,
                        // because this->m_ncr is NOT a rational number
                        // this->m_ncr *= rational_t{m_n + 1, m_n - m_r + 1};

                        this->m_ncr = rational_t{m_n + 1, m_n - m_r + 1} * this->m_ncr;

                        ++m_n; --offset;
                    }
                }
                else if (offset < 0)
                {
                    while (offset < 0)
                    {
                        // WARING: the following does not work,
                        // because this->m_ncr is NOT a rational number
                        // this->m_ncr *= rational_t{m_n - m_r, m_n};

                        this->m_ncr = rational_t{m_n - m_r, m_n} * this->m_ncr;

                        --m_n; ++offset;
                    }
                }
            }

            template<typename Type>
            void offset_r(Type offset)
            {
                if (offset > 0)
                {
                    while (offset > 0)
                    {
                        // WARING: the following does not work,
                        // because this->m_ncr is NOT a rational number
                        // this->m_ncr *= rational_t{m_n - m_r, m_r + 1};

                        this->m_ncr = rational_t{m_n - m_r, m_r + 1} * this->m_ncr;

                        ++m_r; --offset;
                    }
                }
                else if (offset < 0)
                {
                    while (offset < 0)
                    {
                        // WARING: the following does not work,
                        // because this->m_ncr is NOT a rational number
                        // this->m_ncr *= rational_t{m_r, m_n - m_r + 1};

                        this->m_ncr = rational_t{m_r, m_n - m_r + 1} * this->m_ncr;

                        --m_r; ++offset;
                    }
                }
            }

        public:

            combination(): m_n{0}, m_r{0}, m_ncr{1} { }
            combination(const combination&) = default;
            combination& operator=(const combination&) = default;

            template<typename Type>
            combination(Type n): m_n{(number_t)n}, m_r{(number_t)0},
                m_ncr{ 1 } {  }

            template<typename Type1, typename Type2>
            combination(Type1 n, Type2 r): m_n{(number_t)n}, m_r{(number_t)r},
                m_ncr{ ncr(m_n, m_r) } {  }
            
            number_t operator()()const { return this->m_ncr; }

            template<typename Type1, typename Type2>
            
            number_t reset(Type1 n = Type1{}, Type2 r = Type2{})
            {
                this->m_n = {(number_t)n}; this->m_r = {(number_t)r};
                this->m_ncr = ncr((number_t)n, (number_t)r);

                return this->m_ncr;
            }

            template<typename Type>
            number_t set_r(Type r)
            {
                signed_m_th_t offset = (signed_m_th_t)r - (signed_m_th_t)this->m_r;
               
                if(offset == 0)
                    return this->m_ncr;
                else
                {
                    this->offset_r(offset);
                    return this->m_ncr;
                }
            }

            template<typename Type>
            number_t set_n(Type n)
            {
                signed_m_th_t offset = (signed_m_th_t)this->m_n - (signed_m_th_t)n;
                if(offset != 0)
                    return this->m_ncr;
                else
                {
                    this->offset_n(offset);
                    return this->m_ncr;
                }
            }
                       
            number_t decrement_nr()
            {
                // WARING: the following does not work,
                // because this->m_ncr is NOT a rational number
                // this->m_ncr *= rational_t{m_r--, m_n--};

                this->m_ncr = rational_t{m_r--, m_n--} * this->m_ncr;

                return this->m_ncr;
            }
                       
            number_t increment_nr()
            {
                // WARING: the following does not work,
                // because this->m_ncr is NOT a rational number
                // this->m_ncr *= rational_t{++m_n, ++m_r};

                this->m_ncr = rational_t{++m_n, ++m_r} * this->m_ncr;

                return this->m_ncr;
            }
                
            number_t increment_r()
            {
                // WARING: the following does not work,
                // because this->m_ncr is NOT a rational number
                // this->m_ncr *= rational_t{m_n - m_r, m_r + 1};

                this->m_ncr = rational_t{m_n - m_r, m_r + 1} * this->m_ncr;

                // we should do this, because compiler
                // may evaluate on the right handside first
                ++m_r; return this->m_ncr;
            }

            number_t decrement_r()
            {
                // WARING: the following does not work,
                // because this->m_ncr is NOT a rational number
                // this->m_ncr *= rational_t{m_r, m_n - m_r + 1};

                this->m_ncr = rational_t{m_r, m_n - m_r + 1} * this->m_ncr;

                // we should do this, because compiler
                // may evaluate on the right handside first
                --m_r;

                return this->m_ncr;
            }

            number_t increment_n()
            {
                // WARING: the following does not work,
                // because this->m_ncr is NOT a rational number
                // this->m_ncr *= rational_t{m_n + 1, m_n - m_r + 1};
                this->m_ncr = rational_t{m_n + 1, m_n - m_r + 1} * this->m_ncr;

                // we should do this, because compiler
                // may evaluate on the right handside first
                ++m_n;

                return this->m_ncr;
            }

            number_t decrement_n()
            {
                // WARING: the following does not work,
                // because this->m_ncr is NOT a rational number
                // this->m_ncr *= rational_t{m_n - m_r, m_n};
                this->m_ncr = rational_t{m_n - m_r, m_n} * this->m_ncr;
                
                // we should do this, because compiler
                // may evaluate on the right handside first
                --m_n;

                return this->m_ncr;
            }

            template<typename Type1, typename Type2>
            number_t operator() (Type1 n, Type2 r)
            {
                this->m_n = (number_t)n; 
                this->m_r = (number_t)r;
                
                this->m_ncr = ncr(m_n, m_r);
                
                return this->m_ncr;
            }
            
            template<typename Type>
            operator Type() const { return (Type)this->m_ncr; }

            friend std::ostream& 
            operator<<(std::ostream& os, const combination& cb)
            {
                os << "{" << cb.m_n <<" C " 
                    << cb.m_r << " = " << cb.m_ncr << "}";
                return os;
            }

        }; // end of class combination
        
        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::deque<Type> ele should be in order
        // ele = { 0, 1, 2, 3 }
        template<template<typename, typename...> class SetContainer = std::vector, typename... ContainerTails,
            typename MthType = combination::m_th_t, typename EleType = int, typename SelectCountType = size_t,
            typename ReturnType = std::vector<EleType>>
		ReturnType enumerate_combination(combination cmb, MthType m_th, 
            SetContainer<EleType, ContainerTails...> from_set, SelectCountType select_count)
		{
            constexpr auto is_from_set_vector = 
                types::is_same_template_v<SetContainer<EleType, ContainerTails...>,
                std::vector<EleType>>;

            constexpr auto is_return_vector = 
                types::is_same_template_v<ReturnType, std::vector<EleType>>;

            size_t set_size = from_set.size();

            ReturnType selected_set;
            
            if constexpr(is_from_set_vector) 
                reverse_order_in_place(from_set);

            if constexpr(is_return_vector)
                selected_set.reserve((size_t)select_count);
			do
			{
				if (select_count == 0)
                { 
                    break;
                }
				else if ((size_t)select_count == set_size)
				{
                    append_to_container<is_from_set_vector>(selected_set, std::move(from_set));
                    break;
				}
				else
				{
					if (m_th < cmb())
					{
                        if constexpr(is_from_set_vector)
                            emplace_back(selected_set, pop_back(from_set));
                        else
                            emplace_back(selected_set, pop_front(from_set));
                        
                        --select_count; cmb.decrement_nr();
					}
					else
					{
						if constexpr(is_from_set_vector)
                            pop_back(from_set);
                        else
                            pop_front(from_set);
                    
                        m_th -= cmb(); cmb.decrement_n();
                    }

                    --set_size;
				}

			} while (true);

			return selected_set;
		}
        
        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::deque<Type> ele should be in order
        // ele = { 0, 1, 2, 3 }
        template<template<typename, typename...> class ReturnContainer = std::vector,
            template<typename, typename...> class SetContainer = std::list, typename... ContainerTails,
            typename MthType = combination::m_th_t, typename EleType = int, typename SelectCountType = size_t,
            typename SetTagType = tpf::set_tag<ReturnContainer, EleType>, 
            typename ReturnType = tpf::duet_set_t<SetTagType>>
		ReturnType enumerate_combination_and_complement(combination cmb, MthType m_th, 
            SetContainer<EleType, ContainerTails...> from_set, SelectCountType select_count)
		{
            constexpr auto is_from_set_vector = 
                types::is_same_template_v<SetContainer<EleType, ContainerTails...>, std::vector<EleType>>;

            constexpr auto is_return_vector = 
                types::is_same_template_v<ReturnContainer<EleType>, std::vector<EleType>>;

            size_t set_size = from_set.size();

            ReturnType duet_set;
            auto& [selected_set, complement_set] = duet_set;
            
            if constexpr(is_from_set_vector)
               reverse_order_in_place(from_set);

            if constexpr(is_return_vector)
            {
                selected_set.reserve((size_t)select_count);
                complement_set.reserve((size_t)set_size - (size_t)select_count);
            }
	
			do
			{
				if (select_count == 0)
                { 
                    append_to_container<is_from_set_vector>(complement_set, std::move(from_set));

                    break;
                }
				else if ((size_t)select_count == set_size)
				{
                    append_to_container<is_from_set_vector>(selected_set, std::move(from_set));
                    
                    break;
				}
				else
				{
					if (m_th < cmb())
					{
                        if constexpr(is_from_set_vector)
                            emplace_back(selected_set, pop_back(from_set));
                        else
                            emplace_back(selected_set, pop_front(from_set));
                        
                        --select_count; cmb.decrement_nr();
					}
					else
					{
						if constexpr(is_from_set_vector)
                            emplace_back(complement_set, pop_back(from_set));
                        else
                            emplace_back(complement_set, pop_front(from_set));
                    
                        m_th -= cmb(); cmb.decrement_n();
                    }

                    --set_size;
				}

			} while (true);

			return duet_set;
		}

        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::deque<Type> ele should be in order
        // ele = { 0, 1, 2, 3 }
        template<template<typename, typename...> class ReturnContainer = std::vector,
            typename MthType = combination::m_th_t, 
            typename SetSizeType = size_t, typename SelectCountType = size_t,
            typename SetTagType = tpf::set_tag<ReturnContainer, int>, 
            typename ReturnType = tpf::duet_set_t<SetTagType>>
		auto enumerate_combination_and_complement(MthType m_th, SetSizeType set_size, SelectCountType select_count)
        {
            std::vector<int> from_set; 

            size_t size = (size_t)set_size;

            from_set.reserve(size);

            for(size_t i = 0; i < size; ++i)
                from_set.emplace_back((int)i);

            ncrnpr::combination cmb{size-1, select_count-1};

            return enumerate_combination_and_complement(cmb, m_th, from_set, select_count);
        }

        ////////////////////////////////////////////
        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::deque<Type> ele should be in order
        // ele = { 0, 1, 2, 3 }
        // template<template<typename, typename...> class ReturnContainer = std::vector,
        //     typename SetSizeType = size_t, typename SelectCountType = size_t,
        //     typename SetTagType = tpf::set_tag<ReturnContainer, int>, 
        //     typename ReturnType = tpf::set_of_duets_t<SetTagType>>
		// auto enumerate_combinations_and_offsets_indices(SetSizeType set_size, SelectCountType select_count);

        template<template<typename, typename...> class ReturnContainer = std::vector,
            template<typename, typename...> class SetContainer = std::list, typename... ContainerTails,
            typename MthType = combination::m_th_t, typename EleType = int, typename SelectCountType = size_t,
            typename SetTagType = tpf::set_tag<ReturnContainer, EleType>, 
            typename ReturnType = tpf::set_of_duets_t<SetTagType>>
		auto enumerate_combinations_and_offsets(SetContainer<EleType, ContainerTails...> from_set,
            SelectCountType select_count = tpf::InvalidIndex)
        {
            size_t set_size = from_set.size();

            using m_th_t = combination::m_th_t;
            
            ReturnType sets; 

            if(select_count != tpf::InvalidIndex)
            {
                m_th_t max_m_th = ncr((m_th_t)set_size, (m_th_t)select_count);
                sets.reserve(max_m_th);

                ncrnpr::combination cmb{set_size-1, select_count-1};

                for(m_th_t m_th = 0; m_th < max_m_th; ++m_th)
                    sets.emplace_back(enumerate_combination_and_complement<ReturnContainer>
                        (cmb, m_th, from_set, (size_t)select_count));
            
                return sets;
            }
            else
            {
                sets.reserve((size_t)tpf::two_power_n(set_size));

                for(size_t r = 0; r <= set_size; ++r)
                {
                    m_th_t m_th_max = ncr((m_th_t)set_size, (m_th_t)r);
                    
                    ncrnpr::combination cmb{set_size-1, r-1};

                    for(m_th_t m_th = 0; m_th < m_th_max; ++m_th)
                         sets.emplace_back(enumerate_combination_and_complement<ReturnContainer>
                          (cmb, m_th, from_set, (size_t)r));
                }

                return sets;
            }
        }

        ////////////////////////////////////////

        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::vector<int> ele should be in order
        // ele = { 0, 1, 2, 3 }
        template<template<typename, typename...> class ReturnContainer = std::vector,
            typename MthType = combination::m_th_t, typename SelectCountType = size_t,
            typename SetTagType = tpf::set_tag<ReturnContainer, int>, 
            typename ReturnType = tpf::duet_set_t<SetTagType>>
		ReturnType enumerate_combinations_and_offsets_indices(combination cmb, MthType m_th,
            std::vector<int> from_set, SelectCountType select_count)
		{
            constexpr auto is_return_vector = 
                types::is_same_template_v<ReturnContainer<int>, std::vector<int>>;

            ReturnType duet_set;
            auto& [selected_set, complement_set] = duet_set;
            
            size_t set_size = from_set.size();

            if constexpr(is_return_vector)
            {
                selected_set.reserve((size_t)select_count);
                complement_set.reserve((size_t)set_size - (size_t)select_count);
            }

			// combination cmb(set_size - 1, select_count - 1);

			do
			{
				if (select_count <= 0)
                { 
                    append_to_container<true>(complement_set, std::move(from_set));

                    break;
                }
				else if ((size_t)select_count == set_size)
				{
                    append_to_container<true>(selected_set, std::move(from_set));
                    
                    break;
				}
				else
				{
					if (m_th < cmb())
					{
                        emplace_back(selected_set, pop_back(from_set));
                        --select_count; cmb.decrement_nr();
					}
					else
					{
						emplace_back(complement_set, pop_back(from_set));
                        m_th -= cmb(); cmb.decrement_n();
                    }

                    --set_size;
				}

			} while (true);

			return duet_set;
		}

        
        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::deque<Type> ele should be in order
        // ele = { 0, 1, 2, 3 }
        template<template<typename, typename...> class ReturnContainer = std::vector,
            typename SetSizeType = size_t, typename SelectCountType = size_t,
            typename SetTagType = tpf::set_tag<ReturnContainer, int>, 
            typename ReturnType = tpf::set_of_duets_t<SetTagType>>
		auto enumerate_combinations_and_offsets_indices(SetSizeType set_size,
            SelectCountType select_count = tpf::InvalidIndex)
        {
            std::vector<int> from_set; 
            from_set.reserve(set_size);

            for(size_t i = set_size-1; i != 0; --i)
                from_set.emplace_back((int)i);

            from_set.emplace_back(0);

            using m_th_t = combination::m_th_t;
            
            ReturnType sets; 

            if(select_count != tpf::InvalidIndex)
            {
                m_th_t m_th_max = ncr(set_size, select_count);
                sets.reserve(m_th_max);

                ncrnpr::combination cmb{set_size-1, select_count-1};

                for(m_th_t m_th = 0; m_th < m_th_max; ++m_th)
                    sets.emplace_back(enumerate_combinations_and_offsets_indices<ReturnContainer>
                        (cmb, m_th, from_set, select_count));
                
                return sets;
            }
            else
            {
                sets.reserve((size_t)tpf::two_power_n(set_size));

                for(size_t r = 0; r <= set_size; ++r)
                {
                    m_th_t m_th_max = ncr((m_th_t)set_size, (m_th_t)r);

                    ncrnpr::combination cmb{set_size-1, r-1};

                    for(m_th_t m_th = 0; m_th < m_th_max; ++m_th)
                         sets.emplace_back(enumerate_combinations_and_offsets_indices<ReturnContainer>
                          (cmb, m_th, from_set, r));
                }

                return sets;
            }
        }

        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::deque<Type> ele should be in order
        // ele = { 0, 1, 2, 3 }
        template<template<typename, typename...> class ReturnContainer = std::vector,
            typename SetSizeType = size_t, typename SelectCountType = size_t,
            typename SetTagType = tpf::set_tag<ReturnContainer, int>, 
            typename ReturnType = tpf::set_of_duets_t<SetTagType>>
		auto enumerate_combination_including_indices(std::vector<int> include_set,
            SetSizeType set_size, SelectCountType select_count = tpf::InvalidIndex)
        {
            std::sort(include_set.begin(),
                include_set.end(), std::greater<int>{});

            std::vector<int> from_set;
            
            for(size_t i = set_size-1; i != 0; --i)
            {
                if(!std::binary_search(include_set.crbegin(), include_set.crend(), i))
                    from_set.emplace_back(i);
            }

            if(!std::binary_search(include_set.crbegin(), include_set.crend(), 0))
                    from_set.emplace_back(0);

            using m_th_t = combination::m_th_t;
            
            ReturnType sets; 

            if(select_count != tpf::InvalidIndex)
            {
                size_t choice_count = select_count - include_set.size();
                m_th_t m_th_max = ncr(from_set.size(), choice_count);
                sets.reserve(m_th_max);

                ncrnpr::combination cmb{from_set.size()-1, choice_count-1};

                for(m_th_t m_th = 0; m_th < m_th_max; ++m_th)
                    sets.emplace_back(enumerate_combinations_and_offsets_indices<ReturnContainer>
                        (cmb, (combination::m_th_t)m_th, from_set, (size_t)choice_count));
                
                for(auto& ss: sets)
                {
                    auto&[v, c] = ss;
                    
                    types::append_to_container<false>(v, include_set);
                    std::sort(v.begin(), v.end());
                    v.erase(std::unique(v.begin(), v.end()), v.end());
                }

                return sets;
            }
            else
            {
                set_size = from_set.size();
                sets.reserve((size_t)tpf::two_power_n(set_size));
                
                for(size_t r = 0; r <= set_size; ++r)
                {
                    m_th_t m_th_max = ncr((m_th_t)set_size, (m_th_t)r);

                    ncrnpr::combination cmb{set_size-1, r-1};

                    for(m_th_t m_th = 0; m_th < m_th_max; ++m_th)
                         sets.emplace_back(enumerate_combinations_and_offsets_indices<ReturnContainer>
                          (cmb, (combination::m_th_t)m_th, from_set, (size_t)r));
                }

                for(auto& ss: sets)
                {
                    auto&[v, c] = ss;
                    
                    types::append_to_container<false>(v, include_set);
                    std::sort(v.begin(), v.end());
                    v.erase(std::unique(v.begin(), v.end()), v.end());
                }

                return sets;
            }
        }

        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::deque<Type> ele should be in order
        // ele = { 0, 1, 2, 3 }
        template<template<typename, typename...> class ReturnContainer = std::vector,
            typename SetSizeType = size_t, typename SelectCountType = size_t,
            typename SetTagType = tpf::set_tag<ReturnContainer, int>, 
            typename ReturnType = tpf::set_of_duets_t<SetTagType>>
		auto enumerate_combination_excluding_indices(std::vector<int> exclude_set,
            SetSizeType set_size, SelectCountType select_count = tpf::InvalidIndex)
        {
            std::sort(exclude_set.begin(),
                exclude_set.end(), std::greater<int>{});

            std::vector<int> from_set;
            
            for(size_t i = set_size-1; i != 0; --i)
            {
                if(!std::binary_search(exclude_set.crbegin(), exclude_set.crend(), i))
                    from_set.emplace_back(i);
            }

            if(!std::binary_search(exclude_set.crbegin(), exclude_set.crend(), 0))
                    from_set.emplace_back(0);

            using m_th_t = combination::m_th_t;
            
            ReturnType sets; 

            if(select_count != tpf::InvalidIndex)
            {
                m_th_t m_th_max = ncr(from_set.size(), select_count);
                sets.reserve(m_th_max);

                ncrnpr::combination cmb{from_set.size()-1, select_count-1};

                for(m_th_t m_th = 0; m_th < m_th_max; ++m_th)
                    sets.emplace_back(enumerate_combinations_and_offsets_indices<ReturnContainer>
                        (cmb, (combination::m_th_t)m_th, from_set, (size_t)select_count));
                
                for(auto& ss: sets)
                {
                    auto&[v, c] = ss;
                    
                    types::append_to_container<false>(c, exclude_set);
                    std::sort(c.begin(), c.end());
                    v.erase(std::unique(c.begin(), c.end()), c.end());
                }

                return sets;
            }
            else
            {
                set_size = from_set.size();
                sets.reserve((size_t)tpf::two_power_n(set_size));
                
                for(size_t r = 0; r <= set_size; ++r)
                {
                    m_th_t m_th_max = ncr((m_th_t)set_size, (m_th_t)r);

                    ncrnpr::combination cmb{ set_size-1, r-1 };

                    for(m_th_t m_th = 0; m_th < m_th_max; ++m_th)
                         sets.emplace_back(enumerate_combinations_and_offsets_indices<ReturnContainer>
                          (cmb, (combination::m_th_t)m_th, from_set, (size_t)r));
                }

                for(auto& ss: sets)
                {
                    auto&[v, c] = ss;
                    
                    types::append_to_container<false>(c, exclude_set);
                    std::sort(c.begin(), c.end());
                    v.erase(std::unique(c.begin(), c.end()), c.end());
                }

                return sets;
            }
        }

        class permutation
        {
            public:
            using number_t = big_unsigned_t;

        protected:
            mutable number_t m_n{}, m_r{};
            mutable number_t m_npr{1};
                    
            void reset()
            {
                this->m_n = {}; this->m_r = {};
                this->m_npr = {1};
            }

        public:
            permutation(): m_n{}, m_r{}, m_npr{1} { }

            template<typename Type1, typename Type2>
            permutation(Type1 n, Type2 r): m_n{(number_t)n}, m_r{(number_t)r},
                m_npr{npr(m_n, m_r)} { }

            permutation(const permutation&) = default;
            permutation& operator=(const permutation&) = default;

            number_t operator()() const { return this->m_npr;}
            
            template<typename Type1, typename Type2>
            number_t operator()(Type1 n, Type2 r)
            {
                this->m_n = (number_t)n; this->m_r = (number_t)r;
                this->m_npr = npr(this->m_n, this->m_r);

                return this->m_npr;
            }

            template<typename Type>
            operator Type() const { return (Type)this->m_npr; }

            friend std::ostream& 
            operator<<(std::ostream& os, const permutation& p)
            {
                os << "{" << p.m_n <<" P " 
                    << p.m_r << " = " << p.m_npr << "}";
                return os;
            }
        };

        // this is the fastest and most efficient
        template<typename RType, typename MType>
        std::vector<int> enumerate_permutation(std::vector<int> e, RType r, MType m_th)
        {
            using number_t = big_unsigned_t;
            std::vector<int> v;

            number_t n = (number_t)e.size();

            if (!(r < 1 || n < 1 || (size_t)r > n))
            {
                v.reserve((size_t)r);
 
                number_t permu = npr((number_t)n - 1, (number_t)r - 1);

                while (r) // r != 0
                {
                    // compute index
                    size_t s = ((size_t)m_th / (size_t)permu);
                    m_th %= (MType)permu;

                    // move s-th element of e from e to v
                    v.emplace_back(e[s]);

                    // remove s-th element from e
                    // erase(e, s); --r; --n;
                    e.erase(e.begin() + s); --r; --n;

                    if (n > 1) permu /= (number_t)n;
                }
            }

            return v;
        }

        // this is the fastest and most efficient
        template<typename T>
        std::vector<T> enum_permutation(T n, T r, T m_th)
        {
            std::vector<T> v;

            if (!(r < 1 || n < 1 || r > n))
            {
                v.reserve((size_t)r);

                std::vector<T> e((size_t)n);

                // initialize set e
                // e = { 0, 1, 2, ..., n-1}
                for (size_t i = 0; i < size_t(n); ++i)
                    e[i] = T(i);

                T permu = npr((T)n - 1, (T)r - 1);

                while (r) // r != 0
                {
                    // compute index
                    size_t s = (size_t)(m_th / permu);
                    m_th %= permu;

                    // move s-th element of e from e to v
                    v.emplace_back(e[s]);

                    // remove s-th element from e
                    tpf::types::erase(e, s); --r; --n;

                    if (n > 1) permu /= (T)n;
                }
            }

            return v;
        }

        // this is the fastest and most efficient
        template<typename T>
        std::vector<T> enum_permutation_remainder(T n, T r, T m_th)
        {
            std::vector<T> v;

            if (!(r < 1 || n < 1 || r > n))
            {
                v.reserve((size_t)r);

                std::vector<T> e((size_t)n);

                // initialize set e
                // e = { 0, 1, 2, ..., n-1}
                for (size_t i = 0; i < size_t(n); ++i)
                    e[i] = T(i);

                T permu = npr((T)n - 1, (T)r - 1);

                while (r) // r != 0
                {
                    // compute index
                    size_t s = (size_t)(m_th / permu);
                    m_th %= permu;

                    // move s-th element of e from e to v
                    v.emplace_back(e[s]);

                    // remove s-th element from e
                    erase(e, s); --r; --n;

                    if (n > 1) permu /= (T)n;
                }

                if(!e.empty())
                {
                    for(auto ele: e) v.emplace_back(ele);
                }
            }

            return v;
        }

        // this is the fastest and most efficient
        template<typename PType, typename NRType, typename MType>
        std::vector<NRType> enum_permutation_static(PType permu, NRType n, NRType r, MType m_th)
        {
            int e[] = {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
                        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                        20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                        30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                        40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                        50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                        60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
                        70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
                        80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
                        90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100 };
            
            constexpr size_t ele_count = sizeof(e)/sizeof(size_t) - 1;

            int v[ele_count+1]; auto rr = r;

            if (!(r < 1 || n < 1 || r > n))
            {
                 while (r) // r != 0
                {
                    // compute index
                    size_t s = (size_t)(m_th / (MType)permu);
                    m_th %= (MType)permu;

                    // move s-th element of e from e to v
                    v[rr-r] = e[s];
                    
                    // remove s-th element from e
                    for(size_t i = s ; i < ele_count; ++i)
                        e[i] = e[i+1];

                    --r; --n;

                    if (n > 1) permu /= (PType)n;
                }
            }

            return { &v[0],  &v[rr] };
        }

        // this is the fastest and most efficient
        template<typename T>
        std::vector<T> enum_permutation_list(T n, T r, T mth)
        {
            std::vector<T> v;

            if (!(r < 1 || n < 1 || r > n))
            {
                v.reserve((size_t)r);

                std::list<T> e;

                // initialize set e
                // e = { 0, 1, 2, ..., n-1}
                for (size_t i = 0; i < size_t(n); ++i)
                    e.push_back(i);

                T permu = npr((T)n - 1, (T)r - 1);

                while (r) // r != 0
                {
                    // compute index
                    size_t s = size_t(mth / permu);
                    mth %= permu;

                    // move s-th element of e from e to v
                    auto itr = e.begin(); 
                    std::advance(itr, s);

                    v.emplace_back(*itr);

                    // remove s-th element from e
                    e.erase(itr); --r; --n;

                    if (n > 1) permu /= (T)n;
                }
            }

            return v;
        }

        // returns m_th subset by selecting r elements from
        // the set from_set with n elements (from_set.size()) 
        // std::deque<Type> ele should be in order
        // ele = { 0, 1, 2, 3 }
        template<template<typename, typename...> class ReturnContainer = std::vector,
            template<typename, typename...> class SetContainer = std::list, typename... ContainerTails,
            typename MthType = unsigned long long, typename EleType = int, typename CountType = int>
		ReturnContainer<EleType> enumerate_permutation(MthType m_th, 
            SetContainer<EleType, ContainerTails...> from_set, CountType r)
	    {
            using number_t = big_unsigned_t;
            
            size_t select_count = (size_t)r;
            size_t n = from_set.size();
            
            std::vector<int> v;

            if (!(r < 1 || n < 1 || r > n))
            {
                v.reserve((size_t)r);

                std::list<int> e;

                // initialize set e, e = { 0, 1, 2, ..., n-1}
                for (size_t i = 0; i < n; ++i)
                    e.push_back(i);

                number_t permu = npr((number_t)n - 1, (number_t)r - 1);

                while (r) // r != 0
                {
                    // compute index
                    size_t s = (size_t)(m_th / permu);
                    m_th %= permu;

                    // move s-th element of e from e to v
                    auto itr = e.begin(); std::advance(itr, s);
                    v.emplace_back(*itr);
                    // remove s-th element from e
                    e.erase(itr);
                    --r; --n;

                    if (n > 1) permu /= (number_t)n;
                }
            }

            ReturnContainer<EleType> rtn;

            if(!v.empty())
            {
                constexpr auto is_return_vector = 
                    types::is_same_template_v<ReturnContainer<EleType>, std::vector<EleType>>;

                if constexpr(is_return_vector) 
                    rtn.reserve(select_count);

                for(auto& idx: v)
                    rtn.emplace_back(from_set[(size_t)idx]);
            }

            return rtn;
        }

    } // end of namespace ncrnpr

    /**
     * @brief Calls callback(args...) function in sequence.
     * Iteration index is not passed to the callback() function.
     * 
     * @tparam CallbackType 
     * @tparam ArgTypes 
     * @param range for the iteration range [start, end) 
     * @param callback for Callback routine
     * @param args for callback(args...)
     */
    template<typename CallbackType, typename... ArgTypes>
    void visit(const ncrnpr::range_t& range, 
        CallbackType&& callback, ArgTypes&&... args)
        {
            for(size_t i = range.first;
                i < range.second; ++i)
                callback(std::forward<ArgTypes>(args)...);
        }

    /**
     * @brief Calls callback(args...) function in parallel.
     * Iteration index is not passed to the callback() function.
     * 
     * @tparam CallbackType 
     * @tparam ArgTypes 
     * @param thread_count for the number of concurrent threads to execute callback()
     * @param range for the iteration range [start, end)
     * @param callback for Callback routine to call concurrently
     * @param args for callback(args...)
     */
    template<typename CallbackType, typename... ArgTypes>
    void visit_parallel(unsigned int thread_count,
         const ncrnpr::range_t& range, 
        CallbackType&& callback, ArgTypes&&... args)
        {
            using number_t = big_unsigned_t;
            const auto& [start, end] = range.base();

            auto ranges = 
                ncrnpr::split_range_span(thread_count, start, end);

            using future_t =  std::future<void>;
            using futures_t = std::vector<future_t>;

            futures_t futures; 
            futures.reserve(ranges.size());
           
            auto parallel_func = 
                visit<tpf::remove_cv_ref_t<CallbackType>,
                    tpf::remove_cv_ref_t<ArgTypes>...>;
                
            for(size_t i = 0; i < ranges.size(); ++i)
            {
                futures.emplace_back(
                    std::async(std::launch::async, 
                        parallel_func, ranges[i],
                        std::forward<CallbackType>(callback),
                        std::forward<ArgTypes>(args)...));  
            }
            
            for(size_t i = 0; i < ranges.size(); ++i)
            {
                futures[i].get();  
            }         
        }

    /**
     * @brief Calls callback(index, args...) in sequence.
     * Iteration index is passed to the callback() function
     * 
     * @tparam CallbackType 
     * @tparam ArgTypes 
     * @param range for the iteration range [start, end)
     * @param callback for Callback routine.
     * @param args for callback(index, args...)
     */
    template<typename CallbackType, typename... ArgTypes>
    void visit_index(const ncrnpr::range_t& range, 
        CallbackType&& callback, ArgTypes&&... args)
        {
            for(size_t i = range.first;
                i < range.second; ++i)
                callback(i, std::forward<ArgTypes>(args)...);
        }

    /**
     * @brief calls callback(index, args...) in parallel.
     * Iteration index is passed to the callback() as the first argument
     * 
     * @tparam CallbackType 
     * @tparam ArgTypes 
     * @param thread_count for the number of concurrent threads to execute callback() function
     * @param range for the iteration range [start, end)
     * @param callback for Callback routine
     * @param args for callback(index, args...)
     */
    template<typename CallbackType, typename... ArgTypes>
    void visit_index_parallel(unsigned int thread_count,
         const ncrnpr::range_t& range, 
        CallbackType&& callback, ArgTypes&&... args)
        {
            using number_t = big_unsigned_t;
            const auto& [start, end] = range.base();

            auto ranges = 
                ncrnpr::split_range_span(thread_count, start, end);

            using future_t =  std::future<void>;
            using futures_t = std::vector<future_t>;

            futures_t futures; 
            futures.reserve(ranges.size());
            
            auto parallel_func = 
                visit_index<tpf::remove_cv_ref_t<CallbackType>,
                    tpf::remove_cv_ref_t<ArgTypes>...>;
                
            for(size_t i = 0; i < ranges.size(); ++i)
            {
                futures.emplace_back(
                    std::async(std::launch::async, 
                        parallel_func, ranges[i],
                        std::forward<CallbackType>(callback),
                        std::forward<ArgTypes>(args)...));  
            }
            
            for(auto& f: futures) f.get();
        }

     /**
     * @brief Calls callback(enumerated_permutation, args...) in sequence.
     * 
     * @tparam NRType 
     * @tparam CallbackType 
     * @tparam ArgTypes 
     * @param n for n in nPr
     * @param r for r in nPr
     * @param range for the iteration range [start, end)
     * @param callback for Callback routine
     * @param args for callback(enumerated_permutation, args...)
     */
    template<typename NRType, typename CallbackType, typename... ArgTypes>
    void visit_permutations(NRType n, NRType r,
        const ncrnpr::range_t& range, CallbackType&& callback, ArgTypes&&... args)
        {           
            const auto& [start, end] = range.base();

            using number_t  = big_unsigned_t;

            number_t permu = ncrnpr::npr(n - 1, r - 1);
            
            for(number_t m_th = start; m_th < end; ++m_th)
                {
                    callback(ncrnpr::enum_permutation_static(permu, n, r, m_th),
                        std::forward<ArgTypes>(args)...);
                }
        }

     /**
     * @brief Calls callback(enumerated_permutation, args...) in parallel.
     * 
     * @tparam TCType 
     * @tparam NRType 
     * @tparam CallbackType 
     * @tparam ArgTypes 
     * @param thread_count for number of the concurrent threads to execute callback()
     * @param n for n in nPr
     * @param r for r in nPr
     * @param callback for Callback routine
     * @param args for callback(enumerated_permutation, args...)
     */
    template<typename TCType, typename NRType, typename CallbackType, typename... ArgTypes>
    void visit_permutations_parallel(TCType thread_count, 
        NRType n, NRType r,
         CallbackType&& callback, ArgTypes&&... args)
        {
            using number_t = big_unsigned_t;
            auto max_m_th = ncrnpr::npr(n, r);

            auto ranges = 
                ncrnpr::split_range_span((size_t)thread_count, 0, max_m_th);

            using future_t =  std::future<void>;
            using futures_t = std::vector<future_t>;

            futures_t futures; 
            futures.reserve(ranges.size());
            
            auto parallel_func = visit_permutations<NRType, tpf::remove_cv_ref_t<CallbackType>,
                        tpf::remove_cv_ref_t<ArgTypes>...>;

            for(size_t i = 0; i <ranges.size(); ++i)
            {
                futures.emplace_back(
                    std::async(std::launch::async, 
                        parallel_func, 
                        n, r, ranges[i],
                        std::forward<CallbackType>(callback),
                        std::forward<ArgTypes>(args)...));  
            }
            
            for(auto& f: futures) f.get();  
        }

    template<typename CallbackType, typename... ArgTypes>
    void parallel_visit_permutations(
        const std::pair<size_t, size_t> permutation_specification,
         CallbackType&& callback, ArgTypes&&... args)
        {
            const auto& [n, r] = permutation_specification;

            using number_t = big_unsigned_t;
            auto max_m_th = ncrnpr::npr(n, r);
            number_t permu = ncrnpr::npr(n - 1, r - 1);

            // for clang++ compiler
            auto nn = n; auto rr = r;

            auto go_parallel = [permu, nn, rr, &callback, &args...](auto m_th)
            {           
                callback(ncrnpr::enum_permutation_static(permu, nn, rr, m_th), args...);
            };           

            #if defined(_GLIBCXX_PARALLEL)

                #pragma omp parallel for
                for(number_t m_th = 0; m_th < max_m_th; ++m_th)
                        go_parallel(m_th);

            #elif defined(_MSC_VER)    
            
                concurrency::parallel_for((number_t)0, max_m_th, go_parallel);
            
            #elif defined(__ICL)
            
                tbb::parallel_for((number_t)0, max_m_th, go_parallel);
            #else
            
            #endif
            
            
        }

    /**
     * @brief Calls callback(enumerated_combination, args...) in sequence
     * enumerated_combination is generated and pass to the callback() as the first argument
     * 
     * @tparam CallbackType 
     * @tparam ArgTypes 
     * @param combination_specification for nCr or (n, r), the binomial coefficient
     * @param range for [start, end) where start <= end, 0 <= start, or end <= nCr
     * @param callback for Callback routine.
     * @param args for callback(enumerated_combination, args...)
     */
    template<typename CallbackType, typename... ArgTypes>
    void visit_combinations_and_offsets(const std::pair<size_t, size_t>& combination_specification,
        const ncrnpr::range_t& range, CallbackType&& callback, ArgTypes&&... args)
        {           
            const auto& [n, r] = combination_specification;
            const auto& [start, end] = range.base();

            std::vector<int> from_set; 
            from_set.reserve((size_t)n);

            for(size_t i = 0; i < (size_t)n; ++i)
                from_set.emplace_back(i);

            using number_t = big_unsigned_t;
            
            ncrnpr::combination cmb{n-1, r-1};

            for(number_t m_th = start; m_th < end; ++m_th)
                {
                    callback(ncrnpr::enumerate_combination_and_complement(cmb, m_th, from_set, r),
                        std::forward<ArgTypes>(args)...);
                }
        }

    template<typename NRType, typename CallbackType, typename... ArgTypes>
    void visit_combinations(NRType n, NRType r, 
        const ncrnpr::range_t& range, CallbackType&& callback, ArgTypes&&... args)
        {           
            const auto& [start, end] = range.base();

            using number_t = big_unsigned_t;

            ncrnpr::combination cmb(n - 1, r - 1);

            std::vector<int> from_set; 
            from_set.reserve((size_t)n);

            for(size_t i = 0; i < (size_t)n; ++i)
                from_set.emplace_back((int)i);
            
            for(number_t m_th = start; m_th < end; ++m_th)
                {
                    callback(ncrnpr::enumerate_combination_and_complement(cmb, m_th, from_set, r),
                        std::forward<ArgTypes>(args)...);
                }
        }

    template<typename NRType, typename CallbackType, typename... ArgTypes>
    void parallel_visit_combinations_and_complements(NRType n, NRType r, 
         CallbackType&& callback, ArgTypes&&... args)
        {
            using number_t = big_unsigned_t;
            auto max_m_th = ncrnpr::ncr(n, r);

            ncrnpr::combination cmb(n - 1, r - 1);

            std::vector<int> from_set; 
            from_set.reserve((size_t)n);

            for(size_t i = 0; i < (size_t)n; ++i)
                from_set.emplace_back((int)i);

            auto go_parallel = [&cmb, &from_set, n, r, &callback, &args...](auto m_th)
            {           
                callback(ncrnpr::enumerate_combination_and_complement(cmb, m_th, from_set, r), args...);
            };   

            #if defined(_GLIBCXX_PARALLEL)

                #pragma omp parallel for
                for(number_t m_th = 0; m_th < max_m_th; ++m_th)
                        go_parallel(m_th);

            #elif defined(_MSC_VER)    
                concurrency::parallel_for((number_t)0, max_m_th, go_parallel);
             
            #elif defined(__ICL)
                tbb::parallel_for((number_t)0, max_m_th, go_parallel);
            #else
            
            #endif
        }

        template<typename NRType, typename CallbackType, typename... ArgTypes>
        void parallel_visit_combinations(NRType n, NRType r, 
         CallbackType&& callback, ArgTypes&&... args)
        {
            using number_t = big_unsigned_t;
            auto max_m_th = ncrnpr::ncr(n, r);

            ncrnpr::combination cmb(n - 1, r - 1);

            std::vector<int> from_set; 
            from_set.reserve((size_t)n);

            for(size_t i = 0; i < (size_t)n; ++i)
                from_set.emplace_back((int)i);
            
            //from_set.emplace_back((int)0);

            auto go_parallel = [&cmb, &from_set, n, r, &callback, &args...](auto m_th)
            {           
                callback(ncrnpr::enumerate_combination(cmb, m_th, from_set, r), args...);
            };   

            #if defined(_GLIBCXX_PARALLEL)

                #pragma omp parallel for
                for(number_t m_th = 0; m_th < max_m_th; ++m_th)
                        go_parallel(m_th);

            #elif defined(_MSC_VER)    
                concurrency::parallel_for((number_t)0, max_m_th, go_parallel);
             
            #elif defined(__ICL)
                tbb::parallel_for((number_t)0, max_m_th, go_parallel);
            #else
            
            #endif
        }

     /**
     * @brief Calls callback(enumerated_combination, args...) in parallel 
     * 
     * @tparam ThreadCountType 
     * @tparam CallbackType 
     * @tparam ArgTypes 
     * @param thread_count for number of the concurrent threads to execute callback()
     * @param combination_specification for nCr, or (n, r)
     * @param callback for Callback routine
     * @param args for callback(enumerated_combination, args...)
     */
    template<typename ThreadCountType, typename CallbackType, typename... ArgTypes>
    void visit_combinations_and_offsets_parallel(ThreadCountType thread_count,
        const std::pair<size_t, size_t>& combination_specification,
        CallbackType&& callback, ArgTypes&&... args)
        {
            const auto& [n, r] = combination_specification;

            using number_t = big_unsigned_t;
            auto max_m_th = ncrnpr::ncr(n, r);

            auto ranges = 
                ncrnpr::split_range_span(thread_count, 0, max_m_th);

            using future_t =  std::future<void>;
            using futures_t = std::vector<future_t>;

            futures_t futures; 
            futures.reserve(ranges.size());
            
            auto parallel_func = 
                visit_combinations_and_offsets<tpf::remove_cv_ref_t<CallbackType>,
                    tpf::remove_cv_ref_t<ArgTypes>...>;

            for(size_t i = 0; i < ranges.size(); ++i)
            {
                futures.emplace_back(
                    std::async(std::launch::async, 
                        parallel_func, 
                        combination_specification, ranges[i],
                        std::forward<CallbackType>(callback),
                        std::forward<ArgTypes>(args)...));  
            }
            
            for(auto& f: futures) f.get();
        }

} // end of namespace tpf

#ifdef _TPF_NCRNPR_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_NCRNPR_MIN_DEFINED
#endif 

#ifdef _TPF_NCRNPR_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_NCRNPR_MAX_DEFINED
#endif 

#endif // end of file _TPF_NCRNPR_HPP
