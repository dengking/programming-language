   inline const char* first_non_repeated_char(const char* begin, const char* end)
   {
      static const std::size_t lut_size = 256;
      unsigned long long int lut[lut_size];

      std::fill_n(lut,lut_size,std::numeric_limits<unsigned long long int>::max());

      static const unsigned long long int not_yet_encountered = std::numeric_limits<unsigned long long int>::max();
      static const unsigned long long int repeated = not_yet_encountered - 1;

      const char* itr = begin;
      unsigned long long int position = 0;

      while (end != itr)
      {
         unsigned long long int& element = lut[static_cast<unsigned int>(*itr)];

         if (not_yet_encountered == element)
         {
            element = position;
         }
         else if (element < repeated)
         {
            element = repeated;
         }
         ++itr;
         ++position;
      }

      position = repeated;

      for (std::size_t i = 0; i < lut_size; ++i)
      {
         if (lut[i] < position)
            position = lut[i];
      }

      return (repeated != position) ? (begin + position) : end;
   }

   inline const unsigned char* first_non_repeated_char(const unsigned char* begin, const unsigned char* end)
   {
      char * b = reinterpret_cast<char*>(const_cast<unsigned char*>(begin));
      char * e = reinterpret_cast<char*>(const_cast<unsigned char*>(end));

      return const_cast<const unsigned char*>(reinterpret_cast<unsigned char*>(const_cast<char*>(first_non_repeated_char(b,e))));
   }

   inline std::size_t first_non_repeated_char(const std::string& str)
   {
      if (str.empty())
         return static_cast<std::size_t>(std::string::npos);

      const char* itr = first_non_repeated_char(to_ptr(str), to_ptr(str) + str.size());

      if ((to_ptr(str) + str.size()) != itr)
         return static_cast<std::size_t>(itr - to_ptr(str));
      else
         return static_cast<std::size_t>(std::string::npos);
   }
