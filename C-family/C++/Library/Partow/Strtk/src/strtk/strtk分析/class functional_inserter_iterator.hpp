   template <typename Function>
   class functional_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                             void,
                                                             void,
                                                             void,
                                                             void>
   {
   public:

      explicit inline functional_inserter_iterator(Function function)
      : function_(function)
      {}

      functional_inserter_iterator(const functional_inserter_iterator& itr)
      : function_(itr.function_)
      {}

      inline functional_inserter_iterator& operator=(const functional_inserter_iterator& itr)
      {
         if (this != &itr)
         {
            this->function_ = itr.function_;
         }

         return (*this);
      }

      template <typename T>
      inline functional_inserter_iterator& operator=(const T& t)
      {
         function_(t);
         return (*this);
      }

      template <typename T>
      inline void operator()(const T& t)
      {
         function_(t);
      }

      inline functional_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline functional_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline functional_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Function function_;
   };

   template <typename Function>
   inline functional_inserter_iterator<Function> functional_inserter(Function function)
   {
      return (functional_inserter_iterator<Function>(function));
   }

