   template <typename T>
   class counting_back_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                                T,
                                                                void,
                                                                void,
                                                                void>
   {
   public:

      explicit inline counting_back_inserter_iterator(std::size_t& counter)
      : counter_(counter)
      {}

      counting_back_inserter_iterator(const counting_back_inserter_iterator& itr)
      : counter_(itr.counter_)
      {}

      inline counting_back_inserter_iterator& operator=(const counting_back_inserter_iterator& itr)
      {
         if (this != &itr)
         {
            this->counter_ = itr.counter_;
         }

         return (*this);
      }

      inline counting_back_inserter_iterator& operator=(const T&)
      {
         ++counter_;
         return (*this);
      }

      inline void operator()(const T&)
      {
         ++counter_;
      }

      inline counting_back_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline counting_back_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline counting_back_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      std::size_t& counter_;
   };

   template <typename T>
   inline counting_back_inserter_iterator<T> counting_back_inserter(std::size_t& counter_)
   {
      return (counting_back_inserter_iterator<T>(counter_));
   }
