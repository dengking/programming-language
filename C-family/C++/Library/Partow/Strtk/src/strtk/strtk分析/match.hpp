   template <typename Iterator1, typename Iterator2>
   inline bool match(const Iterator1 pattern_begin,
                     const Iterator1 pattern_end,
                     const Iterator2 data_begin,
                     const Iterator2 data_end,
                     const typename std::iterator_traits<Iterator1>::value_type& zero_or_more,
                     const typename std::iterator_traits<Iterator1>::value_type& zero_or_one)
   {
      /*
         Credits: Adapted from code by Jack Handy (2001)
      */
      if (0 == std::distance(data_begin,data_end)) return false;

      Iterator2 d_itr = data_begin;
      Iterator1 p_itr = pattern_begin;
      Iterator2 c_itr = data_begin;
      Iterator2 m_itr = data_begin;

      while ((data_end != d_itr) && (zero_or_more != (*p_itr)))
      {
         if (((*p_itr) != (*d_itr)) && (zero_or_one != (*p_itr)))
         {
            return false;
         }

         ++p_itr;
         ++d_itr;
      }

      while (data_end != d_itr)
      {
         if (zero_or_more == (*p_itr))
         {
            if (pattern_end == (++p_itr))
            {
               return true;
            }

            m_itr = p_itr;
            c_itr = d_itr;
            ++c_itr;
         }
         else if (((*p_itr) == (*d_itr)) || (zero_or_one == (*p_itr)))
         {
            ++p_itr;
            ++d_itr;
         }
         else
         {
            p_itr = m_itr;
            d_itr = c_itr++;
         }
      }

      while ((p_itr != pattern_end) && (zero_or_more == (*p_itr))) ++p_itr;

      return (p_itr == pattern_end);
   }

   inline bool match(const std::string& wild_card,
                     const std::string& str)
   {
      /*
         * : Zero or more match
         ? : Zero or one match
      */
      return match(to_ptr(wild_card), to_ptr(wild_card) + wild_card.size(),
                   to_ptr(str)      , to_ptr(str)       + str      .size(),
                   '*',
                   '?');
   }

   inline bool imatch_char(const char c1, const char c2)
   {
      return std::toupper(c1) == std::toupper(c2);
   }

   template <typename InputIterator>
   inline bool imatch(const InputIterator begin1, const InputIterator end1,
                      const InputIterator begin2, const InputIterator end2)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      details::convert_type_assert<itr_type>();
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return false;
      }

      InputIterator itr1 = begin1;
      InputIterator itr2 = begin2;

      while (end1 != itr1)
      {
         //if (std::toupper(*itr1, std::locale::classic()) != std::toupper(*it2, std::locale::classic()))
         if (std::toupper(*itr1) != std::toupper(*itr2))
         {
            return false;
         }
         ++itr1;
         ++itr2;
      }

      return true;
   }

   template <typename T>
   inline bool imatch(const range::adapter<T>& r1, const range::adapter<T>& r2)
   {
      return imatch(r1.begin(),r1.end(),r2.begin(),r2.end());
   }

   inline bool imatch(const std::string& s1, const std::string& s2)
   {
      return imatch(to_ptr(s1), to_ptr(s1) + s1.size(),
                    to_ptr(s2), to_ptr(s2) + s2.size());
   }

   template <typename Iterator>
   inline Iterator imatch(const std::string& s, const Iterator begin, const Iterator end)
   {
      for (const std::string* itr = begin; end != itr; ++itr)
      {
         if (imatch(s,*itr))
         {
            return itr;
         }
      }
      return end;
   }

   template <typename Allocator,
             template <typename,typename> class Sequence>
   inline bool imatch(const std::string& s, const Sequence<std::string,Allocator>& sequence)
   {
      return (sequence.end() != imatch(s,sequence.begin(),sequence.end()));
   }

   template <typename Comparator, typename Allocator>
   inline bool imatch(const std::string& s, const std::set<std::string,Comparator,Allocator>& set)
   {
      return imatch(s,set.begin(),set.end());
   }

   template <typename Comparator, typename Allocator>
   inline bool imatch(const std::string& s, const std::multiset<std::string,Comparator,Allocator>& multiset)
   {
      return imatch(s,multiset.begin(),multiset.end());
   }
