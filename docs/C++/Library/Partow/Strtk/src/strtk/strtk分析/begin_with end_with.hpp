   template <typename InputIterator>
   inline bool begins_with(const InputIterator pattern_begin,
                           const InputIterator pattern_end,
                           const InputIterator begin,
                           const InputIterator end)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      if (std::distance(pattern_begin,pattern_end) <= std::distance(begin,end))
      {
         return std::equal(pattern_begin,pattern_end,begin);
      }
      else
         return false;
   }

   inline bool begins_with(const std::string& pattern, const std::string& data)
   {
      if (pattern.size() <= data.size())
      {
         return begins_with(to_ptr(pattern), to_ptr(pattern) + pattern.size(),
                            to_ptr(data)   , to_ptr(data)    + data   .size());
      }
      else
         return false;
   }

   template <typename InputIterator>
   inline bool ibegins_with(const InputIterator pattern_begin,
                            const InputIterator pattern_end,
                            const InputIterator begin,
                            const InputIterator end)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      if (std::distance(pattern_begin,pattern_end) <= std::distance(begin,end))
      {
         return std::equal(pattern_begin,pattern_end,begin,imatch_char);
      }
      else
         return false;
   }

   inline bool ibegins_with(const std::string& pattern, const std::string& data)
   {
      if (pattern.size() <= data.size())
      {
         return ibegins_with(to_ptr(pattern), to_ptr(pattern) + pattern.size(),
                             to_ptr(data),    to_ptr(data)    + data   .size());
      }
      else
         return false;
   }

   template <typename InputIterator>
   inline bool ends_with(const InputIterator pattern_begin,
                         const InputIterator pattern_end,
                         const InputIterator begin,
                         const InputIterator end)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      const std::size_t pattern_length = std::distance(pattern_begin, pattern_end);
      const std::size_t data_length    = std::distance(begin, end);

      if (pattern_length <= data_length)
      {
         return std::equal(pattern_begin,
                           pattern_end,
                           begin + (data_length - pattern_length));
      }
      else
         return false;
   }

   inline bool ends_with(const std::string& pattern, const std::string& data)
   {
      if (pattern.size() <= data.size())
      {
         return ends_with(to_ptr(pattern), to_ptr(pattern) + pattern.size(),
                          to_ptr(data)   , to_ptr(data)    + data   .size());
      }
      else
         return false;
   }

   template <typename InputIterator>
   inline bool iends_with(const InputIterator pattern_begin,
                          const InputIterator pattern_end,
                          const InputIterator begin,
                          const InputIterator end)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;

      details::convert_type_assert<itr_type>();

      const std::size_t pattern_length = std::distance(pattern_begin,pattern_end);
      const std::size_t data_length = std::distance(begin,end);

      if (pattern_length <= data_length)
      {
         return std::equal(pattern_begin,
                           pattern_end,
                           begin + (data_length - pattern_length),
                           imatch_char);
      }
      else
         return false;
   }

   inline bool iends_with(const std::string& pattern, const std::string& data)
   {
      if (pattern.size() <= data.size())
      {
         return iends_with(to_ptr(pattern), to_ptr(pattern) + pattern.size(),
                           to_ptr(data)   , to_ptr(data)    + data   .size());
      }
      else
         return false;
   }
