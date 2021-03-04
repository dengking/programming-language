   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline T min_of_cont(const Sequence<T,Allocator>& sequence)
   {
      return (*std::min_element(sequence.begin(),sequence.end()));
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline T min_of_cont(const std::set<T,Comparator,Allocator>& set)
   {
      return (*set.begin());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline T min_of_cont(const std::multiset<T,Comparator,Allocator>& multiset)
   {
      return (*multiset.begin());
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline T max_of_cont(const Sequence<T,Allocator>& sequence)
   {
      return (*std::max_element(sequence.begin(),sequence.end()));
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline T max_of_cont(const std::set<T,Comparator,Allocator>& set)
   {
      return (*set.rbegin());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline T max_of_cont(const std::multiset<T,Comparator,Allocator>& multiset)
   {
      return (*multiset.rbegin());
   }

   template <typename InputIterator>
   inline void min_max_of_range(const InputIterator begin, const InputIterator end,
                                typename std::iterator_traits<InputIterator>::value_type& min_value,
                                typename std::iterator_traits<InputIterator>::value_type& max_value)
   {
      min_value = *begin;
      max_value = *begin;
      InputIterator itr = begin;
      while (end != ++itr)
      {
         if (*itr < min_value)
            min_value = (*itr);
         else if (*itr > max_value)
            max_value = (*itr);
      }
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline void min_max_of_cont(const Sequence<T,Allocator>& sequence,
                               T& min_value,
                               T& max_value)
   {
      min_max_of_range(sequence.begin(),sequence.end(),
                       min_value,
                       max_value);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline void min_max_of_cont(const std::set<T,Comparator,Allocator>& set,
                               T& min_value,
                               T& max_value)
   {
      min_value = (*set.begin());
      max_value = (*set.rbegin());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline void min_max_of_cont(const std::multiset<T,Comparator,Allocator>& multiset,
                               T& min_value,
                               T& max_value)
   {
      min_value = (*multiset.begin());
      max_value = (*multiset.rbegin());
   }
