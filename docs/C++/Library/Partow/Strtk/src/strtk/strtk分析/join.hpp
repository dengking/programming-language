   template <typename InputIterator>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const InputIterator begin,
                    const InputIterator end)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         output += type_to_string(*itr);
         if (end == (++itr))
            break;
         else
            output += delimiter;
      }
   }

   template <typename InputIterator>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const std::pair<InputIterator,InputIterator>& range)
   {
      InputIterator itr = range.first;
      while (range.second != itr)
      {
         output += type_to_string(*itr);
         if (range.second == (++itr))
            break;
         else
            output += delimiter;
      }
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const Sequence<T,Allocator>& sequence)
   {
      join(output,delimiter,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const std::set<T,Comparator,Allocator>& set)
   {
      join(output,delimiter,set.begin(),set.end());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const std::multiset<T,Comparator,Allocator>& multiset)
   {
      join(output,delimiter,multiset.begin(),multiset.end());
   }

   inline void join(std::string& output,
                    const std::string& delimiter,
                    int argc, char* argv[])
   {
      for (int i = 0; i < argc; ++i)
      {
         output += argv[i];
         if (i < (argc - 1))
            output += delimiter;
      }
   }

   template <typename InputIterator>
   inline std::string join(const std::string& delimiter,
                           const InputIterator begin,
                           const InputIterator end)
   {
      std::string output;
      output.reserve(one_kilobyte);
      join(output,delimiter,begin,end);
      return output;
   }

   template <typename InputIterator>
   inline std::string join(const std::string& delimiter,
                           const std::pair<InputIterator,InputIterator>& range)
   {
      std::string output;
      output.reserve(one_kilobyte);
      join(output,delimiter,range.first,range.second);
      return output;
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::string join(const std::string& delimiter,
                           const Sequence<T,Allocator>& sequence)
   {
      if (sequence.empty())
         return "";
      else
         return join(delimiter,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::string join(const std::string& delimiter,
                           const std::set<T,Comparator,Allocator>& set)
   {
      if (set.empty())
         return "";
      else
         return join(delimiter,set.begin(),set.end());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::string join(const std::string& delimiter,
                           const std::multiset<T,Comparator,Allocator>& multiset)
   {
      if (multiset.empty())
         return "";
      else
         return join(delimiter,multiset.begin(),multiset.end());
   }

   inline std::string join(const std::string& delimiter, int argc, char* argv[])
   {
      std::string result;
      result.reserve(one_kilobyte);
      join(result,delimiter,argc,argv);
      return result;
   }

   template <typename InputIterator, typename Predicate>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const InputIterator begin,
                       const InputIterator end)
   {
      InputIterator itr = begin;
      bool first_time   = true;

      while (end != itr)
      {
         if (predicate(*itr))
         {
            if (!first_time)
               output += delimiter;
            else
               first_time = false;

            output += type_to_string(*itr);
         }

         if (end == (++itr))
            break;
      }
   }

   template <typename InputIterator, typename Predicate>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const std::pair<InputIterator,InputIterator>& range)
   {
      InputIterator itr = range.first;
      bool first_time   = true;

      while (range.second != itr)
      {
         if (predicate(*itr))
         {
            if (!first_time)
               output += delimiter;
            else
               first_time = false;

            output += type_to_string(*itr);
         }

         if (range.second == (++itr))
            break;
      }
   }

   template <typename T,
             typename Predicate,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const Sequence<T,Allocator>& sequence)
   {
      join_if(output,delimiter,predicate,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Predicate,
             typename Comparator,
             typename Allocator>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const std::set<T,Comparator,Allocator>& set)
   {
      join_if(output,delimiter,predicate,set.begin(),set.end());
   }

   template <typename T,
             typename Predicate,
             typename Comparator,
             typename Allocator>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const std::multiset<T,Comparator,Allocator>& multiset)
   {
      join_if(output,delimiter,predicate,multiset.begin(),multiset.end());
   }

   template <typename InputIterator, typename Predicate>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const InputIterator begin,
                              const InputIterator end)
   {
      std::string output;
      output.reserve(one_kilobyte);
      join_if(output,delimiter,predicate,begin,end);
      return output;
   }

   template <typename InputIterator, typename Predicate>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const std::pair<InputIterator,InputIterator>& range)
   {
      std::string output;
      output.reserve(one_kilobyte);
      join_if(output,delimiter,predicate,range.first,range.second);
      return output;
   }

   template <typename T,
             typename Predicate,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const Sequence<T,Allocator>& sequence)
   {
      return join(delimiter,predicate,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Predicate,
             typename Comparator,
             typename Allocator>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const std::set<T,Comparator,Allocator>& set)
   {
      return join_if(delimiter,predicate,set.begin(),set.end());
   }

   template <typename T,
             typename Predicate,
             typename Comparator,
             typename Allocator>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const std::multiset<T,Comparator,Allocator>& multiset)
   {
      return join_if(delimiter,predicate,multiset.begin(),multiset.end());
   }
