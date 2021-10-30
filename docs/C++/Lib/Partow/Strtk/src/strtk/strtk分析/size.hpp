   template <std::size_t length>
   struct size_equal_to
   {
      template <typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end) const
      {
         return length == std::distance(begin,end);
      }

      template <typename Iterator>
      inline bool operator()(const std::pair<Iterator,Iterator>& range) const
      {
         return length == std::distance(range.first,range.second);
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence) const
      {
         return length == sequence.size();
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::set<T,Comparator,Allocator>& set) const
      {
         return length == set.size();
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::multiset<T,Comparator,Allocator>& multiset) const
      {
         return length == multiset.size();
      }

      inline bool operator()(const std::string& str) const
      {
         return length == str.size();
      }
   };

   template <std::size_t length>
   struct size_less_than
   {
      template <typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end) const
      {
         return std::distance(begin,end) < static_cast<typename std::iterator_traits<Iterator>::difference_type>(length);
      }

      template <typename Iterator>
      inline bool operator()(const std::pair<Iterator,Iterator>& range) const
      {
         return std::distance(range.first,range.second) < static_cast<typename std::iterator_traits<Iterator>::difference_type>(length);
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence) const
      {
         return sequence.size() < length;
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::set<T,Comparator,Allocator>& set) const
      {
         return set.size() < length;
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::multiset<T,Comparator,Allocator>& multiset) const
      {
         return multiset.size() < length;
      }

      inline bool operator()(const std::string& str) const
      {
         return str.size() < length;
      }
   };

   template <std::size_t length>
   struct size_greater_than
   {
      template <typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end) const
      {
         return std::distance(begin,end) > static_cast<typename std::iterator_traits<Iterator>::difference_type>(length);
      }

      template <typename Iterator>
      inline bool operator()(const std::pair<Iterator,Iterator>& range) const
      {
         return std::distance(range.first,range.second) > static_cast<typename std::iterator_traits<Iterator>::difference_type>(length);
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence) const
      {
         return sequence.size() > length;
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::set<T,Comparator,Allocator>& set) const
      {
         return set.size() > length;
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::multiset<T,Comparator,Allocator>& multiset) const
      {
         return multiset.size() > length;
      }

      inline bool operator()(const std::string& str) const
      {
         return str.size() > length;
      }
   };

   struct size_is_even
   {
      template <typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end) const
      {
         return 0 == (std::distance(begin,end) % 2);
      }

      template <typename Iterator>
      inline bool operator()(const std::pair<Iterator,Iterator>& range) const
      {
         return 0 == (std::distance(range.first,range.second) % 2);
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence) const
      {
         return 0 == (sequence.size() % 2);
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::set<T,Comparator,Allocator>& set) const
      {
         return 0 == (set.size() % 2);
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::multiset<T,Comparator,Allocator>& multiset) const
      {
         return 0 == (multiset.size() % 2);
      }

      inline bool operator()(const std::string& str) const
      {
         return 0 == (str.size() % 2);
      }
   };

   struct size_is_odd
   {
      template <typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end) const
      {
         return 0 != (std::distance(begin,end) % 2);
      }

      template <typename Iterator>
      inline bool operator()(const std::pair<Iterator,Iterator>& range) const
      {
         return 0 != (std::distance(range.first,range.second) % 2);
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence) const
      {
         return 0 != (sequence.size() % 2);
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::set<T,Comparator,Allocator>& set) const
      {
         return 0 != (set.size() % 2);
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::multiset<T,Comparator,Allocator>& multiset) const
      {
         return 0 != (multiset.size() % 2);
      }

      inline bool operator()(const std::string& str) const
      {
         return 0 != (str.size() % 2);
      }
   };
