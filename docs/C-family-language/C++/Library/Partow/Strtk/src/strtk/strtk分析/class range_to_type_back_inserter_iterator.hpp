   template <typename Sequence>
   class range_to_type_back_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                                     void,
                                                                     void,
                                                                     void,
                                                                     void>
   {
   public:

      typedef typename Sequence::value_type value_type;

      explicit inline range_to_type_back_inserter_iterator(Sequence& sequence)
      : sequence_(sequence)
      {}

      range_to_type_back_inserter_iterator(const range_to_type_back_inserter_iterator& itr)
      : sequence_(itr.sequence_)
      {}

      inline range_to_type_back_inserter_iterator& operator=(const range_to_type_back_inserter_iterator& itr)
      {
         if (this != &itr)
         {
            this->sequence_ = itr.sequence_;
         }

         return (*this);
      }

      template <typename Iterator>
      inline range_to_type_back_inserter_iterator& operator=(const std::pair<Iterator,Iterator>& r)
      {
         value_type t;

         if (string_to_type_converter(r.first,r.second,t))
            sequence_.push_back(t);

         return (*this);
      }

      inline range_to_type_back_inserter_iterator& operator=(const std::string& s)
      {
         value_type t;

         if (string_to_type_converter(to_ptr(s), to_ptr(s) + s.size(),t))
            sequence_.push_back(t);

         return (*this);
      }

      template <typename Iterator>
      inline void operator()(const std::pair<Iterator,Iterator>& r) const
      {
         value_type t;

         if (string_to_type_converter(r.first,r.second,t))
            sequence_.push_back(t);
      }

      template <typename Iterator>
      inline void operator()(const Iterator begin, const Iterator end)
      {
         sequence_.push_back(string_to_type_converter<value_type>(begin,end));
      }

      inline range_to_type_back_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline range_to_type_back_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline range_to_type_back_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Sequence& sequence_;
   };

   
   template <typename Sequence>
   inline range_to_type_back_inserter_iterator<Sequence> range_to_type_back_inserter(Sequence& sequence)
   {
      return (range_to_type_back_inserter_iterator<Sequence>(sequence));
   }