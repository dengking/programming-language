   inline std::size_t index_of(const std::string& pattern, const std::string& data)
   {
      if (pattern.empty())
         return std::string::npos;
      else if (data.empty())
         return std::string::npos;
      else if (pattern.size() > data.size())
         return std::string::npos;

      const char* itr = std::search(to_ptr(data)   , to_ptr(data)    + data   .size(),
                                    to_ptr(pattern), to_ptr(pattern) + pattern.size());

      if ((to_ptr(data) + data.size()) == itr)
         return std::string::npos;
      else
         return std::distance(to_ptr(data),itr);
   }

