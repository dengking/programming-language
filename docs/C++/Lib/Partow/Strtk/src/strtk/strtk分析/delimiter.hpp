   template <typename T>
   struct single_delimiter_predicate
   {
   public:

      typedef T value_type;

      single_delimiter_predicate(const T& d)
      : delimiter_(d)
      {}

      inline bool operator()(const T& d) const
      {
         return delimiter_ == d;
      }

   private:

      single_delimiter_predicate<T>& operator=(const single_delimiter_predicate<T>&);
      const T delimiter_;
   };

   template <typename T>
   struct multiple_delimiter_predicate
   {
   public:

      typedef T value_type;

      multiple_delimiter_predicate(const T* d_begin, const T* d_end)
      : length_(std::distance(d_begin,d_end)),
        delimiter_((length_ <= sbo_buffer_size) ? sbo_buffer : new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         std::copy(d_begin,d_end, delimiter_);
      }

      multiple_delimiter_predicate(const T d[], const std::size_t& length)
      : length_(length),
        delimiter_((length_ <= sbo_buffer_size) ? sbo_buffer : new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         std::copy(d,d + length, delimiter_);
      }

      template <typename Iterator>
      multiple_delimiter_predicate(const Iterator begin, const Iterator end)
      : length_(std::distance(begin,end)),
        delimiter_((length_ <= sbo_buffer_size) ? sbo_buffer : new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         //static_assert(T == std::iterator_traits<Iterator>::value_type);
         std::copy(begin,end, delimiter_);
      }

      template <typename Type>
      multiple_delimiter_predicate(const range::adapter<Type>& r)
      : length_(std::distance(r.begin(),r.end())),
        delimiter_((length_ <= sbo_buffer_size) ? sbo_buffer : new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         //static_assert(T == std::iterator_traits<Iterator>::value_type);
         std::copy(r.begin(),r.end(), delimiter_);
      }

     ~multiple_delimiter_predicate()
      {
         if (length_ > sbo_buffer_size)
         {
            delete[] delimiter_;
         }
      }

      inline bool operator()(const T& d) const
      {
         return (std::find(delimiter_,delimiter_end_,d) != delimiter_end_);
      }

   private:

      multiple_delimiter_predicate(const multiple_delimiter_predicate<T>& mdp);
      multiple_delimiter_predicate& operator=(const multiple_delimiter_predicate<T>& mdp);

      std::size_t length_;
      T* delimiter_;
      T* delimiter_end_;
      enum { sbo_buffer_size = 32 };
      T sbo_buffer[sbo_buffer_size];
   };

   struct multiple_char_delimiter_predicate
   {
   public:

      template <typename Iterator>
      multiple_char_delimiter_predicate(const Iterator begin, const Iterator end)
      {
         setup_delimiter_table(begin,end);
      }

      multiple_char_delimiter_predicate(const std::string& s)
      {
         setup_delimiter_table(to_ptr(s), to_ptr(s) + s.size());
      }

      inline bool operator()(const unsigned char& c) const
      {
         return (delimiter_table_[c]);
      }

      inline bool operator()(const char& c) const
      {
         return operator()(static_cast<unsigned char>(c));
      }

   private:

      static const std::size_t table_size = 256;

      template <typename Iterator>
      inline void setup_delimiter_table(const Iterator begin, const Iterator end)
      {
         std::fill_n(delimiter_table_,table_size,false);
         for (Iterator itr = begin; itr != end; ++itr)
         {
            delimiter_table_[static_cast<unsigned char>(*itr)] = true;
         }
      }

      bool delimiter_table_[table_size];
   };
