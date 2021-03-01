   template <typename Container>
   class range_to_type_push_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                                     void,
                                                                     void,
                                                                     void,
                                                                     void>
   {
   public:

      typedef typename Container::value_type value_type;

      explicit inline range_to_type_push_inserter_iterator(Container& container)
      : container_(container)
      {}

      range_to_type_push_inserter_iterator(const range_to_type_push_inserter_iterator& itr)
      : container_(itr.container_)
      {}

      inline range_to_type_push_inserter_iterator& operator=(const range_to_type_push_inserter_iterator& itr)
      {
         if (this != &itr)
         {
            this->container_ = itr.container_;
         }
         return (*this);
      }

      template <typename Iterator>
      inline range_to_type_push_inserter_iterator& operator=(const std::pair<Iterator,Iterator>& r)
      {
         value_type t;

         if (string_to_type_converter(r.first,r.second,t))
            container_.push(t);

         return (*this);
      }

      template <typename Iterator>
      inline void operator()(const std::pair<Iterator,Iterator>& r)
      {
         value_type t;
         if (string_to_type_converter(r.first,r.second,t))
            container_.push(t);
      }

      inline range_to_type_push_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline range_to_type_push_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline range_to_type_push_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Container& container_;
   };

   template <typename Container>
   inline range_to_type_push_inserter_iterator<Container> range_to_type_push_inserter(Container& container)
   {
      return (range_to_type_push_inserter_iterator<Container>(container));
   }

