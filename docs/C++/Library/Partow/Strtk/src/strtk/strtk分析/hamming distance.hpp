   inline std::size_t hamming_distance(const unsigned char* begin1, const unsigned char* end1,
                                       const unsigned char* begin2, const unsigned char* end2)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return std::numeric_limits<std::size_t>::max();
      }

      std::size_t distance = 0;
      const unsigned char* itr1 = begin1;
      const unsigned char* itr2 = begin2;

      while (end1 != itr1)
      {
         distance += high_bit_count(static_cast<unsigned char>(((*itr1++) ^ (*itr2++)) & 0xFF));
      }

      return distance;
   }

   inline std::size_t hamming_distance(const char* begin1, const char* end1,
                                       const char* begin2, const char* end2)
   {
      return hamming_distance(reinterpret_cast<const unsigned char*>(begin1),
                              reinterpret_cast<const unsigned char*>(end1),
                              reinterpret_cast<const unsigned char*>(begin2),
                              reinterpret_cast<const unsigned char*>(end2));
   }

   inline std::size_t hamming_distance(const std::string& str1, const std::string& str2)
   {
      return hamming_distance(to_ptr(str1), to_ptr(str1) + str1.size(),
                              to_ptr(str2), to_ptr(str2) + str2.size());
   }

   template <typename Iterator>
   inline std::size_t hamming_distance_elementwise(const Iterator begin1, const Iterator end1,
                                                   const Iterator begin2, const Iterator end2)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return std::numeric_limits<std::size_t>::max();
      }

      std::size_t distance = 0;
      Iterator itr1 = begin1;
      Iterator itr2 = begin2;

      while (end1 != itr1)
      {
         if ((*itr1) != (*itr2))
            ++distance;
      }

      return distance;
   }

   inline std::size_t hamming_distance_elementwise(const std::string& str1, const std::string& str2)
   {
      return hamming_distance_elementwise(to_ptr(str1), to_ptr(str1) + str1.size(),
                                          to_ptr(str2), to_ptr(str2) + str2.size());
   }