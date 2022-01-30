/*
	Programmed by: Thomas Kim
	First Version: Nov. 12, 2017
	Last Version:  Jan. 28, 2020
*/

#ifndef _TPF_SPLIT_RANGES_HPP
#define _TPF_SPLIT_RANGES_HPP

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
#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <thread>
#include <functional>
#include <future>
#include <utility>

namespace tpf
{
	namespace split_range
	{
		template<typename T>
		using range = std::pair<T, T>;

		typedef range<size_t> range_t;

		template<typename T>
		using rngvctr = std::deque<range<T>>;

		typedef rngvctr<size_t> range_vctr_t;

		template<typename T> std::string
		display_range(const range<T>& rg, bool thousands=true)
		{
			std::ostringstream os;

			if (thousands)
				os.imbue(std::locale(""));
			os << "[" << rg.first << ", " << rg.second << ") - count: " << (rg.second - rg.first);

			return os.str();
		}

		template<typename T> rngvctr<T> 
		split_range_count(T st, T ed, T count=(T)(-1));

		template<typename T> rngvctr<T> 
		split_range_span(T st, T ed, T min_span=(T)(-1));

		template<typename T> std::string 
		display_ranges(const rngvctr<T>& rngs, int width=9, bool thousands=true);

		inline size_t cpu_thread_count() noexcept;

		/*
			st                             ed
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11

			minimum span = 3; [1, 5), [5, 8), [8, 11)

			1, 2,  3,  4, 5		-> [1, 5) : 4
			5, 6,  7,  8			-> [5, 8) : 3
			8, 9, 10,  11			-> [8, 11): 3
		*/
		template<typename T> rngvctr<T> 
		split_range_span(T st, T ed, T min_span)
		{
			T dist = ed - st;

			if(min_span == (T)(-1))
			{
				min_span = dist / (T) cpu_thread_count();
				if(min_span == 0) min_span = dist;
			}
				
			T rnd = dist % min_span;
			T count = dist / min_span;

			rngvctr<T> rng;
			if (rnd == 0)
			{
				T prev = st;
				for (T i{}; i < count; ++i)
				{
					//rng.emplace_back(range<T>(prev, prev + min_span));
					rng.emplace_back(prev, prev + min_span);
					prev += min_span;
				}

				return rng;
			}
			else
			{
				if (count >= rnd)
				{
					T span1 = min_span + 1;

					T prev = st;
					for (T i{}; i < rnd; ++i)
					{
						// rng.emplace_back(range<T>(prev, prev + span1));
						rng.emplace_back(prev, prev + span1);
						prev += span1;
					}

					count = count - rnd;
					for (T i{}; i < count; ++i)
					{
						// rng.emplace_back(range<T>(prev, prev + min_span));
						rng.emplace_back(prev, prev + min_span);
						prev += min_span;
					}

					return rng;
				}
				else
				{
					return split_range_count(st, ed, count);
				}
			}
		}

		template<typename T>
		rngvctr<T> split_range_count(T st, T ed, T count /* = cpu_thread_count */)
		{
			if(count == (T)(-1))
				count = (T)cpu_thread_count();

			T dist = ed - st;
			T span = dist / count;
			T rnd = dist % count;

			rngvctr<T> rng;

			if (rnd == 0)
			{
				T prev = st;
				for (T i{}; i < count; ++i)
				{
					//rng.push_back(range<T>(prev, prev + span));
					rng.emplace_back(prev, prev + span);
					prev += span;
				}

				return rng;
			}
			else
			{
				T span1 = span + 1;

				T prev = st;
				for (T i{}; i < rnd; ++i)
				{
					// rng.emplace_back(range<T>(prev, prev + span1));
					rng.emplace_back(prev, prev + span1);
					prev += span1;
				}
				// rnd = dist % count;
				// rnd cannot be greater than count
				count = count - rnd;
				for (T i{}; i < count; ++i)
				{
					// rng.emplace_back(range<T>(prev, prev + span));
					rng.emplace_back(prev, prev + span);
					prev += span;
				}

				return rng;
			}
		}

