   template <typename Set>
   class inserter_with_valuetype_iterator : public std::iterator<std::output_iterator_tag,
                                                                 typename Set::value_type,
                                                                 void,
                                                                 void,
                                                                 void>
   {
   public:

      explicit inline inserter_with_valuetype_iterator(Set& set)
      : set_(set)
      {}

      inserter_with_valuetype_iterator(const inserter_with_valuetype_iterator& itr)
      : set_(itr.set_)
      {}

      inline inserter_with_valuetype_iterator& operator=(const inserter_with_valuetype_iterator& itr)
      {
         if (this != &itr)
         {
            this->set_ = itr.set_;
         }

         return (*this);
      }

      inline inserter_with_valuetype_iterator& operator=(const typename Set::value_type& v)
      {
         set_.insert(v);
         return (*this);
      }

      inline void operator()(const typename Set::value_type& v)
      {
         set_.insert(v);
      }

      inline inserter_with_valuetype_iterator& operator*()
      {
         return (*this);
      }

      inline inserter_with_valuetype_iterator& operator++()
      {
         return (*this);
      }

      inline inserter_with_valuetype_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Set& set_;
   };

   template <typename Set>
   inline inserter_with_valuetype_iterator<Set> inserter_with_valuetype(Set& set_)
   {
      return (inserter_with_valuetype_iterator<Set>(set_));
   }
