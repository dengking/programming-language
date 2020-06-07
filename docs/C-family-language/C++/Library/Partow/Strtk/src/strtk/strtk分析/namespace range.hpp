   namespace range
   {
      template <typename T>
      class adapter
      {
      public:

         typedef T value_type;
         typedef T* iterator;
         typedef const iterator const_iterator;

         adapter(T* const begin, T* const end)
         : begin_(begin),
           end_(end)
         {}

         adapter(const std::pair<T*,T*>& r)
         : begin_(r.first),
           end_(r.second)
         {}

         adapter(T* const begin, const std::size_t length)
         : begin_(begin),
           end_(begin_ + length)
         {}

         inline iterator begin() const
         {
            return begin_;
         }

         inline iterator end() const
         {
            return end_;
         }

         inline std::size_t size() const
         {
            return std::distance(begin_,end_);
         }

         inline operator std::string() const
         {
            return stringify(begin_,end_);
         }

         inline const T& operator[](const std::size_t& index) const
         {
            return *(begin_ + index);
         }

         inline T& operator[](const std::size_t& index)
         {
            return *(begin_ + index);
         }

      private:

         template <typename Type>
         static inline std::string stringify(Type*,Type*)
         {
            static std::string result = "";
            return result;
         }

         static inline std::string stringify(const char* begin, const char* end)
         {
            return std::string(begin,end);
         }

         iterator begin_;
         iterator end_;
      };

      typedef adapter<const char> string;
      typedef adapter<const unsigned char> ustring;

      template <typename T>
      inline adapter<T> type(const T* begin, const T* end)
      {
         return adapter<T>(begin,end);
      }

      template <typename T, std::size_t N>
      inline adapter<T> type(const T (&t)[N])
      {
         return adapter<T>(t,N);
      }

      static inline adapter<const char> type(const std::string& s)
      {
         return adapter<const char>(to_ptr(s), s.size());
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline adapter<typename Sequence<T,Allocator>::iterator> type(const Sequence<T,Allocator>& seq)
      {
         return adapter<typename Sequence<T,Allocator>::iterator>(seq.begin(),seq.end());
      }

      inline std::string as_string(const adapter<const char>& a)
      {
         return std::string(a.begin(),a.end());
      }

      inline std::string as_string(const adapter<const unsigned char>& a)
      {
         return std::string(a.begin(),a.end());
      }

   } // namespace range
