   template <typename Iterator>
   inline void lexicographically_canonicalize(Iterator begin, Iterator end)
   {
      typedef typename std::iterator_traits<Iterator>::value_type type;
      typedef typename std::pair<Iterator,Iterator> iter_type;
      typedef typename std::list<iter_type> itr_list_type;
      itr_list_type itr_list;

      type smallest = (*std::min_element(begin,end));

      for (Iterator itr = begin; itr != end; ++itr)
      {
         if (*itr == smallest) itr_list.push_back(std::make_pair(itr,itr));
      }

      while (itr_list.size() > 1)
      {
         typename itr_list_type::iterator itr = itr_list.begin();
         while (itr_list.end() != itr)
         {
            ++(*itr).first;
            if (end == (*itr).first)
               itr = itr_list.erase(itr);
            else
               ++itr;
         }

         smallest = *(*itr_list.begin()).first;

         for (itr = (++itr_list.begin()); itr != itr_list.end(); ++itr)
         {
            if (*(*itr).first < smallest)
            {
               smallest = *(*itr).first;
            }
         }

         itr = itr_list.begin();
         while (itr_list.end() != itr)
         {
           if (*(*itr).first != smallest)
              itr = itr_list.erase(itr);
           else
              ++itr;
         }

         itr = itr_list.begin();
         while (itr_list.end() != itr)
         {
            if (end == (*itr).first)
               itr = itr_list.erase(itr);
            else
               ++itr;
         }
      }

      std::rotate(begin,(*itr_list.begin()).second,end);
   }

   inline void lexicographically_canonicalize(std::string& str)
   {
      lexicographically_canonicalize(const_cast<char*>(to_ptr(str)),
                                     const_cast<char*>(to_ptr(str) + str.size()));
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline void lexicographically_canonicalize(Sequence<T,Allocator>& sequence)
   {
      lexicographically_canonicalize(sequence.begin(), sequence.end());
   }
