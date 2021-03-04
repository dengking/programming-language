namespace strtk
{

   static const std::size_t one_kilobyte = 1024;
   static const std::size_t one_megabyte = 1024 * one_kilobyte;
   static const std::size_t one_gigabyte = 1024 * one_megabyte;
   static const std::size_t magic_seed   = 0xA5A5A5A5;
   //Function是函数对象，对buffer进行迭代
   template <typename Tokenizer, typename Function>
   inline std::size_t for_each_token(const std::string& buffer,
                                     Tokenizer& tokenizer,
                                     Function function)
   {
      std::size_t token_count = 0;
      //assgin进行赋值
      tokenizer.assign(buffer);
      //指针也是一种迭代器
      typename Tokenizer::iterator itr = tokenizer.begin();
      typename Tokenizer::const_iterator end = tokenizer.end();

      while (end != itr)
      {
         function(*itr);
         ++itr;
         ++token_count;
      }

      return token_count;
   }
   //对文件进行迭代
   template <typename Function>
   inline std::size_t for_each_line(std::istream& stream,
                                    Function function,
                                    const std::size_t& buffer_size = one_kilobyte)
   {
      std::string buffer;
      //reserve函数的功能？？
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;

      while (std::getline(stream,buffer))
      {
         function(buffer);
         ++line_count;
      }

      return line_count;
   }
   //对文件的前N行进行迭代
   template <typename Function>
   inline std::size_t for_each_line_n(std::istream& stream,
                                      const std::size_t& n,
                                      Function function,
                                      const std::size_t& buffer_size = one_kilobyte)
   {
      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;

      while (std::getline(stream,buffer))
      {
         function(buffer);
         if (n == ++line_count)
            break;
      }

      return line_count;
   }

   template <typename Function>
   inline std::size_t for_each_line(const std::string& file_name,
                                    Function function,
                                    const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line(stream,function,buffer_size);
      else
         return 0;
   }

   template <typename Function>
   inline std::size_t for_each_line_n(const std::string& file_name,
                                      const std::size_t& n,
                                      Function function,
                                      const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line_n(stream,n,function,buffer_size);
      else
         return 0;
   }

   template <typename Function>
   inline std::size_t for_each_line_conditional(std::istream& stream,
                                                Function function,
                                                const std::size_t& buffer_size = one_kilobyte)
   {
      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;

      while (std::getline(stream,buffer))
      {
         if (!function(buffer))
         {
            return line_count;
         }
         ++line_count;
      }

      return line_count;
   }

   template <typename Function>
   inline std::size_t for_each_line_n_conditional(std::istream& stream,
                                                  const std::size_t& n,
                                                  Function function,
                                                  const std::size_t& buffer_size = one_kilobyte)
   {
      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;
      while (std::getline(stream,buffer))
      {
         if (!function(buffer))
         {
            return line_count;
         }
         if (n == ++line_count)
            break;
      }
      return line_count;
   }

   template <typename Function>
   inline std::size_t for_each_line_conditional(const std::string& file_name,
                                                Function function,
                                                const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line_conditional(stream,function,buffer_size);
      else
         return 0;
   }

   template <typename Function>
   inline std::size_t for_each_line_n_conditional(const std::string& file_name,
                                                  const std::size_t& n,
                                                  Function function,
                                                  const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());

      if (stream)
         return for_each_line_n_conditional(stream,n,function,buffer_size);
      else
         return 0;
   }
}