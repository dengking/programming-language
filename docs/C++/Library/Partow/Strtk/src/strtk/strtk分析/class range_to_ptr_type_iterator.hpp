   template <typename T>
   class range_to_ptr_type_iterator : public std::iterator<std::output_iterator_tag,
                                                           void,
                                                           void,
                                                           void,
                                                           void>
   {
   public:

      typedef T value_type;

      explicit inline range_to_ptr_type_iterator(T* pointer, std::size_t& insert_count)
      : pointer_(pointer),
        insert_count_(insert_count)
      {}

      range_to_ptr_type_iterator(const range_to_ptr_type_iterator& itr)
      : pointer_(itr.pointer_)
      {}

      inline range_to_ptr_type_iterator& operator=(const range_to_ptr_type_iterator& itr)
      {
         if (this != &itr)
         {
            this->pointer_ = itr.pointer_;
         }

         return (*this);
      }

      template <typename Iterator>
      inline range_to_ptr_type_iterator& operator=(const std::pair<Iterator,Iterator>& r)
      {
         value_type t = value_type();

         if (string_to_type_converter(r.first,r.second,t))
         {
            (*pointer_) = t;
            ++pointer_;
            ++insert_count_;
         }

         return (*this);
      }

      inline range_to_ptr_type_iterator& operator=(const std::string& s)
      {
         value_type t = value_type();

         if (string_to_type_converter(to_ptr(s), to_ptr(s) + s.size(),t))
         {
            (*pointer_) = t;
            ++pointer_;
            ++insert_count_;
         }

         return (*this);
      }

      template <typename Iterator>
      inline void operator()(const std::pair<Iterator,Iterator>& r) const
      {
         value_type t;
         if (string_to_type_converter(r.first,r.second,t))
         {
            (*pointer_) = t;
            ++pointer_;
            ++insert_count_;
         }
      }

      template <typename Iterator>
      inline void operator()(const Iterator begin, const Iterator end)
      {
         (*pointer_) = string_to_type_converter<T>(begin,end);
         ++pointer_;
         ++insert_count_;
      }

      inline range_to_ptr_type_iterator& operator*()
      {
         return (*this);
      }

      inline range_to_ptr_type_iterator& operator++()
      {
         return (*this);
      }

      inline range_to_ptr_type_iterator operator++(int)
      {
         return (*this);
      }

   private:

      T* pointer_;
      std::size_t& insert_count_;
   };

   template <typename T>
   inline range_to_ptr_type_iterator<T> range_to_ptr_type(T* pointer, std::size_t& insert_count)
   {
      return (range_to_ptr_type_iterator<T>(pointer,insert_count));
   }

   template <typename T>
   inline range_to_ptr_type_iterator<T> range_to_ptr_type(T* pointer)
   {
      static std::size_t insert_count = 0;
      return (range_to_ptr_type_iterator<T>(pointer,insert_count));
   }
