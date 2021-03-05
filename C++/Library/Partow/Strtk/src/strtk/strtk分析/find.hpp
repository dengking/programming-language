   template <typename Iterator, typename OutputIterator>
   inline std::size_t find_all(const Iterator pattern_begin,
                               const Iterator pattern_end,
                               const Iterator begin,
                               const Iterator end,
                               OutputIterator out)
   {
      Iterator itr = begin;
      const std::size_t pattern_length = std::distance(pattern_begin,pattern_end);
      std::size_t match_count = 0;
      while (end != (itr = std::search(itr, end, pattern_begin, pattern_end)))
      {
         (*out) = std::make_pair(itr,itr + pattern_length);
         itr += pattern_length;
         ++out;
         ++match_count;
      }
      return match_count;
   }

   template <typename Iterator,
             typename Range,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t find_all(const Iterator pattern_begin,
                               const Iterator pattern_end,
                               const Iterator begin,
                               const Iterator end,
                               Sequence<Range,Allocator>& seq)
   {
      return find_all(pattern_begin,pattern_end,begin,end,std::back_inserter(seq));
   }

   inline std::size_t ifind(const std::string& pattern, const std::string& data)
   {
      if (pattern.size() > data.size())
         return std::string::npos;

      const char* result_itr = std::search(to_ptr(data),    to_ptr(data)    + data   .size(),
                                           to_ptr(pattern), to_ptr(pattern) + pattern.size(),
                                           imatch_char);

      if ((to_ptr(data) + data.size()) == result_itr)
         return std::string::npos;
      else
         return std::distance(to_ptr(data),result_itr);
   }

   template <typename Iterator, typename OutputIterator>
   inline std::size_t ifind_all(const Iterator pattern_begin,
                                const Iterator pattern_end,
                                const Iterator begin,
                                const Iterator end,
                                OutputIterator out)
   {
      Iterator itr = begin;
      const std::size_t pattern_length = std::distance(pattern_begin,pattern_end);
      std::size_t match_count = 0;

      while (end != (itr = std::search(itr, end, pattern_begin, pattern_end, imatch_char)))
      {
         (*out) = std::make_pair(itr, itr + pattern_length);
         itr += pattern_length;
         ++out;
         ++match_count;
      }

      return match_count;
   }

   template <typename OutputIterator>
   inline std::size_t find_all(const std::string& pattern,
                               const std::string& data,
                               OutputIterator out)
   {
      return find_all(to_ptr(pattern), to_ptr(pattern) + pattern.size(),
                      to_ptr(data)   , to_ptr(data)    + data   .size(),
                      out);
   }

   template <typename Range,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t find_all(const std::string& pattern,
                               const std::string& data,
                               Sequence<Range,Allocator>& seq)
   {
      return find_all(pattern,data,std::back_inserter(seq));
   }

   template <typename OutputIterator>
   inline std::size_t ifind_all(const std::string& pattern,
                                const std::string& data,
                                OutputIterator out)
   {
      return ifind_all(to_ptr(pattern), to_ptr(pattern) + pattern.size(),
                       to_ptr(data)   , to_ptr(data)    + data   .size(),
                       out);
   }

   template <typename Range,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t ifind_all(const std::string& pattern,
                                const std::string& data,
                                Sequence<Range,Allocator>& seq)
   {
      return ifind_all(pattern,data,std::back_inserter(seq));
   }
