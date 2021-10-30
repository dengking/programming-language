   template <typename DelimiterPredicate,
             typename Iterator,
             typename OutputIterator>
   inline std::size_t split(const DelimiterPredicate& delimiter,
                            const Iterator begin,
                            const Iterator end,
                            OutputIterator out,
                            const split_options::type split_option = split_options::default_mode)
   {
      if (begin == end) return 0;

      const bool compress_delimiters    = split_options::perform_compress_delimiters(split_option);
      const bool include_1st_delimiter  = split_options::perform_include_1st_delimiter(split_option);
      const bool include_all_delimiters = (!include_1st_delimiter) && split_options::perform_include_all_delimiters(split_option);
      const bool include_delimiters     = include_1st_delimiter || include_all_delimiters;

      if (compress_delimiters && (!include_delimiters))
      {
         return details::split_compress_delimiters(delimiter,begin,end,out);
      }

      std::size_t token_count = 0;
      std::pair<Iterator,Iterator> range(begin,begin);

      while (end != range.second)
      {
         if (delimiter(*range.second))
         {
            if (include_delimiters)
            {
               if (include_1st_delimiter)
                  ++range.second;
               else if (include_all_delimiters)
                  while ((end != range.second) && delimiter(*range.second)) ++range.second;
               (*out) = range;
               ++out;
               if ((!include_all_delimiters) && compress_delimiters)
                  while ((end != range.second) && delimiter(*range.second)) ++range.second;
            }
            else
            {
               (*out) = range;
               ++out;
                  ++range.second;
            }
            ++token_count;
            range.first = range.second;
         }
         else
            ++range.second;
      }

      if ((range.first != range.second) || delimiter(*(range.second - 1)))
      {
         (*out) = range;
         ++out;
         ++token_count;
      }

      return token_count;
   }

   template <typename DelimiterPredicate,
             typename Iterator,
             typename OutputIterator>
   inline std::size_t split(const DelimiterPredicate& delimiter,
                            const std::pair<Iterator,Iterator>& range,
                            OutputIterator out,
                            const split_options::type split_option = split_options::default_mode)
   {
      return split(delimiter,
                   range.first,range.second,
                   out,
                   split_option);
   }

   template <typename DelimiterPredicate,
             typename Iterator,
             typename OutputIterator>
   inline std::size_t split(const char* delimiters,
                            const std::pair<Iterator,Iterator>& range,
                            OutputIterator out,
                            const split_options::type split_option = split_options::default_mode)
   {
      if (1 == details::strnlength(delimiters,256))
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      range.first,range.second,
                      out,
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      range.first,range.second,
                      out,
                      split_option);
   }

   template <typename DelimiterPredicate,
             typename Iterator,
             typename OutputIterator>
   inline std::size_t split(const std::string& delimiters,
                            const std::pair<Iterator,Iterator>& range,
                            OutputIterator out,
                            const split_options::type split_option = split_options::default_mode)
   {
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      range.first,range.second,
                      out,
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      range.first,range.second,
                      out,
                      split_option);
   }

   template <typename OutputIterator>
   inline std::size_t split(const char* delimiters,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      if (1 == details::strnlength(delimiters,256))
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      to_ptr(str), to_ptr(str) + str.size(),
                      out,
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      to_ptr(str), to_ptr(str) + str.size(),
                      out,
                      split_option);
   }

   template <typename OutputIterator>
   inline std::size_t split(const std::string& delimiters,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      to_ptr(str), to_ptr(str) + str.size(),
                      out,
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      to_ptr(str), to_ptr(str) + str.size(),
                      out,
                      split_option);
   }

   template <typename OutputIterator>
   inline std::size_t split(const std::string::value_type delimiter,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      return split(single_delimiter_predicate<std::string::value_type>(delimiter),
                   to_ptr(str), to_ptr(str) + str.size(),
                   out,
                   split_option);
   }

   template <typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t split(const char* delimiters,
                            const std::string& str,
                            Sequence<std::pair<const char*, const char*>,Allocator>& sequence,
                            const split_options::type& split_option = split_options::default_mode)
   {
      if (1 == details::strnlength(delimiters,256))
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      to_ptr(str), to_ptr(str) + str.size(),
                      std::back_inserter(sequence),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      to_ptr(str), to_ptr(str) + str.size(),
                      std::back_inserter(sequence),
                      split_option);
   }

   template <typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t split(const std::string& delimiters,
                            const std::string& str,
                            Sequence<std::pair<const char*, const char*>,Allocator>& sequence,
                            const split_options::type& split_option = split_options::default_mode)
   {
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      to_ptr(str), to_ptr(str) + str.size(),
                      std::back_inserter(sequence),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      to_ptr(str), to_ptr(str) + str.size(),
                      std::back_inserter(sequence),
                      split_option);
   }

   template <typename DelimiterPredicate,
             typename OutputIterator>
   inline std::size_t split(const DelimiterPredicate& delimiter,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      return split(delimiter,
                   to_ptr(str), to_ptr(str) + str.size(),
                   out,
                   split_option);
   }

   template <typename DelimiterPredicate,
             typename Iterator,
             typename OutputIterator>
   inline std::size_t split_n(const DelimiterPredicate& delimiter,
                              const Iterator begin,
                              const Iterator end,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      if (0 == token_count) return 0;

      if (begin == end) return 0;

      std::size_t match_count = 0;
      std::pair<Iterator,Iterator> range(begin,begin);

      const bool compress_delimiters    = split_options::perform_compress_delimiters(split_option);
      const bool include_1st_delimiter  = split_options::perform_include_1st_delimiter(split_option);
      const bool include_all_delimiters = (!include_1st_delimiter) && split_options::perform_include_all_delimiters(split_option);
      const bool include_delimiters     = include_1st_delimiter || include_all_delimiters;

      while (end != range.second)
      {
         if (delimiter(*range.second))
         {
            if (include_delimiters)
            {
               ++range.second;
               (*out) = range;
               ++out;

               if (++match_count >= token_count)
                  return match_count;

               if (compress_delimiters)
                  while ((end != range.second) && delimiter(*range.second)) ++range.second;
            }
            else
            {
               (*out) = range;
               ++out;

               if (++match_count >= token_count)
                  return match_count;

               if (compress_delimiters)
                  while ((end != (++range.second)) && delimiter(*range.second)) ;
               else
                  ++range.second;
            }

            range.first = range.second;
         }
         else
            ++range.second;
      }

      if ((range.first != range.second) || delimiter(*(range.second - 1)))
      {
         (*out) = range;
         ++out;
         ++match_count;
      }

      return match_count;
   }

   template <typename OutputIterator>
   inline std::size_t split_n(const char* delimiters,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      return split_n(multiple_char_delimiter_predicate(delimiters),
                     to_ptr(str), to_ptr(str) + str.size(),
                     token_count,
                     out,
                     split_option);
   }

   template <typename OutputIterator>
   inline std::size_t split_n(const std::string& delimiters,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        to_ptr(str), to_ptr(str) + str.size(),
                        token_count,
                        out,
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        to_ptr(str), to_ptr(str) + str.size(),
                        token_count,
                        out,
                        split_option);
   }

   template <typename InputIterator, typename OutputIterator>
   inline std::size_t split_n(const std::string& delimiters,
                              const InputIterator begin,
                              const InputIterator end,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        begin,end,
                        token_count,
                        out,
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        begin,end,
                        token_count,
                        out,
                        split_option);
   }

   template <typename OutputIterator>
   inline std::size_t split_n(const std::string::value_type delimiter,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      return split_n(single_delimiter_predicate<std::string::value_type>(delimiter),
                     to_ptr(str), to_ptr(str) + str.size(),
                     token_count,
                     out,
                     split_option);
   }

   template <typename DelimiterPredicate,
             typename OutputIterator>
   inline std::size_t split_n(const DelimiterPredicate& delimiter,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      return split_n(delimiter,
                     to_ptr(str), to_ptr(str) + str.size(),
                     token_count,
                     out,
                     split_option);
   }

      #ifdef strtk_enable_regex

   static const std::string uri_expression     ("((https?|ftp)\\://((\\[?(\\d{1,3}\\.){3}\\d{1,3}\\]?)|(([-a-zA-Z0-9]+\\.)+[a-zA-Z]{2,4}))(\\:\\d+)?(/[-a-zA-Z0-9._?,+&amp;%$#=~\\\\]+)*/?)");
   static const std::string email_expression   ("([\\w\\-\\.]+)@((\\[([0-9]{1,3}\\.){3}[0-9]{1,3}\\])|(([\\w\\-]+\\.)+)([a-zA-Z]{2,4}))");
   static const std::string ip_expression      ("(([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+))");
   static const std::string ieee754_expression ("([-+]?((\\.[0-9]+|[0-9]+\\.[0-9]+)([eE][-+][0-9]+)?|[0-9]+))");

   namespace regex_match_mode
   {
      enum type
      {
         match_all = 0,
         match_1   = 1,
         match_2   = 2,
         match_3   = 3,
         match_4   = 4,
         match_5   = 5,
         match_6   = 6,
         match_7   = 7,
         match_8   = 8,
         match_9   = 9
      };
   }

   template <typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex(const boost::regex& delimiter_expression,
                                  const InputIterator begin,
                                  const InputIterator end,
                                  OutputIterator out,
                                  const regex_match_mode::type mode = regex_match_mode::match_all)
   {
      boost::regex_iterator<InputIterator> itr(begin,end,delimiter_expression);
      boost::regex_iterator<InputIterator> itr_end;
      std::pair<InputIterator,InputIterator> range(begin,begin);
      std::size_t match_count = 0;

      while (itr_end != itr)
      {
         range.first = (*itr)[mode].first;
         range.second = (*itr)[mode].second;
         (*out) = range;
         ++out;
         ++itr;
         ++match_count;
      }

      return match_count;
   }

   template <typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex(const std::string& delimiter_expression,
                                  const InputIterator begin,
                                  const InputIterator end,
                                  OutputIterator out,
                                  const regex_match_mode::type mode = regex_match_mode::match_all)
   {
      const boost::regex regex_expression(delimiter_expression);
      return split_regex(regex_expression,
                         begin,end,
                         out,
                         mode);
   }

   template <typename OutputIterator>
   inline std::size_t split_regex(const std::string& delimiter_expression,
                                  const std::string& text,
                                  OutputIterator out,
                                  const regex_match_mode::type mode = regex_match_mode::match_all)
   {
      return split_regex(delimiter_expression,
                         text.begin(),text.end(),
                         out,
                         mode);
   }

   template <typename OutputIterator>
   inline std::size_t split_regex(const boost::regex& delimiter_expression,
                                  const std::string& text,
                                  OutputIterator out,
                                  const regex_match_mode::type mode = regex_match_mode::match_all)
   {
      return split_regex(delimiter_expression,
                         text.begin(),text.end(),
                         out,
                         mode);
   }

   template <typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex_n(const boost::regex& delimiter_expression,
                                    const InputIterator begin,
                                    const InputIterator end,
                                    const std::size_t& token_count,
                                    OutputIterator out,
                                    const regex_match_mode::type mode = regex_match_mode::match_all)
   {
      boost::sregex_iterator itr(begin,end,delimiter_expression);
      const boost::sregex_iterator itr_end;
      std::pair<InputIterator,InputIterator> range(begin,begin);
      std::size_t match_count = 0;

      while (itr_end != itr)
      {
         range.first = (*itr)[mode].first;
         range.second = (*itr)[mode].second;
         (*out) = range;
         ++out;
         ++itr;

         if (++match_count >= token_count)
            return match_count;
      }

      return match_count;
   }

   template <typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex_n(const std::string& delimiter_expression,
                                    const InputIterator begin,
                                    const InputIterator end,
                                    const std::size_t& token_count,
                                    OutputIterator out,
                                    const regex_match_mode::type mode = regex_match_mode::match_all)
   {
      const boost::regex regex_expression(delimiter_expression);
      return split_regex_n(regex_expression,
                           begin,end,
                           token_count,
                           out,
                           mode);
   }

   template <typename OutputIterator>
   inline std::size_t split_regex_n(const std::string& delimiter_expression,
                                    const std::string& text,
                                    const std::size_t& token_count,
                                    OutputIterator out,
                                    const regex_match_mode::type mode = regex_match_mode::match_all)
   {
      return split_regex_n(delimiter_expression,
                           text.begin(),text.end(),
                           token_count,
                           out,
                           mode);
   }

   template <typename OutputIterator>
   inline std::size_t split_regex_n(const boost::regex& delimiter_expression,
                                    const std::string& text,
                                    const std::size_t& token_count,
                                    OutputIterator out,
                                    const regex_match_mode::type mode = regex_match_mode::match_all)
   {
      return split_regex_n(delimiter_expression,
                           text.begin(),text.end(),
                           token_count,
                           out,
                           mode);
   }

      template <const std::size_t offset_list_size>
   class offset_predicate
   {
   public:

      offset_predicate(const int offset_list[], const bool rotate = false)
      : rotate_(rotate),
        current_index_(0)
      {
         std::copy(offset_list, offset_list + offset_list_size, offset_list_);
         offset_list_[offset_list_size] = 0;
      }

      inline bool operator!() const
      {
         return (0 == offset_list_size);
      }

      inline void reset() const
      {
         current_index_ = 0;
      }

      inline std::size_t size() const
      {
         return offset_list_size;
      }

      inline int next() const
      {
         int result = offset_list_[current_index_++];

         if (rotate_ && (current_index_ >= offset_list_size))
         {
            current_index_ = 0;
         }

         return result;
      }

   private:

      bool rotate_;
      mutable std::size_t current_index_;
      int offset_list_[offset_list_size + 1];
   };

   inline offset_predicate<12> offsets(const int&  v1, const int&  v2, const int&  v3,
                                       const int&  v4, const int&  v5, const int&  v6,
                                       const int&  v7, const int&  v8, const int&  v9,
                                       const int& v10, const int& v11, const int& v12,
                                       const bool& rotate = false)
   {
      const int offset_list[12] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12 };
      return offset_predicate<12>(offset_list,rotate);
   }

   inline offset_predicate<11> offsets(const int&  v1, const int&  v2, const int& v3,
                                       const int&  v4, const int&  v5, const int& v6,
                                       const int&  v7, const int&  v8, const int& v9,
                                       const int& v10, const int& v11,
                                       const bool& rotate = false)
   {
      const int offset_list[11] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 };
      return offset_predicate<11>(offset_list,rotate);
   }

   inline offset_predicate<10> offsets(const int& v1, const int& v2, const int& v3,
                                       const int& v4, const int& v5, const int& v6,
                                       const int& v7, const int& v8, const int& v9,
                                       const int& v10, const bool& rotate = false)
   {
      const int offset_list[10] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 };
      return offset_predicate<10>(offset_list,rotate);
   }

   inline offset_predicate<9> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const int& v7, const int& v8, const int& v9,
                                      const bool& rotate = false)
   {
      const int offset_list[9] = { v1, v2, v3, v4, v5, v6, v7, v8, v9 };
      return offset_predicate<9>(offset_list,rotate);
   }

   inline offset_predicate<8> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const int& v7, const int& v8, const bool& rotate = false)
   {
      const int offset_list[8] = { v1, v2, v3, v4, v5, v6, v7, v8 };
      return offset_predicate<8>(offset_list,rotate);
   }

   inline offset_predicate<7> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const int& v7, const bool& rotate = false)
   {
      const int offset_list[7] = { v1, v2, v3, v4, v5, v6, v7 };
      return offset_predicate<7>(offset_list,rotate);
   }

   inline offset_predicate<6> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const bool& rotate = false)
   {
      const int offset_list[6] = { v1, v2, v3, v4, v5, v6 };
      return offset_predicate<6>(offset_list,rotate);
   }

   inline offset_predicate<5> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const bool& rotate = false)
   {
      const int offset_list[5] = { v1, v2, v3, v4, v5 };
      return offset_predicate<5>(offset_list,rotate);
   }

   inline offset_predicate<4> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const bool& rotate = false)
   {
      const int offset_list[4] = { v1, v2, v3, v4 };
      return offset_predicate<4>(offset_list,rotate);
   }

   inline offset_predicate<3> offsets(const int& v1, const int& v2, const int& v3,
                                      const bool& rotate = false)
   {
      const int offset_list[3] = { v1, v2, v3 };
      return offset_predicate<3>(offset_list,rotate);
   }

   inline offset_predicate<2> offsets(const int& v1, const int& v2, const bool& rotate = false)
   {
      const int offset_list[2] = { v1, v2 };
      return offset_predicate<2>(offset_list,rotate);
   }

   inline offset_predicate<1> offsets(const int& v1,
                                      const bool& rotate = false)
   {
      const int offset_list[1] = { v1 };
      return offset_predicate<1>(offset_list,rotate);
   }

   template <typename OffsetPredicate,
             typename InputIterator,
             typename OutputIterator>
   inline std::size_t offset_splitter(const InputIterator begin,
                                      const InputIterator end,
                                      const OffsetPredicate& offset,
                                      OutputIterator out)
   {
      std::size_t length = 0;

      if (0 == (length = std::distance(begin,end))) return 0;

      std::pair<InputIterator,InputIterator> range(begin,begin);
      std::size_t match_count = 0;
      int offset_length = 0;
      std::size_t increment_amount = 0;

      while ((end != range.second) && (0 < (offset_length = offset.next())))
      {
         increment_amount = std::min<std::size_t>(length,offset_length);
         range.first = range.second;
         range.second += increment_amount;
         length -= increment_amount;
         (*out) = range;
         ++out;
         ++match_count;
      }

      return match_count;
   }

   template <typename OffsetPredicate,
             typename OutputIterator>
   inline std::size_t offset_splitter(const std::string& str,
                                      const OffsetPredicate& offset,
                                      OutputIterator out)
   {
      return offset_splitter(to_ptr(str), to_ptr(str) + str.size(), offset, out);
   }

   template <typename InputIterator,
             typename Predicate,
             typename OutputPair>
   inline bool split_pair(const InputIterator begin,
                          const InputIterator end,
                          const Predicate& delimiter,
                          OutputPair& v1,
                          OutputPair& v2)
   {
      if (0 == std::distance(begin,end)) return false;

      InputIterator itr = begin;

      while (end != itr)
      {
         if (delimiter(*itr))
         {
            v1 = std::make_pair(begin,itr);
            ++itr;
            if (0 != std::distance(itr,end))
            {
               v2 = std::make_pair(itr,end);
               return true;
            }
            else
               return false;
         }
         else
            ++itr;
      }

      return false;
   }

   inline bool split_pair(const std::string::value_type delimiter,
                          const std::string& str,
                          std::pair<const char*,const char*>& v1,
                          std::pair<const char*,const char*>& v2)
   {
      return split_pair(to_ptr(str), to_ptr(str) + str.size(),
                        single_delimiter_predicate<std::string::value_type>(delimiter),
                        v1,
                        v2);
   }

   template <typename DelimiterPredicate>
   inline bool split_pair(const DelimiterPredicate& delimiter,
                          const std::string& str,
                          std::pair<const char*,const char*>& v1,
                          std::pair<const char*,const char*>& v2)
   {
      return split_pair(to_ptr(str), to_ptr(str) + str.size(),
                        delimiter,
                        v1,
                        v2);
   }

   template <typename Function>
   inline std::size_t for_each_token(const std::string& buffer,
                                     const std::string& delimiters,
                                     Function function)
   {
      return split(delimiters,
                   buffer,
                   strtk::functional_inserter<Function>(function));
   }

   template <typename Function>
   inline std::size_t for_each_token(const std::string& buffer,
                                     const char* delimiters,
                                     Function function)
   {
      return split(delimiters,
                   buffer,
                   strtk::functional_inserter<Function>(function));
   }
