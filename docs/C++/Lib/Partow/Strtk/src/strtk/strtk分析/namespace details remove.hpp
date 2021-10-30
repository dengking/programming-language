   namespace details
   {
      template <typename Allocator,
                template <typename,typename> class Sequence>
      struct index_remover_impl
      {
         typedef Sequence<std::size_t,Allocator> sequence_t;
         index_remover_impl(const sequence_t& sequence)
         : itr_(sequence.begin()),
           end_(sequence.end()),
           current_index_(0),
           check_(true)
         {}

         template <typename T>
         inline bool operator()(const T&)
         {
            if (check_)
            {
               if (current_index_++ == *itr_)
               {
                  if (end_ == ++itr_)
                  {
                     check_ = false;
                  }

                  return true;
               }
            }

            return false;
         }

         typename sequence_t::const_iterator itr_;
         typename sequence_t::const_iterator end_;
         std::size_t current_index_;
         bool check_;
      };
   }

   template <typename Allocator,
             template <typename,typename> class Sequence>
   inline details::index_remover_impl<Allocator,Sequence> index_remover(const Sequence<std::size_t,Allocator>& sequence)
   {
      return details::index_remover_impl<Allocator,Sequence>(sequence);
   }

   template <typename Iterator, typename Predicate>
   inline std::size_t remove_inplace(Predicate predicate,
                                     Iterator begin,
                                     Iterator end)
   {
      Iterator itr1 = begin;
      Iterator itr2 = begin;
      std::size_t removal_count = 0;
      while (end != itr1)
      {
         if (predicate(*itr1))
         {
            ++itr1;
            ++removal_count;
         }
         else
         {
            if (itr1 != itr2)
            {
               (*itr2) = (*itr1);
            }
            ++itr1;
            ++itr2;
         }
      }

      return removal_count;
   }

   template <typename T, typename Predicate>
   inline std::size_t remove_inplace(Predicate predicate, const range::adapter<T>& r)
   {
      return remove_inplace(predicate,r.begin(),r.end());
   }

   template <typename Predicate,
             typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t remove_inplace(Predicate predicate, Sequence<T,Allocator>& sequence)
   {
      const std::size_t removal_count = remove_inplace(predicate,sequence.begin(),sequence.end());
      sequence.resize(sequence.size() - removal_count);
      return removal_count;
   }

   inline void remove_inplace(const std::string::value_type c, std::string& s)
   {
      const std::size_t removal_count = remove_inplace(single_delimiter_predicate<std::string::value_type>(c),
                                                       const_cast<char*>(to_ptr(s)),
                                                       const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template <typename Predicate>
   inline void remove_inplace(Predicate predicate, std::string& s)
   {
      const std::size_t removal_count = remove_inplace(predicate,
                                                       const_cast<char*>(to_ptr(s)),
                                                       const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template <typename Iterator, typename Predicate>
   inline std::size_t remove_consecutives_inplace(Predicate predicate,
                                                  Iterator begin,
                                                  Iterator end)
   {
      if (0 == std::distance(begin,end)) return 0;
      Iterator itr1 = begin;
      Iterator itr2 = begin;
      typename std::iterator_traits<Iterator>::value_type prev = *begin;
      std::size_t removal_count = 0;
      ++itr1;
      ++itr2;
      while (end != itr1)
      {
         while ((end != itr1) && (!predicate(*itr1) || !predicate(prev)))
         {
            if (itr1 != itr2)
            {
               (*itr2) = (*itr1);
            }
            prev = (*itr1);
            ++itr1;
            ++itr2;
         }

         while ((end != itr1) && predicate(*itr1))
         {
            ++itr1;
            ++removal_count;
         }
      }

      return removal_count;
   }

   template <typename T, typename Predicate>
   inline std::size_t remove_consecutives_inplace(Predicate predicate, const range::adapter<T>& r)
   {
      return remove_consecutives_inplace(predicate,r.begin(),r.end());
   }

   inline void remove_consecutives_inplace(const std::string::value_type c, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_consecutives_inplace(single_delimiter_predicate<std::string::value_type>(c),
                                                                    const_cast<char*>(to_ptr(s)),
                                                                    const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_consecutives_inplace(const std::string& rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_consecutives_inplace(multiple_char_delimiter_predicate(rem_chars),
                                                                    const_cast<char*>(to_ptr(s)),
                                                                    const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   namespace details
   {
      #if (defined(__MINGW32_VERSION)) ||                                                   \
          (defined(__CYGWIN__) || defined(__CYGWIN32__)) ||                                 \
          (defined(__APPLE__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1070)) || \
          (defined(_WIN32) && (_MSC_VER < 1400))
         inline std::size_t strnlength(const char* s, const std::size_t& n)
         {
            const char *end = reinterpret_cast<const char*>(memchr(s, '\0', n));
            return end ? (size_t) (end - s) : n;
         }
      #else
         inline std::size_t strnlength(const char* s, const std::size_t& n)
         {
            return strnlen(s,n);
         }
      #endif
   }

   inline void remove_consecutives_inplace(const char* rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_consecutives_inplace(multiple_char_delimiter_predicate(
                                                                    rem_chars,
                                                                    rem_chars + details::strnlength(rem_chars,256)),
                                                                    const_cast<char*>(to_ptr(s)),
                                                                    const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template <typename Predicate>
   inline void remove_consecutives_inplace(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_consecutives_inplace(predicate,
                                                                    const_cast<char*>(to_ptr(s)),
                                                                    const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template <typename Iterator>
   inline std::size_t remove_consecutives_inplace(Iterator begin, Iterator end)
   {
      if (0 == std::distance(begin,end)) return 0;

      Iterator itr1 = begin; ++itr1;
      Iterator itr2 = begin; ++itr2;
      typename std::iterator_traits<Iterator>::value_type prev = *begin;
      std::size_t removal_count = 0;

      while (end != itr1)
      {
         while ((end != itr1) && (prev != (*itr1)))
         {
            if (itr1 != itr2)
            {
               (*itr2) = (*itr1);
            }
            prev = (*itr1);
            ++itr1;
            ++itr2;
         }

         while ((end != itr1) && (prev == (*itr1)))
         {
            ++itr1;
            ++removal_count;
         }
      }

      return removal_count;
   }

   template <typename T>
   inline std::size_t remove_consecutives_inplace(const range::adapter<T>& r)
   {
      return remove_consecutives_inplace(r.begin(),r.end());
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline void remove_consecutives_inplace(Sequence<T,Allocator>& sequence)
   {
      const std::size_t removal_count = remove_consecutives_inplace(sequence.begin(),sequence.end());
      sequence.resize(sequence.size() - removal_count);
   }

   inline void remove_consecutives_inplace(std::string& s)
   {
      std::size_t removal_count = remove_consecutives_inplace(const_cast<char*>(to_ptr(s)),
                                                              const_cast<char*>(to_ptr(s) + s.size()));

      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline std::string remove_duplicates(const std::string& str)
   {
      std::string::value_type table[0xFF];
      std::fill_n(table,0xFF,static_cast<char>(0));
      std::string result;
      result.reserve(str.size());
      for (std::size_t i = 0; i < str.size(); ++i)
      {
         const char c = str[i];
         if (0 == table[static_cast<std::size_t>(c)])
         {
            table[static_cast<std::size_t>(c)] = 0x01;
            result += c;
         }
      }
      return result;
   }

   inline std::string remove_duplicates_inplace(std::string& str)
   {
      return remove_duplicates(str);
   }

   template <typename Iterator, typename Predicate>
   inline std::size_t remove_trailing(Predicate predicate,
                                      Iterator begin,
                                      Iterator end)
   {
      const std::size_t length = std::distance(begin,end);
      if (0 == length)
         return 0;

      Iterator itr = begin + (length - 1);
      std::size_t removal_count = 0;

      while ((begin != itr) && predicate(*itr))
      {
         --itr;
         ++removal_count;
      }

      return removal_count;
   }

   template <typename T, typename Predicate>
   inline std::size_t remove_trailing(Predicate predicate, const range::adapter<T>& r)
   {
      return remove_trailing(predicate,r.begin(),r.end());
   }

   inline void remove_trailing(const std::string::value_type c, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_trailing(single_delimiter_predicate<std::string::value_type>(c),
                                                        const_cast<char*>(to_ptr(s)),
                                                        const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_trailing(const std::string& rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_trailing(multiple_char_delimiter_predicate(rem_chars),
                                                        const_cast<char*>(to_ptr(s)),
                                                        const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_trailing(const char* rem_chars, std::string& s)
   {
      const std::size_t removal_count = remove_trailing(multiple_char_delimiter_predicate(
                                                        rem_chars,
                                                        rem_chars + details::strnlength(rem_chars,256)),
                                                        const_cast<char*>(to_ptr(s)),
                                                        const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template <typename Predicate>
   inline void remove_trailing(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_trailing(predicate,
                                                        const_cast<char*>(to_ptr(s)),
                                                        const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template <typename Iterator, typename Predicate>
   inline std::size_t remove_leading(Predicate predicate,
                                     Iterator begin,
                                     Iterator end)
   {
      const std::size_t length = std::distance(begin,end);
      if (0 == length)
         return 0;

      Iterator itr = begin;
      std::size_t removal_count = 0;

      while ((end != itr) && predicate(*itr))
      {
         ++itr;
         ++removal_count;
      }

      std::copy(itr,end,begin);
      return removal_count;
   }

   template <typename T, typename Predicate>
   inline std::size_t remove_leading(Predicate predicate, const range::adapter<T>& r)
   {
      return remove_leading(predicate,r.begin(),r.end());
   }

   inline void remove_leading(const std::string::value_type c, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_leading(single_delimiter_predicate<std::string::value_type>(c),
                                                       const_cast<char*>(to_ptr(s)),
                                                       const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_leading(const std::string& rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_leading(multiple_char_delimiter_predicate(rem_chars),
                                                       const_cast<char*>(to_ptr(s)),
                                                       const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_leading(const char* rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_leading(multiple_char_delimiter_predicate(
                                                       rem_chars,
                                                       rem_chars + details::strnlength(rem_chars,256)),
                                                       const_cast<char*>(to_ptr(s)),
                                                       const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_leading_trailing(const std::string& rem_chars, std::string& s)
   {
      remove_leading(rem_chars,s);
      remove_trailing(rem_chars,s);
   }

   template <typename Predicate>
   inline void remove_leading(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_leading(predicate,
                                                       const_cast<char*>(to_ptr(s)),
                                                       const_cast<char*>(to_ptr(s) + s.size()));
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template <typename Allocator,
             template <typename,typename> class Sequence>
   void remove_empty_strings(Sequence<std::string,Allocator>& seq)
   {
      struct is_empty { static inline bool check(const std::string& s) { return s.empty(); } };
      seq.erase(std::remove_if(seq.begin(),seq.end(),is_empty::check),seq.end());
   }

   template <typename Allocator>
   void remove_empty_strings(std::list<std::string,Allocator>& l)
   {
      struct is_empty { static inline bool check(const std::string& s) { return s.empty(); } };
      l.remove_if(is_empty::check);
   }

   template <typename Comparator, typename Allocator>
   void remove_empty_strings(std::set<std::string,Comparator,Allocator>& set)
   {
      struct is_empty { static inline bool check(const std::string& s) { return s.empty(); } };

      typename std::set<std::string,Comparator,Allocator>::iterator itr = set.begin();

      while (set.end() != itr)
      {
         if ((*itr).empty())
            set.erase(itr++);
         else
            ++itr;
      }
   }

   template <typename Comparator, typename Allocator>
   void remove_empty_strings(std::multiset<std::string,Comparator,Allocator>& set)
   {
      struct is_empty { static inline bool check(const std::string& s) { return s.empty(); } };

      typename std::multiset<std::string,Comparator,Allocator>::iterator itr = set.begin();

      while (set.end() != itr)
      {
         if ((*itr).empty())
            set.erase(itr++);
         else
            ++itr;
      }
   }
s