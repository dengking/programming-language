   template <typename Sequence>
   class back_inserter_with_valuetype_iterator : public std::iterator<std::output_iterator_tag,
                                                                      typename Sequence::value_type,
                                                                      void,
                                                                      void,
                                                                      void>
   {
   public:

      explicit inline back_inserter_with_valuetype_iterator(Sequence& sequence)
      : sequence_(sequence)
      {}

      back_inserter_with_valuetype_iterator(const back_inserter_with_valuetype_iterator& itr)
      : sequence_(itr.sequence_)
      {}

      inline back_inserter_with_valuetype_iterator& operator=(const back_inserter_with_valuetype_iterator& itr)
      {
         if (this != &itr)
         {
            this->sequence_ = itr.sequence_;
         }
         return (*this);
      }

      inline back_inserter_with_valuetype_iterator& operator=(const typename Sequence::value_type& v)
      {
         sequence_.push_back(v);
         return (*this);
      }

      inline void operator()(const typename Sequence::value_type& v)
      {
         sequence_.push_back(v);
      }

      inline back_inserter_with_valuetype_iterator& operator*()
      {
         return (*this);
      }

      inline back_inserter_with_valuetype_iterator& operator++()
      {
         return (*this);
      }

      inline back_inserter_with_valuetype_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Sequence& sequence_;
   };

   template <typename Sequence>
   inline back_inserter_with_valuetype_iterator<Sequence> back_inserter_with_valuetype(Sequence& sequence_)
   {
      return (back_inserter_with_valuetype_iterator<Sequence>(sequence_));
   }
