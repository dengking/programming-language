   struct empty_range
   {
   public:

      template <typename InputIterator>
      inline bool operator()(const InputIterator begin, const InputIterator end)
      {
         return (0 == std::distance(begin,end));
      }
   };

   struct nonempty_range
   {
   public:

      template <typename InputIterator>
      inline bool operator()(const InputIterator begin, const InputIterator end)
      {
         return (0 != std::distance(begin,end));
      }
   };


   template <typename OutputIterator>
   struct filter_non_empty_range
   {
   public:

      filter_non_empty_range(OutputIterator out)
      : out_(out)
      {}

      template <typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range)
      {
         if (range.first != range.second)
         {
            *out_++ = range;
         }
      }

   private:

      OutputIterator out_;
   };

   template <typename OutputPredicate>
   struct filter_on_wildcard_match
   {
   public:

      filter_on_wildcard_match(const std::string& match_pattern,
                               OutputPredicate& predicate,
                               bool allow_through_on_match = true)
      : allow_through_on_match_(allow_through_on_match),
        match_pattern_(match_pattern),
        predicate_(predicate)
      {}

      template <typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range) const
      {
         if (match(match_pattern_.begin(),match_pattern_.end(),range.first,range.second,'*','?') ^ allow_through_on_match_)
         {
            predicate_(range);
         }
      }

      inline void operator() (const std::string& s) const
      {
         if (match(match_pattern_,s) ^ allow_through_on_match_)
         {
            predicate_(s);
         }
      }

   private:

      filter_on_wildcard_match(const filter_on_wildcard_match& fom);
      filter_on_wildcard_match& operator=(const filter_on_wildcard_match& fom);

      bool allow_through_on_match_;
      std::string match_pattern_;
      OutputPredicate& predicate_;
   };

  template <typename OutputPredicate>
   struct filter_on_match
   {
   public:

     filter_on_match(const std::string* begin, const std::string* end,
                     OutputPredicate predicate,
                     bool case_insensitive,
                     bool allow_through_on_match = true)
     : case_insensitive_(case_insensitive),
       allow_through_on_match_(allow_through_on_match),
       begin_(begin),
       end_(end),
       predicate_(predicate)
     {}

      template <typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range) const
      {
         for (const std::string* itr = begin_; end_ != itr; ++itr)
         {
            if (
                 (case_insensitive_ &&
                 (imatch(to_ptr((*itr)), to_ptr((*itr)) + (*itr).size(), range.first,range.second))) ||
                 (!case_insensitive_ && std::equal((*itr).begin(), (*itr).end(), range.first))
               )
            {
               if (allow_through_on_match_)
               {
                  predicate_(range);
               }

               return;
            }
         }

         if (!allow_through_on_match_)
         {
            predicate_(range);
         }
      }

      inline void operator() (const std::string& s) const
      {
         for (const std::string* itr = begin_; end_ != itr; ++itr)
         {
            if (
                 (case_insensitive_ &&
                 (imatch((*itr).begin(),(*itr).end(),s.begin(),s.end()))) ||
                 (!case_insensitive_ && std::equal((*itr).begin(),(*itr).end(),s.begin()))
               )
            {
               if (allow_through_on_match_)
               {
                  predicate_(s);
                  return;
               }
            }
         }

         if (!allow_through_on_match_)
         {
            predicate_(s);
            return;
         }
      }

   private:

      filter_on_match& operator=(const filter_on_match& fom);

   private:

      bool case_insensitive_;
      bool allow_through_on_match_;
      const std::string* begin_;
      const std::string* end_;
      OutputPredicate predicate_;
   };

     template <typename Iterator, typename MatchPredicate>
   inline void skip_while_matching(Iterator& itr,
                                   const Iterator& end,
                                   const MatchPredicate& predicate)
   {
      while (end != itr)
      {
         if (predicate(*itr))
            ++itr;
         else
            break;
      }
   }
