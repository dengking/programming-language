   inline const std::string::value_type* to_ptr(const std::string& s)
   {
      return &s[0];
   }

   inline std::string::value_type* to_ptr(std::string& s)
   {
      return &s[0];
   }

   inline const std::string::value_type* to_ptr(const std::string& s)
   {
      return &s[0];
   }

   inline std::string::value_type* to_ptr(std::string& s)
   {
      return &s[0];
   }

   template <typename Iterator, typename T>
   inline bool string_to_type_converter(const Iterator begin, const Iterator end, T& t)
   {
      typedef typename details::is_valid_iterator<Iterator>::type itr_type;
      typename details::supported_conversion_to_type<T>::type type;
      details::convert_type_assert<itr_type>();
      Iterator itr = begin;
      return string_to_type_converter_impl(itr,end,t,type);
   }

   template <typename Iterator, typename T>
   inline bool string_to_type_converter(const std::pair<Iterator,Iterator>& range, T& t)
   {
      return string_to_type_converter(range.first,range.second,t);
   }

   template <typename T, typename Iterator>
   inline T string_to_type_converter(const Iterator begin, const Iterator end)
   {
      typedef typename details::is_valid_iterator<Iterator>::type itr_type;
      typename details::supported_conversion_to_type<T>::type type;
      details::convert_type_assert<itr_type>();
      T t;
      Iterator itr = begin;
      if (string_to_type_converter_impl(itr,end,t,type))
         return t;
      else
         throw std::invalid_argument("string_to_type_converter() - Failed to convert: " +
                                     std::string(begin,end));
   }

   template <typename T, typename Iterator>
   inline T string_to_type_converter(const std::pair<Iterator,Iterator>& range)
   {
      return string_to_type_converter<T>(range.first,range.second);
   }

   template <typename T>
   inline bool string_to_type_converter(const std::string& s, T& t)
   {
      return string_to_type_converter<const char*,T>(to_ptr(s), to_ptr(s) + s.size(), t);
   }

   template <typename T>
   inline T string_to_type_converter(const std::string& s)
   {
      return string_to_type_converter<T>(to_ptr(s), to_ptr(s) + s.size());
   }

   template <typename T>
   inline bool type_to_string(const T& t, std::string& s)
   {
      typename details::supported_conversion_from_type<T>::type type;
      return type_to_string_converter_impl(t,s,type);
   }

   template <typename T>
   inline std::string type_to_string(const T& t)
   {
      std::string s;
      if (type_to_string<T>(t,s))
         return s;
      else
         throw std::invalid_argument("type_to_string() - Failed to convert type to string");
   }
