   template <typename Container>
   class push_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                       void,
                                                       void,
                                                       void,
                                                       void>
   {
   public:

      explicit inline push_inserter_iterator(Container& container)
      : container_(container)
      {}

      inline push_inserter_iterator& operator=(const push_inserter_iterator& itr)
      {
         if (this != &itr)
         {
            this->container_ = itr.container_;
         }

         return (*this);
      }

      inline push_inserter_iterator<Container>& operator=(typename Container::const_reference v)
      {
         container_.push(v);
         return (*this);
      }

      inline push_inserter_iterator<Container>& operator*()
      {
         return (*this);
      }

      inline push_inserter_iterator<Container>& operator++()
      {
         return (*this);
      }

      inline push_inserter_iterator<Container> operator++(int)
      {
         return (*this);
      }

   private:

      Container& container_;
   };

   template <typename Container>
   inline push_inserter_iterator<Container> push_inserter(Container& c)
   {
      return push_inserter_iterator<Container>(c);
   }
