   inline const std::string::value_type* to_ptr(const std::string& s)
   {
      return &s[0];
   }

   inline std::string::value_type* to_ptr(std::string& s)
   {
      return &s[0];
   }