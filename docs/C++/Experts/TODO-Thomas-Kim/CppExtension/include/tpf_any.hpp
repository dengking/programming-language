/**
 * @file tpf_any.hpp
 * @author Thomas Kim (ThomasKim@TalkPlayFun.com)
 * @brief Stream output operators << are implemented.
 * @version 0.1
 * @date 2020-07-24 ~ 28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _TPF_ANY_HPP
#define _TPF_ANY_HPP

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
#define _TPF_ANY_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_ANY_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

namespace tpf::types
{
	// forward declaration
	template<typename... ElementTypes> class any;

	namespace hidden
	{
		template<typename T>
		struct st_is_constrained_any
		{
			static constexpr bool value = false;
		};

		template<typename... Types>
		struct st_is_constrained_any< types::any<Types...> >
		{
			static constexpr bool value = true;
		};

	} // end of namespace hidden

	template<typename T>
	constexpr bool is_constrained_any_v = hidden::st_is_constrained_any<T>::value;

	template<typename T>
	constexpr bool is_special_type_v = is_constrained_any_v<T> || is_variant_v<T> || types::is_tuple_v<T>;
	
	template<auto N, typename TypeAny>
	using any_element_t = select_nth_type_t<N, typename TypeAny::element_types_t>; 

    template<typename... ElementTypes>
    class any
    {
		public:
			using element_types_t = unique_types_t<ElementTypes...>;
			
			using variant_t = types::to_variant_t<element_types_t>;
			
			static constexpr auto param_count
				= type_count_v<element_types_t>;			
			
			using pointer_types_t = 
				unique_types_t< std::add_pointer_t<ElementTypes>... >;
			
			using element_pointer_types_t = 
				union_type_t<element_types_t, pointer_types_t>;

			using index_vector_t = types::index_tuple_vector_t<param_count>;

		protected:
			std::any m_any;

			int m_dummy{};
		public:

			any(): m_any{} { }

			bool has_value() const noexcept
			{
				return this->m_any.has_value();
			}
			
			void reset() noexcept
			{
				this->m_any.reset();
			}

			void swap(any& other) noexcept
			{
				this->m_any.swap(other);
			}

			const std::type_info& type() const noexcept
			{
				return this->m_any.type();
			}

			template< class ValueType, class... Args,
				typename = std::enable_if_t< std::is_constructible_v<ValueType, remove_cv_ref_t<Args>...>>>
			enable_if_in_list_t<ValueType, element_types_t, std::decay_t<ValueType>&>
			emplace( Args&&... args )
			{
				return this->m_any. template emplace<ValueType>(std::forward<Args>(args)...);
			}

			template< class ValueType, class U, class... Args >
			enable_if_in_list_t<ValueType, element_types_t, std::decay_t<ValueType>&>
			emplace( std::initializer_list<U> il, Args&&... args )
			{
				return this->m_any. template emplace<ValueType>( il, std::forward<Args>(args)...);
			}
			
			std::string current_type() const noexcept;

			std::any& std() noexcept { return m_any; }
			const std::any& std() const noexcept { return m_any; }

			constexpr auto allowed_types() const noexcept
				{ return element_types_t{}; }

			variant_t to_variant() const noexcept;

			// if Type is not in the list, element_types_t,
			// this constructor is disabled
			template<typename Type,
				typename = enable_if_in_list_t<decay_remove_cv_ref_t<Type>, element_types_t>>
			any(Type&& arg): m_any{ std::forward<Type>(arg) } { }

			any(const any&) = default;
			any(any&& rhs) = default;

			any& operator=(const any&) = default;
			any& operator=(any&&) = default;

			// value assignment operator
			template<typename Type,
			typename = enable_if_in_list_t<decay_remove_cv_ref_t<Type>, element_types_t>>
			any& operator=(Type&& arg)
			{
				this->m_any = std::forward<Type>(arg);
				return *this;
			}

			any& operator=(const std::variant<ElementTypes...>& v)
			{
				constexpr size_t Size = sizeof...(ElementTypes);

				try
				{
					types::for_workhorse<Size>([&](auto index)
					{
						if(auto pval = std::get_if<index.Index>(&v))
						{
							this->m_any = *pval; 
							throw 1; //exit loop
						}
					});

				}catch(...) { }

				return *this;
			}

			///////////////////////////////// accessors //////////////////////////////
			template<typename Type,
				typename = enable_if_in_list_t<remove_cv_ref_t<Type>, element_pointer_types_t>>
			Type get()
			{
				// https://en.cppreference.com/w/cpp/utility/any/any_cast
				if constexpr(std::is_pointer_v<remove_cv_ref_t<Type>>)
					return std::any_cast<std::remove_pointer_t<Type>>(&this->m_any);
				else
					return std::any_cast<Type>(this->m_any);
			}

			template<typename Type,
				typename = enable_if_in_list_t<remove_cv_ref_t<Type>, element_pointer_types_t>>
			Type get() const
			{
				// https://en.cppreference.com/w/cpp/utility/any/any_cast
				if constexpr(std::is_pointer_v<remove_cv_ref_t<Type>>)
					return std::any_cast<std::remove_pointer_t<Type>>(&this->m_any);
				else
					return std::any_cast<Type>(this->m_any);
			}
			
			template<typename Type,
				typename = enable_if_in_list_t<remove_cv_ref_t<Type>, element_types_t>>
			operator Type&()
			{
				return this->get<Type&>();
			}

			template<typename Type,
				typename = enable_if_in_list_t<remove_cv_ref_t<Type>, element_types_t>>
			operator const Type&() const
			{
				return this->get<const Type&>();
			}

			template<typename CurrentType, typename CallbackType, typename... ArgTypes,
				typename = enable_if_in_list_t<CurrentType, element_types_t>>
			bool if_any_type(CallbackType&& callback, ArgTypes&&... args)
			{
				if(auto ptr = std::any_cast<CurrentType>(&this->m_any))
				{
					callback(*ptr, std::forward<ArgTypes>(args)...); return true;
				}
				else return false;
			}

			template<typename CurrentType, typename CallbackType, typename... ArgTypes,
				typename = enable_if_in_list_t<CurrentType, element_types_t>>
			bool if_any_type(CallbackType&& callback, ArgTypes&&... args) const
			{
				if(auto ptr = std::any_cast<CurrentType>(&this->m_any))
				{
					callback(*ptr, std::forward<ArgTypes>(args)...); return true;
				}
				else return false;
			}
		
			template<auto TypeIndex, typename CallbackType, typename... ArgTypes,
				typename any_type = types::any<ElementTypes...>,
				typename element_types = typename any_type::element_types_t,
				typename CurrentType = select_nth_type_t<TypeIndex, element_types>,
				typename = enable_if_in_list_t<CurrentType, element_types_t>>
			void if_any_index(CallbackType&& callback, ArgTypes&&... args)
			{
				if(auto ptr = std::any_cast<CurrentType>(&this->m_any)) 
					callback(*ptr, std::forward<ArgTypes>(args)...);
			}

			template<auto TypeIndex, typename CallbackType, typename... ArgTypes,
				typename any_type = types::any<ElementTypes...>,
				typename element_types = typename any_type::element_types_t,
				typename CurrentType = select_nth_type_t<TypeIndex, element_types>,
				typename = enable_if_in_list_t<CurrentType, element_types_t>>
			bool if_any_index(CallbackType&& callback, ArgTypes&&... args) const
			{
				if(auto ptr = std::any_cast<CurrentType>(&this->m_any))
					callback(*ptr, std::forward<ArgTypes>(args)...);
			}

			virtual ~any() { }

			////////////////////////////////////////////////////////////////////////////////
			template<typename Type,
			typename = enable_if_in_list_t<remove_cv_ref_t<Type>, element_types_t>>
			any& operator+=(Type&& arg)
			{
				using type = remove_cv_ref_t<Type>;
				this->get<type&>() += std::forward<Type>(arg);
				return *this;
			}

			template<typename Type,
			typename = enable_if_in_list_t<remove_cv_ref_t<Type>, element_types_t>>
			any& operator-=(Type&& arg)
			{
				using type = remove_cv_ref_t<Type>;
				this->get<type&>() -= std::forward<Type>(arg);
				return *this;
			}
			
			template<typename Type,
			typename = enable_if_in_list_t<remove_cv_ref_t<Type>, element_types_t>>
			any& operator*=(Type&& arg)
			{
				using type = remove_cv_ref_t<Type>;
				this->get<type&>() *= std::forward<Type>(arg);
				return *this;
			}

			template<typename Type,
			typename = enable_if_in_list_t<remove_cv_ref_t<Type>, element_types_t>>
			any& operator/=(Type&& arg)
			{
				using type = remove_cv_ref_t<Type>;
				this->get<type&>() /= std::forward<Type>(arg);
				return *this;
			}

			// template<typename CharType>
			// friend std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& os, const any& a)
			// {
			// 	if(!a.std().has_value())
			// 	{
			// 		os << "no_value"; return os;
			// 	}
			// 	else
			// 	{
			// 		constexpr int Size = sizeof...(ElementTypes);

			// 		try 
			// 		{
			// 			types::for_workhorse<Size>([&os, &a](auto i)
			// 			{
			// 				using element_types_t = typename any::element_types_t;

			// 				using element_t = select_nth_type_t<i.Index, element_types_t>;
							
			// 				if(auto ptr = std::any_cast<element_t>(&a.std()))
			// 				{
			// 					os << *ptr;

			// 					// operation successful
			// 					throw types::no_type_t{};
			// 				}
			// 			});

			// 		}catch(const types::no_type_t&) { }

			// 		return os;
			// 	}
			// }

    }; // end of class any

	template<typename... CallbackTypes>
	struct any_visitors: public CallbackTypes...
	{
		using CallbackTypes::operator()...;
			
			bool report_unhandled_value = false;

		public:

			/// returns true if successful, otherwise false
			template<typename Type, typename AnyType, typename... ArgTypes>
			std::enable_if_t<is_any_v<remove_cv_ref_t<AnyType>>, bool>
			if_any_type(AnyType&& any, ArgTypes&&... args) const
			{
				if(auto ptr = std::any_cast<Type>(&any))
				{
					this->operator()(*ptr, std::forward<ArgTypes>(args)...);
					return true;
				}
				else return false;
			}

			/// returns true if successful, otherwise false
			template<typename Type, typename AnyType, typename... ArgTypes>
			std::enable_if_t<is_constrained_any_v<remove_cv_ref_t<AnyType>>, bool>
			if_any_type(AnyType&& any, ArgTypes&&... args) const
			{
				if(auto ptr = std::any_cast<Type>(&any.std()))
				{
					this->operator()(*ptr, std::forward<ArgTypes>(args)...); return true;
				}
				else return false;
			}
					
			template<typename AnyType, typename... ArgTypes,
				typename any_type = remove_cv_ref_t<AnyType>,
				typename = std::enable_if_t<is_constrained_any_v<any_type>>,
				typename arg_list_t = arg_list_t<typename any_type::element_types_t, type_list_t<remove_cv_ref_t<ArgTypes>...>>>
			void apply(AnyType&& any, ArgTypes&&... args) const;

			template<typename Type, typename... Types, typename... ArgTypes, typename AnyType,
			typename any_type = remove_cv_ref_t<AnyType>,
			typename = std::enable_if_t<is_any_v<any_type> || is_constrained_any_v<any_type>>,
			typename arg_list_t = arg_list_t<type_list_t<Type, Types...>, type_list_t<remove_cv_ref_t<ArgTypes>...>>>
			void apply(AnyType&& any, ArgTypes&&... args) const;

			template<typename ContainerType, typename... ArgTypes>
			void visit(ContainerType&& container, ArgTypes&&... args) const
			{
				for(auto& a: container) this->template apply(a, std::forward<ArgTypes>(args)...);
			}

			template<typename ContainerType, typename... ArgTypes>
			void visit_with_index(ContainerType&& container, ArgTypes&&... args) const
			{
				for(int i = 0; i < (int)container.size(); ++i)
					this->template apply(container[i], i, std::forward<ArgTypes>(args)...);
			}

			template<typename Type, typename ContainerType, typename... ArgTypes>
			void visit_if_any_type(ContainerType&& container, ArgTypes&& ... args) const
			{
				for(auto& a: container)
					this->template if_any_type<Type>(a, std::forward<ArgTypes>(args)... );
			}

			template<typename Type, typename ContainerType, typename... ArgTypes>
			void visit_if_any_type_with_index(ContainerType&& container, ArgTypes&&... args) const
			{
				for(int i = 0; i < (int)container.size(); ++i)
					this->template if_any_type<Type>(container[i], i, std::forward<ArgTypes>(args)... );
			}
	};

	template<typename... CallbackTypes>
	any_visitors(CallbackTypes...)->any_visitors<CallbackTypes...>;

	template<typename Type, typename... Types>
	struct st_process_any
	{
		template<typename AnyType>
		static std::enable_if_t<is_constrained_any_v<remove_cv_ref_t<AnyType>>, std::string>
		current_type(AnyType&& a)
		{
			if(auto ptr = std::any_cast<Type>(&a.std()))
				return Tpf_GetTypeName(Type);
			else
			{
				if constexpr(sizeof...(Types) != 0)
					return st_process_any<Types...>::current_type(a);
				else // this part never really executed
					return "To suppress syntax error";
			}
		}

		template<typename VariantType, typename AnyType>
		static void variant_to_any(const VariantType& var, AnyType& any)
		{
			if(auto ptr = std::get_if<Type>(&var))
				any = *ptr;
			else
			{
				if constexpr(sizeof...(Types) != 0)
					st_process_any<Types...>::variant_to_any(var, any);		
			}
		}

		template<typename AnyType, typename VariantType>
		static void any_to_variant(const AnyType& any, VariantType& var)
		{
			if(auto ptr = std::any_cast<Type>(&any))
				var = *ptr;
			else
			{
				if constexpr(sizeof...(Types) != 0)
					st_process_any<Types...>::any_to_variant(any, var);		
			}
		}
	};

	template<typename Type, typename... Types>
	struct st_process_any<type_list_t<Type, Types...>>
	{
		template<typename AnyType, typename Visitors>
		static void visit_any(Visitors&& visitors, AnyType&& a)
		{
			if(auto ptr = std::any_cast<Type>(&a.std()))
				visitors(*ptr);
			else
			{
				if constexpr(sizeof...(Types) != 0)
					st_process_any<type_list_t<Types...>>::
						visit_any(visitors, std::forward<AnyType>(a));
			}
		}

		template<typename Visitors, typename AnyType, typename... ArgTypes>
		static void apply_to_any(Visitors&& visitor, AnyType&& any, ArgTypes&&... args)
		{
			if constexpr(types::is_type_list_v<Type>)
				st_process_any<Type>::apply_to_any(visitor, any, std::forward<ArgTypes>(args)...);
			else
				visitor.template apply<Type, Types...>(any, std::forward<ArgTypes>(args)...);
		}

		template<typename AnyType, typename VariantType>
		static void any_to_variant(const AnyType& any, VariantType& var)
		{
			st_process_any<Type, Types...>::any_to_variant(any, var);		
		}
	};

	template<typename Type, typename... Types>
	any<Type, Types...> variant_to_any(const std::variant<Type, Types...>& var)
	{
		using any_t = any<Type, Types...>; any_t any;
		st_process_any<Type, Types...>::variant_to_any(var, any);
		
		return any;
	}

	template<typename Type, typename... Types>
	std::any variant_to_std_any(const std::variant<Type, Types...>& var)
	{
		using any_t = std::any; any_t any;
		st_process_any<Type, Types...>::variant_to_any(var, any);
		
		return any;
	}

	template<typename... ElementTypes>
		typename any<ElementTypes...>::variant_t
	any<ElementTypes...>::to_variant() const noexcept
	{
		variant_t var;

		if(!this->has_value()) return var;
		else
		{
			st_process_any<element_types_t>::any_to_variant(this->std(), var);
			return var;
		}
	}

	template<typename... CallbackTypes>
		template<typename Type, typename... Types, typename... ArgTypes,
		typename AnyType, typename any_type, typename, typename arg_list_t>
	void any_visitors<CallbackTypes...>::apply(AnyType&& any, ArgTypes&&... args) const
	{
		if constexpr(is_constrained_any_v<AnyType>)
			this->apply<Type, Types...>(any.std(), std::forward<ArgTypes>(args)...);
		else
		{
			if constexpr(is_type_list_v<Type>)
			{
				st_process_any<Type>::apply_to_any(*this, any, std::forward<ArgTypes>(args)...);
				
				if constexpr(sizeof...(Types) != 0)
					this->apply<Types...>(any, std::forward<ArgTypes>(args)...);
			}
			else
			{
				if(!this->template if_any_type<Type>(any, std::forward<ArgTypes>(args)...))
				{
					if constexpr(sizeof...(Types) != 0)
						this->apply<Types...>(any, std::forward<ArgTypes>(args)...);
					else
					{
						if(this->report_unhandled_value && any.has_value())
						{
							throw std::runtime_error("WARNING - Any has value but not handled");
						}
					}
				}
			}
		}
	}

	template<typename... CallbackTypes>
	template<typename AnyType, typename... ArgTypes, typename any_type, typename, typename arg_list_t>
	void any_visitors<CallbackTypes...>::apply(AnyType&& any, ArgTypes&&... args) const
	{
		using any_t = remove_cv_ref_t<AnyType>;
		using element_types = typename any_t::element_types_t;

		st_process_any<element_types>::apply_to_any(*this, any.std(), std::forward<ArgTypes>(args)...);
	}
	
	template<typename... ElementTypes>
	std::string current_type(const any<ElementTypes...>& a)
	{
		return st_process_any<ElementTypes...>::current_type(a);
	}

	template<typename... ElementTypes>
	std::string any<ElementTypes...>::current_type() const noexcept
	{
		return st_process_any<ElementTypes...>::current_type(*this);
	}

	template<typename Visitors, typename AnyType, typename... ArgTypes>
	void visit_any(Visitors&& visitors, AnyType&& a, ArgTypes&&... args)
	{
		using any_t = remove_cv_ref_t<AnyType>;
		
		st_process_any<typename any_t::element_types_t>::
			apply_to_any(visitors, std::forward<AnyType>(a), std::forward<ArgTypes>(args)...);
	}

	template<typename CurrentType, typename AnyType, typename Handler,
		typename = std::enable_if<std::is_same_v<remove_cv_ref_t<AnyType>, std::any>>>
	void if_any_type(AnyType&& a, Handler&& handler)
	{
		if(auto ptr = std::any_cast<CurrentType>(&a)) handler(*ptr);
	}

} // end of namespace tpf::types


#ifdef _TPF_ANY_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_ANY_MIN_DEFINED
#endif 

#ifdef _TPF_ANY_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_ANY_MAX_DEFINED
#endif 

#endif // end of file _TPF_ANY_HPP