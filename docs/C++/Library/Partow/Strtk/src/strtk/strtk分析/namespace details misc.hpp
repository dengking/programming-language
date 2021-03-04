   namespace details
   {
      struct not_supported_type_tag   {};
      struct unsigned_type_tag        {};
      struct signed_type_tag          {};
      struct real_type_tag            {};
      struct byte_type_tag            {};
      struct bool_type_tag            {};
      struct hex_number_type_tag      {};
      struct hex_string_type_tag      {};
      struct base64_type_tag          {};
      struct ignore_token_type_tag    {};
      struct stdstring_type_tag       {};
      struct stdstring_range_type_tag {};
      struct value_type_tag           {};
      struct sink_type_tag            {};
      struct stl_seq_type_tag         {};
      struct attribute_type_tag       {};
      struct semantic_action_type_tag {};
      struct expect_type_tag          {};
      struct like_type_tag            {};
      struct inrange_type_tag         {};
      struct trim_type_tag            {};
      struct lcase_type_tag           {};
      struct ucase_type_tag           {};
      struct fillchararray_type_tag   {};
      struct truncint_type_tag        {};
      struct decsink_type_tag         {};

      template <typename RealType> struct real_type {};
      template <> struct real_type<float>       { typedef double type;      };
      template <> struct real_type<double>      { typedef double type;      };
      template <> struct real_type<long double> { typedef long double type; };

      template <typename T>
      struct supported_conversion_to_type
      {
         typedef not_supported_type_tag type;
      };

      template <typename T>
      struct supported_conversion_from_type
      {
         typedef not_supported_type_tag type;
      };

      template <bool, typename T = void>
      struct enable_if {};

      template <typename T>
      struct enable_if<true, T> { typedef T type; };

      template <typename T>
      struct supported_iterator_type
      {
         enum { value = false };
      };

      template <typename T>
      struct is_valid_iterator
      {
         typedef typename details::enable_if<details::supported_iterator_type<T>::value,T>::type type;
      };

      template <typename T> struct numeric;
      template <typename T> inline std::size_t type_length(const T&);

      struct no_t  {};
      struct yes_t {};

      template <typename T>
      struct is_pod
      {
         typedef no_t result_t;
         enum { result = false };
      };

      template <typename T>
      struct is_stl_container
      { typedef no_t result_t; };

      #define register_stl_container1(C) \
      template <typename T1, typename T2>struct is_stl_container<C<T1,T2> >{ typedef yes_t result_t; };

      #define register_stl_container2(C) \
      template <typename T1, typename T2, typename T3>struct is_stl_container<C<T1,T2,T3> >{ typedef yes_t result_t; };

      register_stl_container1(std::vector)
      register_stl_container1(std::deque)
      register_stl_container1(std::list)
      register_stl_container1(std::queue)
      register_stl_container1(std::stack)
      register_stl_container2(std::set)
      register_stl_container2(std::multiset)
      register_stl_container2(std::priority_queue)

      #undef register_stl_container1
      #undef register_stl_container2

      template <typename T>
      inline void convert_type_assert(){}

   } // namespace details