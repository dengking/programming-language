template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          Sequence<T,Allocator>& sequence,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      if (!stream) return 0;

      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;

      while (std::getline(stream,buffer))
      {
         ++line_count;
         sequence.push_back(string_to_type_converter<T>(buffer));
      }

      return line_count;
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          std::set<T,Comparator,Allocator>& set,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      if (!stream) return 0;

      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;

      while (std::getline(stream,buffer))
      {
         ++line_count;
         set.insert(string_to_type_converter<T>(buffer));
      }

      return line_count;
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          std::multiset<T,Comparator,Allocator>& multiset,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      if (!stream) return 0;

      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;

      while (std::getline(stream,buffer))
      {
         ++line_count;
         multiset.insert(string_to_type_converter<T>(buffer));
      }

      return line_count;
   }

   template <typename T, typename Container>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          std::queue<T,Container>& queue,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      if (!stream) return 0;

      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;

      while (std::getline(stream,buffer))
      {
         ++line_count;
         queue.push(string_to_type_converter<T>(buffer));
      }

      return line_count;
   }

   template <typename T, typename Container>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          std::stack<T,Container>& stack,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      if (!stream) return 0;

      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;

      while (std::getline(stream,buffer))
      {
         ++line_count;
         stack.push(string_to_type_converter<T>(buffer));
      }

      return line_count;
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          std::priority_queue<T,Container,Comparator>& priority_queue,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      if (!stream) return 0;

      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;

      while (std::getline(stream,buffer))
      {
         ++line_count;
         priority_queue.push(string_to_type_converter<T>(buffer));
      }

      return line_count;
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          Sequence<T,Allocator>& sequence,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (!stream)
         return 0;
      else
         return load_from_text_file(stream,sequence,buffer_size);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          std::set<T,Comparator,Allocator>& set,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (!stream)
         return 0;
      else
         return load_from_text_file(stream,set,buffer_size);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          std::multiset<T,Comparator,Allocator>& multiset,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (!stream)
         return 0;
      else
         return load_from_text_file(stream,multiset,buffer_size);
   }

   template <typename T, typename Container>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          std::queue<T,Container>& queue,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (!stream)
         return 0;
      else
         return load_from_text_file(stream,queue,buffer_size);
   }

   template <typename T, typename Container>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          std::stack<T,Container>& stack,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (!stream)
         return 0;
      else
         return load_from_text_file(stream,stack,buffer_size);
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          std::priority_queue<T,Container,Comparator>& priority_queue,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (!stream)
         return 0;
      else
         return load_from_text_file(stream,priority_queue,buffer_size);
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t write_to_text_file(std::ostream& stream,
                                         const Sequence<T,Allocator>& sequence,
                                         const std::string& delimiter = "")
   {
      if (!stream) return 0;

      std::size_t count = 0;
      typename Sequence<T,Allocator>::const_iterator itr = sequence.begin();
      typename Sequence<T,Allocator>::const_iterator end = sequence.end();

      if (!delimiter.empty())
      {
         while (end != itr)
         {
            stream << (*itr) << delimiter;
            ++itr;
            ++count;
         }
      }
      else
      {
         while (end != itr)
         {
            stream << (*itr);
            ++itr;
            ++count;
         }
      }

      return count;
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t write_to_text_file(std::ostream& stream,
                                         const std::set<T,Comparator,Allocator>& set,
                                         const std::string& delimiter = "")
   {
      if (!stream) return 0;

      std::size_t count = 0;
      typename std::set<T,Comparator,Allocator>::const_iterator itr = set.begin();
      typename std::set<T,Comparator,Allocator>::const_iterator end = set.end();

      if (!delimiter.empty())
      {
         while (end != itr)
         {
            stream << (*itr) << delimiter;
            ++itr;
            ++count;
         }
      }
      else
      {
         while (end != itr)
         {
            stream << (*itr);
            ++itr;
            ++count;
         }
      }

      return count;
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t write_to_text_file(std::ostream& stream,
                                         const std::multiset<T,Comparator,Allocator>& multiset,
                                         const std::string& delimiter = "")
   {
      if (!stream) return 0;

      std::size_t count = 0;
      typename std::multiset<T,Comparator,Allocator>::const_iterator itr = multiset.begin();
      typename std::multiset<T,Comparator,Allocator>::const_iterator end = multiset.end();

      if (!delimiter.empty())
      {
         while (end != itr)
         {
            stream << (*itr) << delimiter;
            ++itr;
            ++count;
         }
      }
      else
      {
         while (end != itr)
         {
            stream << (*itr);
            ++itr;
            ++count;
         }
      }

      return count;
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t write_to_text_file(const std::string& file_name,
                                         const Sequence<T,Allocator>& sequence,
                                         const std::string& delimiter = "")
   {
      std::ofstream stream(file_name.c_str());
      if (!stream)
         return 0;
      else
         return write_to_text_file(stream,sequence,delimiter);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t write_to_text_file(const std::string& file_name,
                                         const std::set<T,Comparator,Allocator>& set,
                                         const std::string& delimiter = "")
   {
      std::ofstream stream(file_name.c_str());
      if (!stream)
         return 0;
      else
         return write_to_text_file(stream,set,delimiter);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t write_to_text_file(const std::string& file_name,
                                         const std::multiset<T,Comparator,Allocator>& multiset,
                                         const std::string& delimiter = "")
   {
      std::ofstream stream(file_name.c_str());
      if (!stream)
         return 0;
      else
         return write_to_text_file(stream,multiset,delimiter);
   }
