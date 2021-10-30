   template <typename Iterator, typename DelimiterPredicate>
   class tokenizer
   {
   private:

      template <typename Iterartor,
                typename Predicate,
                typename T = std::pair<Iterator,Iterator> >
      class tokenizer_iterator : public std::iterator<std::forward_iterator_tag,T>
      {
      protected:

         typedef Iterator iterator;
         typedef const iterator const_iterator;
         typedef typename std::pair<iterator,iterator> range_type;

      public:

         explicit inline tokenizer_iterator(const iterator begin,
                                            const iterator end,
                                            const Predicate& predicate,
                                            const tokenize_options::type tokenize_option = tokenize_options::default_mode)
         : predicate_(predicate),
           end_(end),
           range_(begin,begin),
           current_token_(end,end),
           compress_delimiters_(tokenize_options::perform_compress_delimiters(tokenize_option)),
           include_1st_delimiter_(tokenize_options::perform_include_1st_delimiter(tokenize_option)),
           include_all_delimiters_(tokenize_options::perform_include_all_delimiters(tokenize_option)),
           include_delimiters_(include_1st_delimiter_ || include_all_delimiters_),
           last_token_done_(false)
         {
            if (end != begin)
            {
               this->operator++();
            }
         }

         inline tokenizer_iterator& operator++()
         {
            if (last_token_done_)
            {
               range_.first = range_.second;
               return (*this);
            }
            else if (end_ != range_.second)
            {
               range_.first = range_.second;
            }

            while (end_ != range_.second)
            {
              if (predicate_(*(range_.second)))
              {
                 if (include_delimiters_)
                 {
                    if (include_1st_delimiter_)
                       ++range_.second;
                    else if (include_all_delimiters_)
                       while ((end_ != range_.second) && predicate_(*(range_.second))) ++range_.second;
                    current_token_ = range_;
                    if ((!include_all_delimiters_) && compress_delimiters_)
                       while ((end_ != range_.second) && predicate_(*(range_.second))) ++range_.second;
                 }
                 else
                 {
                    current_token_ = range_;
                    if (compress_delimiters_)
                       while ((end_ != (++range_.second)) && predicate_(*(range_.second))) ;
                    else
                       ++range_.second;
                 }

                 return (*this);
              }
              else
                 ++range_.second;
            }

            if (range_.first != range_.second)
            {
               current_token_.second = range_.second;
               if (!last_token_done_)
               {
                  if (predicate_(*(range_.second - 1)))
                     current_token_.first = range_.second;
                  else
                     current_token_.first = range_.first;
                  last_token_done_ = true;
               }
               else
                  range_.first = range_.second;
            }

            return (*this);
         }

         inline tokenizer_iterator operator++(int)
         {
            tokenizer_iterator tmp = (*this);
            this->operator++();
            return tmp;
         }

         inline tokenizer_iterator& operator+=(const int inc)
         {
            if (inc > 0)
            {
               for (int i = 0; i < inc; ++i, ++(*this)) ;
            }

            return (*this);
         }

         inline T operator*() const
         {
            return current_token_;
         }

         inline std::string as_string() const
         {
            return std::string(current_token_.first,current_token_.second);
         }

         inline bool operator==(const tokenizer_iterator& itr) const
         {
            return (range_ == itr.range_) && (end_ == itr.end_);
         }

         inline bool operator!=(const tokenizer_iterator& itr) const
         {
            return (range_ != itr.range_) || (end_ != itr.end_);
         }

         inline tokenizer_iterator& operator=(const tokenizer_iterator& itr)
         {
            if (this != &itr)
            {
               range_                  = itr.range_;
               current_token_          = itr.current_token_;
               end_                    = itr.end_;
               compress_delimiters_    = itr.compress_delimiters_;
               include_1st_delimiter_  = itr.include_1st_delimiter_;
               include_all_delimiters_ = itr.include_all_delimiters_;
               include_delimiters_     = itr.include_delimiters_;
               last_token_done_        = itr.last_token_done_;
            }

            return (*this);
         }

         inline std::string remaining() const
         {
            return std::string(current_token_.first,end_);
         }

      protected:

         const Predicate& predicate_;
         iterator end_;
         range_type range_;
         range_type current_token_;
         bool compress_delimiters_;
         bool include_1st_delimiter_;
         bool include_all_delimiters_;
         bool include_delimiters_;
         bool last_token_done_;
      };

   public:

      typedef typename std::iterator_traits<Iterator>::value_type value_type;
      typedef DelimiterPredicate predicate;
      typedef tokenizer_iterator<Iterator,DelimiterPredicate> iterator;
      typedef const iterator const_iterator;
      typedef iterator& iterator_ref;
      typedef const_iterator& const_iterator_ref;

      inline tokenizer(const Iterator begin,
                       const Iterator end,
                       const DelimiterPredicate& predicate,
                       const tokenize_options::type tokenize_options = tokenize_options::default_mode)
      : tokenize_options_(tokenize_options),
        predicate_(predicate),
        begin_(begin),
        end_(end),
        begin_itr_(begin_,end_,predicate_,tokenize_options_),
        end_itr_(end_,end_,predicate_,tokenize_options_)
      {}

      inline tokenizer(const std::string& s,
                       const DelimiterPredicate& predicate,
                       const tokenize_options::type tokenize_options = tokenize_options::default_mode)
      : tokenize_options_(tokenize_options),
        predicate_(predicate),
        begin_(to_ptr(s)),
        end_(to_ptr(s) + s.size()),
        begin_itr_(begin_,end_,predicate_,tokenize_options_),
        end_itr_(end_,end_,predicate_,tokenize_options_)
      {}

      inline tokenizer& operator=(const tokenizer& t)
      {
         if (this != &t)
         {
            begin_            = t.begin_;
            end_              = t.end_;
            end_itr_          = t.end_itr_;
            begin_itr_        = t.begin_itr_;
            tokenize_options_ = t.tokenize_options_;
         }

         return (*this);
      }

      inline void assign(const std::string& s) const
      {
         assign(to_ptr(s), to_ptr(s) + s.size());
      }

      inline void assign(const std::string& s)
      {
         assign(to_ptr(s), to_ptr(s) + s.size());
      }

      inline void assign(const Iterator begin, const Iterator end)
      {
         begin_     = begin;
         end_       = end;
         begin_itr_ = iterator(begin_,end_,predicate_,tokenize_options_);
         end_itr_   = iterator(end_  ,end_,predicate_,tokenize_options_);
      }

      inline const_iterator_ref begin() const
      {
         return begin_itr_;
      }

      inline const_iterator_ref end() const
      {
         return end_itr_;
      }

   private:

      tokenize_options::type tokenize_options_;
      const DelimiterPredicate& predicate_;
      Iterator begin_;
      Iterator end_;
      iterator begin_itr_;
      iterator end_itr_;
   };