		template<typename T>
		rngvctr<T> split_range_half(T st, T ed)
		{
			return split_range_count(st, ed, T{2});
		}

		template<typename T> std::string
		display_ranges(const rngvctr<T>& rngs, int width, bool thousands)
		{
			if (!rngs.empty())
			{
				std::ostringstream os;

				if (thousands)
					os.imbue(std::locale(""));

				os << "Range count: " << rngs.size() << ", "<<
					"Max range: " << (rngs[0].second - rngs[0].first) <<
					" elements, Min range: " << (rngs.back().second - rngs.back().first) 
					<< " elements."<<std::endl<<std::endl;

				size_t count = (size_t)rngs.size();
				
				for(size_t i = 0; i<count; ++i)
				{
					os<<std::setw(3)<<i<<": [" << std::setw(width)
						<< rngs[(size_t)i].first << ", " << std::setw(width)
						<< rngs[(size_t)i].second << ") "<< std::setw(width)
						<<(rngs[(size_t)i].second - rngs[(size_t)i].first)
						<< " elements" << std::endl;
				}

				return os.str();
			}
			else
				return std::string("Range empty.");
		}

		inline size_t cpu_thread_count() noexcept
		{
			return (size_t) std::thread::hardware_concurrency(); 
		}

		template<typename RangeType, typename BinOperation, typename SumupOperation,
			typename = std::enable_if_t<tpf::types::is_integer_v<RangeType>>>
		auto parallel_reduce(RangeType start, RangeType end,
			BinOperation&& bin_opr, SumupOperation&& sumup, 
				size_t use_thread_count = tpf::InvalidIndex)
		{
			using return_t = decltype(bin_opr(start, end));

			if(use_thread_count == tpf::InvalidIndex)
				use_thread_count = cpu_thread_count();

			auto ranges = 
				split_range_count((size_t)start, (size_t)end, use_thread_count);

			using future_t = std::future<return_t>;
			using futures_t = std::vector<future_t>;

			futures_t futures;

			if constexpr (!std::is_void_v<return_t>)
			{
				if(ranges.size()==1)
					return bin_opr(start, end);
				else
				{
					size_t count = ranges.size();

					for(size_t i{1}; i < count; ++i)
						futures.emplace_back(std::async(std::launch::async, 
						bin_opr, ranges[i].first, ranges[i].second));

					auto sum = 
						bin_opr(ranges.front().first, ranges.front().second);

					for(auto& f: futures)
							sum = sumup(sum, f.get());

					return sum;
				}
			}
			else
			{
				if(ranges.size()==1)
					bin_opr(start, end);
				else
				{
					size_t count = ranges.size();

					for(size_t i{1}; i < count; ++i)
						futures.emplace_back(std::async(std::launch::async, 
						bin_opr, ranges[i].first, ranges[i].second));

					bin_opr(ranges.front().first, ranges.front().second);

					for(auto& f: futures)
						sumup(sum, f.get());
				}
				
			}
		}

		template<typename ContainerType, typename BinOperation, typename SumupOperation,
			typename = tpf::types::enable_if_container_type_t<ContainerType>>
		auto parallel_reduce(ContainerType&& container,
			BinOperation&& bin_opr, SumupOperation&& sumup, 
				size_t use_thread_count = tpf::InvalidIndex)
			{
				using return_t = decltype(bin_opr(size_t{}, size_t{}));				
				size_t end = container.size();

				if constexpr(!std::is_void_v<return_t>)
				{
					return parallel_reduce(size_t{}, end,
						std::forward<BinOperation>(bin_opr), std::forward<SumupOperation>(sumup), 
						use_thread_count);
				}
				else
				{
					parallel_reduce(size_t{}, end,
						std::forward<BinOperation>(bin_opr), std::forward<SumupOperation>(sumup), 
						use_thread_count);
				}		
			}
	} // end of namespace range

} // end of namespace tpf

#endif // end of _TPF_SPLIT_RANGES_HPP