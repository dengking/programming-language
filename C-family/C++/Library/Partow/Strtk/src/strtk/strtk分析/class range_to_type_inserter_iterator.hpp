   template <typename Set>
   class range_to_type_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                                void,
                                                                void,
                                                                void,
                                                                void>
   {
   public:

      typedef typename Set::value_type value_type;

      explicit inline range_to_type_inserter_iterator(Set& set)
      : set_(set)
      {}

      range_to_type_inserter_iterator(const range_to_type_inserter_iterator& itr)
      : set_(itr.set_)
      {}

      inline range_to_type_inserter_iterator& operator=(const range_to_type_inserter_iterator& itr)
      {
         if (this != &itr)
         {
            this->set_ = itr.set_;
         }

         return (*this);
      }

      template <typename Iterator>
      inline range_to_type_inserter_iterator& operator=(const std::pair<Iterator,Iterator>& r)
      {
         value_type t;
         if (string_to_type_converter(r.first,r.second,t))
            set_.insert(t);

         return (*this);
      }

      template <typename Iterator>
      inline void operator()(const std::pair<Iterator,Iterator>& r)
      {
         value_type t;
         if (string_to_type_converter(r.first,r.second,t))
            set_.insert(t);
      }

      inline range_to_type_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline range_to_type_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline range_to_type_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Set& set_;
   };

   template <typename Set>
   inline range_to_type_inserter_iterator<Set> range_to_type_inserter(Set& set)
   {
      return (range_to_type_inserter_iterator<Set>(set));
   }
