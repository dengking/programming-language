   template <typename InputIterator,
             typename T1 , typename  T2, typename  T3, typename  T4,
             typename T5 , typename  T6, typename  T7, typename  T8,
             typename T9 , typename T10, typename T11, typename T12,
             typename T13, typename T14, typename T15, typename T16>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1&  t1 ,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5&  t5 ,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9&  t9 , T10& t10, T11& t11, T12& t12,
                     T13& t13, T14& t14, T15& t15, T16& t16)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 16;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;

      #define strtk_cmpstmt(N)                                         \
      if (!string_to_type_converter((*itr).first,(*itr).second, t##N)) \
         return false;                                                 \
      ++itr;                                                           \

      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      strtk_cmpstmt(10)
      strtk_cmpstmt(11)
      strtk_cmpstmt(12)
      strtk_cmpstmt(13)
      strtk_cmpstmt(14)
      strtk_cmpstmt(15)
      return string_to_type_converter((*itr).first,(*itr).second,t16);
   }

   template <typename InputIterator,
             typename T1 , typename  T2, typename  T3, typename  T4,
             typename T5 , typename  T6, typename  T7, typename  T8,
             typename T9 , typename T10, typename T11, typename T12,
             typename T13, typename T14, typename T15>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1&  t1 ,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5&  t5 ,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9&  t9 , T10& t10, T11& t11, T12& t12,
                     T13& t13, T14& t14, T15& t15)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 15;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;

      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      strtk_cmpstmt(10)
      strtk_cmpstmt(11)
      strtk_cmpstmt(12)
      strtk_cmpstmt(13)
      strtk_cmpstmt(14)
      return string_to_type_converter((*itr).first,(*itr).second,t15);
   }

   template <typename InputIterator,
             typename T1 , typename  T2, typename  T3, typename  T4,
             typename T5 , typename  T6, typename  T7, typename  T8,
             typename T9 , typename T10, typename T11, typename T12,
             typename T13, typename T14>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1&  t1 ,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5&  t5 ,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9&  t9 , T10& t10, T11& t11, T12& t12,
                     T13& t13, T14& t14)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 14;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;

      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      strtk_cmpstmt(10)
      strtk_cmpstmt(11)
      strtk_cmpstmt(12)
      strtk_cmpstmt(13)
      return string_to_type_converter((*itr).first,(*itr).second,t14);
   }

   template <typename InputIterator,
             typename T1 , typename  T2, typename  T3, typename  T4,
             typename T5 , typename  T6, typename  T7, typename  T8,
             typename T9 , typename T10, typename T11, typename T12,
             typename T13>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1&  t1 ,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5&  t5 ,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9&  t9 , T10& t10, T11& t11, T12& t12,
                     T13& t13)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 13;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;

      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      strtk_cmpstmt(10)
      strtk_cmpstmt(11)
      strtk_cmpstmt(12)
      return string_to_type_converter((*itr).first,(*itr).second,t13);
   }

   template <typename InputIterator,
             typename T1, typename  T2, typename  T3, typename T4,
             typename T5, typename  T6, typename  T7, typename T8,
             typename T9, typename T10, typename T11, typename T12>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9& t9, T10& t10, T11& t11, T12& t12)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 12;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;

      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      strtk_cmpstmt(10)
      strtk_cmpstmt(11)
      return string_to_type_converter((*itr).first,(*itr).second,t12);
   }

   template <typename InputIterator,
             typename T1, typename  T2, typename  T3, typename T4,
             typename T5, typename  T6, typename  T7, typename T8,
             typename T9, typename T10, typename T11>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9, T10& t10, T11& t11)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 11;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);

      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;

      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      strtk_cmpstmt(10)
      return string_to_type_converter((*itr).first,(*itr).second,t11);
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9, typename T10>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9, T10& t10)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 10;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);

      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      return string_to_type_converter((*itr).first,(*itr).second,t10);
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 9;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      return string_to_type_converter((*itr).first,(*itr).second,t9);
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 8;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      return string_to_type_converter((*itr).first,(*itr).second,t8);
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 7;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      return string_to_type_converter((*itr).first,(*itr).second,t7);
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 6;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      return string_to_type_converter((*itr).first,(*itr).second,t6);
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3, typename T4,
             typename T5>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 5;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);

      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      return string_to_type_converter((*itr).first,(*itr).second,t5);
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3, typename T4>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 4;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);

      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      return string_to_type_converter((*itr).first,(*itr).second,t4);
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 3;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);

      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      return string_to_type_converter((*itr).first,(*itr).second,t3);
   }

   template <typename InputIterator, typename T1, typename T2>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 2;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      strtk_cmpstmt(1)
      return string_to_type_converter((*itr).first,(*itr).second,t2);
      #undef strtk_cmpstmt
   }

   template <typename InputIterator, typename T>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T& t)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      static const std::size_t token_count = 1;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];

      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin, end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count > parsed_token_count)
         return false;

      iterator_type_ptr itr = token_list;
      return string_to_type_converter((*itr).first,(*itr).second,t);
   }

   template <typename InputIterator,
             typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            Sequence<T,Allocator>& sequence,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin, end,
                      range_to_type_back_inserter(sequence),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin, end,
                      range_to_type_back_inserter(sequence),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            std::set<T,Comparator,Allocator>& set,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin, end,
                      range_to_type_inserter(set),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin, end,
                      range_to_type_inserter(set),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            std::multiset<T,Comparator,Allocator>& multiset,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin, end,
                      range_to_type_inserter(multiset),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin, end,
                      range_to_type_inserter(multiset),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            std::queue<T,Container>& queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin, end,
                      range_to_type_push_inserter(queue),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin, end,
                      range_to_type_push_inserter(queue),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            std::stack<T,Container>& stack,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin, end,
                      range_to_type_push_inserter(stack),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin, end,
                      range_to_type_push_inserter(stack),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            std::priority_queue<T,Container,Comparator>& priority_queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin, end,
                      range_to_type_push_inserter(priority_queue),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin, end,
                      range_to_type_push_inserter(priority_queue),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse(const std::pair<InputIterator,InputIterator>& range,
                            const std::string& delimiters,
                            Sequence<T,Allocator>& sequence,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      range.first,range.second,
                      range_to_type_back_inserter(sequence),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      range.first,range.second,
                      range_to_type_back_inserter(sequence),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse(const std::pair<InputIterator,InputIterator>& range,
                            const std::string& delimiters,
                            std::set<T,Comparator,Allocator>& set,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      range.first,range.second,
                      range_to_type_inserter(set),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      range.first,range.second,
                      range_to_type_inserter(set),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse(const std::pair<InputIterator,InputIterator>& range,
                            const std::string& delimiters,
                            std::multiset<T,Comparator,Allocator>& multiset,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      range.first,range.second,
                      range_to_type_inserter(multiset),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      range.first,range.second,
                      range_to_type_inserter(multiset),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse(const std::pair<InputIterator,InputIterator>& range,
                            const std::string& delimiters,
                            std::queue<T,Container>& queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      range.first,range.second,
                      range_to_type_push_inserter(queue),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      range.first,range.second,
                      range_to_type_push_inserter(queue),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse(const std::pair<InputIterator,InputIterator>& range,
                            const std::string& delimiters,
                            std::stack<T,Container>& stack,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      range.first,range.second,
                      range_to_type_push_inserter(stack),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      range.first,range.second,
                      range_to_type_push_inserter(stack),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse(const std::pair<InputIterator,InputIterator>& range,
                            const std::string& delimiters,
                            std::priority_queue<T,Container,Comparator>& priority_queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      range.first,range.second,
                      range_to_type_push_inserter(priority_queue),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      range.first,range.second,
                      range_to_type_push_inserter(priority_queue),
                      split_option);
   }

      template <typename InputIterator,
             typename  T1, typename  T2, typename T3,
             typename  T4, typename  T5, typename T6,
             typename  T7, typename  T8, typename T9,
             typename T10, typename T11>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2,  T3&  t3,  T4&  t4, T5& t5, T6& t6, T7& t7,
                     T8& t8, T9& t9, T10& t10, T11& t11,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 12) return false;

      iterator_type_ptr itr = token_list.begin();

      #define strtk_cmpstmt(N)                                         \
      if (!string_to_type_converter((*itr).first,(*itr).second, t##N)) \
         return false;                                                 \
      ++itr;                                                           \

      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      strtk_cmpstmt(10)
      strtk_cmpstmt(11)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3,
             typename T4, typename T5, typename T6,
             typename T7, typename T8, typename T9,
             typename T10>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 11) return false;

      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      strtk_cmpstmt(10)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3,
             typename T4, typename T5, typename T6,
             typename T7, typename T8, typename T9>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 10) return false;

      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      strtk_cmpstmt( 9)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3,
             typename T4, typename T5, typename T6,
             typename T7, typename T8>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 9) return false;

      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      strtk_cmpstmt( 8)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3,
             typename T4, typename T5, typename T6, typename T7>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 8) return false;

      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      strtk_cmpstmt( 7)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3,
             typename T4, typename T5, typename T6>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 7) return false;

      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      strtk_cmpstmt( 6)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3,
             typename T4, typename T5>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4, T5& t5,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 6) return false;

      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      strtk_cmpstmt( 5)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3, typename T4>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
        split(multiple_char_delimiter_predicate(delimiters),
              begin, end,
              std::back_inserter(token_list),
              split_options::compress_delimiters);

      if (token_list.size() < 5) return false;

      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      strtk_cmpstmt( 4)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator,
             typename T1, typename T2, typename T3>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 4) return false;

      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      strtk_cmpstmt( 3)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator,
             typename T1, typename T2>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 3) return false;
      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      strtk_cmpstmt( 2)
      return ca(itr,token_list.end());
   }

   template <typename InputIterator, typename T1>
   inline bool parse(const InputIterator begin, const InputIterator end,
                     const std::string& delimiters,
                     T1& t1,
                     details::container_adder ca)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef typename std::deque<iterator_type>::iterator iterator_type_ptr;

      details::convert_type_assert<itr_type>();

      std::deque<iterator_type> token_list;

      if (1 == delimiters.size())
         split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);
      else
         split(multiple_char_delimiter_predicate(delimiters),
               begin, end,
               std::back_inserter(token_list),
               split_options::compress_delimiters);

      if (token_list.size() < 2) return false;

      iterator_type_ptr itr = token_list.begin();
      strtk_cmpstmt( 1)
      return ca(itr,token_list.end());
      #undef strtk_cmpstmt
   }

   template <typename InputIterator,
             typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              Sequence<T,Allocator>& sequence,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      const std::size_t original_size = sequence.size();

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                 begin, end,
                 n,
                 range_to_type_back_inserter(sequence),
                 split_option);
      else
         split_n(multiple_char_delimiter_predicate(delimiters),
                 begin, end,
                 n,
                 range_to_type_back_inserter(sequence),
                 split_option);

      return sequence.size() - original_size;
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::set<T,Comparator,Allocator>& set,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      const std::size_t original_size = set.size();

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                 begin, end,
                 n,
                 range_to_type_inserter(set),
                 split_option);
      else
         split_n(multiple_char_delimiter_predicate(delimiters),
                 begin, end,
                 n,
                 range_to_type_inserter(set),
                 split_option);

      return set.size() - original_size;
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::multiset<T,Comparator,Allocator>& multiset,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      const std::size_t original_size = multiset.size();

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                 begin, end,
                 n,
                 range_to_type_inserter(multiset),
                 split_option);
      else
         split_n(multiple_char_delimiter_predicate(delimiters),
                 begin, end,
                 n,
                 range_to_type_inserter(multiset),
                 split_option);

      return multiset.size() - original_size;
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::queue<T,Container>& queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      const std::size_t original_size = queue.size();

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                 begin, end,
                 n,
                 range_to_type_push_inserter(queue),
                 split_option);
      else
         split_n(multiple_char_delimiter_predicate(delimiters),
                 begin, end,
                 n,
                 range_to_type_push_inserter(queue),
                 split_option);

      return queue.size() - original_size;
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::stack<T,Container>& stack,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      const std::size_t original_size = stack.size();

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                 begin, end,
                 n,
                 range_to_type_push_inserter(stack),
                 split_option);
      else
         split_n(multiple_char_delimiter_predicate(delimiters),
                 begin, end,
                 n,
                 range_to_type_push_inserter(stack),
                 split_option);

      return stack.size() - original_size;
   }

   template <typename InputIterator,
             typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::priority_queue<T,Container,Comparator>& priority_queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      const std::size_t original_size = priority_queue.size();

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                 begin, end,
                 n,
                 range_to_type_push_inserter(priority_queue),
                 split_option);
      else
         split_n(multiple_char_delimiter_predicate(delimiters),
                 begin, end,
                 n,
                 range_to_type_push_inserter(priority_queue),
                 split_option);

      return priority_queue.size() - original_size;
   }

   template <typename InputIterator, typename T>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              T* out,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      std::size_t insert_count = 0;

      details::convert_type_assert<itr_type>();

      if (1 == delimiters.size())
         split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                 begin, end,
                 n,
                 range_to_ptr_type(out,insert_count),
                 split_option);
      else
         split_n(multiple_char_delimiter_predicate(delimiters),
                 begin, end,
                 n,
                 range_to_ptr_type(out,insert_count),
                 split_option);

      return insert_count;
   }

   template <typename InputIterator,
             typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse_n(const std::pair<InputIterator,InputIterator>& range,
                              const std::string& delimiters,
                              const std::size_t& n,
                              Sequence<T,Allocator>& sequence,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(range.first,range.second,delimiters,n,sequence,split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_n(const std::pair<InputIterator,InputIterator>& range,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::set<T,Comparator,Allocator>& set,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(range.first,range.second,delimiters,n,set,split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_n(const std::pair<InputIterator,InputIterator>& range,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::multiset<T,Comparator,Allocator>& multiset,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(range.first,range.second,delimiters,n,multiset,split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse_n(const std::pair<InputIterator,InputIterator>& range,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::queue<T,Container>& queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(range.first,range.second,delimiters,n,queue,split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse_n(const std::pair<InputIterator,InputIterator>& range,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::stack<T,Container>& stack,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(range.first,range.second,delimiters,n,stack,split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse_n(const std::pair<InputIterator,InputIterator>& range,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::priority_queue<T,Container,Comparator>& priority_queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(range.first,range.second,delimiters,n,priority_queue,split_option);
   }

   template <typename T1 , typename T2 , typename T3 , typename  T4,
             typename T5 , typename T6 , typename T7 , typename  T8,
             typename T9 , typename T10, typename T11, typename T12,
             typename T13, typename T14, typename T15, typename T16>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1&  t1 ,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5&  t5 ,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9&  t9 , T10& t10, T11& t11, T12& t12,
                     T13& t13, T14& t14, T15& t15, T16& t16)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,
                   typename details::ca_type<T16, typename details::is_stl_container<T12>::result_t>::type(t16));
   }

   template <typename T1 , typename T2 , typename T3 , typename  T4,
             typename T5 , typename T6 , typename T7 , typename  T8,
             typename T9 , typename T10, typename T11, typename T12,
             typename T13, typename T14, typename T15>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1&  t1 ,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5&  t5 ,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9&  t9 , T10& t10, T11& t11, T12& t12,
                     T13& t13, T14& t14, T15& t15)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,
                   typename details::ca_type<T15, typename details::is_stl_container<T12>::result_t>::type(t15));
   }

   template <typename T1 , typename T2 , typename T3 , typename  T4,
             typename T5 , typename T6 , typename T7 , typename  T8,
             typename T9 , typename T10, typename T11, typename T12,
             typename T13, typename T14>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1&  t1 ,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5&  t5 ,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9&  t9 , T10& t10, T11& t11, T12& t12,
                     T13& t13, T14& t14)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,
                   typename details::ca_type<T14, typename details::is_stl_container<T12>::result_t>::type(t14));
   }

   template <typename T1 , typename T2 , typename T3 , typename  T4,
             typename T5 , typename T6 , typename T7 , typename  T8,
             typename T9 , typename T10, typename T11, typename T12,
             typename T13>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1&  t1 ,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5&  t5 ,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9&  t9 , T10& t10, T11& t11, T12& t12,
                     T13& t13)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,
                   typename details::ca_type<T13, typename details::is_stl_container<T12>::result_t>::type(t13));
   }

   template <typename T1, typename T2 , typename T3 , typename  T4,
             typename T5, typename T6 , typename T7 , typename  T8,
             typename T9, typename T10, typename T11, typename T12>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9& t9, T10& t10, T11& t11, T12& t12)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,
                   typename details::ca_type<T12, typename details::is_stl_container<T12>::result_t>::type(t12));
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9, typename T10, typename T11>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2&   t2,  T3&  t3, T4& t4,
                     T5& t5, T6&   t6,  T7&  t7, T8& t8,
                     T9& t9, T10& t10, T11& t11)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,
                   typename details::ca_type<T11, typename details::is_stl_container<T11>::result_t>::type(t11));
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9, typename T10>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9, T10& t10)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,
                   typename details::ca_type<T10, typename details::is_stl_container<T10>::result_t>::type(t10));
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,
                   typename details::ca_type<T9, typename details::is_stl_container<T9>::result_t>::type(t9));
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,
                   typename details::ca_type<T8, typename details::is_stl_container<T8>::result_t>::type(t8));
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,
                   typename details::ca_type<T7, typename details::is_stl_container<T7>::result_t>::type(t7));
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,
                   typename details::ca_type<T6,typename details::is_stl_container<T6>::result_t>::type(t6));
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,t4,
                   typename details::ca_type<T5, typename details::is_stl_container<T5>::result_t>::type(t5));
   }

   template <typename T1, typename T2, typename T3, typename T4>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,t3,
                   typename details::ca_type<T4, typename details::is_stl_container<T4>::result_t>::type(t4));
   }

   template <typename T1, typename T2, typename T3>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,t2,
                   typename details::ca_type<T3, typename details::is_stl_container<T3>::result_t>::type(t3));
   }

   template <typename T1, typename T2>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   t1,
                   typename details::ca_type<T2, typename details::is_stl_container<T2>::result_t>::type(t2));
   }

   template <typename T>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T& t)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   typename details::ca_type<T,typename details::is_stl_container<T>::result_t>::type(t));
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            Sequence<T,Allocator>& sequence,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   sequence,
                   split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            std::set<T,Comparator,Allocator>& set,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   set,
                   split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            std::multiset<T,Comparator,Allocator>& multiset,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   multiset,
                   split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            std::queue<T,Container>& queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   queue,
                   split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            std::stack<T,Container>& stack,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   stack,
                   split_option);
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            std::priority_queue<T,Container,Comparator>& priority_queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(to_ptr(data), to_ptr(data) + data.size(),
                   delimiters,
                   priority_queue,
                   split_option);
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse(const int& argc, char* argv[],
                            Sequence<T,Allocator>& sequence,
                            const bool break_on_fail = true)
   {
      T tmp;

      for (int i = 0; i < argc; ++i)
      {
         if (!string_to_type_converter(std::string(argv[i]),tmp))
         {
            if (break_on_fail)
               return i;
            else
               continue;
         }

         sequence.push_back(tmp);
      }

      return argc;
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8,
             typename T9>
   inline std::size_t parse(const int& argc, char* argv[],
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6, T7& t7, T8& t8,
                            T9& t9)

   {
      if (9 != argc) return 0;
      std::size_t result = 0;

      #define strtk_cmpstmt(N)                                  \
      if (!string_to_type_converter(std::string(argv[N]),t##N)) \
         return result;                                         \
      ++result;                                                 \

      strtk_cmpstmt(1)
      strtk_cmpstmt(2)
      strtk_cmpstmt(3)
      strtk_cmpstmt(4)
      strtk_cmpstmt(5)
      strtk_cmpstmt(6)
      strtk_cmpstmt(7)
      strtk_cmpstmt(8)
      strtk_cmpstmt(9)
      return result;
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7, typename T8>
   inline std::size_t parse(const int& argc, char* argv[],
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6, T7& t7, T8& t8)

   {
      if (8 != argc) return 0;
      std::size_t result = 0;
      strtk_cmpstmt(1)
      strtk_cmpstmt(2)
      strtk_cmpstmt(3)
      strtk_cmpstmt(4)
      strtk_cmpstmt(5)
      strtk_cmpstmt(6)
      strtk_cmpstmt(7)
      strtk_cmpstmt(8)
      return result;
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6, typename T7>
   inline std::size_t parse(const int& argc, char* argv[],
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6, T7& t7)

   {
      if (7 != argc) return 0;
      std::size_t result = 0;
      strtk_cmpstmt(1)
      strtk_cmpstmt(2)
      strtk_cmpstmt(3)
      strtk_cmpstmt(4)
      strtk_cmpstmt(5)
      strtk_cmpstmt(6)
      strtk_cmpstmt(7)
      return result;
   }

   template <typename T1, typename T2, typename T3, typename T4,
             typename T5, typename T6>
   inline std::size_t parse(const int& argc, char* argv[],
                            T1& t1, T2& t2, T3& t3, T4& t4,
                            T5& t5, T6& t6)

   {
      if (6 != argc) return 0;
      std::size_t result = 0;
      strtk_cmpstmt(1)
      strtk_cmpstmt(2)
      strtk_cmpstmt(3)
      strtk_cmpstmt(4)
      strtk_cmpstmt(5)
      strtk_cmpstmt(6)
      return result;
   }

   template <typename T1, typename T2, typename T3, typename T4, typename T5>
   inline std::size_t parse(const int& argc, char* argv[],
                            T1& t1, T2& t2, T3& t3, T4& t4, T5& t5)
   {
      if (5 != argc) return 0;
      std::size_t result = 0;
      strtk_cmpstmt(1)
      strtk_cmpstmt(2)
      strtk_cmpstmt(3)
      strtk_cmpstmt(4)
      strtk_cmpstmt(5)
      return result;
   }

   template <typename T1, typename T2, typename T3, typename T4>
   inline std::size_t parse(const int& argc, char* argv[],
                            T1& t1, T2& t2, T3& t3, T4& t4)
   {
      if (4 != argc) return 0;
      std::size_t result = 0;
      strtk_cmpstmt(1)
      strtk_cmpstmt(2)
      strtk_cmpstmt(3)
      strtk_cmpstmt(4)
      return result;
   }

   template <typename T1, typename T2, typename T3>
   inline std::size_t parse(const int& argc, char* argv[],
                            T1& t1, T2& t2, T3& t3)
   {
      if (3 != argc) return 0;
      std::size_t result = 0;
      strtk_cmpstmt(1)
      strtk_cmpstmt(2)
      strtk_cmpstmt(3)
      return result;
   }

   template <typename T1, typename T2>
   inline std::size_t parse(const int& argc, char* argv[],
                            T1& t1, T2& t2)
   {
      if (2 != argc) return 0;
      std::size_t result = 0;
      strtk_cmpstmt(1)
      strtk_cmpstmt(2)
      return result;
   }

   template <typename T1>
   inline std::size_t parse(const int& argc, char* argv[],
                            T1& t1)
   {
      if (1 != argc) return 0;
      std::size_t result = 0;
      strtk_cmpstmt(1)
      return result;
      #undef strtk_cmpstmt
   }

   #define strtk_parse_begin(Type)                                            \
   namespace strtk {                                                          \
   bool parse(const std::string& data, const std::string& delimiters, Type& t)\
   { return parse(data,delimiters                                             \

   #define strtk_parse_type(T) \
   ,t.T                        \

   #define strtk_parse_hex_type(T) \
   ,t.T                            \

   #define strtk_parse_ignore_token() \
   ,ignore_token()                    \

   #define strtk_parse_end() \
   );}}                      \

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              Sequence<T,Allocator>& sequence,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(to_ptr(data), to_ptr(data) + data.size(),
                     delimiters,
                     n,
                     sequence,
                     split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::set<T,Comparator,Allocator>& set,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(to_ptr(data), to_ptr(data) + data.size(),
                     delimiters,
                     n,
                     set,
                     split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::multiset<T,Comparator,Allocator>& multiset,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(to_ptr(data), to_ptr(data) + data.size(),
                     delimiters,
                     n,
                     multiset,
                     split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::queue<T,Container>& queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(to_ptr(data), to_ptr(data) + data.size(),
                     delimiters,
                     n,
                     queue,
                     split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::stack<T,Container>& stack,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(to_ptr(data), to_ptr(data) + data.size(),
                     delimiters,
                     n,
                     stack,
                     split_option);
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::priority_queue<T,Container,Comparator>& priority_queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(to_ptr(data), to_ptr(data) + data.size(),
                     delimiters,
                     n,
                     priority_queue,
                     split_option);
   }

   template <typename T>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              T* out,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(to_ptr(data), to_ptr(data) + data.size(),
                     delimiters,
                     n,
                     out,
                     split_option);
   }

   template <typename T1, typename  T2, typename  T3, typename T4,
             typename T5, typename  T6, typename  T7, typename T8,
             typename T9, typename T10, typename T11, typename T12>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                          T7& t7, T8& t8, T9& t9, T10& t10, T11& t11, T12& t12)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3,t4,t5,t6,
                          t7,t8,t9,t10,t11,t12);
   }

   template <typename T1, typename  T2, typename  T3, typename T4,
             typename T5, typename  T6, typename  T7, typename T8,
             typename T9, typename T10, typename T11>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                          T7& t7, T8& t8, T9& t9, T10& t10, T11& t11)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3,t4,t5,t6,
                          t7,t8,t9,t10,t11);
   }

   template <typename T1, typename  T2, typename  T3, typename T4,
             typename T5, typename  T6, typename  T7, typename T8,
             typename T9, typename T10>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3, T4&  t4, T5& t5, T6& t6,
                          T7& t7, T8& t8, T9& t9, T10& t10)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3,t4,t5,t6,
                          t7,t8,t9,t10);
   }

   template <typename T1, typename  T2, typename  T3, typename T4,
             typename T5, typename  T6, typename  T7, typename T8,
             typename T9>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                          T7& t7, T8& t8, T9& t9)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3,t4,t5,t6,
                          t7,t8,t9);
   }

   template <typename T1, typename  T2, typename  T3, typename T4,
             typename T5, typename  T6, typename  T7, typename T8>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                          T7& t7, T8& t8)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3,t4,t5,t6,
                          t7,t8);
   }

   template <typename T1, typename  T2, typename  T3, typename T4,
             typename T5, typename  T6, typename  T7>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                          T7& t7)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3,t4,t5,t6,t7);
   }

   template <typename T1, typename  T2, typename  T3, typename T4,
             typename T5, typename  T6>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3,t4,t5,t6);
   }

   template <typename T1, typename  T2, typename  T3, typename T4,
             typename T5>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3, T4& t4, T5& t5)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3,t4,t5);
   }

   template <typename T1, typename  T2, typename  T3, typename T4>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3, T4& t4)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3,t4);
   }

   template <typename T1, typename  T2, typename  T3>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2, T3& t3)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2,t3);
   }

   template <typename T1, typename  T2>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1, T2& t2)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1,t2);
   }

   template <typename T1>
   inline bool parse_line(std::ifstream& stream,
                          const std::string& delimiters,
                          T1& t1)
   {
      if (!stream)
         return false;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return false;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          t1);
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse_line(std::ifstream& stream,
                                 const std::string& delimiters,
                                 Sequence<T,Allocator>& sequence,
                                 const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          sequence,
                          split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_line(std::ifstream& stream,
                                 const std::string& delimiters,
                                 std::set<T,Comparator,Allocator>& set,
                                 const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          set,
                          split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_line(std::ifstream& stream,
                                 const std::string& delimiters,
                                 std::multiset<T,Comparator,Allocator>& multiset,
                                 const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          multiset,
                          split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse_line(std::ifstream& stream,
                                 const std::string& delimiters,
                                 std::queue<T,Container>& queue,
                                 const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          queue,
                          split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse_line(std::ifstream& stream,
                                 const std::string& delimiters,
                                 std::stack<T,Container>& stack,
                                 const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          stack,
                          split_option);
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse_line(std::ifstream& stream,
                                 const std::string& delimiters,
                                 std::priority_queue<T,Container,Comparator>& priority_queue,
                                 const split_options::type& split_option = split_options::compress_delimiters)

   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return false;
      return strtk::parse(to_ptr(data), to_ptr(data) + data.size(),
                          delimiters,
                          priority_queue,
                          split_option);
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse_line_n(std::ifstream& stream,
                                   const std::string& delimiters,
                                   const std::size_t& n,
                                   Sequence<T,Allocator>& sequence,
                                   const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return 0;
      return strtk::parse_n(to_ptr(data), to_ptr(data) + data.size(),
                            delimiters,
                            n,
                            sequence,
                            split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_line_n(std::ifstream& stream,
                                   const std::string& delimiters,
                                   const std::size_t& n,
                                   std::set<T,Comparator,Allocator>& set,
                                   const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return 0;
      return strtk::parse_n(to_ptr(data), to_ptr(data) + data.size(),
                            delimiters,
                            n,
                            set,
                            split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_line_n(std::ifstream& stream,
                                   const std::string& delimiters,
                                   const std::size_t& n,
                                   std::multiset<T,Comparator,Allocator>& multiset,
                                   const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return 0;
      return strtk::parse_n(to_ptr(data), to_ptr(data) + data.size(),
                            delimiters,
                            n,
                            multiset,
                            split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse_line_n(std::ifstream& stream,
                                   const std::string& delimiters,
                                   const std::size_t& n,
                                   std::queue<T,Container>& queue,
                                   const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return 0;
      return strtk::parse_n(to_ptr(data), to_ptr(data) + data.size(),
                            delimiters,
                            n,
                            queue,
                            split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse_line_n(std::ifstream& stream,
                                   const std::string& delimiters,
                                   const std::size_t& n,
                                   std::stack<T,Container>& stack,
                                   const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return 0;
      return strtk::parse_n(to_ptr(data), to_ptr(data) + data.size(),
                            delimiters,
                            n,
                            stack,
                            split_option);
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse_line_n(std::ifstream& stream,
                                   const std::string& delimiters,
                                   const std::size_t& n,
                                   std::priority_queue<T,Container,Comparator>& priority_queue,
                                   const split_options::type& split_option = split_options::compress_delimiters)

   {
      if (!stream)
         return 0;
      std::string data;
      data.reserve(strtk::one_kilobyte);
      if (!std::getline(stream,data))
         return 0;
      if (data.empty() || delimiters.empty())
         return 0;
      return strtk::parse_n(to_ptr(data), to_ptr(data) + data.size(),
                            delimiters,
                            n,
                            priority_queue,
                            split_option);
   }
