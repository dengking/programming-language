   namespace split_options
   {
      typedef std::size_t type;
      enum
      {
         default_mode            = 0,
         compress_delimiters     = 1,
         include_1st_delimiter   = 2,
         include_all_delimiters  = 4
      };

      static inline bool perform_compress_delimiters(const type& split_opt)
      {
         return compress_delimiters == (split_opt & compress_delimiters);
      }

      static inline bool perform_include_1st_delimiter(const type& split_opt)
      {
         return include_1st_delimiter == (split_opt & include_1st_delimiter);
      }

      static inline bool perform_include_all_delimiters(const type& split_opt)
      {
         return include_all_delimiters == (split_opt & include_all_delimiters);
      }

   } // namespace split_options
